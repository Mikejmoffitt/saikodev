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
#include "sai/md/vdp_regs.h"

#define VDP_VRAM_SIZE          (0x010000)
#define VDP_VRAM_SIZE_128K     (0x020000)

#ifndef __ASSEMBLER__

//
// Memory
//

// Registers data for MODE1-MODE4. The command is baked in as well.
extern uint16_t g_sai_vdp_reg_mode[4];  // $80, $81, $8B, $8C modeset bits.
// VRAM locations for tables (name, sprite, scroll). 32-bit addresses are used
// for compatibility with 128K VRAM.
extern uint32_t g_sai_vdp_ntbase[3];     // Nametable base addresses (A, B, W).
extern uint32_t g_sai_vdp_sprbase;       // Sprite base address.
extern uint32_t g_sai_vdp_hsbase;        // Horizontal scroll address.
// The current plane size enum. Used for VRAM offset calculations.
extern uint16_t g_sai_vdp_planesize;

#else
	.extern	g_sai_vdp_reg_mode
	.extern	g_sai_vdp_ntbase
	.extern	g_sai_vdp_sprbase
	.extern	g_sai_vdp_hsbase
	.extern	g_sai_vdp_planesize

#endif  // __ASSEMBLER__

#ifndef __ASSEMBLER__

//
// Interface
//

void sai_vdp_init(void);
void sai_vdp_clear_vram(void);  // Zeroes out VRAM.

// Register and misc functions
void sai_vdp_update_scroll_regs(void);  // Call after setting the *base vars.
static inline void sai_vdp_set_reg(uint8_t reg, uint8_t val);
static inline uint16_t sai_vdp_get_status(void);
static inline void sai_vdp_wait_dma(void);

// VRAM address control.
static inline void sai_vdp_set_addr_vramw(uint32_t addr);
static inline void sai_vdp_set_addr_cramw(uint16_t addr);
static inline void sai_vdp_set_addr_vsramw(uint16_t addr);
static inline void sai_vdp_write_word(uint16_t data);
static inline void sai_vdp_set_autoinc(uint8_t bytes);

// Table address config.
static inline void sai_vdp_set_plane_base(uint16_t plane, uint32_t addr);
static inline void sai_vdp_set_spr_base(uint32_t addr);
static inline void sai_vdp_set_hscroll_base(uint32_t addr);
static inline uint32_t sai_vdp_get_plane_base(uint16_t plane);
static inline uint32_t sai_vdp_get_sprite_base(void);
static inline uint32_t sai_vdp_get_hscroll_base(void);
static inline uint16_t sai_vdp_get_plane_w(void);  // In tiles.
static inline uint16_t sai_vdp_get_plane_h(void);  // "
static inline uint32_t sai_vdp_calc_plane_addr(uint16_t plane, uint16_t x, uint16_t y);

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
static inline void sai_vdp_set_blank(bool blank);
static inline void sai_vdp_set_bg_color(uint8_t idx);
static inline uint16_t sai_vdp_get_hvcount(void);
static inline uint16_t sai_vdp_get_hcount(void);
static inline uint16_t sai_vdp_get_vcount(void);
static inline void sai_vdp_set_shadow_highlight(bool enabled);
static inline void sai_vdp_set_left_column_blank(bool enabled);
static inline void sai_vdp_set_interlace_mode(uint8_t mode);
static inline void sai_vdp_set_width(bool h40);

// Misc. configuration options
static inline void sai_vdp_set_128k_vram_mode(bool enabled);
static inline void sai_vdp_set_vs_clk_output(bool enabled);
static inline void sai_vdp_set_spa_output(bool enabled);
static inline void sai_vdp_set_cbus_cpu_mux(bool enabled);
static inline void sai_vdp_set_hs_input(bool enabled);
static inline void sai_vdp_set_sms_vl(bool enabled);
static inline void sai_vdp_set_sms_hl(bool enabled);

// Scroll planes
static inline void sai_vdp_set_plane_size(uint8_t size);
static inline void sai_vdp_set_hscroll_mode(uint8_t mode);
static inline void sai_vdp_set_vscroll_mode(uint8_t mode);

// Enable the window plane, and have it draw from the column/line specified with
// `cell`. Call any of these with 0 to disable the horizontal or vertical window.
static inline void sai_vdp_set_window_top(uint8_t cell);
static inline void sai_vdp_set_window_bottom(uint8_t cell);
static inline void sai_vdp_set_window_right(uint8_t width);
static inline void sai_vdp_set_window_left(uint8_t width);

