#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__

#ifndef __ASSEMBLER__
void sai_cps_io_poll(void);
#else
	.extern	sai_min_cps_io_init;
	.extern	sai_cps_io_poll;
#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
