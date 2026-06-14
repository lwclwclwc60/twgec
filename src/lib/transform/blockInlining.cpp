#include "ast.h"
#include "transform.h"
#include <map>
#include <memory>
namespace transform {

using std::map;
using std::string;
using std::unique_ptr;

bool blockInling(const unique_ptr<ModuleNode> &moduleNode, PassConfig config) {
  std::map<std::string, std::unique_ptr<BlockBodyNode>> blockDefMap;
  for (auto &funDef : moduleNode->funDefs) {
    if (funDef->blockBody)
      blockDefMap.insert({funDef->identifier, funDef->blockBody->clone()});
  }
  for (auto &blockNode : moduleNode->blocks)
    if (blockNode->blockConstructor) {
      std::map<std::string, unique_ptr<ExpressionNode>> callerParamMap;
      for (auto &arg : blockNode->blockConstructor->paramApps->named_args) {
        arg->expNode->appendCallFrame(blockNode->blockConstructor->identifier,
                                      blockNode->blockConstructor->loc);
        callerParamMap.insert({arg->key, std::move(arg->expNode)});
      }
      blockNode->blockBody =
          blockDefMap.at(blockNode->blockConstructor->identifier)->clone();
      blockNode->blockBody->propagateExp(callerParamMap);
      blockNode->blockConstructor = nullptr;
    }
  return true;
}
} // namespace transform