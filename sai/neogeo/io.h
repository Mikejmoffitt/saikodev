#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__

#include "sai/target.h"
#include "sai/neogeo/hw.h"

#ifndef __ASSEMBLER__
void sai_neogeo_io_init(void);
void sai_neogeo_io_poll(void);

#else
	.extern	sai_neogeo_io_init
	.extern	sai_neogeo_io_poll
#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
