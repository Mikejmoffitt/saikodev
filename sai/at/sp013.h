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
#define SP013_OFFS_SPRCFG      0x0A
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
#define SP013_ATTR_FLIPX_BIT   SAI_BITVAL(SP013_ATTR_FLIPX_BIT)
#define SP013_ATTR_FLIPY_BIT   SAI_BITVAL(SP013_ATTR_FLIPY_BIT)

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

extern uint16_t g_sai_sp013_bank;

void sai_sp013_init(void);
void sai_sp013_finish(void);
void sai_sp013_wait_vbl(void);

#else
	
	.extern	g_sai_sp013_bank

	.extern sai_sp013_init
	.extern sai_min_sp013_init
	.extern sai_sp013_finish
	.extern sai_sp013_wait_vbl

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
