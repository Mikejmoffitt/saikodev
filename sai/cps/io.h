#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__

#define SAI_CPS_IO_PL         0x00
#define SAI_CPS_IO_SYS        0x18
#define SAI_CPS_IO_DIP1       0x1A
#define SAI_CPS_IO_DIP2       0x1C
#define SAI_CPS_IO_DIP3       0x11
#define SAI_CPS_IO_SOUND1     0x80
#define SAI_CPS_IO_SOUND2     0x88

// Enums for SAI_CPS_IO_PL0/1 bits.
#define SAI_CPS1_IO_SW_PL_RIGHT_BIT 0
#define SAI_CPS1_IO_SW_PL_LEFT_BIT  1
#define SAI_CPS1_IO_SW_PL_DOWN_BIT  2
#define SAI_CPS1_IO_SW_PL_UP_BIT    3
#define SAI_CPS1_IO_SW_PL_SW1_BIT   4
#define SAI_CPS1_IO_SW_PL_SW2_BIT   5
#define SAI_CPS1_IO_SW_PL_SW3_BIT   6
#define SAI_CPS1_IO_SW_PL_SW4_BIT   7

// TODO: kicks

// For SAI_CPS_IO_SYS
#define SAI_CPS1_IO_SW_COIN1_BIT    0
#define SAI_CPS1_IO_SW_COIN2_BIT    1
#define SAI_CPS1_IO_SW_SERVICE_BIT  2
#define SAI_CPS1_IO_SW_START1_BIT   4
#define SAI_CPS1_IO_SW_START2_BIT   5
#define SAI_CPS1_IO_SW_TEST_BIT     6

#ifndef __ASSEMBLER__

extern uint8_t g_sai_io_dip[3];

void sai_cps_io_init(void);
void sai_cps_io_poll(void);
#else
	.extern	sai_cps_io_init
	.extern	sai_cps_io_poll;
#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
