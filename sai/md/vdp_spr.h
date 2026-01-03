#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdbool.h>
#include <stdint.h>
#endif  // __ASSEMBLER__

#include "sai/md/vdp.h"

//
// Compiler flags:
//
// * SAI_MD_VDP_SPR_DIRECT
//
// Set this flag to tell Saikodev that you don't want it to set the link fields
// or terminate the sprite list. The sprite buffer still exists, but it becomes
// the programmer's responsibility to correctly terminate the sprite list and
// hide unwanted sprites.
//
// In direct mode, set g_sai_vdp_spr_count to the size of the sprite list that
// you will transfer. By default, this will be set to the full sprite list size
// (80) but if you are using less, set it accordingly (especially if using H32
// mode, where only 64 sprites may be used). In direct mode, g_sai_vdp_spr_count
// is not cleared upon calling sai_vdp_spr_finish(), so you may set it once.
//
// g_sai_vdp_spr_next is also still present, and you can use the drawing code
// defined below, but keep in mind that unless you are careful you run the risk
// of clobbering manual sprite list manipulations.
//
// * SAI_MD_VDP_SPR_NO_OFFSET
//
// Set this flag to disable the automatic +128 offset applied to sprites.
//

#ifndef __ASSEMBLER__

typedef struct SaiVdpSpr
{
	uint16_t y;
	uint8_t size;
	uint8_t link;  // Leave this one alone
	uint16_t attr;
	uint16_t x;
} SaiVdpSpr;

extern SaiVdpSpr g_sai_vdp_spr[VDP_SPR_COUNT];
extern SaiVdpSpr *g_sai_vdp_spr_next;
extern uint16_t g_sai_vdp_spr_count;

// Clears the sprite list and sets up link numbers.
void sai_vdp_spr_init(void);

// Prepares the sprite list for a new frame.
void sai_vdp_spr_start(void);

// Terminates the sprite list and schedules a DMA.
void sai_vdp_spr_finish(void);

// All sprite draw routines return true if the sprite list was full.

// Draw a sprite using a parameter struct.
static inline bool sai_vdp_spr_draw_st(const SaiVdpSpr *param);

// Draw a sprite using immediate parameters.
static inline bool sai_vdp_spr_draw(uint16_t x, uint16_t y,
                                    uint16_t attr, uint8_t size);

// Draw a sprite using immediate parameters, after performing bounds checks.
static inline bool sai_vdp_spr_draw_safe(uint16_t x, uint16_t y,
                                         uint16_t attr, uint8_t size);

// Masks off any sprites on scanlines that span between y and the height.
static inline bool sai_vdp_spr_mask_line_full(int16_t y, uint8_t size);

// Masks off any sprites on scanlines that intersect two sprite positions.
static inline bool sai_vdp_spr_mask_line_comb(int16_t y1, uint8_t size1,
                                              int16_t y2, uint8_t size2);

// Inline implementations
// -----------------------------------------------------------------------------

static inline bool sai_vdp_spr_draw_st(const SaiVdpSpr *param)
{
	if (g_sai_vdp_spr_count >= VDP_SPR_COUNT) return true;
#ifndef SAI_MD_VDP_SPR_NO_OFFSET
	const uint16_t x = param->x + VDP_SPR_STATIC_OFFS;
	const uint16_t y = param->y + VDP_SPR_STATIC_OFFS;
#else
	const uint16_t x = param->x;
	const uint16_t y = param->y;
#endif  // SAI_MD_VDP_SPR_NO_OFFSET
	g_sai_vdp_spr_next->y = y;
	g_sai_vdp_spr_next->size = param->size;
	g_sai_vdp_spr_next->attr = param->attr;
	g_sai_vdp_spr_next->x = x;
	g_sai_vdp_spr_next++;
	g_sai_vdp_spr_count++;
	return false;
}

static inline bool sai_vdp_spr_draw(uint16_t x, uint16_t y,
                                    uint16_t attr, uint8_t size)
{
	if (g_sai_vdp_spr_count >= VDP_SPR_COUNT) return true;
#ifndef SAI_MD_VDP_SPR_NO_OFFSET
	x += VDP_SPR_STATIC_OFFS;
	y += VDP_SPR_STATIC_OFFS;
#endif  // SAI_MD_VDP_SPR_NO_OFFSET
	g_sai_vdp_spr_next->y = y;
	g_sai_vdp_spr_next->size = size;
	g_sai_vdp_spr_next->attr = attr;
	g_sai_vdp_spr_next->x = x;
	g_sai_vdp_spr_next++;
	g_sai_vdp_spr_count++;
	return false;
}

static inline bool sai_vdp_spr_draw_safe(uint16_t x, uint16_t y,
                                         uint16_t attr, uint8_t size)
{
	if (y <= VDP_SPR_STATIC_OFFS-32) return true;
	if (y >= VDP_SPR_STATIC_OFFS+240) return true;
	if (x <= VDP_SPR_STATIC_OFFS-32) return true;
	if (y >= VDP_SPR_STATIC_OFFS+320) return true;
	return sai_vdp_spr_draw(y, x, attr, size);
}

static inline bool sai_vdp_spr_mask_line_full(int16_t y, uint8_t size)
{
	if (g_sai_vdp_spr_count >= VDP_SPR_COUNT) return true;
	g_sai_vdp_spr_next->x = 0;
	g_sai_vdp_spr_next->y = y;
	g_sai_vdp_spr_next->size = size;
	g_sai_vdp_spr_next++;
	g_sai_vdp_spr_count++;
	return false;
}

static inline bool sai_vdp_spr_mask_line_overlap(int16_t y1, uint8_t size1,
                                                 int16_t y2, uint8_t size2)
{
	if (g_sai_vdp_spr_count >= VDP_SPR_COUNT-1) return true;
	g_sai_vdp_spr_next->x = 0;
	g_sai_vdp_spr_next->attr = 0;
	g_sai_vdp_spr_next->y = y1;
	g_sai_vdp_spr_next->size = size1;
	g_sai_vdp_spr_next++;
	g_sai_vdp_spr_count++;

	g_sai_vdp_spr_next->x = 1;
	g_sai_vdp_spr_next->attr = 0;
	g_sai_vdp_spr_next->y = y2;
	g_sai_vdp_spr_next->size = size2;
	g_sai_vdp_spr_next++;
	g_sai_vdp_spr_count++;
	return false;
}



#else

	.struct	0
SaiVdpSpr.y:     ds.w 1
SaiVdpSpr.size:  ds.b 1
SaiVdpSpr.link:  ds.b 1
SaiVdpSpr.attr:  ds.w 1
SaiVdpSpr.x:     ds.w 1
SaiVdpSpr.len:

	.extern	g_sai_vdp_spr
	.extern g_sai_vdp_spr_next
	.extern g_sai_vdp_spr_count

	.extern	sai_vdp_spr_init
	.extern	sai_min_vdp_spr_init
	.extern	sai_vdp_spr_start
	.extern	sai_vdp_spr_finish

#endif  // __ASSEMBLER__



#ifdef __cplusplus
}
#endif  // __cplusplus
