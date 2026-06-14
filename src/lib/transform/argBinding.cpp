#include "ast.h"
#include "transform.h"
#include <memory>
namespace transform {

using std::string;
using std::unique_ptr;

bool argBinding(std::map<std::string, std::unique_ptr<FunDefNode>> &funDefs,
                std::unique_ptr<InstructionNode> &instr) {
  if (funDefs.count(instr->identifier) == 0)
    return true;
  // Currently only the function can map positional arg into named arg
  auto funParam = funDefs.find(instr->identifier)->second->params;
  if (instr->paramApps->positional_args.size() > funParam.size()) {
    std::cerr << "Syntax Error: Too much positional arguments. Found at "
              << instr->loc << "\n";
    return false;
  }
  std::vector<std::unique_ptr<NamedParamAppsNode>> bindedArgs;
  for (int i = 0; i < instr->paramApps->positional_args.size(); i++) {
    auto &posArg = instr->paramApps->positional_args[i];
    bindedArgs.push_back(std::make_unique<NamedParamAppsNode>(
        funParam[i], posArg.get()->expNode, posArg->loc));
  }
  instr->paramApps->named_args.insert(
      instr->paramApps->named_args.begin(),
      std::make_move_iterator(bindedArgs.begin()),
      std::make_move_iterator(bindedArgs.end()));
  instr->paramApps->positional_args.clear();
  instr->paramApps->refreshTrace();
  if (instr->paramApps->named_args.size() != funParam.size()) {
    std::cerr << "Syntax Error: Unmatched number of arguments of function `"
              << instr->identifier << "`. Expected: " << funParam.size()
              << " arguments defined at "
              << funDefs.find(instr->identifier)->second->loc << ". Found "
              << instr->paramApps->named_args.size() << " arguments passed at "
              << instr->loc << ".\n";
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