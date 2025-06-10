#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__

// Address of sprite table >> 8 for use with DMA.
#define SAI_CPS2_OBJ_OFFS_BASE 0x00
// Generally $807D, $808E when flipped) 0 and 1 may be mirror bits? Progear doesn't write the upper $8000 until later in the init.
#define SAI_CPS2_OBJ_OFFS_UNK1 0x02
#define SAI_CPS2_OBJ_OFFS_PRIO 0x04
// Usually $0000, $1101 in SSF2, $0001 in 19XX. Maybe sign extension related
#define SAI_CPS2_OBJ_OFFS_UNK2 0x06
// Typically $0040
#define SAI_CPS2_OBJ_OFFS_XOFF 0x08
// Generally $0010
#define SAI_CPS2_OBJ_OFFS_YOFF 0x0A

#ifndef __ASSEMBLER__
void sai_cps2_obj_init(void);
void sai_cps2_obj_on_vbl(void);

#else
	.extern	sai_cps2_obj_init
	.extern	sai_min_cps2_obj_init
	.extern	sai_cps2_obj_on_vbl
#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
