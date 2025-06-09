// Sega Mega Drive / Genesis / System C2 IRQ callback functions.
// These are pointers to normal functions that do not need to return with `rte`.

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#include "sai/target.h"
#include "sai/irq.h"

#ifndef __ASSEMBLER__
#if SAI_TARGET != SAI_TARGET_MD
extern void (*g_irq_fm_callback)(void);
#else
extern void (*g_irq_th_callback)(void);
#endif  // SAI_TARGET

extern void (*g_irq_vbl_callback)(void);
extern void (*g_irq_hbl_callback)(void);

#else
#if SAI_TARGET != SAI_TARGET_MD
	.extern g_irq_fm_callback
#else
	.extern g_irq_th_callback
#endif  // SAI_TARGET
	.extern	g_irq_hbl_callback
	.extern	g_irq_vbl_callback
#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
