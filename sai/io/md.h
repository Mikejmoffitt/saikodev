#pragma once

#ifndef __ASSEMBLER__
#include <stdint.h>
#include <stdbool.h>
#endif  // __ASSEMBLER__
#include "sai/memmap.h"
#include "sai/macro.h"

#ifndef SAI_MD_PAD_COUNT
#define SAI_MD_PAD_COUNT 2
#endif  // SAI_MD_PAD_COUNT

#if SAI_MD_PAD_COUNT > 3
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

// Gamepad functions
void sai_md_pad_init(void);
void sai_md_pad_poll(void);

// Sub-cpu functions
static inline bool sai_md_z80_bus_taken(void);
static inline void sai_md_z80_bus_release(void);
static inline void sai_md_z80_reset_assert(void);
static inline void sai_md_z80_reset_deassert(void);
static inline void sai_md_z80_bus_req(bool wait);

//
// Static implementations
//

static inline bool sai_md_z80_bus_taken(void)
{
	volatile uint8_t *z80_bus = (volatile uint8_t *)MD_Z80_BUSREQ;
	return !(*z80_bus & 0x01);
}

static inline void sai_md_z80_bus_req(bool wait)
{
	volatile uint8_t *z80_bus = (volatile uint8_t *)MD_Z80_BUSREQ;

	*z80_bus = 0x01;
	if (!wait) return;
	while (*z80_bus & 0x01) __asm__("nop");
}

static inline void sai_md_z80_bus_release(void)
{
	volatile uint8_t *z80_bus = (volatile uint8_t *)MD_Z80_BUSREQ;
	*z80_bus = 0x00;
}

static inline void sai_md_z80_reset_deassert(void)
{
	volatile uint8_t *z80_reset = (volatile uint8_t *)MD_Z80_RESET;
	*z80_reset = 0x01;
}

static inline void sai_md_z80_reset_assert(void)
{
	volatile uint8_t *z80_reset = (volatile uint8_t *)MD_Z80_RESET;
	*z80_reset = 0x00;
}

#else
	.struct 0

SaiMdPad.now:	ds.w 1
SaiMdPad.pos:	ds.w 1
SaiMdPad.neg:	ds.w 1
SaiMdPad.prev:	ds.w 1
SaiMdPad.len:

	.extern sai_min_md_pad_init
	.extern sai_min_md_z80_init

.macro	SAI_MD_Z80_RESET_ASSERT
	move.w	#0x000, MD_Z80_RESET
.endm

.macro	SAI_MD_Z80_RESET_DEASSERT
	move.w	#0x100, MD_Z80_RESET
.endm

.macro	SAI_MD_Z80_RESET_WAIT
	SAI_MD_Z80_RESET_ASSERT
	moveq	#20, d0
9:
	dbf	d0, 9b
.endm

.macro	SAI_MD_Z80_BUSREQ
	move.w	#0x100, MD_Z80_BUSREQ
.endm

.macro	SAI_MD_Z80_BUSREQ_WAIT
	SAI_MD_Z80_BUSREQ
9:
	btst	#0, MD_Z80_BUSREQ
	bne.s	9b
.endm

.macro	SAI_MD_Z80_BUSREL
	move.w	#0x000, MD_Z80_BUSREQ
.endm

.macro	SAI_MD_Z80_PAUSE
	SAI_MD_Z80_BUSREQ
9:
	btst.b	#0, #MD_Z80_BUSREQ
	bne.s	9b
.endm

#endif  // __ASSEMBLER__

