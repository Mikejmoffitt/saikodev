// Megadrive palette support.
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

// Functions to read and write to the palette. Color data is cached in a small
// buffer that is queued for transfer during vblank if needed.

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__
#include "sai/target.h"
#include "sai/macro.h"
#include "sai/memmap.h"

#define SAI_PAL333(r, g, b) (((r) << 1) | ((g) << 5) | ((b) << 9))
#define SAI_PAL444(r, g, b) (((r)) | ((g) << 4) | ((b) << 8))
#define SAI_PAL888(r, g, b) (((r) >> 4) | ((g) & 0x00F0) | (((b) << 4) & 0x0F00))
#define SAI_PALHEX(x) SAI_PAL888((((x) >> 16) & 0xFF), (((x) >> 8) & 0xFF), ((x) & 0xFF))
#define SAI_PAL_DIRTY_MASK_FULL 0x000F

#ifndef __ASSEMBLER__

extern uint16_t g_sai_pal[16*4];
extern uint16_t g_sai_pal_dirty;

// Set a single palette color.
static inline void sai_pal_set(uint16_t idx, uint16_t val);

// Caches and schedules a transfer to palette data.
// dest:  palette line (0 - 3).
// src:   Pointer to data to copy from. Data is copied immediately to a cache.
// count: Number of palette lines to copy (1 = sixteen colors).
static inline void sai_pal_load(uint16_t dest, const void *src, uint16_t count);

// Schedules DMA transfers based on which palette lines are marked dirty.
void sai_pal_poll(void);

void sai_pal_init(void);

// Static implementations

static inline void sai_pal_set(uint16_t idx, uint16_t val)
{
	idx = idx % SAI_ARRAYSIZE(g_sai_pal);
	g_sai_pal[idx] = val;
	g_sai_pal_dirty |= SAI_BITVAL(idx/16);
}

static inline void sai_pal_load(uint16_t dest, const void *src, uint16_t count)
{
	// TODO: runtime error check for debug build
	if ((dest+count) > 4) return;
	const uint16_t pal_offs = dest*16;

	const uint32_t *src32 = (const uint32_t *)(src);
	uint32_t *dest32 = (uint32_t *)(&g_sai_pal[pal_offs]);
	uint16_t dirty_bit_mark = SAI_BITVAL(dest);

	while (count--)
	{
		*dest32++ = *src32++;
		*dest32++ = *src32++;
		*dest32++ = *src32++;
		*dest32++ = *src32++;
		*dest32++ = *src32++;
		*dest32++ = *src32++;
		*dest32++ = *src32++;
		*dest32++ = *src32++;
		g_sai_pal_dirty |= dirty_bit_mark;
		dirty_bit_mark <<= 1;
	}
}

#else

	.extern	g_sai_pal
	.extern	g_sai_pal_dirty
	.extern	sai_pal_poll
	.extern	sai_pal_init

#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
