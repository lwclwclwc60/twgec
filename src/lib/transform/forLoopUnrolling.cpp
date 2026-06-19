#include "ast.h"
#include "transform.h"
#include <algorithm>
#include <memory>
namespace transform {

using std::string;
using std::unique_ptr;

namespace {
enum PropagationResult { ERROR = 0, PROPAGATED = 1, UNCHANGED = 2 };
}

PropagationResult unrollingList(std::unique_ptr<ForNode> &forNode,
                                std::unique_ptr<InstrSetNode> &regionToInsert) {
  // Step 2: Verify the for loop
  if (forNode->listExp->kind != EXPRESSION_KIND_VALUE) {
    std::cerr << "Syntax Error: Value is not propagated in the for "
                 "statment list. Found at "
              << forNode->listExp->loc << ".\n";
    return PropagationResult::ERROR;
  }
  if (!dynamic_cast<ListValueNode *>(forNode->listExp->value.get()) &&
      !dynamic_cast<StringValueNode *>(forNode->listExp->value.get())) {
    std::cerr << "Syntax Error: list or string value is expected in the for "
                 "statment list. Found at "
              << forNode->listExp->loc << ".\n";
    return PropagationResult::ERROR;
  }
  // Step 3: Get the region to be inserted
  Location loc = forNode->loc;
  if (auto listValue =
          dynamic_cast<ListValueNode *>(forNode->listExp->value.get()))
    for (auto &item : listValue->items) {
      auto iterRegion = forNode->region->clone();
      std::map<std::string, std::unique_ptr<ExpressionNode>> constDefMap;
      constDefMap.insert({forNode->iterArg, item->clone()});
      iterRegion->propagateExp(constDefMap);
      iterRegion->foldValue();
      std::move(iterRegion->instructions.begin(),
                iterRegion->instructions.end(),
                std::back_inserter(regionToInsert->instructions));
    }
  // TODO: Be able to handle UTF-8
  else if (auto stringValue =
               dynamic_cast<StringValueNode *>(forNode->listExp->value.get()))
    for (size_t i = 0; i < stringValue->value.length(); i++) {
      auto iterRegion = forNode->region->clone();
      auto iterValue = std::make_unique<StringValueNode>(
          stringValue->value.substr(i, 1), loc);
      std::map<std::string, std::unique_ptr<ExpressionNode>> constDefMap;
      constDefMap.insert({forNode->iterArg,
                          ExpressionNode(std::move(iterValue), loc).clone()});
      iterRegion->propagateExp(constDefMap);
      iterRegion->foldValue();
      std::move(iterRegion->instructions.begin(),
                iterRegion->instructions.end(),
                std::back_inserter(regionToInsert->instructions));
    }
  return PropagationResult::PROPAGATED;
}

PropagationResult
unrollingRange(std::unique_ptr<ForNode> &forNode,
               std::unique_ptr<InstrSetNode> &regionToInsert) {
  // Step 2: Verify the for loop
  if (forNode->fromExp->kind != EXPRESSION_KIND_VALUE) {
    std::cerr << "Syntax Error: Value is not propagated in the if "
                 "statment range. Found at "
              << forNode->fromExp->loc << ".\n";
    return PropagationResult::ERROR;
  }
  if (!dynamic_cast<IntValueNode *>(forNode->fromExp->value.get())) {
    std::cerr << "Syntax Error: Integer value is expected in the if "
                 "statment range. Found at "
              << forNode->fromExp->loc << ".\n";
    return PropagationResult::ERROR;
  }
  if (forNode->toExp->kind != EXPRESSION_KIND_VALUE) {
    std::cerr << "Syntax Error: Value is not propagated in the if "
                 "statment range. Found at "
              << forNode->toExp->loc << ".\n";
    return PropagationResult::ERROR;
  }
  if (!dynamic_cast<IntValueNode *>(forNode->toExp->value.get())) {
    std::cerr << "Syntax Error: Integer value is expected in the if "
                 "statment range. Found at "
              << forNode->toExp->loc << ".\n";
    return PropagationResult::ERROR;
  }
  int fromValue =
      dynamic_cast<IntValueNode *>(forNode->fromExp->value.get())->value;
  int toValue =
      dynamic_cast<IntValueNode *>(forNode->toExp->value.get())->value;
  // Step 3: Get the region to be inserted
  Location loc = forNode->loc;
  int step = fromValue < toValue ? 1 : -1;
  int times = std::abs(fromValue - toValue) + 1;
  while (times > 0) {
    auto iterRegion = forNode->region->clone();
    auto iterValue = std::make_unique<IntValueNode>(fromValue, loc);
    std::map<std::string, std::unique_ptr<ExpressionNode>> constDefMap;
    constDefMap.insert(
        {forNode->iterArg, ExpressionNode(std::move(iterValue), loc).clone()});
    iterRegion->propagateExp(constDefMap);
    iterRegion->foldValue();
    fromValue += step;
    times--;
    std::move(iterRegion->instructions.begin(), iterRegion->instructions.end(),
              std::back_inserter(regionToInsert->instructions));
  }
  return PropagationResult::PROPAGATED;
}

PropagationResult forLoopUnrolling(std::unique_ptr<InstrSetNode> &instrSet) {
  std::vector<int> forIdxes;
  auto &compositeInstrs = instrSet->instructions;
  // Step 1: Get the for loop node index
  for (int idx = 0; idx < compositeInstrs.size(); idx++) {
    if (compositeInstrs[idx]->forNode)
      forIdxes.push_back(idx);
    if (auto &branchNode = compositeInstrs[idx]->branchNode) {
      for (auto &ifRegion : branchNode->ifRegions)
        if (PropagationResult::ERROR == forLoopUnrolling(ifRegion->region))
          return PropagationResult::ERROR;
      if (branchNode->elseRegion)
        if (PropagationResult::ERROR ==
            forLoopUnrolling(branchNode->elseRegion))
          return PropagationResult::ERROR;
    }
  }
  if (forIdxes.empty())
    return PropagationResult::UNCHANGED;
  std::reverse(forIdxes.begin(), forIdxes.end());
  for (int idx : forIdxes) {
    // Step 2: Verify the for loop
    auto &forNode = compositeInstrs[idx]->forNode;
    auto pos = compositeInstrs.begin() + idx;
    std::unique_ptr<InstrSetNode> regionToInsert =
        std::make_unique<InstrSetNode>(forNode->loc);
    if (forNode->fromExp && forNode->toExp)
      unrollingRange(forNode, regionToInsert);
    if (forNode->listExp)
      unrollingList(forNode, regionToInsert);
    // Step 4: Insert the regions
    compositeInstrs.erase(pos);
    compositeInstrs.insert(
        pos, std::make_move_iterator(regionToInsert->instructions.begin()),
        std::make_move_iterator(regionToInsert->instructions.end()));
  }
  return PropagationResult::PROPAGATED;
}

bool forLoopUnrolling(const unique_ptr<ModuleNode> &moduleNode,
                      PassConfig config) {
  PropagationResult result = PropagationResult::PROPAGATED;
  while (result == PropagationResult::PROPAGATED) {
    result = PropagationResult::UNCHANGED;
    for (auto &blockNode : moduleNode->blocks)
      for (auto &typedInstrSet : blockNode->blockBody->typedInstrSets) {
        auto blockResult = forLoopUnrolling(typedInstrSet->instrSet);
        if (blockResult == PropagationResult::ERROR)
          return false;
        if (blockResult == PropagationResult::PROPAGATED)
          result = PropagationResult::PROPAGATED;
      }
  }
  return true;
}
} // namespace transform