#include "sai/sai.h"
#include "res.h"

#define TEST_BG_VRAM_FONT_ADDR 0x0000
#define TEST_SPR_VRAM_ADDR (TEST_BG_VRAM_FONT_ADDR+BG_FONT_CHR_BYTES)

#define TEST_FONT_PAL 0
#define TEST_SPR_PAL 1

static inline void print_string(uint32_t vram_addr, const uint16_t attr_base,
                                const char *str)
{
	sai_vdp_set_autoinc(2);
	sai_vdp_set_addr_vramw(vram_addr);

	char val;
	while ((val = *str))
	{
		if (val >= ' ')
		{
			sai_vdp_write_word((val-' ') + attr_base);
		}
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

	if (s_x + SPR_MD_SRC_TEX_W >= 320)
	{
		s_dx = -1;
	}
	else if (s_x <= 0)
	{
		s_dx = 1;
	}
	if (s_y + SPR_MD_SRC_TEX_H >= 224)
	{
		s_dy = -1;
	}
	else if (s_y <= 0)
	{
		s_dy = 1;
	}

	const uint16_t attr_base = VDP_ATTR(VDP_TILE(TEST_SPR_VRAM_ADDR),
	                           /*hflip=*/false,
	                           /*vflip=*/false,
	                           TEST_SPR_PAL,
	                           /*prio=*/false);
#ifndef SAI_MD_VDP_SPR_DIRECT
	sai_vdp_spr_draw(s_x+128,          s_y+128,          attr_base+(SPR_MD_FRAME_OFFS*0), SPR_MD_SIZE);
	sai_vdp_spr_draw(s_x+128+SPR_MD_W, s_y+128,          attr_base+(SPR_MD_FRAME_OFFS*1), SPR_MD_SIZE);
	sai_vdp_spr_draw(s_x+128,          s_y+128+SPR_MD_H, attr_base+(SPR_MD_FRAME_OFFS*2), SPR_MD_SIZE);
	sai_vdp_spr_draw(s_x+128+SPR_MD_W, s_y+128+SPR_MD_H, attr_base+(SPR_MD_FRAME_OFFS*3), SPR_MD_SIZE);
#else
	g_sai_vdp_spr[0].x = s_x+128;
	g_sai_vdp_spr[0].y = s_y+128;
	g_sai_vdp_spr[0].attr = attr_base+(SPR_MD_FRAME_OFFS*0);
	g_sai_vdp_spr[0].size = SPR_MD_SIZE;
	g_sai_vdp_spr[0].link = 1;
	g_sai_vdp_spr[1].x = s_x+128+SPR_MD_W;
	g_sai_vdp_spr[1].y = s_y+128;
	g_sai_vdp_spr[1].attr = attr_base+(SPR_MD_FRAME_OFFS*1);
	g_sai_vdp_spr[1].size = SPR_MD_SIZE;
	g_sai_vdp_spr[1].link = 2;
	g_sai_vdp_spr[2].x = s_x+128;
	g_sai_vdp_spr[2].y = s_y+128+SPR_MD_H;
	g_sai_vdp_spr[2].attr = attr_base+(SPR_MD_FRAME_OFFS*2);
	g_sai_vdp_spr[2].size = SPR_MD_SIZE;
	g_sai_vdp_spr[2].link = 3;
	g_sai_vdp_spr[3].x = s_x+128+SPR_MD_W;
	g_sai_vdp_spr[3].y = s_y+128+SPR_MD_H;
	g_sai_vdp_spr[3].attr = attr_base+(SPR_MD_FRAME_OFFS*3);
	g_sai_vdp_spr[3].size = SPR_MD_SIZE;
	g_sai_vdp_spr[3].link = 0;
	g_sai_vdp_spr_count = 4;
#endif  // SAI_MD_VDP_SPR_DIRECT
}

static void draw_initial_text(void)
{
	const uint16_t attr = VDP_ATTR(VDP_TILE(TEST_BG_VRAM_FONT_ADDR),
	                      /*hflip=*/false,
	                      /*vflip=*/false,
	                      TEST_FONT_PAL,
	                      /*prio=*/false);

	print_string(sai_vdp_calc_plane_addr(VDP_PLANE_A, 0, 0),
	             attr, "Hello World!");

	print_string(sai_vdp_calc_plane_addr(VDP_PLANE_A, 1, 22),
	             attr, "Inputs");
	print_string(sai_vdp_calc_plane_addr(VDP_PLANE_A, 1, 24),
	             attr, "1: ");
	print_string(sai_vdp_calc_plane_addr(VDP_PLANE_A, 1, 25),
	             attr, "2: ");

	print_string(sai_vdp_calc_plane_addr(VDP_PLANE_A, 8, 8),
	             attr | VDP_ATTR(0, false, false, 0, /*prio=*/true),
	             "High Priority");
	print_string(sai_vdp_calc_plane_addr(VDP_PLANE_A, 9, 9),
	             attr, "Low Priority");
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
	str[8] = in & SAI_BTN_X ? 'X' : '.';
	str[9] = in & SAI_BTN_Y ? 'Y' : '.';
	str[10] = in & SAI_BTN_Z ? 'Z' : '.';

	str[12] = in & SAI_BTN_START ? 'S' : '.';
	str[13] = in & SAI_BTN_MODE ? 'M' : '.';
	str[15] = in & SAI_BTN_6B ? '6' : '.';
	str[16] = in & SAI_BTN_UNPLUGGED ? 'U' : '.';
}

static void draw_inputs(void)
{
	const uint16_t attr = VDP_ATTR(VDP_TILE(TEST_BG_VRAM_FONT_ADDR),
	                      /*hflip=*/false,
	                      /*vflip=*/false,
	                      TEST_FONT_PAL,
	                      /*prio=*/false);

	static char s_input_str[] = ".... ...... .. ..";

	set_input_str(g_sai_in[0].now, s_input_str);
	print_string(sai_vdp_calc_plane_addr(VDP_PLANE_A, 4, 24),
	             attr, s_input_str);
	set_input_str(g_sai_in[1].now, s_input_str);
	print_string(sai_vdp_calc_plane_addr(VDP_PLANE_A, 4, 25),
	             attr, s_input_str);
}

void __attribute__((noreturn)) main(void)
{
	sai_init();
	sai_pal_load(TEST_FONT_PAL, wrk_gfx_pal+BG_FONT_PAL_OFFS, 1);
	sai_pal_load(TEST_SPR_PAL, wrk_gfx_pal+SPR_MD_PAL_OFFS, 1);
	sai_vdp_dma_transfer_vram(TEST_SPR_VRAM_ADDR, wrk_gfx_chr+SPR_MD_CHR_OFFS,
	                          SPR_MD_CHR_WORDS, 2);
	sai_vdp_dma_transfer_vram(TEST_BG_VRAM_FONT_ADDR, wrk_gfx_chr+BG_FONT_CHR_OFFS,
	                          BG_FONT_CHR_WORDS, 2);
	draw_initial_text();

	while (true)
	{
		draw_inputs();
		run_test_color_anim();
		move_test_sprite();
		sai_finish();
	}
}
