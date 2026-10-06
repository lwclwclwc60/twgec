#ifndef TRANSFORM_UTILS_STACKTRACE_H
#define TRANSFORM_UTILS_STACKTRACE_H

#include "ast.h"
#include <memory>
#include <string>
#include <vector>

namespace transform::stacktrace {

// Stack trace propagation contract for lowering/inlining passes:
// 1) frame 0 is always the concrete failing instruction site.
// 2) caller frames are prepended from inner caller to outer caller, so
//    printing in reverse order yields the expected call chain.
// 3) helpers mutate only callFrames/trace-related fields, never semantics.
// 4) helpers are pass-agnostic and can be reused by any AST rewrite pass.

inline void prependFrame(std::vector<CallFrame> &frames,
                         const std::string &symbol, const Location &callSite) {
  if (!Location::isStackTraceEnabled())
    return;
  frames.insert(frames.begin(), CallFrame(symbol, callSite.filename,
                                          callSite.line, callSite.column));
}

inline void prependCallFrameToExpression(std::unique_ptr<ExpressionNode> &exp,
                                         const std::string &symbol,
                                         const Location &callSite) {
  if (!exp)
    return;
  prependFrame(exp->loc.callFrames, symbol, callSite);
  if (exp->value)
    prependFrame(exp->value->loc.callFrames, symbol, callSite);
  for (auto &arg : exp->args)
    prependCallFrameToExpression(arg, symbol, callSite);
  exp->refreshTrace();
}

inline void
prependCallFrameToInstruction(std::unique_ptr<InstructionNode> &instruction,
                              const std::string &symbol,
                              const Location &callSite) {
  if (!instruction || !instruction->paramApps)
    return;
  for (auto &arg : instruction->paramApps->named_args)
    prependCallFrameToExpression(arg->expNode, symbol, callSite);
  for (auto &arg : instruction->paramApps->positional_args)
    prependCallFrameToExpression(arg->expNode, symbol, callSite);
}

inline void prependCallFrameToInstrSet(std::unique_ptr<InstrSetNode> &instrSet,
                                       const std::string &symbol,
                                       const Location &callSite) {
  if (!instrSet)
    return;
  for (auto &composite : instrSet->instructions) {
    if (composite->instruction) {
      prependCallFrameToInstruction(composite->instruction, symbol, callSite);
      continue;
    }
    if (composite->branchNode) {
      for (auto &ifRegion : composite->branchNode->ifRegions) {
        prependCallFrameToExpression(ifRegion->condition, symbol, callSite);
        prependCallFrameToInstrSet(ifRegion->region, symbol, callSite);
      }
      prependCallFrameToInstrSet(composite->branchNode->elseRegion, symbol,
                                 callSite);
      continue;
    }
    if (composite->forNode) {
      prependCallFrameToExpression(composite->forNode->fromExp, symbol,
                                   callSite);
      prependCallFrameToExpression(composite->forNode->toExp, symbol, callSite);
      prependCallFrameToExpression(composite->forNode->listExp, symbol,
                                   callSite);
      prependCallFrameToInstrSet(composite->forNode->region, symbol, callSite);
    }
  }
}

inline void
prependCallFrameToBlockBody(std::unique_ptr<BlockBodyNode> &blockBody,
                            const std::string &symbol,
                            const Location &callSite) {
  if (!blockBody)
    return;
  for (auto &metadata : blockBody->metadatas)
    prependCallFrameToExpression(metadata->expNode, symbol, callSite);
  for (auto &typedInstrSet : blockBody->typedInstrSets)
    prependCallFrameToInstrSet(typedInstrSet->instrSet, symbol, callSite);
}

} // namespace transform::stacktrace

#endif
