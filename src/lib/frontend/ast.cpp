#include "frontend/ast.h"
#include "utils/utils.h"
#include <iostream>
#include <sstream>
#include <string>

//------------ print ------------//
void ModuleNode::print(std::string title, int indent) {
  std::cout << "//----" << std::string(title.size(), '-') << "----//\n";
  std::cout << "//--- " << title << " ---//\n";
  std::cout << "//----" << std::string(title.size(), '-') << "----//\n";
  for (auto &constDef : constDefs)
    constDef->print(indent);
  for (auto &metadata : metadatas)
    metadata->print(indent);
  for (auto &funDef : funDefs)
    funDef->print(indent);
  for (auto &block : blocks)
    block->print(indent);
}

void MetadataNode::print(int indent) {
  std::cout << inden(indent) << "__" << key << "__ = " << *expNode << ";\n";
}

void FunDefNode::print(int indent) {
  std::cout << inden(indent) << "def " << identifier << "(";
  for (int i = 0; i < params.size(); i++) {
    std::cout << params[i];
    if (i != params.size() - 1)
      std::cout << ", ";
  }
  std::cout << ") : ";
  if (typedInstrSet)
    typedInstrSet->print(indent);
  if (blockBody) {
    std::cout << "block";
    blockBody->print(indent);
  }
}

void ConstDefNode::print(int indent) {
  std::cout << inden(indent) << "const " << key << " = " << *expNode << ";\n";
}

void BlockNode::print(int indent) {
  std::cout << inden(indent) << "block " << identifier;
  if (blockBody)
    blockBody->print(indent);
  else {
    std::cout << " = ";
    blockConstructor->print(indent);
  }
}

void BlockBodyNode::print(int indent) {
  std::cout << " {\n";
  for (auto &metadata : metadatas)
    metadata->print(indent + 4);
  for (auto &typedInstrSet : typedInstrSets) {
    std::cout << inden(indent + 4);
    typedInstrSet->print(indent + 4);
  }
  std::cout << inden(indent) << "}\n";
}

void TypedInstrSetNode::print(int indent) {
  std::cout << type << " {\n";
  instrSet->print(indent + 4);
  std::cout << inden(indent) << "}\n";
}

void InstrSetNode::print(int indent) {
  for (auto &compositeInstr : instructions)
    compositeInstr->print(indent);
}

void CompositeInstrNode::print(int indent) {
  if (instruction)
    instruction->print(indent);
  if (branchNode)
    branchNode->print(indent);
  if (forNode)
    forNode->print(indent);
}

void BranchNode::print(int indent) {
  bool elif = false;
  for (auto &ifRegion : ifRegions) {
    if (elif)
      std::cout << " else ";
    else
      std::cout << inden(indent);
    ifRegion->print(indent);
    elif = true;
  }
  if (!elseRegion) {
    std::cout << "\n";
    return;
  }
  std::cout << " else {\n";
  elseRegion->print(indent + 4);
  std::cout << inden(indent) << "}\n";
}

void IfRegionNode::print(int indent) {
  std::cout << "if(" << *condition << ") {\n";
  region->print(indent + 4);
  std::cout << inden(indent) << "}";
}

void ForNode::print(int indent) {
  std::cout << inden(indent) << "for(" << iterArg << " in ";
  if (fromExp && toExp) {
    std::cout << *fromExp << "..." << *toExp;
  } else {
    std::cout << *listExp;
  }
  std::cout << ") {\n";
  region->print(indent + 4);
  std::cout << inden(indent) << "}\n";
}

void InstructionNode::print(int indent) {
  std::cout << inden(indent) << identifier;
  paramApps->print(indent);
  std::cout << ";\n";
}

void ParamAppsNode::print(int indent) {
  std::cout << "(";
  for (int i = 0; i < positional_args.size(); i++) {
    std::cout << *positional_args[i]->expNode;
    if (i != positional_args.size() - 1 || !named_args.empty())
      std::cout << ", ";
  }
  for (int i = 0; i < named_args.size(); i++) {
    std::cout << named_args[i].get()->key << " = ";
    std::cout << *named_args[i]->expNode;
    if (i != named_args.size() - 1)
      std::cout << ", ";
  }
  std::cout << ")";
}

