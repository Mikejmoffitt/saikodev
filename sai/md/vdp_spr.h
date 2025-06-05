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

// Draw a sprite using a parameter struct.
static inline void sai_vdp_spr_draw_st(const SaiVdpSpr *param);

// Draw a sprite using immediate parameters.
static inline void sai_vdp_spr_draw(uint16_t x, uint16_t y,
                                    uint16_t attr, uint8_t size);

// Draw a sprite using immediate parameters, after performing bounds checks.
static inline void sai_vdp_spr_draw_safe(uint16_t x, uint16_t y,
                                         uint16_t attr, uint8_t size);

// Masks off any sprites on scanlines that span between y and the height.
static inline void sai_vdp_spr_mask_line_full(int16_t y, uint8_t size);

// Masks off any sprites on scanlines that intersect two sprite positions.
static inline void sai_vdp_spr_mask_line_comb(int16_t y1, uint8_t size1,
                                              int16_t y2, uint8_t size2);

// Inline implementations
// -----------------------------------------------------------------------------

static inline void sai_vdp_spr_draw_st(const SaiVdpSpr *param)
{
	if (g_sai_vdp_spr_count >= VDP_SPR_COUNT) return;
	g_sai_vdp_spr_next->y = param->y;
	g_sai_vdp_spr_next->size = param->size;
	g_sai_vdp_spr_next->attr = param->attr;
	g_sai_vdp_spr_next->x = param->x;
	g_sai_vdp_spr_next++;
	g_sai_vdp_spr_count++;
}

static inline void sai_vdp_spr_draw(uint16_t x, uint16_t y,
                                    uint16_t attr, uint8_t size)
{
	if (g_sai_vdp_spr_count >= VDP_SPR_COUNT) return;
	g_sai_vdp_spr_next->y = y;
	g_sai_vdp_spr_next->size = size;
	g_sai_vdp_spr_next->attr = attr;
	g_sai_vdp_spr_next->x = x;
	g_sai_vdp_spr_next++;
	g_sai_vdp_spr_count++;
}

static inline void sai_vdp_spr_draw_safe(uint16_t x, uint16_t y,
                                         uint16_t attr, uint8_t size)
{
	if (y <= 128-32) return;
	if (y >= 128+240) return;
	if (x <= 128-32) return;
	if (y >= 128+320) return;
	sai_vdp_spr_draw(y, x, attr, size);
}

static inline void sai_vdp_spr_mask_line_full(int16_t y, uint8_t size)
{
	if (g_sai_vdp_spr_count >= VDP_SPR_COUNT) return;
	g_sai_vdp_spr_next->x = 0;
	g_sai_vdp_spr_next->y = y;
	g_sai_vdp_spr_next->size = size;
	g_sai_vdp_spr_next++;
	g_sai_vdp_spr_count++;
}

static inline void sai_vdp_spr_mask_line_overlap(int16_t y1, uint8_t size1,
                                                 int16_t y2, uint8_t size2)
{
	if (g_sai_vdp_spr_count >= VDP_SPR_COUNT-1) return;
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
