
MEGALOADER := $(UTILDIR)/debug/megaloader

$(MEGALOADER): $(UTILDIR)/debug/megaloader.c
	$(CC_HOST) -D_DEFAULT_SOURCE $< -o $@ $(HOSTCFLAGS)

flash: $(OUTDIR)/$(PROJECT_NAME).bin $(MEGALOADER)
	$(MEGALOADER) md $< /dev/ttyUSB0 2> /dev/null

blastem: $(PROJECT_OUTPUT)
	blastem $<

mame: $(PROJECT_OUTPUT)
	mame megadrij -debug -r 1280x960 -prescale 4 -cart $<
