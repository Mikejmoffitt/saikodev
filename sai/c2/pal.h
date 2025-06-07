// C/C2 palette support.
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


#define SAI_PAL_DIRTY_MASK_FULL 0xFFFF

#ifndef __ASSEMBLER__

extern uint16_t g_sai_pal[16*8*4];
extern uint32_t g_sai_pal_dirty;

void sai_pal_init(void);

// Set a single palette color.
static inline void sai_pal_set(uint16_t idx, uint16_t val);

// Caches and schedules a transfer to palette data.
// Dest: palette index (0 - 255 on C/C2).
// Source: Pointer to data to copy from. Data is copied immediately to a cache.
// Count: Number of palette entries to copy (1 = one word = one color).
static inline void sai_pal_load(uint16_t dest, const void *source, uint16_t count);

// Direct palette cache access. After modifying the palette, make sure to mark
// the range as dirty! Otherwise, it may not be uploaded.
static inline void sai_pal_mark_dirty(uint16_t first_index, uint16_t count);

// Copy palette data that needs update into color memory within the global bank.
void sai_pal_poll(void);

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
