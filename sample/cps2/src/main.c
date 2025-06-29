#include "sai/sai.h"
#include "res.h"

static inline void print_string(void *vram_addr, const uint16_t attr,
                                const char *str)
{
	volatile uint32_t *vrm = (volatile uint32_t *)vram_addr;
	char val;
	while ((val = *str))
	{
		if (val >= ' ')
		{
			*vrm = ((BG8_FONT_CODE + (val-' '))<<16) | (attr);
		}
		vrm += SAI_CPS_SCROLL1_OFFS(1, 0)/sizeof(*vrm);
		str++;
	}
}

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

	if (s_x + 256 >= 384)
	{
		s_dx = -1;
	}
	else if (s_x <= 0)
	{
		s_dx = 1;
	}
	if (s_y + 64 >= 224)
	{
		s_dy = -1;
	}
	else if (s_y <= 0)
	{
		s_dy = 1;
	}

	sai_cps_obj_draw(s_x+64, s_y+16, SPR_CPS_CODE+0, CPS_OBJ_AT16(0, CPS_OBJ_SIZE(8, 4)));
	sai_cps_obj_draw(s_x+192, s_y+16, SPR_CPS_CODE+8, CPS_OBJ_AT16(0, CPS_OBJ_SIZE(8, 4)));
}

static void draw_initial_text(void)
{
	const uint32_t attr = SAI_CPS_BG_ATTR(0, 0);
	print_string((void *)(VRAM_SCROLL1 + SAI_CPS_SCROLL1_OFFS(1, 23)),
	              attr, "Inputs");
	print_string((void *)(VRAM_SCROLL1 + SAI_CPS_SCROLL1_OFFS(1, 25)),
	              attr, "1: ");
	print_string((void *)(VRAM_SCROLL1 + SAI_CPS_SCROLL1_OFFS(1, 26)),
	              attr, "2: ");

	print_string((void *)(VRAM_SCROLL1), attr, "Hello CPS World!");
}

static inline void set_input_str(uint16_t in, char *str)
{
	str[0] = in & SAI_BTN_UP ? 'U' : '.';
	str[1] = in & SAI_BTN_DOWN ? 'D' : '.';
	str[2] = in & SAI_BTN_LEFT ? 'L' : '.';
	str[3] = in & SAI_BTN_RIGHT ? 'R' : '.';
	str[5] = in & SAI_BTN_A ? 'A' : '.';
	str[6] = in & SAI_BTN_B ? 'B' : '.';
	str[7] = in & SAI_BTN_C ? 'C' : '.';
	str[8] = in & SAI_BTN_D ? 'D' : '.';
	str[10] = in & SAI_BTN_START ? 'S' : '.';
	str[11] = in & SAI_BTN_COIN ? 'c' : '.';
	str[12] = in & SAI_BTN_TEST ? 't' : '.';
	str[13] = in & SAI_BTN_SERVICE ? 's' : '.';
}

static void draw_inputs(void)
{
	const uint32_t attr = SAI_CPS_BG_ATTR(0, 0);
	static char s_input_str[] = ".... .... ....";

	set_input_str(g_sai_in[0].now, s_input_str);
	print_string((void *)(VRAM_SCROLL1 + SAI_CPS_SCROLL1_OFFS(4, 25)),
	              attr, s_input_str);
	set_input_str(g_sai_in[1].now, s_input_str);
	print_string((void *)(VRAM_SCROLL1 + SAI_CPS_SCROLL1_OFFS(4, 26)),
	              attr, s_input_str);
}

void __attribute__((noreturn)) main(void)
{
	sai_init();
	sai_pal_load(0, (void *)(wrk_gfx_pal+SPR_CPS_PAL_OFFS), 1);
	sai_pal_load(32, (void *)(wrk_gfx_pal+BG8_FONT_PAL_OFFS), 1);
	sai_pal_load(64, (void *)(wrk_gfx_pal+BG8_FONT_PAL_OFFS), 1);
	sai_pal_load(96, (void *)(wrk_gfx_pal+BG8_FONT_PAL_OFFS), 1);
	draw_initial_text();

	while (true)
	{
		draw_inputs();
		run_test_color_anim();
		move_test_sprite();
		sai_finish();
	}
}
