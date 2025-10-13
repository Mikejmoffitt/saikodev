//
// Saikodev Neo-Geo Sprite pools.
//
// As Neo-Geo sprites are also used to render background tilemaps and have no
// priority system, this system allows the user to create managable sprite
// pools at fixed positions within the sprite table.
//
// Sprite pools are of a size determined during initialization. To avoid forcing
// the user into a scheme that necessitates dynamic memory allocation, the pool
// are passed in as pointers during init, and the memory's lifetime and
// ownership is the responsibility of the user.
//

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
#include "sai/neogeo/vram.h"
#include "sai/macro.h"

//
// Sprite properties
//
#define SAI_NEO_SPR_COUNT 381
#define SAI_NEO_SPR_FIXPX_BITS 7
#define SAI_NEO_SPR_FIXPX SAI_BITVAL(SAI_NEO_SPR_FIXPX_BITS)
// Subtract raster Y position from SAI_NEO_SPR_Y_ADJ to get the final Y pos.
#define SAI_NEO_SPR_Y_ADJ 496
#define SAI_NEO_SPR_Y_ADJ_FIX ((SAI_NEO_SPR_Y_ADJ)<<SAI_NEO_SPR_FIXPX_BITS)

// SCB1 contains Tilemap information for sprites, two words per.
// The tile LSBs are in the even word, while attributes and the upper four bits
// for the tile code are in the odd word. This means that writing the tile code
// as a longword and adding in attribute bytes will not work.

// An SCB1 tilemap (strip) is 32 tiles high.
#define SAI_NEO_SCB1_TILES_H 32

#define SAI_NEO_SCB1_FLAG_HF_BIT    0
#define SAI_NEO_SCB1_FLAG_VF_BIT    1
#define SAI_NEO_SCB1_FLAG_ANIM2_BIT 2
#define SAI_NEO_SCB1_FLAG_ANIM3_BIT 3
#define SAI_NEO_SCB1_MSB_BIT        4
#define SAI_NEO_SCB1_PAL_BIT        8

#define SAI_NEO_SCB1_FLAG_HF    SAI_BITVAL(SAI_NEO_SCB1_FLAG_HF_BIT)
#define SAI_NEO_SCB1_FLAG_VF    SAI_BITVAL(SAI_NEO_SCB1_FLAG_VF_BIT)
#define SAI_NEO_SCB1_FLAG_ANIM2 SAI_BITVAL(SAI_NEO_SCB1_FLAG_ANIM2_BIT)
#define SAI_NEO_SCB1_FLAG_ANIM3 SAI_BITVAL(SAI_NEO_SCB1_FLAG_ANIM3_BIT)
#define SAI_NEO_SCB1_MSB        SAI_BITVAL(SAI_NEO_SCB1_MSB_BIT)
#define SAI_NEO_SCB1_PAL        SAI_BITVAL(SAI_NEO_SCB1_PAL_BIT)

#define SAI_NEO_SCB1_ATTR(tile_msb, pal, flags) (((tile_msb) << SAI_NEO_SCB1_MSB_BIT) | \
                                                 ((pal) << SAI_NEO_SCB1_PAL_BIT) | \
                                                 (flags))
// hshrink: $00-$0F
// vshrink: $00-$FF
#define SAI_NEO_SCB2_ATTR(hshrink, vshrink) (((hshrink)<<8)|(vshrink))

// Position (SCB3, SCB4) is set up for a 7-bit fixed point value. You may shift
// an integer position up by SAI_NEO_SPR_FIXPX_BITS or multiply by
// SAI_NEO_SPR_FIXPX. If you are storing your position in fixed point, be sure
// to mask off lower bits of precision so as to not interfere with the size and
// sticky bit using SAI_NEO_SCB3_POS_MASK.
// SCB4 also uses fixed point, but the lower bits are disused so it is okay to
// just leave them.
#define SAI_NEO_SCB3_POS_MASK ((SAI_NEO_SPR_FIXPX)-1)

// For SCB3, the lower bits are used to specify the size and "sticky" bit.
// Size is valid from 0-33, where 33 is a special size that will draw a 32-tile
// strip that wraps the content upon reaching the end if shrunken.
#define SAI_NEO_SCB3_ATTR(size, ypos) ((size & 0x3F) | (ypos))
// Set "sticky" to have a sprite adopt the Y properties of the previous sprite
// and be placed horizontally 16px to the right of the previous. This can be
// used to assemble larger movable graphics out of vertical strips.
#define SAI_NEO_SCB3_STICKY SAI_BITVAL(6)

// A large buffer for SCB data is provided by the user and contains enough space
// to buffer data for all SCB blocks for the specified sprite count.
// Use this macro to calculate the size of the uint16_t array passed in at init:
#define SAI_NEO_SCB_BUFFER_SIZE(pool_capacity) ((pool_capacity)*(0x40+3))

#ifndef __ASSEMBLER__

typedef struct SaiNeoSpriteParam
{
	uint16_t attr, code;
	uint16_t shrink;
	uint16_t size;
	uint16_t x, y;  // set y to SAI_NEO_SCB3_STICKY to stick to previous.
} SaiNeoSpriteParam;

