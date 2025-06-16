
# Source build rules
$(SAIOBJDIR)/%.o: $(SAI)/%.c
	@mkdir -p $(dir $@)
	@bash -c 'printf " \e[96m[  C  ]\e[0m $< --> $@\n"'
	$(CC) $(CFLAGS) -c $< -o $@
ifneq ($(SAI_WANT_ASM_OUT),)
	$(CC) $(CFLAGS) -S $< -o $@.asm
endif

$(SAIOBJDIR)/%.o: $(SAI)/%.cpp
	@mkdir -p $(dir $@)
	@bash -c 'printf " \e[96m[ CPP ]\e[0m $< --> $@\n"'
	$(CPPC) $(CPPFLAGS) -c $< -o $@
ifneq ($(SAI_WANT_ASM_OUT),)
	$(CPPC) $(CPPFLAGS) -S $< -o $@.asm
endif

$(SAIOBJDIR)/%.o: $(SAI)/%.a68
	@mkdir -p $(dir $@)
	@bash -c 'printf " \e[33m[ ASM ]\e[0m $< --> $@\n"'
# TODO: Put this obscenity in a script file or even just make a utility.
	gawk '{gsub(/;/,";#"); printf("%s", $$0 RT)}' RS='"[^"]*"' $< | gawk '{gsub(/\$$/,"0x"); printf("%s", $$0 RT)}' RS='"[^"]*"' | $(AS) $(ASFLAGS) -o $@ -c -

$(OBJDIR)/%.o: $(SRCDIR)/%.c prebuild
	@mkdir -p $(dir $@)
	@bash -c 'printf " \e[96m[  C  ]\e[0m $< --> $@\n"'
	$(CC) $(CFLAGS) -c $< -o $@
ifneq ($(SAI_WANT_ASM_OUT),)
	$(CC) $(CFLAGS) -S $< -o $@.asm
endif

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp prebuild
	@mkdir -p $(dir $@)
	@bash -c 'printf " \e[96m[ CPP ]\e[0m $< --> $@\n"'
	$(CPPC) $(CPPFLAGS) -c $< -o $@
ifneq ($(SAI_WANT_ASM_OUT),)
	$(CPPC) $(CPPFLAGS) -S $< -o $@.asm
endif

$(OBJDIR)/%.o: $(SRCDIR)/%.a68 prebuild
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

$(OUTDIR)/$(PROJECT_NAME).elf: $(SAI_OBJECTS_ASM) $(OBJECTS_ASM) $(SAI_OBJECTS_C) $(OBJECTS_C) $(SAI_OBJECTS_CPP) $(OBJECTS_CPP)
	@mkdir -p $(dir $@)
	@bash -c 'printf " \e[36m[ LNK ]\e[0m ... --> $@\n"'
	$(LD) -o $@ -T $(LDSCRIPT) $(LDFLAGS) $^ $(LIBS)

# Debug output for manual sanity check.
.PHONY: saivars
saivars:
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
	@echo "SAI_SOURCES_C is" "$(SAI_SOURCES_C)"
	@echo "SAI_SOURCES_CPP is" "$(SAI_SOURCES_CPP)"
	@echo "SAI_SOURCES_ASM is" "$(SAI_SOURCES_ASM)"
	@echo "SAI_OBJECTS_C is" "$(SAI_OBJECTS_C)"
	@echo "SAI_OBJECTS_CPP is" "$(SAI_OBJECTS_CPP)"
	@echo "SAI_OBJECTS_ASM is" "$(SAI_OBJECTS_ASM)"
