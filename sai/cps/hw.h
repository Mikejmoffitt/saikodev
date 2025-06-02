// Hardware definitions for Capcom CPS1/2.
#pragma once

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__
#include "sai/target.h"

#define ROM_BASE               (0x000000)
#define WRAM_BASE              (0xFF0000)
#define WRAM_SIZE              (0x10000)
#define VRAM_BASE              (0x900000)
#define VRAM_SIZE              (0x30000)

#define SAI_CPSA_REG_BASE      (0x804100)
#define SAI_CPSB_REG_BASE      (0x804140)

#if SAI_PLATFORM == SAI_PLATFORM_CPS2
#ifndef SAI_CPS2_GOOD_BATTERY
#define SAI_CPS2_REG_BASE      (0xFFFFF0)
#else
#define SAI_CPS2_REG_BASE      (0x400000)
#endif  // SAI_CPS2_GOOD_BATTERY
#endif  // SAI_PLATFORM

#if SAI_PLATFORM == SAI_PLATFORM_CPS2
#define SAI_CPS2_IO_BASE       (0x804000)
#define SAI_CPS2_OBJRAM_BASE   (0x700000)
#define SAI_CPS2_OBJRAM_SIZE   (0x2000)
#define SAI_CPS2_QSOUND_BASE   (0x618000)
#define SAI_CPS2_QSOUND_BYTES  (0x2000)
#else
#define SAI_CPS2_IO_BASE       (0x800000)
#endif  // SAI_PLATFORM
