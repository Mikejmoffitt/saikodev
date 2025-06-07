// System 16/18 palette support.
// Once more thanks to Charles McDonald for describing this hardware.
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

// Functions to read and write to the palette. Color data is cached in a small
// buffer that is queued for transfer during vblank if needed.

#ifndef __ASSEMBLER__
#include <stdint.h>
#include <string.h>
#endif  // __ASSEMBLER__
#include "sai/target.h"
#include "sai/macro.h"
#include "sai/memmap.h"
#include "sai/palcmd.h"

// System 16 has 4K of CRAM, split between the layers.
// The text plane uses colors 0-63, from the first eight 8-color palettes.
// The scroll planes use colors 0-1023, from 128 8-color palettes.
// The sprites use the second half, 1024-2047 as 64 16-color palettes.

#define SAI_PAL_SH SAI_BITVAL(15)

#define SAI_PAL555(r, g, b)  ( \
                              (SAI_BTST(r, 4)?SAI_BITVAL(12):0) | \
                              (SAI_BTST(g, 4)?SAI_BITVAL(13):0) | \
                              (SAI_BTST(b, 4)?SAI_BITVAL(14):0) | \
                              ((((r)>>1) & 0xF)) | \
                              ((((g)>>1) & 0xF)<<4) | \
                              ((((b)>>1) & 0xF)<<8) \
                             )
#define SAI_PAL444(r, g, b) (SAI_PAL555((r)<<1, (g)<<1, (b)<<1))
#define SAI_PAL888(r, g, b) (SAI_PAL555((r)>>3, (g)>>3, (b)>>3))
#define SAI_PAL333(r, g, b) (SAI_PAL555((r)<<2, (g)<<2, (b)<<2))
#define SAI_PALHEX(x) SAI_PAL888((((x) >> 16) & 0xFF), (((x) >> 8) & 0xFF), ((x) & 0xFF))

#define SAI_PAL_DIRTY_MASK_FULL 0xFFFF

#ifndef __ASSEMBLER__

// Erases the palette immediately.
void sai_pal_init(void);

// Set a single palette color.
static inline void sai_pal_set(uint16_t idx, uint16_t val);

// Caches and schedules a transfer to palette data.
// dest:  palette line index (0 - 127: background; 128-191: sprites)
// src:   Pointer to data to copy from.
// count: Number of palette lines to copy (for BG, this is 8 colors; for spr, 16).
static inline void sai_pal_load(uint16_t dest, const void *source, uint16_t count);
static inline void sai_pal_load_bg(uint16_t dest, const void *source, uint16_t count);
static inline void sai_pal_load_spr(uint16_t dest, const void *source, uint16_t count);

// -----------------------------------------------------------------------------
// Static implementations
// -----------------------------------------------------------------------------

static inline void sai_pal_set(uint16_t idx, uint16_t val)
{
	SaiPalCmd *cmd = sai_palcmd_add();
	if (!cmd) return;
	cmd->op_cnt = SAI_PAL_CMD_SET_COLOR | (1-1);
	uint16_t *cram = (uint16_t *)CRAM_BASE;
	cmd->dest = &cram[idx];
	cmd->color = val;
}

static inline void sai_pal_load_bg(uint16_t dest, const void *src, uint16_t count)
{
	SaiPalCmd *cmd = sai_palcmd_add();
	if (!cmd) return;
	cmd->op_cnt = SAI_PAL_CMD_COPY_LINE_HALF | (count-1);
	uint16_t *cram = (uint16_t *)CRAM_BASE;
	cmd->dest = &cram[dest*8];
	cmd->src = src;
}

static inline void sai_pal_load_spr(uint16_t dest, const void *src, uint16_t count)
{
	SaiPalCmd *cmd = sai_palcmd_add();
	if (!cmd) return;
	cmd->op_cnt = SAI_PAL_CMD_COPY_LINE_LONG | (count-1);
	uint16_t *cram = (uint16_t *)CRAM_BASE;
	cmd->dest = &cram[1024+(dest*16)];
	cmd->src = src;
}

static inline void sai_pal_load(uint16_t dest, const void *src, uint16_t count)
{
	if (dest < 128) sai_pal_load_bg(dest, src, count);
	else sai_pal_load_spr(dest-128, src, count);
}

#else

	.extern	sai_pal_init
	.extern	sai_min_pal_init

#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
