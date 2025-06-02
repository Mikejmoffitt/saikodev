#pragma once

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__
#include "sai/target.h"

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

//
// Priority mask registers. Used for per-color masking with the scroll plane
// that is right below sprites. This is a CPS1-only feature.
//
#define SAI_CPSB_OFFS_PRIO0         0x28
#define SAI_CPSB_OFFS_PRIO1         0x2A
#define SAI_CPSB_OFFS_PRIO2         0x2C
#define SAI_CPSB_OFFS_PRIO3         0x2E
