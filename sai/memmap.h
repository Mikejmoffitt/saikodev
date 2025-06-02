#pragma once

#include "sai/target.h"

#if SAI_TARGET==SAI_TARGET_MD
#include "sai/md/hw.h"
#elif SAI_TARGET==SAI_TARGET_C2
#include "sai/md/hw.h"
#elif SAI_TARGET=SAI_TARGET_SYS16B
#include "sai/s16b/hw.h"
#elif SAI_TARGET=SAI_TARGET_SYS18
#error "TODO"
#elif SAI_TARGET=SAI_TARGET_CPS2
#error "TODO"
#endif
