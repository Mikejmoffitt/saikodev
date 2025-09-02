// Toaplan GCU interrupt callback functions.
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#include "sai/target.h"
#include "sai/irq.h"

#ifndef __ASSEMBLER__
extern void (*g_irq_vbl_callback)(void);
#else
	.extern	g_irq_vbl_callback
#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
