
$(MEGALOADER): $(UTILDIR)/debug/megaloader.c
	@$(CC_HOST) -D_DEFAULT_SOURCE $< -o $@ $(HOSTCFLAGS)

flash: $(OUTDIR)/$(PROJECT_NAME).bin $(MEGALOADER)
	$(MEGALOADER) md $< /dev/ttyUSB0 2> /dev/null

mame: $(OUTDIR)/$(PROJECT_NAME).bin
	exec mame megadrij -cart $< -debug -r 1280x960 -prescale 2
