#ifndef CONFIG_INSTRUCTION_H
#define CONFIG_INSTRUCTION_H

#include "utils.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstring>
#include <map>
#include <string>
#include <vector>

namespace config {

enum ASTType {
  AST_INVALID,
  AST_INT,
  AST_STRING,
  AST_BOOL,
  AST_POINT,
  AST_ACTOR_MATCH,
  AST_LIST_BUTTON,
  AST_LIST_STRING,
  AST_LIST_POINT,
  AST_LIST_CUSTOM_WEAPON,
};

namespace {
inline std::string toString(ASTType astType) {
  switch (astType) {
  case AST_BOOL:
    return "bool";
  case AST_INT:
    return "int|string";
  case AST_LIST_POINT:
    return "list[Point]";
  case AST_POINT:
    return "Point";
  case AST_ACTOR_MATCH:
    return "ActorMatch";
  case AST_LIST_BUTTON:
    return "list[Button]";
  case AST_LIST_CUSTOM_WEAPON:
    return "CustomWeapon";
  case AST_STRING:
    return "string";
  case AST_LIST_STRING:
    return "list[string]";
  case AST_INVALID:
  default:
    return "?";
  }
}
} // namespace

enum CodegenType {
  CODEGEN_INVALID,
  CODEGEN_INT,
  CODEGEN_STRING, // two `\"` will be added
  CODEGEN_BOOL,
  CODEGEN_ACTOR_MATCH,
  CODEGEN_ENHFF_ACTOR_MATCH,
  CODEGEN_LIST_BUTTON,
  CODEGEN_LIST_CUSTOM_WEAPON,
  CODEGEN_LIST_STRING,
  CODEGEN_LIST_SPAWN_POINT,  // {x,y}
  CODEGEN_LIST_PATROL_POINT, // {loc{x,y,range},rotation,duration}
};

struct InstructionParam {
  std::string key;
  ASTType astType;
  CodegenType codegenType;
  std::string defaultValue;
};

class InstructionConfig {
public:
  // Variable
  static constexpr const char *defaultScope = "std";
  std::string scope;
  std::string name;
  std::string alias;
  std::string codegenName;
  std::vector<InstructionParam> params;

  // Constructor
  InstructionConfig(std::string _scope, std::string _name, std::string _alias)
      : scope(_scope), name(_name), alias(_alias) {
    assert(std::islower(_name[0]));
    codegenName = _name;
    codegenName[0] = std::toupper(codegenName[0]);
  }
  InstructionConfig(std::string _name, std::string _alias)
      : InstructionConfig(defaultScope, _name, _alias) {}

  // Function
  InstructionConfig setCodegenName(std::string name) {
    codegenName = name;
    return *this;
  }
  InstructionConfig addParam(std::string key, ASTType astType,
                             CodegenType codegenType,
                             std::string defaultValue) {
    params.push_back(
        InstructionParam({key, astType, codegenType, defaultValue}));
    return *this;
  }

