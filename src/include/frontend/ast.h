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
};

enum ExpressionKind {
  EXPRESSION_KIND_VALUE,
  EXPRESSION_KIND_INTRINSIC,
  EXPRESSION_KIND_EXPRESSION,
};

//------------ Global level definition ------------//
class ModuleNode {
public:
  // Variable
  Location loc;
  std::vector<std::unique_ptr<MetadataNode>> metadatas;
  std::vector<std::unique_ptr<BlockNode>> blocks;
  std::vector<std::unique_ptr<FunDefNode>> funDefs;
  std::vector<std::unique_ptr<ConstDefNode>> constDefs;

  // Constructor
  ModuleNode(Location loc) : loc(loc) {}

  // Function
  void print(std::string title, int indent = 0);
  bool propagateExp(std::map<std::string, std::unique_ptr<ExpressionNode>> &);
  bool foldValue();
  bool hasUnresolvedValue(std::set<std::string> except = {});
};

class MetadataNode {
public:
  // Variable
  Location loc;
  std::string key;
  std::unique_ptr<ExpressionNode> expNode;

  // Constructor
  MetadataNode(std::string key, std::unique_ptr<ExpressionNode> expNode,
               Location loc)
      : key(key), expNode(std::move(expNode)), loc(loc) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<MetadataNode> clone();
  bool propagateExp(std::map<std::string, std::unique_ptr<ExpressionNode>> &);
  bool foldValue();
  bool hasUnresolvedValue(std::set<std::string> except = {});
};

class FunDefNode {
public:
  // Variable
  Location loc;
  std::string identifier;
  std::vector<std::string> params;
  std::unique_ptr<TypedInstrSetNode> typedInstrSet;
  std::unique_ptr<BlockBodyNode> blockBody;

  // Constructor
  FunDefNode(const std::string &id, Location loc) : identifier(id), loc(loc) {}
  FunDefNode(const std::string &id, Location loc,
             std::unique_ptr<TypedInstrSetNode> typedInstrSet)
      : identifier(id), loc(loc), typedInstrSet(std::move(typedInstrSet)) {}
  FunDefNode(const std::string &id, Location loc,
             std::unique_ptr<BlockBodyNode> blockBody)
      : identifier(id), loc(loc), blockBody(std::move(blockBody)) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<FunDefNode> clone();
};

class BlockNode {
public:
  // Variable
  Location loc;
  std::string identifier;
  std::unique_ptr<BlockBodyNode> blockBody;
  std::unique_ptr<InstructionNode> blockConstructor;

  // Constructor
  BlockNode(const std::string &id, Location loc) : identifier(id), loc(loc) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<BlockNode> clone();
  bool propagateExp(std::map<std::string, std::unique_ptr<ExpressionNode>> &);
  bool foldValue();
  bool hasUnresolvedValue(std::set<std::string> except = {});
};

class BlockBodyNode {
public:
  // Variable
  Location loc;
  std::vector<std::unique_ptr<MetadataNode>> metadatas;
  std::vector<std::unique_ptr<TypedInstrSetNode>> typedInstrSets;

  // Constructor
  BlockBodyNode(Location loc) : loc(loc) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<BlockBodyNode> clone();
  bool propagateExp(std::map<std::string, std::unique_ptr<ExpressionNode>> &);
  bool foldValue();
  bool hasUnresolvedValue(std::set<std::string> except = {});
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

class ConstDefNode {
public:
  // Variable
  Location loc;
  std::string key;
  std::unique_ptr<ExpressionNode> expNode;

  // Constructor
  ConstDefNode(std::string key, std::unique_ptr<ExpressionNode> expNode,
               Location loc)
      : key(key), expNode(std::move(expNode)), loc(loc) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<ConstDefNode> clone();
};

//------------ Instruction Set level definition ------------//
class TypedInstrSetNode {
public:
  // Variable
  Location loc;
  FunDefType type;
  std::unique_ptr<InstrSetNode> instrSet;

  // Constructor
  TypedInstrSetNode(Location loc, FunDefType type = FUN_DEF_TYPE_INVALID)
      : loc(loc), type(type), instrSet(std::make_unique<InstrSetNode>(loc)) {}
  TypedInstrSetNode(Location loc, FunDefType type,
                    std::unique_ptr<InstrSetNode> instrSet)
      : loc(loc), type(type), instrSet(std::move(instrSet)) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<TypedInstrSetNode> clone();
  bool propagateExp(std::map<std::string, std::unique_ptr<ExpressionNode>> &);
  bool foldValue();
  bool hasUnresolvedValue(std::set<std::string> except = {});
};

class InstrSetNode {
public:
  // Variable
  Location loc;
  std::vector<std::unique_ptr<CompositeInstrNode>> instructions;

