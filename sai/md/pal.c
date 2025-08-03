#include "sai/md/pal.h"
#include "sai/md/vdp_dma_queue.h"
#include <string.h>

uint16_t g_sai_pal[16 * 4];
uint16_t g_sai_pal_dirty = SAI_PAL_DIRTY_MASK_FULL;

// Megadrive implementation, using DMA
void sai_pal_poll(void)
{
	// The g_sai_pal_dirty bitfield is broken down case by case here because
	// consecutive palette lines can be uploaded in one DMA transfer.
	switch (g_sai_pal_dirty & 0xF)
	{
		case 0x0:  // ....
			return;

		case 0x1:  // 0...
			sai_vdp_dma_transfer_cram(0, &g_sai_pal[0], 16, 2);
			break;

		case 0x2:  // .1..
			sai_vdp_dma_transfer_cram(32, &g_sai_pal[16], 16, 2);
			break;

		case 0x3:  // 01..
			sai_vdp_dma_transfer_cram(0, &g_sai_pal[0], 32, 2);
			break;

		case 0x4:  // ..2.
			sai_vdp_dma_transfer_cram(64, &g_sai_pal[32], 16, 2);
			break;

		case 0x5:  // 0.2.
			sai_vdp_dma_transfer_cram(0, &g_sai_pal[0], 16, 2);
			sai_vdp_dma_transfer_cram(64, &g_sai_pal[32], 16, 2);
			break;

		case 0x6:  // .12.
			sai_vdp_dma_transfer_cram(32, &g_sai_pal[16], 16, 2);
			sai_vdp_dma_transfer_cram(64, &g_sai_pal[32], 16, 2);
			break;

		case 0x7:  // 012.
			sai_vdp_dma_transfer_cram(0, &g_sai_pal[0], 48, 2);
			break;

		case 0x8:  // ...3
			sai_vdp_dma_transfer_cram(96, &g_sai_pal[48], 16, 2);
			break;

		case 0x9:  // 0..3
			sai_vdp_dma_transfer_cram(96, &g_sai_pal[48], 16, 2);
			sai_vdp_dma_transfer_cram(0, &g_sai_pal[0], 16, 2);
			break;

		case 0xA:  // .1.3
			sai_vdp_dma_transfer_cram(96, &g_sai_pal[48], 16, 2);
			sai_vdp_dma_transfer_cram(32, &g_sai_pal[16], 16, 2);
			break;

		case 0xB:  // 01.3
			sai_vdp_dma_transfer_cram(96, &g_sai_pal[48], 16, 2);
			sai_vdp_dma_transfer_cram(0, &g_sai_pal[0], 32, 2);
			break;

		case 0xC:  // ..23
			sai_vdp_dma_transfer_cram(64, &g_sai_pal[48], 32, 2);
			break;

		case 0xD:  // 0.23
			sai_vdp_dma_transfer_cram(64, &g_sai_pal[48], 32, 2);
			sai_vdp_dma_transfer_cram(0, &g_sai_pal[0], 16, 2);
			break;

		case 0xE:  // .123
			sai_vdp_dma_transfer_cram(32, &g_sai_pal[48], 48, 2);
			break;

		case 0xF:  // 0123
			sai_vdp_dma_transfer_cram(0, &g_sai_pal[0], 64, 2);
			break;
	}
	g_sai_pal_dirty = 0;
}

void sai_pal_init(void)
{
	memset(g_sai_pal, 0, sizeof(g_sai_pal));
	g_sai_pal_dirty = 0x000F;
}
