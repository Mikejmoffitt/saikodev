//
// Pal command structure used for machines with dedicated color RAM.
//

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

#define SAI_PAL_CMD_QUEUE_DEPTH 16

#define SAI_PAL_CMD_COPY_LINE_LONG 0x0000
#define SAI_PAL_CMD_COPY_LINE_HALF 0x4000
#define SAI_PAL_CMD_SET_COLOR      0x8000

#ifndef __ASSEMBLER__

typedef struct SaiPalCmd
{
	uint16_t op_cnt;
	union
	{
		struct
		{
			uint16_t color;
			uint16_t pad;
		};
		const uint16_t *src;
	};
	uint16_t *dest;
} SaiPalCmd;

extern uint16_t g_sai_pal_cmd_count;
extern SaiPalCmd g_sai_pal_cmd_queue[SAI_PAL_CMD_QUEUE_DEPTH];

void sai_palcmd_init(void);
void sai_palcmd_poll(void);

// Returns NULL if command couldn't be allocated.
static inline SaiPalCmd *sai_palcmd_add(void);

// Static implementations

static inline SaiPalCmd *sai_palcmd_add(void)
{
	if (g_sai_pal_cmd_count >= SAI_ARRAYSIZE(g_sai_pal_cmd_queue)) return NULL;
	SaiPalCmd *ret = &g_sai_pal_cmd_queue[g_sai_pal_cmd_count];
	g_sai_pal_cmd_count++;
	return ret;
}

#else

	.struct 0

SaiPalCmd.op_cnt: ds.w 1
SaiPalCmd.src:
SaiPalCmd.color:  ds.l 1
SaiPalCmd.dest:   ds.l 1
SaiPalCmd.len:

	.extern	sai_palcmd_init
	.extern	sai_palcmd_poll

#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
