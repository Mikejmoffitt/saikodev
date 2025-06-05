#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__

// SAI_PLAYER_COUNT is nominally 2, but can be 4 on CPS2 if desired.
// This changes the pin assignment for the 34-pin expansion connector and
// reduces the number of usable buttons per player, per the 4-player pinout
// (as used by D&D, etc).

#ifndef __ASSEMBLER__
void sai_cps2_io_poll(void);
#else
	.extern	sai_min_cps2_io_init;
	.extern	sai_cps2_io_poll;
#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
