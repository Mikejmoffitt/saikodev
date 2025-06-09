// Atlus palette support.
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

// The SP013 sprite chip can address 64 palettes, each with 16 colors. However,
// ESP Ra.De. and Guwange extend this to 256 colors with some external logic
// and a second plane of ROM data.
//
// The BG038 background chip itself also has 64x16 colors, but with some simple
// additional logic it too can be expanded to 256 colors. This is a common
// arrangement. Aku Gallet and Sailor Moon are unique in that they expand just
// one plane (C) to 64 colors with principally the same technique.
//
// While SP013 always uses the start of color RAM and only addresses using 10
// bits (6 palette sel + 4 color) the BG038 chip can add some global address
// bits using the control register, allowing the color RAM to be split.
// In practice Aku Gallet gives each plane its own palette region, as there
// is enough color memory to do this at 4bpp, but with all of the Cave titles
// that use 8bpp backgrounds, the memory is only sufficient to split it between
// sprites and all the other background planes.
//
// Treatment of color RAM varies from hardware to hardware, but this palette
// support code will just operate on 16-color lines; To copy a 256 color
// palette it is necessary to do a transfer that spans sixteen lines.

#define SAI_PAL555(r, g, b)  (((r)<<5)|((g)<<10)|(b))
#define SAI_PAL444(r, g, b) (SAI_PAL555((r)<<1, (g)<<1, (b)<<1))
#define SAI_PAL888(r, g, b) (SAI_PAL555((r)>>3, (g)>>3, (b)>>3))
#define SAI_PAL333(r, g, b) (SAI_PAL555((r)<<2, (g)<<2, (b)<<2))
#define SAI_PALHEX(x) SAI_PAL888((((x) >> 16) & 0xFF), (((x) >> 8) & 0xFF), ((x) & 0xFF))

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