  std::string toString() {
    const int padding = 28;
    std::string ret;
    std::string frontendName = name;
    std::string frontendAlias = alias;
    if (std::strcmp(scope.c_str(), defaultScope) != 0) {
      frontendName = scope + "::" + frontendName;
      frontendAlias = scope + "::" + frontendAlias;
    }
    assert(frontendName.size() <= padding);
    ret += inden(4) + frontendName + inden(padding - frontendName.size()) +
           frontendAlias + "\n" + inden(7);
    std::sort(params.begin(), params.end(),
              [](const InstructionParam &a, const InstructionParam &b) {
                return a.key < b.key;
              });
    for (auto param : params)
      ret += " " + param.key + ": " + ::config::toString(param.astType) + ",";
    if (ret.back() == ',')
      ret.pop_back();
    return ret;
  }
};

inline std::map<std::string, InstructionConfig>
configVecToMap(const std::vector<InstructionConfig> vec) {
  std::map<std::string, InstructionConfig> map;
  for (auto config : vec) {
    std::string scope = config.scope == "std" ? "" : config.scope + "::";
    assert(map.find(scope + config.name) == map.end() &&
           map.find(scope + config.alias) == map.end());
    map.insert({scope + config.name, config});
    map.insert({scope + config.alias, config});
  }
  return map;
}

const InstructionConfig actorAttributesConfig =
    InstructionConfig("actorAttributes", "設定角色屬性")
        .addParam("actorId", AST_STRING, CODEGEN_STRING, "")
        .addParam("attr", AST_STRING, CODEGEN_STRING, "")
        .addParam("value", AST_INT, CODEGEN_STRING, "");
const InstructionConfig actorDisappearConfig =
    InstructionConfig("actorDisappear", "角色消失")
        .addParam("actorId", AST_STRING, CODEGEN_STRING, "")
        .addParam("duration", AST_INT, CODEGEN_STRING, "600")
        .addParam("delay", AST_INT, CODEGEN_STRING, "0");
const InstructionConfig actorFollowConfig =
    InstructionConfig("actorFollow", "跟隨人物")
        .addParam("actorId", AST_STRING, CODEGEN_STRING, "")
        .addParam("type", AST_STRING, CODEGEN_STRING, "actor")
        .addParam("targetId", AST_STRING, CODEGEN_STRING, "");
const InstructionConfig actorRelocateConfig =
    InstructionConfig("actorRelocate", "移動角色位置")
        .addParam("actorId", AST_STRING, CODEGEN_STRING, "")
        .addParam("x", AST_INT, CODEGEN_STRING, "0")
        .addParam("y", AST_INT, CODEGEN_STRING, "0")
        .addParam("keepAbility", AST_BOOL, CODEGEN_BOOL, "false");
const InstructionConfig actorSpawnLocConfig =
    InstructionConfig("actorSpawnLoc", "角色重生位置")
        .addParam("actorId", AST_STRING, CODEGEN_STRING, "")
        .addParam("x", AST_INT, CODEGEN_STRING, "0")
        .addParam("y", AST_INT, CODEGEN_STRING, "0");
const InstructionConfig actorTalkConfig =
    InstructionConfig("actorTalk", "角色說話")
        .addParam("cleanTalk", AST_BOOL, CODEGEN_BOOL, "true")
        .addParam("text", AST_STRING, CODEGEN_STRING, "")
        .addParam("duration", AST_INT, CODEGEN_STRING, "3000")
        .addParam("wait", AST_BOOL, CODEGEN_BOOL, "false")
        .addParam("actorId", AST_STRING, CODEGEN_STRING, "");
const InstructionConfig addActorConfig =
    InstructionConfig("addActor", "新增角色")
        .addParam("id", AST_STRING, CODEGEN_STRING, "ai*")
        .addParam("name", AST_STRING, CODEGEN_STRING, "")
        .addParam("camp", AST_STRING, CODEGEN_STRING, "skydow")
        .addParam("teamId", AST_INT, CODEGEN_STRING, "0")
        .addParam("weapon1", AST_STRING, CODEGEN_STRING, "default")
        .addParam("weapon2", AST_STRING, CODEGEN_STRING, "default")
        .addParam("localVarname", AST_STRING, CODEGEN_STRING, "actor")
        .addParam("x", AST_INT, CODEGEN_STRING, "0")
        .addParam("y", AST_INT, CODEGEN_STRING, "0")
        .addParam("hp", AST_INT, CODEGEN_STRING, "100")
        .addParam("range", AST_INT, CODEGEN_STRING, "0")
        .addParam("rotation", AST_INT, CODEGEN_STRING, "0")
        .addParam("movableRange", AST_INT, CODEGEN_STRING, "10000")
        .addParam("role", AST_STRING, CODEGEN_INT, "0")
        .addParam("externRole", AST_STRING, CODEGEN_STRING, "")
        .addParam("strength", AST_INT, CODEGEN_STRING, "1")
        .addParam("patrol", AST_LIST_POINT, CODEGEN_LIST_PATROL_POINT, "[]");
const InstructionConfig addDropItemConfig =
    InstructionConfig("addDropItem", "新增放置可拾取道具")
        .addParam("itemCode", AST_STRING, CODEGEN_STRING, "")
        .addParam("x", AST_INT, CODEGEN_STRING, "0")
        .addParam("y", AST_INT, CODEGEN_STRING, "0")
        .addParam("range", AST_INT, CODEGEN_STRING, "0")
        .addParam("scale", AST_INT, CODEGEN_STRING, "1")
        .addParam("type", AST_STRING, CODEGEN_STRING, "paper")
        .addParam("localVarname", AST_STRING, CODEGEN_STRING, "");
const InstructionConfig addMapObjectConfig =
    InstructionConfig("addMapObject", "新增地圖物件")
        .addParam("x", AST_INT, CODEGEN_STRING, "0")
        .addParam("y", AST_INT, CODEGEN_STRING, "0")
        .addParam("range", AST_INT, CODEGEN_STRING, "0")
        .addParam("object", AST_STRING, CODEGEN_STRING, "tree0")
        .addParam("autoTuneHeight", AST_BOOL, CODEGEN_BOOL, "true")
        .addParam("walkable", AST_BOOL, CODEGEN_BOOL, "true");
const InstructionConfig addMapSignConfig =
    InstructionConfig("addMapSign", "新增告示牌")
        .addParam("text", AST_STRING, CODEGEN_STRING, "")
        .addParam("buttons", AST_LIST_BUTTON, CODEGEN_LIST_BUTTON, "[]")
        .addParam("x", AST_INT, CODEGEN_STRING, "0")
        .addParam("y", AST_INT, CODEGEN_STRING, "0")
        .addParam("range", AST_INT, CODEGEN_STRING, "0")
        .addParam("rotation", AST_INT, CODEGEN_STRING, "0")
        .addParam("showButtons", AST_BOOL, CODEGEN_BOOL, "true")
        .setCodegenName("AddMapSIgn");
const InstructionConfig addStuffConfig =
    InstructionConfig("addStuff", "新增武器道具")
        .addParam("code", AST_STRING, CODEGEN_STRING, "item*")
        .addParam("item", AST_STRING, CODEGEN_STRING, "magazine")
        .addParam("refill", AST_BOOL, CODEGEN_BOOL, "true")
        .addParam("refillInterval", AST_INT, CODEGEN_STRING, "10000")
        .addParam("x", AST_INT, CODEGEN_STRING, "0")
        .addParam("y", AST_INT, CODEGEN_STRING, "0")
        .addParam("range", AST_INT, CODEGEN_STRING, "0")
        .addParam("rotation", AST_INT, CODEGEN_STRING, "0");
const InstructionConfig avoidFriendFireConfig =
    InstructionConfig("avoidFriendFire", "迴避友軍攻擊")
        .addParam("value", AST_BOOL, CODEGEN_BOOL, "true")
        .setCodegenName("GameRule");
const InstructionConfig deltaHpConfig =
    InstructionConfig("deltaHp", "角色加減血")
        .addParam("actorCode", AST_STRING, CODEGEN_STRING, "")
        .addParam("type", AST_STRING, CODEGEN_STRING, "heal")
        .addParam("value", AST_INT, CODEGEN_STRING, "")
        .addParam("casterCode", AST_STRING, CODEGEN_STRING, "");
const InstructionConfig enblastEffectConfig =
    InstructionConfig("enblastEffect", "光彈特效")
        .addParam("fromType", AST_STRING, CODEGEN_STRING, "actor")
        .addParam("fromActor", AST_STRING, CODEGEN_STRING, "instance")
        .addParam("toType", AST_STRING, CODEGEN_STRING, "angle")
        .addParam("toAngle", AST_INT, CODEGEN_STRING, "0")
        .addParam("damage", AST_INT, CODEGEN_STRING, "30")
        .addParam("scale", AST_INT, CODEGEN_STRING, "1")
        .addParam("speed", AST_INT, CODEGEN_STRING, "0.7");
const InstructionConfig equipWeaponConfig =
    InstructionConfig("equipWeapon", "人物裝備武器")
        .addParam("actorCode", AST_STRING, CODEGEN_STRING, "")
        .addParam("type", AST_STRING, CODEGEN_STRING, "fist")
        .addParam("hand", AST_INT, CODEGEN_INT, "-1")
        .addParam("isDefault", AST_BOOL, CODEGEN_BOOL, "true");
const InstructionConfig getCookieConfig =
    InstructionConfig("getCookie", "取得Cookies")
        .addParam("cookies", AST_STRING, CODEGEN_STRING, "")
        .addParam("varName", AST_STRING, CODEGEN_STRING, "");
const InstructionConfig getUserStateConfig =
    InstructionConfig("getUserState", "取得玩家狀態")
        .addParam("playerId", AST_STRING, CODEGEN_STRING, "")
        .addParam("category", AST_STRING, CODEGEN_STRING, "")
        .addParam("key", AST_STRING, CODEGEN_STRING, "")
        .addParam("varName", AST_STRING, CODEGEN_STRING, "");
const InstructionConfig mapWarpConfig =
    InstructionConfig("mapWarp", "設定地圖傳送點")
        .addParam("fromX", AST_INT, CODEGEN_STRING, "0")
        .addParam("fromY", AST_INT, CODEGEN_STRING, "0")
        .addParam("toX", AST_INT, CODEGEN_STRING, "0")
        .addParam("toY", AST_INT, CODEGEN_STRING, "0")
        .addParam("direction", AST_STRING, CODEGEN_STRING, "right");
const InstructionConfig missionCompleteConfig =
    InstructionConfig("missionComplete", "任務完成")
        .addParam("camp", AST_STRING, CODEGEN_STRING, "all");
const InstructionConfig longBoConfig =
    InstructionConfig("longBo", "龍波")
        .addParam("actorCode", AST_STRING, CODEGEN_STRING, "");
const InstructionConfig printConfig =
    InstructionConfig("print", "控制台輸出")
        .addParam("type", AST_STRING, CODEGEN_STRING, "log")
        .addParam("text", AST_STRING, CODEGEN_STRING, "")
        .setCodegenName("Console");
const InstructionConfig removeDeviceConfig =
    InstructionConfig("removeDevice", "移除地圖機關")
        .addParam("region_x", AST_INT, CODEGEN_STRING, "0")
        .addParam("region_y", AST_INT, CODEGEN_STRING, "0")
        .addParam("region_w", AST_INT, CODEGEN_STRING, "0")
        .addParam("region_h", AST_INT, CODEGEN_STRING, "0");
const InstructionConfig removeMapObjectConfig =
    InstructionConfig("removeMapObject", "移除地圖物件")
        .addParam("region_x", AST_INT, CODEGEN_STRING, "0")
        .addParam("region_y", AST_INT, CODEGEN_STRING, "0")
        .addParam("region_w", AST_INT, CODEGEN_STRING, "0")
        .addParam("region_h", AST_INT, CODEGEN_STRING, "0");
const InstructionConfig setCookieConfig =
    InstructionConfig("setCookie", "儲存Cookies")
        .addParam("playerId", AST_STRING, CODEGEN_STRING, "")
        .addParam("cookies", AST_STRING, CODEGEN_STRING, "")
        .addParam("type", AST_STRING, CODEGEN_STRING, "string")
        .addParam("value", AST_STRING, CODEGEN_STRING, "");
const InstructionConfig setGlobalConfig =
    InstructionConfig("setGlobal", "儲存全域變數")
        .addParam("key", AST_STRING, CODEGEN_STRING, "")
        .addParam("type", AST_STRING, CODEGEN_STRING, "string")
        .addParam("value", AST_INT, CODEGEN_STRING, "");
const InstructionConfig setObjectVarConfig =
    InstructionConfig("setObjectVar", "儲存物件變數")
        .addParam("object", AST_STRING, CODEGEN_STRING, "")
        .addParam("key", AST_STRING, CODEGEN_STRING, "")
        .addParam("type", AST_STRING, CODEGEN_STRING, "string")
        .addParam("value", AST_INT, CODEGEN_STRING, "");
const InstructionConfig setUserStateConfig =
    InstructionConfig("setUserState", "儲存玩家狀態")
        .addParam("playerId", AST_STRING, CODEGEN_STRING, "")
        .addParam("category", AST_STRING, CODEGEN_STRING, "")
        .addParam("key", AST_STRING, CODEGEN_STRING, "")
        .addParam("type", AST_STRING, CODEGEN_STRING, "")
        .addParam("value", AST_STRING, CODEGEN_STRING, "");
const InstructionConfig setWeaponAbilityConfig =
    InstructionConfig("setWeaponAbility", "設定武器技能")
        .addParam("weapon", AST_STRING, CODEGEN_STRING, "")
        .addParam("level", AST_INT, CODEGEN_INT, "1")
        .addParam("operation", AST_STRING, CODEGEN_STRING, "set")
        .addParam("ability", AST_STRING, CODEGEN_STRING, "sounded");
const InstructionConfig tipOnMapConfig =
    InstructionConfig("tipOnMap", "地圖標示文字")
        .addParam("x", AST_INT, CODEGEN_STRING, "0")
        .addParam("y", AST_INT, CODEGEN_STRING, "0")
        .addParam("duration", AST_INT, CODEGEN_STRING, "3000")
        .addParam("text", AST_STRING, CODEGEN_STRING, "")
        .addParam("html", AST_BOOL, CODEGEN_BOOL, "false");
const InstructionConfig waitConfig =
    InstructionConfig("wait", "等待")
        .addParam("duration", AST_INT, CODEGEN_STRING, "0");
// EnhFF Module
const InstructionConfig EnhFFPlayerMousePositionConfig =
    InstructionConfig("EnhFF", "playerMousePosition", "玩家滑鼠座標")
        .addParam("actorId", AST_STRING, CODEGEN_STRING, "")
        .addParam("varX", AST_STRING, CODEGEN_STRING, "x")
        .addParam("varY", AST_STRING, CODEGEN_STRING, "y")
        .setCodegenName("PlayerMousePositionEnh");
const InstructionConfig EnhFFGeneralCircularRangeConfig =
    InstructionConfig("EnhFF", "generalCircularRange", "廣義圓形範圍")
        .addParam("actorId", AST_STRING, CODEGEN_STRING, "")
        // If actorId is filled. `actor` mode will be used
        // Otherwise, `map` mode will be used
        .addParam("x", AST_INT, CODEGEN_STRING, "0")
        .addParam("y", AST_INT, CODEGEN_STRING, "0")
        .addParam("radius", AST_INT, CODEGEN_STRING, "0")
        .addParam("offsetX", AST_INT, CODEGEN_STRING, "0")
        .addParam("offsetY", AST_INT, CODEGEN_STRING, "0")
        .addParam("duration", AST_INT, CODEGEN_STRING, "10000")
        .addParam("color", AST_STRING, CODEGEN_STRING, "#ffffff")
        .addParam("lineWidth", AST_INT, CODEGEN_STRING, "1")
        .addParam("deltaHpType", AST_STRING, CODEGEN_STRING, "heal")
        .addParam("deltaHpValue", AST_INT, CODEGEN_STRING, "")
        .addParam("deltaHpCD", AST_INT, CODEGEN_STRING, "10000")
        .addParam("deltaHpTarget", AST_ACTOR_MATCH, CODEGEN_ENHFF_ACTOR_MATCH,
                  "[]")
        .addParam("deltaHpCasterCode", AST_STRING, CODEGEN_STRING, "")
        .setCodegenName("GeneralCircularRangeEnhFF");

const std::vector<InstructionConfig> actionList = {
    // Standard Module
    actorAttributesConfig,
    actorDisappearConfig,
    actorFollowConfig,
    actorRelocateConfig,
    actorSpawnLocConfig,
    actorTalkConfig,
    addActorConfig,
    addDropItemConfig,
    addMapObjectConfig,
    addMapSignConfig,
    addStuffConfig,
    avoidFriendFireConfig,
    deltaHpConfig,
    enblastEffectConfig,
    equipWeaponConfig,
    getCookieConfig,
    getUserStateConfig,
    mapWarpConfig,
    missionCompleteConfig,
    longBoConfig,
    printConfig,
    removeDeviceConfig,
    removeMapObjectConfig,
    setCookieConfig,
    setGlobalConfig,
    setObjectVarConfig,
    setUserStateConfig,
    setWeaponAbilityConfig,
    tipOnMapConfig,
    waitConfig,
    EnhFFPlayerMousePositionConfig,
    EnhFFGeneralCircularRangeConfig,
};

const InstructionConfig actorCountConfig =
    InstructionConfig("actorCount", "計算人數")
        .addParam("actor", AST_ACTOR_MATCH, CODEGEN_ACTOR_MATCH, "[]")
        .addParam("varname", AST_STRING, CODEGEN_STRING, "")
        .addParam("op", AST_STRING, CODEGEN_STRING, "==")
        .addParam("value", AST_INT, CODEGEN_STRING, "")
        .addParam("region_x", AST_INT, CODEGEN_STRING, "")
        .addParam("region_y", AST_INT, CODEGEN_STRING, "")
        .addParam("region_w", AST_INT, CODEGEN_STRING, "")
        .addParam("region_h", AST_INT, CODEGEN_STRING, "");
const InstructionConfig actorRegionConfig =
    InstructionConfig("actorRegion", "角色所在區域")
        .addParam("actorId", AST_STRING, CODEGEN_STRING, "")
        .addParam("checkAlive", AST_BOOL, CODEGEN_BOOL, "false")
        .addParam("region_x", AST_INT, CODEGEN_STRING, "0")
        .addParam("region_y", AST_INT, CODEGEN_STRING, "0")
        .addParam("region_w", AST_INT, CODEGEN_STRING, "0")
        .addParam("region_h", AST_INT, CODEGEN_STRING, "0");
const InstructionConfig checkNumberConfig =
    InstructionConfig("checkNumber", "比較數字")
        .addParam("lhs", AST_INT, CODEGEN_STRING, "0")
        .addParam("rhs", AST_INT, CODEGEN_STRING, "0")
        .addParam("op", AST_STRING, CODEGEN_STRING, "==")
        .setCodegenName("NumberCheck");
const InstructionConfig checkStringConfig =
    InstructionConfig("checkString", "比對字串")
        .addParam("value", AST_STRING, CODEGEN_STRING, "")
        .addParam("matchKind", AST_STRING, CODEGEN_STRING, "contain")
        .addParam("str", AST_STRING, CODEGEN_STRING, "")
        .setCodegenName("String");
const InstructionConfig forEachActorConfig =
    InstructionConfig("forEachActor", "找出所有角色")
        .addParam("actor", AST_ACTOR_MATCH, CODEGEN_ACTOR_MATCH, "[]")
        .addParam("varname", AST_STRING, CODEGEN_STRING, "");

const std::vector<InstructionConfig> checkList = {
    actorCountConfig,  actorRegionConfig,  checkNumberConfig,
    checkStringConfig, forEachActorConfig,
};

const InstructionConfig actorAddedConfig =
    InstructionConfig("actorAdded", "角色進入戰場")
        .addParam("actor", AST_ACTOR_MATCH, CODEGEN_ACTOR_MATCH, "[]")
        .addParam("varName", AST_STRING, CODEGEN_STRING, "");
const InstructionConfig actorDeadConfig =
    InstructionConfig("actorDead", "角色死亡")
        .addParam("actor", AST_ACTOR_MATCH, CODEGEN_ACTOR_MATCH, "[]")
        .addParam("varName", AST_STRING, CODEGEN_STRING, "")
        .addParam("hitterVarName", AST_STRING, CODEGEN_STRING, "");
const InstructionConfig actorFireConfig =
    InstructionConfig("actorFire", "角色發動攻擊")
        .addParam("actor", AST_ACTOR_MATCH, CODEGEN_ACTOR_MATCH, "[]")
        .addParam("varName", AST_STRING, CODEGEN_STRING, "")
        .addParam("weapon", AST_STRING, CODEGEN_STRING, "");
const InstructionConfig actorHitConfig =
    InstructionConfig("actorHit", "角色受傷")
        .addParam("actor", AST_ACTOR_MATCH, CODEGEN_ACTOR_MATCH, "[]")
        .addParam("hitter", AST_ACTOR_MATCH, CODEGEN_ACTOR_MATCH, "[]")
        .addParam("actorVarName", AST_STRING, CODEGEN_STRING, "")
        .addParam("hitterVarName", AST_STRING, CODEGEN_STRING, "")
        .addParam("damageValueVarName", AST_STRING, CODEGEN_STRING, "")
        .addParam("weapon", AST_STRING, CODEGEN_STRING, "");
const InstructionConfig clickButtonConfig =
    InstructionConfig("clickButton", "告示牌按鈕")
        .addParam("actor", AST_ACTOR_MATCH, CODEGEN_ACTOR_MATCH, "[]")
        .addParam("varName", AST_STRING, CODEGEN_STRING, "")
        .addParam("buttonId", AST_STRING, CODEGEN_STRING, "")
        .setCodegenName("TalkButton");
const InstructionConfig keyboardPressedConfig =
    InstructionConfig("keyboardPressed", "鍵盤按鍵")
        .addParam("actorId", AST_STRING, CODEGEN_STRING, "*")
        .addParam("varName", AST_STRING, CODEGEN_STRING, "")
        .addParam("timing", AST_STRING, CODEGEN_STRING, "")
        .addParam("key", AST_STRING, CODEGEN_INT, "65");
const InstructionConfig itemPickupConfig =
    InstructionConfig("itemPickup", "拾取武器道具")
        .addParam("actor", AST_ACTOR_MATCH, CODEGEN_ACTOR_MATCH, "[]")
        .addParam("actorVarname", AST_STRING, CODEGEN_STRING, "")
        .addParam("itemVarname", AST_STRING, CODEGEN_STRING, "")
        .addParam("matchKind", AST_STRING, CODEGEN_STRING, "contain")
        .addParam("itemMatchCode", AST_STRING, CODEGEN_STRING, "");
const InstructionConfig mouseEventConfig =
    InstructionConfig("mouseEvent", "滑鼠點擊")
        .addParam("actorId", AST_STRING, CODEGEN_STRING, "*")
        .addParam("actorVarName", AST_STRING, CODEGEN_STRING, "")
        .addParam("XVarName", AST_STRING, CODEGEN_STRING, "")
        .addParam("YVarName", AST_STRING, CODEGEN_STRING, "");
const InstructionConfig releasePowerConfig =
    InstructionConfig("releasePower", "發動技能")
        .addParam("actor", AST_ACTOR_MATCH, CODEGEN_ACTOR_MATCH, "[]")
        .addParam("varName", AST_STRING, CODEGEN_STRING, "")
        .addParam("ability", AST_STRING, CODEGEN_STRING, "")
        .addParam("preventDefault", AST_BOOL, CODEGEN_STRING, "false")
        .addParam("manaUsage", AST_INT, CODEGEN_STRING, "0")
        .addParam("weapon", AST_STRING, CODEGEN_STRING, "");

const std::vector<InstructionConfig> triggerList = {
    actorAddedConfig, actorDeadConfig,   actorFireConfig,
    actorHitConfig,   clickButtonConfig, keyboardPressedConfig,
    itemPickupConfig, mouseEventConfig,  releasePowerConfig,
};
} // namespace config

#endif // CONFIG_INSTRUCTION_H