#ifndef CODEGEN_ACTION_H
#define CODEGEN_ACTION_H

#include "codegen.h"
#include "utils/defaultMap.h"
#include "utils/formatter.h"

namespace codegen {
namespace action {

#define REGISTER_CODE_GEN(name)                                                \
  namespace Action##name {                                                     \
    formatter::JsonObjectNode method(DefaultMap defaultMap,                    \
                                     UserDefinedMetadata userDefinedMeta);     \
  }

// Standard Module
REGISTER_CODE_GEN(ActorAttributes);
REGISTER_CODE_GEN(ActorDisappear);
REGISTER_CODE_GEN(ActorFollow);
REGISTER_CODE_GEN(ActorRelocate);
REGISTER_CODE_GEN(ActorSpawnLoc);
REGISTER_CODE_GEN(ActorTalk);
REGISTER_CODE_GEN(AddActor);
REGISTER_CODE_GEN(AddDropItem);
REGISTER_CODE_GEN(AddMapSign);
REGISTER_CODE_GEN(AddMapObject);
REGISTER_CODE_GEN(AddStuff);
REGISTER_CODE_GEN(AvoidFriendFire);
REGISTER_CODE_GEN(Console);
REGISTER_CODE_GEN(DeltaHp);
REGISTER_CODE_GEN(EnblastEffect);
REGISTER_CODE_GEN(EquipWeapon);
REGISTER_CODE_GEN(LongBo);
REGISTER_CODE_GEN(GetCookie);
REGISTER_CODE_GEN(GetUserState);
REGISTER_CODE_GEN(MapWarp);
REGISTER_CODE_GEN(MissionComplete);
REGISTER_CODE_GEN(RemoveDevice);
REGISTER_CODE_GEN(RemoveMapObject);
REGISTER_CODE_GEN(SetCookie);
REGISTER_CODE_GEN(SetGlobal);
REGISTER_CODE_GEN(SetObjectVar);
REGISTER_CODE_GEN(SetUserState);
REGISTER_CODE_GEN(SetWeaponAbility);
REGISTER_CODE_GEN(ShowSelectDialog);
REGISTER_CODE_GEN(TipOnMap);
REGISTER_CODE_GEN(Wait);

// EnhFF module
REGISTER_CODE_GEN(EnhFFPlayerMousePosition);
REGISTER_CODE_GEN(EnhFFGeneralCircularRange);

#undef REGISTER_CODE_GEN
} // namespace action
} // namespace codegen

#endif