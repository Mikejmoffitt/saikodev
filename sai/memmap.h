#pragma once

#include "sai/target.h"

#if SAI_TARGET == SAI_TARGET_MD || SAI_TARGET == SAI_TARGET_C1 || SAI_TARGET == SAI_TARGET_C2
#include "sai/md/hw.h"
#elif SAI_TARGET == SAI_TARGET_S16B
#include "sai/s16b/hw.h"
#elif SAI_TARGET == SAI_TARGET_S18
#include "sai/s18/hw.h"
#elif SAI_TARGET == SAI_TARGET_CPS
#include "sai/cps/hw.h"
#elif SAI_TARGET == SAI_TARGET_CPS2
#include "sai/cps2/hw.h"
#elif SAI_TARGET == SAI_TARGET_ESPRADE
#include "sai/at/hw.h"
#elif SAI_TARGET == SAI_TARGET_TATSUJIN_OH
#include "sai/toa/hw.h"
#else
#error "UNSUPPORTED TARGET!"
#endif
