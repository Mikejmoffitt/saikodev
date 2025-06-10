#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__
#include "sai/macro.h"
#include "sai/target.h"
#include "sai/memmap.h"

//
// HARDWARE OPTIONS
//
// SAI_SP013_NOSCALE - set this for operation with the old revision.
// SAI_SP013_FIXED - use fixed point positioning for coordinates.

// Read to get blanking status and get IRQ source
// fedc ba98 7654 3210
// .... .... .... .b.. in vertical blanking period
// .... .... .... ..h. horizontal IRQ source
// .... .... .... ...v vertical IRQ source
#define SAI_SP013_OFFS_STATUS      0x00
// Read to ack a vertical blank interrupt.
#define SAI_SP013_OFFS_VIRQ_ACK    0x04
// Read to ack a horizontal blank interrupt.
#define SAI_SP013_OFFS_HIRQ_ACK    0x06

#define SP013_OFFS_SPRDX       0x00
#define SP013_OFFS_SPRDY       0x02
#define SP013_OFFS_SPRSX       0x04
#define SP013_OFFS_SPRSY       0x06
#define SP013_OFFS_SPRBANK     0x08

#define SP013_FIXED_SHIFT      6

#ifdef SAI_SP013_FIXED
#define SP013_SPR_HIDE_POS     (384<<SP013_FIXED_SHIFT)
#else
#define SP013_SPR_HIDE_POS     384
#endif

// Sprite / chip configuration register.
#define SP013_OFFS_CONFIG      0x0A
// fedc ba98 7654 3210
// ..yx .... .... .... Enable fixed-point pos for y, x (maybe late rev only?)
// .... ??.. .... .... Yet unknown
// .... ..h. .... .... '1' = 240px @ ~57Hz; '0' = 224px @ ~60Hz
// .... ...w .... .... '1' = 384px, 32MHz clock; '0' = 320px, 28MHz clock
// .... .... .?.. .... Unknown; esprade and uo poko set it
// .... .... ...c .... Blanks the rightmost pixel...?
// .... .... .... rr.. Moves the raster some amount.
#define SP013_CONFIG_FIXED     (SAI_BITVAL(13) | SAI_BITVAL(14))
#define SP013_CONFIG_H_240     SAI_BITVAL(9)
#define SP013_CONFIG_W_384     SAI_BITVAL(8)
#define SP013_CONFIG_DEFAULT   SP013_CONFIG_H_240

// Notes from experiments:
// rr bit 0 seems to move the blanking 1px to the right.
// bit 1 seems to extend it but only if bit 0 is set.
// if 'c' and 'rr' bit 0 are set, the rightmost pixel is blanked reducing it to
// 319 pixels...
// These are old notes and I am suspiucious of their accuracy.

#define SP013_OFFS_UNK0C       0x0C
#define SP013_OFFS_UNK0E       0x0E
#define SP013_OFFS_UNK10       0x10
#define SP013_OFFS_UNK12       0x12
#define SP013_OFFS_UNK14       0x14
#define SP013_OFFS_UNK16       0x16
#define SP013_OFFS_UNK18       0x18
#define SP013_OFFS_UNK1A       0x1A
#define SP013_OFFS_UNK1C       0x1C
#define SP013_OFFS_UNK1E       0x1E
#define SP013_OFFS_UNK20       0x20
#define SP013_OFFS_EX_SYSX_CUT 0x40
#define SP013_OFFS_FLIP        0x68
#define SP013_OFFS_WDOG        0x6A

// Sprite encryption / scrambling, used only by Sailor Moon. The data is shifted
// in through repeated writes. Just about every game puts in a stream that ends
// up using unencrypted assets.
#define SP013_OFFS_FORMAT      0x6C


// bits for reading SP013_STATUS
#define SP013_STATUS_VIRQ_BIT  0
#define SP013_STATUS_HIRQ_BIT  1
#define SP013_STATUS_VBL_BIT   2

#define SP013_BANK_SIZE        0x4000
#define SP013_SPRITE_COUNT     (SP013_BANK_SIZE/16)

// Sprite attribute information
#define SP013_ATTR_PAL_SHIFT   8
#define SP013_ATTR_PRI_SHIFT   4

#define SP013_ATTR_FLIPX_BIT   3
#define SP013_ATTR_FLIPY_BIT   2
#define SP013_ATTR_FLIPX       SAI_BITVAL(SP013_ATTR_FLIPX_BIT)
#define SP013_ATTR_FLIPY       SAI_BITVAL(SP013_ATTR_FLIPY_BIT)

