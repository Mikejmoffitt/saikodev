
# Flags shared by both C and C++.
COMMON_FLAGS := -mcpu=68000
COMMON_FLAGS += -O2
# precious memory cycles for function calls
COMMON_FLAGS += -fomit-frame-pointer
COMMON_FLAGS += -frename-registers -fshort-enums
COMMON_FLAGS += -Wall -Wextra -Wno-unused-function
COMMON_FLAGS += -ffreestanding
COMMON_FLAGS += -ffunction-sections -fdata-sections -fconserve-stack
COMMON_FLAGS += -fwrapv
COMMON_FLAGS += -fno-gcse
COMMON_FLAGS += -fms-extensions
COMMON_FLAGS += -fno-web -fno-unit-at-a-time
# TODO: investigate lto breaking some asm interop.
#COMMON_FLAGS += -flto
COMMON_FLAGS += -I$(SRCDIR) -I$(SAI_ROOT)
COMMON_FLAGS += -DSAI_TARGET=$(SAI_TARGET)

# For C.
CFLAGS := $(COMMON_FLAGS)
CFLAGS += -std=gnu2x
CFLAGS += -Wno-strict-aliasing

# For C++.
CPPFLAGS := $(COMMON_FLAGS)
CPPFLAGS += -std=gnu++2b

# For ASM.
ASFLAGS := $(COMMON_FLAGS) -I$(OBJDIR)
ASFLAGS += -Wa,-I$(SRCDIR) -Wa,-I$(OBJDIR) -Wa,-I$(SAI_ROOT)
ASFLAGS += -mcpu=68000 -Wa,--bitwise-or
ASFLAGS += -Wa,--register-prefix-optional
ASFLAGS += -x assembler-with-cpp

ifeq ($(TARGET_SYSTEM),MDK_TARGET_C2)
ASFLAGS += -Wa,--def -Wa,MDK_TARGET_C2=1
endif

# Linker.
GCC_VER := $(shell $(CC) -dumpversion)
LDFLAGS += -nostartfiles
LDFLAGS += -T $(LDSCRIPT)
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,-Map $(PROJECT_NAME).map

LIBS := -L $(SAI_ENV)/m68k-elf/lib -lnosys
LIBS += -L $(SAI_ENV)/lib/gcc/m68k-elf/$(GCC_VER) -lgcc

# C (on the host, for tools, etc)
HOSTCFLAGS := -O3 -std=gnu11
