//
// Toaplan GCU "GP9001" sprite drawing system.
//

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#include "sai/toa/gcu.h"

//
// Sprite Data as seen in video RAM:
//
// Sprite data begins at 0x1800 (word address) and has 256 entries that follow
// the format laid out below.

// $00 - 設定・色 attributes, color, etc.
// fedc ba98 7654 3210
// d... .... .... .... 表示              display
// .m.. .... .... .... 連鎖              chain
// ..y. .... .... .... Y反転             flip
// ...x .... .... .... X反転             flip
// .... pppp .... .... プライオリティー　priority
// .... .... cccc cc.. 色　              color
// .... .... .... ..CC パターンコード    pattern code (bits 16-17)

// $01 - パターン pattern code. Many titles use this exclusively.
// fedc ba98 7654 3210
// CCCC CCCC CCCC CCCC パターンコード    pattern code (bits 0-15)

// $02 - X立場 position and attributes
// xxxx xxxx x... .... X立場　           position
// .... .... .... ssss 幅-1              width-1

// $03 - Y立場 position and attributes
// yyyy yyyy y... .... Y立場　           position
// .... .... .... ssss 丈-1              height-1

// The position data is lined up for Toaplan's 7-bit fixed point system, which
// is as precise as you can get while still allowing freedom to position in
// and around a 320x240 screen. This also allows for a little trick where the
// offset required to center a sprite about its center is "baked in" to sprite
// size metadata, allowing for a quick position correction.

#define GCU_DF_BIT 15
#define GCU_CF_BIT 14
#define GCU_VF_BIT 13
#define GCU_HF_BIT 12
#define GCU_DF SAI_BITVAL(GCU_DF_BIT)
#define GCU_CF SAI_BITVAL(GCU_CF_BIT)
#define GCU_VF SAI_BITVAL(GCU_VF_BIT)
#define GCU_HF SAI_BITVAL(GCU_HF_BIT)

// Pal:   $00 - $3F
// Prio:    0 -   7
#define GCU_SPR_ATTR(pal, prio) (GCU_DF | ((prio)<<8) | ((pal)<<2))
#define GCU_SPR_AT32(pal, prio) (GCU_SPR_ATTR(pal, prio)<<16)
#define GCU_SPR_SIZE(x, y) (((y)<<16)|(x))

#ifndef __ASSEMBLER__

// Struct representing the data in a sprite.
typedef struct GcuSpr
{
	union
	{
		struct
		{
			uint16_t attr;  // Use GCU_SPR_ATTR macro.
			uint16_t code;
		};
		uint32_t atcode;  // Use GCU_SPR_AT32 macro.
	};
	uint16_t x;  // width in lower four bits.
	uint16_t y;  // height in lower four bits.
} GcuSpr;

// State for the sprite renderer.
extern uint16_t g_sai_gcu_spr_count;  // Count of sprites drawn.
extern uint16_t g_sai_gcu_spr_next;   // VRAM index of the next sprite.

// A single sprite is drawn using the arguments provided.
static inline void sai_gcu_spr_draw(uint32_t attrcode, uint16_t x, uint16_t y, uint32_t size)
{
	if (g_sai_gcu_spr_count >= GCU_SPR_COUNT) return;

	volatile uint16_t *gcu_addr   = (volatile uint16_t *)(GCU_BASE+GCU_ADDR_OFFS);
	volatile uint32_t *gcu_data32 = (volatile uint32_t *)(GCU_BASE+GCU_DATA_OFFS);

	sai_gcu_wait_access();
	*gcu_addr = g_sai_gcu_spr_next;
	*gcu_data32 = attrcode;
	// The size code is added like this in order to support the Toaplan
	// technique of baking the position offset to center a sprite in the unused
	// bits of the size data.
	*gcu_data32 = ((size & 0xFFFF0000) + (x << 16)) | ((size & 0x0000FFFF) + y);

	g_sai_gcu_spr_next += sizeof(GcuSpr)/sizeof(uint16_t);
	g_sai_gcu_spr_count++;
}

// A single sprite is drawn using draw parameters stored in a struct.
static inline void sai_gcu_spr_draw_st(const GcuSpr *spr)
{
	if (g_sai_gcu_spr_count >= GCU_SPR_COUNT) return;

	volatile uint16_t *gcu_addr   = (volatile uint16_t *)(GCU_BASE+GCU_ADDR_OFFS);
	volatile uint32_t *gcu_data32 = (volatile uint32_t *)(GCU_BASE+GCU_DATA_OFFS);
	const uint32_t *spr32 = (const uint32_t *)(spr);

	sai_gcu_wait_access();
	*gcu_addr = g_sai_gcu_spr_next;
	*gcu_data32 = *spr32++;
	*gcu_data32 = *spr32;

	g_sai_gcu_spr_next += sizeof(GcuSpr)/sizeof(uint16_t);
	g_sai_gcu_spr_count++;
}

// The sprite system is initialized.
void sai_gcu_spr_init(void);

// Call before waiting for vertical blank. Sprites drawn on the previous frame
// are hidden from view.
void sai_gcu_spr_finish(void);

#else

	.struct	0

GcuSpr.attr:           ds.w 1
GcuSpr.code:           ds.w 1
GcuSpr.x:              ds.w 1
GcuSpr.y:              ds.w 1
GcuSpr.len:

	.extern	g_sai_gcu_spr_count
	.extern	g_sai_gcu_spr_next

	.extern	sai_gcu_spr_init
	.extern	sai_gcu_spr_finish

#endif  // __ASSEMBLER__


#ifdef __cplusplus
}
#endif  // __cplusplus

