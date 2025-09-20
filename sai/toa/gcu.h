//
// Toaplan GCU "GP9001" support code.
//

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include "sai/toa/hw.h"
#include <stdbool.h>
#include <stdint.h>
#endif  // __ASSEMBLER__

//
// Offsets from GCU_BASE
//
#define GCU_ADDR_OFFS      (0x0000)
#define GCU_DATA_OFFS      (0x0004)
#define GCU_REG_ADDR_OFFS  (0x0008)
#define GCU_REG_DATA_OFFS  (0x000C)
#define GCU_STATUS_OFFS    (0x000C)

#define GCU_STATUS_BLANKING_BIT 0

#define GCU_VRAM_PAGE_SIZE (0x0800)

#define GCU_VRAM_A_BASE    (0x0000)
#define GCU_VRAM_B_BASE    (0x0800)
#define GCU_VRAM_C_BASE    (0x1000)
#define GCU_VRAM_SPR_BASE  (0x1800)

//
// Register indices
//
#define GCU_REG_SCROLL_A_H 0x0
#define GCU_REG_SCROLL_A_V 0x1
#define GCU_REG_SCROLL_B_H 0x2
#define GCU_REG_SCROLL_B_V 0x3
#define GCU_REG_SCROLL_C_H 0x4
#define GCU_REG_SCROLL_C_V 0x5
#define GCU_REG_SCROLL_S_H 0x6
#define GCU_REG_SCROLL_S_V 0x7

#define GCU_REG_INITIALIZE 0xE
#define GCU_REG_IRQ_ACK    0xF

// OR with the register select index for a screen flip.
#define GCU_REG_FLIP       SAI_BITVAL(7)

//
// Scroll and plane enums
//

#define GCU_PLANE_A 0
#define GCU_PLANE_B 1
#define GCU_PLANE_C 2
#define GCU_PLANE_S 3

#define GCU_PLANE_W_PX 512
#define GCU_PLANE_H_PX 512
#define GCU_PLANE_TILESIZE 16
#define GCU_PLANE_W_TILES (GCU_PLANE_W_PX / GCU_PLANE_TILESIZE)
#define GCU_PLANE_H_TILES (GCU_PLANE_H_PX / GCU_PLANE_TILESIZE)

#define GCU_RASTER_W 320
#define GCU_RASTER_H 240

//
// GCU Attributes and VRAM math
//
#define GCU_DF_BIT 15
#define GCU_VF_BIT 14
#define GCU_HF_BIT 13
#define GCU_DF SAI_BITVAL(GCU_DF_BIT)
#define GCU_VF SAI_BITVAL(GCU_VF_BIT)
#define GCU_HF SAI_BITVAL(GCU_HF_BIT)

// Pal:   $00 - $3F
// Prio:    0 -   7
#define GCU_ATTR(pal, prio) (((prio)<<8) | ((pal)<<2))
#define GCU_SIZE(x, y) (((y)<<16)|(x))

#define GCU_VRAM_OFFS(x, y) ((((x)+((y)*(GCU_PLANE_W_TILES)))*2) & (GCU_VRAM_PAGE_SIZE-1))

#ifndef __ASSEMBLER__

// The GCU can only accept VRAM writes *outside* of vertical blank!
// On hardware with the external line counter and sync line buffer the status is
// checked using an I/O port.
static inline void sai_gcu_wait_access(void)
{
	// Check using the GCU status.
	asm volatile ("0: btst #GCU_STATUS_BLANKING_BIT, (GCU_BASE+GCU_STATUS_OFFS).l; "
	              "bne.s 0b;" : : : "cc");
}

//
// Scroll configuration for each plane.
//

typedef struct SaiGcuPlaneCfg
{
	int16_t x, y;
} SaiGcuPlaneCfg;

extern SaiGcuPlaneCfg g_sai_gcu_scroll[4];

// Sets up GCU with initial register configuration.
void sai_gcu_init(void);
void sai_gcu_on_vbl(void);

#else

	.struct	0
SaiGcuPlaneCfg.x:      ds.w 1
SaiGcuPlaneCfg.y:      ds.w 1
SaiGcuPlaneCfg.len:

	.extern	sai_min_gcu_init
	.extern	sai_gcu_init
	.extern	sai_gcu_on_vbl

#endif  // __ASSEMBLER__

//
// GCU Sprites.
//

// The position data is lined up for Toaplan's 7bit fixed point system, which
// is as precise as you can get while still allowing freedom to position in
// and around a 320x240 screen.

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

//
// GCU Background tiles.
//

#ifdef __cplusplus
}
#endif  // __cplusplus

