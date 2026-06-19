#ifndef KEYWORD_H
#define KEYWORD_H

#include <map>
#include <string>

using std::map;
using std::string;

namespace keyword {

class KeywordEnum {
public:
  // Variables
  const string enumType;
  const map<string, string> codegenMap;
  // Constructor
  KeywordEnum(const string enumType, const map<string, string> codegenMap)
      : enumType(enumType), codegenMap(codegenMap) {};
  // Functions
  const std::pair<bool, string> get(string key,
                                    std::map<string, string> extraEnum = {});
  bool isEmpty();
};

namespace campKind {
const string keywordSkydow = "skydow";
const string keywordSkydowZh = "天影";
const string keywordRoyal = "royal";
const string keywordRoyalZh = "皇家";
const string keywordThird = "third";
const string keywordThirdZh = "第三";
const string keywordCivil = "civil";
const string keywordCivilZh = "平民";
static KeywordEnum keywordEnum("camp kind",
                               {
                                   {keywordSkydow, keywordSkydow},
                                   {keywordSkydowZh, keywordSkydow},
                                   {keywordRoyal, keywordRoyal},
                                   {keywordRoyalZh, keywordRoyal},
                                   {keywordThird, keywordThird},
                                   {keywordThirdZh, keywordThird},
                                   {keywordCivil, keywordCivil},
                                   {keywordCivilZh, keywordCivil},
                               });
} // namespace campKind

namespace campMissionCompleteKind {
const string keywordSkydow = "skydow";
const string keywordSkydowZh = "天影";
const string keywordRoyal = "royal";
const string keywordRoyalZh = "皇家";
const string keywordThird = "third";
const string keywordThirdZh = "第三";
const string keywordAll = "all";
const string keywordAllZh = "所有";
static KeywordEnum keywordEnum("camp mission complete kind",
                               {
                                   {keywordSkydow, keywordSkydow},
                                   {keywordSkydowZh, keywordSkydow},
                                   {keywordRoyal, keywordRoyal},
                                   {keywordRoyalZh, keywordRoyal},
                                   {keywordThird, keywordThird},
                                   {keywordThirdZh, keywordThird},
                                   {keywordAll, keywordAll},
                                   {keywordAllZh, keywordAll},
                               });
} // namespace campMissionCompleteKind

namespace weaponKind {
const string keywordDefault = "default";
const string keywordKongFu = "kongfu";
const string keywordFist = "fist";
const string keywordFistZh = "拳頭";
const string keywordKnife = "knife";
const string keywordKnifeZh = "小刀";
const string keywordHook = "hook";
const string keywordHookZh = "爪子";
const string keywordSword = "sword";
const string keywordSwordZh = "大刀";
const string keywordShield = "shield";
const string keywordShieldZh = "盾牌";
const string keywordHandgun = "handgun";
const string keywordHandgunZh = "手槍";
const string keywordRifle = "rifle";
const string keywordRifleZh = "步槍";
const string keywordSnipegun = "snipegun";
const string keywordSnipegunZh = "狙擊槍";
const string keywordShotgun = "shotgun";
const string keywordShotgunZh = "霰彈槍";
const string keywordMinigun = "minigun";
const string keywordMinigunZh = "迷你砲機槍";
const string keywordBite = "bite";
const string keywordBiteZh = "屍爪";
const string keywordGiantAxe = "giantAxe";
const string keywordGiantAxeZh = "雙刃巨斧";
const string keywordIceSaber = "iceSaber";
const string keywordIceSaberZh = "冰劍";
const string keywordRocketLauncher = "rocketLauncher";
const string keywordRocketLauncherZh = "導航火箭炮";
const string keywordBowSet = "bowSet";
const string keywordBowSetZh = "水晶弓";
const string keywordDoubleGuns = "doubleGuns";
const string keywordDoubleGunsZh = "白銀雙鷹";
const string keywordLaserGun = "laserGun";
const string keywordLaserGunZh = "激光槍";
const string keywordAnaestheticRifle = "anaestheticRifle";
const string keywordAnaestheticRifleZh = "麻醉槍";
const string keywordShurikens = "shurikens";
const string keywordShurikensZh = "手裏劍";
const string keywordNunchaku = "nunchaku";
const string keywordNunchakuZh = "雙截刃";
const string keywordScythe = "scythe";
const string keywordScytheZh = "飛鎖鐮刀";
static KeywordEnum
    keywordEnum("weapon kind",
                {
                    {keywordDefault, keywordDefault},
                    {keywordKongFu, keywordKongFu},
                    {keywordFist, keywordKongFu},
                    {keywordFistZh, keywordKongFu},
                    {keywordKnife, keywordKnife},
                    {keywordKnifeZh, keywordKnife},
                    {keywordHook, keywordHook},
                    {keywordHookZh, keywordHook},
                    {keywordSword, keywordSword},
                    {keywordSwordZh, keywordSword},
                    {keywordShield, keywordShield},
                    {keywordShieldZh, keywordShield},
                    {keywordHandgun, keywordHandgun},
                    {keywordHandgunZh, keywordHandgun},
                    {keywordRifle, keywordRifle},
                    {keywordRifleZh, keywordRifle},
                    {keywordSnipegun, keywordSnipegun},
                    {keywordSnipegunZh, keywordSnipegun},
                    {keywordShotgun, keywordShotgun},
                    {keywordShotgunZh, keywordShotgun},
                    {keywordMinigun, keywordMinigun},
                    {keywordMinigunZh, keywordMinigun},
                    {keywordBite, keywordBite},
                    {keywordBiteZh, keywordBite},
                    {keywordGiantAxe, keywordGiantAxe},
                    {keywordGiantAxeZh, keywordGiantAxe},
                    {keywordIceSaber, keywordIceSaber},
                    {keywordIceSaberZh, keywordIceSaber},
                    {keywordRocketLauncher, keywordRocketLauncher},
                    {keywordRocketLauncherZh, keywordRocketLauncher},
                    {keywordBowSet, keywordBowSet},
                    {keywordBowSetZh, keywordBowSet},
                    {keywordDoubleGuns, keywordDoubleGuns},
                    {keywordDoubleGunsZh, keywordDoubleGuns},
                    {keywordLaserGun, keywordLaserGun},
                    {keywordLaserGunZh, keywordLaserGun},
                    {keywordAnaestheticRifle, keywordAnaestheticRifle},
                    {keywordAnaestheticRifleZh, keywordAnaestheticRifle},
                    {keywordShurikens, keywordShurikens},
                    {keywordShurikensZh, keywordShurikens},
                    {keywordNunchaku, keywordNunchaku},
                    {keywordNunchakuZh, keywordNunchaku},
                    {keywordScythe, keywordScythe},
                    {keywordScytheZh, keywordScythe},
                });
} // namespace weaponKind

namespace roleKind {
const string keywordRole0 = "影俠";
const string keywordRole1 = "影娘";
const string keywordRole2 = "天影武士1";
const string keywordRole3 = "天影武士2";
const string keywordRole4 = "皇家騎士";
const string keywordRole5 = "公主騎士";
const string keywordRole6 = "皇家生化人";
const string keywordRole7 = "皇家機械人";
const string keywordRole8 = "男傭兵";
const string keywordRole9 = "女傭兵";
const string keywordRole10 = "黑森盜賊團首領";
const string keywordRole11 = "盜賊隊長";
const string keywordRole12 = "盜賊嘍囉";
const string keywordRole13 = "男平民";
const string keywordRole14 = "女平民";
const string keywordRole15 = "平民老人";
const string keywordRole16 = "平民小孩";
const string keywordRole17 = "天影士兵";
const string keywordRole18 = "皇家士兵";
const string keywordRole19 = "克拉瑪族男性";
const string keywordRole20 = "克拉瑪族女性";
const string keywordRole21 = "晶石怪";
const string keywordRole22 = "石怪";
const string keywordRole23 = "冰石怪";
const string keywordRole24 = "伊蒂絲";
const string keywordRole25 = "伊蒂絲(戴眼鏡)";
const string keywordRole26 = "冰兒";
const string keywordRole27 = "role27"; // 冰魂
const string keywordRole28 = "冰妖王";
const string keywordRole29 = "阿薩斯";
const string keywordRole30 = "研究員查理";
const string keywordRole31 = "冰魂";
const string keywordRole32 = "隱居士";
const string keywordRole33 = "馬茲席塔";
const string keywordRole34 = "隱之墨";
const string keywordRole35 = "隱心";
const string keywordRole36 = "七月教教主";
const string keywordRole37 = "馬茲席塔元精";
const string keywordRole38 = "馬茲席塔元靈";
const string keywordRole39 = "兔子";
const string keywordRole40 = "鹿";
const string keywordRole41 = "藍猩猩";
const string keywordRole42 = "憤怒藍猩猩";
const string keywordRole43 = "男傭兵2";
const string keywordRole44 = "女傭兵2";
const string keywordRole45 = "阿波羅元帥";
const string keywordRole46 = "亞提米絲";
const string keywordRole47 = "黑炎軍士";
const string keywordRole48 = "黑炎軍相";
const string keywordRole49 = "黑炎軍馬";
const string keywordRole50 = "黑炎軍卒";
const string keywordRole51 = "男靈能訓練官";
const string keywordRole52 = "女靈能訓練官";
const string keywordRole53 = "阿薩斯研究員";
const string keywordRole54 = "九方一色";
const string keywordRole55 = "靈骨";
const string keywordRole56 = "獵豹";
const string keywordRole57 = "隱形人";
const string keywordRole58 = "隱形精靈";
// 59 and 60 are missed
const string keywordRole61 = "破腦屍怪";
const string keywordRole62 = "影破腦";
const string keywordRole63 = "影破腦屍怪";
const string keywordRole64 = "肥腦屍怪";
const string keywordRole65 = "影肥腦屍怪";
const string keywordRole66 = "瘋狗屍";
const string keywordRole67 = "千年屍妖";
const string keywordRole68 = "噁吐屍妖";
const string keywordRole69 = "青面鬼";
const string keywordRole70 = "屍血蟲";
const string keywordRole71 = "role71"; // 躺平的阿波羅姐姐
const string keywordRole72 = "role72"; // 不認識
const string keywordRole73 = "role73"; // 赤焰精靈 (重複)
const string keywordRole74 = "熊";
const string keywordRole75 = "飛魚";
const string keywordRole76 = "赤焰王女";
const string keywordRole77 = "赤焰王女(躺下)";
const string keywordRole78 = "不動佛";
const string keywordRole79 = "赤焰精靈";
const string keywordRole80 = "火球";
const string keywordRole81 = "杭特博士";
const string keywordRole82 = "杭特博士(低頭)";
const string keywordRole83 = "崔酒中尉";
const string keywordRole84 = "role85"; // 老人
const string keywordRole85 = "role86"; // 白猩猩
const string keywordRole86 = "蜜蜂";
static KeywordEnum keywordEnum(
    "role kind",
    {
        {keywordRole0, "0"},   {keywordRole1, "1"},   {keywordRole2, "2"},
        {keywordRole3, "3"},   {keywordRole4, "4"},   {keywordRole5, "5"},
        {keywordRole6, "6"},   {keywordRole7, "7"},   {keywordRole8, "8"},
        {keywordRole9, "9"},   {keywordRole10, "10"}, {keywordRole11, "11"},
        {keywordRole12, "12"}, {keywordRole13, "13"}, {keywordRole14, "14"},
        {keywordRole15, "15"}, {keywordRole16, "16"}, {keywordRole17, "17"},
        {keywordRole18, "18"}, {keywordRole19, "19"}, {keywordRole20, "20"},
        {keywordRole21, "21"}, {keywordRole22, "22"}, {keywordRole23, "23"},
        {keywordRole24, "24"}, {keywordRole25, "25"}, {keywordRole26, "26"},
        {keywordRole27, "27"}, {keywordRole28, "28"}, {keywordRole29, "29"},
        {keywordRole30, "30"}, {keywordRole31, "31"}, {keywordRole32, "32"},
        {keywordRole33, "33"}, {keywordRole34, "34"}, {keywordRole35, "35"},
        {keywordRole36, "36"}, {keywordRole37, "37"}, {keywordRole38, "38"},
        {keywordRole39, "39"}, {keywordRole40, "40"}, {keywordRole41, "41"},
        {keywordRole42, "42"}, {keywordRole43, "43"}, {keywordRole44, "44"},
        {keywordRole45, "45"}, {keywordRole46, "46"}, {keywordRole47, "47"},
        {keywordRole48, "48"}, {keywordRole49, "49"}, {keywordRole50, "50"},
        {keywordRole51, "51"}, {keywordRole52, "52"}, {keywordRole53, "53"},
        {keywordRole54, "54"}, {keywordRole55, "55"}, {keywordRole56, "56"},
        {keywordRole57, "57"}, {keywordRole58, "58"}, {keywordRole61, "61"},
        {keywordRole62, "62"}, {keywordRole63, "63"}, {keywordRole64, "64"},
        {keywordRole65, "65"}, {keywordRole66, "66"}, {keywordRole67, "67"},
        {keywordRole68, "68"}, {keywordRole69, "69"}, {keywordRole70, "70"},
        {keywordRole71, "71"}, {keywordRole72, "72"}, {keywordRole73, "73"},
        {keywordRole74, "74"}, {keywordRole75, "75"}, {keywordRole76, "76"},
        {keywordRole77, "77"}, {keywordRole78, "78"}, {keywordRole79, "79"},
        {keywordRole80, "80"}, {keywordRole81, "81"}, {keywordRole82, "82"},
        {keywordRole83, "83"}, {keywordRole84, "84"}, {keywordRole85, "85"},
        {keywordRole86, "86"},
    });
} // namespace roleKind

namespace metadataKind {
// Int
const string stageWidth = "stageWidth";
const string stageHeight = "stageHeight";
const string roomSize = "roomSize";
const string lives = "lives";
const string maxAbilityLevel = "maxAbilityLevel";
const string bornDuration = "bornDuration";
const string bornLockDuration = "bornLockDuration";
const string minPlayers = "minPlayers";
// Boolean
const string supportSignin = "supportSignin";
const string mustLogin = "mustLogin";
const string allowGuest = "allowGuest";
const string supportMsgServer = "supportMsgServer";
const string runGame = "runGame";
const string campOpSkydow = "campOpSkydow";
const string campOpRoyal = "campOpRoyal";
const string campOpThird = "campOpThird";
const string setInitFocus = "setInitFocus";
const string setBornDuration = "setBornDuration";
const string nextGameEnabled = "nextGameEnabled";
const string playDefaultMusic = "playDefaultMusic";
const string disableNextGameOnMissionComplete =
    "disableNextGameOnMissionComplete";
const string useDefaultItems = "useDefaultItems";
const string defCarryItems = "defCarryItems";
const string useDefaultCampLocs = "useDefaultCampLocs";
const string useCustomWeapons = "useCustomWeapons";
const string useCustomFarWeapons = "useCustomFarWeapons";
const string useCustomItems = "useCustomItems";
// String
const string title = "title";
const string map = "map";
const string schema = "schema";
const string stageBackgroundColor = "stageBackgroundColor";
const string gamezoneCode = "gamezoneCode";
// List
const string skydowLocs = "skydowLocs";
const string royalLocs = "royalLocs";
const string thirdLocs = "thirdLocs";
const string initFocus = "initFocus";
const string preloadSources = "preloadSources";
const string preloadResourcesExclude = "preloadResourcesExclude";
const string carryItemCodes = "carryItemCodes";
// Unsupport
const string customWeapons = "customWeapons";
const string customFarWeapons = "customFarWeapons";
const string customItems = "customItems";
} // namespace metadataKind

namespace valueTypeKind {
const string keywordString = "string";
const string keywordStringZh = "字串";
const string keywordInteger = "integer";
const string keywordIntegerZh = "整數";
const string keywordInt = "int";
const string keywordNumber = "number";
const string keywordNumberZh = "數字";
const string keywordNum = "num";
const string keywordBoolean = "boolean";
const string keywordBooleanZh = "布林值";
const string keywordBool = "bool";
const string keywordJson = "json";
static KeywordEnum keywordEnum("value type kind",
                               {
                                   {keywordString, keywordString},
                                   {keywordStringZh, keywordString},
                                   {keywordInteger, keywordInteger},
                                   {keywordIntegerZh, keywordInteger},
                                   {keywordInt, keywordInteger},
                                   {keywordNumber, keywordNumber},
                                   {keywordNumberZh, keywordNumber},
                                   {keywordNum, keywordNumber},
                                   {keywordBoolean, keywordBoolean},
                                   {keywordBooleanZh, keywordBoolean},
                                   {keywordBool, keywordBoolean},
                                   {keywordJson, keywordJson},
                               });
} // namespace valueTypeKind

namespace matchKind {
const string keywordContain = "contain";
const string keywordEqual = "equal";
const string keywordRegexp = "regexp";
const string keywordRegex = "regex";
static KeywordEnum keywordEnum("match kind",
                               {
                                   {keywordContain, keywordContain},
                                   {keywordEqual, keywordEqual},
                                   {keywordRegexp, keywordRegexp},
                                   {keywordRegex, keywordRegexp},
                               });
} // namespace matchKind

namespace abilityKind {
const string keywordEnblastShield = "enblastShield";
const string keywordWarp = "warp";
const string keywordBugform = "bugform";
const string keywordGiant = "giant";
const string keywordShrink = "shrink";
const string keywordShocked = "shocked";
const string keywordAnaestheticShot = "anaestheticShot";
const string keywordDizzy = "dizzy";
const string keywordDrain = "drain";
const string keywordFireShield = "fireShield";
const string keywordHiddenEyes = "hiddenEyes";
const string keywordHelong = "helong";
const string keywordIceShocked = "iceShocked";
const string keywordJump = "jump";
const string keywordNinjatsu = "ninjatsu";
const string keywordNunchakuPower = "nunchakuPower";
const string keywordRaged = "raged";
const string keywordRebirth = "rebirth";
const string keywordShield = "shield";
const string keywordEagleEyes = "eagleEyes";
const string keywordShieldRepair = "shieldRepair";
const string keywordShieldWave = "shieldWave";
const string keywordTornado = "tornado";
const string keywordTornadoUp = "tornadoUp";
const string keywordZombie = "zombie";
const string keywordSkeleton = "skeleton";
const string keywordTreeman = "treeman";
const string keywordSoulsplit = "soulsplit";
const string keywordDazed = "dazed";
const string keywordAxeWhirl = "axeWhirl";
const string keywordSurroundFrisbee = "surroundFrisbee";
const string keywordNineDragons = "nineDragons";
const string keywordSkyJumps = "skyJumps";
const string keywordDarkRainstorm = "darkRainstorm";
const string keywordSounded = "sounded";
const string keywordNothing = "nothing";
const string keywordEnblastShieldZh = "光牙風甲";
const string keywordWarpZh = "閃現術";
const string keywordBugformZh = "蟲化術";
const string keywordGiantZh = "巨人術";
const string keywordShrinkZh = "縮骨功";
const string keywordShockedZh = "被震暈";
const string keywordAnaestheticShotZh = "被麻醉";
const string keywordDizzyZh = "暈眩";
const string keywordDrainZh = "吸血術";
const string keywordFireShieldZh = "天火護甲";
const string keywordHiddenEyesZh = "天隱之眼";
const string keywordHelongZh = "黑龍領主";
const string keywordIceShockedZh = "被冰凍";
const string keywordJumpZh = "輕功跳躍";
const string keywordNinjatsuZh = "分身術";
const string keywordNunchakuPowerZh = "雙刃亂舞";
const string keywordRagedZh = "血刃";
const string keywordRebirthZh = "爆血轉生";
const string keywordShieldZh = "防護罩";
const string keywordEagleEyesZh = "鷹眼";
const string keywordShieldRepairZh = "修復盾牌";
const string keywordShieldWaveZh = "盾衝擊波";
const string keywordTornadoZh = "真氣風暴";
const string keywordTornadoUpZh = "被真氣風暴捲入";
const string keywordZombieZh = "噬魂秘藥";
const string keywordSkeletonZh = "響骨衣";
const string keywordTreemanZh = "樹人";
const string keywordSoulsplitZh = "靈魂出竅";
const string keywordDazedZh = "眼冒金星";
const string keywordAxeWhirlZh = "旋風斧";
const string keywordSurroundFrisbeeZh = "護身迴旋鏢";
const string keywordNineDragonsZh = "九龍閃";
const string keywordSkyJumpsZh = "影天縱";
const string keywordDarkRainstormZh = "黑炎暴雨";
const string keywordSoundedZh = "有聲假技能";
const string keywordNothingZh = "無聲假技能";

static KeywordEnum
    keywordEnum("ability kind",
                {
                    {keywordEnblastShield, keywordEnblastShield},
                    {keywordWarp, keywordWarp},
                    {keywordBugform, keywordBugform},
                    {keywordGiant, keywordGiant},
                    {keywordShrink, keywordShrink},
                    {keywordShocked, keywordShocked},
                    {keywordAnaestheticShot, keywordAnaestheticShot},
                    {keywordDizzy, keywordDizzy},
                    {keywordDrain, keywordDrain},
                    {keywordFireShield, keywordFireShield},
                    {keywordHiddenEyes, keywordHiddenEyes},
                    {keywordHelong, keywordHelong},
                    {keywordIceShocked, keywordIceShocked},
                    {keywordJump, keywordJump},
                    {keywordNinjatsu, keywordNinjatsu},
                    {keywordNunchakuPower, keywordNunchakuPower},
                    {keywordRaged, keywordRaged},
                    {keywordRebirth, keywordRebirth},
                    {keywordShield, keywordShield},
                    {keywordEagleEyes, keywordEagleEyes},
                    {keywordShieldRepair, keywordShieldRepair},
                    {keywordShieldWave, keywordShieldWave},
                    {keywordTornado, keywordTornado},
                    {keywordTornadoUp, keywordTornadoUp},
                    {keywordZombie, keywordZombie},
                    {keywordSkeleton, keywordSkeleton},
                    {keywordTreeman, keywordTreeman},
                    {keywordSoulsplit, keywordSoulsplit},
                    {keywordDazed, keywordDazed},
                    {keywordAxeWhirl, keywordAxeWhirl},
                    {keywordSurroundFrisbee, keywordSurroundFrisbee},
                    {keywordNineDragons, keywordNineDragons},
                    {keywordSkyJumps, keywordSkyJumps},
                    {keywordDarkRainstorm, keywordDarkRainstorm},
                    {keywordSounded, keywordSounded},
                    {keywordNothing, keywordNothing},
                    {keywordEnblastShieldZh, keywordEnblastShield},
                    {keywordWarpZh, keywordWarp},
                    {keywordBugformZh, keywordBugform},
                    {keywordGiantZh, keywordGiant},
                    {keywordShrinkZh, keywordShrink},
                    {keywordShockedZh, keywordShocked},
                    {keywordAnaestheticShotZh, keywordAnaestheticShot},
                    {keywordDizzyZh, keywordDizzy},
                    {keywordDrainZh, keywordDrain},
                    {keywordFireShieldZh, keywordFireShield},
                    {keywordHiddenEyesZh, keywordHiddenEyes},
                    {keywordHelongZh, keywordHelong},
                    {keywordIceShockedZh, keywordIceShocked},
                    {keywordJumpZh, keywordJump},
                    {keywordNinjatsuZh, keywordNinjatsu},
                    {keywordNunchakuPowerZh, keywordNunchakuPower},
                    {keywordRagedZh, keywordRaged},
                    {keywordRebirthZh, keywordRebirth},
                    {keywordShieldZh, keywordShield},
                    {keywordEagleEyesZh, keywordEagleEyes},
                    {keywordShieldRepairZh, keywordShieldRepair},
                    {keywordShieldWaveZh, keywordShieldWave},
                    {keywordTornadoZh, keywordTornado},
                    {keywordTornadoUpZh, keywordTornadoUp},
                    {keywordZombieZh, keywordZombie},
                    {keywordSkeletonZh, keywordSkeleton},
                    {keywordTreemanZh, keywordTreeman},
                    {keywordSoulsplitZh, keywordSoulsplit},
                    {keywordDazedZh, keywordDazed},
                    {keywordAxeWhirlZh, keywordAxeWhirl},
                    {keywordSurroundFrisbeeZh, keywordSurroundFrisbee},
                    {keywordNineDragonsZh, keywordNineDragons},
                    {keywordSkyJumpsZh, keywordSkyJumps},
                    {keywordDarkRainstormZh, keywordDarkRainstorm},
                    {keywordSoundedZh, keywordSounded},
                    {keywordNothingZh, keywordNothing},
                });
} // namespace abilityKind

namespace operationKind {
const string keywordGT = ">";
const string keywordGTE = ">=";
const string keywordLT = "<";
const string keywordLTE = "<=";
const string keywordEQ = "==";
const string keywordNE = "!=";
static KeywordEnum keywordEnum("operation kind", {
                                                     {keywordGT, keywordGT},
                                                     {keywordGTE, keywordGTE},
                                                     {keywordLT, keywordLT},
                                                     {keywordLTE, keywordLTE},
                                                     {keywordEQ, keywordEQ},
                                                     {keywordNE, keywordNE},
                                                 });
} // namespace operationKind

namespace followActorKind {
const string keywordStop = "stop";
const string keywordStopZh = "停止跟隨";
const string keywordSelf = "self";
const string keywordSelfZh = "跟隨自己";
const string keywordActor = "actor";
const string keywordActorZh = "跟隨人物";
const string keywordClosestPlayer = "closestPlayer";
const string keywordClosestPlayerZh = "跟隨離最近的玩家角色";
const string keywordClosestFriendPlayer = "closestFriendPlayer";
const string keywordClosestFriendPlayerZh = "跟隨離最近的友好玩家";
const string keywordClosestEnemyPlayer = "closestEnemyPlayer";
const string keywordClosestEnemyPlayerZh = "跟隨離最近的敵方玩家";
static KeywordEnum
    keywordEnum("follow actor kind",
                {
                    {keywordStop, keywordStop},
                    {keywordStopZh, keywordStop},
                    {keywordSelf, keywordSelf},
                    {keywordSelfZh, keywordSelf},
                    {keywordActor, keywordActor},
                    {keywordActorZh, keywordActor},
                    {keywordClosestPlayer, keywordClosestPlayer},
                    {keywordClosestPlayerZh, keywordClosestPlayer},
                    {keywordClosestFriendPlayer, keywordClosestFriendPlayer},
                    {keywordClosestFriendPlayerZh, keywordClosestFriendPlayer},
                    {keywordClosestEnemyPlayer, keywordClosestEnemyPlayer},
                    {keywordClosestEnemyPlayerZh, keywordClosestEnemyPlayer},
                });
} // namespace followActorKind

namespace keyPressTimingKind {
const string keywordPressed = "pressed";
const string keywordPressedZh = "點擊";
const string keywordDown = "down";
const string keywordDownZh = "按下";
const string keywordRelease = "release";
const string keywordReleaseZh = "放開";
static KeywordEnum keywordEnum("key press timing kind",
                               {
                                   {keywordPressed, keywordPressed},
                                   {keywordPressedZh, keywordPressed},
                                   {keywordDown, keywordDown},
                                   {keywordDownZh, keywordDown},
                                   {keywordRelease, keywordRelease},
                                   {keywordReleaseZh, keywordRelease},
                               });
} // namespace keyPressTimingKind

namespace keyPressKeyKind {
const string keywordA = "A";
const string keywordB = "B";
const string keywordC = "C";
const string keywordD = "D";
const string keywordE = "E";
const string keywordF = "F";
const string keywordG = "G";
const string keywordH = "H";
const string keywordI = "I";
const string keywordJ = "J";
const string keywordK = "K";
const string keywordL = "L";
const string keywordM = "M";
const string keywordN = "N";
const string keywordO = "O";
const string keywordP = "P";
const string keywordQ = "Q";
const string keywordR = "R";
const string keywordS = "S";
const string keywordT = "T";
const string keywordU = "U";
const string keywordV = "V";
const string keywordW = "W";
const string keywordX = "X";
const string keywordY = "Y";
const string keywordZ = "Z";
const string keywordSpace = "Space";
static KeywordEnum keywordEnum(
    "key press key kind",
    {{keywordA, "65"}, {keywordB, "66"}, {keywordC, "67"},    {keywordD, "68"},
     {keywordE, "69"}, {keywordF, "70"}, {keywordG, "71"},    {keywordH, "72"},
     {keywordI, "73"}, {keywordJ, "74"}, {keywordK, "75"},    {keywordL, "76"},
     {keywordM, "77"}, {keywordN, "78"}, {keywordO, "79"},    {keywordP, "80"},
     {keywordQ, "81"}, {keywordR, "82"}, {keywordS, "83"},    {keywordT, "84"},
     {keywordU, "85"}, {keywordV, "86"}, {keywordW, "87"},    {keywordX, "88"},
     {keywordY, "89"}, {keywordZ, "90"}, {keywordSpace, "32"}});
} // namespace keyPressKeyKind

namespace actorAttrKind {
const string keywordMaxHp = "maxHp";
const string keywordMaxHpZh = "最大血量"; // 1 ~ 99999
const string keywordMinHp = "minHp";
const string keywordMinHpZh = "最低血量"; // 0 ~ 99999
const string keywordHp = "hp";
const string keywordHpZh = "血量"; // 0 ~ 999999
const string keywordMp = "mp";
const string keywordMpZh = "氣"; // 0 ~ 100
const string keywordGroup = "group";
const string keywordGroupZh = "團隊號碼"; // -9999 ~ 9999
const string keywordWeight = "weight";
const string keywordWeightZh = "體重"; // -9999 ~ 9999
const string keywordStrength = "strength";
const string keywordStrengthZh = "拳頭傷害力"; // -9999 ~ 9999
const string keywordLumi = "lumi";
const string keywordLumiZh = "發光範圍"; // -1 ~ 9
const string keywordPoisonDmgInt = "poisonDmgInt";
const string keywordPoisonDmgIntZh = "中毒受傷間隔"; // 100 ~ 999999
const string keywordReputation = "reputation";
const string keywordReputationZh = "陣營聲望"; // 0 ~ 10000
const string keywordBodyScale = "bodyScale";
const string keywordBodyScaleZh = "體型縮放"; // 0.1 ~ 3
const string keywordHitChargeScale = "hitChargeScale";
const string keywordHitChargeScaleZh = "受傷時的回氣率"; // 0 ~ 10
const string keywordWarpStunPower = "warpStunPower";
const string keywordWarpStunPowerZh = "閃現術的閃光強度"; // 0 ~ 1
const string keywordCloseWeaponSpeed = "closeWeaponSpeed";
const string keywordCloseWeaponSpeedZh = "近身武器的揮動速度"; // 0.01 ~ 100
static KeywordEnum
    keywordEnum("actor attribute kind",
                {
                    {keywordMaxHp, keywordMaxHp},
                    {keywordMaxHpZh, keywordMaxHp},
                    {keywordMinHp, keywordMinHp},
                    {keywordMinHpZh, keywordMinHp},
                    {keywordHp, keywordHp},
                    {keywordHpZh, keywordHp},
                    {keywordMp, keywordMp},
                    {keywordMpZh, keywordMp},
                    {keywordGroup, keywordGroup},
                    {keywordGroupZh, keywordGroup},
                    {keywordWeight, keywordWeight},
                    {keywordWeightZh, keywordWeight},
                    {keywordStrength, keywordStrength},
                    {keywordStrengthZh, keywordStrength},
                    {keywordLumi, keywordLumi},
                    {keywordLumiZh, keywordLumi},
                    {keywordPoisonDmgInt, keywordPoisonDmgInt},
                    {keywordPoisonDmgIntZh, keywordPoisonDmgInt},
                    {keywordReputation, keywordReputation},
                    {keywordReputationZh, keywordReputation},
                    {keywordBodyScale, keywordBodyScale},
                    {keywordBodyScaleZh, keywordBodyScale},
                    {keywordHitChargeScale, keywordHitChargeScale},
                    {keywordHitChargeScaleZh, keywordHitChargeScale},
                    {keywordWarpStunPower, keywordWarpStunPower},
                    {keywordWarpStunPowerZh, keywordWarpStunPower},
                    {keywordCloseWeaponSpeed, keywordCloseWeaponSpeed},
                    {keywordCloseWeaponSpeedZh, keywordCloseWeaponSpeed},
                });
} // namespace actorAttrKind

namespace actorBrainKind {
const string keywordAI = "ai";
const string keywordAIZh = "電腦";
const string keywordPlayer = "player";
const string keywordPlayerZh = "玩家";
const string keywordAll = "all";
const string keywordAllZh = "全部";
static KeywordEnum keywordEnum("actor controller kind",
                               {
                                   {keywordAI, keywordAI},
                                   {keywordAIZh, keywordAI},
                                   {keywordPlayer, keywordPlayer},
                                   {keywordPlayerZh, keywordPlayer},
                                   {keywordAll, keywordAll},
                                   {keywordAllZh, keywordAll},
                               });
} // namespace actorBrainKind

namespace deltaHpKind {
const string keywordHeal = "heal";
const string keywordHealZh = "治療";
const string keywordDamage = "damage";
const string keywordDamageZh = "流血受傷";
static KeywordEnum keywordEnum("delta hp kind",
                               {
                                   {keywordHeal, keywordHeal},
                                   {keywordHealZh, keywordHeal},
                                   {keywordDamage, keywordDamage},
                                   {keywordDamageZh, keywordDamage},
                               });
} // namespace deltaHpKind

namespace SetWeaponAbilityOperationKind {
const string keywordSet = "set";
const string keywordSetZh = "設定技能";
const string keywordNone = "none";
const string keywordNoneZh = "無技能";
const string keywordRemove = "remove";
const string keywordRemoveZh = "預設技能";
static KeywordEnum keywordEnum("set ability operation kind",
                               {
                                   {keywordSet, keywordSet},
                                   {keywordSetZh, keywordSet},
                                   {keywordNone, keywordNone},
                                   {keywordNoneZh, keywordNone},
                                   {keywordRemove, keywordRemove},
                                   {keywordRemoveZh, keywordRemove},
                               });
} // namespace SetWeaponAbilityOperationKind

namespace CustomWeaponAttackKind {
const string keywordSlash = "slash";
const string keywordSlashZh = "雙手橫劈";
const string keywordPierce = "pierce";
const string keywordPierceZh = "雙手突刺";
const string keywordSwing = "swing";
const string keywordSwingZh = "橫掃千軍";
const string keywordBrandish = "brandish";
const string keywordBrandishZh = "關刀砍劈";
const string keywordKnife = "knife";
const string keywordKnifeZh = "小刀前刺";
const string keywordBackslash = "backslash";
const string keywordBackslashZh = "單手橫劈";
const string keywordSword = "sword";
const string keywordSwordZh = "大刀砍劈";
static KeywordEnum keywordEnum("custom weapon attack kind",
                               {
                                   {keywordSlash, keywordSlash},
                                   {keywordSlashZh, keywordSlash},
                                   {keywordPierce, keywordPierce},
                                   {keywordPierceZh, keywordPierce},
                                   {keywordSwing, keywordSwing},
                                   {keywordSwingZh, keywordSwing},
                                   {keywordBrandish, keywordBrandish},
                                   {keywordBrandishZh, keywordBrandish},
                                   {keywordKnife, keywordKnife},
                                   {keywordKnifeZh, keywordKnife},
                                   {keywordBackslash, keywordBackslash},
                                   {keywordBackslashZh, keywordBackslash},
                                   {keywordSword, keywordSword},
                                   {keywordSwordZh, keywordSword},
                               });
} // namespace CustomWeaponAttackKind

namespace MapObjectKind {
const string keywordBox0 = "box0";
const string keywordBox0Zh = "大箱子";
const string keywordBox1 = "box1";
const string keywordBox1Zh = "小箱子";
const string keywordHouse0 = "house0";
const string keywordChair0 = "chair0";
const string keywordChair0Zh = "石椅";
const string keywordChair1 = "chair1";
const string keywordChair1Zh = "木椅";
const string keywordHouse0Zh = "5x4橫向房子";
const string keywordHouse1 = "house1";
const string keywordHouse1Zh = "5x4縱向房子";
const string keywordHouse2 = "house2";
const string keywordHouse2Zh = "4x4橫向房子";
const string keywordHouse3 = "house3";
const string keywordHouse3Zh = "4x4縱向房子";
const string keywordSky12 = "sky012";
const string keywordSky12Zh = "草地";
const string keywordTree0 = "tree0";
const string keywordTree0Zh = "3x3綠樹";
const string keywordTree1 = "tree1";
const string keywordTree1Zh = "3x3淺綠樹";
const string keywordTree5 = "tree5";
const string keywordTree5Zh = "3x3棕樹";
const string keywordShine1E = "shine1_e";
const string keywordShine1EZh = "白光(弱左)";
const string keywordShine1N = "shine1_n";
const string keywordShine1NZh = "白光(弱下)";
const string keywordShine1S = "shine1_s";
const string keywordShine1SZh = "白光(弱上)";
const string keywordShine1W = "shine1_w";
const string keywordShine1WZh = "白光(弱右)";
const string keywordShine2E = "shine2_e";
const string keywordShine2EZh = "白光(長左)";
const string keywordShine2N = "shine2_n";
const string keywordShine2NZh = "白光(長下)";
const string keywordShine2S = "shine2_s";
const string keywordShine2SZh = "白光(長上)";
const string keywordShine2W = "shine2_w";
const string keywordShine2WZh = "白光(長右)";
const string keywordShine3E = "shine3_e";
const string keywordShine3EZh = "白光(左)";
const string keywordShine3N = "shine3_n";
const string keywordShine3NZh = "白光(下)";
const string keywordShine3S = "shine3_s";
const string keywordShine3SZh = "白光(上)";
const string keywordShine3W = "shine3_w";
const string keywordShine3WZh = "白光(右)";
static KeywordEnum keywordEnum(
    "map object kind",
    {
        {keywordBox0, keywordBox0},       {keywordBox0Zh, keywordBox0},
        {keywordBox1, keywordBox1},       {keywordBox1Zh, keywordBox1},
        {keywordHouse0, keywordHouse0},   {keywordHouse0Zh, keywordHouse0},
        {keywordChair0, keywordChair0},   {keywordChair0Zh, keywordChair0},
        {keywordChair1, keywordChair1},   {keywordChair1Zh, keywordChair1},
        {keywordHouse1, keywordHouse1},   {keywordHouse1Zh, keywordHouse1},
        {keywordHouse2, keywordHouse2},   {keywordHouse2Zh, keywordHouse2},
        {keywordHouse3, keywordHouse3},   {keywordHouse3Zh, keywordHouse3},
        {keywordSky12, keywordSky12},     {keywordSky12Zh, keywordSky12},
        {keywordTree0, keywordTree0},     {keywordTree0Zh, keywordTree0},
        {keywordTree1, keywordTree1},     {keywordTree1Zh, keywordTree1},
        {keywordTree5, keywordTree5},     {keywordTree5Zh, keywordTree5},
        {keywordShine1E, keywordShine1E}, {keywordShine1EZh, keywordShine1E},
        {keywordShine1N, keywordShine1N}, {keywordShine1NZh, keywordShine1N},
        {keywordShine1S, keywordShine1S}, {keywordShine1SZh, keywordShine1S},
        {keywordShine1W, keywordShine1W}, {keywordShine1WZh, keywordShine1W},
        {keywordShine2E, keywordShine2E}, {keywordShine2EZh, keywordShine2E},
        {keywordShine2N, keywordShine2N}, {keywordShine2NZh, keywordShine2N},
        {keywordShine2S, keywordShine2S}, {keywordShine2SZh, keywordShine2S},
        {keywordShine2W, keywordShine2W}, {keywordShine2WZh, keywordShine2W},
        {keywordShine3E, keywordShine3E}, {keywordShine3EZh, keywordShine3E},
        {keywordShine3N, keywordShine3N}, {keywordShine3NZh, keywordShine3N},
        {keywordShine3S, keywordShine3S}, {keywordShine3SZh, keywordShine3S},
        {keywordShine3W, keywordShine3W}, {keywordShine3WZh, keywordShine3W},
    });
} // namespace MapObjectKind

namespace MapWarpKind {
const string keywordLeft = "left";
const string keywordLeftZh = "左";
const string keywordRight = "right";
const string keywordRightZh = "右";
const string keywordDown = "down";
const string keywordDownZh = "下";
const string keywordUp = "up";
const string keywordUpZh = "上";
const string keywordFall = "fall";
const string keywordFallZh = "停留時傳送";
const string keywordStay = "stay";
const string keywordStayZh = "停留時不傳送";
static KeywordEnum keywordEnum("map warp kind",
                               {
                                   {keywordLeft, keywordLeft},
                                   {keywordLeftZh, keywordLeft},
                                   {keywordRight, keywordRight},
                                   {keywordRightZh, keywordRight},
                                   {keywordDown, keywordDown},
                                   {keywordDownZh, keywordDown},
                                   {keywordUp, keywordUp},
                                   {keywordUpZh, keywordUp},
                                   {keywordFall, keywordFall},
                                   {keywordFallZh, keywordFall},
                                   {keywordStay, keywordStay},
                                   {keywordStayZh, keywordStay},
                               });
} // namespace MapWarpKind

namespace DropItemKind {
const string keywordCoin = "coin";
const string keywordCoin2 = "coin2";
const string keywordPaper = "paper";
const string keywordSpellPaper = "spellPaper";
const string keywordBook = "book";
const string keywordRedStone = "redStone";
const string keywordIceStone = "iceStone";
const string keywordFlagRed = "flagRed";
const string keywordFlagBlue = "flagBlue";
const string keywordGoldenKey = "goldenKey";
const string keywordSilverKey = "silverKey";
const string keywordMedicalBox = "medicalBox";
const string keywordWine = "wine";
const string keywordSandbag = "sandbag";
const string keywordBox = "box";
const string keywordSoldierSet = "soldierSet";
const string keywordMap = "map";
const string keywordMirror = "mirror";
const string keywordMeatLeg = "meatLeg";
const string keywordMeatLegPoison = "meatLegPoison";
const string keywordMeatLegZombie = "meatLegZombie";
const string keywordSerumBlue = "serumBlue";
const string keywordSariraFire = "sariraFire";
const string keywordSariraIce = "sariraIce";
const string keywordSariraElec = "sariraElec";
const string keywordSariraDark = "sariraDark";
const string keywordSariraTree = "sariraTree";
static KeywordEnum keywordEnum("map warp kind",
                               {
                                   {keywordCoin, keywordCoin},
                                   {keywordCoin2, keywordCoin2},
                                   {keywordPaper, keywordPaper},
                                   {keywordSpellPaper, keywordSpellPaper},
                                   {keywordBook, keywordBook},
                                   {keywordRedStone, keywordRedStone},
                                   {keywordIceStone, keywordIceStone},
                                   {keywordFlagRed, keywordFlagRed},
                                   {keywordFlagBlue, keywordFlagBlue},
                                   {keywordGoldenKey, keywordGoldenKey},
                                   {keywordSilverKey, keywordSilverKey},
                                   {keywordMedicalBox, keywordMedicalBox},
                                   {keywordWine, keywordWine},
                                   {keywordSandbag, keywordSandbag},
                                   {keywordBox, keywordBox},
                                   {keywordSoldierSet, keywordSoldierSet},
                                   {keywordMap, keywordMap},
                                   {keywordMirror, keywordMirror},
                                   {keywordMeatLeg, keywordMeatLeg},
                                   {keywordMeatLegPoison, keywordMeatLegPoison},
                                   {keywordMeatLegZombie, keywordMeatLegZombie},
                                   {keywordSerumBlue, keywordSerumBlue},
                                   {keywordSariraFire, keywordSariraFire},
                                   {keywordSariraIce, keywordSariraIce},
                                   {keywordSariraElec, keywordSariraElec},
                                   {keywordSariraDark, keywordSariraDark},
                                   {keywordSariraTree, keywordSariraTree},
                               });
} // namespace DropItemKind

} // namespace keyword

#endif