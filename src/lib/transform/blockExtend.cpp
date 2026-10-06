#include "ast.h"
#include "transform.h"
#include "transform/utils/stackTrace.h"
#include <map>
#include <memory>
#include <set>

namespace transform {

using std::map;
using std::set;
using std::string;
using std::unique_ptr;

namespace {

bool bindInheritanceArgs(const unique_ptr<FunDefNode> &childFunDef,
                         const unique_ptr<FunDefNode> &baseFunDef,
                         const unique_ptr<InstructionNode> &inheritanceCall,
                         map<string, unique_ptr<ExpressionNode>> &boundArgs) {
  const auto &baseParams = baseFunDef->params;
  const auto &positionalArgs = inheritanceCall->paramApps->positional_args;
  const auto &namedArgs = inheritanceCall->paramApps->named_args;

  if (positionalArgs.size() > baseParams.size()) {
    std::cerr << "Syntax Error: Too many positional arguments when extending `"
              << childFunDef->identifier << "` at " << inheritanceCall->loc
              << ". Base function `" << baseFunDef->identifier
              << "` defines only " << baseParams.size() << " parameters.\n";
    return false;
  }

  set<string> baseParamSet;
  for (const auto &param : baseParams)
    baseParamSet.insert(param);

  set<string> seenArgKeys;
  for (size_t i = 0; i < positionalArgs.size(); i++) {
    const string &paramName = baseParams[i];
    auto clonedExp = positionalArgs[i]->expNode->clone();
    seenArgKeys.insert(paramName);
    boundArgs.insert({paramName, std::move(clonedExp)});
  }

  for (const auto &namedArg : namedArgs) {
    const string &argName = namedArg->key;
    if (baseParamSet.count(argName) == 0) {
      std::cerr << "Syntax Error: Unknown inherited argument `" << argName
                << "` found when extending `" << childFunDef->identifier
                << "` at " << namedArg->loc << ". Base function `"
                << baseFunDef->identifier << "` does not define this "
                << "parameter.\n";
      return false;
    }
    if (seenArgKeys.count(argName) != 0) {
      std::cerr << "Syntax Error: Duplicated inherited argument `" << argName
                << "` found when extending `" << childFunDef->identifier
                << "` at " << namedArg->loc << ".\n";
      return false;
    }
    auto clonedExp = namedArg->expNode->clone();
    seenArgKeys.insert(argName);
    boundArgs.insert({argName, std::move(clonedExp)});
  }

  if (seenArgKeys.size() != baseParams.size()) {
    std::cerr << "Syntax Error: Unmatched number of arguments when extending `"
              << childFunDef->identifier << "`. Base function `"
              << baseFunDef->identifier << "` requires " << baseParams.size()
              << " arguments, but only " << seenArgKeys.size()
              << " were provided at " << inheritanceCall->loc << ".\n";
    return false;
  }

  return true;
}

} // namespace

bool blockExtend(const unique_ptr<ModuleNode> &moduleNode, PassConfig config) {
  (void)config;
  map<string, unique_ptr<FunDefNode>> initializedFunDefs;

  for (auto &funDef : moduleNode->funDefs) {
    if (!funDef->inheritanceCall) {
      initializedFunDefs.insert({funDef->identifier, funDef->clone()});
      continue;
    }

    const string &baseIdentifier = funDef->inheritanceCall->identifier;
    if (initializedFunDefs.count(baseIdentifier) == 0) {
      std::cerr << "Compilation Error: Function `" << baseIdentifier
                << "` must be defined before inherited by `"
                << funDef->identifier << "` at " << funDef->loc << ".\n";
      return false;
    }

    auto &baseFunDef = initializedFunDefs.at(baseIdentifier);
    if (!baseFunDef->blockBody) {
      std::cerr << "Syntax Error: `" << funDef->identifier
                << "` expects a block-typed base function, but `"
                << baseIdentifier << "` is not block-typed at " << funDef->loc
                << ".\n";
      return false;
    }

    map<string, unique_ptr<ExpressionNode>> boundArgs;
    if (!bindInheritanceArgs(funDef, baseFunDef, funDef->inheritanceCall,
                             boundArgs))
      return false;

    funDef->blockBody = baseFunDef->blockBody->clone();
    funDef->blockBody->propagateExp(boundArgs);
    stacktrace::prependCallFrameToBlockBody(funDef->blockBody, baseIdentifier,
                                            funDef->inheritanceCall->loc);

    funDef->inheritanceCall = nullptr;
    initializedFunDefs.insert({funDef->identifier, funDef->clone()});
  }

  return true;
}
} // namespace transform