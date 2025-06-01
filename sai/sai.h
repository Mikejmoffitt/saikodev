#pragma once

#include "sai/target.h"

#ifndef SAI_TARGET
#error "SAI_TARGET must be defined using a value from target.h!"
#endif  // SAI_TARGET

#include "sai/macro.h"
#include "sai/memmap.h"
#if SAI_TARGET==SAI_TARGET_MD
#include "sai/io/md.h"
#include "sai/irq/md.h"
#include "sai/pal/md.h"
#include "sai/video/vdp.h"
#include "sai/video/vdp_dma_queue.h"
#elif SAI_TARGET==SAI_TARGET_C2
#error "TODO"
#elif SAI_TARGET=SAI_TARGET_SYS18
#error "TODO"
#elif SAI_TARGET=SAI_TARGET_SYS16A
#error "TODO"
#elif SAI_TARGET=SAI_TARGET_SYS16B
#error "TODO"
#elif SAI_TARGET=SAI_TARGET_CPS2
#error "TODO"
#endif

#ifndef __ASSEMBLER__
// Call at the end of the main loop to handle
// * finalization of sprite list(s)
// * palette management / transfer preparation
// * frame synchronization
// * schedule transfers
// * input polling
void sai_finish(void);
#endif  // __ASSEMBLER__
