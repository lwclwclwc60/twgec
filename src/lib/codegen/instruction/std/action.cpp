#include "instruction/action.h"
#include "codegen.h"
#include "keyword.h"
#include "utils/defaultMap.h"
#include "utils/formatter.h"
#include <map>

using namespace codegen;
using namespace formatter;
using namespace keyword;

JsonObjectNode
action::ActionActorAttributes::method(DefaultMap defaultMap,
                                      UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode opNode = JsonObjectNode({
      {"attr", defaultMap.get("attr", actorAttrKind::keywordEnum)},
      {"operation", "\"set\""},
      {"value", defaultMap.get("value")},
  });
  JsonArrayNode opsNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(opNode));
  return JsonObjectNode({
      {"actorCode", defaultMap.get("actorId")},
      {"ops", opsNode.to_string(24)},
      {"takeFriendAIFire", "\"u\""},
      {"takeFriendPlayerFire", "\"u\""},
      {"exitWormhole", "\"u\""},
      {"setStatic", "\"u\""},
      {"setTornadoRes", "\"u\""},
      {"bloodType", "\"u\""},
  });
}

JsonObjectNode
action::ActionActorDisappear::method(DefaultMap defaultMap,
                                     UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"actorCode", defaultMap.get("actorId")},
      {"duration", defaultMap.get("duration")},
      {"delay", defaultMap.get("delay")},
  });
}

JsonObjectNode
action::ActionActorFollow::method(DefaultMap defaultMap,
                                  UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"actorCode", defaultMap.get("actorId")},
      {"followType", defaultMap.get("type", followActorKind::keywordEnum)},
      {"targetCode", defaultMap.get("targetId")},
  });
}

JsonObjectNode
action::ActionActorRelocate::method(DefaultMap defaultMap,
                                    UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode locNode = JsonObjectNode({
      {"x", defaultMap.get("x")},
      {"y", defaultMap.get("y")},
      {"range", "\"0\""},
  });
  JsonArrayNode locsNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(locNode));
  return JsonObjectNode({
      {"actorCode", defaultMap.get("actorId")},
      {"locType", "\"loc\""},
      {"locs", locsNode.to_string(24)},
      {"shiftX", "\"0\""},
      {"shiftY", "\"0\""},
      {"keepAbStates", defaultMap.get("keepAbility")},
  });
}

JsonObjectNode
action::ActionActorSpawnLoc::method(DefaultMap defaultMap,
                                    UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode locNode = JsonObjectNode({
      {"x", defaultMap.get("x")},
      {"y", defaultMap.get("y")},
      {"range", "\"0\""},
  });
  JsonArrayNode locsNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(locNode));
  return JsonObjectNode({
      {"actorSelect", "\"code\""},
      {"actorCode", defaultMap.get("actorId")},
      {"locType", "\"loc\""},
      {"locs", locsNode.to_string(24)},
      {"relocate", "true"},
      {"disabled", "false"},
  });
}

JsonObjectNode
action::ActionActorTalk::method(DefaultMap defaultMap,
                                UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"cleanTalk", defaultMap.get("cleanTalk")},
      {"speech", defaultMap.get("text")},
      {"duration", defaultMap.get("duration")},
      {"wait", defaultMap.get("wait")},
      {"waitMore", "\"100\""},
      {"actorCode", defaultMap.get("actorId")},
  });
}

