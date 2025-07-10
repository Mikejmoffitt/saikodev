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

#ifndef __ASSEMBLER__

//
// Gamepad functions
//

// Sets up I/O pins for pad use.
void sai_md_pad_init(void);
// Reads inputs from pads and places them in g_sai_in.
void sai_md_pad_poll(void);

//
// Sub-cpu functions
//

// Clears Z80 memory and installs a stub program.
void sai_md_z80_init(void);

// Loads a program into Z80 memory and resets it.
void sai_md_z80_load(const uint8_t *prg_data, uint16_t prg_bytes);

// Returns true if the Z80 bus request has completed.
static inline bool sai_md_z80_bus_taken(void);
// Releases the Z80 bus and unpauses the Z80.
static inline void sai_md_z80_bus_release(void);
// Pulls down the reset line on the Z80.
static inline void sai_md_z80_reset_assert(void);
// Releases the reset line on the Z80.
static inline void sai_md_z80_reset_deassert(void);
// Requests the Z80 bus and optionally waits for confirmation.
static inline void sai_md_z80_bus_req(bool wait);

// -----------------------------------------------------------------------------
//
// Static implementations
//
// -----------------------------------------------------------------------------

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

//
// Gamepad functions
//

// Sets up I/O pins for pad use (uses no RAM).
	.extern sai_min_md_pad_init
// Gives Z80 inactive stub program (uses no work RAM).
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

#ifdef __cplusplus
}
#endif  // __cplusplus
