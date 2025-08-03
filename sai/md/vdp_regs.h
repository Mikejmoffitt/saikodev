	#pragma once
#include "sai/macro.h"

/* 0000-BFFF (1536 tiles) free */
#define VRAM_SCRA_BASE_DEFAULT  0xC000
#define VRAM_SCRW_BASE_DEFAULT  0xD000
#define VRAM_SCRB_BASE_DEFAULT  0xE000
/* F000-F7FF (64 tiles) free */
#define VRAM_HSCR_BASE_DEFAULT  0xF800
#define VRAM_SPR_BASE_DEFAULT   0xFC00

// Register indices by name
#define VDP_MODESET1  0x00
#define VDP_MODESET2  0x01
#define VDP_SCRABASE  0x02
#define VDP_SCRWBASE  0x03
#define VDP_SCRBBASE  0x04
#define VDP_SPRBASE   0x05
#define VDP_128_SPCGA 0x06
#define VDP_BGCOL     0x07
#define VDP_M3HSCROLL 0x08
#define VDP_M3VSCROLL 0x09
#define VDP_HINTC     0x0A
#define VDP_MODESET3  0x0B
#define VDP_MODESET4  0x0C
#define VDP_HSCRBASE  0x0D
#define VDP_128_BPCGA 0x0E
#define VDP_AUTOINC   0x0F
#define VDP_PLANESIZE 0x10
#define VDP_WINHORI   0x11
#define VDP_WINVERT   0x12
#define VDP_DMALEN1   0x13
#define VDP_DMALEN2   0x14
#define VDP_DMASRC1   0x15
#define VDP_DMASRC2   0x16
#define VDP_DMASRC3   0x17

// Register bits

