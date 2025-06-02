#pragma once

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__
#include "sai/target.h"

// CPS-A-01 Registers
// Memory bases for various areas are dictated by specifying the 68k-space
// address (presumably accessed via DMA). The lower 8 bits are dropped, so
// the address should be divided by 256 (shifted by 8).
#define SAI_CPSA_OFFS_OBJA_BASE        0x00
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
#define SAI_CPSA_OFFS_TILEMAP_UNK      0x24
#define SAI_CPSA_OFFS_ID               0x26
#define SAI_CPSA_SCANLINE              0x28