//------------ clone ------------//
std::unique_ptr<MetadataNode> MetadataNode::clone() {
  return std::make_unique<MetadataNode>(key, expNode->clone(), loc);
}

std::unique_ptr<FunDefNode> FunDefNode::clone() {
  std::unique_ptr<FunDefNode> funDefNode;
  if (typedInstrSet) {
    funDefNode =
        std::make_unique<FunDefNode>(identifier, loc, typedInstrSet->clone());
  } else {
    funDefNode =
        std::make_unique<FunDefNode>(identifier, loc, blockBody->clone());
  }
  for (auto &param : params)
    funDefNode->params.push_back(param);
  return funDefNode;
}

std::unique_ptr<ConstDefNode> ConstDefNode::clone() {
  return std::make_unique<ConstDefNode>(key, expNode->clone(), loc);
}

std::unique_ptr<BlockNode> BlockNode::clone() {
  auto blockNode = std::make_unique<BlockNode>(identifier, loc);
  if (blockConstructor)
    blockNode->blockConstructor = blockConstructor->clone();
  if (blockBody)
    blockNode->blockBody = blockBody->clone();
  return blockNode;
}

std::unique_ptr<BlockBodyNode> BlockBodyNode::clone() {
  auto newNode = std::make_unique<BlockBodyNode>(loc);
  for (auto &typedInstr : typedInstrSets)
    newNode->typedInstrSets.push_back(typedInstr->clone());
  for (auto &metadata : metadatas)
    newNode->metadatas.push_back(metadata->clone());
  return newNode;
}

std::unique_ptr<TypedInstrSetNode> TypedInstrSetNode::clone() {
  auto newNode =
      std::make_unique<TypedInstrSetNode>(loc, type, instrSet->clone());
  return newNode;
}

std::unique_ptr<InstrSetNode> InstrSetNode::clone() {
  auto newNode = std::make_unique<InstrSetNode>(loc);
  for (auto &instr : instructions)
    newNode->instructions.push_back(instr->clone());
  return newNode;
}

std::unique_ptr<CompositeInstrNode> CompositeInstrNode::clone() {
  if (instruction)
    return std::make_unique<CompositeInstrNode>(loc, instruction->clone());
  else if (branchNode)
    return std::make_unique<CompositeInstrNode>(loc, branchNode->clone());
  else if (forNode)
    return std::make_unique<CompositeInstrNode>(loc, forNode->clone());
  return nullptr;
}

std::unique_ptr<BranchNode> BranchNode::clone() {
  auto newNode = std::make_unique<BranchNode>(loc);
  for (auto &ifRegion : ifRegions)
    newNode->ifRegions.push_back(ifRegion->clone());
  if (elseRegion)
    newNode->elseRegion = elseRegion->clone();
  return newNode;
}

std::unique_ptr<IfRegionNode> IfRegionNode::clone() {
  return std::make_unique<IfRegionNode>(condition->clone(), region->clone(),
                                        loc);
}

std::unique_ptr<ForNode> ForNode::clone() {
  if (fromExp && toExp)
    return std::make_unique<ForNode>(iterArg, fromExp->clone(), toExp->clone(),
                                     region->clone(), loc);
  return std::make_unique<ForNode>(iterArg, listExp->clone(), region->clone(),
                                   loc);
}

std::unique_ptr<InstructionNode> InstructionNode::clone() {
  return std::make_unique<InstructionNode>(identifier, paramApps->clone(), loc);
}

std::unique_ptr<ParamAppsNode> ParamAppsNode::clone() {
  auto newNode = std::make_unique<ParamAppsNode>(loc);
  for (auto &namedArg : named_args)
    newNode.get()->named_args.push_back(namedArg.get()->clone());
  for (auto &positionalArg : positional_args)
    newNode.get()->positional_args.push_back(positionalArg.get()->clone());
  return newNode;
}

std::unique_ptr<NamedParamAppsNode> NamedParamAppsNode::clone() {
  auto newExp = std::move(expNode.get()->clone());
  return std::make_unique<NamedParamAppsNode>(key, newExp, loc);
}

std::unique_ptr<PositionalParamAppsNode> PositionalParamAppsNode::clone() {
  auto newExp = std::move(expNode.get()->clone());
  return std::make_unique<PositionalParamAppsNode>(newExp, loc);
}