/*
$80 - Mode Set Register 1
v... .... SMSVL - Columns 24-31 (but not 32-39) forced to a scroll of 0.
.h.. .... SMSHL - SMS Legacy, unused or unknown effect.
..l. .... LCB   - Leftmost column blank. Covers leftmost 8-pixel column.
...I .... IE1   - Horizontal interrupt enable. Will trigger when set.
.... e... VC0   - Outputs bit 0 of V. counter on HSync pin. Set to 0!
.... .f.. FCE   - Full color enable. Enables full palette bit depth.
.... ..s. M3    - HV counter latch enable. When set, HV counter is stopped.
.... ...o OVER  - Overlay mode. Enables locking to ext. sync via csync pin.
*/
#define VDP_MODESET1_SMSVL     SAI_BITVAL(7)
#define VDP_MODESET1_SMSHL     SAI_BITVAL(6)
#define VDP_MODESET1_LCB       SAI_BITVAL(5)
#define VDP_MODESET1_IE1       SAI_BITVAL(4)
#define VDP_MODESET1_VC0       SAI_BITVAL(3)
#define VDP_MODESET1_M4        SAI_BITVAL(2)
#define VDP_MODESET1_M3        SAI_BITVAL(1)
#define VDP_MODESET1_DE        SAI_BITVAL(0)
#define VDP_MODESET1_DEFAULT   (VDP_MODESET1_M4)
/*
$81 - Mode Set Register 2
v... .... VR128 - VRAM 128K mode. Enables bus for second set of VRAM.
.d.. .... DISP  - Enables display. Shows the background color when set to 0.
..I. .... IE0   - Vertical interrupt enable.
...d .... M1    - Enables DMA functionality.
.... h... M2    - Sets vertical size to 30 cells (PAL only).
.... .m.. M5    - Enables MD display mode. When cleared, VDP is in SMS mode.
.... ..s. SMSSZ - Obsolete SMS sprite size selection.
.... ...M SMSMG - Obsolete SMS sprite mag. Makes scroll offset sync pulse.
*/
#define VDP_MODESET2_VR128     SAI_BITVAL(7)
#define VDP_MODESET2_DISP      SAI_BITVAL(6)
#define VDP_MODESET2_IE0       SAI_BITVAL(5)
#define VDP_MODESET2_M1        SAI_BITVAL(4)
#define VDP_MODESET2_M2        SAI_BITVAL(3)
#define VDP_MODESET2_M5        SAI_BITVAL(2)
#define VDP_MODESET2_SMSSZ     SAI_BITVAL(1)
#define VDP_MODESET2_SMSMG     SAI_BITVAL(0)
#define VDP_MODESET2_DEFAULT   (VDP_MODESET2_DISP | VDP_MODESET2_M5 | VDP_MODESET2_M1)
/*
$8B - Mode Set Register 3
a... .... ADMUX - Drives color bus with pixel data when set, else CPU address.
.c.. .... DRAMS - Enables work DRAM signals when set. Set to 0.
..xx .... UNK   - Unknown meaning or effect.
.... I... IE2   - Enables external interrupts (e.g. controller TH pin).
.... .v.. VCELL - Vertical scroll mode. See VdpVmode enum.
.... ..h. HS1   - Horizontal scroll mode (bit 1). See VdpHmode enum.
.... ...l HS0   - Horizontal scroll mode (bit 0).
*/
#define VDP_MODESET3_ADMUX     SAI_BITVAL(7)
#define VDP_MODESET3_DRAMS     SAI_BITVAL(6)
#define VDP_MODESET3_IE2       SAI_BITVAL(3)
#define VDP_MODESET3_VCELL     SAI_BITVAL(2)
#define VDP_MODESET3_HS1       SAI_BITVAL(1)
#define VDP_MODESET3_HS0       SAI_BITVAL(0)
#define VDP_MODESET3_DEFAULT   VDP_MODESET3_ADMUX
/*
Register $0C - Mode Set Register 4
r... .... RS1   - Select external dot clock (EDCLK). Used for H40 on MD.
.s.. .... VSCLK - Outputs pixel clock on VSync pin. Used by 32x and C/C2.
..h. .... HSCIN - Hsync pin becomes an input. Used by 32x.
...c .... SPAEN - Enable sprite/plane indicator pin as output. Used by C/C2.
.... S... SHI   - Enable shadow/highlight mode.
.... .L.. LSM1  - Interlace mode (bit 1). See VdpInterlaceMode enum.
.... ..l. LSM0  - Interlace mode (bit 0).
.... ...R RS0   - Selects horizontal cell mode and dot clock divisor.
*/
#define VDP_MODESET4_RS1       SAI_BITVAL(7)
#define VDP_MODESET4_VSCLK     SAI_BITVAL(6)
#define VDP_MODESET4_HSCIN     SAI_BITVAL(5)
#define VDP_MODESET4_SPAEN     SAI_BITVAL(4)
#define VDP_MODESET4_SHI       SAI_BITVAL(3)
#define VDP_MODESET4_LSM1      SAI_BITVAL(2)
#define VDP_MODESET4_LSM0      SAI_BITVAL(1)
#define VDP_MODESET4_RS0       SAI_BITVAL(0)
#if SAI_TARGET != SAI_TARGET_MD
#define VDP_MODESET4_DEFAULT   (VDP_MODESET4_RS0 | VDP_MODESET4_SPAEN | VDP_MODESET4_VSCLK)
#else
#define VDP_MODESET4_DEFAULT   (VDP_MODESET4_RS0 | VDP_MODESET4_RS1)
#endif  // SAI_TARGET

//
// Macros and Aliases
//

// Memory-mapped IO
#define VDP_OFFS_DATA          (0x00)
#define VDP_OFFS_CTRL          (0x04)
#define VDP_OFFS_STATUS        (0x04)
#define VDP_OFFS_HVCOUNT       (0x08)
#define VDP_OFFS_DBG_SEL       (0x18)
#define VDP_OFFS_DBG_DATA      (0x1C)

