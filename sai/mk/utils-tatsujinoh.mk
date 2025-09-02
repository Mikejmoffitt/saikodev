
SAI_TESTROM_NAME := truxton2

.PHONY: rom
rom: $(OUTDIR)/rom/tp024_1.bin

$(OUTDIR)/rom/tp024_1.bin: $(PROJECT_OUTPUT) $(BSPLIT) $(BINPAD)
	mkdir -p $(@D)
	mkdir -p $(WRKDIR)
# Program data
	$(BINPAD) $(PROJECT_OUTPUT) 0x80000
	cp $(PROJECT_OUTPUT) $(@D)/tp024_1.bin
# MSM6295 ADPCM Data
	dd if=/dev/zero of=$(WRKDIR)/adpcm_blank.bin bs=512K count=1
	cp $(WRKDIR)/adpcm_blank.bin $(@D)/tp024_2.bin
# GCU CHR Data
	dd if=/dev/zero of=$(WRKDIR)/gfx_blank.bin bs=1M count=1
	cp $(WRKDIR)/gfx_blank.bin $(@D)/tp024_3.bin
	cp $(WRKDIR)/gfx_blank.bin $(@D)/tp024_4.bin

$(OUTDIR)/$(SAI_TESTROM_NAME)/tp024_1.bin: $(OUTDIR)/rom/tp024_1.bin $(BSPLIT) $(BINPAD)
	mkdir -p $(@D)
	cp $(OUTDIR)/rom/tp024_1.bin $(@D)/tp024_1.bin
	cp $(OUTDIR)/rom/tp024_2.bin $(@D)/tp024_2.bin
	cp $(OUTDIR)/rom/tp024_3.bin $(@D)/tp024_3.bin
	cp $(OUTDIR)/rom/tp024_4.bin $(@D)/tp024_4.bin

mame: $(OUTDIR)/$(SAI_TESTROM_NAME)/tp024_1.bin
	mame $(SAI_TESTROM_NAME) -debug -r 1280x960 -prescale 4 -rompath $(shell pwd)/$(OUTDIR) -ror

mamet: $(OUTDIR)/$(SAI_TESTROM_NAME)/tp024_1.bin
	mame $(SAI_TESTROM_NAME) -debug -r 960x1280 -prescale 4 -rompath $(shell pwd)/$(OUTDIR)
