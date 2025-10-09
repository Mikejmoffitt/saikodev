//
// Saikodev Neo-Geo hardware definition
//

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

//
// Board Notes:
// * Introduces the PCG "text" layer independent of the GCU.
// * Has an external discrete logic line counter used to assist the text plane
//   access control; used instead of GCU status register for blanking checks.
// * No sub CPU, so all I/O is done directly by the CPU.
//

#define ROM_BASE               (0x000000)
#define WRAM_BASE              (0x100000)
#define WRAM_SIZE              (0x00F300)
#define SYSMEM_BASE            (0x10FD80)
#define CRAM_BASE              (0x400000)
#define CRAM_SIZE                (0x2000)
#define MEMCARD_BASE           (0x800000)
#define SYSROM_BASE            (0xC00000)
#define BRAM_BASE              (0xD00000)

#ifdef __cplusplus
}
#endif  // __cplusplus