  // Constructor
  InstrSetNode(Location loc) : loc(loc) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<InstrSetNode> clone();
  bool propagateExp(std::map<std::string, std::unique_ptr<ExpressionNode>> &);
  bool foldValue();
  bool hasUnresolvedValue(std::set<std::string> except = {});
};

class CompositeInstrNode {
public:
  // Variable
  Location loc;
  std::unique_ptr<InstructionNode> instruction;
  std::unique_ptr<BranchNode> branchNode;
  std::unique_ptr<ForNode> forNode;

  // Constructor
  CompositeInstrNode(Location loc) : loc(loc) {}
  CompositeInstrNode(Location loc, std::unique_ptr<InstructionNode> instruction)
      : loc(loc), instruction(std::move(instruction)) {}
  CompositeInstrNode(Location loc, std::unique_ptr<BranchNode> branchNode)
      : loc(loc), branchNode(std::move(branchNode)) {}
  CompositeInstrNode(Location loc, std::unique_ptr<ForNode> forNode)
      : loc(loc), forNode(std::move(forNode)) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<CompositeInstrNode> clone();
  bool propagateExp(std::map<std::string, std::unique_ptr<ExpressionNode>> &);
  bool foldValue();
  bool hasUnresolvedValue(std::set<std::string> except = {});
};

class BranchNode {
public:
  // Variable
  Location loc;
  std::vector<std::unique_ptr<IfRegionNode>> ifRegions;
  std::unique_ptr<InstrSetNode> elseRegion;

  // Constructor
  BranchNode(Location loc) : loc(loc) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<BranchNode> clone();
  bool propagateExp(std::map<std::string, std::unique_ptr<ExpressionNode>> &);
  bool foldValue();
  bool hasUnresolvedValue(std::set<std::string> except = {});
};

class ForNode {
public:
  // Variable
  Location loc;
  std::string iterArg;
  std::unique_ptr<ExpressionNode> listExp;
  std::unique_ptr<ExpressionNode> fromExp;
  std::unique_ptr<ExpressionNode> toExp;
  std::unique_ptr<InstrSetNode> region;

  // Range Semantics Constructor `for i in exp...exp`
  ForNode(std::string iterArg, std::unique_ptr<ExpressionNode> fromExp,
          std::unique_ptr<ExpressionNode> toExp,
          std::unique_ptr<InstrSetNode> region, Location loc)
      : loc(loc), iterArg(iterArg), fromExp(std::move(fromExp)),
        toExp(std::move(toExp)), region(std::move(region)) {}
  // List Semantics Constructor `for i in list`
  ForNode(std::string iterArg, std::unique_ptr<ExpressionNode> listExp,
          std::unique_ptr<InstrSetNode> region, Location loc)
      : loc(loc), iterArg(iterArg), listExp(std::move(listExp)),
        region(std::move(region)) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<ForNode> clone();
  bool propagateExp(std::map<std::string, std::unique_ptr<ExpressionNode>> &);
  bool foldValue();
  bool hasUnresolvedValue(std::set<std::string> except = {});
};

class IfRegionNode {
public:
  // Variable
  Location loc;
  std::unique_ptr<ExpressionNode> condition;
  std::unique_ptr<InstrSetNode> region;

  // Constructor
  IfRegionNode(std::unique_ptr<ExpressionNode> condition,
               std::unique_ptr<InstrSetNode> region, Location loc)
      : condition(std::move(condition)), region(std::move(region)), loc(loc) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<IfRegionNode> clone();
  bool propagateExp(std::map<std::string, std::unique_ptr<ExpressionNode>> &);
  bool foldValue();
  bool hasUnresolvedValue(std::set<std::string> except = {});
};

//------------ Instruction level definition ------------//
class InstructionNode {
public:
  // Variable
  Location loc;
  std::string identifier;
  std::unique_ptr<ParamAppsNode> paramApps;

  // Constructor
  InstructionNode(std::string identifier,
                  std::unique_ptr<ParamAppsNode> paramApps, Location loc)
      : identifier(identifier), paramApps(std::move(paramApps)), loc(loc) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<InstructionNode> clone();
  bool propagateExp(std::map<std::string, std::unique_ptr<ExpressionNode>> &);
  bool foldValue();
  bool hasUnresolvedValue(std::set<std::string> except = {});
};

class ParamAppsNode {
public:
  // Variable
  Location loc;
  std::vector<std::unique_ptr<PositionalParamAppsNode>> positional_args;
  std::vector<std::unique_ptr<NamedParamAppsNode>> named_args;

  // Constructor
  ParamAppsNode(Location loc) : loc(loc) {}

