#include "instruction/action.h"
#include "codegen.h"
#include "utils/defaultMap.h"
#include "utils/formatter.h"

using namespace codegen;
using namespace formatter;
using namespace keyword;

JsonObjectNode action::ActionEnhFFPlayerMousePosition::method(
    DefaultMap defaultMap, UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"actorCode", defaultMap.get("actorId")},
      {"varX", defaultMap.get("varX")},
      {"varY", defaultMap.get("varY")},
      {"wait", "true"},
  });
}

JsonObjectNode action::ActionEnhFFGeneralCircularRange::method(
    DefaultMap defaultMap, UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode posNode = JsonObjectNode({
      {"x", defaultMap.get("x")},
      {"y", defaultMap.get("y")},
  });
  std::string defType = defaultMap.get(
      "defType", EnhFFGeneralCircularRangeDefTypeKind::keywordEnum);
  JsonObjectNode regionNode = JsonObjectNode();
  regionNode.addNode("defType", defType)
      .addNode("offX", defaultMap.get("offsetX"))
      .addNode("offY", defaultMap.get("offsetY"));
  if (defType == "\"rectangle\"") {
    regionNode.addNode("reWid", defaultMap.get("width"))
        .addNode("reHei", defaultMap.get("height"));
  } else {
    regionNode.addNode("ciRad", defaultMap.get("radius"));
  }
  std::string actorCode = defaultMap.get("actorId");
  JsonObjectNode dataNode = JsonObjectNode();
  if (actorCode != "\"\"") {
    dataNode.addNode("select", "\"actor\"")
        .addNode("actorCode", actorCode)
        .addNode("onActorLayer", "\"over\"");
  } else {
    dataNode.addNode("select", "\"map\"")
        .addNode("pos", posNode.to_string(24))
        .addNode("onMapLayer", "\"sky\"");
  }
  std::string duration = defaultMap.get("duration");
  dataNode.addNode("region", regionNode.to_string(24))
      .addNode("duration", duration)
      .addNode("color", defaultMap.get("color"))
      .addNode("alpha", defaultMap.get("alpha"))
      .addNode("lineWidth", defaultMap.get("lineWidth"))
      .addNode("lineAlign", "\"1\"")
      .addNode("fill", defaultMap.get("fill"))
      .addNode("fillColor", defaultMap.get("fillColor"))
      .addNode("fillAlpha", defaultMap.get("fillAlpha"))
      .addNode("angle", "\"0\"")
      .addNode("rot", "false")
      .addNode("orbit", "false")
      .addNode("bornAnim", "\"none\"")
      .addNode("disAnim", "\"none\"");
  std::string deltaHpValue = defaultMap.get("deltaHpValue");
  std::string deltaHpTarget = defaultMap.get("deltaHpTarget");
  if (deltaHpValue != "\"\"") {
    JsonObjectNode damageTypeNode =
        JsonObjectNode("damageTypeCode", "\"none\"");
    JsonObjectNode dhSetNode = JsonObjectNode({
        {"deltaType", defaultMap.get("deltaHpType", deltaHpKind::keywordEnum)},
        {"hp", deltaHpValue},
        {"casterCode", defaultMap.get("deltaHpCasterCode")},
        {"damageType", damageTypeNode.to_string(28)},
        {"dmgWTypeOpt", "\"default\""},
        {"hitAngleOpt", "\"default\""},
        {"drainPctOpt", "\"default\""},
        {"flicker", "\"default\""},
        {"noReact", "\"default\""},
        {"hitSfx", "\"default\""},
        {"sfxOpt", "\"default\""},
        {"__env__", "\"range\""},
    });
    dataNode.addNode("deltaHp", "true")
        .addNode("dhRepeat", "\"cd\"")
        .addNode("dhCD", defaultMap.get("deltaHpCD"))
        .addNode("dhSet", dhSetNode.to_string(24));
    if (deltaHpTarget != "[]") {
      JsonObjectNode dhTgtItemNode =
          JsonObjectNode({{"op", "\"in\""}, {"ma", deltaHpTarget}});
      JsonArrayNode dhTgtNode =
          JsonArrayNode(std::make_shared<JsonObjectNode>(dhTgtItemNode));
      dataNode.addNode("dhAll", "false")
          .addNode("dhTgt", dhTgtNode.to_string(28));
    } else {
      dataNode.addNode("dhAll", "true");
    }
  } else {
    dataNode.addNode("deltaHp", "false");
  }
  return dataNode;
}