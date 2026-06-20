#include "codegen.h"
#include "ast.h"
#include "instruction.h"
#include "instruction/action.h"
#include "instruction/check.h"
#include "instruction/metadata.h"
#include "instruction/trigger.h"
#include "keyword.h"
#include "utils/utils.h"
#include <fstream>
#include <iostream>

using namespace codegen;

void CodeGenerator::codegen(std::string filepath) {
  std::ofstream outputFile(filepath);
  if (!outputFile)
    return;
  preCodegen();
  codegenModuleNode(outputFile);
  outputFile.close();
}

void CodeGenerator::preCodegen() {
  for (auto &metadata : moduleNode->metadatas) {
    if (metadata->key != keyword::metadataKind::customWeapons)
      continue;
    auto listNode =
        dynamic_cast<ListValueNode *>(metadata->expNode->value.get());
    if (!listNode)
      continue;
    for (auto &item : listNode->items) {
      auto cwNode = dynamic_cast<CustomWeaponValueNode *>(item->value.get());
      if (!cwNode)
        continue;
      for (auto &namedArg : cwNode->paramApps->named_args)
        if (namedArg->key == "code") {
          auto stringNode =
              dynamic_cast<StringValueNode *>(namedArg->expNode->value.get());
          if (!stringNode)
            break;
          string keyName = "custom_" + stringNode->value;
          userDefinedMeta.customWeaponsKeywordEnum.insert({keyName, keyName});
        }
    }
  }
}

void CodeGenerator::codegenModuleNode(std::ofstream &of) {
  of << "{" << std::endl;
  metadata::ConfigSetup::setup(of, moduleNode->metadatas);
  codegenBlocks(of);
  of << "}" << std::endl;
}

void CodeGenerator::codegenBlocks(std::ofstream &of) {
  of << inden(4) << "\"events\": [";
  if (moduleNode->blocks.size() == 0) {
    of << "]" << std::endl;
    return;
  }
  of << std::endl;
  for (auto i = 0; i < moduleNode->blocks.size(); i++) {
    auto &block = moduleNode->blocks[i];
    of << inden(8) << "{" << std::endl;
    of << inden(12) << "\"id\": \"" << block->identifier << "\"," << std::endl;
    metadata::BlockSetup::setup(of, block->blockBody->metadatas);
    codegenTypedInstrSet(of, block->blockBody->getActions());
    codegenTypedInstrSet(of, block->blockBody->getChecks());
    codegenTypedInstrSet(of, block->blockBody->getTriggers());
    of << inden(8) << "}";
    if (i != moduleNode->blocks.size() - 1)
      of << ",";
    of << std::endl;
  }
  of << inden(4) << "]" << std::endl;
}

void CodeGenerator::codegenTypedInstrSet(
    std::ofstream &of, std::unique_ptr<TypedInstrSetNode> &typedInstrSet) {
  std::string endingStr =
      typedInstrSet->type == FUN_DEF_TYPE_TRIGGERS ? "]\n" : "],\n";
  auto &instructions = typedInstrSet->instrSet->instructions;
  of << inden(12) << "\"" << typedInstrSet->type << "\": [";
  if (instructions.empty()) {
    of << endingStr;
    return;
  }
  of << std::endl;
  for (auto i = 0; i < instructions.size(); i++) {
    of << inden(16) << "{\n";
    of << inden(20) << "\"type\": ";
    switch (typedInstrSet->type) {
    case FUN_DEF_TYPE_ACTIONS:
      codegenAction(of, instructions[i]->instruction);
      break;
    case FUN_DEF_TYPE_CHECKS:
      codegenCheck(of, instructions[i]->instruction);
      break;
    case FUN_DEF_TYPE_TRIGGERS:
      codegenTrigger(of, instructions[i]->instruction);
      break;
    default:
      break;
    }
    of << "\n" << inden(16) << "}";
    if (i != instructions.size() - 1)
      of << ",";
    of << std::endl;
  }
  of << inden(12) << endingStr;
}

#define CODEGEN_FUNC method(defaultMap, userDefinedMeta).to_string(20)