  // Function
  void print(int indent = 0);
  std::unique_ptr<ParamAppsNode> clone();
  void addNamedArg(std::unique_ptr<NamedParamAppsNode> namedArg);
  void addPositionalArg(std::unique_ptr<PositionalParamAppsNode> positionalArg);
  void refreshTrace();
  bool propagateExp(std::map<std::string, std::unique_ptr<ExpressionNode>> &);
  bool foldValue();
  bool hasUnresolvedValue(std::set<std::string> except = {});
};

class NamedParamAppsNode {
public:
  // Variable
  Location loc;
  std::string key;
  std::unique_ptr<ExpressionNode> expNode;

  // Constructor
  NamedParamAppsNode(const std::string &key,
                     std::unique_ptr<ExpressionNode> &expNode, Location loc)
      : key(key), expNode(std::move(expNode)), loc(loc) {
    refreshTrace();
  }

  // Function
  std::unique_ptr<NamedParamAppsNode> clone();
  void refreshTrace();
  bool propagateExp(std::map<std::string, std::unique_ptr<ExpressionNode>> &);
  bool foldValue();
  bool hasUnresolvedValue(std::set<std::string> except = {});
};

class PositionalParamAppsNode {
public:
  // Variable
  Location loc;
  std::unique_ptr<ExpressionNode> expNode;

  // Constructor
  PositionalParamAppsNode(std::unique_ptr<ExpressionNode> &expNode,
                          Location loc)
      : expNode(std::move(expNode)), loc(loc) {
    refreshTrace();
  }

  // Function
  std::unique_ptr<PositionalParamAppsNode> clone();
  void refreshTrace();
  bool propagateExp(std::map<std::string, std::unique_ptr<ExpressionNode>> &);
  bool foldValue();
  bool hasUnresolvedValue(std::set<std::string> except = {});
};

//------------ Expression level definition ------------//
class ValueNode {
public:
  // Variable
  Location loc;

  // Constructor
  ValueNode(Location loc) : loc(loc) {}

  // Function
  virtual std::unique_ptr<ValueNode> clone();
  virtual void refreshTrace() {}
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
  void refreshTrace();
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
  void refreshTrace();
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
  void refreshTrace();
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
  void refreshTrace();
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
  void refreshTrace();
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

class ExpressionNode {
public:
  // Variable
  ExpressionKind kind;
  std::unique_ptr<ValueNode> value;
  std::vector<std::unique_ptr<ExpressionNode>> args;
  ExpOpType op;
  Location loc;

  // Constructor
  ExpressionNode(std::unique_ptr<ValueNode> value, Location loc)
      : kind(EXPRESSION_KIND_VALUE), value(std::move(value)),
        op(EXP_OP_TYPE_VOID), loc(loc) {
    refreshTrace();
  }
  ExpressionNode(std::unique_ptr<ExpressionNode> lhs,
                 std::unique_ptr<ExpressionNode> rhs, ExpOpType op,
                 Location loc)
      : kind(EXPRESSION_KIND_EXPRESSION), op(op), loc(loc) {
    args.push_back(std::move(lhs));
    args.push_back(std::move(rhs));
    refreshTrace();
  }
  ExpressionNode(std::vector<std::unique_ptr<ExpressionNode>> args,
                 ExpOpType op, Location loc)
      : kind(EXPRESSION_KIND_INTRINSIC), args(std::move(args)), op(op),
        loc(loc) {
    refreshTrace();
  }

