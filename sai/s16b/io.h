#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdint.h>
#include <stdbool.h>
#endif  // __ASSEMBLER__
#include "sai/memmap.h"
#include "sai/macro.h"
#include "sai/s16b/io.h"

#define S16_IO_MISC_OFFS       (0x0001)
#define S16_IO_MISC_FLIP       SAI_BITVAL(6)
#define S16_IO_MISC_DISP_EN    SAI_BITVAL(5)
#define S16_IO_MISC_LAMP2      SAI_BITVAL(3)
#define S16_IO_MISC_LAMP1      SAI_BITVAL(2)
#define S16_IO_MISC_CNT2       SAI_BITVAL(1)
#define S16_IO_MISC_CNT1       SAI_BITVAL(0)

#define S16_IO_MISC_DEFAULT    (S16_IO_MISC_DISP_EN)

// The four input ports map to edge pins.

// Some player special inputs. st21STCC
#define S16_IO_INPUT_1_OFFS    (0x1001)
#define S16_IO_INPUT_1_PINA    SAI_BITVAL(7)
#define S16_IO_INPUT_1_PINZ    SAI_BITVAL(6)
#define S16_IO_INPUT_1_PINY    SAI_BITVAL(5)
#define S16_IO_INPUT_1_PINX    SAI_BITVAL(4)
#define S16_IO_INPUT_1_PIN23   SAI_BITVAL(3)
#define S16_IO_INPUT_1_PIN22   SAI_BITVAL(2)
#define S16_IO_INPUT_1_PIN21   SAI_BITVAL(1)
#define S16_IO_INPUT_1_PIN20   SAI_BITVAL(0)

#define S16_IO_INPUT_1_COIN1_BIT   0
#define S16_IO_INPUT_1_COIN2_BIT   1
#define S16_IO_INPUT_1_TEST_BIT    2
#define S16_IO_INPUT_1_SERVICE_BIT 3
#define S16_IO_INPUT_1_START1_BIT  4
#define S16_IO_INPUT_1_START2_BIT  5

// Generally player 1 inputs. LRUD4213
#define S16_IO_INPUT_2_OFFS    (0x1003)
#define S16_IO_INPUT_2_PIN15   SAI_BITVAL(7)
#define S16_IO_INPUT_2_PIN14   SAI_BITVAL(6)
#define S16_IO_INPUT_2_PIN13   SAI_BITVAL(5)
#define S16_IO_INPUT_2_PIN12   SAI_BITVAL(4)
#define S16_IO_INPUT_2_PIN11   SAI_BITVAL(3)
#define S16_IO_INPUT_2_PIN10   SAI_BITVAL(2)
#define S16_IO_INPUT_2_PIN9    SAI_BITVAL(1)
#define S16_IO_INPUT_2_PIN8    SAI_BITVAL(0)

// Additional inputs.
#define S16_IO_INPUT_3_OFFS    (0x1005)
#define S16_IO_INPUT_3_PINW    SAI_BITVAL(7)
#define S16_IO_INPUT_3_PINV    SAI_BITVAL(6)
#define S16_IO_INPUT_3_PINU    SAI_BITVAL(5)
#define S16_IO_INPUT_3_PINT    SAI_BITVAL(4)
#define S16_IO_INPUT_3_PIN19   SAI_BITVAL(3)
#define S16_IO_INPUT_3_PIN18   SAI_BITVAL(2)
#define S16_IO_INPUT_3_PIN17   SAI_BITVAL(1)
#define S16_IO_INPUT_3_PIN16   SAI_BITVAL(0)

// Generally player 2 inputs. LRUD4213
#define S16_IO_INPUT_4_OFFS    (0x1007)
#define S16_IO_INPUT_4_PINS    SAI_BITVAL(7)
#define S16_IO_INPUT_4_PINR    SAI_BITVAL(6)
#define S16_IO_INPUT_4_PINP    SAI_BITVAL(5)
#define S16_IO_INPUT_4_PINN    SAI_BITVAL(4)
#define S16_IO_INPUT_4_PINM    SAI_BITVAL(3)
#define S16_IO_INPUT_4_PINL    SAI_BITVAL(2)
#define S16_IO_INPUT_4_PINK    SAI_BITVAL(1)
#define S16_IO_INPUT_4_PINJ    SAI_BITVAL(0)

#define S16_IO_DIP2_OFFS       (0x2001)
#define S16_IO_DIP1_OFFS       (0x2003)

#ifndef __ASSEMBLER__
extern uint16_t g_sai_io_misc_ctrl;  // only lower byte used.
extern uint8_t g_sai_io_dip[2];

void sai_s16b_io_init(void);
void sai_s16b_io_poll(void);
#else
	.extern	g_sai_io_misc_ctrl
	.extern	g_sai_io_dip

	.extern	sai_s16b_io_init
	.extern	sai_min_s16b_io_init
	.extern	sai_s16b_io_poll
#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