typedef struct SaiNeoSprPool
{
	uint16_t fixed_shrink;  // If set, SCB2 is set once and left alone.
	uint16_t pool_capacity; // Maximum sprite count.

	uint16_t sprite_count;  // Quantity of sprites drawn/used.
	uint16_t sprite_count_prev;  // From the last frame.

	uint16_t scb_vram_addr[4];  // VRAM Address for start of pool SCBs.
	uint16_t *scb_buf[4];   // Start address of user-owned buffer.
	uint16_t *scb_next[4];  // Pointer to next buffer area to add to.
} SaiNeoSprPool;

// Sets up a sprite pool struct with buffers and configuration.
// pool: SaiNeoSprPool object pointer.
// scb_buffers: pointer to SCB buffer memory.
// Declare SCB buffer size with SAI_NEO_SCB_BUFFER_SIZE() macro.
void sai_neo_spr_pool_init(SaiNeoSprPool *pool, uint16_t *scb_buffer,
                           uint16_t start_index,
                           uint16_t pool_capacity,
                           uint16_t fixed_shrink);

// Call before sai_finish() after having drawn sprites to the pool.
// Sprites are erased immediately to avoid wasting vblank transfer time.
void sai_neo_spr_pool_finish(SaiNeoSprPool *pool);

// Call right after sai_finish() to transfer sprites.
void sai_neo_spr_pool_on_vbl(SaiNeoSprPool *pool);

// Draws a sprite with full parameters. It is assumed that the tile code
// increments by 1 for each tile.
static inline void sai_neo_spr_pool_draw(SaiNeoSprPool *pool,
                                         uint16_t attr, uint16_t code,
                                         uint16_t shrink, uint16_t tiles_h,
                                         uint16_t x, uint16_t y);

// ---------------------
static inline void sai_neo_spr_pool_draw(SaiNeoSprPool *pool,
                                         uint16_t attr, uint16_t code,
                                         uint16_t shrink, uint16_t tiles_h,
                                         uint16_t x, uint16_t y)
{
	if (pool->sprite_count >= pool->pool_capacity) return;

	uint16_t *scb1 = pool->scb_next[0];
	uint16_t *scb2 = pool->scb_next[1];
	uint16_t *scb3 = pool->scb_next[2];
	uint16_t *scb4 = pool->scb_next[3];

	*scb4 = x;
	*scb2 = shrink;
	uint16_t size = tiles_h;
	const uint16_t shrink_y = shrink & 0xFF;
	// Write tilemap data.
	for (uint16_t i = 0; i < tiles_h; i++)
	{
		*scb1++ = code++;
		*scb1++ = attr;
	}
	// When shrinking vertically, the tilemap and height need adjustment.
	if (shrink_y != 0xFF)
	{
		// Clear out the next tile to prevent shrink artifacts.
		if (tiles_h < 32)
		{
			*scb1++ = 0;
			*scb1++ = 0;
		}
		// Reduce size field based on utilization from shrink
		size = (((tiles_h * (shrink_y+1)) >> 8) - 1);
	}

	*scb3 = SAI_NEO_SCB3_ATTR(size, y);

	// Advance sprites
	pool->sprite_count++;
	pool->scb_next[0]+=64;
	pool->scb_next[1]++;
	pool->scb_next[2]++;
	pool->scb_next[3]++;
}

static inline void sai_neo_spr_pool_stick(SaiNeoSprPool *pool,
                                          uint16_t attr, uint16_t code,
                                          uint16_t shrink, uint16_t tiles_h)
{
	if (pool->sprite_count >= pool->pool_capacity) return;

	uint16_t *scb1 = pool->scb_next[0];
	uint16_t *scb2 = pool->scb_next[1];
	uint16_t *scb3 = pool->scb_next[2];
	uint16_t *scb4 = pool->scb_next[3];

	*scb2 = shrink;  // Only the H shrink data is actually respected.
	const uint16_t shrink_y = shrink & 0xFF;
	// Write tilemap data.
	for (uint16_t i = 0; i < tiles_h; i++)
	{
		*scb1++ = code++;
		*scb1++ = attr;
	}
	// When shrinking vertically, the tilemap needs adjustment.
	if (shrink_y != 0xFF)
	{
		// Clear out the next tile to prevent shrink artifacts.
		if (tiles_h < 32)
		{
			*scb1++ = 0;
			*scb1++ = 0;
		}
	}

	// Althrough sticky sprites do not read the height or position data,
	// the sprite pool tranfsfer code does use the height.
	*scb3 = SAI_NEO_SCB3_STICKY | tiles_h;

	// Advance sprites
	pool->sprite_count++;
	pool->scb_next[0]+=64;
	pool->scb_next[1]++;
	pool->scb_next[2]++;
	pool->scb_next[3]++;
}

#else

	.struct	0
SaiNeoSprPool.fixed_shrink:         ds.w 1
SaiNeoSprPool.pool_capacity:        ds.w 1
SaiNeoSprPool.sprite_count:         ds.w 1
SaiNeoSprPool.sprite_count_prev:    ds.w 1
SaiNeoSprPool.scb_vram_addr:        ds.w 4
SaiNeoSprPool.scb_buf:              ds.l 4
SaiNeoSprPool.scb_next:             ds.l 4
SaiNeoSprPool.len:

#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
