#ifndef AST_H
#define AST_H

#include "utils/location.h"
#include <cassert>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

// Global level
class ModuleNode;
class MetadataNode;
class FunDefNode;
class ConstDefNode;
class BlockNode;
class BlockBodyNode;
// Instruction Set level
class TypedInstrSetNode;
class InstrSetNode;
class CompositeInstrNode;
class BranchNode;
class IfRegionNode;
class ForNode;
// Instruction level
class InstructionNode;
class ParamAppsNode;
class NamedParamAppsNode;
class PositionalParamAppsNode;
// Expression level
class ExpressionNode;
class ValueNode;
class ActorMatchValueNode;
class ButtonValueNode;
class CustomWeaponValueNode;
class BoolValueNode;
class IntValueNode;
class ListValueNode;
class PointValueNode;
class StringValueNode;
class VariableValueNode;

using ExpressionMap = std::map<std::string, std::unique_ptr<ExpressionNode>>;
using SymbolSet = std::set<std::string>;

//------------ Enumeration definition ------------//
enum FunDefType {
  FUN_DEF_TYPE_INVALID,
  FUN_DEF_TYPE_ACTIONS,
  FUN_DEF_TYPE_CHECKS,
  FUN_DEF_TYPE_TRIGGERS,
};

enum ExpOpType {
  EXP_OP_TYPE_VOID,
  EXP_OP_TYPE_AND,
  EXP_OP_TYPE_OR,
  EXP_OP_TYPE_EQUAL,
  EXP_OP_TYPE_NOT_EQUAL,
  EXP_OP_TYPE_LESS_THAN,
  EXP_OP_TYPE_LESS_THAN_EQUAL,
  EXP_OP_TYPE_GREATER_THAN,
  EXP_OP_TYPE_GREATER_THAN_EQUAL,
  EXP_OP_TYPE_ADD,
  EXP_OP_TYPE_SUB,
  EXP_OP_TYPE_MUL,
  EXP_OP_TYPE_DIV,
  EXP_OP_TYPE_MOD,
  EXP_OP_TYPE_INTRINSIC_TO_STRING,
  EXP_OP_TYPE_INTRINSIC_TO_INT,
  EXP_OP_TYPE_INTRINSIC_TO_BOOL,
  EXP_OP_TYPE_INTRINSIC_GET_INDEX,
  EXP_OP_TYPE_INTRINSIC_GET_LENGTH,
  EXP_OP_TYPE_INTRINSIC_GET_SLICE,
};

enum ExpressionKind {
  EXPRESSION_KIND_VALUE,
  EXPRESSION_KIND_INTRINSIC,
  EXPRESSION_KIND_EXPRESSION,
};

class ASTNode {
public:
  Location loc;

  ASTNode(Location loc) : loc(loc) {}
  virtual ~ASTNode() = default;

  virtual void print(int indent = 0) {}
  virtual bool propagateExp(ExpressionMap &) {
    return true;
  }
  virtual bool foldValue() { return true; }
  virtual bool hasUnresolvedValue(SymbolSet except = {}) {
    return false;
  }
  virtual void refreshTrace() {}
};

//------------ Global level definition ------------//
class ModuleNode : public ASTNode {
public:
  std::vector<std::unique_ptr<MetadataNode>> metadatas;
  std::vector<std::unique_ptr<BlockNode>> blocks;
  std::vector<std::unique_ptr<FunDefNode>> funDefs;
  std::vector<std::unique_ptr<ConstDefNode>> constDefs;

  // Constructor
  ModuleNode(Location loc) : ASTNode(loc) {}

  // Function
  void print(std::string title, int indent = 0);
  bool propagateExp(ExpressionMap &) override;
  bool foldValue() override;
  bool hasUnresolvedValue(SymbolSet except = {}) override;
};

class MetadataNode : public ASTNode {
public:
  std::string key;
  std::unique_ptr<ExpressionNode> expNode;

  // Constructor
  MetadataNode(std::string key, std::unique_ptr<ExpressionNode> expNode,
               Location loc)
      : ASTNode(loc), key(key), expNode(std::move(expNode)) {}

  // Function
  void print(int indent = 0) override;
  std::unique_ptr<MetadataNode> clone();
  bool propagateExp(ExpressionMap &) override;
  bool foldValue() override;
  bool hasUnresolvedValue(SymbolSet except = {}) override;
};

class FunDefNode : public ASTNode {
public:
  std::string identifier;
  std::vector<std::string> params;
  std::unique_ptr<TypedInstrSetNode> typedInstrSet;
  std::unique_ptr<BlockBodyNode> blockBody;

