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
#include "sai/neogeo/lspc.h"
#include "sai/neogeo/vram.h"
#include "sai/macro.h"

// A large buffer for SCB data is provided by the user and contains enough space
// to buffer data for all SCB blocks for the specified sprite count.
// Use this macro to calculate the size of the uint16_t array passed in at init.
#define SAI_NEO_SCB_BUFFER_SIZE(count) ((count)*(SAI_NEO_SCB1_WORDS+3))

#ifndef __ASSEMBLER__

// Draw parameters.
typedef struct SaiNeoSprPrm
{
	uint16_t scb1_attr, code;
	uint16_t scb2_shrink;
	uint16_t size;
	uint16_t x, y;  // set y to SAI_NEO_SCB3_STICKY to stick to previous.
} SaiNeoSprPrm;

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
void sai_neo_spr_pool_init(SaiNeoSprPool *pool,
                           uint16_t *scb_buffer,
                           uint16_t start_index,
                           uint16_t pool_capacity,
                           uint16_t fixed_shrink);

// Call right after sai_finish() to transfer sprites.
void sai_neo_spr_pool_transfer(SaiNeoSprPool *pool);

// Draws a sprite with full parameters. It is assumed that the tile code
// increments by 1 for each tile.
static inline void sai_neo_spr_pool_draw(SaiNeoSprPool *pool,
                                         uint16_t scb1_attr,
                                         uint16_t scb1_code,
                                         uint16_t scb2_shrink,
                                         uint16_t tiles_h,
                                         uint16_t x, uint16_t y);

// Draws a "sticky" sprite to the right of the previous sprite.
static inline void sai_neo_spr_pool_draw_sticky(SaiNeoSprPool *pool,
                                                uint16_t scb1_attr,
                                                uint16_t scb1_code,
                                                uint16_t scb2_shrink,
                                                uint16_t tiles_h);

// Draws a group of horizontally chained sprites with incremending codes,
static inline void sai_neo_spr_pool_draw_grp(SaiNeoSprPool *pool,
                                             uint16_t scb1_attr,
                                             uint16_t scb1_code,
                                             uint16_t scb2_shrink,
                                             uint16_t tiles_w,
                                             uint16_t tiles_h,
                                             uint16_t x, uint16_t y);

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



// -----------------------------------------------------------------------------
//
// Static inline draw function implementations
//
// -----------------------------------------------------------------------------

#ifndef __ASSEMBLER__

// Clears out an extra tile in scb1 if scaling, and reduces size to not show
// beyond what's needed for the sprite size.
static inline uint16_t sai_neo_spr_pool_size_adj(uint16_t scb2_shrink,
                                                 uint16_t tiles_h,
                                                 uint16_t *scb1)
{
	const uint16_t shrink_y = scb2_shrink & 0xFF;
	// When shrinking vertically, the tilemap and height need adjustment.
	if (shrink_y != 0xFF)
	{
		// Clear out the next tile to prevent shrink artifacts.
		if (tiles_h < 32)
		{
			*(uint32_t *)(scb1) = 0;
		}
		// Reduce size field based on utilization from shrink
		// TODO: This math is junk do it again
		return (((tiles_h<<4)+0xF) * ((shrink_y+0xF)>>4))>>8;
	}
	else
	{
		return tiles_h;
	}
}

static inline void sai_neo_spr_pool_draw(SaiNeoSprPool *pool,
                                         uint16_t scb1_attr,
                                         uint16_t scb1_code,
                                         uint16_t scb2_shrink,
                                         uint16_t tiles_h,
                                         uint16_t x, uint16_t y)
{
	if (pool->sprite_count >= pool->pool_capacity) return;

	uint16_t *scb1 = pool->scb_next[0];
	uint16_t *scb2 = pool->scb_next[1];
	uint16_t *scb3 = pool->scb_next[2];
	uint16_t *scb4 = pool->scb_next[3];

	*scb4 = x;
	*scb2 = scb2_shrink;

	// Write tilemap data.
	for (uint16_t i = 0; i < tiles_h; i++)
	{
		*scb1++ = scb1_code++;
		*scb1++ = scb1_attr;
	}

	uint16_t size = sai_neo_spr_pool_size_adj(scb2_shrink, tiles_h, scb1);
	*scb3 = SAI_NEO_SCB3_ATTR(size, y);

	// Advance sprites
	pool->sprite_count++;
	pool->scb_next[0]+=64;
	pool->scb_next[1]++;
	pool->scb_next[2]++;
	pool->scb_next[3]++;
}

static inline void sai_neo_spr_pool_sticky(SaiNeoSprPool *pool,
                                           uint16_t scb1_attr,
                                           uint16_t scb1_code,
                                           uint16_t scb2_shrink,
                                           uint16_t tiles_h)
{
	if (pool->sprite_count >= pool->pool_capacity) return;

	uint16_t *scb1 = pool->scb_next[0];
	uint16_t *scb2 = pool->scb_next[1];
	uint16_t *scb3 = pool->scb_next[2];

	*scb2 = scb2_shrink;  // Only the H shrink data is actually respected.
	// Write tilemap data.
	for (uint16_t i = 0; i < tiles_h; i++)
	{
		*scb1++ = scb1_code++;
		*scb1++ = scb1_attr;
	}

	// SCB1 blanking for scaling
	(void)sai_neo_spr_pool_size_adj(scb2_shrink, tiles_h, scb1);

	// Althrough sticky sprites do not read the height or position data,
	// the sprite pool tranfsfer code does use the height.
	*scb3 = SAI_NEO_SCB3_STICKY | tiles_h;

	// Advance sprites
	pool->sprite_count++;
	pool->scb_next[0]+=SAI_NEO_SCB1_WORDS;
	pool->scb_next[1]++;
	pool->scb_next[2]++;
	pool->scb_next[3]++;
}

static inline void sai_neo_spr_pool_draw_grp(SaiNeoSprPool *pool,
                                             uint16_t scb1_attr,
                                             uint16_t scb1_code,
                                             uint16_t scb2_shrink,
                                             uint16_t tiles_w,
                                             uint16_t tiles_h,
                                             uint16_t x, uint16_t y)
{
	sai_neo_spr_pool_draw(pool, scb1_attr, scb1_code,
	                      scb2_shrink, tiles_h, x, y);
	for (uint16_t i = 0; i < tiles_w-1; i++)
	{
		scb1_code += tiles_h;
		sai_neo_spr_pool_sticky(pool, scb1_attr, scb1_code,
		                        scb2_shrink, tiles_h);
	}
}

#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
