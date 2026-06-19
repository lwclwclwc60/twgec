#include "codegen/instruction/trigger.h"
#include "keyword.h"
#include "utils/defaultMap.h"
#include "utils/formatter.h"

using namespace codegen;
using namespace formatter;
using namespace keyword;

JsonObjectNode
trigger::TriggerActorAdded::method(DefaultMap defaultMap,
                                   UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"actorMatches", defaultMap.get("actor")},
      {"varname", defaultMap.get("varName")},
  });
}

JsonObjectNode
trigger::TriggerActorDead::method(DefaultMap defaultMap,
                                  UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"actorMatches", defaultMap.get("actor")},
      {"varname", defaultMap.get("varName")},
      {"timing", "\"preDead\""},
      {"deathsVarname", "\"deaths\""},
      {"hitterMatches", "[]"},
      {"hitterVarname", defaultMap.get("hitterVarName")},
  });
}

JsonObjectNode
trigger::TriggerActorFire::method(DefaultMap defaultMap,
                                  UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorMatches", defaultMap.get("actor")},
      {"varname", defaultMap.get("varName")},
      {"fireTriggerType", "\"all\""},
      {"damageTypes", "[]"},
  });
  if (defaultMap.get("weapon") != "\"\"") {
    dataNode.addNode("checkWeaponType", "true")
        .addNode("weaponTypeCheck", "\"include\"")
        .addNode("weaponType", defaultMap.get("weapon"));
  }
  return dataNode;
}

JsonObjectNode
trigger::TriggerActorHit::method(DefaultMap defaultMap,
                                 UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorMatches", defaultMap.get("actor")},
      {"varname", defaultMap.get("actorVarName")},
      {"hitterMatches", defaultMap.get("hitter")},
      {"hitterVarname", defaultMap.get("hitterVarName")},
      {"damageTypes", "[]"},
      {"damageVarname", defaultMap.get("damageValueVarName")},
      {"aliveType", "\"skip\""},
      {"checkFallDmg", "\"a\""},
      {"checkDeviceDmg", "\"a\""},
  });
  if (defaultMap.get("weapon") != "\"\"") {
    dataNode.addNode("checkWeaponType", "true")
        .addNode("weaponTypeCheck", "\"include\"")
        .addNode("weaponType", defaultMap.get("weapon"));
  }
  return dataNode;
}

JsonObjectNode
trigger::TriggerClickButton::method(DefaultMap defaultMap,
                                    UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"actorMatches", defaultMap.get("actor")},
      {"varname", defaultMap.get("varName")},
      {"checkButtoncode", "true"},
      {"buttonCode", defaultMap.get("buttonId")},
      {"varnameDevice", "\"\""},
  });
}

JsonObjectNode
trigger::TriggerDialogConfirm::method(DefaultMap defaultMap,
                                      UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"dialogId", "\"\""},
      {"playerCodeVarname", defaultMap.get("playerLocalId")},
      {"value", defaultMap.get("buttonName")},
      {"evtLocal", "\"\""},
  });
}

JsonObjectNode
trigger::TriggerKeyboardPressed::method(DefaultMap defaultMap,
                                        UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"playerId", defaultMap.get("actorId")},
      // `{instance.id}` is needed for parsing the playerLocal
      {"playerLocal", defaultMap.get("varName")},
      {"timing", defaultMap.get("timing", keyPressTimingKind::keywordEnum)},
      {"key", defaultMap.get("key", keyPressKeyKind::keywordEnum)},
      {"keyLocal", "\"\""},
      {"ctrl", "\"\""},
      {"shift", "\"\""},
  });
}

JsonObjectNode
trigger::TriggerItemPickup::method(DefaultMap defaultMap,
                                   UserDefinedMetadata userDefinedMeta) {
  JsonObjectNode itemMatchNode = JsonObjectNode({
      {"method", defaultMap.get("matchKind", matchKind::keywordEnum)},
      {"itemCode", defaultMap.get("itemMatchCode")},
      {"itemType", "\"\""},
  });
  return JsonObjectNode({
      {"actorMatches", defaultMap.get("actor")},
      {"varname", defaultMap.get("actorVarname")},
      {"itemMatch", itemMatchNode.to_string(24)},
      {"itemVarname", defaultMap.get("itemVarname")},
  });
}

JsonObjectNode
trigger::TriggerMouseEvent::method(DefaultMap defaultMap,
                                   UserDefinedMetadata userDefinedMeta) {
  return JsonObjectNode({
      {"playerId", defaultMap.get("actorId")},
      {"playerLocal", defaultMap.get("actorVarName")},
      {"eventType", "\"click\""},
      {"btn", "0"},
      {"localMouseX", defaultMap.get("XVarName")},
      {"localMouseY", defaultMap.get("YVarName")},
      {"displayId", "\"\""},
  });
}

JsonObjectNode
trigger::TriggerReleasePower::method(DefaultMap defaultMap,
                                     UserDefinedMetadata userDefinedMeta) {
  std::string ability = defaultMap.get("ability", abilityKind::keywordEnum);
  std::string weaponType =
      defaultMap.get("weapon", weaponKind::keywordEnum,
                     userDefinedMeta.customWeaponsKeywordEnum);

  JsonObjectNode abilityNode = JsonObjectNode({
      {"value", ability},
  });
  JsonObjectNode dataNode = JsonObjectNode({
      {"actorMatches", defaultMap.get("actor")},
      {"varname", defaultMap.get("varName")},
      {"preventDefault", defaultMap.get("preventDefault")},
      {"manaUsage", defaultMap.get("manaUsage")},
      {"checkAbilityLevel", "0"},
  });
  if (ability != "\"\"")
    dataNode.addNode("checkAbility", "true")
        .addNode("ability", abilityNode.to_string(24));
  if (weaponType != "\"\"")
    dataNode.addNode("checkWeaponType", "true")
        .addNode("weaponType", weaponType);
  return dataNode;
}
