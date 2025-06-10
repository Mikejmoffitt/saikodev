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

#ifdef __ASSEMBLER__
.macro	cps2_wdog_pet
	move.l	#0xFFFFFFFF, (0xFFFFFFFC).l
.endm
#endif  // __ASSEMBLER__

// Offsets from SAI_CPS2_IO_BASE.

// P1, P2
#define SAI_CPS2_IO_PL0 0x00
// Kick, P3/P4
#define SAI_CPS2_IO_PL1 0x10
// starts, coins, EEPROM read
#define SAI_CPS2_IO_SYS 0x20
// Volume, and bit flags for B board expansions.
#define SAI_CPS2_IO_VOL 0x30
// EEPROM signals, Z80 control
#define SAI_CPS2_IO_EEP 0x40
// Output port of unknown purpose.
#define SAI_CPS2_IO_OU1 0x80
// Output port of unknown purpose.
#define SAI_CPS2_IO_OU2 0x90
// SRAM control. Bit 0 is set to 1 to enble DRAM refresh.
#define SAI_CPS2_IO_SRM 0xA0
// Object RAM bank (swaps in what's at $700000 by twiddling bit 14)
#define SAI_CPS2_IO_BNK 0xE0


// Enums for SAI_CPS2_IO_PL0/1 bits. These match CPS1 fortunately.
#define SAI_CPS2_IO_SW_PL_RIGHT_BIT 0
#define SAI_CPS2_IO_SW_PL_LEFT_BIT  1
#define SAI_CPS2_IO_SW_PL_DOWN_BIT  2
#define SAI_CPS2_IO_SW_PL_UP_BIT    3
#define SAI_CPS2_IO_SW_PL_SW1_BIT   4
#define SAI_CPS2_IO_SW_PL_SW2_BIT   5
#define SAI_CPS2_IO_SW_PL_SW3_BIT   6
#define SAI_CPS2_IO_SW_PL_SW4_BIT   7

// Enums for SAI_CPS2_IO_PL1 bits, when used as kick inputs.
#define SAI_CPS2_IO_SW_PL1_KICK1_BIT 0
#define SAI_CPS2_IO_SW_PL1_KICK2_BIT 1
#define SAI_CPS2_IO_SW_PL1_KICK3_BIT 2
#define SAI_CPS2_IO_SW_PL2_KICK1_BIT 4
#define SAI_CPS2_IO_SW_PL2_KICK2_BIT 5
// P2 kick 3 is located on SAI_CPS2_IO_SYS, for some reason.

// Enums for SAI_CPS2_IO_SYS bits. This also contains P2 strong kick.
#define SAI_CPS2_IO_SYS_EEP_DO_BIT    0
#define SAI_CPS2_IO_SW_TEST_BIT       1
#define SAI_CPS2_IO_SW_SERVICE_BIT    2
#define SAI_CPS2_IO_SW_START1_BIT     8
#define SAI_CPS2_IO_SW_START2_BIT     9
#define SAI_CPS2_IO_SW_START3_BIT    10
#define SAI_CPS2_IO_SW_START4_BIT    11
#define SAI_CPS2_IO_SW_COIN1_BIT     12
#define SAI_CPS2_IO_SW_COIN2_BIT     13
#define SAI_CPS2_IO_SW_COIN3_BIT     14
#define SAI_CPS2_IO_SW_COIN4_BIT     15
#define SAI_CPS2_IO_SW_PL2_KICK3_BIT 14

// Enums for SAI_CPS2_IO_VOL bits.
#define SAI_CPS2_IO_SW_VOL_DOWN     1
#define SAI_CPS2_IO_SW_VOL_UP       2

// Enums for SAI_CPS2_IO_EEP bits. Remember that DO is on the SYS port.
#define SAI_CPS2_IO_EEP_COINC1_BIT  0
#define SAI_CPS2_IO_EEP_COINC2_BIT  1

// Bring high before accessing Z80 memory!
#define SAI_CPS2_IO_EEP_Z80_RESET   4
#define SAI_CPS2_IO_EEP_LOCK1_BIT   5
#define SAI_CPS2_IO_EEP_LOCK2_BIT   6
#define SAI_CPS2_IO_EEP_CN5_PIN7    7
#define SAI_CPS2_IO_EEP_CN5_PIN8    8

#define SAI_CPS2_IO_EEP_DI_BIT     12
#define SAI_CPS2_IO_EEP_CLK_BIT    13
#define SAI_CPS2_IO_EEP_CS_BIT     14

#ifndef __ASSEMBLER__
void sai_cps2_io_poll(void);
#else
	.extern	sai_min_cps2_io_init;
	.extern	sai_cps2_io_poll;
#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
