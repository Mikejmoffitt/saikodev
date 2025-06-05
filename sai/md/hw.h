// Hardware definitions for Mega Drive / Genesis, and System C/C2.
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__
#include "sai/target.h"

#define ROM_BASE               (0x000000)
#define WRAM_BASE              (0xFF0000)
#define WRAM_SIZE              (0x010000)
#define VRAM_SIZE              (0x010000)
#define VRAM_SIZE_128K         (0x020000)
#define SRAM_BASE              (0x200000)
#define SRAM_CTRL              (0xA130F1)

/* IO (MD Pads) */
#define MD_IO_BASE             (0xA10000)

/* SYS and TMSS */
#if SAI_TARGET == SAI_TARGET_MD
#define MD_Z80_WRAM_SIZE       (0x2000)
#define MD_Z80_WRAM            (0xA00000)
#define MD_Z80_BUSREQ          (0xA11100)
#define MD_Z80_RESET           (0xA11200)
#define TMSS_PORT              (0xA14000)
#endif  // SAI_TARGET

/* VDP */
#define VDP_BASE               (0xC00000)

/* PSG */
#define PSG_BASE               (0xC00011)

/* OPN */
#if SAI_TARGET == SAI_TARGET_C2
#define OPN_BASE               (0x840100)
#else
#define OPN_BASE               (0xA04000)
#endif  // SAI_TARGET

#if SAI_TARGET == SAI_TARGET_C2
/* Color RAM */
#define CRAM_BASE              (0x8C0000)
/* I/O ports (buttons, some peripheral control) */
#define S5296_BASE             (0x840000)
#define SYSC_PROT_BASE         (0x800000)
/* ADPCM */
#define SYSC_UPD7759_BASE      (0x880000)

#endif  // SAI_TARGET

#ifdef __cplusplus
}
#endif  // __cplusplus
