
SAI_TESTROM_NAME := ooparts

# Prepares images for use with 27C040 or compatible ROMs.
.PHONY: rom
rom: $(OUTDIR)/rom/ic32.bin

$(OUTDIR)/rom/ic32.bin: $(PROJECT_OUTPUT) $(BSPLIT) $(BINPAD)
	mkdir -p $(@D)
	mkdir -p $(WRKDIR)
	$(BINPAD) $(PROJECT_OUTPUT) 0x200000
	$(SPLIT) -b 1048576 $(PROJECT_OUTPUT) $(WRKDIR)/prg
	$(BSPLIT) s $(WRKDIR)/prgaa $(@D)/ic32.bin $(@D)/ic31.bin
	$(BSPLIT) s $(WRKDIR)/prgab $(@D)/ic34.bin $(@D)/ic33.bin

$(OUTDIR)/$(SAI_TESTROM_NAME)/ooparts.ic32: $(PROJECT_OUTPUT) $(BSPLIT) $(BINPAD)
	mkdir -p $(@D)
	mkdir -p $(WRKDIR)
	$(BINPAD) $(PROJECT_OUTPUT) 0x200000
	$(SPLIT) -b 1048576 $(PROJECT_OUTPUT) $(WRKDIR)/prg
	$(BSPLIT) s $(WRKDIR)/prgaa $(@D)/ooparts.ic32 $(@D)/ooparts.ic31
	$(BSPLIT) s $(WRKDIR)/prgab $(@D)/ooparts.ic34 $(@D)/ooparts.ic33
	# TODO adpcm
	dd if=/dev/zero of=$(@D)/epr-13655.ic4 bs=1 count=1

mame: $(OUTDIR)/$(SAI_TESTROM_NAME)/ooparts.ic32
	mame $(SAI_TESTROM_NAME) -debug -r 1280x960 -prescale 4 -rompath $(shell pwd)/$(OUTDIR) -ror
