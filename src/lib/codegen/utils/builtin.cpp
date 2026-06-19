#include "utils/builtin.h"
#include <string>

using namespace codegen;
using namespace keyword;
using namespace codegen::formatter;

namespace {
JsonObjectNode getButtonNode(std::unique_ptr<ExpressionNode> &expNode) {
  buttonDefaultMap.clearInputMap();
  auto buttonValueNode = dynamic_cast<ButtonValueNode *>(expNode->value.get());
  if (!buttonValueNode) {
    std::cerr << "Button-typed value is expected at " << expNode->loc << "\n";
    return JsonObjectNode();
  }
  buttonDefaultMap.addInputMap(buttonValueNode->paramApps->named_args);
  return JsonObjectNode({
      {"buttonCode", buttonDefaultMap.get("id")},
      {"close", "true"},
      {"label", buttonDefaultMap.get("text")},
      {"icon", "\"\""},
  });
}

JsonObjectNode getDialogButtonNode(std::unique_ptr<ExpressionNode> &expNode) {
  buttonDefaultMap.clearInputMap();
  auto buttonValueNode = dynamic_cast<ButtonValueNode *>(expNode->value.get());
  if (!buttonValueNode) {
    std::cerr << "Button-typed value is expected at " << expNode->loc << "\n";
    return JsonObjectNode();
  }
  buttonDefaultMap.addInputMap(buttonValueNode->paramApps->named_args);
  return JsonObjectNode({
      {"value", buttonDefaultMap.get("id")},
      {"label", buttonDefaultMap.get("text")},
      {"icon", "\"\""},
  });
}

JsonObjectNode getCustomWeaponsNode(std::unique_ptr<ExpressionNode> &expNode) {
  customWeaponDefaultMap.clearInputMap();
  auto customWeaponValueNode =
      dynamic_cast<CustomWeaponValueNode *>(expNode->value.get());
  if (!customWeaponValueNode) {
    std::cerr << "CustomWeapon-typed value is expected at " << expNode->loc
              << "\n";
    return JsonObjectNode();
  }
  customWeaponDefaultMap.addInputMap(
      customWeaponValueNode->paramApps->named_args);
  JsonObjectNode fireNode = JsonObjectNode({
      {"fireType", customWeaponDefaultMap.get(
                       "fireType", CustomWeaponAttackKind::keywordEnum)},
      {"deltaDamage", "0"},
  });
  JsonArrayNode firesNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(fireNode));
  JsonObjectNode pivotOnHandScaleNode = JsonObjectNode({
      {"x", customWeaponDefaultMap.get("pivotOnHandXScale")},
      {"y", customWeaponDefaultMap.get("pivotOnHandYScale")},
  });
  JsonObjectNode pivotOnHandNode = JsonObjectNode({
      {"x", customWeaponDefaultMap.get("pivotOnHandX")},
      {"y", customWeaponDefaultMap.get("pivotOnHandY")},
      {"degrees", customWeaponDefaultMap.get("pivotOnHandDegree")},
      {"scale", pivotOnHandScaleNode.to_string(40)},
  });
  JsonObjectNode pivotOnIconScaleNode = JsonObjectNode({
      {"x", customWeaponDefaultMap.get("pivotOnIconXScale")},
      {"y", customWeaponDefaultMap.get("pivotOnIconYScale")},
  });
  JsonObjectNode pivotOnIconNode = JsonObjectNode({
      {"x", customWeaponDefaultMap.get("pivotOnIconX")},
      {"y", customWeaponDefaultMap.get("pivotOnIconY")},
      {"degrees", customWeaponDefaultMap.get("pivotOnIconDegree")},
      {"scale", pivotOnIconScaleNode.to_string(40)},
  });
  JsonObjectNode spriteNode = JsonObjectNode({
      {"pivotOnHand", pivotOnHandNode.to_string(36)},
      {"pivotOnIcon", pivotOnIconNode.to_string(36)},
  });
  JsonObjectNode configNode = JsonObjectNode({
      {"type", "\"close\""},
      {"clipAlias", customWeaponDefaultMap.get("reference")},
      {"scaleOnGround", customWeaponDefaultMap.get("scaleOnGround")},
      {"scaleOnIcon", customWeaponDefaultMap.get("scaleOnIcon")},
      {"pickTestFunc", "\"wider\""},
      {"weight", customWeaponDefaultMap.get("weight")},
      {"damage", customWeaponDefaultMap.get("damage")},
      {"swapTime", customWeaponDefaultMap.get("swapTime")},
      {"frameName", "\"BLADE_TYPE\""},
      {"clip", "\"\""},
      {"fireTime", customWeaponDefaultMap.get("fireTime")},
      {"fires", firesNode.to_string(32)},
      {"sprite", spriteNode.to_string(32)},
  });
  JsonObjectNode actorMatchNode = JsonObjectNode({
      {"code", customWeaponDefaultMap.get("code")},
      {"config", configNode.to_string(28)},
  });
  return actorMatchNode;
}

