#pragma once



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

/* Macro used for calling subroutines without RAM in startup code. */
#ifdef __ASSEMBLER__

.macro	calla6	routine
	lea	(. + 12).l, a6
	jmp	(\routine).l
.endm

.macro	calla6_safe	routine
	move.l	a6, -(sp)
	calla6	\routine
	move.l	(sp)+, a6
.endm
#endif
