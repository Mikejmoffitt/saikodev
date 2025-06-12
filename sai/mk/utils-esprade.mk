
SAI_TESTROM_NAME := esprade

# Prepares images for use with 27C040 or compatible ROMs.
.PHONY: rom
rom: $(OUTDIR)/rom/u42.bin

$(OUTDIR)/rom/u42.bin: $(PROJECT_OUTPUT) $(BSPLIT) $(BINPAD)
	mkdir -p $(@D)
	mkdir -p $(WRKDIR)
# Program data
	$(BINPAD) $(PROJECT_OUTPUT) 0x100000
	$(BSPLIT) s $(PROJECT_OUTPUT) $(@D)/u42.bin $(@D)/u41.bin
# SP013 data split
	$(BSPLIT) s $(RESDIR)/spr.chr $(WRKDIR)/spr.ch1 $(WRKDIR)/spr.ch2
# SP013 (lower)
	$(BINPAD) $(WRKDIR)/spr.ch1 16777216
	split -b 4194304 $(WRKDIR)/spr.ch1 $(WRKDIR)/x
	mv $(WRKDIR)/xaa $(@D)/u64.bin
	mv $(WRKDIR)/xab $(@D)/u66.bin
# SP013 (upper)
	$(BINPAD) $(WRKDIR)/spr.ch2 16777216
	split -b 4194304 $(WRKDIR)/spr.ch2 $(WRKDIR)/x
	mv $(WRKDIR)/xaa $(@D)/u63.bin
	mv $(WRKDIR)/xab $(@D)/u65.bin
# BG038 A
	cp $(RESDIR)/bga.chr $(WRKDIR)/bga.chr
	$(BINPAD) $(WRKDIR)/bga.chr 16777216
	split -b 4194304 $(WRKDIR)/bga.chr $(WRKDIR)/x
	mv $(WRKDIR)/xaa $(@D)/u54.bin
	mv $(WRKDIR)/xab $(@D)/u55.bin
# BG038 B
	cp $(RESDIR)/bgb.chr $(WRKDIR)/bgb.chr
	$(BINPAD) $(WRKDIR)/bgb.chr 16777216
	split -b 4194304 $(WRKDIR)/bgb.chr $(WRKDIR)/x
	mv $(WRKDIR)/xaa $(@D)/u52.bin
	mv $(WRKDIR)/xab $(@D)/u53.bin
# BG038 C
	cp $(RESDIR)/bgc.chr $(WRKDIR)/bgc.chr
	$(BINPAD) $(WRKDIR)/bgc.chr 16777216
	split -b 4194304 $(WRKDIR)/bgc.chr $(WRKDIR)/x
	mv $(WRKDIR)/xaa $(@D)/u51.bin
# ADPCM
	cp $(RESDIR)/ymz.ymz $(WRKDIR)/ymz.ymz
	$(BINPAD) $(WRKDIR)/ymz.ymz 16777216
	split -b 4194304 $(WRKDIR)/ymz.ymz $(WRKDIR)/x
	mv $(WRKDIR)/xaa $(@D)/u19.bin

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

mame: $(OUTDIR)/$(SAI_TESTROM_NAME)/u42.int
	mame $(SAI_TESTROM_NAME) -debug -r 1280x960 -prescale 4 -rompath $(shell pwd)/$(OUTDIR) -ror

mamet: $(OUTDIR)/$(SAI_TESTROM_NAME)/u42.int
	mame $(SAI_TESTROM_NAME) -debug -r 960x1280 -prescale 4 -rompath $(shell pwd)/$(OUTDIR)
