// In general IRQ callbacks can be registered by the user.
// Actions that are considered a standard part of the interrupt handler (an
// acknowledgement for the IRQ source, setting a vbl hit flag, etc) are
// performed before calling the user routine.
//
// Handler storage and vectors are defined and implemented on a platform basis.
//
// The use of this callback is optional. As the first byte of a pointer does not
// physically reach the address bus, the first byte is thus used as an indicator
// of what kind of pointer has been set.
// If the first byte is zero, as is the case with the address of a function in
// ROM loaded normally, the callback function is called with typical register
// clobber protection of d0-d1/a0-a1.
// If the first byte is positive but nonzero, no function is called.
// If the first byte is negative (bit 7 is set) then the callback is executed
// with minimal stack protection (a0 is preserved by necessity for the call).
// callback is executed without any register protection on the stack. This is
// meant for routines hand-written for performance reasons.

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#define SAI_IRQ_HANDLER_UNUSED 0x68000000

#ifndef __ASSEMBLER__

#include <stdint.h>

extern uint8_t g_sai_vbl_wait_flag;

void sai_irq_vbl_wait(void);

#else

	.extern	g_sai_vbl_wait_flag

.macro	virq_handler cb
	sf	g_sai_vbl_wait_flag
	irq_handler \cb
.endm

.macro	irq_handler cb
	tst.b	\cb
	bpl.s	0f
	// Reduced protection case
	move.l	a0, -(sp)
	movea.l	\cb, a0
	jsr	(a0)
	move.l	(sp)+, a0
	rte
0:
	bne.s	1f
	movem.l	d0-d1/a0-a1, -(sp)
	movea.l	\cb, a0
	jsr	(a0)
	movem.l	(sp)+, d0-d1/a0-a1
1:
	rte
.endm

	.extern	_v_irq1
	.extern	_v_irq2
	.extern	_v_irq3
	.extern	_v_irq4
	.extern	_v_irq5
	.extern	_v_irq6
	.extern	_v_irq7

	.extern	sai_irq_vbl_wait

#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
