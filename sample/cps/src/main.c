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
	sai_pal_set(SAI_CPS_BG_PAL, 0xF000 | SAI_PAL888(0, s_val, s_val));
}

void move_test_sprite(void)
{
	static int16_t s_dx = 1;
	static int16_t s_dy = 1;
	static int16_t s_x = 0;
	static int16_t s_y = 0;
	s_x += s_dx;
	s_y += s_dy;

	if (s_x + SPR_TIA_W >= 320)
	{
		s_dx = -1;
	}
	else if (s_x <= 0)
	{
		s_dx = 1;
	}
	if (s_y + SPR_TIA_H >= 240)
	{
		s_dy = -1;
	}
	else if (s_y <= 0)
	{
		s_dy = 1;
	}

	sai_cps_obj_draw(s_x, s_y, SPR_TIA_CODE, CPS_OBJ_AT16(0, SPR_TIA_SIZE));
	sai_cps_obj_draw(s_x+128, s_y, SPR_TIA_CODE, CPS_OBJ_AT16(0, SPR_TIA_SIZE));
	sai_cps_obj_draw(s_x+128, s_y+128, SPR_TIA_CODE, CPS_OBJ_AT16(0, SPR_TIA_SIZE));
	sai_cps_obj_draw(s_x+128, s_y+128, SPR_TIA_CODE, CPS_OBJ_AT16(0, SPR_TIA_SIZE));
}

void __attribute__((noreturn)) main(void)
{
	sai_init();
	sai_pal_fill(0, 0xF000 | SAI_PAL888(0xFF, 0x00, 0x00), 16*32*8);
	sai_pal_load(0, (void *)(wrk_spr_pal+SPR_TIA_PAL_OFFS), 1);
	while (true)
	{
		run_test_color_anim();
		move_test_sprite();
		sai_finish();
	}
}
