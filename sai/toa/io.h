#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__

#include "sai/target.h"
#include "sai/toa/hw.h"

// TODO: Variations for different implementations
#define SAI_TOA_IO_OFFS_DIP1      0x00
#define SAI_TOA_IO_OFFS_DIP2      0x02
#define SAI_TOA_IO_OFFS_DIP3      0x04
#define SAI_TOA_IO_OFFS_P1        0x06
#define SAI_TOA_IO_OFFS_P2        0x08
#define SAI_TOA_IO_OFFS_SW        0x0A

#define SAI_TOA_IO_OFFS_PCM_CMD   0x10
#define SAI_TOA_IO_OFFS_PCM_BANK  0x12
#define SAI_TOA_IO_OFFS_OPM       0x14
#define SAI_TOA_IO_OFFS_OPM_DATA  0x16
#define SAI_TOA_IO_OFFS_TEXT_BANK 0x18
#define SAI_TOA_IO_OFFS_LOCKOUT   0x1E

#ifndef __ASSEMBLER__
void sai_toa_io_init(void);
void sai_toa_io_poll(void);

#else
	.extern	sai_toa_io_init
	.extern	sai_toa_io_poll
#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
