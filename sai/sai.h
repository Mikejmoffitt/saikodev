#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#include "sai/target.h"

#ifndef SAI_TARGET
#error "SAI_TARGET must be defined using a value from target.h!"
#endif  // SAI_TARGET

#include "sai/macro.h"
#include "sai/input.h"
#include "sai/irq.h"
#include "sai/memmap.h"
// MD / C1 / C2 share a lot.
#if SAI_TARGET == SAI_TARGET_MD || SAI_TARGET == SAI_TARGET_C1 | SAI_TARGET == SAI_TARGET_C2
#include "sai/md/irq.h"
#include "sai/md/vdp.h"
#include "sai/md/vdp_dma_queue.h"
#include "sai/md/vdp_spr.h"
#include "sai/md/vdp_csp.h"
#if SAI_TARGET == SAI_TARGET_MD
#include "sai/md/io.h"
#include "sai/md/pal.h"
#else
#include "sai/c2/io.h"
#include "sai/c2/pal.h"
#include "sai/c2/prot.h"
#include "sai/c2/s5296.h"
#include "sai/c2/upd7759.h"
#endif  // SAI_TARGET

#elif SAI_TARGET == SAI_TARGET_S16B
#include "sai/s16b/pal.h"
#elif SAI_TARGET == SAI_TARGET_S18
#include "sai/s16b/pal.h"
#include "sai/s18/io.h"
#elif SAI_TARGET == SAI_TARGET_CPS
#include "sai/cps/pal.h"
#include "sai/cps/ppu.h"
#include "sai/cps/io.h"
#include "sai/cps/obj.h"
#elif SAI_TARGET == SAI_TARGET_CPS2
#include "sai/cps/pal.h"
#include "sai/cps/ppu.h"
#include "sai/cps2/io.h"
#include "sai/cps2/obj.h"
#elif SAI_TARGET == SAI_TARGET_ESPRADE
#include "sai/at/io.h"
#include "sai/at/pal.h"
#include "sai/at/sp013.h"
#include "sai/at/bg038.h"
#include "sai/at/ymz.h"
#endif

#ifndef __ASSEMBLER__

// Call after C runtime initialization.
void sai_init(void);

// Call at the end of the main loop to handle
// * finalization of sprite list(s)
// * palette management / transfer preparation
// * frame synchronization
// * schedule transfers
// * input polling
void sai_finish(void);
#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