void CodeGenerator::codegenAction(std::ofstream &of,
                                  std::unique_ptr<InstructionNode> &instrNode) {
  auto configMap = configVecToMap(config::actionList);
  std::string identifier = instrNode->identifier;
  if (configMap.find(identifier) == configMap.end()) {
    std::cerr << "Codegen error: Cannot found the corresponding action name \""
              << identifier << "\" at " << instrNode.get()->loc << "\n";
    return;
  }
  auto config = configMap.at(identifier);
  DefaultMap defaultMap = DefaultMap(config, instrNode->paramApps->named_args);
  of << "\"" << config.codegenName << "\",\n";
  of << inden(20) << "\"data\": ";
  // Standard Module
  if (identifier == "actorAttributes" || identifier == "設定角色屬性")
    of << action::ActionActorAttributes::CODEGEN_FUNC;
  if (identifier == "actorDisappear" || identifier == "角色消失")
    of << action::ActionActorDisappear::CODEGEN_FUNC;
  if (identifier == "actorFollow" || identifier == "跟隨人物")
    of << action::ActionActorFollow::CODEGEN_FUNC;
  if (identifier == "actorRelocate" || identifier == "移動角色位置")
    of << action::ActionActorRelocate::CODEGEN_FUNC;
  if (identifier == "actorSpawnLoc" || identifier == "角色重生位置")
    of << action::ActionActorSpawnLoc::CODEGEN_FUNC;
  if (identifier == "actorTalk" || identifier == "角色說話")
    of << action::ActionActorTalk::CODEGEN_FUNC;
  if (identifier == "addActor" || identifier == "新增角色")
    of << action::ActionAddActor::CODEGEN_FUNC;
  if (identifier == "addDropItem" || identifier == "新增放置可拾取道具")
    of << action::ActionAddDropItem::CODEGEN_FUNC;
  if (identifier == "addMapSign" || identifier == "新增告示牌")
    of << action::ActionAddMapSign::CODEGEN_FUNC;
  if (identifier == "addMapObject" || identifier == "新增地圖物件")
    of << action::ActionAddMapObject::CODEGEN_FUNC;
  if (identifier == "addStuff" || identifier == "新增武器道具")
    of << action::ActionAddStuff::CODEGEN_FUNC;
  if (identifier == "avoidFriendFire" || identifier == "迴避友軍攻擊")
    of << action::ActionAvoidFriendFire::CODEGEN_FUNC;
  if (identifier == "closeDialog" || identifier == "關閉彈出視窗")
    of << action::ActionCloseDialog::CODEGEN_FUNC;
  if (identifier == "deltaHp" || identifier == "角色加減血")
    of << action::ActionDeltaHp::CODEGEN_FUNC;
  if (identifier == "enblastEffect" || identifier == "光彈特效")
    of << action::ActionEnblastEffect::CODEGEN_FUNC;
  if (identifier == "equipWeapon" || identifier == "人物裝備武器")
    of << action::ActionEquipWeapon::CODEGEN_FUNC;
  if (identifier == "getCookie" || identifier == "取得Cookies")
    of << action::ActionGetCookie::CODEGEN_FUNC;
  if (identifier == "getUserState" || identifier == "取得玩家狀態")
    of << action::ActionGetUserState::CODEGEN_FUNC;
  if (identifier == "mapWarp" || identifier == "設定地圖傳送點")
    of << action::ActionMapWarp::CODEGEN_FUNC;
  if (identifier == "missionComplete" || identifier == "任務完成")
    of << action::ActionMissionComplete::CODEGEN_FUNC;
  if (identifier == "longBo" || identifier == "龍波")
    of << action::ActionLongBo::CODEGEN_FUNC;
  if (identifier == "print" || identifier == "控制台輸出")
    of << action::ActionConsole::CODEGEN_FUNC;
  if (identifier == "removeDevice" || identifier == "移除地圖機關")
    of << action::ActionRemoveDevice::CODEGEN_FUNC;
  if (identifier == "removeMapObject" || identifier == "移除地圖物件")
    of << action::ActionRemoveMapObject::CODEGEN_FUNC;
  if (identifier == "setCookie" || identifier == "儲存Cookies")
    of << action::ActionSetCookie::CODEGEN_FUNC;
  if (identifier == "setGlobal" || identifier == "儲存全域變數")
    of << action::ActionSetGlobal::CODEGEN_FUNC;
  if (identifier == "setObjectVar" || identifier == "儲存物件變數")
    of << action::ActionSetObjectVar::CODEGEN_FUNC;
  if (identifier == "setUserState" || identifier == "儲存玩家狀態")
    of << action::ActionSetUserState::CODEGEN_FUNC;
  if (identifier == "setWeaponAbility" || identifier == "設定武器技能")
    of << action::ActionSetWeaponAbility::CODEGEN_FUNC;
  if (identifier == "showSelectDialog" || identifier == "彈出選項視窗")
    of << action::ActionShowSelectDialog::CODEGEN_FUNC;
  if (identifier == "tipOnMap" || identifier == "地圖標示文字")
    of << action::ActionTipOnMap::CODEGEN_FUNC;
  if (identifier == "wait" || identifier == "等待")
    of << action::ActionWait::CODEGEN_FUNC;
  // EnhFF Module
  if (identifier == "EnhFF::playerMousePosition" ||
      identifier == "EnhFF::玩家滑鼠座標")
    of << action::ActionEnhFFPlayerMousePosition::CODEGEN_FUNC;
  if (identifier == "EnhFF::generalCircularRange" ||
      identifier == "EnhFF::廣義圓形範圍")
    of << action::ActionEnhFFGeneralCircularRange::CODEGEN_FUNC;
  return;
}

