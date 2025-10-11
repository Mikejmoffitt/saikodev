//
// "Text" PCG layer.
//
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#include "sai/target.h"
#include "sai/toa/hw.h"

// Nametable (layout)
#define TVRAM_NT_OFFS    (0x000000)
// Line select
#define TVRAM_VADDR_OFFS (0x002000)
// Horizontal scroll
#define TVRAM_HADDR_OFFS (0x003000)
// Character graphics data (tiles)
#define TVRAM_CHR_OFFS   (0x100000)

//
// Attributes and address math
//

// Text layer interaction macros
#define TEXT_OFFS(x, y)     (2*((x)+(0x40*(y))))
#define TEXT_ADDR(x, y)     (TVRAM_BASE+TEXT_OFFS((x),(y)))
#define TEXT_ATTR(chr, pal) (((pal)<<10)|(chr))

// Access check
#ifdef __ASSEMBLER__

	.macro	TEXT_WAIT_ACCESS trashreg=d0
0:
	move.w	TOA_V_LINE, \trashreg
	bmi.b	1f
	move.w	TOA_V_LINE, \trashreg
1:
	addq.w	#1, \trashreg
	cmpi.b	#0xF1, \trashreg
	bcs.b	0b
	.endm

	.macro	TEXT_WAIT_VDISP trashreg=d0
0:
	move.w	TOA_V_LINE, \trashreg
	bmi.b	1f
	move.w	TOA_V_LINE, \trashreg
1:
	cmpi.b	#0xEF, \trashreg
	bcc.b	0b
	.endm

#else

static inline void sai_toa_text_wait_access(void)
{
	asm volatile ("jsr sai_toa_text_wait_access_asm" : : : "d0", "cc");
}

#endif  // __ASSEMBLER__

#ifndef __ASSEMBLER__
extern uint8_t g_sai_toa_text_bank;
void sai_toa_text_init(void);
void sai_toa_text_on_vbl(void);
void sai_toa_text_wait_access(void);
void sai_toa_text_load_chr(const uint16_t *chr, uint16_t bytes,
                           uint32_t tile_index);


#else
	.extern	sai_toa_text_init
	.extern	g_sai_toa_text_bank
	.extern	sai_toa_text_wait_access
	.extern	sai_toa_text_on_vbl
	.extern	sai_toa_text_load_chr;
#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
