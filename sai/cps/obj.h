//
// Original CPS sprites
//
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
void sai_cps_obj_init(void);
#else
	.extern	sai_cps_obj_init
	.extern	sai_min_cps_obj_init

#endif  // __ASSEMBLER__

#ifdef __cplusplus
}
#endif  // __cplusplus
