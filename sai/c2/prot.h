//
// C/C2 Protection and Video Configuration CPLD.
//

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#include "sai/macro.h"

#define SYSC_PROT_OFFS_SECURITY 0x001
#define SYSC_PROT_OFFS_PALBANK  0x001
#define SYSC_PROT_OFFS_VCTRL    0x201

// 7654 3210
// .... .p.. MD palette compatibility mode
// .... ..v. Something video related? ought to be 1.
// .... ...b Screen blank
#define SYSC_PROT_VCTRL_PALMODE SAI_BITVAL(2)
#define SYSC_PROT_VCTRL_VID     SAI_BITVAL(1)
#define SYSC_PROT_VCTRL_BLANK   SAI_BITVAL(0)

#define SYSC_PROT_VCTRL_DEFAULT (SYSC_PROT_VCTRL_VID | SYSC_PROT_VCTRL_PALMODE)

#ifndef __ASSEMBLER__

extern uint16_t g_sai_sysc_vctrl;

void sai_sysc_vctrl_init(void);
void sai_sysc_vctrl_on_vbl(void);

static inline void sai_sysc_prot_set_pal_bank(uint16_t bg_bank, uint16_t spr_bank);

static inline void sai_sysc_vctrl_set_md_color_mode(bool en);
static inline void sai_sysc_vctrl_blank(bool en);

// --------------------

static inline void sai_sysc_prot_set_pal_bank(uint16_t bg_bank, uint16_t spr_bank)
{
	volatile uint8_t *prot = (volatile uint8_t *)(SYSC_PROT_OFFS_PALBANK);
	*prot = (spr_bank << 2) | bg_bank;
}

static inline void sai_sysc_vctrl_set_md_color_mode(bool en)
{
	if (en) g_sai_sysc_vctrl |= SYSC_PROT_VCTRL_PALMODE;
	else g_sai_sysc_vctrl &= ~(SYSC_PROT_VCTRL_PALMODE);
}

static inline void sai_sysc_vctrl_blank(bool en)
{
	if (en) g_sai_sysc_vctrl |= SYSC_PROT_VCTRL_BLANK;
	else g_sai_sysc_vctrl &= ~(SYSC_PROT_VCTRL_BLANK);
}

#else
	.extern	g_sai_sysc_vctrl

	.extern	sai_sysc_vctrl_init
	.extern	sai_min_sysc_vctrl_init
	.extern	sai_sysc_vctrl_on_vbl
#endif

#ifdef __cplusplus
}
#endif  // __cplusplus