JsonObjectNode
action::ActionAddActor::method(DefaultMap defaultMap,
                               UserDefinedMetadata userDefinedMeta) {

  std::string externRole = defaultMap.get("externRole");
  bool hasExternRole = externRole != "\"\"";
  std::string role =
      hasExternRole ? "8" : defaultMap.get("role", roleKind::keywordEnum);
  JsonObjectNode roleNode = JsonObjectNode({{"dr", role}});
  if (hasExternRole)
    roleNode.addNode("rsrc", externRole);

  JsonObjectNode weapon0Node = JsonObjectNode({
      {"w0Type", defaultMap.get("weapon1", weaponKind::keywordEnum,
                                userDefinedMeta.customWeaponsKeywordEnum)},
  });
  JsonObjectNode weapon1Node = JsonObjectNode({
      {"w1Type", defaultMap.get("weapon2", weaponKind::keywordEnum,
                                userDefinedMeta.customWeaponsKeywordEnum)},
  });
  JsonObjectNode locationNode = JsonObjectNode({
      {"x", defaultMap.get("x")},
      {"y", defaultMap.get("y")},
      {"range", defaultMap.get("range")},
  });
  return JsonObjectNode({
      {"actorCode", defaultMap.get("id")},
      {"name", defaultMap.get("name")},
      {"role", roleNode.to_string(24)},
      {"actorType", "\"defaultType\""},
      {"weapon0", weapon0Node.to_string(24)},
      {"weapon1", weapon1Node.to_string(24)},
      {"camp", defaultMap.get("camp", campKind::keywordEnum)},
      {"group", defaultMap.get("teamId")},
      {"location", locationNode.to_string(24)},
      {"shiftX", "0"},
      {"shiftY", "0"},
      {"spawnLoc", "true"},
      {"rotation", defaultMap.get("rotation")},
      {"idleRotate", "true"},
      {"maxhp", defaultMap.get("hp")},
      {"manaPower", "\"0\""},
      {"lives", "\"1\""},
      {"preferAbilityLevel", "\"smart\""},
      {"maxAbilityLevel", "1"},
      {"weight", "\"4\""},
      {"strength", defaultMap.get("strength")},
      {"vision", "\"300\""},
      {"range", defaultMap.get("movableRange")},
      {"score", "\"10\""},
      {"bloodType", "\"default\""},
      {"unbrokenArmor", "true"},
      {"tornadoRes", "\"none\""},
      {"distractWhenHit", "true"},
      {"thinkInterval", "60"},
      {"farAutoLevel", "\"0\""},
      {"patrols", "true"},
      {"patrolPath", defaultMap.get("patrol")},
      {"bornDelayDuration", "\"0\""},
      {"bornDuration", "\"1000\""},
      {"bornLockDuration", "\"1000\""},
      {"bornEffect", "\"none\""},
      {"bornAnim", "\"fadein\""},
      {"localVarname", defaultMap.get("localVarname")},
      {"globalVarname", "\"\""},
  });
}

JsonObjectNode
action::ActionAddDropItem::method(DefaultMap defaultMap,
                                  UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode locationNode = JsonObjectNode({
      {"x", defaultMap.get("x")},
      {"y", defaultMap.get("y")},
      {"range", defaultMap.get("range")},
  });
  return JsonObjectNode({
      {"location", locationNode.to_string(24)},
      {"shiftX", "0"},
      {"shiftY", "0"},
      {"itemCode", defaultMap.get("itemCode")},
      {"itemType", defaultMap.get("type", DropItemKind::keywordEnum)},
      {"itemScale", defaultMap.get("scale")},
      {"localVarname", defaultMap.get("localVarname")},
  });
}

JsonObjectNode
action::ActionAddMapSign::method(DefaultMap defaultMap,
                                 UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode btnsGroupNode = JsonObjectNode({
      {"rowSize", "\"1\""},
      {"btnMinWidth", "\"1\""},
      {"textAlign", "\"center\""},
      {"varnameButton", "\"\""},
      {"btns", defaultMap.get("buttons")},
  });
  JsonObjectNode locNode = JsonObjectNode({
      {"x", defaultMap.get("x")},
      {"y", defaultMap.get("y")},
      {"range", defaultMap.get("range")},
  });
  JsonArrayNode locsNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(locNode));
  return JsonObjectNode({
      {"localVarname", "\"\""},
      {"locs", locsNode.to_string(24)},
      {"shape", "\"wood\""},
      {"rotation", defaultMap.get("rotation")},
      {"speech", defaultMap.get("text")},
      {"parsePolicy", "\"view\""},
      {"showButtons", defaultMap.get("showButtons")},
      {"btnsGroup", btnsGroupNode.to_string(24)},
  });
}

JsonObjectNode
action::ActionAddManaCircle::method(DefaultMap defaultMap,
                                    UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode locNode = JsonObjectNode({
      {"x", defaultMap.get("x")},
      {"y", defaultMap.get("y")},
      {"range", "\"0\""},
  });
  JsonArrayNode locsNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(locNode));
  JsonObjectNode actorMatchNode = JsonObjectNode("skip", "true");
  std::string heal = defaultMap.get("heal") == "\"0\"" ? "false" : "true";
  return JsonObjectNode({
      {"deviceCode", "\"device\""},
      {"localVarname", "\"random\""},
      {"locs", locsNode.to_string(24)},
      {"actorMatch", actorMatchNode.to_string(24)},
      {"power", "\"0\""},
      {"useColor", "\"origin\""},
      {"layer", "\"groundTop\""},
      {"heal", heal},
      {"healPower", defaultMap.get("heal")},
      {"healActorCooldown", defaultMap.get("healActorCooldown")},
      {"healDeviceCooldown", defaultMap.get("healDeviceCooldown")},
  });
}