  // Constructor
  FunDefNode(const std::string &id, Location loc)
      : ASTNode(loc), identifier(id) {}
  FunDefNode(const std::string &id, Location loc,
             std::unique_ptr<TypedInstrSetNode> typedInstrSet)
      : ASTNode(loc), identifier(id), typedInstrSet(std::move(typedInstrSet)) {}
  FunDefNode(const std::string &id, Location loc,
             std::unique_ptr<BlockBodyNode> blockBody)
      : ASTNode(loc), identifier(id), blockBody(std::move(blockBody)) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<FunDefNode> clone();
};

class BlockNode : public ASTNode {
public:
  std::string identifier;
  std::unique_ptr<BlockBodyNode> blockBody;
  std::unique_ptr<InstructionNode> blockConstructor;

  // Constructor
  BlockNode(const std::string &id, Location loc)
      : ASTNode(loc), identifier(id) {}

  // Function
  void print(int indent = 0) override;
  std::unique_ptr<BlockNode> clone();
  bool propagateExp(ExpressionMap &) override;
  bool foldValue() override;
  bool hasUnresolvedValue(SymbolSet except = {}) override;
};

class BlockBodyNode : public ASTNode {
public:
  std::vector<std::unique_ptr<MetadataNode>> metadatas;
  std::vector<std::unique_ptr<TypedInstrSetNode>> typedInstrSets;

  // Constructor
  BlockBodyNode(Location loc) : ASTNode(loc) {}

  // Function
  void print(int indent = 0) override;
  std::unique_ptr<BlockBodyNode> clone();
  bool propagateExp(ExpressionMap &) override;
  bool foldValue() override;
  bool hasUnresolvedValue(SymbolSet except = {}) override;
  void setActionsIdx(int idx) { actionsIdx = idx; };
  void setChecksIdx(int idx) { checksIdx = idx; };
  void setTriggersIdx(int idx) { triggersIdx = idx; };
  std::unique_ptr<TypedInstrSetNode> &getActions() {
    return typedInstrSets[actionsIdx];
  };
  std::unique_ptr<TypedInstrSetNode> &getChecks() {
    return typedInstrSets[checksIdx];
  };
  std::unique_ptr<TypedInstrSetNode> &getTriggers() {
    return typedInstrSets[triggersIdx];
  };

private:
  int actionsIdx = -1;
  int checksIdx = -1;
  int triggersIdx = -1;
};

class ConstDefNode : public ASTNode {
public:
  std::string key;
  std::unique_ptr<ExpressionNode> expNode;

  // Constructor
  ConstDefNode(std::string key, std::unique_ptr<ExpressionNode> expNode,
               Location loc)
      : ASTNode(loc), key(key), expNode(std::move(expNode)) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<ConstDefNode> clone();
};

//------------ Instruction Set level definition ------------//
class TypedInstrSetNode : public ASTNode {
public:
  FunDefType type;
  std::unique_ptr<InstrSetNode> instrSet;

  // Constructor
  TypedInstrSetNode(Location loc, FunDefType type = FUN_DEF_TYPE_INVALID)
      : ASTNode(loc), type(type), instrSet(std::make_unique<InstrSetNode>(loc)) {}
  TypedInstrSetNode(Location loc, FunDefType type,
                    std::unique_ptr<InstrSetNode> instrSet)
      : ASTNode(loc), type(type), instrSet(std::move(instrSet)) {}

  // Function
  void print(int indent = 0) override;
  std::unique_ptr<TypedInstrSetNode> clone();
  bool propagateExp(ExpressionMap &) override;
  bool foldValue() override;
  bool hasUnresolvedValue(SymbolSet except = {}) override;
};

class InstrSetNode : public ASTNode {
public:
  std::vector<std::unique_ptr<CompositeInstrNode>> instructions;

  // Constructor
  InstrSetNode(Location loc) : ASTNode(loc) {}

  // Function
  void print(int indent = 0) override;
  std::unique_ptr<InstrSetNode> clone();
  bool propagateExp(ExpressionMap &) override;
  bool foldValue() override;
  bool hasUnresolvedValue(SymbolSet except = {}) override;
};

class CompositeInstrNode : public ASTNode {
public:
  std::unique_ptr<InstructionNode> instruction;
  std::unique_ptr<BranchNode> branchNode;
  std::unique_ptr<ForNode> forNode;

