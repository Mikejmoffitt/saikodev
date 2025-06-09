
SAI_S16B_TESTROM_NAME := ddcrewd

# Prepares images for use with 27C040 or compatible ROMs.
.PHONY: rom
rom: $(OUTDIR)/rom/a4.bin

$(OUTDIR)/rom/a4.bin: $(PROJECT_OUTPUT) $(BSPLIT) $(BINPAD)
	mkdir -p $(@D)
	mkdir -p $(WRKDIR)
	$(BINPAD) $(PROJECT_OUTPUT) 0x100000
	$(SPLIT) -b 524288 $(PROJECT_OUTPUT) $(WRKDIR)/prg
	$(BSPLIT) s $(WRKDIR)/prgaa $(@D)/a4.bin $(@D)/a6.bin
	$(BSPLIT) s $(WRKDIR)/prgab $(@D)/a5.bin $(@D)/a7.bin
# tiles
	dd if=/dev/zero of=$(@D)/c1.bin bs=64 count=4096
	cp $(@D)/c1.bin $(@D)/c2.bin
	cp $(@D)/c1.bin $(@D)/c3.bin
# sprites
	dd if=/dev/zero of=$(@D)/c10.bin bs=64 count=8192
	cp $(@D)/c10.bin $(@D)/a10.bin
	cp $(@D)/c10.bin $(@D)/c11.bin
	cp $(@D)/c10.bin $(@D)/a11.bin
	cp $(@D)/c10.bin $(@D)/c12.bin
	cp $(@D)/c10.bin $(@D)/a12.bin
	cp $(@D)/c10.bin $(@D)/c13.bin
	cp $(@D)/c10.bin $(@D)/a13.bin
# sound CPU
	dd if=/dev/zero of=$(@D)/c7.bin bs=64 count=2048
	dd if=/dev/zero of=$(@D)/c6.bin bs=64 count=8192
	cp $(@D)/c6.bin $(@D)/c5.bin
	cp $(@D)/c6.bin $(@D)/c4.bin

$(OUTDIR)/$(SAI_S16B_TESTROM_NAME)/bootleg_epr-14160.a4: rom
	mkdir -p $(@D)
	mkdir -p $(WRKDIR)
# prg
	cp $(OUTDIR)/rom/a4.bin $(@D)/bootleg_epr-14160.a4
	cp $(OUTDIR)/rom/a6.bin $(@D)/bootleg_epr-14161.a6
	cp $(OUTDIR)/rom/a5.bin $(@D)/mpr-14139.a5
	cp $(OUTDIR)/rom/a7.bin $(@D)/mpr-14141.a7
# tiles
	cp $(OUTDIR)/rom/c1.bin $(@D)/epr-14127.c1
	cp $(OUTDIR)/rom/c2.bin $(@D)/epr-14128.c2
	cp $(OUTDIR)/rom/c2.bin $(@D)/epr-14129.c3
# sprites
	cp $(OUTDIR)/rom/c10.bin $(@D)/mpr-14134.c10
	cp $(OUTDIR)/rom/a10.bin $(@D)/mpr-14142.a10
	cp $(OUTDIR)/rom/c11.bin $(@D)/mpr-14135.c11
	cp $(OUTDIR)/rom/a11.bin $(@D)/mpr-14143.a11
	cp $(OUTDIR)/rom/c12.bin $(@D)/mpr-14136.c12
	cp $(OUTDIR)/rom/a12.bin $(@D)/mpr-14144.a12
	cp $(OUTDIR)/rom/c13.bin $(@D)/epr-14137.c13
	cp $(OUTDIR)/rom/a13.bin $(@D)/epr-14145.a13
# sound CPU
	cp $(OUTDIR)/rom/c7.bin $(@D)/epr-14133.c7
	cp $(OUTDIR)/rom/c6.bin $(@D)/mpr-14132.c6
	cp $(OUTDIR)/rom/c5.bin $(@D)/mpr-14131.c5
	cp $(OUTDIR)/rom/c4.bin $(@D)/epr-14130.c4

mame: $(OUTDIR)/$(SAI_S16B_TESTROM_NAME)/bootleg_epr-14160.a4
	mame $(SAI_S16B_TESTROM_NAME) -debug -r 1280x896 -prescale 4 -rompath $(shell pwd)/$(OUTDIR)
