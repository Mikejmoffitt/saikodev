#include "sai/sai.h"
#include "res.h"

#define TEST_BG_VRAM_FONT_ADDR 0x0000
#define TEST_SPR_VRAM_ADDR (TEST_BG_VRAM_FONT_ADDR+BG_FONT_CHR_BYTES)

#define TEST_FONT_PAL 0
#define TEST_SPR_PAL 1

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
	                           /*prio=*/true);
	sai_vdp_spr_draw(s_x+128,          s_y+128,          attr_base+(SPR_MD_FRAME_OFFS*0), SPR_MD_SIZE);
	sai_vdp_spr_draw(s_x+128+SPR_MD_W, s_y+128,          attr_base+(SPR_MD_FRAME_OFFS*1), SPR_MD_SIZE);
	sai_vdp_spr_draw(s_x+128,          s_y+128+SPR_MD_H, attr_base+(SPR_MD_FRAME_OFFS*2), SPR_MD_SIZE);
	sai_vdp_spr_draw(s_x+128+SPR_MD_W, s_y+128+SPR_MD_H, attr_base+(SPR_MD_FRAME_OFFS*3), SPR_MD_SIZE);
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
	while (true)
	{
		run_test_color_anim();
		move_test_sprite();
		sai_finish();
	}
}
