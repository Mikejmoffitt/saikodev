// Saikodev Sega 315-5313 / YM7101
// Michael Moffitt 2018-2025
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdbool.h>
#include <stdint.h>
#endif  // __ASSEMBLER__

#include "sai/target.h"
#include "sai/macro.h"
#include "sai/memmap.h"

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
#define VDP_MODESET2_DEFAULT   (VDP_MODESET2_M5 | VDP_MODESET2_M1 | VDP_MODESET2_IE0)
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
#if SAI_TARGET == SAI_TARGET_C2
#define VDP_MODESET4_DEFAULT   (VDP_MODESET4_RS0 | VDP_MODESET4_SPAEN | VDP_MODESET4_VSCLK)
#else
#define VDP_MODESET4_DEFAULT   (VDP_MODESET4_RS0 | VDP_MODESET4_RS1)
#endif  // SAI_TARGET_C2

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
#define VDP_PLANESIZE_UNDx32   0x02
#define VDP_PLANESIZE_128x32   0x03

#define VDP_PLANESIZE_32x64    0x10
#define VDP_PLANESIZE_64x64    0x11
#define VDP_PLANESIZE_UNDx64   0x12
#define VDP_PLANESIZE_128x64   0x13

#define VDP_PLANESIZE_32xUND   0x20
#define VDP_PLANESIZE_64xUND   0x21
#define VDP_PLANESIZE_UNDxUND  0x22
#define VDP_PLANESIZE_128xUND  0x23

#define VDP_PLANESIZE_32x128   0x30
#define VDP_PLANESIZE_64x128   0x31
#define VDP_PLANESIZE_UNDx128  0x32
#define VDP_PLANESIZE_128x128  0x33

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

// Tile attributes
#define VDP_PRIO 0x8000
#define VDP_HF 0x0800
#define VDP_VF 0x1000
#define VDP_PALSHIFT 13

#define VDP_ATTR(_tile, _hflip, _vflip, _pal, _prio) (((_tile) & 0x7FF) | \
                 (((_hflip) ? VDP_HF : 0) | ((_vflip) ? VDP_VF : 0) | \
                  (((_pal) & 0x3) << VDP_PALSHIFT) | ((_prio) ? VDP_PRIO : 0)))

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
#define VDP_REGST(regno, value) (((regno << 8) | 0x8000) | (value & 0xFF))

// Indices into NT array and/or args into layer functions
#define VDP_PLANE_A            0
#define VDP_PLANE_B            1
#define VDP_PLANE_WINDOW       2

// Resolution Settings
#define VDP_INTERLACE_NONE     0
#define VDP_INTERLACE_NORMAL   (VDP_MODESET4_LSM0)
#define VDP_INTERLACE_DOUBLE   (VDP_MODESET4_LSM0 | VDP_MODESET4_LSM1)


#ifndef __ASSEMBLER__

//
// Memory
//

// VRAM locations for tables (name, sprite, scroll).
extern uint32_t g_sai_vdp_ntbase[3];     // Nametable base addresses (A, B, W).
extern uint32_t g_sai_vdp_sprbase;       // Sprite base address.
extern uint32_t g_sai_vdp_hsbase;        // Horizontal scroll address.

// Registers bits. The command is baked in as well.
extern uint16_t g_sai_vdp_reg_mode[4];  // $80, $81, $8B, $8C modeset bits.

// Vertical blank synchronization flag
extern uint8_t g_vbl_wait_flag;
#else
	.extern	g_sai_vdp_ntbase
	.extern	g_sai_vdp_sprbase
	.extern	g_sai_vdp_hsbase

	.extern	g_sai_vdp_reg_mode

	.extern	g_vbl_wait_flag
#endif  // __ASSEMBLER__

#ifndef __ASSEMBLER__
//
// Initialization and system functions
//
void sai_vdp_clear_vram(void);  // Zeroes out VRAM.
void sai_vdp_wait_vbl(void);    // Waits until vertical IRQ is hit.

//
// Registers
//
void sai_vdp_update_scroll_regs(void);  // Call after setting the *base vars.
static inline void sai_vdp_set_reg(uint8_t reg, uint8_t val);
static inline uint16_t sai_vdp_get_status(void);
static inline void sai_vdp_wait_dma(void);

