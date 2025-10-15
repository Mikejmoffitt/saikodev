//
// Saikodev Neo-Geo FIX layer support.
//

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__

#include "sai/neogeo/vram.h"
#include "sai/neogeo/reg.h"

#define FIX_ATTR(pal) ((pal)<<12)
#define FIX_OFFS(x, y) (y + (x*0x20))
#define FIX_ADDR(x, y) (VRAM_FIXMAP+FIX_OFFS(x, y))

#ifndef __ASSEMBLER__

void sai_neo_fix_init(void);

static inline void sai_neo_fix_select(bool cartridge);

// --------------------------------------------------

static inline void sai_neo_fix_select(bool cartridge)
{
	if (cartridge) *(volatile uint8_t *)(SAI_NEO_REG_CRTFIX) = 0;
	else *(volatile uint8_t *)(SAI_NEO_REG_BRDFIX) = 0;
}

#else

	.extern	sai_neo_fix_init

#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