  // Constructor
  CompositeInstrNode(Location loc) : ASTNode(loc) {}
  CompositeInstrNode(Location loc, std::unique_ptr<InstructionNode> instruction)
    : ASTNode(loc), instruction(std::move(instruction)) {}
  CompositeInstrNode(Location loc, std::unique_ptr<BranchNode> branchNode)
    : ASTNode(loc), branchNode(std::move(branchNode)) {}
  CompositeInstrNode(Location loc, std::unique_ptr<ForNode> forNode)
    : ASTNode(loc), forNode(std::move(forNode)) {}

  // Function
  void print(int indent = 0) override;
  std::unique_ptr<CompositeInstrNode> clone();
  bool propagateExp(ExpressionMap &) override;
  bool foldValue() override;
  bool hasUnresolvedValue(SymbolSet except = {}) override;
};

class BranchNode : public ASTNode {
public:
  std::vector<std::unique_ptr<IfRegionNode>> ifRegions;
  std::unique_ptr<InstrSetNode> elseRegion;

  // Constructor
  BranchNode(Location loc) : ASTNode(loc) {}

  // Function
  void print(int indent = 0) override;
  std::unique_ptr<BranchNode> clone();
  bool propagateExp(ExpressionMap &) override;
  bool foldValue() override;
  bool hasUnresolvedValue(SymbolSet except = {}) override;
};

class ForNode : public ASTNode {
public:
  std::string iterArg;
  std::unique_ptr<ExpressionNode> listExp;
  std::unique_ptr<ExpressionNode> fromExp;
  std::unique_ptr<ExpressionNode> toExp;
  std::unique_ptr<InstrSetNode> region;

  // Range Semantics Constructor `for i in exp...exp`
  ForNode(std::string iterArg, std::unique_ptr<ExpressionNode> fromExp,
          std::unique_ptr<ExpressionNode> toExp,
          std::unique_ptr<InstrSetNode> region, Location loc)
      : ASTNode(loc), iterArg(iterArg), fromExp(std::move(fromExp)),
        toExp(std::move(toExp)), region(std::move(region)) {}
  // List Semantics Constructor `for i in list`
  ForNode(std::string iterArg, std::unique_ptr<ExpressionNode> listExp,
          std::unique_ptr<InstrSetNode> region, Location loc)
      : ASTNode(loc), iterArg(iterArg), listExp(std::move(listExp)),
        region(std::move(region)) {}

  // Function
  void print(int indent = 0) override;
  std::unique_ptr<ForNode> clone();
  bool propagateExp(ExpressionMap &) override;
  bool foldValue() override;
  bool hasUnresolvedValue(SymbolSet except = {}) override;
};

class IfRegionNode : public ASTNode {
public:
  std::unique_ptr<ExpressionNode> condition;
  std::unique_ptr<InstrSetNode> region;

  // Constructor
  IfRegionNode(std::unique_ptr<ExpressionNode> condition,
               std::unique_ptr<InstrSetNode> region, Location loc)
    : ASTNode(loc), condition(std::move(condition)),
      region(std::move(region)) {}

  // Function
  void print(int indent = 0) override;
  std::unique_ptr<IfRegionNode> clone();
  bool propagateExp(ExpressionMap &) override;
  bool foldValue() override;
  bool hasUnresolvedValue(SymbolSet except = {}) override;
};

//------------ Instruction level definition ------------//
enum InstructionIntrinsicType {
  INSTRUCTION_INTRINSIC_NON,
  INSTRUCTION_INTRINSIC_ASSERT,
};

class InstructionNode : public ASTNode {
public:
  std::string identifier;
  std::unique_ptr<ParamAppsNode> paramApps;
  InstructionIntrinsicType intrinsicType = INSTRUCTION_INTRINSIC_NON;
  bool isNoOp = false;

  // Constructor
  InstructionNode(std::string identifier,
                  std::unique_ptr<ParamAppsNode> paramApps, Location loc,
                  InstructionIntrinsicType intrinsicType,
                  bool isNoOp = false)
      : ASTNode(loc), identifier(identifier), paramApps(std::move(paramApps)),
        intrinsicType(intrinsicType), isNoOp(isNoOp) {}

  // Function
  void print(int indent = 0) override;
  std::unique_ptr<InstructionNode> clone();
  bool propagateExp(ExpressionMap &) override;
  bool foldValue() override;
  bool hasUnresolvedValue(SymbolSet except = {}) override;
};

class ParamAppsNode : public ASTNode {
public:
  std::vector<std::unique_ptr<PositionalParamAppsNode>> positional_args;
  std::vector<std::unique_ptr<NamedParamAppsNode>> named_args;

