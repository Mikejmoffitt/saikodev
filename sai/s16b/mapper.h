// Enormous thanks to Charles McDonald for documenting these registers.
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdint.h>
#include <stdbool.h>
#endif  // __ASSEMBLER__
#include "sai/memmap.h"
#include "sai/macro.h"
#include "sai/s16b/hw.h"

// Sound CPU command
#define S16_MAPPER_OFFS_SND              0x07

// Region control offsets
#define S16_MAPPER_OFFS_R0_CTRL          0x21
#define S16_MAPPER_OFFS_R0_ADDR          0x23
#define S16_MAPPER_OFFS_R1_CTRL          0x25
#define S16_MAPPER_OFFS_R1_ADDR          0x27
#define S16_MAPPER_OFFS_R2_CTRL          0x29
#define S16_MAPPER_OFFS_R2_ADDR          0x2B
#define S16_MAPPER_OFFS_R3_CTRL          0x2D
#define S16_MAPPER_OFFS_R3_ADDR          0x2F
#define S16_MAPPER_OFFS_R4_CTRL          0x31
#define S16_MAPPER_OFFS_R4_ADDR          0x33
#define S16_MAPPER_OFFS_R5_CTRL          0x35
#define S16_MAPPER_OFFS_R5_ADDR          0x37
#define S16_MAPPER_OFFS_R6_CTRL          0x39
#define S16_MAPPER_OFFS_R6_ADDR          0x3B
#define S16_MAPPER_OFFS_R7_CTRL          0x3D
#define S16_MAPPER_OFFS_R7_ADDR          0x3F

#ifdef __ASSEMBLER__
	.extern	sai_min_s16_mapper_init
#endif   // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
