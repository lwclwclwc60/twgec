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

PropagationResult
ifStatementPropagation(std::unique_ptr<InstrSetNode> &instrSet) {
  std::vector<int> branchIdxes;
  auto &compositeInstrs = instrSet->instructions;
  // Step 1: Get the branch node index
  for (int idx = 0; idx < compositeInstrs.size(); idx++)
    if (compositeInstrs[idx]->branchNode)
      branchIdxes.push_back(idx);
  if (branchIdxes.empty())
    return PropagationResult::UNCHANGED;
  std::reverse(branchIdxes.begin(), branchIdxes.end());
  for (int idx : branchIdxes) {
    auto &branchNode = compositeInstrs[idx]->branchNode;
    auto pos = compositeInstrs.begin() + idx;
    // Step 2: Get the region to be inserted
    std::unique_ptr<InstrSetNode> regionToInsert = nullptr;
    for (auto &ifRegion : branchNode->ifRegions) {
      if (ifRegion->condition->kind != EXPRESSION_KIND_VALUE) {
        std::cerr << "Syntax Error: Value is not propagated in the if "
                     "statment condition. Found at "
                  << ifRegion->condition->loc << ".\n";
        return PropagationResult::ERROR;
      }
      if (!dynamic_cast<BoolValueNode *>(ifRegion->condition->value.get())) {
        std::cerr << "Syntax Error: Boolean value is expected in the if "
                     "statment condition. Found at "
                  << ifRegion->loc << ".\n";
        return PropagationResult::ERROR;
      }
      bool ifTrue =
          dynamic_cast<BoolValueNode *>(ifRegion->condition->value.get())
              ->value;
      if (ifTrue) {
        regionToInsert = ifRegion->region->clone();
        break;
      }
    }
    if (!regionToInsert && branchNode->elseRegion)
      regionToInsert = branchNode->elseRegion->clone();
    // Step 3: Flatten the branch node
    compositeInstrs.erase(pos);
    if (regionToInsert)
      compositeInstrs.insert(
          pos, std::make_move_iterator(regionToInsert->instructions.begin()),
          std::make_move_iterator(regionToInsert->instructions.end()));
  }
  return PropagationResult::PROPAGATED;
}

bool ifStatementPropagation(const unique_ptr<ModuleNode> &moduleNode,
                            PassConfig config) {
  PropagationResult result = PropagationResult::PROPAGATED;
  while (result == PropagationResult::PROPAGATED) {
    result = PropagationResult::UNCHANGED;
    for (auto &blockNode : moduleNode->blocks)
      for (auto &typedInstrSet : blockNode->blockBody->typedInstrSets) {
        auto blockResult = ifStatementPropagation(typedInstrSet->instrSet);
        if (blockResult == PropagationResult::ERROR)
          return false;
        if (blockResult == PropagationResult::PROPAGATED)
          result = PropagationResult::PROPAGATED;
      }
  }
  return true;
}
} // namespace transform