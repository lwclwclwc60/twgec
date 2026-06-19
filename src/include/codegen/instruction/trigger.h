#ifndef CODEGEN_TRIGGER_H
#define CODEGEN_TRIGGER_H

#include "codegen.h"
#include "utils/defaultMap.h"
#include "utils/formatter.h"

namespace codegen {
namespace trigger {

#define REGISTER_CODE_GEN(name)                                                \
  namespace Trigger##name {                                                    \
    formatter::JsonObjectNode method(DefaultMap defaultMap,                    \
                                     UserDefinedMetadata userDefinedMeta);     \
  }

REGISTER_CODE_GEN(ActorAdded);
REGISTER_CODE_GEN(ActorDead);
REGISTER_CODE_GEN(ActorFire);
REGISTER_CODE_GEN(ActorHit);
REGISTER_CODE_GEN(ClickButton);
REGISTER_CODE_GEN(DialogConfirm);
REGISTER_CODE_GEN(KeyboardPressed);
REGISTER_CODE_GEN(ItemPickup);
REGISTER_CODE_GEN(MouseEvent);
REGISTER_CODE_GEN(ReleasePower);

#undef REGISTER_CODE_GEN_ACTION
} // namespace trigger
} // namespace codegen

#endif