#define VDP_DATA               (VDP_BASE+VDP_OFFS_DATA)
#define VDP_CTRL               (VDP_BASE+VDP_OFFS_CTRL)
#define VDP_STATUS             (VDP_BASE+VDP_OFFS_STATUS)
#define VDP_HVCOUNT            (VDP_BASE+VDP_OFFS_HVCOUNT)
#define VDP_DBG_SEL            (VDP_BASE+VDP_OFFS_DBG_SEL)
#define VDP_DBG_DATA           (VDP_BASE+VDP_OFFS_DBG_DATA)

// Status register
#define VDP_STATUS_PAL         SAI_BITVAL(0)
#define VDP_STATUS_DMA         SAI_BITVAL(1)
#define VDP_STATUS_HB          SAI_BITVAL(2)
#define VDP_STATUS_VB          SAI_BITVAL(3)
#define VDP_STATUS_OD          SAI_BITVAL(4)
#define VDP_STATUS_SC          SAI_BITVAL(5)
#define VDP_STATUS_SO          SAI_BITVAL(6)
#define VDP_STATUS_VI          SAI_BITVAL(7)
#define VDP_STATUS_FIFO_FULL   SAI_BITVAL(8)
#define VDP_STATUS_FIFO_EMPTY  SAI_BITVAL(9)

// Plane size
#define VDP_PLANESIZE_32x32    0x00
#define VDP_PLANESIZE_64x32    0x01
#define VDP_PLANESIZE_128x32   0x03
#define VDP_PLANESIZE_32x64    0x10
#define VDP_PLANESIZE_64x64    0x11
#define VDP_PLANESIZE_32x128   0x30

#define VDP_PLANESIZE_DEFAULT  VDP_PLANESIZE_64x32

// Table address shift and mask values
#define VDP_SCRA_SHIFT         (13)
#define VDP_SCRA_MASK          (0xE000)
#define VDP_SCRW_SHIFT         (11)
#define VDP_SCRB_MASK          (0xE000)
#define VDP_SCRB_SHIFT         (13)
#define VDP_SCRW_MASK          (0xF000)
#define VDP_SCRW_MASK_H32      (0xF800)
#define VDP_SPR_SHIFT          (9)
#define VDP_SPR_MASK           (0xFC00)
#define VDP_SPR_MASK_H32       (0xFE00)
#define VDP_HSCR_SHIFT         (10)
#define VDP_HSCR_MASK          (0xFC00)

// Scroll mode bits
#define VDP_HSCROLL_PLANE 0
#define VDP_HSCROLL_UNDEF (VDP_MODESET3_HS0)
#define VDP_HSCROLL_CELL (VDP_MODESET3_HS1)
#define VDP_HSCROLL_LINE (VDP_MODESET3_HS0 | VDP_MODESET3_HS1)

#define VDP_VSCROLL_PLANE 0
#define VDP_VSCROLL_CELL (VDP_MODESET3_VCELL)

#define VDP_HSCROLL_DEFAULT 0
#define VDP_VSCROLL_DEFAULT 0

// Tile attributes
#define VDP_PRIO 0x8000
#define VDP_HF 0x0800
#define VDP_VF 0x1000
#define VDP_PALSHIFT 13

// Attributes and sprite props
#define VDP_ATTR(_tile, _hflip, _vflip, _pal, _prio) (((_tile) & 0x7FF) | \
                 (((_hflip) ? VDP_HF : 0) | ((_vflip) ? VDP_VF : 0) | \
                  (((_pal) & 0x3) << VDP_PALSHIFT) | ((_prio) ? VDP_PRIO : 0)))
#define VDP_SIZE(w, h) (((h-1) & 0x3) | (((w-1) & 0x3) << 2))
#define VDP_SPR_COUNT 80

#define VDP_TILE(address) ((address)>>5)
#define VDP_ADDR_FROM_TILE(tile) ((tile)<<5)

