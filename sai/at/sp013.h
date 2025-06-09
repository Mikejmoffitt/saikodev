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
#define SP013_CONFIG_DEFAULT   0x3F00

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

#define SP013_ATTR(pal, pri) (((pal) << SP013_ATTR_PAL_SHIFT)|((pri) << SP013_ATTR_PRI_SHIFT))
#define SP013_SIZE(sy, sx) (((sy)<<4)|(sx))

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
	int16_t xpos, ypos;
	uint16_t size;
	uint16_t pad[3];
#else
	int16_t xpos, ypos;
	union
	{
		struct
		{
			uint16_t attr;
			uint16_t code;
		};
		uint32_t atcode;
	};
	uint16_t sx, sy;
	uint16_t size;
	uint16_t pad;
#endif
} Sp013Spr;
_Static_assert(sizeof(Sp013Spr) == 16);

extern uint16_t g_sai_sp013_spr_count;
extern Sp013Spr *g_sai_sp013_spr_next;

void sai_sp013_init(void);
void sai_sp013_finish(void);

// Initializes the sprite list.
void sai_sp013_spr_reset(void);

#else
	
	.extern	g_sai_sp013_bank
	.extern	g_sai_sp013_spr_count
	.extern	g_sai_sp013_spr_next

	.extern sai_sp013_init
	.extern sai_min_sp013_init
	.extern sai_sp013_finish
	.extern sai_sp013_wait_vbl

	.extern	sai_sp013_spr_reset

	.struct 0

#ifdef SAI_SP013_NOSCALE
Sp013Spr.attr: ds.w 1
Sp013Spr.code: ds.w 1
Sp013Spr.xpos: ds.w 1
Sp013Spr.ypos: ds.w 1
Sp013Spr.size: ds.w 1
Sp013Spr.pad:  ds.w 3
#else
Sp013Spr.xpos: ds.w 1
Sp013Spr.ypos: ds.w 1
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
