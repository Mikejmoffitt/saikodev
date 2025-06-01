// Sega Mega Drive / Genesis / System C2 IRQ callback functions.
// These are pointers to normal functions that do not need to return with `rte`.

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

#include "sai/target.h"

#ifndef __ASSEMBLER__
#if SAI_TARGET == SAI_TARGET_C2
extern void (*g_irq_fm_callback)(void);
#else
extern void (*g_irq_th_callback)(void);
#endif  // SAI_TARGET

extern void (*g_irq_vbl_callback)(void);
extern void (*g_irq_hbl_callback)(void);

#else
#if SAI_TARGET == SAI_TARGET_C2
	.extern g_irq_fm_callback
#else
	.extern g_irq_th_callback
#endif  // SAI_TARGET
	.extern	g_irq_hbl_callback
	.extern	g_irq_vbl_callback

	.extern	_v_irq1
	.extern	_v_irq2
	.extern	_v_irq3
	.extern	_v_irq4
	.extern	_v_irq5
	.extern	_v_irq6
	.extern	_v_irq7
#endif  // __ASSEMBLER__
