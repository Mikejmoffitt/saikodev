#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#include "sai/target.h"
#include "sai/macro.h"

// CPS-A-01 "PPU1" Registers.
// Memory bases for various areas are dictated by specifying the 68k-space
// address (presumably accessed via DMA). The lower 8 bits are dropped, so
// the address should be divided by 256 (shifted by 8).
#define SAI_CPSA_OFFS_OBJ_BASE         0x00
#define SAI_CPSA_OFFS_SCROLL1_BASE     0x02
#define SAI_CPSA_OFFS_SCROLL2_BASE     0x04
#define SAI_CPSA_OFFS_SCROLL3_BASE     0x06
#define SAI_CPSA_OFFS_ROWSCROLL_BASE   0x08
#define SAI_CPSA_OFFS_PALETTE_BASE     0x0A

#define SAI_CPSA_OFFS_SCROLL1_X        0x0C
#define SAI_CPSA_OFFS_SCROLL1_Y        0x0E
#define SAI_CPSA_OFFS_SCROLL2_X        0x10
#define SAI_CPSA_OFFS_SCROLL2_Y        0x12
#define SAI_CPSA_OFFS_SCROLL3_X        0x14
#define SAI_CPSA_OFFS_SCROLL3_Y        0x16
#define SAI_CPSA_OFFS_STAR1_X          0x18
#define SAI_CPSA_OFFS_STAR1_Y          0x1A
#define SAI_CPSA_OFFS_STAR2_X          0x1C
#define SAI_CPSA_OFFS_STAR2_Y          0x1E

#define SAI_CPSA_OFFS_ROWSCROLL_START  0x20

// The values respected by this register for the layer enable change based upon
// the CPS-B-xx number, or CPS-B-21 configuration. For CPS2, CPS-B-01, and most
// instances of batteryless CPS-B-21, the values are as seen below.
//
// fedc ba98 7654 3210
// f... .... .... .... | flip screen
// .?.. .... .... .... | unknown. Daimakaimura sets this
// .... .... ..ss .... | starfield enable (TODO: does this really work?)
// .... .... .... 3... | scroll 3 enable? could this be size change?
// .... .... .... .2.. | scroll 2 enable
// .... .... .... ..1. | scroll 1 enable
// .... .... .... ...L | line scroll enable for scroll 2
#define SAI_CPSA_OFFS_VIDEO_CTRL       0x22

#define SAI_CPSA_VIDEO_CTRL_FLIP       SAI_BITVAL(15)
#define SAI_CPSA_VIDEO_CTRL_UNK14      SAI_BITVAL(14)
#define SAI_CPSA_VIDEO_CTRL_STAR2      SAI_BITVAL(5)
#define SAI_CPSA_VIDEO_CTRL_STAR1      SAI_BITVAL(4)
#define SAI_CPSA_VIDEO_CTRL_SCROLL3    SAI_BITVAL(3)
#define SAI_CPSA_VIDEO_CTRL_SCROLL2    SAI_BITVAL(2)
#define SAI_CPSA_VIDEO_CTRL_SCROLL1    SAI_BITVAL(1)
#define SAI_CPSA_VIDEO_CTRL_LINESCROLL SAI_BITVAL(0)

#define SAI_CPSA_VIDEO_CTRL_DEFAULT (SAI_CPSA_VIDEO_CTRL_SCROLL1 | \
                                     SAI_CPSA_VIDEO_CTRL_SCROLL2 | \
                                     SAI_CPSA_VIDEO_CTRL_SCROLL3)

#define SAI_CPSA_OFFS_TILEMAP_UNK      0x24
#define SAI_CPSA_OFFS_ID               0x26
#define SAI_CPSA_SCANLINE              0x28

// CPS-B-21 "PPU2" registers.
#define SAI_CPSB_OFFS_MULT_X           0x00
#define SAI_CPSB_OFFS_MULT_Y           0x02
#define SAI_CPSB_OFFS_MULT_LSB         0x04
#define SAI_CPSB_OFFS_MULT_MSB         0x06
#define SAI_CPSB_OFFS_MULT_TC          0x08