std::unique_ptr<ExpressionNode> ExpressionNode::clone() {
  if (kind == EXPRESSION_KIND_VALUE)
    return std::make_unique<ExpressionNode>(value->clone(), loc);
  std::vector<std::unique_ptr<ExpressionNode>> clonedArgs;
  for (auto &arg : args)
    clonedArgs.push_back(arg->clone());
  if (kind == EXPRESSION_KIND_EXPRESSION)
    return std::make_unique<ExpressionNode>(std::move(clonedArgs[0]),
                                            std::move(clonedArgs[1]), op, loc);
  return std::make_unique<ExpressionNode>(std::move(clonedArgs), op, loc);
}

std::unique_ptr<ValueNode> ValueNode::clone() {
  assert(false && "ValueNode clone is forbidden\n");
  return nullptr;
}

std::unique_ptr<ValueNode> ListValueNode::clone() {
  auto newNode = std::make_unique<ListValueNode>(loc);
  for (auto &item : items)
    newNode.get()->items.push_back(item->clone());
  return newNode;
}

std::unique_ptr<ValueNode> PointValueNode::clone() {
  return std::make_unique<PointValueNode>(x->clone(), y->clone(), loc);
}

std::unique_ptr<ValueNode> ActorMatchValueNode::clone() {
  return std::make_unique<ActorMatchValueNode>(paramApps->clone(), loc);
}

std::unique_ptr<ValueNode> ButtonValueNode::clone() {
  return std::make_unique<ButtonValueNode>(paramApps->clone(), loc);
}

std::unique_ptr<ValueNode> CustomWeaponValueNode::clone() {
  return std::make_unique<CustomWeaponValueNode>(paramApps->clone(), loc);
}

std::unique_ptr<ValueNode> StringValueNode::clone() {
  return std::make_unique<StringValueNode>(value, loc);
}

std::unique_ptr<ValueNode> IntValueNode::clone() {
  return std::make_unique<IntValueNode>(value, loc);
}

std::unique_ptr<ValueNode> BoolValueNode::clone() {
  return std::make_unique<BoolValueNode>(value, loc);
}

std::unique_ptr<ValueNode> VariableValueNode::clone() {
  return std::make_unique<VariableValueNode>(value, loc);
}

//------------ propagateExp ------------//
bool ModuleNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  bool ret = true;
  for (auto &metadata : metadatas)
    ret &= metadata->propagateExp(varExpMap);
  for (auto &block : blocks)
    ret &= block->propagateExp(varExpMap);
  return ret;
}

bool MetadataNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  return expNode->propagateExp(varExpMap);
}

bool BlockNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  return blockBody->propagateExp(varExpMap);
}

bool BlockBodyNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  bool ret = true;
  for (auto &metadata : metadatas)
    ret &= metadata->propagateExp(varExpMap);
  for (auto &typedInstr : typedInstrSets)
    ret &= typedInstr->propagateExp(varExpMap);
  return ret;
}

bool TypedInstrSetNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  return instrSet->propagateExp(varExpMap);
}

bool InstrSetNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  bool ret = true;
  for (auto &compositeInstr : instructions)
    ret &= compositeInstr->propagateExp(varExpMap);
  return ret;
}

bool CompositeInstrNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  if (instruction)
    return instruction->propagateExp(varExpMap);
  if (branchNode)
    return branchNode->propagateExp(varExpMap);
  if (forNode)
    return forNode->propagateExp(varExpMap);
  return true;
}

bool BranchNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  bool ret = true;
  for (auto &ifRegion : ifRegions)
    ret &= ifRegion->propagateExp(varExpMap);
  if (elseRegion)
    ret &= elseRegion->propagateExp(varExpMap);
  return ret;
}

bool IfRegionNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  bool ret = true;
  ret &= region->propagateExp(varExpMap);
  ret &= condition->propagateExp(varExpMap);
  return ret;
}

bool ForNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  bool ret = true;
  if (varExpMap.find(iterArg) != varExpMap.end()) {
    std::cerr << "Compilation Error: iteration variable \'" << iterArg
              << "\' is redefined at " << loc << "\n";
    return false;
  }
  if (fromExp && toExp) {
    ret &= fromExp->propagateExp(varExpMap);
    ret &= toExp->propagateExp(varExpMap);
  } else {
    ret &= listExp->propagateExp(varExpMap);
  }
  ret &= region->propagateExp(varExpMap);
  return ret;
}