// Table address config.
static inline void sai_vdp_set_plane_base(uint16_t plane, uint32_t addr);
static inline void sai_vdp_set_spr_base(uint32_t addr);
static inline void sai_vdp_set_hscroll_base(uint32_t addr);
static inline uint32_t md_vdp_get_plane_base(uint16_t plane);
static inline uint32_t md_vdp_get_sprite_base(void);
static inline uint32_t md_vdp_get_hscroll_base(void);

// Interrupts. Returns the prior enablement status.
static inline bool sai_vdp_set_vint_en(bool enabled);
static inline bool sai_vdp_set_thint_en(bool enabled);
static inline bool sai_vdp_set_hint_en(bool enabled);
static inline void sai_vdp_set_hint_line(uint8_t line);

// Scroll planes.
static inline void sai_vdp_set_plane_size(uint8_t size);  // VDP_PLANESIZE
static inline void sai_vdp_set_hscroll_mode(uint8_t mode);  // VDP_HSCROLL
static inline void sai_vdp_set_vscroll_mode(uint8_t mode);  // VDP_VSCROLL
static inline uint8_t sai_vdp_get_hscroll_mode(void);
static inline uint8_t sai_vdp_get_vscroll_mode(void);

// Window config.
static inline void sai_vdp_set_window_top(uint8_t cells);
static inline void sai_vdp_set_window_bottom(uint8_t cells);
static inline void sai_vdp_set_window_right(uint8_t cells);
static inline void sai_vdp_set_window_left(uint8_t cells);

// Raster
static inline void md_vdp_set_blank(bool blank);
static inline uint16_t md_vdp_get_hvcount(void);
static inline uint16_t md_vdp_get_hcount(void);
static inline uint16_t md_vdp_get_vcount(void);

// Scroll planes
static inline void md_vdp_set_plane_size(uint8_t size);
static inline void md_vdp_set_hscroll_mode(uint8_t mode);
static inline void md_vdp_set_vscroll_mode(uint8_t mode);

// Get the current plane dimensions in cells (pixels / 8).
static inline uint16_t md_vdp_get_plane_width(void);
static inline uint16_t md_vdp_get_plane_height(void);

// Enable the window plane, and have it draw from the column/line specified with
// `cell`. Call any of these with 0 to disable the horizontal or vertical window.
static inline void md_vdp_set_window_top(uint8_t cell);
static inline void md_vdp_set_window_bottom(uint8_t cell);
static inline void md_vdp_set_window_right(uint8_t width);
static inline void md_vdp_set_window_left(uint8_t width);

// -----------------------------------------------------------------------------
//
// Static implementations
//
// -----------------------------------------------------------------------------

static inline void sai_vdp_write_ctrl(uint16_t val)
{
	volatile uint16_t *port_ctrl = (volatile uint16_t *)(VDP_CTRL);
	*port_ctrl = val;
}

static inline void sai_vdp_set_reg(uint8_t reg, uint8_t val)
{
	uint16_t write_val = 0x8000 | (reg << 8);
	write_val |= val;
	sai_vdp_write_ctrl(write_val);
}

static inline uint16_t sai_vdp_get_status(void)
{
	volatile uint16_t *port_status = (volatile uint16_t *)(VDP_STATUS);
	return *port_status;
}

static inline void sai_vdp_wait_dma(void)
{
	while (sai_vdp_get_status() & VDP_STATUS_DMA)
	{
		__asm__ volatile ("\tnop\n");
	}
}

// Bases and Planes
static inline void sai_vdp_set_plane_base(uint16_t plane, uint32_t addr)
{
	g_sai_vdp_ntbase[plane] = addr;
}

static inline void sai_vdp_set_spr_base(uint32_t addr)
{
	g_sai_vdp_sprbase = addr;
}

static inline void sai_vdp_set_hscroll_base(uint32_t addr)
{
	g_sai_vdp_hsbase = addr;
}

static inline uint32_t md_vdp_get_plane_base(uint16_t plane)
{
	return g_sai_vdp_ntbase[plane];
}

static inline uint32_t md_vdp_get_sprite_base(void)
{
	return g_sai_vdp_sprbase;
}

static inline uint32_t md_vdp_get_hscroll_base(void)
{
	return g_sai_vdp_hsbase;
}

