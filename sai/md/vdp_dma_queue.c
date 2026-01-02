#include "sai/md/vdp_dma_queue.h"
#include "sai/md/vdp_regs.h"
#include "sai/md/vdp_spr.h"
#include "sai/md/vdp.h"
#include "sai/macro.h"

#define SAI_MD_VDP_DMA_QUEUE_DEPTH_DEFAULT 32

#define VDP_DMA_SRC_FILL 0x80
#define VDP_DMA_SRC_COPY 0xC0

#ifndef SAI_MD_VDP_DMA_QUEUE_DEPTH
#define SAI_MD_VDP_DMA_QUEUE_DEPTH SAI_MD_VDP_DMA_QUEUE_DEPTH_DEFAULT
#endif  // SAI_MD_VDP_DMA_QUEUE_DEPTH
// Used with modulo operator, so should be power of 2.
_Static_assert(SAI_NUM_IS_POW2(SAI_MD_VDP_DMA_QUEUE_DEPTH), "DMA queue depth != power of 2!");

// Struct representing pre-calculated register values for the VDP's DMA.
typedef struct DmaCmd DmaCmd;
struct DmaCmd
{
	uint16_t stride;
	uint16_t len_1;
	uint16_t len_2;
	uint16_t src_1;  // Used as data for DMA fill.
	uint16_t src_2;
	uint16_t src_3;
	uint32_t ctrl;
};

_Static_assert(sizeof(DmaCmd) == 0x10);

// DMA queue ring buffer.
static uint16_t s_dma_q_cnt;
static uint16_t s_dma_q_write_idx;
static uint16_t s_dma_q_read_idx;
static DmaCmd s_dma_q[SAI_MD_VDP_DMA_QUEUE_DEPTH];

static inline void process_single_command(DmaCmd *cmd)
{
	register void *a0 asm ("a0") = cmd;
	asm volatile ("bsr.w sai_vdp_dma_process_cmd"
	              :
	              : "a" (a0)
	              : "d0", "a1", "memory", "cc" );
}

static inline uint16_t next_read_idx(void)
{
	return (s_dma_q_read_idx + 1) % SAI_MD_VDP_DMA_QUEUE_DEPTH;
}

static inline uint16_t next_write_idx(void)
{
	return (s_dma_q_write_idx + 1) % SAI_MD_VDP_DMA_QUEUE_DEPTH;
}

void sai_vdp_dma_init(void)
{
	s_dma_q_read_idx = 0;
	s_dma_q_write_idx = 0;
	s_dma_q_cnt = 0;
}

// Calculate required register values for a transfer.
// If the queue cannot accept any more transfers, a flush begins.
static inline void enqueue_int(uint16_t op, uint32_t bus, uint32_t dest, uint32_t src, uint16_t n, uint16_t stride)
{
	// A command slot is chosen from one of the two queues, based on the type.
	DmaCmd *cmd;

	// If the DMA queue is full, force a flush.
	if (s_dma_q_cnt >= SAI_MD_VDP_DMA_QUEUE_DEPTH) sai_vdp_dma_flush();

	cmd = &s_dma_q[s_dma_q_write_idx];
	s_dma_q_write_idx = next_write_idx();

	// DMA register values are calculated ahead of time to be consumed during
	// VBlank faster.
	cmd->stride = VDP_REGST(VDP_AUTOINC, stride);
	cmd->len_1 = VDP_REGST(VDP_DMALEN1, n & 0xFF);
	cmd->len_2 = VDP_REGST(VDP_DMALEN2, (n >> 8) & 0xFF);

	switch (op)
	{
		case DMA_OP_NONE:
			return;

		case DMA_OP_TRANSFER:
			src = src >> 1;
			cmd->src_1 = VDP_REGST(VDP_DMASRC1, src & 0xFF);
			src = src >> 8;
			cmd->src_2 = VDP_REGST(VDP_DMASRC2, src & 0xFF);
			src = src >> 8;
			cmd->src_3 = VDP_REGST(VDP_DMASRC3, src & 0x7F);
			break;

		case DMA_OP_FILL:
			cmd->src_1 = (src & 0xFF) << 8;  // fill byte.
			cmd->src_3 = VDP_REGST(VDP_DMASRC3, VDP_DMA_SRC_FILL);
			break;

		case DMA_OP_COPY:
			cmd->src_1 = VDP_REGST(VDP_DMASRC1, src & 0xFF);
			cmd->src_2 = VDP_REGST(VDP_DMASRC2, (src >> 8) & 0xFF);
			cmd->src_3 = VDP_REGST(VDP_DMASRC3, VDP_DMA_SRC_COPY);
			break;
	}

	cmd->ctrl = VDP_CTRL_DMA_BIT | VDP_CTRL_ADDR(dest) | bus;

	s_dma_q_cnt++;
}