bool InstructionNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  return paramApps->propagateExp(varExpMap);
}

bool ParamAppsNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  bool ret = true;
  for (auto &namedArg : named_args)
    ret &= namedArg->propagateExp(varExpMap);
  for (auto &positionalArg : positional_args)
    ret &= positionalArg->propagateExp(varExpMap);
  return ret;
}

bool NamedParamAppsNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  return expNode->propagateExp(varExpMap);
}

bool PositionalParamAppsNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  return expNode->propagateExp(varExpMap);
}

bool ExpressionNode::propagateExp(
    std::map<std::string, std::unique_ptr<ExpressionNode>> &varExpMap) {
  bool ret = true;
  if (kind == EXPRESSION_KIND_VALUE) {
    if (auto *varNode = dynamic_cast<VariableValueNode *>(value.get())) {
      if (varExpMap.find(varNode->value) == varExpMap.end())
        return false;
      auto &constExp = varExpMap[varNode->value];
      if (constExp->kind == EXPRESSION_KIND_VALUE) {
        value = constExp->value->clone();
      } else {
        value = nullptr;
        kind = constExp->kind;
        args.clear();
        for (auto &arg : constExp->args)
          args.push_back(arg->clone());
        op = constExp->op;
      }
    } else if (auto *listNode = dynamic_cast<ListValueNode *>(value.get())) {
      for (auto &item : listNode->items)
        item->propagateExp(varExpMap);
    } else if (auto *actorMatchNode =
                   dynamic_cast<ActorMatchValueNode *>(value.get())) {
      actorMatchNode->paramApps->propagateExp(varExpMap);
    } else if (auto *buttonNode =
                   dynamic_cast<ButtonValueNode *>(value.get())) {
      buttonNode->paramApps->propagateExp(varExpMap);
    } else if (auto *customWeaponNode =
                   dynamic_cast<CustomWeaponValueNode *>(value.get())) {
      customWeaponNode->paramApps->propagateExp(varExpMap);
    }
  } else {
    for (auto &arg : args)
      ret &= arg->propagateExp(varExpMap);
  }
  return ret;
}

//------------ foldValue ------------//
bool ModuleNode::foldValue() {
  bool ret = true;
  for (auto &metadata : metadatas)
    ret &= metadata->foldValue();
  for (auto &block : blocks)
    ret &= block->foldValue();
  return ret;
}

bool MetadataNode::foldValue() { return expNode->foldValue(); }

bool BlockNode::foldValue() { return blockBody->foldValue(); }

bool BlockBodyNode::foldValue() {
  bool ret = true;
  for (auto &metadata : metadatas)
    ret &= metadata->foldValue();
  for (auto &typedInstr : typedInstrSets)
    ret &= typedInstr->foldValue();
  return ret;
}

bool TypedInstrSetNode::foldValue() { return instrSet->foldValue(); }

bool InstrSetNode::foldValue() {
  bool ret = true;
  for (auto &compositeInstr : instructions)
    ret &= compositeInstr->foldValue();
  return ret;
}

bool CompositeInstrNode::foldValue() {
  if (instruction)
    return instruction->foldValue();
  if (branchNode)
    return branchNode->foldValue();
  if (forNode)
    return forNode->foldValue();
  return true;
}

bool BranchNode::foldValue() {
  bool ret = true;
  int consecutiveFalseCount = 0;
  int firstTrueIdx = -1;
  for (auto idx = 0; idx < ifRegions.size(); idx++) {
    ret &= ifRegions.at(idx)->foldValue();
    if (auto boolValue = dynamic_cast<BoolValueNode *>(
            ifRegions.at(idx)->condition->value.get())) {
      if (boolValue->value) {
        firstTrueIdx = idx;
        break;
      } else if (consecutiveFalseCount == idx) {
        consecutiveFalseCount++;
      }
    }
  }
  if (firstTrueIdx != -1) {
    elseRegion = nullptr;
    if (firstTrueIdx + 1 < ifRegions.size())
      ifRegions.erase(ifRegions.begin() + firstTrueIdx + 1, ifRegions.end());
  }
  if (0 < consecutiveFalseCount && consecutiveFalseCount < ifRegions.size())
    ifRegions.erase(ifRegions.begin(),
                    ifRegions.begin() + consecutiveFalseCount);
  if (elseRegion)
    ret &= elseRegion->foldValue();
  return ret;
}

