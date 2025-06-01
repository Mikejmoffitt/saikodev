
SAI_ENV ?= /opt/toolchains/mars/m68k-elf
SAI_BIN := $(SAI_ENV)/bin

# If the user didn't specify the Saikodev dir, assume it's at the project root.
SAI_ROOT ?= sai
UTILDIR := $(SAI_ROOT)/util

# TODO: Detect WSL

# Host tools
CC_HOST := cc
CC := $(SAI_BIN)/m68k-elf-gcc
CPPC := $(SAI_BIN)/m68k-elf-g++
AS := $(SAI_BIN)/m68k-elf-gcc
LD := $(SAI_BIN)/m68k-elf-gcc
NM := $(SAI_BIN)/m68k-elf-nm
OBJCOPY := $(SAI_BIN)/m68k-elf-objcopy

SPLIT := split

# Utilities packed in
BIN2S := $(UTILDIR)/core/bin2s
BIN2H := $(UTILDIR)/core/bin2h
BINPAD := $(UTILDIR)/core/binpad
BSPLIT := $(UTILDIR)/core/bsplit
PNGTO := $(UTILDIR)/image/pngto/pngto
PNG2CSP := $(UTILDIR)/png2csp/png2csp

# Flags shared by both C and C++.
COMMON_FLAGS := $(FLAGS)
COMMON_FLAGS += -mcpu=68000
COMMON_FLAGS += -O2
COMMON_FLAGS += -fomit-frame-pointer
COMMON_FLAGS += -frename-registers -fshort-enums
COMMON_FLAGS += -Wall -Wextra -Wno-unused-function
COMMON_FLAGS += -ffreestanding
COMMON_FLAGS += -ffunction-sections -fdata-sections -fconserve-stack
COMMON_FLAGS += -fwrapv
COMMON_FLAGS += -fno-gcse
COMMON_FLAGS += -fms-extensions
COMMON_FLAGS += -fno-web -fno-unit-at-a-time
#COMMON_FLAGS += -flto
COMMON_FLAGS += -I$(SRCDIR) -I$(MDKSRCDIR) -I.
COMMON_FLAGS += -D$(TARGET_SYSTEM)

# For C.
CFLAGS := $(COMMON_FLAGS)
CFLAGS += -std=gnu2x
CFLAGS += -Wno-strict-aliasing

# For C++.
CPPFLAGS := $(COMMON_FLAGS)
CPPFLAGS += -std=gnu++2b

# For ASM.
ASFLAGS := $(COMMON_FLAGS) -I$(OBJDIR)
ASFLAGS += -Wa,-I$(SRCDIR) -Wa,-I$(OBJDIR) -Wa,-I$(MDKSRCDIR)
ASFLAGS += -mcpu=68000 -Wa,--bitwise-or
ASFLAGS += -Wa,--register-prefix-optional
ASFLAGS += -x assembler-with-cpp

# Linker.
CC_VER := $(shell $(CC) -dumpversion)
LDFLAGS += -nostartfiles
LDFLAGS += -T $(LDSCRIPT)
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,-Map $(PROJECT_NAME).map

LIBS := -L $(MDK_ENV)/m68k-elf/lib -lnosys
LIBS += -L $(MDK_ENV)/lib/gcc/m68k-elf/$(CC_VER) -lgcc

# Host
HOSTCFLAGS := -O3 -std=gnu11
