SAI_TESTROM_NAME := sfa3ud

# Prepares images for use with 27C4096 program ROMs, 27C322 graphics ROMs.
.PHONY: rom
rom: $(OUTDIR)/rom/03.bin

$(OUTDIR)/rom/03.bin: $(PROJECT_OUTPUT) $(BSPLIT) $(BINPAD)
	mkdir -p $(@D)
	mkdir -p $(WRKDIR)
# Program
	cp $(PROJECT_OUTPUT) $(WRKDIR)/prg.bin
	$(BINPAD) $(WRKDIR)/prg.bin 0x400000
	$(SPLIT) -b 524288 $(WRKDIR)/prg.bin $(WRKDIR)/prg
	$(BSPLIT) x $(WRKDIR)/prgaa $(@D)/03.bin
	$(BSPLIT) x $(WRKDIR)/prgab $(@D)/04.bin
	$(BSPLIT) x $(WRKDIR)/prgac $(@D)/05.bin
	$(BSPLIT) x $(WRKDIR)/prgad $(@D)/06.bin
	$(BSPLIT) x $(WRKDIR)/prgae $(@D)/07.bin
	$(BSPLIT) x $(WRKDIR)/prgaf $(@D)/08.bin
	$(BSPLIT) x $(WRKDIR)/prgag $(@D)/09.bin
	$(BSPLIT) x $(WRKDIR)/prgah $(@D)/10.bin
# Graphics
	cp $(RESDIR)/gfx.chr $(WRKDIR)/chr.bin
	$(BINPAD) $(WRKDIR)/chr.bin 0x2000000
	$(BSPLIT) s $(WRKDIR)/chr.bin $(WRKDIR)/chr_e.bin $(WRKDIR)/chr_o.bin
	$(SPLIT) -b 4194304 $(WRKDIR)/chr_e.bin $(WRKDIR)/chr_e_
	$(SPLIT) -b 4194304 $(WRKDIR)/chr_o.bin $(WRKDIR)/chr_o_
	mv $(WRKDIR)/chr_e_aa $(@D)/13.bin
	mv $(WRKDIR)/chr_e_ab $(@D)/15.bin
	mv $(WRKDIR)/chr_e_ac $(@D)/17.bin
	mv $(WRKDIR)/chr_e_ad $(@D)/19.bin
	mv $(WRKDIR)/chr_o_aa $(@D)/14.bin
	mv $(WRKDIR)/chr_o_ab $(@D)/16.bin
	mv $(WRKDIR)/chr_o_ac $(@D)/18.bin
	mv $(WRKDIR)/chr_o_ad $(@D)/20.bin
# Sound CPU
	dd if=/dev/zero bs=32K count=4 of=$(@D)/01.bin
	dd if=/dev/zero bs=32K count=4 of=$(@D)/02.bin
# QSound audio
	dd if=/dev/zero bs=1M count=4 of=$(WRKDIR)/blank4
	cp $(WRKDIR)/blank4 $(@D)/11.bin
	cp $(WRKDIR)/blank4 $(@D)/12.bin
# TODO: pick up from resdir

$(OUTDIR)/$(SAI_TESTROM_NAME)/sz3ud.03c: $(OUTDIR)/rom/03.bin $(BSPLIT) $(BINPAD)
	mkdir -p $(@D)
	mkdir -p $(WRKDIR)
	cp $(OUTDIR)/rom/03.bin $(@D)/sz3ud.03c
	cp $(OUTDIR)/rom/04.bin $(@D)/sz3ud.04c
	cp $(OUTDIR)/rom/05.bin $(@D)/sz3.05c
	cp $(OUTDIR)/rom/06.bin $(@D)/sz3.06c
	cp $(OUTDIR)/rom/07.bin $(@D)/sz3.07c
	cp $(OUTDIR)/rom/08.bin $(@D)/sz3.08c
	cp $(OUTDIR)/rom/09.bin $(@D)/sz3.09c
	cp $(OUTDIR)/rom/10.bin $(@D)/sz3.10b
	cp $(OUTDIR)/rom/13.bin $(@D)/sz3.13m
	cp $(OUTDIR)/rom/15.bin $(@D)/sz3.15m
	cp $(OUTDIR)/rom/17.bin $(@D)/sz3.17m
	cp $(OUTDIR)/rom/19.bin $(@D)/sz3.19m
	cp $(OUTDIR)/rom/14.bin $(@D)/sz3.14m
	cp $(OUTDIR)/rom/16.bin $(@D)/sz3.16m
	cp $(OUTDIR)/rom/18.bin $(@D)/sz3.18m
	cp $(OUTDIR)/rom/20.bin $(@D)/sz3.20m
	cp $(OUTDIR)/rom/01.bin $(@D)/sz3.01
	cp $(OUTDIR)/rom/02.bin $(@D)/sz3.02
	cp $(OUTDIR)/rom/11.bin $(@D)/sz3.11m
	cp $(OUTDIR)/rom/12.bin $(@D)/sz3.12m
	cp misc/phoenix.key $(@D)/
	cp misc/qsound_hle.zip $(OUTDIR)
	dd if=/dev/zero of=$(@D)/epr-13655.ic4 bs=1 count=1

mame: $(OUTDIR)/$(SAI_TESTROM_NAME)/sz3ud.03c
	mame $(SAI_TESTROM_NAME) -debug -r 1280x960 -prescale 4 -rompath $(shell pwd)/$(OUTDIR)

mamer: $(OUTDIR)/$(SAI_TESTROM_NAME)/sz3ud.03c
	mame $(SAI_TESTROM_NAME) -debug -r 960x1280 -prescale 4 -rompath $(shell pwd)/$(OUTDIR) -rol