bool IfRegionNode::foldValue() {
  bool ret = true;
  ret &= region->foldValue();
  ret &= condition->foldValue();
  return ret;
}

bool ForNode::foldValue() {
  bool ret = true;
  if (fromExp && toExp) {
    ret &= fromExp->foldValue();
    ret &= toExp->foldValue();
  } else {
    ret &= listExp->foldValue();
  }
  ret &= region->foldValue();
  return ret;
}

bool InstructionNode::foldValue() { return paramApps->foldValue(); }

bool ParamAppsNode::foldValue() {
  bool ret = true;
  for (auto &namedArg : named_args)
    ret &= namedArg->foldValue();
  for (auto &positionalArg : positional_args)
    ret &= positionalArg->foldValue();
  return ret;
}

bool NamedParamAppsNode::foldValue() { return expNode->foldValue(); }

bool PositionalParamAppsNode::foldValue() { return expNode->foldValue(); }

namespace {

bool foldNestedValueNode(ValueNode *valueNode) {
  bool ret = true;
  auto valuePoint = dynamic_cast<PointValueNode *>(valueNode);
  auto valueActorMatch = dynamic_cast<ActorMatchValueNode *>(valueNode);
  auto valueButton = dynamic_cast<ButtonValueNode *>(valueNode);
  auto valueCustomWeapon = dynamic_cast<CustomWeaponValueNode *>(valueNode);
  auto valueList = dynamic_cast<ListValueNode *>(valueNode);
  if (valuePoint) {
    ret &= valuePoint->x->foldValue();
    ret &= valuePoint->y->foldValue();
  } else if (valueList) {
    for (auto &item : valueList->items)
      ret &= item->foldValue();
  } else if (valueActorMatch) {
    ret &= valueActorMatch->paramApps->foldValue();
  } else if (valueButton) {
    ret &= valueButton->paramApps->foldValue();
  } else if (valueCustomWeapon) {
    ret &= valueCustomWeapon->paramApps->foldValue();
  }
  return ret;
}

std::unique_ptr<ValueNode>
foldUnaryIntrinsicValue(const ExpressionNode &arg, ExpOpType op) {
  auto argStr = dynamic_cast<StringValueNode *>(arg.value.get());
  auto argInt = dynamic_cast<IntValueNode *>(arg.value.get());
  auto argBool = dynamic_cast<BoolValueNode *>(arg.value.get());
  auto argList = dynamic_cast<ListValueNode *>(arg.value.get());
  if (argStr) {
    if (op == EXP_OP_TYPE_INTRINSIC_TO_STRING)
      return std::make_unique<StringValueNode>(argStr->value, argStr->loc);
    if (op == EXP_OP_TYPE_INTRINSIC_TO_INT) {
      std::istringstream iss(argStr->value);
      int res;
      if (iss >> res)
        return std::make_unique<IntValueNode>(res, argStr->loc);
    }
    if (op == EXP_OP_TYPE_INTRINSIC_GET_LENGTH)
      return std::make_unique<IntValueNode>(argStr->value.length(),
                                            argStr->loc);
  }
  if (argInt) {
    if (op == EXP_OP_TYPE_INTRINSIC_TO_STRING)
      return std::make_unique<StringValueNode>(std::to_string(argInt->value),
                                               argInt->loc);
    if (op == EXP_OP_TYPE_INTRINSIC_TO_INT)
      return std::make_unique<IntValueNode>(argInt->value, argInt->loc);
    if (op == EXP_OP_TYPE_INTRINSIC_TO_BOOL)
      return std::make_unique<BoolValueNode>(argInt->value, argInt->loc);
  }
  if (argBool) {
    if (op == EXP_OP_TYPE_INTRINSIC_TO_INT)
      return std::make_unique<IntValueNode>(argBool->value, argBool->loc);
    if (op == EXP_OP_TYPE_INTRINSIC_TO_BOOL)
      return std::make_unique<BoolValueNode>(argBool->value, argBool->loc);
  }
  if (argList) {
    if (op == EXP_OP_TYPE_INTRINSIC_GET_LENGTH)
      return std::make_unique<IntValueNode>(argList->items.size(),
                                            argList->loc);
  }
  return nullptr;
}

bool foldStringBinary(ExpressionNode &exp, StringValueNode *lhs,
                      StringValueNode *rhs) {
  if (exp.op == EXP_OP_TYPE_ADD) {
    exp.value =
        std::make_unique<StringValueNode>(lhs->value + rhs->value, lhs->loc);
    return true;
  }
  if (exp.op == EXP_OP_TYPE_EQUAL) {
    exp.value =
        std::make_unique<BoolValueNode>(lhs->value == rhs->value, lhs->loc);
    return true;
  }
  if (exp.op == EXP_OP_TYPE_NOT_EQUAL) {
    exp.value =
        std::make_unique<BoolValueNode>(lhs->value != rhs->value, lhs->loc);
    return true;
  }
  return false;
}

bool foldIntBinary(ExpressionNode &exp, IntValueNode *lhs, IntValueNode *rhs) {
  if (exp.op == EXP_OP_TYPE_ADD) {
    exp.value = std::make_unique<IntValueNode>(lhs->value + rhs->value,
                                               lhs->loc);
    return true;
  }
  if (exp.op == EXP_OP_TYPE_SUB) {
    exp.value = std::make_unique<IntValueNode>(lhs->value - rhs->value,
                                               lhs->loc);
    return true;
  }
  if (exp.op == EXP_OP_TYPE_MUL) {
    exp.value = std::make_unique<IntValueNode>(lhs->value * rhs->value,
                                               lhs->loc);
    return true;
  }
  if (exp.op == EXP_OP_TYPE_DIV) {
    exp.value = std::make_unique<IntValueNode>(lhs->value / rhs->value,
                                               lhs->loc);
    return true;
  }
  if (exp.op == EXP_OP_TYPE_MOD) {
    exp.value = std::make_unique<IntValueNode>(lhs->value % rhs->value,
                                               lhs->loc);
    return true;
  }
  if (exp.op == EXP_OP_TYPE_EQUAL) {
    exp.value =
        std::make_unique<BoolValueNode>(lhs->value == rhs->value, lhs->loc);
    return true;
  }
  if (exp.op == EXP_OP_TYPE_NOT_EQUAL) {
    exp.value =
        std::make_unique<BoolValueNode>(lhs->value != rhs->value, lhs->loc);
    return true;
  }
  if (exp.op == EXP_OP_TYPE_LESS_THAN) {
    exp.value = std::make_unique<BoolValueNode>(lhs->value < rhs->value,
                                                lhs->loc);
    return true;
  }
  if (exp.op == EXP_OP_TYPE_LESS_THAN_EQUAL) {
    exp.value = std::make_unique<BoolValueNode>(lhs->value <= rhs->value,
                                                lhs->loc);
    return true;
  }
  if (exp.op == EXP_OP_TYPE_GREATER_THAN) {
    exp.value = std::make_unique<BoolValueNode>(lhs->value > rhs->value,
                                                lhs->loc);
    return true;
  }
  if (exp.op == EXP_OP_TYPE_GREATER_THAN_EQUAL) {
    exp.value = std::make_unique<BoolValueNode>(lhs->value >= rhs->value,
                                                lhs->loc);
    return true;
  }
  return false;
}

bool foldBoolBinary(ExpressionNode &exp, BoolValueNode *lhs,
                    BoolValueNode *rhs) {
  if (exp.op == EXP_OP_TYPE_EQUAL) {
    exp.value =
        std::make_unique<BoolValueNode>(lhs->value == rhs->value, lhs->loc);
    return true;
  }
  if (exp.op == EXP_OP_TYPE_NOT_EQUAL) {
    exp.value =
        std::make_unique<BoolValueNode>(lhs->value != rhs->value, lhs->loc);
    return true;
  }
  if (exp.op == EXP_OP_TYPE_AND) {
    exp.value =
        std::make_unique<BoolValueNode>(lhs->value && rhs->value, lhs->loc);
    return true;
  }
  if (exp.op == EXP_OP_TYPE_OR) {
    exp.value =
        std::make_unique<BoolValueNode>(lhs->value || rhs->value, lhs->loc);
    return true;
  }
  return false;
}

bool isUnaryIntrinsicOp(ExpOpType op) {
  return op == EXP_OP_TYPE_INTRINSIC_TO_STRING ||
         op == EXP_OP_TYPE_INTRINSIC_TO_INT ||
         op == EXP_OP_TYPE_INTRINSIC_TO_BOOL ||
         op == EXP_OP_TYPE_INTRINSIC_GET_LENGTH;
}

bool isBinaryIntrinsicOp(ExpOpType op) {
  return op == EXP_OP_TYPE_INTRINSIC_GET_INDEX;
}

void finalizeFoldedExpression(ExpressionNode &exp) {
  exp.kind = EXPRESSION_KIND_VALUE;
  exp.args.clear();
  exp.op = EXP_OP_TYPE_VOID;
}

bool foldIntrinsicExpression(ExpressionNode &exp) {
  if (exp.args.empty())
    return false;

  for (auto &arg : exp.args) {
    if (!arg->foldValue() || arg->kind != EXPRESSION_KIND_VALUE)
      return false;
  }

  // Binary intrinsic
  if (isBinaryIntrinsicOp(exp.op)) {
    if (exp.args.size() != 2)
      return false;
    auto *lhsList = dynamic_cast<ListValueNode *>(exp.args[0]->value.get());
    auto *lhsStr = dynamic_cast<StringValueNode *>(exp.args[0]->value.get());
    auto *rhsInt = dynamic_cast<IntValueNode *>(exp.args[1]->value.get());
    if (!rhsInt)
      return false;
    const int idx = rhsInt->value;
    if (idx < 0)
      return false;

    if (lhsList) {
      if (static_cast<size_t>(idx) >= lhsList->items.size())
        return false;
      exp.value = lhsList->items.at(static_cast<size_t>(idx))->value->clone();
      finalizeFoldedExpression(exp);
      return true;
    }
    if (lhsStr) {
      if (static_cast<size_t>(idx) >= lhsStr->value.size())
        return false;
      exp.value =
          std::make_unique<StringValueNode>(lhsStr->value.substr(idx, 1),
                                            lhsStr->loc);
      finalizeFoldedExpression(exp);
      return true;
    }
    return false;
  }

  // Unary intrinsic
  if (!isUnaryIntrinsicOp(exp.op) || exp.args.size() != 1)
    return false;

  auto foldedValue = foldUnaryIntrinsicValue(*exp.args[0], exp.op);
  if (!foldedValue)
    return false;
  exp.value = std::move(foldedValue);
  finalizeFoldedExpression(exp);
  return true;
}

bool foldBinaryExpression(ExpressionNode &exp) {
  if (exp.args.size() != 2)
    return false;

  auto &lhs = exp.args[0];
  auto &rhs = exp.args[1];
  if (!lhs->foldValue() || lhs->kind != EXPRESSION_KIND_VALUE)
    return false;
  if (!rhs->foldValue() || rhs->kind != EXPRESSION_KIND_VALUE)
    return false;

  assert(lhs->value.get());
  assert(rhs->value.get());

  auto lhsStr = dynamic_cast<StringValueNode *>(lhs->value.get());
  auto lhsInt = dynamic_cast<IntValueNode *>(lhs->value.get());
  auto lhsBool = dynamic_cast<BoolValueNode *>(lhs->value.get());
  auto rhsStr = dynamic_cast<StringValueNode *>(rhs->value.get());
  auto rhsInt = dynamic_cast<IntValueNode *>(rhs->value.get());
  auto rhsBool = dynamic_cast<BoolValueNode *>(rhs->value.get());

  bool isFolded = false;
  if (lhsStr && rhsStr)
    isFolded = foldStringBinary(exp, lhsStr, rhsStr);
  else if (lhsInt && rhsInt)
    isFolded = foldIntBinary(exp, lhsInt, rhsInt);
  else if (lhsBool && rhsBool)
    isFolded = foldBoolBinary(exp, lhsBool, rhsBool);

  if (isFolded)
    finalizeFoldedExpression(exp);
  return isFolded;
}

} // namespace

