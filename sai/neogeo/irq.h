// Saikodev Neo-Geo IRQ callbacks.
// These are pointers to normal functions that do not need to return with `rte`.

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#include "sai/target.h"
#include "sai/irq.h"
#include "sai/macro.h"

//
// IRQ Ack bits
//
SAI_ENUMSTART
SAI_ENUMNEXT(SAI_NEO_ACKBIT_RESET)
SAI_ENUMNEXT(SAI_NEO_ACKBIT_TIMER)
SAI_ENUMNEXT(SAI_NEO_ACKBIT_VBLANK)
SAI_ENUMEND

#ifndef __ASSEMBLER__

extern void (*g_irq_vbl_callback)(void);
extern void (*g_irq_timer_callback)(void);

#else
	.extern	g_irq_vbl_callback
	.extern	g_irq_timer_callback
#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
