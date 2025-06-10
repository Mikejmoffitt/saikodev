SAI_TESTROM_NAME := sf2hfj

# TODO
.PHONY: rom
rom: $(OUTDIR)/rom/8f.bin

$(OUTDIR)/rom/8f.bin: $(PROJECT_OUTPUT) $(BSPLIT) $(BINPAD)
	mkdir -p $(@D)
	mkdir -p $(WRKDIR)
# Program
	$(BINPAD) $(PROJECT_OUTPUT) 0x200000
	$(SPLIT) -b 524288 $(PROJECT_OUTPUT) $(WRKDIR)/prg
	$(BSPLIT) x $(WRKDIR)/prgaa $(@D)/8f.bin
	$(BSPLIT) x $(WRKDIR)/prgab $(@D)/7f.bin
	$(BSPLIT) x $(WRKDIR)/prgac $(@D)/6f.bin
# Graphics
	dd if=/dev/zero bs=512K count=1 of=$(WRKDIR)/blank512
	cp $(WRKDIR)/blank512 $(@D)/3a.bin
	cp $(WRKDIR)/blank512 $(@D)/4a.bin
	cp $(WRKDIR)/blank512 $(@D)/5a.bin
	cp $(WRKDIR)/blank512 $(@D)/6a.bin
	cp $(WRKDIR)/blank512 $(@D)/7a.bin
	cp $(WRKDIR)/blank512 $(@D)/8a.bin
	cp $(WRKDIR)/blank512 $(@D)/9a.bin
	cp $(WRKDIR)/blank512 $(@D)/10a.bin
	cp $(WRKDIR)/blank512 $(@D)/3c.bin
	cp $(WRKDIR)/blank512 $(@D)/4c.bin
	cp $(WRKDIR)/blank512 $(@D)/5c.bin
	cp $(WRKDIR)/blank512 $(@D)/6c.bin
# Sound CPU
	dd if=/dev/zero bs=32K count=1 of=$(WRKDIR)/blank32
	cp $(WRKDIR)/blank32 $(@D)/12a.bin
# ADPCM
	dd if=/dev/zero bs=128K count=1 of=$(WRKDIR)/blank128
	cp $(WRKDIR)/blank128 $(@D)/11c.bin
	cp $(WRKDIR)/blank128 $(@D)/12c.bin


# TODO

$(OUTDIR)/$(SAI_TESTROM_NAME)/s2tj_23.8f: $(OUTDIR)/rom/8f.bin $(BSPLIT) $(BINPAD)
	mkdir -p $(@D)
	mkdir -p $(WRKDIR)
	cp $(OUTDIR)/rom/8f.bin $(@D)/s2tj_23.8f
	cp $(OUTDIR)/rom/7f.bin $(@D)/s2tj_22.7f
	cp $(OUTDIR)/rom/6f.bin $(@D)/s2tj_21.6f
	cp $(OUTDIR)/rom/3a.bin $(@D)/s92_01.3a
	cp $(OUTDIR)/rom/4a.bin $(@D)/s92_02.4a
	cp $(OUTDIR)/rom/5a.bin $(@D)/s92_03.5a
	cp $(OUTDIR)/rom/6a.bin $(@D)/s92_04.6a
	cp $(OUTDIR)/rom/7a.bin $(@D)/s92_05.7a
	cp $(OUTDIR)/rom/8a.bin $(@D)/s92_06.8a
	cp $(OUTDIR)/rom/9a.bin $(@D)/s92_07.9a
	cp $(OUTDIR)/rom/10a.bin $(@D)/s92_08.10a
	cp $(OUTDIR)/rom/3c.bin $(@D)/s2t_10.3c
	cp $(OUTDIR)/rom/4c.bin $(@D)/s2t_11.4c
	cp $(OUTDIR)/rom/5c.bin $(@D)/s2t_12.5c
	cp $(OUTDIR)/rom/6c.bin $(@D)/s2t_13.6c
	cp $(OUTDIR)/rom/12a.bin $(@D)/s92_09.12a
	cp $(OUTDIR)/rom/11c.bin $(@D)/s92_18.11c
	cp $(OUTDIR)/rom/12c.bin $(@D)/s92_19.12c
	cp pld/* $(@D)/

mame: $(OUTDIR)/$(SAI_TESTROM_NAME)/s2tj_23.8f
	mame $(SAI_TESTROM_NAME) -debug -r 1280x960 -prescale 4 -rompath $(shell pwd)/$(OUTDIR)

mamet: $(OUTDIR)/$(SAI_TESTROM_NAME)/s2tj_23.8f
	mame $(SAI_TESTROM_NAME) -debug -r 960x1280 -prescale 4 -rompath $(shell pwd)/$(OUTDIR) -rol
