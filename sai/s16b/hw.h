//
// System 16B definitions. Also used by System 18.
//
// While 315-5195 exists to allow for flexibility with the memory map, Saikodev
// offers a single memory configuration that is initialized in the boot code.
//
// If you really want to change it up yourself, you may define
//    SAI_S16_CUSTOM_MAP
// and then provide the memory bases and sizes yourself.
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__

#define ROM_BASE               (0x000000)

// Region assignments
#define S16_REGION_ROM0        0
#define S16_REGION_ROM1        1
#define S16_REGION_ROM2        2
#define S16_REGION_WRAM        3
#define S16_REGION_VRAM        4
#define S16_REGION_SPR         5
#define S16_REGION_CRAM        6
#define S16_REGION_IO          7

// Base standard memory map set for the mapper
#define WRAM_BASE              (0xE00000)
#define WRAM_SIZE              (0x004000)
#define CRAM_BASE              (0x700000)

#define S16_ROM0_BASE          (0x000000)
#define S16_ROM1_BASE          (0x200000)
#define S16_ROM2_BASE          (0x400000)

#define S16_SPR_BASE           (0x600000)
#define S16_SPR_SIZE           (0x800)
#define S16_CRAM_BASE          (CRAM_BASE)
#define S16_CRAM_SIZE          (0x1000)
#define S16_VRAM_BASE          (0x800000)
#define S16_VRAM_SIZE          (0x10000)
#define S16_WRAM_BASE          (WRAM_BASE)
#define S16_WRAM_SIZE          (0x4000)
#define S16_IO_BASE            (0xC40000)

#define S16_MAPPER_BASE        (0xC00000)

#ifdef __cplusplus
}
#endif  // __cplusplus