JsonObjectNode getPatrolPathNode(std::unique_ptr<ExpressionNode> &expNode) {
  customWeaponDefaultMap.clearInputMap();
  auto pointValueNode = dynamic_cast<PointValueNode *>(expNode->value.get());
  if (!pointValueNode) {
    std::cerr << "Point-typed value is expected at " << expNode->loc << "\n";
    return JsonObjectNode();
  }
  auto xValueNode =
      dynamic_cast<IntValueNode *>(pointValueNode->x->value.get());
  if (!xValueNode) {
    std::cerr << "Int-typed value is expected at " << pointValueNode->x->loc
              << "\n";
    return JsonObjectNode();
  }
  auto yValueNode =
      dynamic_cast<IntValueNode *>(pointValueNode->y->value.get());
  if (!yValueNode) {
    std::cerr << "Int-typed value is expected at " << pointValueNode->y->loc
              << "\n";
    return JsonObjectNode();
  }
  JsonObjectNode locNode = JsonObjectNode({
      {"x", "\"" + std::to_string(xValueNode->value) + "\""},
      {"y", "\"" + std::to_string(yValueNode->value) + "\""},
      {"range", "\"0\""},
  });
  JsonObjectNode patrolPathNode = JsonObjectNode({
      {"loc", locNode.to_string(32)},
      {"rotation", "\"0\""},
      {"duration", "\"3000\""},
  });
  return patrolPathNode;
}

JsonObjectNode getSpawnPointNode(std::unique_ptr<ExpressionNode> &expNode) {
  customWeaponDefaultMap.clearInputMap();
  auto pointValueNode = dynamic_cast<PointValueNode *>(expNode->value.get());
  if (!pointValueNode) {
    std::cerr << "Point-typed value is expected at " << expNode->loc << "\n";
    return JsonObjectNode();
  }
  auto xValueNode =
      dynamic_cast<IntValueNode *>(pointValueNode->x->value.get());
  if (!xValueNode) {
    std::cerr << "Int-typed value is expected at " << pointValueNode->x->loc
              << "\n";
    return JsonObjectNode();
  }
  auto yValueNode =
      dynamic_cast<IntValueNode *>(pointValueNode->y->value.get());
  if (!yValueNode) {
    std::cerr << "Int-typed value is expected at " << pointValueNode->y->loc
              << "\n";
    return JsonObjectNode();
  }
  return JsonObjectNode({
      {"x", "\"" + std::to_string(xValueNode->value) + "\""},
      {"y", "\"" + std::to_string(yValueNode->value) + "\""},
  });
}
} // namespace

DefaultMap actorMatchDefaultMap = DefaultMap(
    {
        {"matchKind", {config::AST_STRING, config::CODEGEN_STRING, "contain"}},
        {"controller", {config::AST_STRING, config::CODEGEN_STRING, "all"}},
        {"id", {config::AST_STRING, config::CODEGEN_STRING, ""}},
        {"group", {config::AST_INT, config::CODEGEN_STRING, ""}},
    },
    "actorMatch");

DefaultMap buttonDefaultMap = DefaultMap(
    {
        {"id", {config::AST_STRING, config::CODEGEN_STRING, ""}},
        {"text", {config::AST_STRING, config::CODEGEN_STRING, ""}},
    },
    "button");