// -----------------------------------------------------------------------------
//
// Static implementations
//
// -----------------------------------------------------------------------------

static inline void sai_vdp_write_ctrl32(uint32_t val)
{
	volatile uint32_t *port_ctrl32 = (volatile uint32_t *)(VDP_CTRL);
	*port_ctrl32 = val;
	SAI_BARRIER();
}

static inline void sai_vdp_write_ctrl(uint16_t val)
{
	volatile uint16_t *port_ctrl = (volatile uint16_t *)(VDP_CTRL);
	*port_ctrl = val;
	SAI_BARRIER();
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

// VRAM address control.
static inline void sai_vdp_set_addr_vramw(uint32_t addr)
{
	sai_vdp_write_ctrl32(VDP_CTRL_ADDR(addr) | VDP_VRAM_ADDR_CMD);
}

static inline void sai_vdp_set_addr_cramw(uint16_t addr)
{
	sai_vdp_write_ctrl32(VDP_CTRL_ADDR(addr) | VDP_CRAM_ADDR_CMD);
}

static inline void sai_vdp_set_addr_vsramw(uint16_t addr)
{
	sai_vdp_write_ctrl32(VDP_CTRL_ADDR(addr) | VDP_VSRAM_ADDR_CMD);
}

static inline void sai_vdp_write_word(uint16_t data)
{
	volatile uint16_t *port_data = (volatile uint16_t *)(VDP_DATA);
	*port_data = data;
	SAI_BARRIER();
}

static inline void sai_vdp_set_autoinc(uint8_t bytes)
{
	sai_vdp_set_reg(VDP_AUTOINC, bytes);
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

static inline uint32_t sai_vdp_get_plane_base(uint16_t plane)
{
	return g_sai_vdp_ntbase[plane];
}

static inline uint32_t sai_vdp_get_sprite_base(void)
{
	return g_sai_vdp_sprbase;
}

static inline uint32_t sai_vdp_get_hscroll_base(void)
{
	return g_sai_vdp_hsbase;
}

static inline uint16_t sai_vdp_get_plane_w(void)
{
	switch (g_sai_vdp_planesize)
	{
		case VDP_PLANESIZE_32x32:
		case VDP_PLANESIZE_32x64:
		case VDP_PLANESIZE_32x128:
			return 32;
		case VDP_PLANESIZE_64x32:
		case VDP_PLANESIZE_64x64:
			return 64;
		case VDP_PLANESIZE_128x32:
			return 128;
		default:
			return 0;
	}
}

static inline uint16_t sai_vdp_get_plane_h(void)
{
	switch (g_sai_vdp_planesize)
	{
		case VDP_PLANESIZE_32x32:
		case VDP_PLANESIZE_64x32:
		case VDP_PLANESIZE_128x32:
			return 32;
		case VDP_PLANESIZE_32x64:
		case VDP_PLANESIZE_64x64:
			return 64;
		case VDP_PLANESIZE_32x128:
			return 128;
		default:
			return 0;
	}
}

static inline uint32_t sai_vdp_calc_plane_addr(uint16_t plane, uint16_t x, uint16_t y)
{
	const uint32_t base = g_sai_vdp_ntbase[plane];
	return base + (x*2) + (y*2*sai_vdp_get_plane_w());
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
	g_sai_vdp_planesize = size;
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
static inline void sai_vdp_set_blank(bool blank)
{
	if (blank) g_sai_vdp_reg_mode[0] |= VDP_MODESET1_DE;
	else g_sai_vdp_reg_mode[0] &= ~(VDP_MODESET1_DE);
	sai_vdp_write_ctrl(g_sai_vdp_reg_mode[0]);
}

static inline void sai_vdp_set_bg_color(uint8_t idx)
{
	sai_vdp_set_reg(VDP_BGCOL, idx);
}

static inline uint16_t sai_vdp_get_hvcount(void)
{
	volatile uint16_t *port_hv = (volatile uint16_t *)(VDP_HVCOUNT);
	return *port_hv;
}

static inline uint16_t sai_vdp_get_hcount(void)
{
	return sai_vdp_get_hvcount() & 0x00FF;
}

static inline uint16_t sai_vdp_get_vcount(void)
{
	return sai_vdp_get_hvcount() >> 8;
}

static inline void sai_vdp_set_shadow_highlight(bool enabled)
{
	if (enabled) g_sai_vdp_reg_mode[3] |= VDP_MODESET4_SHI;
	else g_sai_vdp_reg_mode[3] &= ~(VDP_MODESET4_SHI);
	sai_vdp_write_ctrl(g_sai_vdp_reg_mode[3]);
}

static inline void sai_vdp_set_left_column_blank(bool enabled)
{
	if (enabled) g_sai_vdp_reg_mode[0] |= VDP_MODESET1_LCB;
	else g_sai_vdp_reg_mode[0] &= ~(VDP_MODESET1_LCB);
	sai_vdp_write_ctrl(g_sai_vdp_reg_mode[0]);
}

static inline void sai_vdp_set_interlace_mode(uint8_t mode)
{
	g_sai_vdp_reg_mode[3] &= ~(VDP_MODESET4_LSM1 | VDP_MODESET4_LSM0);
	g_sai_vdp_reg_mode[3] |= mode;
	sai_vdp_write_ctrl(g_sai_vdp_reg_mode[3]);
}

static inline void sai_vdp_set_width(bool h40)
{
	if (h40)
	{
		g_sai_vdp_reg_mode[3] |= (VDP_MODESET4_RS0 | VDP_MODESET4_RS1);
	}
	else
	{
		g_sai_vdp_reg_mode[3] &= ~(VDP_MODESET4_RS0 | VDP_MODESET4_RS1);
	}
	sai_vdp_write_ctrl(g_sai_vdp_reg_mode[3]);
}

static inline void sai_vdp_set_128k_vram_mode(bool enabled)
{
	if (enabled) g_sai_vdp_reg_mode[1] |= VDP_MODESET2_VR128;
	else g_sai_vdp_reg_mode[1] &= ~(VDP_MODESET2_VR128);
	sai_vdp_write_ctrl(g_sai_vdp_reg_mode[1]);
}

static inline void sai_vdp_set_vs_clk_output(bool enabled)
{
	if (enabled) g_sai_vdp_reg_mode[3] |= VDP_MODESET4_VSCLK;
	else g_sai_vdp_reg_mode[3] &= ~(VDP_MODESET4_VSCLK);
	sai_vdp_write_ctrl(g_sai_vdp_reg_mode[3]);
}

static inline void sai_vdp_set_spa_output(bool enabled)
{
	if (enabled) g_sai_vdp_reg_mode[3] |= VDP_MODESET4_SPAEN;
	else g_sai_vdp_reg_mode[3] &= ~(VDP_MODESET4_SPAEN);
	sai_vdp_write_ctrl(g_sai_vdp_reg_mode[3]);
}

static inline void sai_vdp_set_cbus_cpu_mux(bool enabled)
{
	if (enabled) g_sai_vdp_reg_mode[2] |= VDP_MODESET3_ADMUX;
	else g_sai_vdp_reg_mode[2] &= ~(VDP_MODESET3_ADMUX);
	sai_vdp_write_ctrl(g_sai_vdp_reg_mode[2]);
}

static inline void sai_vdp_set_hs_input(bool enabled)
{
	if (enabled) g_sai_vdp_reg_mode[3] |= VDP_MODESET4_HSCIN;
	else g_sai_vdp_reg_mode[3] &= ~(VDP_MODESET4_HSCIN);
	sai_vdp_write_ctrl(g_sai_vdp_reg_mode[3]);
}

static inline void sai_vdp_set_sms_vl(bool enabled)
{
	if (enabled) g_sai_vdp_reg_mode[0] |= VDP_MODESET1_SMSVL;
	else g_sai_vdp_reg_mode[0] &= ~(VDP_MODESET1_SMSVL);
	sai_vdp_write_ctrl(g_sai_vdp_reg_mode[0]);
}

static inline void sai_vdp_set_sms_hl(bool enabled)
{
	if (enabled) g_sai_vdp_reg_mode[0] |= VDP_MODESET1_SMSHL;
	else g_sai_vdp_reg_mode[0] &= ~(VDP_MODESET1_SMSHL);
	sai_vdp_write_ctrl(g_sai_vdp_reg_mode[0]);
}

#endif

#ifdef __cplusplus
}
#endif  // __cplusplus
