#include "ast.h"
#include "transform.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>
namespace transform {

using std::map;
using std::set;
using std::string;
using std::unique_ptr;
using std::vector;

bool isValidFuncCaller(std::unique_ptr<InstructionNode> &caller,
                       map<string, unique_ptr<FunDefNode>> &funDefMap,
                       FunDefType expectedType) {
  FunDefNode *funDef = funDefMap[caller->identifier].get();
  if (funDef->typedInstrSet->type != expectedType) {
    std::cerr << "Syntax Error: Unmatched function type. "
              << funDef->typedInstrSet->type
              << "-typed function cannot be called inside the " << expectedType
              << " scope. Found at " << caller->loc << ".\n";
    return false;
  }
  set<string> paramSet;
  set<string> argSet;
  std::map<string, unique_ptr<ExpressionNode>> namedArgsMap;
  for (string param : funDef->params)
    paramSet.insert(param);
  for (auto &namedArg : caller->paramApps->named_args) {
    namedArgsMap.insert({namedArg->key, namedArg->expNode->clone()});
    argSet.insert(namedArg->key);
  }
  if (paramSet != argSet) {
    std::cerr << "Syntax Error: Unmatched paramter naming at " << funDef->loc
              << "(function definition) and " << caller->loc
              << "(function application).\n";
    return false;
  }

  return true;
}

bool functionInling(
    std::map<std::string, std::unique_ptr<FunDefNode>> &funDefMap,
    std::unique_ptr<InstrSetNode> &instrSet, FunDefType expectedType) {
  std::vector<int> funCallerIdxes;
  auto &compositeInstrs = instrSet->instructions;
  // Step 1: Get the function caller index
  for (int idx = 0; idx < compositeInstrs.size(); idx++) {
    if (auto &primitiveInstr = compositeInstrs[idx]->instruction) {
      if (funDefMap.count(primitiveInstr->identifier) == 0)
        continue;
      if (!isValidFuncCaller(primitiveInstr, funDefMap, expectedType))
        return false;
      funCallerIdxes.push_back(idx);
    } else if (auto &branchNode = compositeInstrs[idx]->branchNode) {
      for (auto &ifRegion : branchNode->ifRegions)
        if (!functionInling(funDefMap, ifRegion->region, expectedType))
          return false;
      if (branchNode->elseRegion)
        if (!functionInling(funDefMap, branchNode->elseRegion, expectedType))
          return false;
    } else if (auto &forNode = compositeInstrs[idx]->forNode) {
      if (!functionInling(funDefMap, forNode->region, expectedType))
        return false;
    }
  }
  std::reverse(funCallerIdxes.begin(), funCallerIdxes.end());
  // Step 2: Inline the function
  for (int idx : funCallerIdxes) {
    std::map<std::string, unique_ptr<ExpressionNode>> callerParamMap;
    auto &callerInstr = compositeInstrs[idx]->instruction;
    auto pos = compositeInstrs.begin() + idx;
    auto funDefNode = funDefMap[callerInstr->identifier]->clone();
    for (auto &arg : callerInstr->paramApps->named_args) {
      arg->expNode->appendCallFrame(callerInstr->identifier, callerInstr->loc);
      callerParamMap.insert({arg->key, std::move(arg->expNode)});
    }
    compositeInstrs.erase(pos);
    auto clonedFunRootInstrSet =
        funDefNode.get()->typedInstrSet.get()->instrSet->clone();
    clonedFunRootInstrSet->propagateExp(callerParamMap);
    compositeInstrs.insert(
        pos,
        std::make_move_iterator(clonedFunRootInstrSet->instructions.begin()),
        std::make_move_iterator(clonedFunRootInstrSet->instructions.end()));
  }
  return true;
}

bool functionInling(const unique_ptr<ModuleNode> &moduleNode,
                    PassConfig config) {
  std::map<std::string, std::unique_ptr<FunDefNode>> funDefsMap;
  bool ret = true;
  for (auto &funDef : moduleNode->funDefs) {
    if (funDef->blockBody)
      for (auto &typedInstrSet : funDef->blockBody->typedInstrSets)
        ret &= functionInling(funDefsMap, typedInstrSet->instrSet,
                              typedInstrSet->type);
    else if (funDef->typedInstrSet)
      ret &= functionInling(funDefsMap, funDef->typedInstrSet->instrSet,
                            funDef->typedInstrSet->type);
    funDefsMap.insert({funDef->identifier, funDef->clone()});
  }
  if (!ret)
    return ret;
  for (auto &blockNode : moduleNode->blocks)
    for (auto &typedInstrSet : blockNode->blockBody->typedInstrSets)
      ret &= functionInling(funDefsMap, typedInstrSet->instrSet,
                            typedInstrSet->type);
  moduleNode->funDefs.clear();
  return ret;
}
} // namespace transform