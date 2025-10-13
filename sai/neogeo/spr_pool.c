#include "sai/neogeo/spr_pool.h"

void sai_neo_spr_pool_init(SaiNeoSprPool *pool,
                              uint16_t *scb_buffer,
                              uint16_t start_index,
                              uint16_t pool_capacity,
                              uint16_t fixed_shrink)
{
	pool->fixed_shrink = fixed_shrink;
	pool->pool_capacity = pool_capacity;
	// Mark sprites for initial transfer.
	pool->sprite_count = pool_capacity;

	// VRAM destination addresses for each buffer chunk.
	pool->scb_vram_addr[0] = VRAM_SCB1+(0x40*start_index);
	pool->scb_vram_addr[1] = VRAM_SCB2+(start_index);
	pool->scb_vram_addr[2] = VRAM_SCB3+(start_index);
	pool->scb_vram_addr[3] = VRAM_SCB4+(start_index);

	pool->scb_buf[1] = &scb_buffer[pool_capacity*0];
	pool->scb_buf[2] = &scb_buffer[pool_capacity*1];
	pool->scb_buf[3] = &scb_buffer[pool_capacity*2];
	pool->scb_buf[0] = &scb_buffer[pool_capacity*3];
	// Clean out the buffer data.
	uint16_t *scb1 = pool->scb_buf[0];
	for (uint16_t i = 0; i < pool_capacity*0x40; i++) *scb1++ = 0;
	uint16_t *scb2 = pool->scb_buf[1];
	for (uint16_t i = 0; i < pool_capacity; i++) *scb2++ = SAI_NEO_SCB2_ATTR(0xF, 0xFF);  // full size.
	uint16_t *scb3 = pool->scb_buf[2];
	for (uint16_t i = 0; i < pool_capacity; i++) *scb3++ = 0;
	uint16_t *scb4 = pool->scb_buf[3];
	for (uint16_t i = 0; i < pool_capacity; i++) *scb4++ = (NEO_RASTER_W)*(SAI_NEO_SPR_FIXPX);
}
