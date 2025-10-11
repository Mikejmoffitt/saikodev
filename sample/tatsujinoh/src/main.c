#include "sai/sai.h"
#include "res.h"

static inline void print_string(uint16_t vram_offs, const uint16_t attr_base,
                                const char *str)
{
	sai_toa_text_wait_access();

	volatile uint16_t *vram = (volatile uint16_t *)(TEXT_ADDR(0, 0) + vram_offs);

	char val;
	while ((val = *str))
	{
		if (val >= ' ')
		{
			*vram = attr_base + (val-' ');
		}
		vram += TEXT_OFFS(1, 0)/sizeof(uint16_t);
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

	sai_pal_set(0, SAI_PAL888(0, s_val, s_val));
}

static void move_test_sprite(void)
{
	static int16_t s_dx = 1;
	static int16_t s_dy = 1;
	static int16_t s_x = 0;
	static int16_t s_y = 0;

	s_x += s_dx;
	s_y += s_dy;

	if (s_x + GCU_BIGSPRITE_SRC_TEX_W >= GCU_RASTER_W)
	{
		s_dx = -1;
	}
	else if (s_x <= 0)
	{
		s_dx = 1;
	}
	if (s_y + GCU_BIGSPRITE_SRC_TEX_H >= GCU_RASTER_H)
	{
		s_dy = -1;
	}
	else if (s_y <= 0)
	{
		s_dy = 1;
	}

	sai_gcu_spr_draw(GCU_AT32(1, 0) | GCU_BIGSPRITE_CODE,
	                 GCU_SPR_STATIC_OFFS_FIX+(s_x<<GCU_SPR_FIXPX_BITS),
	                 GCU_SPR_STATIC_OFFS_FIX+(s_y<<GCU_SPR_FIXPX_BITS),
	                 GCU_BIGSPRITE_SIZE);
}

static void draw_initial_text(void)
{
	const uint32_t attr = TEXT_ATTR(0, 1);
	print_string((TEXT_OFFS(1, 22)),
	              attr, "Inputs");
	print_string((TEXT_OFFS(1, 24)),
	              attr, "1: ");
	print_string((TEXT_OFFS(1, 25)),
	              attr, "2: ");

	print_string(TEXT_OFFS(0, 0),
	             attr, "Hello World!");
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
	const uint16_t attr = TEXT_ATTR(0, 1);

	static char s_input_str[] = ".... .... ....";

	set_input_str(g_sai_in[0].now, s_input_str);
	print_string(TEXT_OFFS(2, 24),
	             attr, s_input_str);
	set_input_str(g_sai_in[1].now, s_input_str);
	print_string(TEXT_OFFS(2, 25),
	             attr, s_input_str);
}

void __attribute__((noreturn)) main(void)
{
	sai_init();
	sai_pal_load(0x40+0x01, &wrk_txt_pal[TXT_FONT_PAL_OFFS], TXT_FONT_PAL_LEN/16);
	sai_pal_load(0x01, &wrk_gcu_pal[GCU_FILLER_PAL_OFFS], GCU_FILLER_PAL_LEN/16);
	sai_toa_text_load_chr(vel_get_wrk_txt_chr(TXT_FONT),
	                      TXT_FONT_CHR_BYTES,
	                      0);

	sai_finish();

	draw_initial_text();

	while (true)
	{
		draw_inputs();
		run_test_color_anim();
		move_test_sprite();
		sai_finish();
	}
}
