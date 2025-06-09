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

#if SAI_PLAYER_COUNT > 2
#error "C2 does not support a player count greater than 2!"
#endif  // SAI_PLAYER_COUNT

// On C2, ports D and H are outputs.
#define SYSC_IO_DIR_DEFAULT    0x88  // ports D and H as outputs

// Ports A, B (P1 and P2 respectively)
#define SYSC_IO_PL_LEFT        SAI_BITVAL(7)
#define SYSC_IO_PL_RIGHT       SAI_BITVAL(6)
#define SYSC_IO_PL_UP          SAI_BITVAL(5)
#define SYSC_IO_PL_DOWN        SAI_BITVAL(4)
#define SYSC_IO_PL_D           SAI_BITVAL(3)
#define SYSC_IO_PL_C           SAI_BITVAL(2)
#define SYSC_IO_PL_B           SAI_BITVAL(1)
#define SYSC_IO_PL_A           SAI_BITVAL(0)

// Port C - Misc in
#define SYSC_IO_MI_RESET       SAI_BITVAL(7)
#define SYSC_IO_MI_ADPCM_BUY   SAI_BITVAL(6)
#define SYSC_IO_MI_CN2_PIN8    SAI_BITVAL(5)
#define SYSC_IO_MI_CN2_PIN7    SAI_BITVAL(4)
#define SYSC_IO_MI_CN2_PIN5    SAI_BITVAL(3)
#define SYSC_IO_MI_CN2_PIN4    SAI_BITVAL(2)
#define SYSC_IO_MI_CN2_PIN3    SAI_BITVAL(1)
#define SYSC_IO_MI_CN2_PIN2    SAI_BITVAL(0)

// Port D - Misc out
#define SYSC_IO_MO_WDOG_CTRL   SAI_BITVAL(7)
#define SYSC_IO_MO_MUTE        SAI_BITVAL(6)
#define SYSC_IO_MO_CN2_PIN10   SAI_BITVAL(5)
#define SYSC_IO_MO_CN2_PIN11   SAI_BITVAL(4)
#define SYSC_IO_MO_LOCKOUT2    SAI_BITVAL(3)
#define SYSC_IO_MO_LOCKOUT1    SAI_BITVAL(2)
#define SYSC_IO_MO_METER2      SAI_BITVAL(1)
#define SYSC_IO_MO_METER1      SAI_BITVAL(0)
#define SYSC_IO_MO_DEFAULT     (SYSC_IO_MO_WDOG_CTRL | \
                                SYSC_IO_MO_CN2_PIN10 | \
                                SYSC_IO_MO_CN2_PIN11)

// Port E - System
#define SYSC_IO_SYS_UNUSED     SAI_BITVAL(7)
#define SYSC_IO_SYS_SELECT     SAI_BITVAL(6)
#define SYSC_IO_SYS_START2     SAI_BITVAL(5)
#define SYSC_IO_SYS_START1     SAI_BITVAL(4)
#define SYSC_IO_SYS_SERVICE    SAI_BITVAL(3)
#define SYSC_IO_SYS_TEST       SAI_BITVAL(2)
#define SYSC_IO_SYS_COIN1      SAI_BITVAL(1)
#define SYSC_IO_SYS_COIN2      SAI_BITVAL(0)

// Port F and G are DIP 1 and 2.

// Port H - Misc out 2 / Banking control
#define SYSC_IO_MO2_CN4_PINA19 SAI_BITVAL(7)
#define SYSC_IO_MO2_CN4_PINB19 SAI_BITVAL(6)
#define SYSC_IO_MO2_ADPCMBANK3 SIA_BITVAL(5)
#define SYSC_IO_MO2_ADPCMBANK2 SIA_BITVAL(4)
#define SYSC_IO_MO2_ADPCMBANK1 SIA_BITVAL(3)
#define SYSC_IO_MO2_ADPCMBANK0 SIA_BITVAL(2)
#define SYSC_IO_MO2_PALBANK1   SAI_BITVAL(1)
#define SYSC_IO_MO2_PALBANK0   SAI_BITVAL(0)
#define SYSC_IO_MO2_DEFAULT    (0)


#ifndef __ASSEMBLER__
void sai_c2_io_init(void);
void sai_c2_io_poll(void);
#else
	.extern	sai_c2_io_init
	.extern	sai_min_c2_io_init
	.extern	sai_c2_io_poll
#endif  // __ASEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