void CodeGenerator::codegenCheck(std::ofstream &of,
                                 std::unique_ptr<InstructionNode> &instrNode) {
  auto configMap = configVecToMap(config::checkList);
  std::string identifier = instrNode->identifier;
  if (configMap.find(identifier) == configMap.end()) {
    std::cerr << "Codegen error: Cannot found the corresponding check name \""
              << identifier << "\" at " << instrNode.get()->loc << "\n";
    return;
  }
  auto config = configMap.at(identifier);
  DefaultMap defaultMap = DefaultMap(config, instrNode->paramApps->named_args);
  of << "\"" << config.codegenName << "\",\n";
  of << inden(20) << "\"data\": ";
  if (identifier == "actorCount" || identifier == "計算人數")
    of << check::CheckActorCount::CODEGEN_FUNC;
  if (identifier == "actorRegion" || identifier == "角色所在區域")
    of << check::CheckActorRegion::CODEGEN_FUNC;
  if (identifier == "forEachActor" || identifier == "找出所有角色")
    of << check::CheckForEachActor::CODEGEN_FUNC;
  if (identifier == "checkString" || identifier == "比對字串")
    of << check::CheckString::CODEGEN_FUNC;
  if (identifier == "checkNumber" || identifier == "比較數字")
    of << check::CheckNumber::CODEGEN_FUNC;
  return;
}

void CodeGenerator::codegenTrigger(
    std::ofstream &of, std::unique_ptr<InstructionNode> &instrNode) {
  auto configMap = configVecToMap(config::triggerList);
  std::string identifier = instrNode->identifier;
  if (configMap.find(identifier) == configMap.end()) {
    std::cerr << "Codegen error: Cannot found the corresponding trigger name \""
              << identifier << "\" at " << instrNode.get()->loc << "\n";
    return;
  }
  auto config = configMap.at(identifier);
  DefaultMap defaultMap = DefaultMap(config, instrNode->paramApps->named_args);
  of << "\"" << config.codegenName << "\",\n";
  of << inden(20) << "\"data\": ";
  if (identifier == "actorAdded" || identifier == "角色進入戰場")
    of << trigger::TriggerActorAdded::CODEGEN_FUNC;
  if (identifier == "actorDead" || identifier == "角色死亡")
    of << trigger::TriggerActorDead::CODEGEN_FUNC;
  if (identifier == "actorFire" || identifier == "角色發動攻擊")
    of << trigger::TriggerActorFire::CODEGEN_FUNC;
  if (identifier == "actorHit" || identifier == "角色受傷")
    of << trigger::TriggerActorHit::CODEGEN_FUNC;
  if (identifier == "clickButton" || identifier == "告示牌按鈕")
    of << trigger::TriggerClickButton::CODEGEN_FUNC;
  if (identifier == "dialogConfirm" || identifier == "視窗確認")
    of << trigger::TriggerDialogConfirm::CODEGEN_FUNC;
  if (identifier == "keyboardPressed" || identifier == "鍵盤按鍵")
    of << trigger::TriggerKeyboardPressed::CODEGEN_FUNC;
  if (identifier == "itemPickup" || identifier == "拾取武器道具")
    of << trigger::TriggerItemPickup::CODEGEN_FUNC;
  if (identifier == "mouseEvent" || identifier == "滑鼠點擊")
    of << trigger::TriggerMouseEvent::CODEGEN_FUNC;
  if (identifier == "releasePower" || identifier == "發動技能")
    of << trigger::TriggerReleasePower::CODEGEN_FUNC;
  return;
}
#undef CODEGEN_FUNC