// Register manipulation.
#define VDP_VRAM_ADDR_CMD      (0x40000000)
#define VDP_CRAM_ADDR_CMD      (0xC0000000)
#define VDP_VSRAM_ADDR_CMD     (0x40000010)
#ifndef __ASSEMBLER__
#define VDP_CTRL_ADDR(_addr) ((((uint32_t)(_addr) & 0x3FFF) << 16) | \
                             (((uint32_t)(_addr) & 0xC000) >> 14))
#else
#define VDP_CTRL_ADDR(_addr) ((((_addr) & 0x3FFF) << 16) | (((_addr) & 0xC000) >> 14))
#endif  // __ASSEMBLER__
#define VDP_REGST(regno, value) ((((regno) << 8) | 0x8000) | ((value) & 0xFF))

// Indices into NT array and/or args into layer functions
#define VDP_PLANE_A            0
#define VDP_PLANE_B            1
#define VDP_PLANE_WINDOW       2

// Resolution Settings
#define VDP_INTERLACE_NONE     0
#define VDP_INTERLACE_NORMAL   (VDP_MODESET4_LSM0)
#define VDP_INTERLACE_DOUBLE   (VDP_MODESET4_LSM0 | VDP_MODESET4_LSM1)

/*
-----------------------------------------------------------------------------
Debug registers. Selected with VDPPORT_DBG_SEL, written with VDPPORT_DBG_DATA
Unlike regular VDP registers, these are word-sized (16-bit).
-----------------------------------------------------------------------------

Register $00 - PSG debug and layer mixing
.sss .... .... .... SPRST - Sprite render state bits. Function unknown.
.... cc.. .... .... PSOCN - PSG Override attenuation source channel.
.... ..p. .... .... PSOVR - PSG Override - has all PSG channels share volume.
.... ...l l... .... LYSEL - Select layer to display with SOLO.
.... .... .S.. .... SOLO  - Hide all but one layer, selected by LYSEL.
.... .... ..j. .... JUNK5 - Seems to corrupt VRAM address access. See notes.
.... .... .... k... KILL1 - Seems to halt the game, or interrupts.
.... .... .... ..j. JUNK1 - Halts, fills the screen with a junk pattern.

LYSEL should be 00 in normal circumstances, but if it is used without setting
SOLO, the VDP will still try to display the selected layer, which cases the
layer data to have a bus conflict with the output from the normal rendering
pipeline. The end result is usually a logical AND between the color indices,
but as this relies on analog behavior of a bus conflict, it's not stable, and
is not recommended. It may even be bad for the VDP!

At the moment this register is written, the remainder of the output line
buffer may be filled with a repeating junk pattern. It may be a scroll table.

JUNK5 is interesting. It has the following effects that I can observe:
* The screen is filled with garbage data
* Sprites lose their horizontal position, adopting arbitrary values
* VRAM is corrupted in a predictable pattern, from my testing in H40:
  Tile $0000, $0020, $0041, $0061, $0082, $00A2... so on and so forth
  Within each tile, bytes are corrupt in this order:
  Every even tile in the pattern: $01, $04, $0B, $0E
  Every odd tile in the pattern: $11, $14, $1B, $1E

  Going by VRAM addresses (addressing byte-wise):
  $0001, $0004, $000B, $000E, $0411, $0414, $041B, $041E, $0801, $0804... etc

  The value of the junk data written is not yet clear.

  If I enable this register during active display, and then disable it
  shortly after (~a few scanlines' duration), it does not appear to corrupt
  VRAM, at least not near $0000 where it is plainly obvious.
*/

#define VDP_DBG00_JUNK0   0x0001
#define VDP_DBG00_DMAST   0x0002
#define VDP_DBG00_SOLO    0x0040
#define VDP_DBG00_LYSEL0  0x0080
#define VDP_DBG00_LYSEL1  0x0100
#define VDP_DBG00_PSOVR   0x0200
#define VDP_DBG00_PSOCN0  0x0400
#define VDP_DBG00_PSOCN1  0x0800
#define VDP_DBG00_SPRST0  0x1000
#define VDP_DBG00_SPRST1  0x2000
#define VDP_DBG00_SPRST2  0x4000

