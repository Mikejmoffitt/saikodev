#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__

#define CPS_IO_OFFS_PL1        0x00
#define CPS_IO_OFFS_SYS        0x18
#define CPS_IO_OFFS_DIP1       0x1A
#define CPS_IO_OFFS_DIP2       0x1C
#define CPS_IO_OFFS_DIP3       0x11
#define CPS_IO_OFFS_SOUND1     0x80
#define CPS_IO_OFFS_SOUND2     0x88

#ifndef __ASSEMBLER__
void sai_cps_io_poll(void);
#else
	.extern	sai_cps_io_poll;
#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
