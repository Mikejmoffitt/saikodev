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
	sai_pal_set(((0x40+0x3F)*256), SAI_PAL888(0, s_val, s_val));
}

static void move_test_sprite(void)
{
	static int16_t s_dx = 1;
	static int16_t s_dy = 1;
	static int16_t s_x = 0;
	static int16_t s_y = 0;
	s_x += s_dx;
	s_y += s_dy;
	if (s_x + SPR_TIA_W >= 320)
	{
		sai_ymz_play(YMZ_BOUNCE_OFFS, 1);
		s_dx = -1;
	}
	else if (s_x <= 0)
	{
		sai_ymz_play(YMZ_BOUNCE_OFFS, 1);
		s_dx = 1;
	}
	if (s_y + SPR_TIA_H >= 240)
	{
		sai_ymz_play(YMZ_BOUNCE2_OFFS, 2);
		s_dy = -1;
	}
	else if (s_y <= 0)
	{
		sai_ymz_play(YMZ_BOUNCE2_OFFS, 2);
		s_dy = 1;
	}
	sai_sp013_draw(s_x, s_y, SP013_AT32(0, 3)|SPR_TIA_CODE, SPR_TIA_SIZE);
}

void __attribute__((noreturn)) main(void)
{
	sai_init();
	sai_ymz_init(wrk_ymz_dat);
	sai_pal_load(0, &wrk_spr_pal[SPR_TIA_PAL_OFFS], SPR_TIA_PAL_LEN/16);
	sai_pal_load(0x40*256, &wrk_bga_pal[BGA_FONT_PAL_OFFS], BGA_FONT_PAL_LEN/16);
	sai_pal_load(0x41*256, &wrk_bgb_pal[BGB_FONT_PAL_OFFS], BGB_FONT_PAL_LEN/16);
	sai_pal_load(0x42*256, &wrk_bgc_pal[BGC_FONT_PAL_OFFS], BGC_FONT_PAL_LEN/16);
	sai_ymz_play(YMZ_TRK1_OFFS, 0);
	while (true)
	{
		run_test_color_anim();
		move_test_sprite();
		sai_finish();
	}
}
