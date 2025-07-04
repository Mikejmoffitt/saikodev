// Saikodev high-level input.
#pragma once

#ifndef __ASSEMBLER__
#include <stdint.h>
#include <stdbool.h>
#endif  // __ASSEMBLER__
#include "sai/macro.h"
#include "sai/target.h"
#include "sai/input_bits.h"

// Number of players to poll for. This is respected by different input drivers
// in different ways, so the acceptable values vary.
#ifndef SAI_PLAYER_COUNT
#define SAI_PLAYER_COUNT 2
#endif  // SAI_PLAYER_COUNT

#ifndef __ASSEMBLER__

// Struct representing the state of a single player's input.
typedef struct SaiInput
{
	uint16_t now;
	uint16_t prev;
	uint16_t pos;
	uint16_t neg;
} SaiInput;

extern SaiInput g_sai_in[SAI_PLAYER_COUNT];

void sai_input_init(void);
void sai_input_pre_poll(void);
void sai_input_post_poll(void);

#else

	.extern	g_sai_in;

	.struct	0
SaiInput.now:	ds.w 1
SaiInput.prev:	ds.w 1
SaiInput.pos:	ds.w 1
SaiInput.neg:	ds.w 1
SaiInput.len:

	.extern	sai_min_input_init
	.extern	sai_input_init
	.extern	sai_input_update_edges

#endif  // __ASSEMBLER__