DefaultMap customWeaponDefaultMap = DefaultMap(
    {
        {"reference", {config::AST_STRING, config::CODEGEN_STRING, ""}},
        {"code", {config::AST_STRING, config::CODEGEN_STRING, ""}},
        {"scaleOnGround", {config::AST_INT, config::CODEGEN_INT, "0.8"}},
        {"scaleOnIcon", {config::AST_INT, config::CODEGEN_INT, "1"}},
        {"weight", {config::AST_INT, config::CODEGEN_INT, "6"}},
        {"damage", {config::AST_INT, config::CODEGEN_INT, "25"}},
        {"swapTime", {config::AST_INT, config::CODEGEN_INT, "800"}},
        {"fireTime", {config::AST_INT, config::CODEGEN_INT, "1500"}},
        {"fireType", {config::AST_STRING, config::CODEGEN_STRING, "backslash"}},
        {"pivotOnHandX", {config::AST_INT, config::CODEGEN_INT, "0"}},
        {"pivotOnHandXScale", {config::AST_INT, config::CODEGEN_INT, "1"}},
        {"pivotOnHandY", {config::AST_INT, config::CODEGEN_INT, "0"}},
        {"pivotOnHandYScale", {config::AST_INT, config::CODEGEN_INT, "1"}},
        {"pivotOnHandDegree", {config::AST_INT, config::CODEGEN_INT, "0"}},
        {"pivotOnIconX", {config::AST_INT, config::CODEGEN_INT, "0"}},
        {"pivotOnIconXScale", {config::AST_INT, config::CODEGEN_INT, "1"}},
        {"pivotOnIconY", {config::AST_INT, config::CODEGEN_INT, "0"}},
        {"pivotOnIconYScale", {config::AST_INT, config::CODEGEN_INT, "1"}},
        {"pivotOnIconDegree", {config::AST_INT, config::CODEGEN_INT, "0"}},
    },
    "customWeapon");

JsonArrayNode getActorMatchesNode(const std::shared_ptr<ValueNode> &valueNode) {
  actorMatchDefaultMap.clearInputMap();
  auto actorMatchValueNode =
      dynamic_cast<ActorMatchValueNode *>(valueNode.get());
  if (!actorMatchValueNode) {
    std::cerr << "ActorMatch-typed value is expected at " << valueNode->loc
              << "\n";
    return JsonArrayNode();
  }
  actorMatchDefaultMap.addInputMap(actorMatchValueNode->paramApps->named_args);
  JsonObjectNode actorCodeNode = JsonObjectNode({
      {"method", actorMatchDefaultMap.get("matchKind", matchKind::keywordEnum)},
      {"actorCode", actorMatchDefaultMap.get("id")},
  });
  JsonArrayNode actorCodesNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(actorCodeNode));
  JsonObjectNode campNode = JsonObjectNode("campAll", "true");
  JsonObjectNode actorMatchNode = JsonObjectNode({
      {"actorCodes", actorCodesNode.to_string(32)},
      {"brain",
       actorMatchDefaultMap.get("controller", actorBrainKind::keywordEnum)},
      {"camp", campNode.to_string(32)},
      {"excludeActorCodes", "[]"},
  });
  if (actorMatchDefaultMap.get("group") != "\"\"") {
    actorMatchNode.addNode("checkGroup", "true")
        .addNode("group", actorMatchDefaultMap.get("group"));
  }
  return JsonArrayNode(std::make_shared<JsonObjectNode>(actorMatchNode));
}

JsonArrayNode getButtonListNode(const std::shared_ptr<ValueNode> &valueNode) {
  JsonArrayNode arrayNode = JsonArrayNode();
  if (auto listNode = dynamic_cast<ListValueNode *>(valueNode.get()))
    for (auto &item : listNode->items)
      arrayNode.addNode(std::make_shared<JsonObjectNode>(getButtonNode(item)));
  else
    std::cerr << "list-typed value is expected at " << valueNode->loc << "\n";
  return arrayNode;
}

JsonArrayNode
getDialogButtonListNode(const std::shared_ptr<ValueNode> &valueNode) {
  JsonArrayNode arrayNode = JsonArrayNode();
  if (auto listNode = dynamic_cast<ListValueNode *>(valueNode.get()))
    for (auto &item : listNode->items)
      arrayNode.addNode(
          std::make_shared<JsonObjectNode>(getDialogButtonNode(item)));
  else
    std::cerr << "list-typed value is expected at " << valueNode->loc << "\n";
  return arrayNode;
}

