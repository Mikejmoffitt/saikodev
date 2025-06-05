#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#include "sai/cps/hw.h"

#ifndef SAI_CPS2_GOOD_BATTERY
#define SAI_CPS2_REG_BASE      (0xFFFFF0)
#else
#define SAI_CPS2_REG_BASE      (0x400000)
#endif  // SAI_CPS2_GOOD_BATTERY

#define SAI_CPS2_IO_BASE       (0x804000)
#define SAI_CPS2_OBJRAM_BASE   (0x700000)
#define SAI_CPS2_OBJRAM_SIZE   (0x2000)
#define SAI_CPS2_QSOUND_BASE   (0x618000)
#define SAI_CPS2_QSOUND_BYTES  (0x2000)

#ifndef __ASEMBLER__
.macro	cps2_wdog_pet
	move.l	#0xFFFFFFFF,(0xFFFFFFFC).l
.endm
#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
