#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__
#include "sai/memmap.h"

// YMZ280B 68000 support code
// 2025 Mike Moffitt

// This struct us emitted by ymztool.

#ifndef __ASSEMBLER__

typedef struct YmzDat
{
	uint8_t key;  // also fn8
	uint8_t fn;
	uint8_t tl;
	uint8_t pan;
	uint8_t start_address[3];
	uint8_t loop_start_address[3];
	uint8_t loop_end_address[3];
	uint8_t end_address[3];
} YmzDat;

_Static_assert(sizeof(YmzDat) == 0x10);

#else

	.struct	0
YmzDat.fn8:
YmzDat.key:                ds.b 1
YmzDat.fn:                 ds.b 1
YmzDat.tl:                 ds.b 1
YmzDat.pan:                ds.b 1
YmzDat.start_address:      ds.b 3
YmzDat.loop_start_address: ds.b 3
YmzDat.loop_end_address:   ds.b 3
YmzDat.end_address:        ds.b 3
YmzDat.len:

#endif

// Offsets from YMZ_BASE.
#define YMZ_OFFS_CTRL 0x00
#define YMZ_OFFS_DATA 0x02

#ifndef __ASSEMBLER__

// Returns the status bits and acknowledges channel interrupts.
// Each bit corresponds to the channel(s) that triggered an interrupt.
static inline uint16_t sai_sai_ymz_status(void);

// Initializes the YMZ280B support code with the YmzDat blob.
void sai_ymz_init(const void *sai_ymz_data_blob);

// Resets all channels of the YMZ280B.
void sai_ymz_reset(void);

// Stops a channel.
void sai_ymz_stop(uint16_t channel);

// Stops all channels.
void sai_ymz_stop_all(void);

// Plays a sample. Loop info comes from the data blob.
void sai_ymz_play(uint16_t blob_offs, uint16_t channel);

// Plays a sample with forced looping.
void sai_ymz_play_loop(uint16_t blob_offs, uint16_t channel);

// Plays a sample, forced non-looping.
void sai_ymz_play_once(uint16_t blob_offs, uint16_t channel);

// -----------------------

static inline uint16_t sai_sai_ymz_status(void)
{
	volatile uint16_t *ctrl = (volatile uint16_t *)(YMZ_BASE + YMZ_OFFS_CTRL);
	return *ctrl;
}

#else

	.extern sai_ymz_init
	.extern sai_ymz_reset
	.extern sai_ymz_stop
	.extern sai_ymz_stop_all
	.extern sai_ymz_play
	.extern sai_ymz_play_loop

#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
