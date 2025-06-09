#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__

#include "sai/target.h"

// TODO: other hardware variants, including DonPachi with SAI_SP013_NOSCALE

#if SAI_TARGET == SAI_TARGET_ESPRADE

#define ROM_BASE               (0x000000)
#define WRAM_BASE              (0x100000)
#define WRAM_SIZE              (0x010000)
#define CRAM_BASE              (0xC00000)
#define CRAM_SIZE              0x8000

#define YMZ_BASE               (0x300000)
#define AT_IO_BASE             (0xD00000)

#define SP013_VRAM_BASE        (0x400000)
#define BG038_A_VRAM_BASE      (0x500000)
#define BG038_B_VRAM_BASE      (0x600000)
#define BG038_C_VRAM_BASE      (0x700000)
#define SP013_CTRL_BASE        (0x800000)
#define BG038_A_CTRL_BASE      (0x900000)
#define BG038_B_CTRL_BASE      (0xA00000)
#define BG038_C_CTRL_BASE      (0xB00000)

#define SP013_VRAM_SIZE        (0x10000)
#define SP013_VRAM_PAGE_SIZE   (0x4000)
#define BG038_A_VRAM_SIZE      (0x8000)
#define BG038_B_VRAM_SIZE      (0x8000)
#define BG038_C_VRAM_SIZE      (0x8000)

#endif  // SAI_TARGET

#ifdef __cplusplus
}
#endif  // __cplusplus
