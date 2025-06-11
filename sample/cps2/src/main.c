#include "sai/sai.h"
#include "res.h"

static void run_test_color_anim(void)
{
	static int16_t s_val = 0;
	static int16_t s_delta = 4;
	s_val += s_delta;
	if (s_val >= 0x100)
	{
		s_val = 0xFF;
		s_delta = -4;
	}
	else if (s_val < 0)
	{
		s_val = 0;
		s_delta = 4;
	}
	sai_pal_set(16*32, 0xF000 | SAI_PAL888(0, s_val, s_val));
}

void __attribute__((noreturn)) main(void)
{
	sai_init();
	sai_pal_load(0, (void *)(wrk_gfx_pal+SPR_TIA_PAL_OFFS), 1);
	while (true)
	{
		run_test_color_anim();
		sai_finish();
	}
}
