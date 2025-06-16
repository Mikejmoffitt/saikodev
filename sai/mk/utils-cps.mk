SAI_TESTROM_NAME := sfzch


.PHONY: rom
rom: $(OUTDIR)/rom/23.bin

$(OUTDIR)/rom/23.bin: $(PROJECT_OUTPUT) $(BSPLIT) $(BINPAD)
	mkdir -p $(@D)
	mkdir -p $(WRKDIR)
# Program
	$(BINPAD) $(PROJECT_OUTPUT) 0x400000
	$(SPLIT) -b 524288 $(PROJECT_OUTPUT) $(WRKDIR)/prg
	$(BSPLIT) x $(WRKDIR)/prgaa $(@D)/23.bin
	$(BSPLIT) x $(WRKDIR)/prgab $(@D)/22.bin
	$(BSPLIT) x $(WRKDIR)/prgac $(@D)/21.bin
	$(BSPLIT) x $(WRKDIR)/prgad $(@D)/20.bin
# Graphics
	cp $(RESDIR)/gfx.chr $(WRKDIR)/chr.bin
	$(BINPAD) $(WRKDIR)/chr.bin 0x2000000
	$(BSPLIT) s $(WRKDIR)/chr.bin $(WRKDIR)/chr_h.bin $(WRKDIR)/chr_l.bin 4
	$(BSPLIT) s $(WRKDIR)/chr_h.bin $(WRKDIR)/chr_0.bin $(WRKDIR)/chr_2.bin 2
	$(BSPLIT) s $(WRKDIR)/chr_l.bin $(WRKDIR)/chr_4.bin $(WRKDIR)/chr_6.bin 2
	$(SPLIT) -b 524288 $(WRKDIR)/chr_0.bin $(WRKDIR)/chr_0_
	$(SPLIT) -b 524288 $(WRKDIR)/chr_2.bin $(WRKDIR)/chr_2_
	$(SPLIT) -b 524288 $(WRKDIR)/chr_4.bin $(WRKDIR)/chr_4_
	$(SPLIT) -b 524288 $(WRKDIR)/chr_6.bin $(WRKDIR)/chr_6_
	cp $(WRKDIR)/chr_0_aa $(@D)/01.bin
	cp $(WRKDIR)/chr_2_aa $(@D)/02.bin
	cp $(WRKDIR)/chr_4_aa $(@D)/03.bin
	cp $(WRKDIR)/chr_6_aa $(@D)/04.bin
	cp $(WRKDIR)/chr_0_ab $(@D)/05.bin
	cp $(WRKDIR)/chr_2_ab $(@D)/06.bin
	cp $(WRKDIR)/chr_4_ab $(@D)/07.bin
	cp $(WRKDIR)/chr_6_ab $(@D)/08.bin
	cp $(WRKDIR)/chr_0_ac $(@D)/10.bin
	cp $(WRKDIR)/chr_2_ac $(@D)/11.bin
	cp $(WRKDIR)/chr_4_ac $(@D)/12.bin
	cp $(WRKDIR)/chr_6_ac $(@D)/13.bin
	cp $(WRKDIR)/chr_0_ad $(@D)/14.bin
	cp $(WRKDIR)/chr_2_ad $(@D)/15.bin
	cp $(WRKDIR)/chr_4_ad $(@D)/16.bin
	cp $(WRKDIR)/chr_6_ad $(@D)/17.bin
# Sound CPU
	dd if=/dev/zero bs=32K count=1 of=$(WRKDIR)/blank32
	cp $(WRKDIR)/blank32 $(@D)/09.bin
# ADPCM
	dd if=/dev/zero bs=128K count=1 of=$(WRKDIR)/blank128
	cp $(WRKDIR)/blank128 $(@D)/18.bin
	cp $(WRKDIR)/blank128 $(@D)/19.bin

$(OUTDIR)/$(SAI_TESTROM_NAME)/sfzch23.8f: $(OUTDIR)/rom/23.bin $(BSPLIT) $(BINPAD)
	mkdir -p $(@D)
	mkdir -p $(WRKDIR)
	cp $(OUTDIR)/rom/23.bin $(@D)/sfzch23
	cp $(OUTDIR)/rom/22.bin $(@D)/sfza22
	cp $(OUTDIR)/rom/21.bin $(@D)/sfzch21
	cp $(OUTDIR)/rom/20.bin $(@D)/sfza20
	cp $(OUTDIR)/rom/01.bin $(@D)/sfz_01.3a
	cp $(OUTDIR)/rom/02.bin $(@D)/sfz_02.4a
	cp $(OUTDIR)/rom/03.bin $(@D)/sfz_03.5a
	cp $(OUTDIR)/rom/04.bin $(@D)/sfz_04.6a
	cp $(OUTDIR)/rom/05.bin $(@D)/sfz_05.7a
	cp $(OUTDIR)/rom/06.bin $(@D)/sfz_06.8a
	cp $(OUTDIR)/rom/07.bin $(@D)/sfz_07.9a
	cp $(OUTDIR)/rom/08.bin $(@D)/sfz_08.10a
	cp $(OUTDIR)/rom/10.bin $(@D)/sfz_10.3c
	cp $(OUTDIR)/rom/11.bin $(@D)/sfz_11.4c
	cp $(OUTDIR)/rom/12.bin $(@D)/sfz_12.5c
	cp $(OUTDIR)/rom/13.bin $(@D)/sfz_13.6c
	cp $(OUTDIR)/rom/14.bin $(@D)/sfz_14.7c
	cp $(OUTDIR)/rom/15.bin $(@D)/sfz_15.8c
	cp $(OUTDIR)/rom/16.bin $(@D)/sfz_16.9c
	cp $(OUTDIR)/rom/17.bin $(@D)/sfz_17.10c
	cp $(OUTDIR)/rom/09.bin $(@D)/sfz_09.12a
	cp $(OUTDIR)/rom/18.bin $(@D)/sfz_18.11c
	cp $(OUTDIR)/rom/19.bin $(@D)/sfz_19.12c
