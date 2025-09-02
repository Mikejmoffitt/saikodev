//
// "Text" PCG layer.
//
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#include "sai/target.h"

// Nametable (layout)
#define TVRAM_NT_OFFS    (0x000000)
// Line select
#define TVRAM_VADDR_OFFS (0x002000)
// Horizontal scroll
#define TVRAM_HADDR_OFFS (0x003000)
// Character graphics data (tiles)
#define TVRAM_CHR_OFFS   (0x100000)

//
// Attributes and address math
//

// Text layer interaction macros

#define TEXT_AREA(w, h)     ((((h)-1)<<16)|((w)-1))
#define TEXT_OFFS(x, y)     (2*((y)+(0x40*(x))))
#define TEXT_ADDR(x, y)     (TVRMSTR+TEXT_OFFS((x),(y)))
#define TEXT_ATTR(chr, pal) (((pal)<<10)|(chr))

#ifdef __cplusplus
}
#endif  // __cplusplus
