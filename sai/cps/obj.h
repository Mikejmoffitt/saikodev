//
// Original CPS sprites. While CPS2 sprites operate through a different
// interface, the sprite structure is the same.
//
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

// Defines sprite block size, in tiles.
// Block sprites assume graphics are atlased on a 256px wide texture.
#define CPS_OBJ_SIZE(w, h) ((w-1)|((h-1)<<4))
#define CPS_OBJ_ATTR(pal)  (pal)
#define CPS_OBJ_AT16(pal, size)  (CPS_OBJ_ATTR(pal) | (size << 8))
#define CPS_OBJ_FLIPX_BIT  (5)
#define CPS_OBJ_FLIPY_BIT  (6)
#define CPS_OBJ_FLIPX      (SAI_BITVAL(CPS_OBJ_FLIPX_BIT))
#define CPS_OBJ_FLIPY      (SAI_BITVAL(CPS_OBJ_FLIPY_BIT))

#define CPS_OBJ_COUNT_MAX  256
#define CPS_OBJ_BANK_OFFS  0x8000

#ifndef __ASSEMBLER__
typedef struct CpsObj
{
	int16_t x;
	int16_t y;
	int16_t code;
	union
	{
		struct
		{
			uint8_t size;
			uint8_t attr;
		};
		uint16_t sizeattr;
	};
} CpsObj;

_Static_assert(sizeof(CpsObj) == 8);
#else
	.struct	0
CpsObj.x:		ds.w 1
CpsObj.y:		ds.w 1
CpsObj.code:	ds.w 1
CpsObj.sizeattr:
CpsObj.size:	ds.b 1
CpsObj.attr:	ds.b 1
CpsObj.len:

#endif  // __ASSEMBLER__

#ifndef __ASSEMBLER__

#if SAI_TARGET != SAI_TARGET_CPS2
extern CpsObj *g_sai_cps_obj_next;
extern uint16_t g_sai_cps_obj_count;
#endif  // SAI_TARGET

#if SAI_TARGET != SAI_TARGET_CPS2
void sai_cps_obj_init(void);
void sai_cps_obj_finish(void);
void sai_cps_obj_on_vbl(void);
void sai_cps_obj_reset(void);

static inline CpsObj *sai_cps_obj_draw(int16_t x, int16_t y,
                                       uint16_t code, uint16_t sizeattr)
{
	if (g_sai_cps_obj_count >= CPS_OBJ_COUNT_MAX) return --g_sai_cps_obj_next;
	CpsObj *obj = g_sai_cps_obj_next++;
	obj->x = x;
	obj->y = y;
	obj->code = code;
	obj->sizeattr = sizeattr;
	g_sai_cps_obj_count++;
	return obj;
}
#endif  // SAI_TARGET

#else

#if SAI_TARGET != SAI_TARGET_CPS2
	.extern	g_sai_cps_obj_next
	.extern	g_sai_cps_obj_count

	.extern	sai_cps_obj_init
	.extern	sai_min_cps_obj_init
	.extern	sai_cps_obj_finish
	.extern	sai_cps_obj_on_vbl
	.extern	sai_cps_obj_reset
#endif  // SAI_TARGET

#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
