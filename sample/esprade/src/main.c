#include "sai/sai.h"
#include "res.h"

static inline void print_string(void *vram_addr, const uint32_t attr_base,
                                const char *str)
{
	volatile uint32_t *vrm = (volatile uint32_t *)vram_addr;
	char val;
	while ((val = *str))
	{
		if (val >= ' ')
		{
			*vrm = (val-' ') | attr_base;
		}
		vrm++;
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

	if (g_sai_in[0].now & SAI_BTN_C)
	{
		sai_sp013_draw_sc(s_x/8, s_y/8, SP013_AT32(1, 3)|SPR_TIA_CODE, SPR_TIA_SIZE, 0x20, 0x20);
	}
	if (g_sai_in[0].now & SAI_BTN_B)
	{
		sai_sp013_draw_sc(s_x/4, s_y/4, SP013_AT32(1, 3)|SPR_TIA_CODE, SPR_TIA_SIZE, 0x40, 0x40);
	}
	if (g_sai_in[0].now & SAI_BTN_A)
	{
		sai_sp013_draw_sc(s_x/2, s_y/2, SP013_AT32(1, 3)|SPR_TIA_CODE, SPR_TIA_SIZE, 0x80, 0x80);
	}
	sai_sp013_draw(s_x, s_y, SP013_AT32(1, 3)|SPR_TIA_CODE, SPR_TIA_SIZE);
}

static void draw_initial_text(void)
{
	const uint32_t attr = BG038_AT32(0, 3);
	print_string((void *)(BG038_C_VRAM_BASE + BG038_VRAM_NT8 + BG038_VRAM_OFFS8(1, 22)),
	              attr, "Inputs");
	print_string((void *)(BG038_C_VRAM_BASE + BG038_VRAM_NT8 + BG038_VRAM_OFFS8(1, 24)),
	              attr, "1: ");
	print_string((void *)(BG038_C_VRAM_BASE + BG038_VRAM_NT8 + BG038_VRAM_OFFS8(1, 25)),
	              attr, "2: ");

	print_string((void *)(BG038_A_VRAM_BASE + BG038_VRAM_NT8),
	             attr, "Hello World!");

	print_string((void *)(BG038_A_VRAM_BASE + BG038_VRAM_NT8 + BG038_VRAM_OFFS8(0, 1)),
	              attr, "Plane A   *");
	print_string((void *)(BG038_B_VRAM_BASE + BG038_VRAM_NT8 + BG038_VRAM_OFFS8(0, 1)),
	              attr, "Plane  B  *");
	print_string((void *)(BG038_C_VRAM_BASE + BG038_VRAM_NT8 + BG038_VRAM_OFFS8(0, 1)),
	              attr, "Plane   C *");

	print_string((void *)(BG038_A_VRAM_BASE + BG038_VRAM_NT8 + BG038_VRAM_OFFS8(8, 8)),
	              BG038_AT32(0, 3), "High Priority");
	print_string((void *)(BG038_A_VRAM_BASE + BG038_VRAM_NT8 + BG038_VRAM_OFFS8(9, 9)),
	              BG038_AT32(0, 2), "Low Priority");
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
	const uint32_t attr = BG038_AT32(0, 3);
	static char s_input_str[] = ".... .... ....";

	set_input_str(g_sai_in[0].now, s_input_str);
	print_string((void *)(BG038_C_VRAM_BASE + BG038_VRAM_NT8 + BG038_VRAM_OFFS8(4, 24)),
	              attr, s_input_str);
	set_input_str(g_sai_in[1].now, s_input_str);
	print_string((void *)(BG038_C_VRAM_BASE + BG038_VRAM_NT8 + BG038_VRAM_OFFS8(4, 25)),
	              attr, s_input_str);
}

void __attribute__((noreturn)) main(void)
{
	sai_init();
	sai_ymz_init(wrk_ymz_dat);
	sai_pal_load(0x01*16, &wrk_spr_pal[SPR_TIA_PAL_OFFS], SPR_TIA_PAL_LEN/16);
	sai_pal_load(0x40*16, &wrk_bga_pal[BGA_FONT_PAL_OFFS], BGA_FONT_PAL_LEN/16);

	// Set plane A to 8x8 for string display
	g_sai_bg038[0].flagy &= ~BG038_SCRY_TILE16;
	g_sai_bg038[1].flagy &= ~BG038_SCRY_TILE16;
	g_sai_bg038[2].flagy &= ~BG038_SCRY_TILE16;
	sai_bg038_set_disable(0, false);
	sai_bg038_set_disable(1, false);
	sai_bg038_set_disable(2, false);

	sai_finish();

	draw_initial_text();

	sai_ymz_play(YMZ_TRK1_OFFS, 0);
	while (true)
	{
		draw_inputs();
		run_test_color_anim();
		move_test_sprite();
		sai_finish();
	}
}
