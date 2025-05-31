#pragma once

#include <stdint.h>
#include "sai/memmap.h"

#define MD_IO_VERSION          (MD_IO_BASE + 0x01)
#define MD_IO_DATA1            (MD_IO_BASE + 0x03)
#define MD_IO_DATA2            (MD_IO_BASE + 0x05)
#define MD_IO_DATA3            (MD_IO_BASE + 0x07)
#define MD_IO_CTRL1            (MD_IO_BASE + 0x09)
#define MD_IO_CTRL2            (MD_IO_BASE + 0x0B)
#define MD_IO_CTRL3            (MD_IO_BASE + 0x0D)
#define MD_IO_TXD1             (MD_IO_BASE + 0x0F)
#define MD_IO_RXD1             (MD_IO_BASE + 0x11)
#define MD_IO_SERIO1           (MD_IO_BASE + 0x13)
#define MD_IO_TXD2             (MD_IO_BASE + 0x15)
#define MD_IO_RXD2             (MD_IO_BASE + 0x17)
#define MD_IO_SERIO2           (MD_IO_BASE + 0x19)
#define MD_IO_TXD3             (MD_IO_BASE + 0x1B)
#define MD_IO_RXD3             (MD_IO_BASE + 0x1D)
#define MD_IO_SERIO3           (MD_IO_BASE + 0x1F)
