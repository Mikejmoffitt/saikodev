#include "sai/sai.h"

void __attribute__((noreturn)) main(void)
{
	sai_vdp_set_vint_en(true);
	while (true)
	{
		sai_md_pad_poll();
		sai_vdp_wait_vbl();
		sai_vdp_dma_flush();
	}
}
