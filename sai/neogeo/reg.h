// Saikodev Neo-Geo memory mapped registers.
//
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#include "sai/macro.h"

//
// I/O registers.
//

#define SAI_NEO_REG_P1CNT       0x300000
#define SAI_NEO_REG_DIPSW       0x300001
#define SAI_NEO_REG_SOUND       0x320000
#define SAI_NEO_REG_STATUS_A    0x320001
#define SAI_NEO_REG_P2CNT       0x340000
#define SAI_NEO_REG_STATUS_B    0x380000
#define SAI_NEO_REG_POUTPUT     0x380001
#define SAI_NEO_REG_SLOT        0x380021
#define SAI_NEO_REG_SYS_BASE    0x3A0000
#define SAI_NEO_REG_NOSHADOW    0x3A0001
#define SAI_NEO_REG_SWPBIOS     0x3A0003
#define SAI_NEO_REG_CRDUNLOCK1  0x3A0005
#define SAI_NEO_REG_CRDLOCK2    0x3A0007
#define SAI_NEO_REG_BRDFIX      0x3A000B
#define SAI_NEO_REG_SRAMLOCK    0x3A000D
#define SAI_NEO_REG_PALBANK1    0x3A000F
#define SAI_NEO_REG_SHADOW      0x3A0011
#define SAI_NEO_REG_SWPROM      0x3A0013
#define SAI_NEO_REG_CRDLOCK1    0x3A0015
#define SAI_NEO_REG_CRDUNLOCK2  0x3A0017
#define SAI_NEO_REG_CRDNORMAL   0x3A0019
#define SAI_NEO_REG_CRTFIX      0x3A001B
#define SAI_NEO_REG_SRAMUNLOCK  0x3A001D
#define SAI_NEO_REG_PALBANK0    0x3A001F
#define SAI_NEO_REG_VRAMADDR    0x3C0000
#define SAI_NEO_REG_VRAMRW      0x3C0002
#define SAI_NEO_REG_VRAMMOD     0x3C0004
#define SAI_NEO_REG_LSPCMODE    0x3C0006
#define SAI_NEO_REG_TIMERHIGH   0x3C0008
#define SAI_NEO_REG_TIMERLOW    0x3C000A
#define SAI_NEO_REG_IRQACK      0x3C000C
#define SAI_NEO_REG_TIMERSTOP   0x3C000E

#ifdef __cplusplus
}
#endif  // __cplusplus
