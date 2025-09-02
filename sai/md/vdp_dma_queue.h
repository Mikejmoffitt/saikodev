// Saikodev Sega 315-5313 / YM7101 DMA functions
// Michael Moffitt 2018-2025
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdbool.h>
#include <stdint.h>
#endif  // __ASSEMBLER__

#include "sai/md/vdp.h"
#include "sai/memmap.h"
//
// Compiler flags:
//
// * SAI_MD_VDP_DMA_QUEUE_DEPTH
//
// The default value is 16, but you may override this value by defining it.
//

#ifndef __ASSEMBLER__
void sai_vdp_dma_init(void);

// Schedule a DMA for next vblank from 68K mem to VRAM
void sai_vdp_dma_transfer_vram(uint32_t dest, const void *src, uint16_t words, uint16_t stride);
void sai_vdp_dma_transfer_cram(uint32_t dest, const void *src, uint16_t words, uint16_t stride);
void sai_vdp_dma_transfer_vsram(uint32_t dest, const void *src, uint16_t words, uint16_t stride);
// Special high-priority DMA for sprite tables. Schedules transfers that will
// run before any others.
void sai_vdp_dma_transfer_spr_vram(uint16_t count);

// Schedule a DMA for next vblank to fill n words at dest with val.
void sai_vdp_dma_fill_vram(uint32_t dest, uint16_t val, uint16_t bytes, uint16_t stride);

// Schedule a DMA for next vblank to copy n words from VRAM src to VRAM dest.
void sai_vdp_dma_copy_vram(uint32_t dest, uint16_t src, uint16_t bytes, uint16_t stride);

// Execute DMA commands sitting in the queue.
void sai_vdp_dma_flush(void);

// Clears the specified plane to 0 by way of DMA fill.
void sai_vdp_dma_clear_plane(uint16_t plane);

#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