# The PLD dump is not actually used, so
	dd if=/dev/zero of=$(@D)/sfz63b.1a bs=1 count=104

# Old news for testing on SF2HF

# for sf2hfj and other similarly shaped boards
$(OUTDIR)/rom/8f.bin: $(PROJECT_OUTPUT) $(BSPLIT) $(BINPAD)
	mkdir -p $(@D)
	mkdir -p $(WRKDIR)
# Program
	$(BINPAD) $(PROJECT_OUTPUT) 0x400000
	$(SPLIT) -b 524288 $(PROJECT_OUTPUT) $(WRKDIR)/prg
	$(BSPLIT) x $(WRKDIR)/prgaa $(@D)/8f.bin
	$(BSPLIT) x $(WRKDIR)/prgab $(@D)/7f.bin
	$(BSPLIT) x $(WRKDIR)/prgac $(@D)/6f.bin
# Graphics
	cp $(RESDIR)/spr.chr $(WRKDIR)/chr.bin
	$(BINPAD) $(WRKDIR)/chr.bin 0x2000000
	$(BSPLIT) s $(WRKDIR)/chr.bin $(WRKDIR)/chr_h.bin $(WRKDIR)/chr_l.bin 4
	$(BSPLIT) s $(WRKDIR)/chr_h.bin $(WRKDIR)/chr_0.bin $(WRKDIR)/chr_2.bin 2
	$(BSPLIT) s $(WRKDIR)/chr_l.bin $(WRKDIR)/chr_4.bin $(WRKDIR)/chr_6.bin 2
	$(SPLIT) -b 524288 $(WRKDIR)/chr_0.bin $(WRKDIR)/chr_0_
	$(SPLIT) -b 524288 $(WRKDIR)/chr_2.bin $(WRKDIR)/chr_2_
	$(SPLIT) -b 524288 $(WRKDIR)/chr_4.bin $(WRKDIR)/chr_4_
	$(SPLIT) -b 524288 $(WRKDIR)/chr_6.bin $(WRKDIR)/chr_6_
	cp $(WRKDIR)/chr_0_aa $(@D)/3a.bin
	cp $(WRKDIR)/chr_2_aa $(@D)/4a.bin
	cp $(WRKDIR)/chr_4_aa $(@D)/5a.bin
	cp $(WRKDIR)/chr_6_aa $(@D)/6a.bin
	cp $(WRKDIR)/chr_0_ab $(@D)/7a.bin
	cp $(WRKDIR)/chr_2_ab $(@D)/8a.bin
	cp $(WRKDIR)/chr_4_ab $(@D)/9a.bin
	cp $(WRKDIR)/chr_6_ab $(@D)/10a.bin
	cp $(WRKDIR)/chr_0_ac $(@D)/3c.bin
	cp $(WRKDIR)/chr_2_ac $(@D)/4c.bin
	cp $(WRKDIR)/chr_4_ac $(@D)/5c.bin
	cp $(WRKDIR)/chr_6_ac $(@D)/6c.bin
# Sound CPU
	dd if=/dev/zero bs=32K count=1 of=$(WRKDIR)/blank32
	cp $(WRKDIR)/blank32 $(@D)/12a.bin
# ADPCM
	dd if=/dev/zero bs=128K count=1 of=$(WRKDIR)/blank128
	cp $(WRKDIR)/blank128 $(@D)/11c.bin
	cp $(WRKDIR)/blank128 $(@D)/12c.bin

mame: $(OUTDIR)/$(SAI_TESTROM_NAME)/sfzch23.8f
	mame $(SAI_TESTROM_NAME) -debug -r 1280x960 -prescale 4 -rompath $(shell pwd)/$(OUTDIR)

mamet: $(OUTDIR)/$(SAI_TESTROM_NAME)/sfzch23.8f
	mame $(SAI_TESTROM_NAME) -debug -r 960x1280 -prescale 4 -rompath $(shell pwd)/$(OUTDIR) -rol