/*
Register $01 - Clock and counter functions
u... .... .... .... UNKF
.u.. .... .... .... UNKE
..u. .... .... .... UNKD
...u .... .... .... UNKC
.... u... .... .... UNKB
.... .s.. .... .... SCBAD - Modifies scroll plane B's pixel data addressing.
.... ..s. .... .... SCAAD - Modifies scroll plane A's pixel data addressing.
.... ...t .... .... TILEF - Changes tile fetch addressing.
.... .... h... .... HSCRL - Makes Hscroll table read from address 0 (?)
.... .... .jjj .... JUNK  - Causes a moving pattern of junk.
.... .... .... u... UNK3  - Seems to lock up.
.... .... .... .v.. VCTST - Makes V counter increment very pixel. Not useful.
.... .... .... ..e. EDCKO - Makes EDCLK output DCLK. Not useful on MD.
.... .... .... ...z Z80CK - Doubles the Z80 and PSG clock to about 7.67Mhz!
*/
#define VDP_DBG01_Z80CK   0x0001
#define VDP_DBG01_EDCKO   0x0002
#define VDP_DBG01_VCTST   0x0004
#define VDP_DBG01_JUNK0   0x0010
#define VDP_DBG01_JUNK1   0x0020
#define VDP_DBG01_JUNK2   0x0040
#define VDP_DBG01_HSCRL   0x0080
#define VDP_DBG01_UNKF    0x8000
#define VDP_DBG01_UNKE    0x4000
#define VDP_DBG01_UNKD    0x2000
#define VDP_DBG01_UNKC    0x1000
#define VDP_DBG01_UNKB    0x0800
#define VDP_DBG01_UNKA    0x0400
#define VDP_DBG01_UNK9    0x0200
#define VDP_DBG01_UNK8    0x0100

/*
SCBAA/SCAAD notes:
Observed effects:
* Scroll plane repeatedly fetches (or displays) four pixels out of a 16
  pixel length of tile data.
* The other plane may fetch pixel data for what should be horizontally
  adjacent tiles.
* The other plane's pixel data seems to find its way into the plane data.
* The behavior of the above is dependent on the lower four bits of the
  horizontal scroll.
* Depending on how far the horizontal scroll is set, data from the sprite
  pixel fetch may appear to the right in what is normally blanked. This data
  is presented as if it is part of the scroll plane's fetched pixel data, so
  it assumes the attributes described by the scroll plane nametable.

TILEF notes:
* Every other column seems to have its tile layout data offset, so that the
  plane looks as though it's been coarsely interlaced vertically.
* In some cases, completely wrong tiles are fetched
* Data lingering from sprite pixel fetches sometimes appears in tiles

HSCRL notes:
* I think there is more to this than the H scroll table being redirected.
* Some tiles horizontally wobble within their boundaries unreliably.
* Sprite fetches affect the strange corrupted horizontal value.

Writing to this register frequently seems to increase the likelihood of a
crash or freeze, be it through corrupting the raster counters or halting the
Z80 (not yet investigated in detail).

Register $02 - Unknown
Reading from this register returns a walking bit pattern in the lower byte.

Register $03 - Unknown
Writing to this mid-screen screws up horizontal sync temporarily.

Register $04 - Unknown

Register $05 - Unknown
Writing to this mid-screen has a subtle effect on the horizontal scroll value
used for Plane B

Register $06 - Unknown
Writing to this mid-screen causes a small streak of junk pixel data to appear
that seems to come from sprite pixel data.

Register $07 - Unknown
Similar effect to reg $06.

Register $08 - Unknown
Very subtle corruption of sprite pixel data at the instant it is written.
*/

// Debug bits
#define VDP_DEBUG_LYSEL_NONE    0x00
#define VDP_DEBUG_LYSEL_SPRITE  0x01
#define VDP_DEBUG_LYSEL_PLANE_A 0x02
#define VDP_DEBUG_LYSEL_PLANE_B 0x03
