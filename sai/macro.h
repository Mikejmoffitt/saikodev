#pragma once

//
// Bilingual enums, with strict limits
//
#ifdef __ASSEMBLER__
#define SAI_ENUMSTART .struct 0
#else
#define SAI_ENUMSTART enum{
#endif  // __ASSEMBLER__

#ifdef __ASSEMBLER__
#define SAI_ENUMNEXT(name) name##: ds.b 1
#else
#define SAI_ENUMNEXT(name) name ,
#endif  // __ASSEMBLER__

#ifdef __ASSEMBLER__
#define SAI_ENUMEND  // end of enum
#else
#define SAI_ENUMEND };
#endif



#define SAI_NUM_IS_POW2(x) ((x & (x - 1)) == 0)

#define SAI_ARRAYSIZE(x) (sizeof(x) / sizeof(x[0]))

#define SAI_BITVAL(x) (1 << x)
#define SAI_BTST(x, n) ((x) & (SAI_BITVAL(n)))
#define SAI_BCLR(x, n) ((x) &= (~(SAI_BITVAL(n))))
#define SAI_BSET(x, n) ((x) |= (SAI_BITVAL(n)))

#define SAI_MAX(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
       _a > _b ? _a : _b; })

#define SAI_MIN(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
       _a < _b ? _a : _b; })

#define SAI_BARRIER() __asm__ __volatile__("": : :"memory")

/* Macro used for calling subroutines without RAM in startup code. */
#ifdef __ASSEMBLER__

.macro	calla6	routine
	lea	(. + 12).l, a6
	jmp	(\routine).l
.endm

.macro	calla1	routine
	lea	(. + 12).l, a1
	jmp	(\routine).l
.endm

.macro	calla6_safe	routine
	move.l	a6, -(sp)
	calla6	\routine
	move.l	(sp)+, a6
.endm
#endif
