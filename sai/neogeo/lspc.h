//
// Saikodev Neo-Geo LSPC (Sprites) support.
//

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#include "sai/neogeo/vram.h"

#ifndef __ASSEMBLER__

// Initializes sprite system hardware and clears VRAM.
void sai_neo_lspc_init(void);

#else

	.extern	sai_neo_lspc_init

#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
