// Hardware definitions for Capcom CPS1/2.
#pragma once

#define ROM_BASE               (0x000000)
#define WRAM_BASE              (0xFF0000)
#define WRAM_SIZE              (0x10000)
#define VRAM_BASE              (0x900000)
#define VRAM_SIZE              (0x30000)

#define SAI_CPSA_REG_BASE      (0x804100)
#define SAI_CPSB_REG_BASE      (0x804140)
#define SAI_CPS_IO_BASE        (0x800000)

// VRAM layout
#define VRAM_OBJRAM            (VRAM_BASE+0x18000)
#define VRAM_SCROLL1           (VRAM_BASE+0x0C000)
#define VRAM_SCROLL2           (VRAM_BASE+0x04000)
#define VRAM_SCROLL3           (VRAM_BASE+0x08000)
#define VRAM_ROWSCROLL         (VRAM_BASE+0x02000)
#define VRAM_PALETTE           (VRAM_BASE+0x00000)

#define SAI_CPS_SCROLL_X_DEFAULT -64
#define SAI_CPS_SCROLL_Y_DEFAULT -16
