//
// Saikodev Neo-Geo FIX layer support.
//

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#include "sai/neogeo/vram.h"

#define FIX_ATTR(pal) ((pal)<<12)
#define FIX_OFFS(x, y) (y + (x*0x20))
#define FIX_ADDR(x, y) (VRAM_FIXMAP+FIX_OFFS(x, y))

#ifndef __ASSEMBLER__

void sai_neo_fix_init(void);

#else

	.extern	sai_neo_fix_init

#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