  // Constructor
  ParamAppsNode(Location loc) : ASTNode(loc) {}

  // Function
  void print(int indent = 0) override;
  std::unique_ptr<ParamAppsNode> clone();
  void addNamedArg(std::unique_ptr<NamedParamAppsNode> namedArg);
  void addPositionalArg(std::unique_ptr<PositionalParamAppsNode> positionalArg);
  void refreshTrace() override;
  bool propagateExp(ExpressionMap &) override;
  bool foldValue() override;
  bool hasUnresolvedValue(SymbolSet except = {}) override;
};

class NamedParamAppsNode : public ASTNode {
public:
  std::string key;
  std::unique_ptr<ExpressionNode> expNode;

  // Constructor
  NamedParamAppsNode(const std::string &key,
                     std::unique_ptr<ExpressionNode> &expNode, Location loc)
      : ASTNode(loc), key(key), expNode(std::move(expNode)) {
    refreshTrace();
  }

  // Function
  std::unique_ptr<NamedParamAppsNode> clone();
  void refreshTrace() override;
  bool propagateExp(ExpressionMap &) override;
  bool foldValue() override;
  bool hasUnresolvedValue(SymbolSet except = {}) override;
};

class PositionalParamAppsNode : public ASTNode {
public:
  std::unique_ptr<ExpressionNode> expNode;

  // Constructor
  PositionalParamAppsNode(std::unique_ptr<ExpressionNode> &expNode,
                          Location loc)
      : ASTNode(loc), expNode(std::move(expNode)) {
    refreshTrace();
  }

  // Function
  std::unique_ptr<PositionalParamAppsNode> clone();
  void refreshTrace() override;
  bool propagateExp(ExpressionMap &) override;
  bool foldValue() override;
  bool hasUnresolvedValue(SymbolSet except = {}) override;
};

//------------ Expression level definition ------------//
class ValueNode : public ASTNode {
public:
  // Constructor
  ValueNode(Location loc) : ASTNode(loc) {}

  // Function
  virtual std::unique_ptr<ValueNode> clone();
  void refreshTrace() override {}
};

class ActorMatchValueNode : public ValueNode {
public:
  // Variable
  std::unique_ptr<ParamAppsNode> paramApps;

  // Constructor
  ActorMatchValueNode(std::unique_ptr<ParamAppsNode> paramApps, Location loc)
      : paramApps(std::move(paramApps)), ValueNode(loc) {
    refreshTrace();
  }

  // Function
  std::unique_ptr<ValueNode> clone();
  void refreshTrace() override;
};

class ButtonValueNode : public ValueNode {
public:
  // Variable
  std::unique_ptr<ParamAppsNode> paramApps;

  // Constructor
  ButtonValueNode(std::unique_ptr<ParamAppsNode> paramApps, Location loc)
      : paramApps(std::move(paramApps)), ValueNode(loc) {
    refreshTrace();
  }

  // Function
  std::unique_ptr<ValueNode> clone();
  void refreshTrace() override;
};

class CustomWeaponValueNode : public ValueNode {
public:
  // Variable
  std::unique_ptr<ParamAppsNode> paramApps;

  // Constructor
  CustomWeaponValueNode(std::unique_ptr<ParamAppsNode> paramApps, Location loc)
      : paramApps(std::move(paramApps)), ValueNode(loc) {
    refreshTrace();
  }

  // Function
  std::unique_ptr<ValueNode> clone();
  void refreshTrace() override;
};

class BoolValueNode : public ValueNode {
public:
  // Variable
  bool value;

  // Constructor
  BoolValueNode(bool value, Location loc) : ValueNode(loc), value(value) {}

  // Function
  std::unique_ptr<ValueNode> clone();
  void refreshTrace() override {}
};

class IntValueNode : public ValueNode {
public:
  // Variable
  int value;

  // Constructor
  IntValueNode(int value, Location loc) : ValueNode(loc), value(value) {}

  // Function
  std::unique_ptr<ValueNode> clone();
  void refreshTrace() override {}
};

class ListValueNode : public ValueNode {
public:
  // Variable
  std::vector<std::unique_ptr<ExpressionNode>> items = {};

  // Constructor
  ListValueNode(Location loc) : ValueNode(loc) {}

  // Function
  std::unique_ptr<ValueNode> clone();
  void refreshTrace() override;
};

class PointValueNode : public ValueNode {
public:
  // Variable
  std::unique_ptr<ExpressionNode> x;
  std::unique_ptr<ExpressionNode> y;