  // Function
  std::unique_ptr<ExpressionNode> clone();
  void appendCallFrame(const std::string &symbol, const Location &callSite);
  void appendCallFrames(const std::vector<CallFrame> &frames);
  void refreshTrace();
  bool propagateExp(std::map<std::string, std::unique_ptr<ExpressionNode>> &);
  bool foldValue();
  bool hasUnresolvedValue(std::set<std::string> except = {});
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
  default:
    os << "";
    break;
  }
  return os;
}

inline std::ostream &operator<<(std::ostream &os,
                                std::unique_ptr<ValueNode> valueNode) {
  if (auto stringNode = dynamic_cast<StringValueNode *>(valueNode.get())) {
    os << "\"" << stringNode->value << "\"";
  } else if (auto intNode = dynamic_cast<IntValueNode *>(valueNode.get())) {
    os << intNode->value;
  } else if (auto boolNode = dynamic_cast<BoolValueNode *>(valueNode.get())) {
    os << (boolNode->value ? "true" : "false");
  } else if (auto varNode =
                 dynamic_cast<VariableValueNode *>(valueNode.get())) {
    os << varNode->value;
  } else if (auto ptNode = dynamic_cast<PointValueNode *>(valueNode.get())) {
    os << "Point(" << *(ptNode->x.get()) << "," << *(ptNode->y.get()) << ")";
  } else if (auto actorMatchNode =
                 dynamic_cast<ActorMatchValueNode *>(valueNode.get())) {
    os << "ActorMatch(";
    for (auto i = 0; i < actorMatchNode->paramApps->named_args.size(); i++) {
      os << actorMatchNode->paramApps->named_args[i]->key << " = "
         << *actorMatchNode->paramApps->named_args[i]->expNode.get();
      if (i != actorMatchNode->paramApps->named_args.size() - 1)
        os << ", ";
    }
    os << ")";
  } else if (auto buttonNode =
                 dynamic_cast<ButtonValueNode *>(valueNode.get())) {
    os << "Button(";
    for (auto i = 0; i < buttonNode->paramApps->named_args.size(); i++) {
      os << buttonNode->paramApps->named_args[i]->key << " = "
         << *buttonNode->paramApps->named_args[i]->expNode.get();
      if (i != buttonNode->paramApps->named_args.size() - 1)
        os << ", ";
    }
    os << ")";
  } else if (auto customWeaponNode =
                 dynamic_cast<CustomWeaponValueNode *>(valueNode.get())) {
    os << "CustomWeapon(";
    for (auto i = 0; i < customWeaponNode->paramApps->named_args.size(); i++) {
      os << customWeaponNode->paramApps->named_args[i]->key << " = "
         << *customWeaponNode->paramApps->named_args[i]->expNode.get();
      if (i != customWeaponNode->paramApps->named_args.size() - 1)
        os << ", ";
    }
    os << ")";
  } else if (auto listNode = dynamic_cast<ListValueNode *>(valueNode.get())) {
    os << "[";
    for (auto i = 0; i < listNode->items.size(); i++) {
      os << *(listNode->items[i]->clone().get());
      if (i != listNode->items.size() - 1)
        os << ",";
    }
    os << "]";
  }
  return os;
}

// TODO: Unify it with the unique_ptr
inline std::ostream &operator<<(std::ostream &os,
                                std::shared_ptr<ValueNode> valueNode) {
  if (auto stringNode = dynamic_cast<StringValueNode *>(valueNode.get())) {
    os << "\"" << stringNode->value << "\"";
  } else if (auto intNode = dynamic_cast<IntValueNode *>(valueNode.get())) {
    os << intNode->value;
  } else if (auto boolNode = dynamic_cast<BoolValueNode *>(valueNode.get())) {
    os << (boolNode->value ? "true" : "false");
  } else if (auto varNode =
                 dynamic_cast<VariableValueNode *>(valueNode.get())) {
    os << varNode->value;
  } else if (auto ptNode = dynamic_cast<PointValueNode *>(valueNode.get())) {
    os << "Point(" << *(ptNode->x.get()) << "," << *(ptNode->y.get()) << ")";
  } else if (auto actorMatchNode =
                 dynamic_cast<ActorMatchValueNode *>(valueNode.get())) {
    os << "ActorMatch(";
    for (auto i = 0; i < actorMatchNode->paramApps->named_args.size(); i++) {
      os << actorMatchNode->paramApps->named_args[i]->key << " = "
         << *actorMatchNode->paramApps->named_args[i]->expNode.get();
      if (i != actorMatchNode->paramApps->named_args.size() - 1)
        os << ", ";
    }
    os << ")";
  } else if (auto buttonNode =
                 dynamic_cast<ButtonValueNode *>(valueNode.get())) {
    os << "Button(";
    for (auto i = 0; i < buttonNode->paramApps->named_args.size(); i++) {
      os << buttonNode->paramApps->named_args[i]->key << " = "
         << *buttonNode->paramApps->named_args[i]->expNode.get();
      if (i != buttonNode->paramApps->named_args.size() - 1)
        os << ", ";
    }
    os << ")";
  } else if (auto customWeaponNode =
                 dynamic_cast<CustomWeaponValueNode *>(valueNode.get())) {
    os << "CustomWeapon(";
    for (auto i = 0; i < customWeaponNode->paramApps->named_args.size(); i++) {
      os << customWeaponNode->paramApps->named_args[i]->key << " = "
         << *customWeaponNode->paramApps->named_args[i]->expNode.get();
      if (i != customWeaponNode->paramApps->named_args.size() - 1)
        os << ", ";
    }
    os << ")";
  } else if (auto listNode = dynamic_cast<ListValueNode *>(valueNode.get())) {
    os << "[";
    for (auto i = 0; i < listNode->items.size(); i++) {
      os << *(listNode->items[i]->clone().get());
      if (i != listNode->items.size() - 1)
        os << ",";
    }
    os << "]";
  }
  return os;
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