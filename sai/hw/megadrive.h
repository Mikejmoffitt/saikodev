// Hardware definitions for Mega Drive / Genesis, and System C/C2.
#pragma once

#include <stdint.h>

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
#ifndef SAI_TARGET_C2
#define SYS_Z80_PRG            (0xA00000)
#define SYS_Z80_BUSREQ         (0xA11100)
#define SYS_Z80_RESET          (0xA11200)
#define TMSS_PORT              (0xA14000)
#endif  // SAI_TARGET_C2

/* VDP */
#define VDP_BASE               (0xC00000)

/* PSG */
#define PSG_BASE               (0xC00011)

/* OPN */
#ifdef SAI_TARGET_C2
#define OPN_BASE               (0x840100)
#else
#define OPN_BASE               (0xA04000)
#endif  // SAI_TARGET_C2

#ifdef SAI_TARGET_C2
/* Color RAM */
#define CRAM_BASE              (0x8C0000)
/* I/O ports (buttons, some peripheral control) */
#define SYSC_IO_BASE           (0x840000)
#define SYSC_PROT_BASE         (0x800000)
/* ADPCM */
#define SYSC_UPD7759_BASE      (0x880000)

#endif  // SAI_TARGET_C2
