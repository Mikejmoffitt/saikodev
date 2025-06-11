//
// CPS2 sprites. Generally they follow the same sprite definition format and
// pull from the same ROMs.
//

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__

#include "sai/cps/obj.h"  // For definitions of the sprite structures.

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

#define SAI_CPS2_OBJ_PRIO_DEFAULT 0x1234
#define SAI_CPS2_OBJ_XOFF_DEFAULT 0x0040
#define SAI_CPS2_OBJ_YOFF_DEFAULT 0x0010

#define CPS2_OBJ_COUNT_MAX 1024

#ifndef __ASSEMBLER__

extern CpsObj *g_sai_cps_obj_next;
extern uint16_t g_sai_cps_obj_count;

void sai_cps2_obj_init(void);
void sai_cps2_obj_on_vbl(void);
void sai_cps2_obj_finish(void);
void sai_cps2_obj_on_vbl(void);
void sai_cps2_obj_reset(void);

// This interface intentionally mirrors the CPS version.
static inline CpsObj *sai_cps_obj_draw(int16_t x, int16_t y,
                                       uint16_t code, uint16_t sizeattr)
{
	if (g_sai_cps_obj_count >= CPS2_OBJ_COUNT_MAX) return --g_sai_cps_obj_next;
	CpsObj *obj = g_sai_cps_obj_next++;
	obj->x = x;
	obj->y = y;
	obj->code = code;
	obj->sizeattr = sizeattr;
	g_sai_cps_obj_count++;
	return obj;
}

#else
	.extern	g_sai_cps_obj_next
	.extern	g_sai_cps_obj_count

	.extern	sai_cps2_obj_init
	.extern	sai_cps2_obj_on_vbl
	.extern	sai_cps2_obj_finish
	.extern	sai_cps2_obj_on_vbl
	.extern	sai_cps2_obj_reset
#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
