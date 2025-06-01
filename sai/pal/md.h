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
#include <string.h>
#endif  // __ASSEMBLER__
#include "sai/target.h"
#include "sai/macro.h"
#include "sai/memmap.h"

#define SAI_PAL333(r, g, b) (((r) << 1) | ((g) << 5) | ((b) << 9))
#define SAI_PAL_DIRTY_MASK_FULL 0x000F

#ifndef __ASSEMBLER__

extern uint16_t g_sai_pal[16*4];
extern uint16_t g_sai_pal_dirty;

// Set a single palette color.
static inline void sai_pal_set(uint16_t idx, uint16_t val);

// Caches and schedules a transfer to palette data.
// Dest: palette index (0 - 63 on MD, 0 - 255 on C/C2).
// Source: Pointer to data to copy from. Data is copied immediately to a cache.
// Count: Number of palette entries to copy (1 = one word = one color).
static inline void sai_pal_load(uint16_t dest, const void *source, uint16_t count);

// Direct palette cache access. After modifying the palette, make sure to mark
// the range as dirty! Otherwise, it may not be uploaded.
static inline void sai_pal_mark_dirty(uint16_t first_index, uint16_t count);

// Schedules DMA transfers based on which palette lines are marked dirty.
void sai_pal_poll(void);

void sai_pal_init(void);

// Static implementations

static inline void sai_pal_set(uint16_t idx, uint16_t val)
{
	idx = idx % SAI_ARRAYSIZE(g_sai_pal);
	g_sai_pal[idx] = val;
	g_sai_pal_dirty |= (1 << (idx >> 4));
}

static inline void sai_pal_load(uint16_t dest, const void *source, uint16_t count)
{
	if (dest + count > SAI_ARRAYSIZE(g_sai_pal)) return;
	sai_pal_mark_dirty(dest, count);
	memcpy(&g_sai_pal[dest], source, count * sizeof(uint16_t));
}

static inline void sai_pal_mark_dirty(uint16_t first_index, uint16_t count)
{
	const uint16_t first_line = (first_index / 16) % (SAI_ARRAYSIZE(g_sai_pal) / 16);
	const uint16_t last_line = (count / 16) % (SAI_ARRAYSIZE(g_sai_pal) / 16);
	if (last_line < first_line)
	{
		g_sai_pal_dirty |= SAI_PAL_DIRTY_MASK_FULL;
		return;
	}

	uint32_t dirty_mask = (1 << first_line);
	for (uint16_t i = 0; i <= last_line - first_line; i++)
	{
		g_sai_pal_dirty |= dirty_mask;
		dirty_mask = dirty_mask << 1;
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
