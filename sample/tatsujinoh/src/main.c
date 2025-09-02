#include "sai/sai.h"

static inline void print_string_gcu(uint16_t vram_addr, const uint16_t attr_base,
                                    const char *str)
{
	(void)vram_addr;
	(void)attr_base;
	(void)str;
/*	volatile uint16_t *gcu_addr = (volatile uint16_t *)(GCU_BASE+GCU_ADDR_OFFS);
	volatile uint16_t *gcu_data = (volatile uint16_t *)(GCU_BASE+GCU_DATA_OFFS);
	*gcu_addr = vram_addr;

	char val;
	while ((val = *str))
	{
		sai_gcu_wait_access();
		if (val >= ' ')
		{
			*gcu_data = (val-' ') | attr_base;
		}
		str++;
	}*/
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

	if (s_x + 32 >= GCU_RASTER_W)
	{
		s_dx = -1;
	}
	else if (s_x <= 0)
	{
		s_dx = 1;
	}
	if (s_y + 32 >= GCU_RASTER_H)
	{
		s_dy = -1;
	}
	else if (s_y <= 0)
	{
		s_dy = 1;
	}

//	sai_sp013_draw(s_x, s_y, SP013_AT32(1, 3)|SPR_ESPRADE_CODE, SPR_ESPRADE_SIZE);
}

static void draw_initial_text(void)
{
/*	const uint32_t attr = BG038_AT32(0, 3);
	print_string_gcu((GCU_VRAM_A_BASE + GCU_VRAM_OFFS(1, 22)),
	              attr, "Inputs");
	print_string_gcu((GCU_VRAM_A_BASE + GCU_VRAM_OFFS(1, 24)),
	              attr, "1: ");
	print_string_gcu((GCU_VRAM_A_BASE + GCU_VRAM_OFFS(1, 25)),
	              attr, "2: ");

	print_string_gcu((BG038_A_VRAM_BASE + BG038_VRAM_NT8),
	             attr, "Hello World!");

	print_string_gcu((GCU_VRAM_A_BASE + GCU_VRAM_OFFS(0, 1)),
	              attr, "Plane A   *");
	print_string_gcu((GCU_VRAM_B_BASE + GCU_VRAM_OFFS(0, 1)),
	              attr, "Plane  B  *");
	print_string_gcu((GCU_VRAM_C_BASE + GCU_VRAM_OFFS(0, 1)),
	              attr, "Plane   C *");

	print_string_gcu((BG038_A_VRAM_BASE + GCU_VRAM_OFFS(8, 8)),
	              BG038_AT32(0, 3), "High Priority");
	print_string_gcu((BG038_A_VRAM_BASE + GCU_VRAM_OFFS(9, 9)),
	              BG038_AT32(0, 2), "Low Priority");*/
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
	const uint16_t attr = GCU_ATTR(0, 3);

	static char s_input_str[] = ".... .... ....";

	set_input_str(g_sai_in[0].now, s_input_str);
	print_string_gcu(GCU_VRAM_A_BASE + GCU_VRAM_OFFS(2, 12),
	              attr, s_input_str);
	set_input_str(g_sai_in[1].now, s_input_str);
	print_string_gcu(GCU_VRAM_A_BASE + GCU_VRAM_OFFS(2, 13),
	              attr, s_input_str);
}

void __attribute__((noreturn)) main(void)
{
	sai_init();
//	sai_pal_load(0x01*16, &wrk_spr_pal[SPR_ESPRADE_PAL_OFFS], SPR_ESPRADE_PAL_LEN/16);
//	sai_pal_load(0x40*16, &wrk_bga_pal[BGA_FONT_PAL_OFFS], BGA_FONT_PAL_LEN/16);

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
