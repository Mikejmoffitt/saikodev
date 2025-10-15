//
// Backup RAM configuration data.
//
#pragma once

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__

//
// Struct representing backup RAM data for this game.
//
#ifndef __ASSEMBLER__

typedef struct
{
	uint16_t boot_count;
} BramData;

extern BramData g_bram;

#else

	.struct	0
BramData.boot_count:   ds.w 1
BramData.len:

#endif  // __ASSEMBLER__