// For defining the attr field alone
#define SP013_ATTR(pal, pri) (((pal) << SP013_ATTR_PAL_SHIFT)|((pri) << SP013_ATTR_PRI_SHIFT))
// For defining attributes with atcode
#define SP013_AT32(pal, pri) ((SP013_ATTR(pal, pri)) << 16)
// Size is specified in 16x16 steps. (2, 2) == 32x32
#define SP013_SIZE(sy, sx) (((sy-1)<<4)|(sx-1))

#ifndef __ASSEMBLER__

typedef struct Sp013Spr
{
#ifdef SAI_SP013_NOSCALE
	union
	{
		struct
		{
			uint16_t attr;
			uint16_t code;
		};
		uint32_t atcode;
	};
	int16_t x, y;
	uint16_t size;
	uint16_t pad[3];
#else
	int16_t x, y;
	union
	{
		struct
		{
			uint16_t attr;
			uint16_t code;
		};
		uint32_t atcode;
	};
	union
	{
		struct
		{
			uint16_t sx, sy;
		};
		uint32_t scale;
	};
	uint16_t size;
	uint16_t pad;
#endif
} Sp013Spr;
_Static_assert(sizeof(Sp013Spr) == 16);

// We don't need to think too hard about making sure we stay within the bounds
// of the sprite RAM. In the worst case, we run in to the next bank, and since
// we are toggling bit 1 of the bank bit and not bit 0 that basically gives us
// a free buffer area. It'd take so long to overrun two banks, so by that time
// you'd already be in blanking and not see the effect. The data is ephemeral,
// so there won't be lasting effects. Even if it somehow went beyond all banks
// it'd wrap within the 1MiB of address range given to the sprite RAM area. In
// other words, write to this with reckless abandon.
extern Sp013Spr *g_sai_sp013_spr_next;

void sai_sp013_init(void);
void sai_sp013_finish(void);
void sai_sp013_on_vbl(void);

// Initializes the sprite list.
void sai_sp013_spr_reset(void);

static inline Sp013Spr *sai_sp013_draw(int16_t x, int16_t y,
                                       uint32_t atcode, uint16_t size);
#ifndef SAI_SP013_NOSCALE
static inline Sp013Spr *sai_sp013_draw_sc(int16_t x, int16_t y,
                                          uint32_t atcode, uint16_t size,
                                          uint16_t sx, uint16_t sy);
#endif  // SAI_SP013_NOSCALE

// ------------------------------------
static inline Sp013Spr *sai_sp013_draw(int16_t x, int16_t y,
                                       uint32_t atcode, uint16_t size)
{
	Sp013Spr *spr = g_sai_sp013_spr_next++;
	spr->x = x;
	spr->y = y;
	spr->atcode = atcode;
	spr->size = size;
#ifndef SAI_SP013_NOSCALE
	spr->scale = 0x01000100;
#endif  // SAI_SP013_NOSCALE
	return spr;
}

#ifndef SAI_SP013_NOSCALE
static inline Sp013Spr *sai_sp013_draw_sc(int16_t x, int16_t y,
                                          uint32_t atcode, uint16_t size,
                                          uint16_t sx, uint16_t sy)
{
	Sp013Spr *spr = g_sai_sp013_spr_next++;
	spr->x = x;
	spr->y = y;
	spr->atcode = atcode;
	spr->size = size;
	spr->sx = sx;
	spr->sy = sy;
	return spr;
}
#endif  // SAI_SP013_NOSCALE

#else
	.extern	g_sai_sp013_bank
	.extern	g_sai_sp013_spr_count
	.extern	g_sai_sp013_spr_next

	.extern	sai_sp013_init
	.extern	sai_min_sp013_init
	.extern	sai_sp013_finish
	.extern	sai_sp013_on_vbl

	.extern	sai_sp013_spr_reset

	.struct 0

#ifdef SAI_SP013_NOSCALE
Sp013Spr.attr: ds.w 1
Sp013Spr.code: ds.w 1
Sp013Spr.x: ds.w 1
Sp013Spr.y: ds.w 1
Sp013Spr.size: ds.w 1
Sp013Spr.pad:  ds.w 3
#else
Sp013Spr.x: ds.w 1
Sp013Spr.y: ds.w 1
Sp013Spr.attr: ds.w 1
Sp013Spr.code: ds.w 1
Sp013Spr.sx:   ds.w 1
Sp013Spr.sy:   ds.w 1
Sp013Spr.size: ds.w 1
Sp013Spr.pad:  ds.w 1
#endif  // SAI_SP013_NOSCALE
Sp013Spr.len:

.macro	SP013_WAIT_VBL_POLL
9:
	btst	#SP013_STATUS_VBL, (SP013_STATUS).l
	bne.s	9b
.endm

.macro	SP013_WAIT_VDISP_POLL
9:
	btst	#SP013_STATUS_VBL, (SP013_STATUS).l
	beq.s	9b
.endm

#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
