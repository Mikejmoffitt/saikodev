#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#include "sai/target.h"

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__

// TODO: other GCU-based boards

#if SAI_TARGET == SAI_TARGET_TATSUJIN_OH

//
// Board Notes:
// * Introduces the PCG "text" layer independent of the GCU.
// * Has an external discrete logic line counter used to assist the text plane
//   access control; used instead of GCU status register for blanking checks.
// * No sub CPU, so all I/O is done directly by the CPU.
//

#define ROM_BASE               (0x000000)
#define WRAM_BASE              (0x100000)
#define WRAM_SIZE              (0x010000)
#define GCU_BASE               (0x200000)
#define CRAM_BASE              (0x300000)
#define CRAM_SIZE                (0x1000)
#define TVRAM_BASE             (0x400000)
#define TVRAM_SIZE               (0x2000)
#define TOA_V_LINE             (0x600000)
#define TOA_IO_BASE            (0x700000)

#endif  // SAI_TARGET

#ifdef __cplusplus
}
#endif  // __cplusplus
