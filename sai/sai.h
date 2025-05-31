#pragma once

#include "sai/target.h"

#ifndef SAI_TARGET
#error "SAI_TARGET must be defined using a value from sai/target.h!"
#endif  // SAI_TARGET

#include "sai/macro.h"
#if SAI_TARGET==SAI_TARGET_MD
#include "sai/video/vdp.h"
#include "sai/io/mdpad.h"
#elif SAI_TARGET==SAI_TARGET_C2
#include "sai/io/c2.h"
#include "sai/megadrive.h"
#elif SAI_TARGET=SAI_TARGET_SYS18
#error "TODO"
#elif SAI_TARGET=SAI_TARGET_SYS16A
#error "TODO"
#elif SAI_TARGET=SAI_TARGET_SYS16B
#error "TODO"
#elif SAI_TARGET=SAI_TARGET_CPS2
#error "TODO"
#endif
