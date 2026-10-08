#include "ast.h"
#include "location.h"
#include "transform.h"
#include <algorithm>
#include <iterator>
#include <memory>
#include <set>
namespace transform {

using std::string;
using std::unique_ptr;

bool throwRedefinitionError(std::string name, Location loc1, Location loc2) {
  std::cerr << "Compilation Error: Redefinition of `" << name << "` found at "
            << loc1 << " and " << loc2 << "\n";
  return false;
}

bool funParamRedefinitionChecking(const unique_ptr<ModuleNode> &moduleNode) {
  bool ret = true;
  for (const auto &funDef : moduleNode->funDefs) {
    std::set<std::string> paramSet;
    for (const auto &param : funDef->params) {
      if (paramSet.count(param) != 0) {
        std::cerr << "Compilation Error: Duplicated parameter `" << param
                  << "` in function `" << funDef->identifier << "` at "
                  << funDef->loc << "\n";
        ret = false;
      }
      paramSet.insert(param);
    }
  }
  return ret;
}

bool redefinitionChecking(const unique_ptr<ModuleNode> &moduleNode) {
  std::map<std::string, std::unique_ptr<ConstDefNode>> constDefMap;
  std::map<std::string, std::unique_ptr<FunDefNode>> funDefMap;
  std::map<std::string, std::unique_ptr<BlockNode>> blockMap;
  for (auto &constDef : moduleNode->constDefs) {
    if (constDefMap.count(constDef->key) != 0)
      return throwRedefinitionError(
          constDef->key, constDefMap.at(constDef->key)->loc, constDef->loc);
    constDefMap.insert({constDef->key, constDef->clone()});
  }
  for (auto &funDef : moduleNode->funDefs) {
    if (constDefMap.count(funDef->identifier) != 0)
      return throwRedefinitionError(funDef->identifier,
                                    constDefMap.at(funDef->identifier)->loc,
                                    funDef->loc);
    if (funDefMap.count(funDef->identifier) != 0)
      return throwRedefinitionError(funDef->identifier,
                                    funDefMap.at(funDef->identifier)->loc,
                                    funDef->loc);
    funDefMap.insert({funDef->identifier, funDef->clone()});
  }
  for (auto &block : moduleNode->blocks) {
    if (constDefMap.count(block->identifier) != 0)
      return throwRedefinitionError(block->identifier,
                                    constDefMap.at(block->identifier)->loc,
                                    block->loc);
    if (funDefMap.count(block->identifier) != 0)
      return throwRedefinitionError(
          block->identifier, funDefMap.at(block->identifier)->loc, block->loc);
    if (blockMap.count(block->identifier) != 0)
      return throwRedefinitionError(
          block->identifier, blockMap.at(block->identifier)->loc, block->loc);
    blockMap.insert({block->identifier, block->clone()});
  }
  return true;
}

bool constMutableRedefinitionChecking(
    std::unique_ptr<InstrSetNode> &instrSet,
    const std::map<std::string, Location> &constDefLocMap) {
  for (auto &compositeInstr : instrSet->instructions) {
    if (compositeInstr->mutableVarDef) {
      const auto &varName = compositeInstr->mutableVarDef->identifier;
      if (constDefLocMap.count(varName) != 0)
        return throwRedefinitionError(varName, constDefLocMap.at(varName),
                                      compositeInstr->mutableVarDef->loc);
      continue;
    }

    if (compositeInstr->branchNode) {
      auto &branchNode = compositeInstr->branchNode;
      for (auto &ifRegion : branchNode->ifRegions)
        if (!constMutableRedefinitionChecking(ifRegion->region, constDefLocMap))
          return false;
      if (branchNode->elseRegion)
        if (!constMutableRedefinitionChecking(branchNode->elseRegion,
                                              constDefLocMap))
          return false;
      continue;
    }

    if (compositeInstr->forNode)
      if (!constMutableRedefinitionChecking(compositeInstr->forNode->region,
                                            constDefLocMap))
        return false;
  }
  return true;
}

bool constMutableRedefinitionChecking(
    const unique_ptr<ModuleNode> &moduleNode) {
  std::map<std::string, Location> constDefLocMap;
  for (const auto &constDef : moduleNode->constDefs)
    constDefLocMap.insert({constDef->key, constDef->loc});

  for (auto &funDef : moduleNode->funDefs) {
    if (funDef->typedInstrSet)
      if (!constMutableRedefinitionChecking(funDef->typedInstrSet->instrSet,
                                            constDefLocMap))
        return false;
    if (funDef->blockBody)
      for (auto &typedInstrSet : funDef->blockBody->typedInstrSets)
        if (!constMutableRedefinitionChecking(typedInstrSet->instrSet,
                                              constDefLocMap))
          return false;
  }

  for (auto &block : moduleNode->blocks)
    if (block->blockBody)
      for (auto &typedInstrSet : block->blockBody->typedInstrSets)
        if (!constMutableRedefinitionChecking(typedInstrSet->instrSet,
                                              constDefLocMap))
          return false;

  return true;
}

bool hasUnitializedFun(std::unique_ptr<InstrSetNode> &instrSet,
                       std::set<std::string> uninitializedFunDef) {
  bool ret = false;
  for (auto &compositeInstr : instrSet->instructions) {
    if (!compositeInstr->instruction)
      continue;
    if (uninitializedFunDef.count(compositeInstr->instruction->identifier)) {
      std::cerr << "Compilation Error: function `"
                << compositeInstr->instruction->identifier
                << "` is defined before used at "
                << compositeInstr->instruction->loc << ".\n";
      ret = true;
    } else if (auto &branchNode = compositeInstr->branchNode) {
      for (auto &ifRegion : branchNode->ifRegions)
        if (hasUnitializedFun(ifRegion->region, uninitializedFunDef))
          ret = true;
      if (branchNode->elseRegion)
        if (hasUnitializedFun(branchNode->elseRegion, uninitializedFunDef))
          ret = true;
    } else if (auto &forNode = compositeInstr->forNode) {
      if (hasUnitializedFun(forNode->region, uninitializedFunDef))
        ret = true;
    }
  }
  return ret;
}

bool useBeforeDefineChecking(const unique_ptr<ModuleNode> &moduleNode) {
  bool ret = true;
  std::set<std::string> allFunDef;
  for (auto &funDef : moduleNode->funDefs)
    allFunDef.insert(funDef->identifier);
  std::set<std::string> definedFunDef;
  for (auto &funDef : moduleNode->funDefs) {
    if (funDef->inheritanceCall) {
      const auto &baseIdentifier = funDef->inheritanceCall->identifier;
      if (definedFunDef.count(baseIdentifier) == 0) {
        std::cerr << "Compilation Error: Function `" << baseIdentifier
                  << "` must be defined before inherited by `"
                  << funDef->identifier << "` at " << funDef->loc << ".\n";
        ret = false;
      }
    }

    std::set<std::string> uninitializedFunDef;
    std::set_difference(
        allFunDef.begin(), allFunDef.end(), definedFunDef.begin(),
        definedFunDef.end(),
        std::inserter(uninitializedFunDef, uninitializedFunDef.begin()));
    if (funDef->blockBody)
      for (auto &typedInstrSet : funDef->blockBody->typedInstrSets)
        if (hasUnitializedFun(typedInstrSet->instrSet, uninitializedFunDef))
          ret = false;
    if (funDef->typedInstrSet)
      if (hasUnitializedFun(funDef->typedInstrSet->instrSet,
                            uninitializedFunDef))
        ret = false;
    definedFunDef.insert(funDef->identifier);
  }
  return ret;
}

bool symbolChecking(const unique_ptr<ModuleNode> &moduleNode,
                    PassConfig config) {
  return redefinitionChecking(moduleNode) &&
         constMutableRedefinitionChecking(moduleNode) &&
         funParamRedefinitionChecking(moduleNode) &&
         useBeforeDefineChecking(moduleNode);
}
} // namespace transform