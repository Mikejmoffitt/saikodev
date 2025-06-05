#pragma once

#include "sai/target.h"

#ifndef SAI_TARGET
#error "SAI_TARGET must be defined using a value from target.h!"
#endif  // SAI_TARGET

#include "sai/macro.h"
#include "sai/input.h"
#include "sai/memmap.h"
#if SAI_TARGET==SAI_TARGET_MD
#include "sai/md/io.h"
#include "sai/md/irq.h"
#include "sai/md/pal.h"
#include "sai/md/vdp.h"
#include "sai/md/vdp_dma_queue.h"
#include "sai/md/vdp_spr.h"
#elif SAI_TARGET==SAI_TARGET_C2
#include "sai/c2/io.h"
#include "sai/c2/prot.h"
#include "sai/c2/s5296.h"
#include "sai/c2/upd7759.h"
#include "sai/md/irq.h"
#include "sai/md/pal.h"
#include "sai/md/vdp.h"
#include "sai/md/vdp_dma_queue.h"
#include "sai/md/vdp_spr.h"
#elif SAI_TARGET=SAI_TARGET_SYS16B
#error "TODO"
#elif SAI_TARGET=SAI_TARGET_SYS18
#error "TODO"
#elif SAI_TARGET=SAI_TARGET_CPS
#error "TODO"
#elif SAI_TARGET=SAI_TARGET_CPS2
#error "TODO"
#endif

#ifndef __ASSEMBLER__

// Called after C runtime initialization.
void sai_init(void);

// Call at the end of the main loop to handle
// * finalization of sprite list(s)
// * palette management / transfer preparation
// * frame synchronization
// * schedule transfers
// * input polling
void sai_finish(void);
#endif  // __ASSEMBLER__