// Interrupt config
static inline bool sai_vdp_set_hint_en(bool enabled)
{
	const bool ret = g_sai_vdp_reg_mode[0] & VDP_MODESET1_IE1;
	if (enabled) g_sai_vdp_reg_mode[0] |= VDP_MODESET1_IE1;
	else g_sai_vdp_reg_mode[0] &= ~(VDP_MODESET1_IE1);
	sai_vdp_write_ctrl(g_sai_vdp_reg_mode[0]);
	return ret;
}

static inline bool sai_vdp_set_vint_en(bool enabled)
{
	const bool ret = g_sai_vdp_reg_mode[1] & VDP_MODESET2_IE0;
	if (enabled) g_sai_vdp_reg_mode[1] |= VDP_MODESET2_IE0;
	else g_sai_vdp_reg_mode[1] &= ~(VDP_MODESET2_IE0);
	sai_vdp_write_ctrl(g_sai_vdp_reg_mode[1]);
	return ret;
}

static inline bool sai_vdp_set_thint_en(bool enabled)
{
	const bool ret = g_sai_vdp_reg_mode[2] & VDP_MODESET3_IE2;
	if (enabled) g_sai_vdp_reg_mode[2] |= VDP_MODESET3_IE2;
	else g_sai_vdp_reg_mode[2] &= ~(VDP_MODESET3_IE2);
	sai_vdp_write_ctrl(g_sai_vdp_reg_mode[2]);
	return ret;
}

static inline void sai_vdp_set_hint_line(uint8_t line)
{
	sai_vdp_set_reg(VDP_HINTC, line);
}

// Scroll
static inline void sai_vdp_set_hscroll_mode(uint8_t mode)
{
	g_sai_vdp_reg_mode[2] &= 0xFC;
	g_sai_vdp_reg_mode[2] |= mode;
	sai_vdp_write_ctrl(g_sai_vdp_reg_mode[2]);
}

static inline void sai_vdp_set_vscroll_mode(uint8_t mode)
{
	if (mode) g_sai_vdp_reg_mode[2] |= VDP_MODESET3_VCELL;
	else g_sai_vdp_reg_mode[2] &= ~(VDP_MODESET3_VCELL);
	sai_vdp_write_ctrl(g_sai_vdp_reg_mode[2]);
}

static inline void sai_vdp_set_plane_size(uint8_t size)
{
	sai_vdp_set_reg(VDP_PLANESIZE, size);
}

static inline uint8_t sai_vdp_get_hscroll_mode(void)
{
	return g_sai_vdp_reg_mode[2] & 0x03;
}

static inline uint8_t sai_vdp_get_vscroll_mode(void)
{
	return g_sai_vdp_reg_mode[2] & VDP_MODESET3_VCELL;
}

// Window config
static inline void sai_vdp_set_window_top(uint8_t cell)
{
	sai_vdp_set_reg(VDP_WINVERT, cell & 0x1F);
}

static inline void sai_vdp_set_window_bottom(uint8_t cell)
{
	sai_vdp_set_reg(VDP_WINVERT, 0x80 | (cell & 0x1F));
}

static inline void sai_vdp_set_window_right(uint8_t cell)
{
	sai_vdp_set_reg(VDP_WINHORI, 0x80 | (cell & 0x1F));
}

static inline void sai_vdp_set_window_left(uint8_t cell)
{
	sai_vdp_set_reg(VDP_WINHORI, cell & 0x1F);
}

// Raster
static inline void md_vdp_set_blank(bool blank)
{
	if (blank) g_sai_vdp_reg_mode[0] |= VDP_MODESET1_DE;
	else g_sai_vdp_reg_mode[0] &= ~(VDP_MODESET1_DE);
	sai_vdp_write_ctrl(g_sai_vdp_reg_mode[0]);
}

static inline uint16_t md_vdp_get_hvcount(void)
{
	volatile uint16_t *port_hv = (volatile uint16_t *)(VDP_HVCOUNT);
	return *port_hv;
}

static inline uint16_t md_vdp_get_hcount(void)
{
	return md_vdp_get_hvcount() & 0x00FF;
}

static inline uint16_t md_vdp_get_vcount(void)
{
	return md_vdp_get_hvcount() >> 8;
}
#endif

#ifdef __cplusplus
}
#endif  // __cplusplus
