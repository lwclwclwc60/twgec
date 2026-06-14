#include "ast.h"
#include "keyword.h"
#include "transform.h"
namespace transform {

using std::string;
using std::unique_ptr;

bool implicitListPromotion(const unique_ptr<ModuleNode> &moduleNode,
                           PassConfig config) {
  for (auto &metadata : moduleNode->metadatas) {
    if ((metadata->key == keyword::metadataKind::royalLocs ||
         metadata->key == keyword::metadataKind::skydowLocs ||
         metadata->key == keyword::metadataKind::thirdLocs ||
         metadata->key == keyword::metadataKind::customWeapons) &&
        !dynamic_cast<ListValueNode *>(metadata->expNode->value.get())) {
      auto listNode =
          std::make_unique<ListValueNode>(metadata->expNode->value->loc);
      listNode->items.push_back(metadata->expNode->clone());
      listNode->refreshTrace();
      metadata->expNode->value = std::move(listNode);
      metadata->expNode->refreshTrace();
    }
  }
  return true;
}
} // namespace transform