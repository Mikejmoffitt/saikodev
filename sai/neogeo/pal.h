// Neo-Geo palette support.
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

// The Neo-Geo has two banks of 256 palettes.
// Sprites can use all palettes. The FIX layer is able to use the first 16.


#define SAI_PAL555(r, g, b)  ((((((r)>>1)&0xF)<<8)|((((g)>>1)&0xF)<<4)|((((b)>>1)&0xF)<<0)) | \
                              (((r) & 0x1) << 14) | \
                              (((g) & 0x1) << 13) | \
                              (((b) & 0x1) << 12))
#define SAI_PAL444(r, g, b) (SAI_PAL555((r)<<1, (g)<<1, (b)<<1))
#define SAI_PAL888(r, g, b) (SAI_PAL555((r)>>3, (g)>>3, (b)>>3))
#define SAI_PAL333(r, g, b) (SAI_PAL555((r)<<2, (g)<<2, (b)<<2))
#define SAI_PALHEX(x) SAI_PAL888((((x) >> 16) & 0xFF), (((x) >> 8) & 0xFF), ((x) & 0xFF))

// A neo-geo palette color with the "dark" bit set.
#define SAI_PAL555D(r, g, b, d) (SAI_PAL555(r, g, b) | 0x8000)

#ifndef __ASSEMBLER__

// Erases the palette immediately.
void sai_pal_init(void);

// Set a single palette color.
static inline void sai_pal_set(uint16_t idx, uint16_t val);

// Registers a command to fill a range to a single palette color.
static inline void sai_pal_fill(uint16_t idx, uint16_t val, uint16_t count);

// Caches and schedules a transfer to palette data.
// dest:  palette line index
// src:   Pointer to data to copy from.
// count: Number of palette lines to copy (16 colors per).
static inline void sai_pal_load(uint16_t dest, const void *source, uint16_t count);

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
	cmd->dest = &cram[(dest*16)];
	cmd->src = src;
}

#else

	.extern	sai_pal_init
	.extern	sai_min_pal_init

#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
