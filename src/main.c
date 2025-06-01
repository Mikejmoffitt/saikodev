#include "sai.h"

void main(void) __attribute__((noreturn))
{
	while (true)
	{
		sai_md_pad_poll();
		sai_vdp_wait_vbl();
		sai_vdp_dma_flush();
	}
}
