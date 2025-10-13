//
// Toaplan GCU "GP9001" support code.
//

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#include "sai/toa/hw.h"
#include "sai/macro.h"

#ifndef __ASSEMBLER__
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

#define GCU_BG_PAGE_WORDS  (0x0800)
#define GCU_SPR_PAGE_WORDS (0x0400)

#define GCU_VRAM_A_BASE    (0x0000)
#define GCU_VRAM_B_BASE    (0x0800)
#define GCU_VRAM_C_BASE    (0x1000)
#define GCU_VRAM_SPR_BASE  (0x1800)

SAI_ENUMSTART
SAI_ENUMNEXT(GCU_PLANE_A)
SAI_ENUMNEXT(GCU_PLANE_B)
SAI_ENUMNEXT(GCU_PLANE_C)
SAI_ENUMNEXT(GCU_PLANE_S)
SAI_ENUMEND

#define GCU_RASTER_W 320
#define GCU_RASTER_H 240

//
// Sprites
//
#define GCU_SPR_COUNT 256
#define GCU_SPR_FIXPX_BITS 7
#define GCU_SPR_FIXPX SAI_BITVAL(GCU_SPR_FIXPX_BITS)
#define GCU_SPR_STATIC_OFFS 96
#define GCU_SPR_STATIC_OFFS_FIX ((GCU_SPR_STATIC_OFFS)<<GCU_SPR_FIXPX_BITS)

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

#define GCU_REG_CONFIG     0xE
#define GCU_REG_IRQ_ACK    0xF

// OR with the register select index for a screen flip.
#define GCU_REG_FLIP       SAI_BITVAL(7)

// Configuration register bits.
// fedc ba98 7654 3210
// .1.. .... .... .... Unknown. Set to 1 in all titles.
// ...v .... .... .... Vsync pin becomes an input. For syncing two GCUs.
// .... .h.. .... .... Hsync phase/counter reset upon Vsync.
// .... ..s. .... .... Sprite rendering disable.
// .... ...S .... .... Only a single sprite is rendered.
// .... .... b... .... Blanking signal seems to take longer to change, runs slow.
// .... .... .... b... Sprite sort memory written to $0000 ?
// .... .... .... .d.. Sprite render debug (shows 8x1 sliver per sprite).
// .... .... .... ..rr Reset bits.

#define GCU_CONFIG_B14_BIT               0xE
#define GCU_CONFIG_VSYNC_IN_BIT          0xC
#define GCU_CONFIG_HSYNC_PHASE_RESET_BIT 0xA
#define GCU_CONFIG_SPRITES_DISABLE_BIT   0x9
#define GCU_CONFIG_SPRITES_SINGLE_BIT    0x8
#define GCU_CONFIG_SPRITE_SLOW_BIT       0x7
#define GCU_CONFIG_SORT_DEBUG_BIT        0x3
#define GCU_CONFIG_DRAW_DEBUG_BIT        0x2
#define GCU_CONFIG_RESET1_BIT            0x1
#define GCU_CONFIG_RESET0_BIT            0x0

#ifndef __ASSEMBLER__

// The GCU can only accept VRAM writes *outside* of vertical blank!
// On hardware with the external line counter and sync line buffer the status is
// checked using an I/O port.
static inline void sai_gcu_wait_access(void)
{
	const volatile uint16_t *gcu_status = (const volatile uint16_t *)(GCU_BASE+GCU_STATUS_OFFS);
	while (!(*gcu_status & SAI_BITVAL(GCU_STATUS_BLANKING_BIT))) {}
}

//
// Scroll configuration for each plane.
//

// Indices are GCU_PLANE_*.
typedef struct SaiGcuPlaneCfg
{
	int16_t x, y;
} SaiGcuPlaneCfg;

extern SaiGcuPlaneCfg g_sai_gcu_scroll[4];

// Configure the GCU registers and initialize Video RAM.
void sai_gcu_init(void);

// Updates scroll registers with single frame queue to sync with sprites.
void sai_gcu_on_vbl(void);

// Plane ID is one of GCU_PLANE_*.
// Clearing the sprite list is possible but is pointless if the sprite system
// is managed.
void sai_gcu_clear_plane(uint16_t plane_id);

#else

	.struct	0

SaiGcuPlaneCfg.x:      ds.w 1
SaiGcuPlaneCfg.y:      ds.w 1
SaiGcuPlaneCfg.len:

	.extern	g_sai_gcu_scroll
	.extern	g_sai_gcu_cfg

	.extern	sai_min_gcu_init
	.extern	sai_gcu_init
	.extern	sai_gcu_on_vbl
	.extern	sai_gcu_clear_plane

#endif  // __ASSEMBLER__


#ifdef __cplusplus
}
#endif  // __cplusplus