JsonObjectNode
action::ActionAddMapObject::method(DefaultMap defaultMap,
                                   UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode locNode = JsonObjectNode({
      {"x", defaultMap.get("x")},
      {"y", defaultMap.get("y")},
      {"range", defaultMap.get("range")},
  });
  JsonArrayNode locsNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(locNode));
  std::string forceBlock =
      defaultMap.get("walkable") == "true" ? "false" : "true";
  return JsonObjectNode({
      {"locType", "\"locs\""},
      {"pick", "\"random\""},
      {"locs", locsNode.to_string(24)},
      {"resource", defaultMap.get("object", MapObjectKind::keywordEnum)},
      {"autoLayer", defaultMap.get("autoTuneHeight")},
      {"forceBlock", forceBlock},
  });
}

JsonObjectNode
action::ActionAddStuff::method(DefaultMap defaultMap,
                               UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode locationNode = JsonObjectNode({
      {"x", defaultMap.get("x")},
      {"y", defaultMap.get("y")},
      {"range", defaultMap.get("range")},
  });
  return JsonObjectNode({
      {"itemCode", defaultMap.get("code")},
      {"itemType", defaultMap.get("item")},
      {"loop", defaultMap.get("refill")},
      {"loopInterval", defaultMap.get("refillInterval")},
      {"location", locationNode.to_string(24)},
      {"rotation", defaultMap.get("rotation")},
      {"cameraZoomPolicy", "\"none\""},
      {"localVarname", "\"item\""},
  });
}

JsonObjectNode
action::ActionAvoidFriendFire::method(DefaultMap defaultMap,
                                      UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"avoidFriendFire", defaultMap.get("value")},
      {"avoidFriendFireValue", "\"y\""},
  });
}

JsonObjectNode
action::ActionCloseDialog::method(DefaultMap defaultMap,
                                  UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"playerTarget", "\"one\""},
      {"playerId", defaultMap.get("actorId")},
      {"dialogId", defaultMap.get("dialogId")},
      {"closeMehtod", "\"cancel\""},
  });
}

JsonObjectNode
action::ActionConsole::method(DefaultMap defaultMap,
                              UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"logType", defaultMap.get("type")},
      {"text", defaultMap.get("text")},
      {"value", "\"\""},
  });
}

JsonObjectNode
action::ActionShowSelectDialog::method(DefaultMap defaultMap,
                                       UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode msgImageNode =
      JsonObjectNode(std::vector<std::pair<std::string, std::string>>{
          {"msgImageAlias", "\"\""},
          {"msgImageSide", "\"left\""},
      });
  JsonObjectNode controlsNode = JsonObjectNode({
      {"buttonIconSide", "\"right\""},
      {"conBottom", "8"},
      {"conBottomUnit", "\"px\""},
      {"confirmLabel", "\"確定\""},
      {"confirmIcon", "\"check\""},
      {"hideCancelButton", "true"},
  });
  JsonObjectNode transitionNode = JsonObjectNode({
      {"tranClaz", "\"slide\""},
      {"tranDuration", "\"250\""},
      {"tranDirection", "\"up\""},
  });
  JsonObjectNode optionsNode = JsonObjectNode({
      {"maxWidth", "\"\""},
      {"backgroundImage", "\"\""},
      {"transition", transitionNode.to_string(28)},
  });
  return JsonObjectNode({
      {"playerTarget", "\"one\""},
      {"playerId", defaultMap.get("actorId")},
      {"dialogId", defaultMap.get("dialogId")},
      {"title", "\"\""},
      {"message", defaultMap.get("message")},
      {"messageType", "\"html\""},
      {"msgImage", msgImageNode.to_string(24)},
      {"controls", controlsNode.to_string(24)},
      {"options", optionsNode.to_string(24)},
      {"defaultValue", "\"\""},
      {"selectOptions", defaultMap.get("options")},
  });
}

