#pragma once

#include <stdint.h>
#include "sai/memmap.h"
#include "sai/macro.h"

#ifndef SAI_MD_PAD_COUNT
#define SAI_MD_PAD_COUNT 2
#endif  // SAI_MD_PAD_COUNT

#if SAI_MD_PAD_COUNT > 3)
#error "SAI_MD_PAD_COUNT > 3!"
#endif  // SAI_MD_PAD_COUNT

// Memory map.
#define MD_IO_VERSION          (MD_IO_BASE + 0x01)
#define MD_IO_DATA1            (MD_IO_BASE + 0x03)
#define MD_IO_DATA2            (MD_IO_BASE + 0x05)
#define MD_IO_DATA3            (MD_IO_BASE + 0x07)
#define MD_IO_CTRL1            (MD_IO_BASE + 0x09)
#define MD_IO_CTRL2            (MD_IO_BASE + 0x0B)
#define MD_IO_CTRL3            (MD_IO_BASE + 0x0D)
#define MD_IO_TXD1             (MD_IO_BASE + 0x0F)
#define MD_IO_RXD1             (MD_IO_BASE + 0x11)
#define MD_IO_SERIO1           (MD_IO_BASE + 0x13)
#define MD_IO_TXD2             (MD_IO_BASE + 0x15)
#define MD_IO_RXD2             (MD_IO_BASE + 0x17)
#define MD_IO_SERIO2           (MD_IO_BASE + 0x19)
#define MD_IO_TXD3             (MD_IO_BASE + 0x1B)
#define MD_IO_RXD3             (MD_IO_BASE + 0x1D)
#define MD_IO_SERIO3           (MD_IO_BASE + 0x1F)

// Button state values.
#define MD_PAD_UP              (SAI_BITVAL(0))
#define MD_PAD_DOWN            (SAI_BITVAL(1))
#define MD_PAD_LEFT            (SAI_BITVAL(2))
#define MD_PAD_RIGHT           (SAI_BITVAL(3))
#define MD_PAD_B               (SAI_BITVAL(4))
#define MD_PAD_C               (SAI_BITVAL(5))
#define MD_PAD_A               (SAI_BITVAL(6))
#define MD_PAD_START           (SAI_BITVAL(7))
#define MD_PAD_Z               (SAI_BITVAL(8))
#define MD_PAD_Y               (SAI_BITVAL(9))
#define MD_PAD_X               (SAI_BITVAL(10))
#define MD_PAD_MODE            (SAI_BITVAL(11))
#define MD_PAD_6B              (SAI_BITVAL(14))
#define MD_PAD_UNPLUGGED       (SAI_BITVAL(15))

#ifndef __ASSEMBLER__
// Struct representing the state of a gamepad.
typedef struct SaiMdPad
{
	uint16_t now;
	uint16_t pos;
	uint16_t neg;
	uint16_t prev;
} SaiMdPad;

extern SaiMdPad g_md_pad[SAI_MD_PAD_COUNT];

void sai_md_pad_init(void);
void sai_md_pad_poll(void);
#else
	.struct 0

SaiMdPad.now:	ds.w 1
SaiMdPad.pos:	ds.w 1
SaiMdPad.neg:	ds.w 1
SaiMdPad.prev:	ds.w 1
SaiMdPad.len:

#endif  // __ASSEMBLER__
