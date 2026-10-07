#include "ast.h"
#include "transform.h"
#include <map>
#include <memory>
#include <set>
namespace transform {

using std::string;
using std::unique_ptr;

bool argBinding(std::map<std::string, std::unique_ptr<FunDefNode>> &funDefs,
                std::unique_ptr<InstructionNode> &instr) {
  if (funDefs.count(instr->identifier) == 0)
    return true;
  auto &funDef = funDefs.find(instr->identifier)->second;
  auto &funParam = funDef->params;
  auto &defaultParamValues = funDef->defaultParamValues;
  if (instr->paramApps->positional_args.size() > funParam.size()) {
    std::cerr << "Syntax Error: Too much positional arguments. Found at "
              << instr->loc << "\n";
    return false;
  }

  std::map<string, std::unique_ptr<NamedParamAppsNode>> bindedArgsMap;
  std::vector<string> bindedArgOrder;

  // Handle positional args
  for (size_t i = 0; i < instr->paramApps->positional_args.size(); i++) {
    auto &posArg = instr->paramApps->positional_args[i];
    const auto &paramName = funParam[i];
    bindedArgOrder.push_back(paramName);
    bindedArgsMap.insert(
        {paramName, std::make_unique<NamedParamAppsNode>(
                        paramName, posArg.get()->expNode, posArg->loc)});
  }

  std::set<string> paramSet;
  for (const auto &paramName : funParam)
    paramSet.insert(paramName);

  // Handle named args
  for (auto &namedArg : instr->paramApps->named_args) {
    const auto &argKey = namedArg->key;
    if (paramSet.count(argKey) == 0) {
      std::cerr << "Syntax Error: Unmatched paramter naming at " << funDef->loc
                << "(function definition) and " << instr->loc
                << "(function application).\n";
      return false;
    }
    if (bindedArgsMap.count(argKey) != 0) {
      std::cerr << "Syntax Error: Duplicated argument `" << argKey
                << "` found at " << namedArg->loc << "\n";
      return false;
    }
    bindedArgOrder.push_back(argKey);
    bindedArgsMap.insert({
        argKey,
        std::make_unique<NamedParamAppsNode>(argKey, namedArg->expNode,
                                             namedArg->loc),
    });
  }

  // Handle default args
  for (size_t i = 0; i < funParam.size(); i++) {
    const auto &paramName = funParam[i];
    if (bindedArgsMap.count(paramName) != 0)
      continue;
    if (i < defaultParamValues.size() && defaultParamValues[i]) {
      auto defaultExp = defaultParamValues[i]->clone();
      bindedArgOrder.push_back(paramName);
      bindedArgsMap.insert(
          {paramName, std::make_unique<NamedParamAppsNode>(
                          paramName, defaultExp, defaultExp->loc)});
    }
  }

  instr->paramApps->argNamesInOrder.clear();
  instr->paramApps->named_args.clear();
  instr->paramApps->positional_args.clear();
  for (const auto &argKey : bindedArgOrder)
    instr->paramApps->addNamedArg(std::move(bindedArgsMap[argKey]));
  instr->paramApps->refreshTrace();

  std::set<string> bindedArgSet;
  for (const auto &namedArg : instr->paramApps->named_args)
    bindedArgSet.insert(namedArg->key);
  if (bindedArgSet.size() != funParam.size()) {
    std::cerr << "Syntax Error: Unmatched number of arguments of function `"
              << instr->identifier << "`. Expected: " << funParam.size()
              << " arguments defined at " << funDef->loc << ". Found "
              << bindedArgSet.size() << " arguments passed at " << instr->loc
              << ".\n";
    return false;
  }
  return true;
}

bool argBinding(std::map<std::string, std::unique_ptr<FunDefNode>> &funDefs,
                std::unique_ptr<InstrSetNode> &instrSet) {
  for (auto &compositeInstr : instrSet->instructions) {
    if (compositeInstr->instruction) {
      if (!argBinding(funDefs, compositeInstr->instruction))
        return false;
    } else if (auto &branchNode = compositeInstr->branchNode) {
      for (auto &ifRegion : branchNode->ifRegions)
        if (!argBinding(funDefs, ifRegion->region))
          return false;
      if (branchNode->elseRegion)
        if (!argBinding(funDefs, branchNode->elseRegion))
          return false;
    } else if (auto &forNode = compositeInstr->forNode) {
      if (!argBinding(funDefs, forNode->region))
        return false;
    }
  }
  return true;
}

bool argBinding(const unique_ptr<ModuleNode> &moduleNode, PassConfig config) {
  std::map<std::string, std::unique_ptr<FunDefNode>> funDefsMap;
  bool ret = true;
  // initalize func param
  for (auto &funDef : moduleNode->funDefs)
    funDefsMap.insert({funDef->identifier, funDef->clone()});
  // FunDef arg binding
  for (auto &funDef : moduleNode->funDefs)
    if (funDef->blockBody)
      for (auto &typedInstrSet : funDef->blockBody->typedInstrSets)
        ret &= argBinding(funDefsMap, typedInstrSet->instrSet);
    else if (funDef->typedInstrSet)
      ret &= argBinding(funDefsMap, funDef->typedInstrSet->instrSet);
  // blockNode arg binding
  for (auto &blockNode : moduleNode->blocks)
    if (blockNode->blockBody)
      for (auto &typedInstrSet : blockNode->blockBody->typedInstrSets)
        ret &= argBinding(funDefsMap, typedInstrSet->instrSet);
    else if (blockNode->blockConstructor)
      ret &= argBinding(funDefsMap, blockNode->blockConstructor);
  return ret;
}
} // namespace transform