JsonObjectNode
getEnhFFActorMatchesNode(const std::shared_ptr<ValueNode> &valueNode) {
  actorMatchDefaultMap.clearInputMap();
  auto actorMatchValueNode =
      dynamic_cast<ActorMatchValueNode *>(valueNode.get());
  if (!actorMatchValueNode) {
    std::cerr << "ActorMatch-typed value is expected at " << valueNode->loc
              << "\n";
    return JsonObjectNode();
  }
  actorMatchDefaultMap.addInputMap(actorMatchValueNode->paramApps->named_args);
  auto controllerMap = [](std::string s) -> std::string {
    if (s.find(actorBrainKind::keywordAI) != -1)
      return "\"1\"";
    else if (s.find(actorBrainKind::keywordPlayer) != -1)
      return "\"2\"";
    return "\"0\"";
  };
  std::string controller = controllerMap(
      actorMatchDefaultMap.get("controller", actorBrainKind::keywordEnum));

  JsonObjectNode maNode = JsonObjectNode({
      {"brain", controller},
      {"relship", "\"0\""},
      {"survival", "\"1\""},
  });
  if (actorMatchDefaultMap.get("id") != "\"\"") {
    JsonObjectNode facSubNode = JsonObjectNode({
        {"op", "\"in\""},
        {"mac", JsonObjectNode(
                    {
                        {"method", actorMatchDefaultMap.get(
                                       "matchKind", matchKind::keywordEnum)},
                        {"actorCode", actorMatchDefaultMap.get("id")},
                    })
                    .to_string(48)},
    });
    JsonArrayNode facNode =
        JsonArrayNode(std::make_shared<JsonObjectNode>(facSubNode));
    maNode.addNode("cac", "true").addNode("fac", facNode.to_string(40));
  }
  if (actorMatchDefaultMap.get("group") != "\"\"") {
    JsonObjectNode compSubNode = JsonObjectNode({
        {"op", "\"==\""},
        {"val", actorMatchDefaultMap.get("group")},
    });
    JsonArrayNode compNode =
        JsonArrayNode(std::make_shared<JsonObjectNode>(compSubNode));
    JsonObjectNode fgrSubNode = JsonObjectNode({
        {"op", "\"in\""},
        {"comp", compNode.to_string(48)},
    });
    JsonArrayNode fgrNode =
        JsonArrayNode(std::make_shared<JsonObjectNode>(fgrSubNode));
    maNode.addNode("cgr", "true").addNode("fgr", fgrNode.to_string(40));
  }
  return maNode;
}

JsonArrayNode
getCustomWeaponsListNode(const std::shared_ptr<ValueNode> &valueNode) {
  JsonArrayNode arrayNode = JsonArrayNode();
  if (auto listNode = dynamic_cast<ListValueNode *>(valueNode.get()))
    for (auto &item : listNode->items)
      arrayNode.addNode(
          std::make_shared<JsonObjectNode>(getCustomWeaponsNode(item)));
  else
    std::cerr << "list-typed value is expected at " << valueNode->loc << "\n";
  return arrayNode;
}

JsonArrayNode
getPatrolPathListNode(const std::shared_ptr<ValueNode> &valueNode) {
  JsonArrayNode arrayNode = JsonArrayNode();
  if (auto listNode = dynamic_cast<ListValueNode *>(valueNode.get()))
    for (auto &item : listNode->items)
      arrayNode.addNode(
          std::make_shared<JsonObjectNode>(getPatrolPathNode(item)));
  else
    std::cerr << "list-typed value is expected at " << valueNode->loc << "\n";
  return arrayNode;
}

JsonArrayNode
getSpawnPointListNode(const std::shared_ptr<ValueNode> &valueNode) {
  JsonArrayNode arrayNode = JsonArrayNode();
  if (auto listNode = dynamic_cast<ListValueNode *>(valueNode.get()))
    for (auto &item : listNode->items)
      arrayNode.addNode(
          std::make_shared<JsonObjectNode>(getSpawnPointNode(item)));
  else
    std::cerr << "list-typed value is expected at " << valueNode->loc << "\n";
  return arrayNode;
}