  // Constructor
  PointValueNode(std::unique_ptr<ExpressionNode> x,
                 std::unique_ptr<ExpressionNode> y, Location loc)
      : x(std::move(x)), y(std::move(y)), ValueNode(loc) {
    refreshTrace();
  }

  // Function
  std::unique_ptr<ValueNode> clone();
  void refreshTrace() override;
};

class StringValueNode : public ValueNode {
public:
  // Variable
  std::string value;

  // Constructor
  StringValueNode(std::string value, Location loc)
      : ValueNode(loc), value(value) {}

  // Function
  std::unique_ptr<ValueNode> clone();
};

class VariableValueNode : public ValueNode {
public:
  // Variable
  std::string value;

  // Constructor
  VariableValueNode(std::string value, Location loc)
      : ValueNode(loc), value(value) {}

  // Function
  std::unique_ptr<ValueNode> clone();
};

class ExpressionNode : public ASTNode {
public:
  // Variable
  ExpressionKind kind;
  std::unique_ptr<ValueNode> value;
  std::vector<std::unique_ptr<ExpressionNode>> args;
  ExpOpType op;

  // Constructor
  ExpressionNode(std::unique_ptr<ValueNode> value, Location loc)
      : kind(EXPRESSION_KIND_VALUE), value(std::move(value)),
        op(EXP_OP_TYPE_VOID), ASTNode(loc) {
    refreshTrace();
  }
  ExpressionNode(std::unique_ptr<ExpressionNode> lhs,
                 std::unique_ptr<ExpressionNode> rhs, ExpOpType op,
                 Location loc)
      : ASTNode(loc), kind(EXPRESSION_KIND_EXPRESSION), op(op) {
    args.push_back(std::move(lhs));
    args.push_back(std::move(rhs));
    refreshTrace();
  }
  ExpressionNode(std::vector<std::unique_ptr<ExpressionNode>> args,
                 ExpOpType op, Location loc)
      : ASTNode(loc), kind(EXPRESSION_KIND_INTRINSIC),
        args(std::move(args)), op(op) {
    refreshTrace();
  }

  // Function
  std::unique_ptr<ExpressionNode> clone();
  void appendCallFrame(const std::string &symbol, const Location &callSite);
  void appendCallFrames(const std::vector<CallFrame> &frames);
  void refreshTrace() override;
  bool propagateExp(ExpressionMap &) override;
  bool foldValue() override;
  bool hasUnresolvedValue(SymbolSet except = {}) override;
};

//------------ operator<< ------------//
inline std::ostream &operator<<(std::ostream &os, FunDefType type);
inline std::ostream &operator<<(std::ostream &os, ExpOpType type);
inline std::ostream &operator<<(std::ostream &os,
                                std::unique_ptr<ValueNode> valueNode);
inline std::ostream &operator<<(std::ostream &os,
                                const ExpressionNode &expNode);

inline std::ostream &operator<<(std::ostream &os, FunDefType type) {
  switch (type) {
  case FUN_DEF_TYPE_ACTIONS:
    os << "actions";
    break;
  case FUN_DEF_TYPE_CHECKS:
    os << "checks";
    break;
  case FUN_DEF_TYPE_TRIGGERS:
    os << "triggers";
    break;
  default:
    os << "invalid";
    break;
  }
  return os;
}

inline std::ostream &operator<<(std::ostream &os, ExpOpType type) {
  switch (type) {
  case EXP_OP_TYPE_AND:
    os << "&&";
    break;
  case EXP_OP_TYPE_OR:
    os << "||";
    break;
  case EXP_OP_TYPE_EQUAL:
    os << "==";
    break;
  case EXP_OP_TYPE_NOT_EQUAL:
    os << "!=";
    break;
  case EXP_OP_TYPE_LESS_THAN:
    os << "<";
    break;
  case EXP_OP_TYPE_LESS_THAN_EQUAL:
    os << "<=";
    break;
  case EXP_OP_TYPE_GREATER_THAN:
    os << ">";
    break;
  case EXP_OP_TYPE_GREATER_THAN_EQUAL:
    os << ">=";
    break;
  case EXP_OP_TYPE_ADD:
    os << "+";
    break;
  case EXP_OP_TYPE_SUB:
    os << "-";
    break;
  case EXP_OP_TYPE_MUL:
    os << "*";
    break;
  case EXP_OP_TYPE_DIV:
    os << "/";
    break;
  case EXP_OP_TYPE_MOD:
    os << "%";
    break;
  case EXP_OP_TYPE_INTRINSIC_TO_STRING:
    os << "twge::to_string";
    break;
  case EXP_OP_TYPE_INTRINSIC_TO_INT:
    os << "twge::to_int";
    break;
  case EXP_OP_TYPE_INTRINSIC_TO_BOOL:
    os << "twge::to_bool";
    break;
  case EXP_OP_TYPE_INTRINSIC_GET_INDEX:
    os << "twge::get_index";
    break;
  case EXP_OP_TYPE_INTRINSIC_GET_LENGTH:
    os << "twge::get_length";
    break;
  case EXP_OP_TYPE_INTRINSIC_GET_SLICE:
    os << "twge::get_slice";
    break;
  default:
    os << "";
    break;
  }
  return os;
}

