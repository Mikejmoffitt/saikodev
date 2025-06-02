#pragma once


#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__
#include "sai/target.h"
#include "sai/cps/cpsa.h"
#include "sai/cps/cpsb.h"
#include "sai/cps/cps2.h"

// CPS2 object registers
#if SAI_TARGET == SAI_TARGET_CPS2
#define SAI_VIDCON_OFFS_OBJ2_BASE        0x36
#define SAI_VIDCON_OFFS_UNK1             0x38
#define SAI_VIDCON_OFFS_PRIO             0x3A
#define SAI_VIDCON_OFFS_UNK2             0x3C
#define SAI_VIDCON_OFFS_XOFF             0x3E
#define SAI_VIDCON_OFFS_YOFF             0x40
#endif  // SAI_TARGET


