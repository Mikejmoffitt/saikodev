PROJECT_NAME := saisample
SAI_ROOT := ../sai

# Project directories.
SRCDIR := src
RESDIR := res
OBJDIR := obj

# Sources.
SOURCES_C := $(shell find $(SRCDIR)/ -type f -name '*.c')
SOURCES_CPP := 
SOURCES_ASM := $(shell find $(SRCDIR)/ -type f -name '*.a68')

# Saikodev sources and default flags come from here.
include $(SAI_ROOT)/mk/md.mk
include $(SAI_ROOT)/mk/m68k-flags.mk
include $(SAI_ROOT)/mk/asm-filter.mk

# Additional user flags and libraries.
CFLAGS +=
CPPFLAGS +=
ASFLAGS +=
LDFLAGS +=
LIBS +=

# Objects derived from source lists.
OBJECTS_C := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES_C)) \
             $(patsubst $(MDKSRCDIR)/%.c,$(OBJDIR)/%.o,$(MDKSOURCES_C))
OBJECTS_CPP := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES_CPP)) \
               $(patsubst $(MDKSRCDIR)/%.cpp,$(OBJDIR)/%.o,$(MDKSOURCES_CPP))
OBJECTS_ASM := $(patsubst $(SRCDIR)/%.a68,$(OBJDIR)/%.o,$(SOURCES_ASM)) \
               $(patsubst $(MDKSRCDIR)/%.a68,$(OBJDIR)/%.o,$(MDKSOURCES_ASM))

.PHONY: all vars

# Debug output for manual sanity check.
vars:
	@echo "GCC_VER is" "$(GCC_VER)"
	@echo "CFLAGS is" "$(CFLAGS)"
	@echo "CPPFLAGS is" "$(CPPFLAGS)"
	@echo "LDFLAGS is" "$(LDFLAGS)"
	@echo "MDKSOURCES_C is" "$(MDKSOURCES_C)"
	@echo "MDKSOURCES_ASM is" "$(MDKSOURCES_ASM)"
	@echo "SOURCES_C is" "$(SOURCES_C)"
	@echo "SOURCES_CPP is" "$(SOURCES_CPP)"
	@echo "SOURCES_ASM is" "$(SOURCES_ASM)"
	@echo "OBJECTS_C is" "$(OBJECTS_C)"
	@echo "OBJECTS_CPP is" "$(OBJECTS_CPP)"
	@echo "OBJECTS_ASM is" "$(OBJECTS_ASM)"