JsonObjectNode
action::ActionDeltaHp::method(DefaultMap defaultMap,
                              UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"actorCode", defaultMap.get("actorCode")},
      {"deltaType", defaultMap.get("type", deltaHpKind::keywordEnum)},
      {"hp", defaultMap.get("value")},
      {"casterCode", defaultMap.get("casterCode")},
  });
}

JsonObjectNode
action::ActionEnblastEffect::method(DefaultMap defaultMap,
                                    UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode fromPosNode = JsonObjectNode({
      {"posType", defaultMap.get("fromType")},
      {"actorCode", defaultMap.get("fromActor")},
      {"alive", "true"},
  });
  return JsonObjectNode({
      {"fromPos", fromPosNode.to_string(24)},
      {"toType", defaultMap.get("toType")},
      {"toAngle", defaultMap.get("toAngle")},
      {"damage", defaultMap.get("damage")},
      {"scale", defaultMap.get("scale")},
      {"speed", defaultMap.get("speed")},
  });
}

JsonObjectNode
action::ActionEquipWeapon::method(DefaultMap defaultMap,
                                  UserDefinedMetadata userDefinedMeta) {
  std::string handIndex = defaultMap.get("hand");
  std::string weaponIndex = "-1";
  if (handIndex == "1")
    weaponIndex = "0";
  if (handIndex == "2")
    weaponIndex = "1";
  return JsonObjectNode({
      {"actorCode", defaultMap.get("actorCode")},
      {"weaponType", defaultMap.get("type", weaponKind::keywordEnum,
                                    userDefinedMeta.customWeaponsKeywordEnum)},
      {"weaponIndex", weaponIndex},
      {"checkOwnership", "false"},
      {"makeDefault", defaultMap.get("isDefault")},
  });
}

JsonObjectNode
action::ActionGetCookie::method(DefaultMap defaultMap,
                                UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"ckey", defaultMap.get("cookies")},
      {"key", defaultMap.get("varName")},
  });
}

JsonObjectNode
action::ActionGetUserState::method(DefaultMap defaultMap,
                                   UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"playerId", defaultMap.get("playerId")},
      {"category", defaultMap.get("category")},
      {"key", defaultMap.get("key")},
      {"varname", defaultMap.get("varName")},
  });
}

JsonObjectNode
action::ActionMapWarp::method(DefaultMap defaultMap,
                              UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode locationNode = JsonObjectNode({
      {"x", defaultMap.get("fromX")},
      {"y", defaultMap.get("fromY")},
      {"range", "\"0\""},
  });
  JsonObjectNode targetNode = JsonObjectNode({
      {"x", defaultMap.get("toX")},
      {"y", defaultMap.get("toY")},
      {"range", "\"0\""},
  });
  return JsonObjectNode({
      {"location", locationNode.to_string(24)},
      {"op", "\"add\""},
      {"direction", defaultMap.get("direction", MapWarpKind::keywordEnum)},
      {"warpSpeed", "\"imme\""},
      {"target", targetNode.to_string(24)},
  });
}

JsonObjectNode
action::ActionMissionComplete::method(DefaultMap defaultMap,
                                      UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"targetGroup",
       defaultMap.get("camp", campMissionCompleteKind::keywordEnum)},
  });
}

JsonObjectNode
action::ActionLongBo::method(DefaultMap defaultMap,
                             UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"posType", "\"actor\""},
      {"actorCode", defaultMap.get("actorCode")},
      {"alive", "true"},
      {"longBoType", "\"he\""},
      {"moveType", "\"path\""},
      {"setRotation", "\"default\""},
      {"pathLen", "\"40\""},
      {"maxTurns", "\"5\""},
      {"headScale", "\"1\""},
      {"tailScale", "\"1\""},
      {"speed", "\"1\""},
  });
}

JsonObjectNode
action::ActionRemoveDevice::method(DefaultMap defaultMap,
                                   UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode regionNode = JsonObjectNode({
      {"defType", "\"rect\""},
      {"x", defaultMap.get("region_x")},
      {"y", defaultMap.get("region_y")},
      {"w", defaultMap.get("region_w")},
      {"h", defaultMap.get("region_h")},
  });
  JsonArrayNode regionsNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(regionNode));
  return JsonObjectNode({
      {"searchType", "\"byLoc\""},
      {"setDeviceType", "\"all\""},
      {"regions", regionsNode.to_string(24)},
      {"exRegions", "[]"},
      {"action", "\"remove\""},
  });
}

