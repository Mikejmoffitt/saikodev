
SAI_C2_TESTROM_NAME := bloxeedc

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

# TODO: Don't yet have a good test setup for anything larger than 512KB in size.
$(OUTDIR)/$(SAI_C2_TESTROM_NAME)/epr-12908.ic32: $(PROJECT_OUTPUT) $(BSPLIT) $(BINPAD)
	mkdir -p $(@D)
	mkdir -p $(WRKDIR)
	$(BINPAD) $(PROJECT_OUTPUT) 0x080000
	$(SPLIT) -b 262144 $(PROJECT_OUTPUT) $(WRKDIR)/prg
	$(BSPLIT) s $(WRKDIR)/prgaa $(@D)/epr-12908.ic32 $(@D)/epr-12907.ic31
	$(BSPLIT) s $(WRKDIR)/prgab $(@D)/epr-12993.ic34 $(@D)/epr-12992.ic33

mame: $(OUTDIR)/$(SAI_C2_TESTROM_NAME)/epr-12908.ic32
	mame $(SAI_C2_TESTROM_NAME) -debug -r 1280x960 -prescale 4 -rompath $(shell pwd)/$(OUTDIR)
