//
// Based on the target platform, the input bitfield assignments change.
//
#pragma once

#include "sai/target.h"
#include "sai/macro.h"

//
// Mega Drive / Genesis
//
#if SAI_TARGET == SAI_TARGET_MD

#define SAI_BTN_UP             (SAI_BITVAL(0))
#define SAI_BTN_DOWN           (SAI_BITVAL(1))
#define SAI_BTN_LEFT           (SAI_BITVAL(2))
#define SAI_BTN_RIGHT          (SAI_BITVAL(3))
#define SAI_BTN_B              (SAI_BITVAL(4))
#define SAI_BTN_C              (SAI_BITVAL(5))
#define SAI_BTN_A              (SAI_BITVAL(6))
#define SAI_BTN_START          (SAI_BITVAL(7))
#define SAI_BTN_Z              (SAI_BITVAL(8))
#define SAI_BTN_Y              (SAI_BITVAL(9))
#define SAI_BTN_X              (SAI_BITVAL(10))
#define SAI_BTN_MODE           (SAI_BITVAL(11))
#define SAI_BTN_6B             (SAI_BITVAL(15))
#define SAI_BTN_UNPLUGGED      (SAI_BITVAL(14))

//
// System C/C2/18
//
#elif SAI_TARGET == SAI_TARGET_C1 || SAI_TARGET == SAI_TARGET_C2 || SAI_TARGET == SAI_TARGET_S18

// Player 1/2 inputs
#define SAI_BTN_UP             (SAI_BITVAL(5))
#define SAI_BTN_DOWN           (SAI_BITVAL(4))
#define SAI_BTN_LEFT           (SAI_BITVAL(7))
#define SAI_BTN_RIGHT          (SAI_BITVAL(6))
#define SAI_BTN_A              (SAI_BITVAL(0))
#define SAI_BTN_B              (SAI_BITVAL(1))
#define SAI_BTN_C              (SAI_BITVAL(2))
#define SAI_BTN_D              (SAI_BITVAL(3))
#define SAI_BTN_COIN           (SAI_BITVAL(8))
#define SAI_BTN_START          (SAI_BITVAL(9))
#define SAI_BTN_TEST           (SAI_BITVAL(10))
#define SAI_BTN_SERVICE        (SAI_BITVAL(11))
#define SAI_BTN_SELECT         (SAI_BITVAL(12))

//
// CPS
//
#elif SAI_TARGET == SAI_TARGET_CPS

#define SAI_BTN_RIGHT          (SAI_BITVAL(0))
#define SAI_BTN_LEFT           (SAI_BITVAL(1))
#define SAI_BTN_DOWN           (SAI_BITVAL(2))
#define SAI_BTN_UP             (SAI_BITVAL(3))
#define SAI_BTN_A              (SAI_BITVAL(4))
#define SAI_BTN_B              (SAI_BITVAL(5))
#define SAI_BTN_C              (SAI_BITVAL(6))
#define SAI_BTN_D              (SAI_BITVAL(8))
#define SAI_BTN_E              (SAI_BITVAL(9))
#define SAI_BTN_F              (SAI_BITVAL(10))
#define SAI_BTN_START          (SAI_BITVAL(11))
#define SAI_BTN_COIN           (SAI_BITVAL(12))
#define SAI_BTN_TEST           (SAI_BITVAL(13))
#define SAI_BTN_SERVICE        (SAI_BITVAL(14))

//
// CPS2
//
#elif SAI_TARGET == SAI_TARGET_CPS2

#define SAI_BTN_RIGHT          (SAI_BITVAL(0))
#define SAI_BTN_LEFT           (SAI_BITVAL(1))
#define SAI_BTN_DOWN           (SAI_BITVAL(2))
#define SAI_BTN_UP             (SAI_BITVAL(3))
#define SAI_BTN_A              (SAI_BITVAL(4))
#define SAI_BTN_B              (SAI_BITVAL(5))
#define SAI_BTN_C              (SAI_BITVAL(6))
#define SAI_BTN_D              (SAI_BITVAL(8))
#define SAI_BTN_E              (SAI_BITVAL(9))
#define SAI_BTN_F              (SAI_BITVAL(10))
#define SAI_BTN_START          (SAI_BITVAL(11))
#define SAI_BTN_COIN           (SAI_BITVAL(12))
#define SAI_BTN_TEST           (SAI_BITVAL(13))
#define SAI_BTN_SERVICE        (SAI_BITVAL(14))

//
// Sysem 16B
//
#elif SAI_TARGET == SAI_TARGET_S16B

#define SAI_BTN_LEFT           (SAI_BITVAL(7)
#define SAI_BTN_RIGHT          (SAI_BITVAL(6)
#define SAI_BTN_UP             (SAI_BITVAL(5)
#define SAI_BTN_DOWN           (SAI_BITVAL(4)
#define SAI_BTN_D              (SAI_BITVAL(3)
#define SAI_BTN_A              (SAI_BITVAL(1)
#define SAI_BTN_B              (SAI_BITVAL(2)
#define SAI_BTN_C              (SAI_BITVAL(0)
#define SAI_BTN_COIN           (SAI_BITVAL(8))
#define SAI_BTN_START          (SAI_BITVAL(9))
#define SAI_BTN_TEST           (SAI_BITVAL(10))
#define SAI_BTN_SERVICE        (SAI_BITVAL(11))

//
// Atlus ESP Ra.De.
//
#elif SAI_TARGET == SAI_TARGET_ESPRADE

#define SAI_BTN_UP             (SAI_BITVAL(0))
#define SAI_BTN_DOWN           (SAI_BITVAL(1))
#define SAI_BTN_LEFT           (SAI_BITVAL(2))
#define SAI_BTN_RIGHT          (SAI_BITVAL(3))
#define SAI_BTN_A              (SAI_BITVAL(4))
#define SAI_BTN_B              (SAI_BITVAL(5))
#define SAI_BTN_C              (SAI_BITVAL(6))
#define SAI_BTN_START          (SAI_BITVAL(7))
#define SAI_BTN_COIN           (SAI_BITVAL(8))
#define SAI_BTN_TEST           (SAI_BITVAL(9))
#define SAI_BTN_D              (SAI_BITVAL(10))
#define SAI_BTN_E              (SAI_BITVAL(11))
#define SAI_BTN_SERVICE        (SAI_BITVAL(12))


#else

// Some generic mappings
#define SAI_BTN_UP             (SAI_BITVAL(0))
#define SAI_BTN_DOWN           (SAI_BITVAL(1))
#define SAI_BTN_LEFT           (SAI_BITVAL(2))
#define SAI_BTN_RIGHT          (SAI_BITVAL(3))
#define SAI_BTN_A              (SAI_BITVAL(4))
#define SAI_BTN_B              (SAI_BITVAL(5))
#define SAI_BTN_C              (SAI_BITVAL(6))
#define SAI_BTN_D              (SAI_BITVAL(7))
#define SAI_BTN_E              (SAI_BITVAL(8))
#define SAI_BTN_F              (SAI_BITVAL(9))
#define SAI_BTN_START          (SAI_BITVAL(10))
#define SAI_BTN_COIN           (SAI_BITVAL(11))
#define SAI_BTN_TEST           (SAI_BITVAL(12))
#define SAI_BTN_SERVICE        (SAI_BITVAL(13))

#endif
