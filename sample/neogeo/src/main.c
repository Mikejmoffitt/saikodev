#include "sai/sai.h"
#include "res.h"

#define PAL_TEXT 0
#define PAL_BACKGROUND 2
#define PAL_LOGO 5

#define TMAP_STARTING_INDEX  1
#define SPR_POOL_STARTING_INDEX (TMAP_STARTING_INDEX+32)

#define SPR_POOL_CAPACITY 16

static SaiNeoTmap    s_tmap;
static SaiNeoSprPool s_sprpool;

static uint16_t s_scb_buffer[SAI_NEO_SCB_BUFFER_SIZE(SPR_POOL_CAPACITY)];

static void print_string_fix(uint16_t vram_addr, const uint16_t attr_base,
                             const char *str)
{
	sai_neo_lspc_vram_set_mod(FIX_OFFS(1, 0));
	sai_neo_lspc_vram_set_addr(FIX_ADDR(0, 2)+vram_addr);
	char val;
	while ((val = *str++))
	{
		if (val >= ' ')
		{
			sai_neo_lspc_vram_write((val+FIX_FONT_CODE-' ') | attr_base);
		}
		else
		{
			sai_neo_lspc_vram_write((FIX_FONT_CODE) | attr_base);
		}
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

	sai_pal_set((256*16)-1, SAI_PAL888(0, s_val, s_val));
}

static void move_test_sprite(void)
{
	static int16_t s_dx = 1;
	static int16_t s_dy = 1;
	static int16_t s_x = 0;
	static int16_t s_y = 0;
	s_x += s_dx;
	s_y += s_dy;

	if (s_x + CHR_NEOGEO_W > NEO_RASTER_W-9)
	{
		s_dx = -1;
	}
	else if (s_x <= 8)
	{
		s_dx = 1;
	}
	if (s_y + CHR_NEOGEO_H > NEO_RASTER_H)
	{
		s_dy = -1;
	}
	else if (s_y <= 0)
	{
		s_dy = 1;
	}

	static const uint16_t attr = SAI_NEO_SCB1_ATTR(CHR_NEOGEO_CODE_MSB, PAL_LOGO, 0);
	uint16_t code = CHR_NEOGEO_CODE;

	static const uint16_t shrink = SAI_NEO_SCB2_ATTR(0xF, 0xFF);

	sai_neo_spr_pool_draw_grp(&s_sprpool,
	                          attr,
	                          code,
	                          shrink,
	                          CHR_NEOGEO_TILES_W,
	                          CHR_NEOGEO_TILES_H,
	                          s_x<<SAI_NEO_SPR_FIXPX_BITS,
	                          (SAI_NEO_SPR_Y_ADJ-s_y)<<SAI_NEO_SPR_FIXPX_BITS);
}


static void draw_initial_text(void)
{
	const uint32_t attr = FIX_ATTR(PAL_TEXT);
	print_string_fix((FIX_OFFS(1, 22)), attr, "Inputs");
	print_string_fix((FIX_OFFS(1, 24)), attr, "1: ");
	print_string_fix((FIX_OFFS(1, 25)), attr, "2: ");

	print_string_fix(FIX_OFFS(1, 1), attr, "Hello World!");
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
	static char s_input_str[] = ".... .... ....";
	const uint32_t attr = FIX_ATTR(PAL_TEXT);
	set_input_str(g_sai_in[0].now, s_input_str);
	print_string_fix(FIX_OFFS(4, 24), attr, s_input_str);
	set_input_str(g_sai_in[1].now, s_input_str);
	print_string_fix(FIX_OFFS(4, 25), attr, s_input_str);
}

static void draw_background(void)
{
	sai_neo_lspc_vram_set_mod(1);
	const uint16_t vram = VRAM_SCB1+(TMAP_STARTING_INDEX*SAI_NEO_SCB1_WORDS);
	sai_neo_lspc_vram_set_addr(vram);

	for (uint16_t i = 0; i < 32*32; i++)
	{
		sai_neo_lspc_vram_write(CHR_BACKGROUND_CODE + i);
		sai_neo_lspc_vram_write(SAI_NEO_SCB1_ATTR(CHR_BACKGROUND_CODE_MSB, PAL_BACKGROUND, 0));
	}
}

static void move_background(void)
{
	static int16_t s_bg_x = 0;
	static int16_t s_bg_y = 0;
	static int16_t s_bg_dx = 0.75*SAI_NEO_SPR_FIXPX;
	static int16_t s_bg_dy = 0.25*SAI_NEO_SPR_FIXPX;

	static const int16_t kaccel = 8;
	static const int16_t kspeed_max = 5*SAI_NEO_SPR_FIXPX;

	if (g_sai_in[0].now & SAI_BTN_DOWN) s_bg_dy += kaccel;
	else if (g_sai_in[0].now & SAI_BTN_UP) s_bg_dy -= kaccel;
	if (g_sai_in[0].now & SAI_BTN_RIGHT) s_bg_dx += kaccel;
	else if (g_sai_in[0].now & SAI_BTN_LEFT) s_bg_dx -= kaccel;
	if (s_bg_dx > kspeed_max) s_bg_dx = kspeed_max;
	else if (s_bg_dx < -kspeed_max) s_bg_dx = -kspeed_max;
	if (s_bg_dy > kspeed_max) s_bg_dy = kspeed_max;
	else if (s_bg_dy < -kspeed_max) s_bg_dy = -kspeed_max;

	s_bg_x += s_bg_dx;
	s_bg_y -= s_bg_dy;

	sai_neo_tmap_set_scroll_x(&s_tmap, s_bg_x);
	sai_neo_tmap_set_scroll_y(&s_tmap, s_bg_y);
}

// This is the main "USER" routine for the game.
void main(void)
{
	bool attract = false;
	switch (g_sai_neo_user_request)
	{
		case SAI_NEO_UREQ_INIT:
			// Set up Soft DIPs, etc.
			g_sai_neo_user_mode_for_system = SAI_NEO_USERMODE_INIT;
			sai_neo_system_return();
			break;

		case SAI_NEO_UREQ_EYE_CATCH:
			// Eyecatcher animation. Not used in this sample.
			sai_neo_system_return();
			break;

		case SAI_NEO_UREQ_ATTRACT:
			// Show attract mode, demo play, how to play, score table, etc.
			// On AES, this is where the game itself takes place.
			attract = true;
			break;

		case SAI_NEO_UREQ_TITLE:
			// Show the title screen, proceed to the game.
			break;
	}
	sai_init();

	g_sai_neo_user_mode_for_system = attract ?
	                                 SAI_NEO_USERMODE_ATTRACT :
	                                 SAI_NEO_USERMODE_INGAME;

	sai_neo_tmap_init(&s_tmap, TMAP_STARTING_INDEX, true);
	sai_neo_spr_pool_init(&s_sprpool, s_scb_buffer, SPR_POOL_STARTING_INDEX,
	                      SPR_POOL_CAPACITY, false);

	uint16_t attract_timer = 0;

	sai_pal_load(PAL_TEXT, vel_get_wrk_fix_pal(FIX_FONT), FIX_FONT_PAL_LEN/16);
	sai_pal_load(PAL_BACKGROUND, vel_get_wrk_chr_pal(CHR_BACKGROUND), CHR_BACKGROUND_PAL_LEN/16);
	sai_pal_load(PAL_LOGO, vel_get_wrk_chr_pal(CHR_NEOGEO), CHR_NEOGEO_PAL_LEN/16);

	sai_finish();

	draw_initial_text();


	draw_background();

	while (true)
	{
		if (attract) attract_timer++;
		run_test_color_anim();
		move_test_sprite();
		move_background();
		draw_inputs();

		sai_finish();
		sai_neo_tmap_transfer(&s_tmap);
		sai_neo_spr_pool_transfer(&s_sprpool);
	}

	sai_neo_system_return();

}

// This function is called by the BIOS if a player has hit start and has
// credits. The bits in g_sai_neo_start_flag indicate which player(s) have
// pressed start. Here, these bits can be cleared to block the player from
// starting. Upon return, if the bits are still high, credit(s) will be
// decremented accordingly.
void sai_neo_entry_player_start(void)
{
	for (uint16_t i = 0; i < 4; i++)
	{
		if (SAI_BTST(g_sai_neo_start_flag, i)) g_sai_neo_player_mode[i] = 1;
	}
}