inline std::ostream &printValueNode(std::ostream &os, const ValueNode *valueNode) {
  if (!valueNode)
    return os;
  if (auto stringNode = dynamic_cast<const StringValueNode *>(valueNode)) {
    os << "\"" << stringNode->value << "\"";
  } else if (auto intNode = dynamic_cast<const IntValueNode *>(valueNode)) {
    os << intNode->value;
  } else if (auto boolNode = dynamic_cast<const BoolValueNode *>(valueNode)) {
    os << (boolNode->value ? "true" : "false");
  } else if (auto varNode = dynamic_cast<const VariableValueNode *>(valueNode)) {
    os << varNode->value;
  } else if (auto ptNode = dynamic_cast<const PointValueNode *>(valueNode)) {
    os << "Point(" << *ptNode->x << "," << *ptNode->y << ")";
  } else if (auto actorMatchNode =
                 dynamic_cast<const ActorMatchValueNode *>(valueNode)) {
    os << "ActorMatch(";
    for (size_t i = 0; i < actorMatchNode->paramApps->named_args.size(); i++) {
      os << actorMatchNode->paramApps->named_args[i]->key << " = "
         << *actorMatchNode->paramApps->named_args[i]->expNode;
      if (i != actorMatchNode->paramApps->named_args.size() - 1)
        os << ", ";
    }
    os << ")";
  } else if (auto buttonNode =
                 dynamic_cast<const ButtonValueNode *>(valueNode)) {
    os << "Button(";
    for (size_t i = 0; i < buttonNode->paramApps->named_args.size(); i++) {
      os << buttonNode->paramApps->named_args[i]->key << " = "
         << *buttonNode->paramApps->named_args[i]->expNode;
      if (i != buttonNode->paramApps->named_args.size() - 1)
        os << ", ";
    }
    os << ")";
  } else if (auto customWeaponNode =
                 dynamic_cast<const CustomWeaponValueNode *>(valueNode)) {
    os << "CustomWeapon(";
    for (size_t i = 0; i < customWeaponNode->paramApps->named_args.size(); i++) {
      os << customWeaponNode->paramApps->named_args[i]->key << " = "
         << *customWeaponNode->paramApps->named_args[i]->expNode;
      if (i != customWeaponNode->paramApps->named_args.size() - 1)
        os << ", ";
    }
    os << ")";
  } else if (auto listNode = dynamic_cast<const ListValueNode *>(valueNode)) {
    os << "[";
    for (size_t i = 0; i < listNode->items.size(); i++) {
      os << *(listNode->items[i]);
      if (i != listNode->items.size() - 1)
        os << ",";
    }
    os << "]";
  }
  return os;
}

inline std::ostream &operator<<(std::ostream &os,
                                std::unique_ptr<ValueNode> valueNode) {
  return printValueNode(os, valueNode.get());
}

inline std::ostream &operator<<(std::ostream &os,
                                std::shared_ptr<ValueNode> valueNode) {
  return printValueNode(os, valueNode.get());
}

inline std::ostream &operator<<(std::ostream &os,
                                const ExpressionNode &expNode) {
  if (expNode.kind == EXPRESSION_KIND_VALUE)
    os << expNode.value->clone();
  else if (expNode.kind == EXPRESSION_KIND_EXPRESSION) {
    os << "(" << *expNode.args[0].get() << " " << expNode.op << " "
       << *expNode.args[1].get() << ")";
  } else {
    os << expNode.op << "(";
    for (size_t i = 0; i < expNode.args.size(); ++i) {
      os << *expNode.args[i].get();
      if (i < expNode.args.size() - 1)
        os << ", ";
    }
    os << ")";
  }
  return os;
}

#endif