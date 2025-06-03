#pragma once

#ifndef __ASSEMBLER__
#include <stdbool.h>
#include <stdint.h>
#endif  // __ASSEMBLER__
#include "sai/macro.h"
#include "sai/target.h"

//
// HARDWARE OPTIONS
// ================
//
// SAI_BG038_COUNT: anywhere from 1-4
// SAI_BG038_COLOR_GROUP_SPREAD: if set, color groups are assigned by index + 1.
//                               else, all take group 1, leaving 0 to sprites.

// ------------------------------------------------------------------------------
//
// BG038 Background Generators
//
// ------------------------------------------------------------------------------
//
// The background generators are simple and straight-forward tile renderers. Each
// plane has its own attribute memory (32KiB) laid out as follows:
//
// $0000 - $0FFF: Nametable (main / 16x16)
// $1000 - $03FF: Line pointer and line scroll table
// $4000 - $7FFF: Nametable (8x8)
//
// It is possible to physically equip the chip with only 16KiB of RAM. In such a
// configuration, the upper address bit that would select between the 16 and 8
// pixel sized nametables does nothing, and this memory is shared.
//
// VRAM Data:
//
// Each nametable entry is four bytes, and is laid out like so:
//
// fedc ba98 7654 3210 fedc ba98 7654 3210
// pp.. .... .... .... .... .... .... .... Priority (0 is the lowest)
// ..cc cccc .... .... .... .... .... .... Color (lower six palette bits)
// .... .... .... ...t tttt tttt tttt tttt Tile index
//
// The scroll table has 512 entries, where each maps to one line of the plane.
// For each line, there is a four-byte entry like so:
//
// fedc ba98 7654 3210 fedc ba98 7654 3210
// .... ...s ssss ssss .... .... .... .... Scroll Offset
// .... .... .... .... 1... .... .... .... Unknown yet? Seems to be set to 1
// .... .... .... .... .... ...l llll llll Line source (starting at 1)
//
// Registers:
//
// +$00 - Scroll register X
//
// fedc ba98 7654 3210
// f... .... .... .... X flip
// .l.. .... .... .... Line scroll table enable
// .... ...s ssss ssss Plane X scroll
//
// +$02 - Scroll register Y
//
// fedc ba98 7654 3210
// F... .... .... .... Y flip
// .L.. .... .... .... Line pointer table enable
// ..t. .... .... .... Tile size (1 = 16x16)
// .... ...s ssss ssss Plane Y scroll
//
//
// +$04 - Control register
//
// fedc ba98 7654 3210
// .... .... ...d .... Disable
// .... .... .... pppp Palette group select (upper four palette bits)
//

// SAI_BG038_COUNT can vary by the hardware.
#ifndef SAI_BG038_COUNT
#define SAI_BG038_COUNT 3
#endif  // BG038_COUNT

#define BG038_OFFS_SCRX            0
#define BG038_OFFS_SCRY            2
#define BG038_OFFS_CTRL            4

// Bits for the pal base / ctrl register
#define BG038_CTRL_DISABLE_BIT     4
#define BG038_CTRL_DISABLE         SAI_BITVAL(BG038_CTRL_DISABLE_BIT)

#define BG038_SCRX_FLIP        SAI_BITVAL(15)
#define BG038_SCRX_LINESCROLL  SAI_BITVAL(14)

#define BG038_SCRY_FLIP        SAI_BITVAL(15)
#define BG038_SCRY_LINEPTR     SAI_BITVAL(14)

#define BG038_ATTR(pal, prio) (((pri)<<0x6)|((pal)))
#define BG038_ATTR16(pal, prio) (BG038_ATTR(pal, prio)<<8)

#define BG038_VRAM_NT              0x0000
#define BG038_VRAM_NT_SIZE         0x1000
#define BG038_VRAM_LINE_PTR        0x1000
#define BG038_VRAM_LINE_PTR_SIZE   0x0400
#define BG038_VRAM_NT8             0x4000
#define BG038_VRAM_NT8_SIZE        0x4000

#ifndef __ASSEMBLER__

// Tilemap entry
typedef struct Bg038Tile
{
	union
	{
		struct
		{
			uint8_t attr;
			uint8_t pad[3];
		};
		uint32_t code;  // 0-131072
	}
} Bg038Tile;

// Scroll table entry
typedef struct Bg038LineDef
{
	int16_t scroll;  // 0-512
	uint16_t src;  // 0-512; bit 15 often set
} Bg038LineDef;

// Struct representing register state to send to BG038.
typedef struct Bg038State
{
	int16_t sx, sy;
	uint16_t flagx, flagy;  // ORed with sx, sy before reaching registers.
	uint16_t ctrl;
} Bg038State;

extern Bg038State g_sai_bg038[SAI_BG038_COUNT];

void sai_bg038_init(void);
void sai_bg038_on_vbl(void);

static inline void sai_bg038_set_disable(uint16_t idx, bool disable);
static inline void sai_bg038_set_color_group(uint16_t idx, uint8_t group);

// Inline implementations
static inline void sai_bg038_set_disable(uint16_t idx, bool disable)
{
	if (disable) SAI_BSET(g_sai_bg038[idx].ctrl, BG038_CTRL_DISABLE_BIT);
	else SAI_BCLR(g_sai_bg038[idx].ctrl, BG038_CTRL_DISABLE_BIT);
}

static inline void sai_bg038_set_color_group(uint16_t idx, uint8_t group)
{
	g_sai_bg038[idx].ctrl &= 0xFFF0;
	g_sai_bg038[idx].ctrl |= group;
}


#else

	.struct 0
Bg038Tile.attr:
Bg038Tile.code: ds.l 1
Bg038Tile.len:

	.struct 0
Bg038LineDef.scroll: ds.w 1
Bg038LineDef.src:    ds.w 1
Bg038LineDef.len:

	.struct 0
Bg038State.sx:       ds.w 1
Bg038State.sy:       ds.w 1
Bg038State.flagx:    ds.w 1
Bg038State.flagy:    ds.w 1
Bg038State.ctrl:     ds.w 1
Bg038State.len:

	.extern	g_sai_bg038;

	.extern	sai_bg038_init
	.extern	sai_min_bg038_init
	.extern	sai_bg038_on_vbl



#endif  // __ASSEMBLER__
