//
// Saikodev Neo-Geo VRAM addresses. An address refers to the location of a
// 16-bit word.
//
// Lower zone:
// $0000 - $6FFF | SCB1        | Sprite tilemaps
// $7000 - $74FF | FIXMAP      | FIX Layer
// $7500 - $7FFF | FIX Ext.    | Unused / For mapper use

// Upper zone:
// $8000 - $81FF | SCB2        | Shrink values
// $8200 - $83FF | SCB3        | Sprite Y, height, flag
// $8300 - $84FF | SCB4        | Sprite X

// $8600 - $867F | objlist (0) | Sprite work area, even lines
// $8680 - $867F | objlist (1) | Sprite work area, odd lines
// $8700 - $87FF | Unused      | Free
//
#pragma once

#define VRAM_SCB1   0x0000
#define VRAM_FIXMAP 0x7000
#define VRAM_SCB2   0x8000
#define VRAM_SCB3   0x8200
#define VRAM_SCB4   0x8400

