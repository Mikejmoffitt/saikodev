// Composite sprite support for Velella output.
//
// Data exists in the CHR block for tile data, as well as in the MAP data for
// drawing instructions. The map data allows for drawing of arbitrarily sized
// metasprites out of a list of frames.
//
// Composite sprites may be used in one of two ways:
//
//
//
// Fixed Mode
// ----------
//
// All of the CHR data associated with a frame set is loaded into VRAM once at
// initialization of the CSP object; depending on the amount of artwork, this
// may not be practical for larger objects.
//
// The user is responsible for specifying the location in VRAM and passes the
// desired address into the initialization function, and uploading the data
// into the specified region.
//
// The advantages to this mode are that it does not demand bandwidth for tile
// transfers to draw sprites, and multiple instances of an object can share
// video memory.
//
// Usage:
//
// 1) allocate or otherwise set aside N words in VRAM.
//
// const uint16_t vram_address = 0x1000;  // random example value.
// const uint16_t fixed_words = sai_vdp_csp_get_fixed_vram_words(mapping);
//
// 2) initialize parameters
//
// SaiMdCspParam param;
// const uint16_t attr = VDP_ATTR(/*tile=*/0,  // Tile left at 0.
//                                /*_hflip=*/false, /*_vflip=*/false,
//                                /*_pal=*/0,
//                                /*_prio*/=false);
// sai_vdp_csp_init(&param,
//                 gfx_chr_data+SPR_*_CHR_OFFS,  // from resource CHR block
//                 gfx_map_data+SPR_*_MAP_OFFS,  // from resource MAP block
//                 vram_address,                 // Fixed location.
//                 attr,                         // Base attributes.
//                 /*use_dma=*/false);
//
// 3) transfer tile data
//
// sai_vdp_csp_transfer(&param);
//
// Naturally, if the same character data shall be used by multiple objects, it
// is not necessary to transfer multiple copies. In such a case, the call to
// sai_vdp_csp_transfer() may be skipped after the first.
//
// 4) perform draw calls
//
// With the parameter struct configured and the tile data in VRAM, we are now
// set up to begin drawing metasprites.
//
// Set x and y in the parameter to position the origin point of the object.
// Attr may be modified to change palette lines, flip about the x and y axis,
// and set priority. Finally, frame should be set to the desired frame number.
//
// sai_vdp_csp_draw(&param);
//
// Ordinarily the Megadrive VDP requires an offset of 128 to be added to both
// X and Y coordinates. However, this offset has already been baked in to the
// metasprite mapping data, so 0, 0 refers to the top-left corner of the screen.
//
//
//
// DMA Mode
// --------
//
// For DMA mode, the user is meant to allocate enough VRAM to hold enough
// tiles for one frame of animation at a time. Sprite calls will refer to the
// same tile index, and the tile data itself is transferred before the sprites
// are drawn.
//
// This method consumes transfer bandwidth during the vertical blanking interval
// but consumes far less VRAM at a given moment in exchange. This is recommended
// for objects with lots of animation frames or large objects that may not be
// practical for fixed usage.
//
// Usage:
//
// 1) allocate or otherwise set aside N words in VRAM.
//
// const uint16_t vram_address = 0x1000;  // random example value.
// const uint16_t dma_words = sai_vdp_csp_get_dma_vram_words(mapping);
//
// 2) initialize parameters
//
// SaiMdCspParam param;
// const uint16_t attr = VDP_ATTR(/*tile=*/0,  // Tile left at 0.
//                                /*_hflip=*/false, /*_vflip=*/false,
//                                /*_pal=*/0,
//                                /*_prio*/=false);
// sai_vdp_csp_init(&param,
//                 gfx_chr_data+SPR_*_CHR_OFFS,  // from resource CHR block
//                 gfx_map_data+SPR_*_MAP_OFFS,  // from resource MAP block
//                 vram_address,                 // Fixed location.
//                 attr,                         // Base attributes.
//                 /*use_dma=*/true);
//
// 3) perform draw calls
//
// The usage of the parameter data for drawing sprites is the same as for
// fixed mode. However, the draw call will automatically queue the transfer
// of tile data as necessary.

#pragma once

#ifndef __ASSEMBLER__
#include <stdint.h>
#include <stdbool.h>
#endif  // __ASSEMBLER__

#include "sai/md/vdp_spr.h"
#include "sai/md/vdp_dma_queue.h"

