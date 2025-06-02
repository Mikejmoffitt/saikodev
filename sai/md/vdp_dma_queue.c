#include "sai/md/vdp_dma_queue.h"
#include "sai/md/vdp.h"
#include "sai/macro.h"

#define VDP_CTRL_DMA_BIT     0x00000080
#define VDP_CTRL_VRAM_READ   0x00000000
#define VDP_CTRL_VRAM_WRITE  0x40000000
#define VDP_CTRL_VSRAM_READ  0x00000010
#define VDP_CTRL_VSRAM_WRITE 0x40000010
#define VDP_CTRL_CRAM_READ   0x00000020
#define VDP_CTRL_CRAM_WRITE  0xC0000000

#define VDP_DMA_SRC_FILL 0x80
#define VDP_DMA_SRC_COPY 0xC0

#define DMA_QUEUE_PRIO_DEPTH 8
#define DMA_QUEUE_DEPTH 64
// Used with modulo operator, so should be power of 2.
_Static_assert(SAI_NUM_IS_POW2(DMA_QUEUE_DEPTH), "DMA queue depth != power of 2!");

typedef enum DmaOp DmaOp;
enum DmaOp
{
	DMA_OP_NONE,
	DMA_OP_TRANSFER,
	DMA_OP_SPR_TRANSFER,
	DMA_OP_COPY,
	DMA_OP_FILL
};

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

// Special high priority sprite list(s) queue.
static uint16_t s_dma_prio_q_idx;
static DmaCmd s_dma_prio_q_cmd[DMA_QUEUE_PRIO_DEPTH];
// DMA queue ring buffer.
static uint16_t s_dma_q_write_idx;
static uint16_t s_dma_q_read_idx;
static DmaCmd s_dma_q[DMA_QUEUE_DEPTH];

void sai_vdp_dma_init(void)
{
	s_dma_q_read_idx = 0;
	s_dma_q_write_idx = 0;
	s_dma_prio_q_idx = 0;
}

// Calculate required register values for a transfer
static inline void enqueue_int(DmaOp op, uint32_t bus, uint16_t dest, uint32_t src, uint16_t n, uint16_t stride)
{
	// A command slot is chosen from one of the two queues, based on the type.
	DmaCmd *cmd;
	if (op == DMA_OP_SPR_TRANSFER)
	{
		if (s_dma_prio_q_idx >= SAI_ARRAYSIZE(s_dma_prio_q_cmd)) return;
		cmd = &s_dma_prio_q_cmd[s_dma_prio_q_idx];
		s_dma_prio_q_idx++;
	}
	else
	{
		cmd = &s_dma_q[s_dma_q_write_idx];
		s_dma_q_write_idx = (s_dma_q_write_idx + 1) %
		                     SAI_ARRAYSIZE(s_dma_q);
		if (s_dma_q_write_idx == s_dma_q_read_idx) return;
	}

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
		case DMA_OP_SPR_TRANSFER:
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
}

static inline void sai_vdp_dma_enqueue(DmaOp op, uint32_t bus, uint16_t dest, uint32_t src, uint16_t n, uint16_t stride)
{
	if (op != DMA_OP_TRANSFER && op != DMA_OP_SPR_TRANSFER)
	{
		enqueue_int(op, bus, dest, src, n, stride);
		return;
	}
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
	enqueue_int(op, bus, dest, src, n, stride);
}

// Schedule a DMA for next vblank from 68K mem to VRAM
void sai_vdp_dma_transfer_vram(uint16_t dest, const void *src, uint16_t words, uint16_t stride)
{
	sai_vdp_dma_enqueue(DMA_OP_TRANSFER, VDP_CTRL_VRAM_WRITE,
	                    dest, (uint32_t)src, words, stride);
}

void sai_vdp_dma_transfer_cram(uint16_t dest, const void *src, uint16_t words, uint16_t stride)
{
	sai_vdp_dma_enqueue(DMA_OP_TRANSFER, VDP_CTRL_CRAM_WRITE,
	                    dest, (uint32_t)src, words, stride);
}

void sai_vdp_dma_transfer_vsram(uint16_t dest, const void *src, uint16_t words, uint16_t stride)
{
	sai_vdp_dma_enqueue(DMA_OP_TRANSFER, VDP_CTRL_VSRAM_WRITE,
	                    dest, (uint32_t)src, words, stride);
}

void sai_vdp_dma_transfer_spr_vram(uint16_t dest, const void *src, uint16_t words, uint16_t stride)
{
	sai_vdp_dma_enqueue(DMA_OP_SPR_TRANSFER, VDP_CTRL_VRAM_WRITE,
	                    dest, (uint32_t)src, words, stride);
}

// Schedule a DMA for next vblank to fill specified bytes at dest with val.
void sai_vdp_dma_fill_vram(uint16_t dest, uint16_t val, uint16_t bytes, uint16_t stride)
{
	sai_vdp_dma_enqueue(DMA_OP_FILL, VDP_CTRL_VRAM_WRITE, dest, val, bytes, stride);
}

// Schedule a DMA for next vblank to copy specified bytes from VRAM src to VRAM dest.
void sai_vdp_dma_copy_vram(uint16_t dest, uint16_t src, uint16_t bytes, uint16_t stride)
{
	sai_vdp_dma_enqueue(DMA_OP_COPY, VDP_CTRL_VRAM_WRITE, dest, src, bytes, stride);
}

void sai_vdp_dma_process_cmd(DmaCmd *cmd);  // dma_process.a68

void sai_vdp_dma_flush(void)
{
	const bool hint_en = sai_vdp_set_hint_en(false);
	const bool vint_en = sai_vdp_set_vint_en(false);
	const bool thint_en = sai_vdp_set_thint_en(false);

	// Process high-priority slots first.
	for (uint16_t i = 0; i < SAI_ARRAYSIZE(s_dma_prio_q_cmd); i++)
	{
		if (s_dma_prio_q_cmd[i].stride == 0) break;
		register void *a0 asm ("a0") = &s_dma_prio_q_cmd[i];
		asm volatile ("bsr.w sai_vdp_dma_process_cmd"
		              :
		              : "a" (a0)
		              :  "d0", "a1", "memory", "cc" );
	}
	s_dma_prio_q_idx = 0;

	// Process all queued transfers.
	while (s_dma_q_read_idx != s_dma_q_write_idx)
	{
		DmaCmd *cmd = &s_dma_q[s_dma_q_read_idx];
		register void *a0 asm ("a0") = cmd;
		asm volatile ("bsr.w sai_vdp_dma_process_cmd"
		              :
		              : "a" (a0)
		              :  "d0", "a1", "memory", "cc" );
		s_dma_q_read_idx = (s_dma_q_read_idx + 1) % DMA_QUEUE_DEPTH;
	}

	sai_vdp_set_hint_en(hint_en);
	sai_vdp_set_vint_en(vint_en);
	sai_vdp_set_thint_en(thint_en);
}
