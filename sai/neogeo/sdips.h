//
// Saikodev Macros for defining Soft DIPs.
//

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__

// Struct for parsing Soft DIP data.
#ifndef __ASSEMBLER__

typedef struct SaiNeoSdipData
{
	char title[16];      // Game name, as seen in the menu.
	uint16_t timers[2];  // In BCD, mmss. 0xFFFF for unused.
	uint16_t counts[2];  // From 1 - 99 in binary. 0xFF for unused.
	uint8_t enums[10];   // Upper nybble: default value; Lower nybble: count.
	char strings[0];     // Variable length array of 12 byte strings.
} SaiNeoSdipData;

#else

	.struct	0
SaiNeoSdipData.title:          ds.b 16
SaiNeoSdipData.timers:         ds.w 2
SaiNeoSdipData.counts:         ds.w 2
SaiNeoSdipData.enums:          ds.b 10
SaiNeoSdipData.strings:

#endif  // __ASSEMBLER__

// Assembler macros 

#ifdef __cplusplus
}
#endif  // __cplusplus
