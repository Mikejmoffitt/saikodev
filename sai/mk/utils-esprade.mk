
SAI_TESTROM_NAME := esprade

# Prepares images for use with 27C040 or compatible ROMs.
.PHONY: rom
rom: $(OUTDIR)/rom/u42.bin

$(OUTDIR)/rom/u42.bin: $(PROJECT_OUTPUT) $(BSPLIT) $(BINPAD)
	mkdir -p $(@D)
	mkdir -p $(WRKDIR)
	$(BINPAD) $(PROJECT_OUTPUT) 0x100000
	$(BSPLIT) s $(PROJECT_OUTPUT) $(@D)/u42.bin $(@D)/u41.bin
# TODO: graphics
	dd if=/dev/zero of=$(@D)/u63.bin bs=1M count=4
	cp $(@D)/u63.bin $(@D)/u64.bin
	cp $(@D)/u63.bin $(@D)/u65.bin
	cp $(@D)/u63.bin $(@D)/u66.bin
	cp $(@D)/u63.bin $(@D)/u54.bin
	cp $(@D)/u63.bin $(@D)/u55.bin
	cp $(@D)/u63.bin $(@D)/u52.bin
	cp $(@D)/u63.bin $(@D)/u53.bin
	cp $(@D)/u63.bin $(@D)/u51.bin
	cp $(@D)/u63.bin $(@D)/u19.bin

$(OUTDIR)/$(SAI_TESTROM_NAME)/u42.int: $(OUTDIR)/rom/u42.bin $(BSPLIT) $(BINPAD)
	mkdir -p $(@D)
	mkdir -p $(WRKDIR)
	cp $(OUTDIR)/rom/u42.bin $(@D)/u42.int
	cp $(OUTDIR)/rom/u41.bin $(@D)/u41.int
	cp $(OUTDIR)/rom/u63.bin $(@D)/esp_u63.u63
	cp $(OUTDIR)/rom/u64.bin $(@D)/esp_u64.u64
	cp $(OUTDIR)/rom/u65.bin $(@D)/esp_u65.u65
	cp $(OUTDIR)/rom/u66.bin $(@D)/esp_u66.u66
	cp $(OUTDIR)/rom/u54.bin $(@D)/esp_u54.u54
	cp $(OUTDIR)/rom/u55.bin $(@D)/esp_u55.u55
	cp $(OUTDIR)/rom/u52.bin $(@D)/esp_u52.u52
	cp $(OUTDIR)/rom/u53.bin $(@D)/esp_u53.u53
	cp $(OUTDIR)/rom/u51.bin $(@D)/esp_u51.u51
	cp $(OUTDIR)/rom/u19.bin $(@D)/esp_u19.u19
	dd if=/dev/zero of=$(@D)/eeprom-esprade.bin bs=1 count=128
	$(BSPLIT) s $(WRKDIR)/prgaa $(@D)/ooparts.ic32 $(@D)/ooparts.ic31
	$(BSPLIT) s $(WRKDIR)/prgab $(@D)/ooparts.ic34 $(@D)/ooparts.ic33
	# TODO adpcm
	dd if=/dev/zero of=$(@D)/epr-13655.ic4 bs=1 count=1

mame: $(OUTDIR)/$(SAI_TESTROM_NAME)/u42.int
	mame $(SAI_TESTROM_NAME) -debug -r 1280x960 -prescale 4 -rompath $(shell pwd)/$(OUTDIR) -ror

mamet: $(OUTDIR)/$(SAI_TESTROM_NAME)/u42.int
	mame $(SAI_TESTROM_NAME) -debug -r 960x1280 -prescale 4 -rompath $(shell pwd)/$(OUTDIR)