#define SAI_CPSB_OFFS_CHECK1           0x0A
#define SAI_CPSB_OFFS_CHECK2           0x0C

#define SAI_CPSB_OFFS_RASTER1          0x0E
#define SAI_CPSB_OFFS_RASTER2          0x10
#define SAI_CPSB_OFFS_RASTER3          0x12

// As with CPSA_VIDEO_CTRL, the register locations or masks may change based
// upon the hardware configuration. These values cover the common use case of
// CPS-B-21, CPS-B-01, etc.
//
// fedc ba98 7654 3210
// ?... .... .... .... | unknown, seemingly no-op. Set by a few games.
// ..00 .... .... .... | draw order 0 (00  none, 01  scroll 1, etc)
// .... 11.. .... .... | draw order 1
// .... ..22 .... .... | draw order 2
// .... .... 33.. .... | draw order 3
// .... .... ..ss .... | starfield enable
// .... .... .... 3... | scroll 1 enable
// .... .... .... .2.. | scroll 2 enable
// .... .... .... ..1. | scroll 0 enable
//
// 0x2A42 is a typical default.
#define SAI_CPSB_OFFS_LAYER_CTRL    0x26

#define SAI_CPSB_LAYER_CTRL_DRAW0_BIT  12
#define SAI_CPSB_LAYER_CTRL_DRAW1_BIT  10
#define SAI_CPSB_LAYER_CTRL_DRAW2_BIT  8
#define SAI_CPSB_LAYER_CTRL_DRAW3_BIT  6
#define SAI_CPSB_LAYER_CTRL_STAR2_EN   SAI_BITVAL(4)
#define SAI_CPSB_LAYER_CTRL_STAR1_EN   SAI_BITVAL(3)
#define SAI_CPSB_LAYER_CTRL_SCROLL3_EN SAI_BITVAL(2)
#define SAI_CPSB_LAYER_CTRL_SCROLL2_EN SAI_BITVAL(1)
#define SAI_CPSB_LAYER_CTRL_SCROLL1_EN SAI_BITVAL(0)

// The default enables the three layers, and orders them like so:
// top: scroll 1
//      scroll 2
//      scroll 3
// bot: --------

#define SAI_CPSB_LAYER_ORDER_DEF(ly0, ly1, ly2, ly3) (((ly0) << SAI_CPSB_LAYER_CTRL_DRAW0_BIT) | \
                                                      ((ly1) << SAI_CPSB_LAYER_CTRL_DRAW1_BIT) | \
                                                      ((ly2) << SAI_CPSB_LAYER_CTRL_DRAW2_BIT) | \
                                                      ((ly3) << SAI_CPSB_LAYER_CTRL_DRAW3_BIT))

#define SAI_CPSB_LAYER_ORDER_DEFAULT (SAI_CPSB_LAYER_ORDER_DEF(1, 2, 3, 0) | 0xE)

//
// Priority mask registers. Used for per-color masking with the scroll plane
// that is right below sprites. This is a CPS1-only feature.
//
#define SAI_CPSB_OFFS_PRIO0         0x28
#define SAI_CPSB_OFFS_PRIO1         0x2A
#define SAI_CPSB_OFFS_PRIO2         0x2C
#define SAI_CPSB_OFFS_PRIO3         0x2E

//
// Palette control. Bitfield which indicates which palette regions should be
// copied. Regions are organized based upon the layer they are used on.
//
// fedc ba98 7654 3210
// .... .... ..ss .... | copy star plane palettes (unverified?)
// .... .... .... 3... | copy scroll 3 palettes
// .... .... .... .2.. | copy scroll 2 palettes
// .... .... .... ..1. | copy scroll 1 palettes
// .... .... .... ...O | copy object plane palettes
//
// There is a quirk (hardware bug?) described in mame/capcom/cps1_v.cpp, where
// if bit 0 (object plane palette) is not set, the first page from VRAM is
// copied to whichever plane next has a bit set. For example, if only bit 1 is
// set (scroll 1), scroll 1 takes data from VRAM page 0.
//
// Games typically just write $3F to copy everything at once. For a CPS2 game
// I expect $0F is an adequate value to write.
#define SAI_CPSB_OFFS_PAL_CTRL      0x30

