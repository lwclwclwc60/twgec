#include "ast.h"
#include "transform.h"

namespace transform {
namespace {

void substituteMutableVars(std::unique_ptr<ExpressionNode> &expNode,
                           const ExpressionMap &mutableVarMap) {
  if (!expNode)
    return;

  if (expNode->kind == EXPRESSION_KIND_VALUE) {
    if (auto *varNode =
            dynamic_cast<VariableValueNode *>(expNode->value.get())) {
      if (mutableVarMap.count(varNode->value) != 0)
        expNode = mutableVarMap.at(varNode->value)->clone();
      return;
    }
    if (auto *listNode = dynamic_cast<ListValueNode *>(expNode->value.get())) {
      for (auto &item : listNode->items)
        substituteMutableVars(item, mutableVarMap);
      return;
    }
    if (auto *pointNode =
            dynamic_cast<PointValueNode *>(expNode->value.get())) {
      substituteMutableVars(pointNode->x, mutableVarMap);
      substituteMutableVars(pointNode->y, mutableVarMap);
      return;
    }
    if (auto *actorMatchNode =
            dynamic_cast<ActorMatchValueNode *>(expNode->value.get())) {
      for (auto &arg : actorMatchNode->paramApps->named_args)
        substituteMutableVars(arg->expNode, mutableVarMap);
      for (auto &arg : actorMatchNode->paramApps->positional_args)
        substituteMutableVars(arg->expNode, mutableVarMap);
      return;
    }
    if (auto *buttonNode =
            dynamic_cast<ButtonValueNode *>(expNode->value.get())) {
      for (auto &arg : buttonNode->paramApps->named_args)
        substituteMutableVars(arg->expNode, mutableVarMap);
      for (auto &arg : buttonNode->paramApps->positional_args)
        substituteMutableVars(arg->expNode, mutableVarMap);
      return;
    }
    if (auto *customWeaponNode =
            dynamic_cast<CustomWeaponValueNode *>(expNode->value.get())) {
      for (auto &arg : customWeaponNode->paramApps->named_args)
        substituteMutableVars(arg->expNode, mutableVarMap);
      for (auto &arg : customWeaponNode->paramApps->positional_args)
        substituteMutableVars(arg->expNode, mutableVarMap);
      return;
    }
    return;
  }

  // non EXPRESSION_KIND_VALUE
  for (auto &arg : expNode->args)
    substituteMutableVars(arg, mutableVarMap);
}

void substituteMutableVars(std::unique_ptr<InstructionNode> &instruction,
                           const ExpressionMap &mutableVarMap) {
  if (!instruction || !instruction->paramApps)
    return;
  for (auto &arg : instruction->paramApps->named_args)
    substituteMutableVars(arg->expNode, mutableVarMap);
  for (auto &arg : instruction->paramApps->positional_args)
    substituteMutableVars(arg->expNode, mutableVarMap);
}

bool expandMutableVars(std::unique_ptr<InstrSetNode> &instrSet,
                       ExpressionMap &mutableVarMap) {
  std::vector<std::unique_ptr<CompositeInstrNode>> expandedInstructions;

  for (auto &compositeInstr : instrSet->instructions) {
    if (compositeInstr->mutableVarDef) {
      auto &varDef = compositeInstr->mutableVarDef;
      substituteMutableVars(varDef->expNode, mutableVarMap);
      mutableVarMap[varDef->identifier] = varDef->expNode->clone();
      continue;
    }

    if (compositeInstr->instruction) {
      substituteMutableVars(compositeInstr->instruction, mutableVarMap);
      expandedInstructions.push_back(std::move(compositeInstr));
      continue;
    }

    if (compositeInstr->branchNode) {
      auto &branchNode = compositeInstr->branchNode;
      for (auto &ifRegion : branchNode->ifRegions) {
        ExpressionMap branchMap;
        for (const auto &it : mutableVarMap)
          branchMap.insert({it.first, it.second->clone()});
        substituteMutableVars(ifRegion->condition, branchMap);
        if (!expandMutableVars(ifRegion->region, branchMap))
          return false;
      }

      if (branchNode->elseRegion) {
        ExpressionMap elseMap;
        for (const auto &it : mutableVarMap)
          elseMap.insert({it.first, it.second->clone()});
        if (!expandMutableVars(branchNode->elseRegion, elseMap))
          return false;
      }

      expandedInstructions.push_back(std::move(compositeInstr));
      continue;
    }

    if (compositeInstr->forNode) {
      auto &forNode = compositeInstr->forNode;
      if (forNode->fromExp && forNode->toExp) {
        substituteMutableVars(forNode->fromExp, mutableVarMap);
        substituteMutableVars(forNode->toExp, mutableVarMap);
      } else if (forNode->listExp) {
        substituteMutableVars(forNode->listExp, mutableVarMap);
      }

      ExpressionMap loopMap;
      for (const auto &it : mutableVarMap)
        loopMap.insert({it.first, it.second->clone()});
      loopMap.erase(forNode->iterArg);
      if (!expandMutableVars(forNode->region, loopMap))
        return false;

      expandedInstructions.push_back(std::move(compositeInstr));
      continue;
    }

    expandedInstructions.push_back(std::move(compositeInstr));
  }

  instrSet->instructions = std::move(expandedInstructions);
  return true;
}

} // namespace

bool mutableVarExpansion(const std::unique_ptr<ModuleNode> &moduleNode,
                         PassConfig config) {
  for (auto &funDef : moduleNode->funDefs) {
    if (funDef->typedInstrSet) {
      ExpressionMap mutableVarMap;
      if (!expandMutableVars(funDef->typedInstrSet->instrSet, mutableVarMap))
        return false;
    }
    if (funDef->blockBody)
      for (auto &typedInstrSet : funDef->blockBody->typedInstrSets) {
        ExpressionMap mutableVarMap;
        if (!expandMutableVars(typedInstrSet->instrSet, mutableVarMap))
          return false;
      }
  }

  for (auto &block : moduleNode->blocks)
    if (block->blockBody)
      for (auto &typedInstrSet : block->blockBody->typedInstrSets) {
        ExpressionMap mutableVarMap;
        if (!expandMutableVars(typedInstrSet->instrSet, mutableVarMap))
          return false;
      }

  return true;
}
} // namespace transform