JsonObjectNode
action::ActionRemoveMapObject::method(DefaultMap defaultMap,
                                      UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode regionNode = JsonObjectNode({
      {"defType", "\"rect\""},
      {"x", defaultMap.get("region_x")},
      {"y", defaultMap.get("region_y")},
      {"w", defaultMap.get("region_w")},
      {"h", defaultMap.get("region_h")},
  });
  JsonArrayNode regionsNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(regionNode));
  return JsonObjectNode({
      {"locType", "\"regions\""},
      {"regions", regionsNode.to_string(24)},
      {"exRegions", "[]"},
      {"resourceType", "\"all\""},
  });
}

JsonObjectNode
action::ActionSetCookie::method(DefaultMap defaultMap,
                                UserDefinedMetadata userDefinedMeta) {

  string playerId = defaultMap.get("playerId");
  JsonObjectNode dataNode;
  dataNode = (playerId == "\"\"") ? JsonObjectNode("playerTarget", "\"all\"")
                                  : JsonObjectNode({{"playerTarget", "\"one\""},
                                                    {"playerId", playerId}});

  dataNode.addNode("key", defaultMap.get("cookies"))
      .addNode("valueType", defaultMap.get("type", valueTypeKind::keywordEnum))
      .addNode("value", defaultMap.get("value"));
  return dataNode;
}

JsonObjectNode
action::ActionSetGlobal::method(DefaultMap defaultMap,
                                UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"key", defaultMap.get("key")},
      {"valueType", defaultMap.get("type", valueTypeKind::keywordEnum)},
      {"value", defaultMap.get("value")},
  });
}

JsonObjectNode
action::ActionSetObjectVar::method(DefaultMap defaultMap,
                                   UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"objectId", defaultMap.get("object")},
      {"key", defaultMap.get("key")},
      {"valueType", defaultMap.get("type", valueTypeKind::keywordEnum)},
      {"value", defaultMap.get("value")},
  });
}

JsonObjectNode
action::ActionSetUserState::method(DefaultMap defaultMap,
                                   UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"playerId", defaultMap.get("playerId")},
      {"category", defaultMap.get("category")},
      {"key", defaultMap.get("key")},
      {"valueType", defaultMap.get("type", valueTypeKind::keywordEnum)},
      {"value", defaultMap.get("value")},
  });
}

JsonObjectNode
action::ActionSetWeaponAbility::method(DefaultMap defaultMap,
                                       UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode campNode = JsonObjectNode("campAll", "true");
  JsonObjectNode abilityNode = JsonObjectNode(
      "value", defaultMap.get("ability", abilityKind::keywordEnum));
  return JsonObjectNode({
      {"weaponType", defaultMap.get("weapon", weaponKind::keywordEnum,
                                    userDefinedMeta.customWeaponsKeywordEnum)},
      {"camp", campNode.to_string(24)},
      {"abilityLevel", defaultMap.get("level")},
      {"operation",
       defaultMap.get("operation", SetWeaponAbilityOperationKind::keywordEnum)},
      {"ability", abilityNode.to_string(24)},
  });
}

JsonObjectNode
action::ActionTipOnMap::method(DefaultMap defaultMap,
                               UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode locNode = JsonObjectNode({
      {"x", defaultMap.get("x")},
      {"y", defaultMap.get("y")},
      {"range", "\"0\""},
  });
  JsonArrayNode locsNode =
      JsonArrayNode(std::make_shared<JsonObjectNode>(locNode));
  return JsonObjectNode({
      {"locs", locsNode.to_string(24)},
      {"shiftX", "\"0\""},
      {"shiftY", "\"0\""},
      {"cleanTalk", "false"},
      {"speech", defaultMap.get("text")},
      {"html", defaultMap.get("html")},
      {"align", "\"center\""},
      {"scalePolicy", "\"withWindowMinOne\""},
      {"stay", "\"time\""},
      {"duration", defaultMap.get("duration")},
  });
}

JsonObjectNode action::ActionWait::method(DefaultMap defaultMap,
                                          UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"duration", defaultMap.get("duration")},
  });
}