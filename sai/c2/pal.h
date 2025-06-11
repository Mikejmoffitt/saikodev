// C/C2 palette support.
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdint.h>
#include <stddef.h>
#endif  // __ASSEMBLER__
#include "sai/target.h"
#include "sai/macro.h"
#include "sai/memmap.h"
#include "sai/palcmd.h"

// The C2 palette is banked in two stages.
// The global bank is controlled by pins from the I/O controller, and banks out
// all palette data at once. The global bank is respected not only when
// rendering, but also when the CPU reads and writes from it.
// Within that, background and sprite colors are banked into four units that are
// controlled by the protection CPLD. Changing this bank affects rendering, but
// the CPU can write to anywhere within the bank.
//
// The 4K of color memory is thusly arranged in this hierarchy:
//
// global bank 0-3                 1K global banks
//   sub-bank 0-3 background data  four 128 byte groupings
//   sub-bank 0-3 sprite data      "
//
// Surprisingly all four sub-banks for the background colors are laid out,
// followed by the same pattern for sprites.
//
// It is possible to arrange for the background and sprite data to lie in
// contiguous memory by using background sub-bank 3 and sprite sub-bank 0.
//
// Banks may be controlled by sai_sysc_prot_set_pal_bank() for sub-banks and
// by setting the SYSC_IO_MO2_PALBANK* bits on IO port H.
//
// As C/C2 has its own dedicated color RAM, the color data is not cached, and
// instead a palette command list is used to schedule transfers for vblank.

// C/C2 supports two palette organizations. One lines up with MD palettes
// gracefully while the other represents a straight-across arrangement
// you might expect from other 5-bit RGB machines.
// As MAME does not seem to respect the palette organization bit for now
// the default is to retain MD compatibility.
#ifdef SAI_C2_PAL_STRAIGHT
#define SAI_PAL555(r, g, b)  ((((r) & 0x1F)) | \
                             (((g) & 0x1F) << 5) | \
                             (((b) & 0x1F) << 10))
#else
#define SAI_PAL555(r, g, b)  ( \
                              (SAI_BTST(r, 4)?SAI_BITVAL(12):0) | \
                              (SAI_BTST(g, 4)?SAI_BITVAL(13):0) | \
                              (SAI_BTST(b, 4)?SAI_BITVAL(14):0) | \
                              ((((r)>>1) & 0xF)) | \
                              ((((g)>>1) & 0xF)<<4) | \
                              ((((b)>>1) & 0xF)<<8) \
                             )
#endif

#define SAI_PAL444(r, g, b) (SAI_PAL555((r)<<1, (g)<<1, (b)<<1))
#define SAI_PAL888(r, g, b) (SAI_PAL555((r)>>3, (g)>>3, (b)>>3))
#define SAI_PAL333(r, g, b) (SAI_PAL555((r)<<2, (g)<<2, (b)<<2))
#define SAI_PALHEX(x) SAI_PAL888((((x) >> 16) & 0xFF), (((x) >> 8) & 0xFF), ((x) & 0xFF))

#ifndef __ASSEMBLER__

// Clears palette and initializes queue system.
void sai_pal_init(void);

// Registers a command to set a single palette color.
static inline void sai_pal_set(uint16_t idx, uint16_t val);

// Registers a command to fill a range to a single palette color.
static inline void sai_pal_fill(uint16_t idx, uint16_t val, uint16_t count);

// Schedules a transfer to palette data.
// dest:  palette line (0 - 63).
// src:   Pointer to data to copy from. Data must exist at least until next vbl.
// count: Number of palette lines to copy (1 = sixteen colors).
static inline void sai_pal_load(uint16_t dest, const void *src, uint16_t count);

// -----------------------------------------------------------------------------
// Static implementations
// -----------------------------------------------------------------------------

static inline void sai_pal_set(uint16_t idx, uint16_t val)
{
	sai_pal_fill(idx, val, 1);
}

static inline void sai_pal_fill(uint16_t idx, uint16_t val, uint16_t count)
{
	SaiPalCmd *cmd = sai_palcmd_add();
	if (!cmd) return;
	cmd->op_cnt = SAI_PAL_CMD_SET_COLOR | (count-1);
	uint16_t *cram = (uint16_t *)CRAM_BASE;
	cmd->dest = &cram[idx];
	cmd->color = val;
}

static inline void sai_pal_load(uint16_t dest, const void *src, uint16_t count)
{
	SaiPalCmd *cmd = sai_palcmd_add();
	if (!cmd) return;
	cmd->op_cnt = SAI_PAL_CMD_COPY_LINE_LONG | (count-1);
	uint16_t *cram = (uint16_t *)CRAM_BASE;
	cmd->dest = &cram[dest*16];
	cmd->src = src;
}

#else

	.extern	sai_min_pal_init
	.extern	sai_pal_init

#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