bool ExpressionNode::foldValue() {
  if (kind == EXPRESSION_KIND_VALUE)
    return foldNestedValueNode(value.get());

  if (kind == EXPRESSION_KIND_INTRINSIC)
    return foldIntrinsicExpression(*this);

  if (kind == EXPRESSION_KIND_EXPRESSION)
    return foldBinaryExpression(*this);

  return false;
}

//------------ hasUnresolvedValue ------------//
bool ModuleNode::hasUnresolvedValue(std::set<std::string> except) {
  bool ret = false;
  for (auto &metadata : metadatas)
    ret |= metadata->hasUnresolvedValue(except);
  for (auto &block : blocks)
    ret |= block->hasUnresolvedValue(except);
  return ret;
}

bool MetadataNode::hasUnresolvedValue(std::set<std::string> except) {
  return expNode->hasUnresolvedValue(except);
}

bool BlockNode::hasUnresolvedValue(std::set<std::string> except) {
  return blockBody->hasUnresolvedValue(except);
}

bool BlockBodyNode::hasUnresolvedValue(std::set<std::string> except) {
  bool ret = false;
  for (auto &metadata : metadatas)
    ret |= metadata->hasUnresolvedValue(except);
  for (auto &typedInstr : typedInstrSets)
    ret |= typedInstr->hasUnresolvedValue(except);
  return ret;
}

