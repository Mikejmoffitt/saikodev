
SAI_C2_TESTROM_NAME := ooparts

$(OUTDIR)/$(SAI_C2_TESTROM_NAME)/ooparts.ic32: $(PROJECT_OUTPUT) $(BSPLIT) $(BINPAD)
	mkdir -p $(@D)
	mkdir -p $(WRKDIR)
	$(BINPAD) $(PROJECT_OUTPUT) 0x200000
	$(SPLIT) -b 1048576 $(PROJECT_OUTPUT) $(WRKDIR)/prg
	$(BSPLIT) s $(WRKDIR)/prgaa $(@D)/ooparts.ic32 $(@D)/ooparts.ic31
	$(BSPLIT) s $(WRKDIR)/prgab $(@D)/ooparts.ic34 $(@D)/ooparts.ic33
	# TODO adpcm
	dd if=/dev/zero of=$(@D)/epr-13655.ic4 bs=1 count=1

mame: $(OUTDIR)/$(SAI_C2_TESTROM_NAME)/ooparts.ic32
	mame ooparts -debug -r 1280x960 -prescale 4 -rompath $(shell pwd)/$(OUTDIR) -ror
