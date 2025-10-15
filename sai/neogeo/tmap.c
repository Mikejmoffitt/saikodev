#include "sai/neogeo/tmap.h"

void sai_neo_tmap_init(SaiNeoTmap *tmap, uint16_t start_index, uint16_t cull)
{
	tmap->cull = cull;

	tmap->scb_vram_addr[0] = VRAM_SCB1+(64*start_index);
	tmap->scb_vram_addr[1] = VRAM_SCB2+(start_index);
	tmap->scb_vram_addr[2] = VRAM_SCB3+(start_index);
	tmap->scb_vram_addr[3] = VRAM_SCB4+(start_index);

	sai_neo_lspc_vram_set_mod(1);

	// Erase the tilemap.
	sai_neo_lspc_vram_set_addr(tmap->scb_vram_addr[0]);
	for (uint16_t i = 0; i < SAI_NEO_TMAP_TILES_W*SAI_NEO_TMAP_TILES_H; i++)
	{
		sai_neo_lspc_vram_write(0);
		sai_neo_lspc_vram_write(0);
	}
	// Set all shrinking values to full-size.
	sai_neo_lspc_vram_set_addr(tmap->scb_vram_addr[1]);
	for (uint16_t i = 0; i < SAI_NEO_TMAP_TILES_W; i++)
	{
		sai_neo_lspc_vram_write(SAI_NEO_SCB2_ATTR(0xF, 0xFF));
	}

	if (!tmap->cull)
	{
		// The first sprite is given a size of 33, and the rest sticky.
		sai_neo_lspc_vram_set_addr(tmap->scb_vram_addr[2]);
		sai_neo_lspc_vram_write(SAI_NEO_SCB3_ATTR(33, SAI_NEO_SPR_Y_ADJ));
		for (uint16_t i = 1; i < SAI_NEO_TMAP_TILES_W; i++)
		{
			sai_neo_lspc_vram_write(SAI_NEO_SCB3_STICKY);
		}

		// Sprite 0 is set to X=0; the rest do not require this attribute.
		sai_neo_lspc_vram_set_addr(tmap->scb_vram_addr[3]);
		sai_neo_lspc_vram_write(SAI_NEO_SCB4_ATTR(0));
	}

	tmap->scroll32      = 0x00000000;
	tmap->scroll32_prev = 0xFFFFFFFF;

	sai_neo_tmap_transfer(tmap);
}

void sai_neo_tmap_transfer(SaiNeoTmap *tmap)
{
	if (tmap->cull)
	{
		// If the position has changed, set X and Y positions for all sprites.
		// Along the way, if X is out of range, sprites are hidden.
		if (tmap->scroll32 != tmap->scroll32_prev)
		{
			// SCB3: Xpos
			sai_neo_lspc_vram_set_mod(1);
			sai_neo_lspc_vram_set_addr(tmap->scb_vram_addr[3]);
			// Spread X pos for all columns.
			uint16_t set_x = tmap->scroll[0];
			for (uint16_t i = 0; i < SAI_NEO_TMAP_TILES_W; i++)
			{
				sai_neo_lspc_vram_write(set_x);
				set_x += SAI_NEO_SPR_TILESIZE*SAI_NEO_SPR_FIXPX;
			}

			// SCB4: Ypos
			// Now write Y position, but check if the X position is out of view
			// and if so hide the sprite .
			const uint16_t bias_x = (SAI_NEO_SPR_TILESIZE*SAI_NEO_SPR_FIXPX);
			uint16_t chk_x = tmap->scroll[0] + bias_x;
			uint16_t lim_x = (NEO_RASTER_W* SAI_NEO_SPR_FIXPX) + bias_x;

			const uint16_t ypos = SAI_NEO_SCB3_ATTR(33, tmap->scroll[1]);
			sai_neo_lspc_vram_set_addr(tmap->scb_vram_addr[2]);
			for (uint16_t i = 0; i < SAI_NEO_TMAP_TILES_W; i++)
			{
				if (chk_x < lim_x) sai_neo_lspc_vram_write(ypos);
				else sai_neo_lspc_vram_write(0);
				chk_x += SAI_NEO_SPR_TILESIZE*SAI_NEO_SPR_FIXPX;
			}
			tmap->scroll32_prev = tmap->scroll32;
		}
	}
	else
	{
		// Set up mod so we can write Y then X in sequence.
		sai_neo_lspc_vram_set_mod(VRAM_SCB4-VRAM_SCB3);
		sai_neo_lspc_vram_set_addr(tmap->scb_vram_addr[2]);
		sai_neo_lspc_vram_write(SAI_NEO_SCB3_ATTR(33, tmap->scroll[1]));
		sai_neo_lspc_vram_write(SAI_NEO_SCB4_ATTR(tmap->scroll[0]));
	}
}
