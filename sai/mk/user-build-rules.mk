
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(PREBUILD)
	@mkdir -p $(dir $@)
	@bash -c 'printf " \e[96m[  C  ]\e[0m $< --> $@\n"'
	$(CC) $(CFLAGS) -c $< -o $@
ifneq ($(SAI_WANT_ASM_OUT),)
	$(CC) $(CFLAGS) -S $< -o $@.asm
endif

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(PREBUILD)
	@mkdir -p $(dir $@)
	@bash -c 'printf " \e[96m[ CPP ]\e[0m $< --> $@\n"'
	$(CPPC) $(CPPFLAGS) -c $< -o $@
ifneq ($(SAI_WANT_ASM_OUT),)
	$(CPPC) $(CPPFLAGS) -S $< -o $@.asm
endif

$(OBJDIR)/%.o: $(SRCDIR)/%.a68 $(PREBUILD)
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