static inline void sai_vdp_dma_enqueue(uint16_t op, uint32_t bus, uint32_t dest, uint32_t src, uint16_t n, uint16_t stride)
{
	if (op == DMA_OP_TRANSFER)
	{
		// check that the source address + length won't cross a 128KIB boundary
		// based on SGDK's DMA validation.
		const uint32_t limit = 0x20000 - (src & 0x1FFFF);

		// If the transfer will cross the 128KiB boundary, transfer the latter
		// half first, then truncate the transfer's length to fill the rest.
		if (n > (limit >> 1))
		{
			enqueue_int(op, bus,
			            dest + limit,
			            src + limit,
			            n - (limit >> 1), stride);
			n = limit >> 1;
		}
	}
	enqueue_int(op, bus, dest, src, n, stride);
}

// Schedule a DMA for next vblank from 68K mem to VRAM
void sai_vdp_dma_transfer_vram(uint32_t dest, const void *src, uint16_t words, uint16_t stride)
{
	sai_vdp_dma_enqueue(DMA_OP_TRANSFER, VDP_CTRL_VRAM_WRITE,
	                    dest, (uint32_t)src, words, stride);
}

void sai_vdp_dma_transfer_cram(uint32_t dest, const void *src, uint16_t words, uint16_t stride)
{
	sai_vdp_dma_enqueue(DMA_OP_TRANSFER, VDP_CTRL_CRAM_WRITE,
	                    dest, (uint32_t)src, words, stride);
}

void sai_vdp_dma_transfer_vsram(uint32_t dest, const void *src, uint16_t words, uint16_t stride)
{
	sai_vdp_dma_enqueue(DMA_OP_TRANSFER, VDP_CTRL_VSRAM_WRITE,
	                    dest, (uint32_t)src, words, stride);
}

// Schedule a DMA for next vblank to fill specified bytes at dest with val.
void sai_vdp_dma_fill_vram(uint32_t dest, uint16_t val, uint16_t bytes, uint16_t stride)
{
	sai_vdp_dma_enqueue(DMA_OP_FILL, VDP_CTRL_VRAM_WRITE, dest, val, bytes, stride);
}

// Schedule a DMA for next vblank to copy specified bytes from VRAM src to VRAM dest.
void sai_vdp_dma_copy_vram(uint32_t dest, uint16_t src, uint16_t bytes, uint16_t stride)
{
	sai_vdp_dma_enqueue(DMA_OP_COPY, VDP_CTRL_VRAM_WRITE, dest, src, bytes, stride);
}

void sai_vdp_dma_flush(void)
{
	const bool hint_en = sai_vdp_set_hint_en(false);
	const bool vint_en = sai_vdp_set_vint_en(false);
	const bool thint_en = sai_vdp_set_thint_en(false);

	// Process all queued transfers.
	for (uint16_t i = 0; i < s_dma_q_cnt; i++)
	{
//	while (s_dma_q_read_idx != s_dma_q_write_idx)
//	{
		DmaCmd *cmd = &s_dma_q[s_dma_q_read_idx];
		process_single_command(cmd);
		s_dma_q_read_idx = next_read_idx();
	}

	s_dma_q_cnt = 0;

	sai_vdp_set_hint_en(hint_en);
	sai_vdp_set_vint_en(vint_en);
	sai_vdp_set_thint_en(thint_en);
}

void sai_vdp_dma_clear_plane(uint16_t plane)
{
	const uint32_t base = g_sai_vdp_ntbase[plane];
	const uint16_t bytes = sizeof(uint16_t) *
	                       sai_vdp_get_plane_w() *
	                       sai_vdp_get_plane_h();
	sai_vdp_dma_fill_vram(base, 0, bytes, 1);
}
