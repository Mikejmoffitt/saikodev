#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdint.h>
#include <stddef.h>
#endif  // __ASSEMBLER__
#include "sai/target.h"
#include "sai/macro.h"
#include "sai/memmap.h"
#include "sai/palcmd.h"

#define SAI_PAL444(r, g, b) (((b)>>4) | ((g)&0xF0) | (((r)<<4)&0xF00))
#define SAI_PAL555(r, g, b) (SAI_PAL444((r)>>1, (g)>>1, (b)>>1))
#define SAI_PAL888(r, g, b) (SAI_PAL444((r), (g), (b)))
#define SAI_PAL333(r, g, b) (SAI_PAL444((r)<<1, (g)<<1, (b)<<1))
#define SAI_PALHEX(x) SAI_PAL888((((x) >> 16) & 0xFF), (((x) >> 8) & 0xFF), ((x) & 0xFF))

#ifndef __ASSEMBLER__

// Clears palette and initializes queue system.
void sai_pal_init(void);

// Registers a command to set a single palette color.
static inline void sai_pal_set(uint16_t idx, uint16_t val);

// Schedules a transfer to palette data.
// dest:  palette line (0 - 31).
// src:   Pointer to data to copy from. Data must exist at least until next vbl.
// count: Number of palette lines to copy (1 = sixteen colors).
static inline void sai_pal_load(uint16_t dest, const void *src, uint16_t count);

// -----------------------------------------------------------------------------
// Static implementations
// -----------------------------------------------------------------------------

static inline void sai_pal_set(uint16_t idx, uint16_t val)
{
	SaiPalCmd *cmd = sai_palcmd_add();
	if (!cmd) return;
	cmd->op_cnt = SAI_PAL_CMD_SET_COLOR | (1-1);
	uint16_t *cram = (uint16_t *)CRAM_BASE;
	cmd->dest = &cram[idx];
	cmd->color = val;
}

static inline void sai_pal_load(uint16_t dest, const void *src, uint16_t count)
{
	SaiPalCmd *cmd = sai_palcmd_add();
	if (!cmd) return;
	cmd->op_cnt = SAI_PAL_CMD_COPY_LINE_LONG | (count-1);
	uint16_t *cram = (uint16_t *)CRAM_BASE;
	cmd->dest = &cram[dest*16];
	cmd->src = src;
}

#else

	.extern	sai_min_pal_init
	.extern	sai_pal_init

#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
