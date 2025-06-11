PROJECT_NAME := saisample
SAI := ../../sai

# Project directories.
WRKDIR := wrk
SRCDIR := src
RESDIR := res
OBJDIR := $(WRKDIR)/obj
SAIOBJDIR := $(WRKDIR)/saiobj
OUTDIR := out

PROJECT_OUTPUT := $(OUTDIR)/$(PROJECT_NAME).bin

.PHONY: all vars clean

# Saikodev sources and default flags come from here.
include $(SAI)/cps2/Sources.mk
include $(SAI)/mk/m68k-flags.mk
include $(SAI)/mk/utils.mk
include $(SAI)/mk/utils-cps2.mk
include $(SAI)/mk/sai-build-rules.mk

# Linker script.


# Flags may be ammended here for user code.
CFLAGS +=
CPPFLAGS +=
ASFLAGS +=
LDFLAGS +=
LIBS +=
LDSCRIPT := $(SAI)/ld/cps2.ld

# Sources.
SOURCES_ASM := $(shell find $(SRCDIR)/ -type f -name '*.a68')
SOURCES_C   := $(shell find $(SRCDIR)/ -type f -name '*.c')
SOURCES_CPP := $(shell find $(SRCDIR)/ -type f -name '*.cpp')

OBJECTS_ASM := $(patsubst $(SRCDIR)/%.a68,$(OBJDIR)/%.o,$(SOURCES_ASM))
OBJECTS_C   := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES_C))
OBJECTS_CPP := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES_CPP))

all: $(PROJECT_OUTPUT)
.DEFAULT_GOAL := all

clean:
	rm -rf $(OUTDIR) $(OBJDIR) $(SAIOBJDIR) $(WRKDIR)

# Source build rules
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(RESDIR)/spr.chr
	@mkdir -p $(dir $@)
	@bash -c 'printf " \e[96m[  C  ]\e[0m $< --> $@\n"'
	$(CC) $(CFLAGS) -c $< -o $@
ifneq ($(SAI_WANT_ASM_OUT),)
	$(CC) $(CFLAGS) -S $< -o $@.asm
endif

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(RESDIR)/spr.chr
	@mkdir -p $(dir $@)
	@bash -c 'printf " \e[96m[ CPP ]\e[0m $< --> $@\n"'
	$(CPPC) $(CPPFLAGS) -c $< -o $@
ifneq ($(SAI_WANT_ASM_OUT),)
	$(CPPC) $(CPPFLAGS) -S $< -o $@.asm
endif

$(OBJDIR)/%.o: $(SRCDIR)/%.a68 $(RESDIR)/spr.chr
	@mkdir -p $(dir $@)
	@bash -c 'printf " \e[33m[ ASM ]\e[0m $< --> $@\n"'
# TODO: Put this obscenity in a script file or even just make a utility.
	gawk '{gsub(/;/,";#"); printf("%s", $$0 RT)}' RS='"[^"]*"' $< | gawk '{gsub(/\$$/,"0x"); printf("%s", $$0 RT)}' RS='"[^"]*"' | $(AS) $(ASFLAGS) -o $@ -c -


# Final output
$(PROJECT_OUTPUT): $(OUTDIR)/$(PROJECT_NAME).elf $(BINPAD)
	@mkdir -p $(dir $@)
	@bash -c 'printf " \e[36m[ PAD ]\e[0m ... --> $@\n"'
	$(OBJCOPY) -O binary $< $@
	$(BINPAD) $@
	@bash -c 'printf "\n"'
	@bash -c 'printf "\e[92m \tBuild Complete. --> $@\n"'
	@bash -c 'printf " \e[0m\n"'

$(OUTDIR)/$(PROJECT_NAME).elf: $(SAI_OBJECTS_ASM) $(OBJECTS_ASM) $(SAI_OBJECTS_C) $(OBJECTS_C) # $(SAI_OBJECTS_CPP) $(OBJECTS_CPP)
	@mkdir -p $(dir $@)
	@bash -c 'printf " \e[36m[ LNK ]\e[0m ... --> $@\n"'
	$(LD) -o $@ -T $(LDSCRIPT) $(LDFLAGS) $^ $(LIBS)

# Resource data
.PHONY: $(RESDIR)/spr.chr
$(RESDIR)/spr.chr: png/spr.ini $(VELELLA)
	mkdir -p $(WRKDIR)
	mkdir -p $(@D)
	$(VELELLA) $<
	mv $(WRKDIR)/spr.chr $(@D)
