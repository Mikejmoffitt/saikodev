#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__

#include "sai/target.h"

// TODO: Different for Guwange
#define SAI_AT_IO_OFFS_P1      (0)
#define SAI_AT_IO_OFFS_P2      (2)

// EEP data in is on the Player 2 IO port.
#define SAI_AT_IO_EEP_DO_BIT   11
// EEP data out is set by writing to the Player 1 port.
// TODO: For Guwange, the mapping is different.
#define SAI_AT_IO_EEP_CS_BIT   9
#define SAI_AT_IO_EEP_CLK_BIT  10
#define SAI_AT_IO_EEP_DI_BIT   11
#define SAI_AT_IO_LOCK1_BIT    12
#define SAI_AT_IO_LOCK2_BIT    13
#define SAI_AT_IO_CNT1_BIT     14
#define SAI_AT_IO_CNT2_BIT     15

#ifndef __ASSEMBLER__
void sai_at_io_init(void);
void sai_at_io_poll(void);
#else
	.extern	sai_at_io_init
	.extern	sai_at_io_poll
#endif  // __ASSEMBLER__
#ifdef __cplusplus
}
#endif  // __cplusplus
