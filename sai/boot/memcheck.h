#pragma once

#ifndef __ASSEMBLER__
// in:
//   a0.l = base address of memory region
//   d0.w = test value
//   d1.w = word count
//   a6.l = return address
// return: Z flag clear if ok (bne to error); in such a case a0.l is the address.
	.extern	sai_min_memcheck
#endif  // __ASSEMBLER__
