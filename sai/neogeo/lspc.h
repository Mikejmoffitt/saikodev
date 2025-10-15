//
// Saikodev Neo-Geo LSPC (Sprites) support.
//

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#include "sai/neogeo/vram.h"
#include "sai/neogeo/reg.h"

//
// Sprite properties
//

// Sprite index #0 seems to be special in that it renders on the very top, so
// sprite #1 is the lowest priority.

// Position (SCB3, SCB4) is set up for a 7-bit fixed point value. You may shift
// an integer position up by SAI_NEO_SPR_FIXPX_BITS or multiply by
// SAI_NEO_SPR_FIXPX.
#define SAI_NEO_SPR_COUNT 381
#define SAI_NEO_SPR_FIXPX_BITS 7
#define SAI_NEO_SPR_FIXPX SAI_BITVAL(SAI_NEO_SPR_FIXPX_BITS)
// Subtract raster Y position from SAI_NEO_SPR_Y_ADJ to get the final Y pos.
#define SAI_NEO_SPR_Y_ADJ 496
#define SAI_NEO_SPR_Y_ADJ_FIX ((SAI_NEO_SPR_Y_ADJ)<<SAI_NEO_SPR_FIXPX_BITS)
// For masking off the 7-bit fixed point position data.
#define SIA_NEO_SPR_POS_MASK (~(SAI_NEO_SPR_FIXPX-1))
#define SAI_NEO_SPR_TILESIZE 16

//
// Sprite register bits and macros
//

// SCB1 contains Tilemap information for sprites, two words per.
// The tile LSBs are in the even word, while attributes and the upper four bits
// for the tile code are in the odd word. This means that writing the tile code
// as a longword and adding in attribute bytes will not work.

// An SCB1 tilemap (strip) is 32 tiles high.
#define SAI_NEO_SCB1_TILES_H           32
#define SAI_NEO_SCB1_WORDS             ((SAI_NEO_SCB1_TILES_H)*2)

#define SAI_NEO_SCB1_FLAG_HF_BIT       0
#define SAI_NEO_SCB1_FLAG_VF_BIT       1
#define SAI_NEO_SCB1_FLAG_ANIM2_BIT    2
#define SAI_NEO_SCB1_FLAG_ANIM3_BIT    3
#define SAI_NEO_SCB1_MSB_BIT           4
#define SAI_NEO_SCB1_PAL_BIT           8

#define SAI_NEO_SCB1_FLAG_HF           SAI_BITVAL(SAI_NEO_SCB1_FLAG_HF_BIT)
#define SAI_NEO_SCB1_FLAG_VF           SAI_BITVAL(SAI_NEO_SCB1_FLAG_VF_BIT)
#define SAI_NEO_SCB1_FLAG_ANIM2        SAI_BITVAL(SAI_NEO_SCB1_FLAG_ANIM2_BIT)
#define SAI_NEO_SCB1_FLAG_ANIM3        SAI_BITVAL(SAI_NEO_SCB1_FLAG_ANIM3_BIT)
#define SAI_NEO_SCB1_MSB               SAI_BITVAL(SAI_NEO_SCB1_MSB_BIT)
#define SAI_NEO_SCB1_PAL               SAI_BITVAL(SAI_NEO_SCB1_PAL_BIT)

#define SAI_NEO_SCB1_ATTR(tile_msb, pal, flags) (((tile_msb) << SAI_NEO_SCB1_MSB_BIT) | \
                                                 ((pal) << SAI_NEO_SCB1_PAL_BIT) | \
                                                 (flags))
// SCB2: Horizontal and Vertical shrink.
// 0xF/0xFF is the full non-shrunken value for H and V respectively.
#define SAI_NEO_SCB2_HSHRINK_BIT       8
#define SAI_NEO_SCB2_ATTR(hshrink, vshrink) (((hshrink)<<SAI_NEO_SCB2_HSHRINK_BIT)|(vshrink))

// SCB3: Y position, sticky bit, Y size
// The Y position grows from the bottom of the screen, and the position seen on
// the raster is SAI_NEO_SPR_Y_ADJ - Y (integer).
//
// Setting the sticky bit will cause a sprite to "stick" to the previous one.
// Be sure to use SIA_NEO_SPR_POS_MASK to stop subpixel precision from messing
// with the size or sticky bits when adding attributes and position data.
#define SAI_NEO_SCB3_SIZE_MASK         0x3F
#define SAI_NEO_SCB3_STICKY_BIT        6

// Size is valid from 0-33, where 33 is a special size that will draw a 32-tile
// strip that wraps the content upon reaching the end if shrunken.
#define SAI_NEO_SCB3_ATTR(size, ypos)  ((size & SAI_NEO_SCB3_SIZE_MASK) | (ypos))

// Set "sticky" to have a sprite adopt the Y properties of the previous sprite
// and be placed horizontally 16px to the right of the previous. This can be
// used to assemble larger movable graphics out of vertical strips.
// When using the sticky feature you can ignore the rest of the data in SCB3.
#define SAI_NEO_SCB3_STICKY            SAI_BITVAL(SAI_NEO_SCB3_STICKY_BIT)

// SCB4: X position
// SCB4 also uses fixed point, but the lower bits are disused so it is okay to
// just leave them at whatever value without masking.
#define SAI_NEO_SCB4_ATTR(xpos)        (xpos)

#ifndef __ASSEMBLER__

// Initializes sprite system hardware and clears VRAM.
void sai_neo_lspc_init(void);

// VRAM access.
static inline void sai_neo_lspc_vram_set_addr(uint16_t addr);
static inline void sai_neo_lspc_vram_set_mod(uint16_t mod);
static inline void sai_neo_lspc_vram_write(uint16_t data);
static inline uint16_t sai_neo_lspc_vram_read(void);

// -----------------------------------------------------------------------------

static inline void sai_neo_lspc_vram_set_addr(uint16_t addr)
{
	*(volatile uint16_t *)(SAI_NEO_REG_VRAMADDR) = addr;
}

static inline void sai_neo_lspc_vram_set_mod(uint16_t mod)
{
	*(volatile uint16_t *)(SAI_NEO_REG_VRAMMOD) = mod;
}

static inline void sai_neo_lspc_vram_write(uint16_t data)
{
	*(volatile uint16_t *)(SAI_NEO_REG_VRAMRW) = data;
}

static inline uint16_t sai_neo_lspc_vram_read(void)
{
	return *(volatile uint16_t *)(SAI_NEO_REG_VRAMRW);
}

#else

	.extern	sai_neo_lspc_init

#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