#define SAI_CPSB_PAL_CTRL_STAR2     SAI_BITVAL(5)
#define SAI_CPSB_PAL_CTRL_STAR1     SAI_BITVAL(4)
#define SAI_CPSB_PAL_CTRL_SCROLL3   SAI_BITVAL(3)
#define SAI_CPSB_PAL_CTRL_SCROLL2   SAI_BITVAL(2)
#define SAI_CPSB_PAL_CTRL_SCROLL1   SAI_BITVAL(1)
#define SAI_CPSB_PAL_CTRL_OBJ       SAI_BITVAL(0)
#define SAI_CPSB_PAL_CTRL_DEFAULT   (SAI_CPSB_PAL_CTRL_OBJ | \
                                     SAI_CPSB_PAL_CTRL_SCROLL1 | \
                                     SAI_CPSB_PAL_CTRL_SCROLL2 | \
                                     SAI_CPSB_PAL_CTRL_SCROLL3 | \
                                     SAI_CPSB_PAL_CTRL_STAR1 | \
                                     SAI_CPSB_PAL_CTRL_STAR2)

#define SAI_CPSB_ID                 0x32
#define SAI_CPSB_SCANLINE           0x3E

//
// Background tilemaps are defined as such:
//
// fedc ba98 7654 3210 fedc ba98 7654 3210
// .... ...p p... .... .... .... .... .... Priority (?)
// .... .... .yx. .... .... .... .... .... Y/X flip
// .... .... ...c cccc .... .... .... .... Color
// .... .... .... .... aaaa aaaa aaaa aaaa Tile address
#define SAI_CPS_BG_FLIPX           (SAI_BITVAL(5))
#define SAI_CPS_BG_FLIPY           (SAI_BITVAL(6))
#define SAI_CPS_BG_ATTR(pal, prio) ((pal) | ((prio)<<7))

// VRAM position calculation
#define SAI_CPS_SCROLL1_OFFS(x, y) (((x)*128)+((y)*4))
#define SAI_CPS_SCROLL2_OFFS(x, y) (((x)*64)+((y)*4))
#define SAI_CPS_SCROLL3_OFFS(x, y) (((x)*32)+((y)*4))

#define SAI_CPS_SCROLL_X_DEFAULT -64
#define SAI_CPS_SCROLL_Y_DEFAULT -16

#define SAI_CPS_PLANE_SCROLL1 0
#define SAI_CPS_PLANE_SCROLL2 1
#define SAI_CPS_PLANE_SCROLL3 2
#define SAI_CPS_PLANE_STAR1 3
#define SAI_CPS_PLANE_STAR2 4

#ifndef __ASSEMBLER__
void sai_cps_ppu_init(void);
void sai_cps_ppu_trigger_pal_dma(void);

static inline void sai_cps_set_scroll_x(uint16_t plane, int16_t val);
static inline void sai_cps_set_scroll_y(uint16_t plane, int16_t val);

// inline implementations ------------------------------------------------------

static inline void sai_cps_set_scroll_x(uint16_t plane, int16_t val)
{
	volatile int16_t *reg = (volatile int16_t *)(SAI_CPSA_REG_BASE+SAI_CPSA_OFFS_SCROLL1_X+(plane*2));
	*reg = val;
}

static inline void sai_cps_set_scroll_y(uint16_t plane, int16_t val)
{
	volatile int16_t *reg = (volatile int16_t *)(SAI_CPSA_REG_BASE+SAI_CPSA_OFFS_SCROLL1_Y+(plane*2));
	*reg = val;
}

#else
	.extern	sai_cps_ppu_init
	.extern	sai_min_cps_ppu_init
	.extern	sai_cps_ppu_trigger_pal_dma
#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
