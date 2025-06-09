#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__
#include "sai/macro.h"
#include "sai/target.h"
#include "sai/memmap.h"

// On C2, ports D and H are outputs.
#define S18_IO_DIR_DEFAULT    0x88  // ports D and H as outputs
#define S18_IO_CNT_VID_S18_EN SAI_BITVAL(1)
#define S18_IO_CNT_VID_VDP_EN SAI_BITVAL(2)
#define S18_IO_CNT_DEFAULT    ((S18_IO_CNT_VID_S18_EN) | \
                               (S18_IO_CNT_VID_VDP_EN))

// Ports A, B, C (P1, P2, and P3 respectively)
#define S18_IO_PL_LEFT        SAI_BITVAL(7)
#define S18_IO_PL_RIGHT       SAI_BITVAL(6)
#define S18_IO_PL_UP          SAI_BITVAL(5)
#define S18_IO_PL_DOWN        SAI_BITVAL(4)
#define S18_IO_PL_D           SAI_BITVAL(3)
#define S18_IO_PL_C           SAI_BITVAL(2)
#define S18_IO_PL_B           SAI_BITVAL(1)
#define S18_IO_PL_A           SAI_BITVAL(0)

// Port D - Misc out
#define S18_IO_MO_GREYSCALE   SAI_BITVAL(6)
#define S18_IO_MO_FLIP        SAI_BITVAL(5)
#define S18_IO_MO_LOCKOUT2    SAI_BITVAL(3)
#define S18_IO_MO_LOCKOUT1    SAI_BITVAL(2)
#define S18_IO_MO_METER2      SAI_BITVAL(1)
#define S18_IO_MO_METER1      SAI_BITVAL(0)
#define S18_IO_MO_DEFAULT     (0)

// Port E - System
#define S18_IO_SYS_UNUSED     SAI_BITVAL(7)
#define S18_IO_SYS_SELECT     SAI_BITVAL(6)
#define S18_IO_SYS_START2     SAI_BITVAL(5)
#define S18_IO_SYS_START1     SAI_BITVAL(4)
#define S18_IO_SYS_SERVICE    SAI_BITVAL(3)
#define S18_IO_SYS_TEST       SAI_BITVAL(2)
#define S18_IO_SYS_COIN1      SAI_BITVAL(1)
#define S18_IO_SYS_COIN2      SAI_BITVAL(0)

// Port G - Dip switch

// Port H - Tile graphics bank

#ifndef __ASSEMBLER__
extern uint16_t g_sai_io_vrom_bank;
extern uint16_t g_sai_io_dip;

void sai_s18_io_init(void);
void sai_s18_io_poll(void);
#else
	.extern	g_sai_io_vrom_bank
	.extern	g_sai_io_dip

	.extern	sai_s18_io_init
	.extern	sai_min_s18_io_init
	.extern	sai_s18_io_poll


#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
