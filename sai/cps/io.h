#pragma once

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__
#include "sai/target.h"

// CPS_PLAYER_COUNT is nominally 2, but can be 4 on CPS2 if desired.
// This changes the pin assignment for the 34-pin expansion connector and
// reduces the number of usable buttons per player, per the 4-player pinout
// (as used by D&D, etc).

#ifndef SAI_CPS2_4P
#define CPS_PLAYER_COUNT       2
#else
#define CPS_PLAYER_COUNT       4
#endif  // CPS_PLAYER_COUNT



#ifndef __ASSEMBLER__

// Struct representing the state of a gamepad.
typedef struct SaiCpsInput
{
	uint16_t now;
	uint16_t pos;
	uint16_t neg;
	uint16_t prev;
} SaiCpsInput;

extern SaiCpsInput g_cps_in[CPS_PLAYER_COUNT]

#else

.struct	0
SaiCpsInput.now = 

	.extern	g_cps_in
#endif  __ASSEMBLER__

#if SAI_TARGET == SAI_TARGET_CPS2
void sai_cps2_io_poll(void);
#elif SAI_TARGET == SAI_TARGET_CPS
void sai_cps_io_poll(void);
#endif  // SAI_TARGET