bool TypedInstrSetNode::hasUnresolvedValue(std::set<std::string> except) {
  return instrSet->hasUnresolvedValue(except);
}

bool InstrSetNode::hasUnresolvedValue(std::set<std::string> except) {
  bool ret = false;
  for (auto &compositeInstr : instructions)
    ret |= compositeInstr->hasUnresolvedValue(except);
  return ret;
}

bool CompositeInstrNode::hasUnresolvedValue(std::set<std::string> except) {
  if (instruction)
    return instruction->hasUnresolvedValue(except);
  if (branchNode)
    return branchNode->hasUnresolvedValue(except);
  if (forNode)
    return forNode->hasUnresolvedValue(except);
  return true;
}

bool BranchNode::hasUnresolvedValue(std::set<std::string> except) {
  bool ret = false;
  for (auto &ifRegion : ifRegions)
    ret |= ifRegion->hasUnresolvedValue(except);
  if (elseRegion)
    ret |= elseRegion->hasUnresolvedValue(except);
  return ret;
}

bool IfRegionNode::hasUnresolvedValue(std::set<std::string> except) {
  bool ret = false;
  ret |= region->hasUnresolvedValue(except);
  ret |= condition->hasUnresolvedValue(except);
  return ret;
}

bool ForNode::hasUnresolvedValue(std::set<std::string> except) {
  bool ret = false;
  except.insert(iterArg);
  if (fromExp && toExp) {
    ret |= fromExp->hasUnresolvedValue(except);
    ret |= toExp->hasUnresolvedValue(except);
  } else {
    ret |= listExp->hasUnresolvedValue(except);
  }
  ret |= region->hasUnresolvedValue(except);
  return ret;
}

