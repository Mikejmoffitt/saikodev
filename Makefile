PROJECT_NAME := saisample
SAI := sai

# Project directories.
SRCDIR := src
RESDIR := res
OBJDIR := obj
OUTDIR := out

# Saikodev sources and default flags come from here.
include $(SAI)/mk/md.mk
include $(SAI)/mk/m68k-flags.mk
include $(SAI)/mk/utils.mk
include $(SAI)/mk/utils-md.mk

# User flags and files

CFLAGS +=
CPPFLAGS +=
ASFLAGS +=
LDFLAGS +=
LIBS +=

# Sources.
SOURCES_C += $(shell find $(SRCDIR)/ -type f -name '*.c')
SOURCES_CPP += $(shell find $(SRCDIR)/ -type f -name '*.cpp')
SOURCES_ASM += $(shell find $(SRCDIR)/ -type f -name '*.a68')

# Objects derived from source lists.
OBJECTS_C +=   $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES_C))
OBJECTS_CPP += $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES_CPP))
OBJECTS_ASM += $(patsubst $(SRCDIR)/%.a68,$(OBJDIR)/%.o,$(SOURCES_ASM))

.PHONY: all vars clean

all:
	$(OBJDIR)/$(PROJECT_NAME).bin

# Debug output for manual sanity check.
vars:
	@echo "GCC_VER is" "$(GCC_VER)"
	@echo "CFLAGS is" "$(CFLAGS)"
	@echo "CPPFLAGS is" "$(CPPFLAGS)"
	@echo "ASFLAGS is" "$(ASFLAGS)"
	@echo "LDFLAGS is" "$(LDFLAGS)"
	@echo "SOURCES_C is" "$(SOURCES_C)"
	@echo "SOURCES_CPP is" "$(SOURCES_CPP)"
	@echo "SOURCES_ASM is" "$(SOURCES_ASM)"
	@echo "OBJECTS_C is" "$(OBJECTS_C)"
	@echo "OBJECTS_CPP is" "$(OBJECTS_CPP)"
	@echo "OBJECTS_ASM is" "$(OBJECTS_ASM)"

clean:
	rm -rf $(OUTDIR) $(OBJDIR)

# Source build rules
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@bash -c 'printf " \e[96m[  C  ]\e[0m $< --> $@\n"'
	$(CC) $(CFLAGS) -c $< -o $@
ifneq ($(SAI_WANT_ASM_OUT),)
	$(CC) $(CFLAGS) -S $< -o $@.asm
endif

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	@bash -c 'printf " \e[96m[ CPP ]\e[0m $< --> $@\n"'
	$(CPPC) $(CPPFLAGS) -c $< -o $@
ifneq ($(SAI_WANT_ASM_OUT),)
	$(CPPC) $(CPPFLAGS) -S $< -o $@.asm
endif

$(OBJDIR)/%.o: $(SRCDIR)/%.a68
	@mkdir -p $(dir $@)
	@bash -c 'printf " \e[33m[ ASM ]\e[0m $< --> $@\n"'
# TODO: Put this obscenity in a script file or even just make a utility.
	awk '{gsub(/;/,";#"); printf("%s", $$0 RT)}' RS='"[^"]*"' $< | awk '{gsub(/\$$/,"0x"); printf("%s", $$0 RT)}' RS='"[^"]*"' | $(AS) $(ASFLAGS) -o $@ -c -



# Final output
$(OUTDIR)/$(PROJECT_NAME).elf: $(OBJECTS_ASM) $(OBJECTS_C) $(OBJECTS_CPP)
	@mkdir -p $(dir $@)
	@bash -c 'printf " \e[36m[ LNK ]\e[0m ... --> $@\n"'
	$(LD) -o $@ $(LDFLAGS) $(OBJECTS_ASM) $(OBJECTS_C) $(OBJECTS_CPP) $(LIBS)

$(OUTDIR)/$(PROJECT_NAME).bin: $(OUTDIR)/$(PROJECT_NAME).elf $(BINPAD)
	@mkdir -p $(dir $@)
	@bash -c 'printf " \e[36m[ PAD ]\e[0m ... --> $@\n"'
	$(OBJCOPY) -O binary $< $@
	$(BINPAD) $@
	@bash -c 'printf "\e[92m \tBuild Complete. --> $@\n"'
