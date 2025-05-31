#pragma once

#if SAI_TARGET==SAI_TARGET_MD
#include "sai/hw/megadrive.h"
#elif SAI_TARGET==SAI_TARGET_C2
#include "sai/hw/megadrive.h"
#elif SAI_TARGET=SAI_TARGET_SYS18
#error "TODO"
#elif SAI_TARGET=SAI_TARGET_SYS16A
#error "TODO"
#elif SAI_TARGET=SAI_TARGET_SYS16B
#error "TODO"
#elif SAI_TARGET=SAI_TARGET_CPS2
#error "TODO"
#endif
