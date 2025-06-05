//
// Sega 315-5296 I/O chip offsets
//
// How this chip is used varies per platform, so it is not baked in here
//
#pragma once

#ifndef __ASSEMBLER__
#include <stdint.h>
#include <stdbool.h>
#endif  // __ASSEMBLER__
#include "sai/memmap.h"
#include "sai/macro.h"

// Data ports
#define S5296_OFFS_PORTA       0x01
#define S5296_OFFS_PORTB       0x03
#define S5296_OFFS_PORTC       0x05
#define S5296_OFFS_PORTD       0x07
#define S5296_OFFS_PORTE       0x09
#define S5296_OFFS_PORTF       0x0A
#define S5296_OFFS_PORTG       0x0C
#define S5296_OFFS_PORTH       0x0E
// Protection registers reading "SEGA"
#define S5296_OFFS_PROT0       0x11
#define S5296_OFFS_PROT1       0x13
#define S5296_OFFS_PROT2       0x15
#define S5296_OFFS_PROT3       0x17
// Control registers
#define S5296_OFFS_CNT         0x19
#define S5296_OFFS_DIR         0x1B
#define S5296_OFFS_CNT2        0x1D
#define S5296_OFFS_CTRL2       0x1F

// CNT register:
// Dedicated CNT0-CNT2 output pins are controlled here.
// 7654 3210
// kk.. .... CKOT clock divider (/4, /8, /16, /2)
// ..dd .... CNT2 clock divder (/4, /8, /16, /2)
// .... m... CNT2 output mode (1 = clock out, 0 = programmable
// .... .qqq Data to output
//

// DIR register
// Whole ports are configured as input/output together.
// A high bit indicates an output.
// 7654 3210
// h... .... Port H direction
// .g.. .... Port G direction
// ..f. .... Port F direction
// ...e .... Port E direction
// .... d... Port D direction
// .... .c.. Port C direction
// .... ..b. Port B direction
// .... ...a Port A direction
