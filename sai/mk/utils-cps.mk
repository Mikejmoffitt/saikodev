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
	dd if=/dev/zero bs=1M count=4 of=$(WRKDIR)/blank4
# TODO

$(OUTDIR)/$(SAI_TESTROM_NAME)/s2tj_23.8f: $(OUTDIR)/rom/03.bin $(BSPLIT) $(BINPAD)
	mkdir -p $(@D)
	mkdir -p $(WRKDIR)

mame: $(OUTDIR)/$(SAI_TESTROM_NAME)/s2tj_23.8f
	mame $(SAI_TESTROM_NAME) -debug -r 1280x960 -prescale 4 -rompath $(shell pwd)/$(OUTDIR)

mamet: $(OUTDIR)/$(SAI_TESTROM_NAME)/s2tj_23.8f
	mame $(SAI_TESTROM_NAME) -debug -r 960x1280 -prescale 4 -rompath $(shell pwd)/$(OUTDIR) -rol
