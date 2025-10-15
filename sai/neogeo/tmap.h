//
// Saikodev Neo-Geo 512x512 pixel (32x32 tile) Tilemap support.
//
// By setting up 32 sprites as 32 tile tall chained sprites, it is possible to
// easily scroll a tilemap by just moving the position of the first sprite, and
// treating the SCB1 block as a contiguous tilemap.
//
// This simple object initializes a chosen block of 32 sprites for use as a
// tilemap, and updates scroll and culls off-screen sprite stripes during the
// vertical blank interval.
//
// The tilemap supports a culling function, where columns that are outside of
// the visible screen area are disabled. This reduces the per-scanline sprite
// bandwidth consumption to 2/3 (21 sprites instead of 32) which will reduce the
// likelihood of sprite dropout. It may even afford the use of another layer.
//
// The culling function incurs a tiny CPU overhead as it has to propogate the
// position data across all 32 sprite strips, but in many cases this is likely
// worth it.

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdint.h>
#include <stdbool.h>
#endif  // __ASSEMBLER__
#include "sai/neogeo/hw.h"
#include "sai/neogeo/lspc.h"
#include "sai/neogeo/vram.h"
#include "sai/macro.h"

#define SAI_NEO_TMAP_TILES_W 32
#define SAI_NEO_TMAP_TILES_H 32

#ifndef __ASSEMBLER__

typedef struct SaiNeoTmap
{
	uint16_t cull;              // Cull off-screen sprite columns.
	uint16_t scb_vram_addr[4];  // VRAM Address for start of tmap SCBs.
	union
	{
		struct
		{
			uint16_t scroll[2];         // Fixed-point 9.7, applied during transfer.
			uint16_t scroll_prev[2];    // Used to skip redundant work when culling.
		};
		struct  // longword alias for comparison.
		{
			uint32_t scroll32;
			uint32_t scroll32_prev;
		};
	};
} SaiNeoTmap;

// Sets VRAM address pointers and clears tilemap.
// With cull on, columns off-screen are hidden to improve sprite bandwidth.
// With cull off, rendering takes nearly no time but eats 32 sprites per line.
void sai_neo_tmap_init(SaiNeoTmap *tmap, uint16_t start_index, uint16_t cull);

// Sets scroll values to be picked up during transfer.
static inline void sai_neo_tmap_set_scroll_x(SaiNeoTmap *tmap, uint16_t sx);
static inline void sai_neo_tmap_set_scroll_y(SaiNeoTmap *tmap, uint16_t sy);

// Returns the VRAM address of the tilemap in VRAM.
static inline uint16_t sai_neo_tmap_get_vram(const SaiNeoTmap *tmap);

// Updates scroll position and hides sprites that are offscreen.
void sai_neo_tmap_transfer(SaiNeoTmap *tmap);

// -----------------------------------------------------------------------------

static inline void sai_neo_tmap_set_scroll_x(SaiNeoTmap *tmap, uint16_t sx)
{
	tmap->scroll[0] = sx;
}

static inline void sai_neo_tmap_set_scroll_y(SaiNeoTmap *tmap, uint16_t sy)
{
	tmap->scroll[1] = sy & SIA_NEO_SPR_POS_MASK;
}

#else

#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
