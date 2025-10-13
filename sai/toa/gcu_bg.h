//
// Toaplan GCU "GP9001" backgrounds.
//

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#include "sai/toa/gcu.h"

//
// GCU Background tiles.
//
// Background planes A, B, and C start at $0000, $0800, and $1000 repsectively.
//
// Each tile is two words, composed as such:
//
// $00 - 設定・色 attributes, color, etc.
// fedc ba98 7654 3210
// .... pppp .... .... プライオリティー　priority
// .... .... .ccc cccc 色　              color

// $01 - パターン pattern code. Many titles use this exclusively.
// fedc ba98 7654 3210
// CCCC CCCC CCCC CCCC パターンコード    pattern code (bits 0-15)

#define GCU_BG_W_PX 512
#define GCU_BG_H_PX 512
#define GCU_BG_TILESIZE 16
#define GCU_BG_W_TILES (GCU_BG_W_PX / GCU_BG_TILESIZE)
#define GCU_BG_H_TILES (GCU_BG_H_PX / GCU_BG_TILESIZE)

//  pal: $00-$7F
// prio: $0 - $F
#define GCU_BG_ATTR(pal, prio) (((prio)<<8) | ((pal)))
#define GCU_BG_AT32(pal, prio) (GCU_BG_ATTR(pal, prio)<<16)

// VRAM Math
#define GCU_BG_OFFS(x, y)        (((x)+((y)*GCU_BG_W_TILES))*2)
#define GCU_BG_ADDR(plane, x, y) (((plane)*GCU_BG_PAGE_WORDS) + GCU_BG_OFFS(x, y))

#ifdef __cplusplus
}
#endif  // __cplusplus