#ifndef __ASSEMBLER__
typedef struct SaiMdCspRef
{
	uint16_t spr_count;      // Number of SPR entries and hardware sprites.
	uint16_t spr_list_offs;  // Offset into spr list table (pre-multiplied idx)
	uint16_t tile_index;     // Added to attr, or used as DMA source offset.
	uint16_t tile_words;     // DMA size parameter if in DMA mode.
} SaiMdCspRef;

typedef struct SaiMdCspSpr
{
	int16_t dy;
	uint8_t size;
	uint8_t pad;
	uint16_t attr;  // relative to tile index
	int16_t dx;
	int16_t flip_dy;
	uint16_t pad2[2];
	int16_t flip_dx;
} SaiMdCspSpr;

typedef struct SaiMdCspHeader
{
	uint16_t ref_count;
	uint16_t spr_list_base_offs;// Offset from header start to spr list.
	uint16_t fixed_vram_words;  // VRAM words to hold all CHR data.
	uint16_t dma_vram_words;    // VRAM words to hold one frames of CHR data.
	SaiMdCspRef refs[0];
} SaiMdCspHeader;

typedef struct SaiMdCspParam
{
	// Data.
	uint32_t vram_base;      // Base VRAM address used for tile data.
	const uint8_t *chr;      // CHR data (used for DMA).
	const SaiMdCspHeader *map;     // Mapping data.
	const SaiMdCspRef *ref;  // Ref list.
	const SaiMdCspSpr *spr;  // SPR list.
	uint16_t tile_base;      // VDP_TILE(vram_base)
	// User parameters.
	uint16_t attr;           // VDP_ATTR(0, ...);
	int16_t x, y;            // Screen coordinates. 0,0 is the top-left.
	int16_t frame;           // Metasprite frame number.
	uint16_t fixed_chr_words;
	uint16_t dma_chr_words;
	int16_t frame_last;      // Last frame draw/transferred (for DMA).
} SaiMdCspParam;

//
// Interface
//

// Set up a CSP parameter struct.
void sai_vdp_csp_init(SaiMdCspParam *s,
                     const uint8_t *chr,
                     const uint8_t *map,
                     uint32_t vram_base,
                     uint16_t attr,
                     bool use_dma);


// Copies tile data into VRAM. For fixed usage, this copies all tile data at
// once, and should be called after init (though, it may be skipped for
// repeated usage of the same data.)
// For DMA usage, it is not necessary to manually call this function.
static inline void sai_vdp_csp_transfer(SaiMdCspParam *s);

// Draws a CSP metasprite using the method specified in the struct.
void sai_vdp_csp_draw(SaiMdCspParam *s);

// Same as dai_vdp_csp_draw, but without edge culling nor frame bound checks.
void sai_vdp_csp_draw_fast(SaiMdCspParam *s);

//
// Mapping header access.
//

static inline uint16_t sai_vdp_csp_get_ref_count(const uint8_t *map);
static inline const SaiMdCspRef *sai_vdp_csp_get_refs(const uint8_t *map);
static inline const SaiMdCspSpr *sai_vdp_csp_get_sprs(const uint8_t *map);
static inline uint16_t sai_vdp_csp_get_fixed_vram_words(const uint8_t *map);
static inline uint16_t sai_vdp_csp_get_dma_vram_words(const uint8_t *map);

// -----------------------------------------------------------------------------
// Static implementations
// -----------------------------------------------------------------------------

static inline void sai_vdp_csp_transfer(SaiMdCspParam *s)
{
	sai_vdp_dma_transfer_vram(s->vram_base, s->chr, s->fixed_chr_words, 2);
}

static inline uint16_t sai_vdp_csp_get_ref_count(const uint8_t *map)
{
	const SaiMdCspHeader *header = (const SaiMdCspHeader *)map;
	return header->ref_count;
}

static inline const SaiMdCspRef *sai_vdp_csp_get_refs(const uint8_t *map)
{
	const SaiMdCspHeader *header = (const SaiMdCspHeader *)map;
	return &header->refs[0];
}

static inline const SaiMdCspSpr *sai_vdp_csp_get_sprs(const uint8_t *map)
{
	const SaiMdCspHeader *header = (const SaiMdCspHeader *)map;
	return (SaiMdCspSpr *)(map + header->spr_list_base_offs);
}

static inline uint16_t sai_vdp_csp_get_fixed_vram_words(const uint8_t *map)
{
	const SaiMdCspHeader *header = (const SaiMdCspHeader *)map;
	return header->fixed_vram_words;
}

static inline uint16_t sai_vdp_csp_get_dma_vram_words(const uint8_t *map)
{
	const SaiMdCspHeader *header = (const SaiMdCspHeader *)map;
	return header->dma_vram_words;
}

#endif // __ASSEMBLER__


