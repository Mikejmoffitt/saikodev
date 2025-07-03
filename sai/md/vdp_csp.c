#include "sai/md/vdp_csp.h"

void sai_vdp_csp_init(SaiMdCspParam *s,
                     const uint8_t *chr,
                     const uint8_t *map,
                     uint32_t vram_base,
                     uint16_t attr,
                     bool use_dma)
{
	const SaiMdCspHeader *header = (const SaiMdCspHeader *)map;
	const uint16_t ref_count = header->ref_count;

	s->vram_base = vram_base;
	s->chr = chr;
	s->map = header;
	s->ref = sai_vdp_csp_get_refs(map);
	s->spr = sai_vdp_csp_get_sprs(map);
	s->tile_base = VDP_TILE(vram_base);

	s->attr = attr;
	s->x = 0;
	s->y = 0;
	s->frame = 0;
	s->frame_last = -1;
	s->fixed_chr_words = use_dma ? 0 : sai_vdp_csp_get_fixed_vram_words(map);
	s->dma_chr_words = use_dma ? sai_vdp_csp_get_dma_vram_words(map) : 0;
}


void sai_vdp_csp_draw(SaiMdCspParam *s)
{
	(void)s;
	// TODO
}

void sai_vdp_csp_draw_fast(SaiMdCspParam *s)
{
	const SaiMdCspRef *ref = &s->ref[s->frame];
	if (ref->spr_count > 0)
	{
		const bool use_dma = s->dma_chr_words > 0;

		const SaiMdCspSpr *spr = (SaiMdCspSpr *)((const uint8_t *)(s->spr)+ref->spr_list_offs);

		const uint16_t tile_index = ref->tile_index;
		const uint16_t tile_base = use_dma ? s->tile_base : s->tile_base+tile_index;
		const uint16_t attr_base = s->attr;

		int16_t x = s->x;
		int16_t y = s->y;

		for (uint16_t i = 0; i < ref->spr_count; i++)
		{
			const uint16_t attr_comb = (attr_base ^ spr->attr);
			const bool hf = (attr_comb & VDP_HF) ? true : false;
			const bool vf = (attr_comb & VDP_VF) ? true : false;
			const uint16_t attr = attr_comb + tile_base;
			x += (hf) ? spr->flip_dx : spr->dx;
			y += (vf) ? spr->flip_dy : spr->dy;

			if (sai_vdp_spr_draw(x, y, attr, spr->size)) break;
			spr++;
		}

		// Judge whether we need to schedule a DMA and do so.
		if (!use_dma) return;

		const bool new_frame = s->frame != s->frame_last;
		if (!new_frame) return;
		sai_vdp_dma_transfer_vram(s->vram_base,
		                          &s->chr[VDP_ADDR_FROM_TILE(tile_index)],
		                          ref->tile_words, 2);
	}
	s->frame_last = s->frame;
}
