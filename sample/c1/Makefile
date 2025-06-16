#
# Saikodev System C1 sample project Makefile
#
# You may copy this and modify as you see fit for your own project.
#
# Notes:
#
# * Change the SAI variable to point to the location of the `sai` submodule
#   directory relative to this file.
# * You can explicitly name sources instead of using wildcards as I have below.
# * Use the `prebuild` phony target for build steps that should execute before
#   the project source is compiled (graphics resource listings, etc).
# * The expected output data differs from system to system (for grahpics data,
#   sound data, etc).
#

# Project Setup  ---------------------------------------------------------------

PROJECT_NAME := saisample
SAI := ../../sai

WRKDIR := wrk
SRCDIR := src
RESDIR := res
OBJDIR := $(WRKDIR)/obj
SAIOBJDIR := $(WRKDIR)/saiobj
OUTDIR := out

PROJECT_OUTPUT := $(OUTDIR)/$(PROJECT_NAME).bin

.PHONY: all vars clean

# Saikodev Build Rules and Sources ---------------------------------------------

include $(SAI)/c2/SourcesC1.mk
include $(SAI)/mk/m68k-flags.mk
include $(SAI)/mk/utils.mk
include $(SAI)/mk/utils-c1.mk

# Linker -----------------------------------------------------------------------

LDSCRIPT := $(SAI)/ld/c1.ld
LDFLAGS +=
LIBS +=

# Compilation Flags ------------------------------------------------------------
CFLAGS +=
CPPFLAGS +=
ASFLAGS +=

# User Sources -----------------------------------------------------------------

SOURCES_ASM := $(shell find $(SRCDIR)/ -type f -name '*.a68')
SOURCES_C   := $(shell find $(SRCDIR)/ -type f -name   '*.c')
SOURCES_CPP := $(shell find $(SRCDIR)/ -type f -name '*.cpp')

OBJECTS_ASM := $(patsubst $(SRCDIR)/%.a68,$(OBJDIR)/%.o,$(SOURCES_ASM))
OBJECTS_C   := $(patsubst $(SRCDIR)/%.c,  $(OBJDIR)/%.o,$(SOURCES_C))
OBJECTS_CPP := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES_CPP))

# Recipes ----------------------------------------------------------------------

all: $(PROJECT_OUTPUT)
.DEFAULT_GOAL := all

clean:
	rm -rf $(OUTDIR) $(OBJDIR) $(SAIOBJDIR) $(WRKDIR)

# Prebuild should depend on any targets that are required before compilation.
.PHONY: prebuild
prebuild:

include $(SAI)/mk/build-rules.mk
