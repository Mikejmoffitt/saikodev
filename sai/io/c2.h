#pragma once

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__
#include "sai/macro.h"
#include "sai/memmap.h"

/* I/O port data locations */
#define SYSC_IO_PORTA          (SYSC_IO_BASE + 0x01)
#define SYSC_IO_PORTB          (SYSC_IO_BASE + 0x03)
#define SYSC_IO_PORTC          (SYSC_IO_BASE + 0x05)
#define SYSC_IO_PORTD          (SYSC_IO_BASE + 0x07)
#define SYSC_IO_PORTE          (SYSC_IO_BASE + 0x09)
#define SYSC_IO_PORTF          (SYSC_IO_BASE + 0x0B)
#define SYSC_IO_PORTG          (SYSC_IO_BASE + 0x0D)
#define SYSC_IO_PORTH          (SYSC_IO_BASE + 0x0F)
/* I/O protection registers (for reading the string "SEGA") */
#define SYSC_IO_PROT0          (SYSC_IO_BASE + 0x11)
#define SYSC_IO_PROT1          (SYSC_IO_BASE + 0x13)
#define SYSC_IO_PROT2          (SYSC_IO_BASE + 0x15)
#define SYSC_IO_PROT3          (SYSC_IO_BASE + 0x17)
/* I/O control registers */
#define SYSC_IO_CTRL0          (SYSC_IO_BASE + 0x19)
#define SYSC_IO_CTRL1          (SYSC_IO_BASE + 0x1B)
#define SYSC_IO_CTRL2          (SYSC_IO_BASE + 0x1D)
#define SYSC_IO_CTRL3          (SYSC_IO_BASE + 0x1F)
/* Protection register */
#define SYSC_PROT_SECURITY     (SYSC_PROTECTION_BASE + 0x001)
#define SYSC_PROT_VCTRL        (SYSC_PROTECTION_BASE + 0x201)
#define SYSC_UPD7759_PHRASE    (SYSC_IO_UPD7759_BASE + 1)

// Definitions for the port bits

// Ports A, B (P1 and P2 respectively)
#define SYSC_PL_LEFT           SAI_BITVAL(7)
#define SYSC_PL_RIGHT          SAI_BITVAL(6)
#define SYSC_PL_UP             SAI_BITVAL(5)
#define SYSC_PL_DOWN           SAI_BITVAL(4)
#define SYSC_PL_D              SAI_BITVAL(3)
#define SYSC_PL_C              SAI_BITVAL(2)
#define SYSC_PL_B              SAI_BITVAL(1)
#define SYSC_PL_A              SAI_BITVAL(0)

#define SYSC_SYS_UNUSED        SAI_BITVAL(7)
#define SYSC_SYS_SELECT        SAI_BITVAL(6)
#define SYSC_SYS_START2        SAI_BITVAL(5)
#define SYSC_SYS_START1        SAI_BITVAL(4)
#define SYSC_SYS_SERVICE       SAI_BITVAL(3)
#define SYSC_SYS_TEST          SAI_BITVAL(2)
#define SYSC_SYS_COIN1         SAI_BITVAL(1)
#define SYSC_SYS_COIN2         SAI_BITVAL(0)

#ifdef __ASSEMBLER__
	.extern	sai_min_c2_io_init
#endif  // __ASEMBLER__