bool InstructionNode::hasUnresolvedValue(std::set<std::string> except) {
  return paramApps->hasUnresolvedValue(except);
}

bool ParamAppsNode::hasUnresolvedValue(std::set<std::string> except) {
  bool ret = false;
  for (auto &namedArg : named_args)
    ret |= namedArg->hasUnresolvedValue(except);
  for (auto &positionalArg : positional_args)
    ret |= positionalArg->hasUnresolvedValue(except);
  return ret;
}

bool NamedParamAppsNode::hasUnresolvedValue(std::set<std::string> except) {
  return expNode->hasUnresolvedValue(except);
}

bool PositionalParamAppsNode::hasUnresolvedValue(std::set<std::string> except) {
  return expNode->hasUnresolvedValue(except);
}

bool ExpressionNode::hasUnresolvedValue(std::set<std::string> except) {
  if (kind == EXPRESSION_KIND_VALUE) {
    if (auto varNode = dynamic_cast<VariableValueNode *>(value.get())) {
      if (except.count(varNode->value) > 0)
        return false;
      std::cerr << "Compilation Error: Found unresolvable variable `"
                << varNode->value << "` at " << varNode->loc << "\n";
      return true;
    }
    return false;
  } else {
    std::cerr << "Compilation Error: Found unresolvable expression `" << *this
              << "` at " << loc << "\n";
    return true;
  }
}