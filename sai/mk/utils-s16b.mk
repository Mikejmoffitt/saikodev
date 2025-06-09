
SAI_S16B_TESTROM_NAME := cottond

# Prepares images for use with 27C040 or compatible ROMs.
.PHONY: rom
rom: $(OUTDIR)/rom/a7.bin

$(OUTDIR)/rom/a7.bin: $(PROJECT_OUTPUT) $(BSPLIT) $(BINPAD)
	mkdir -p $(@D)
	mkdir -p $(WRKDIR)
	$(BINPAD) $(PROJECT_OUTPUT) 0x80000
	$(SPLIT) -b 262144 $(PROJECT_OUTPUT) $(WRKDIR)/prg
	$(BSPLIT) s $(WRKDIR)/prgaa $(@D)/a7.bin $(@D)/a5.bin
	$(BSPLIT) s $(WRKDIR)/prgab $(@D)/a8.bin $(@D)/a6.bin
# tiles
	dd if=/dev/zero of=$(@D)/a14.bin bs=64 count=2048
	cp $(@D)/a14.bin $(@D)/a15.bin
	cp $(@D)/a14.bin $(@D)/a16.bin
	cp $(@D)/a14.bin $(@D)/b14.bin
	cp $(@D)/a14.bin $(@D)/b15.bin
	cp $(@D)/a14.bin $(@D)/b16.bin
# sprites
	cp $(@D)/a14.bin $(@D)/b5.bin
	cp $(@D)/a14.bin $(@D)/b1.bin
	cp $(@D)/a14.bin $(@D)/b6.bin
	cp $(@D)/a14.bin $(@D)/b2.bin
	cp $(@D)/a14.bin $(@D)/b7.bin
	cp $(@D)/a14.bin $(@D)/b3.bin
	cp $(@D)/a14.bin $(@D)/b8.bin
	cp $(@D)/a14.bin $(@D)/b4.bin
	cp $(@D)/a14.bin $(@D)/b10.bin
	cp $(@D)/a14.bin $(@D)/a1.bin
	cp $(@D)/a14.bin $(@D)/b11.bin
	cp $(@D)/a14.bin $(@D)/a2.bin
	cp $(@D)/a14.bin $(@D)/b12.bin
	cp $(@D)/a14.bin $(@D)/a3.bin
	cp $(@D)/a14.bin $(@D)/b13.bin
	cp $(@D)/a14.bin $(@D)/a4.bin
# sound CPU
	dd if=/dev/zero of=$(@D)/a10.bin bs=64 count=512
	cp $(@D)/a14.bin $(@D)/a11.bin
# pld...
	dd if=/dev/zero of=$(@D)/b9.bin bs=1 count=235

$(OUTDIR)/$(SAI_S16B_TESTROM_NAME)/bootleg_epr-13921a.a7: rom
	mkdir -p $(@D)
	mkdir -p $(WRKDIR)
# prg
	cp $(OUTDIR)/rom/a7.bin $(@D)/bootleg_epr-13921a.a7
	cp $(OUTDIR)/rom/a5.bin $(@D)/bootleg_epr-13919a.a5
	cp $(OUTDIR)/rom/a8.bin $(@D)/bootleg_epr-13922a.a8
	cp $(OUTDIR)/rom/a6.bin $(@D)/bootleg_epr-13920a.a6
# tiles
	cp $(OUTDIR)/rom/a14.bin $(@D)/opr-13862.a14
	cp $(OUTDIR)/rom/b14.bin $(@D)/opr-13877.b14
	cp $(OUTDIR)/rom/a15.bin $(@D)/opr-13863.a15
	cp $(OUTDIR)/rom/b15.bin $(@D)/opr-13878.b15
	cp $(OUTDIR)/rom/a16.bin $(@D)/opr-13864.a16
	cp $(OUTDIR)/rom/b16.bin $(@D)/opr-13879.b16
# sprites
	cp $(OUTDIR)/rom/b5.bin $(@D)/opr-13869.b5
	cp $(OUTDIR)/rom/b1.bin $(@D)/opr-13865.b1
	cp $(OUTDIR)/rom/b6.bin $(@D)/opr-13870.b6
	cp $(OUTDIR)/rom/b2.bin $(@D)/opr-13866.b2
	cp $(OUTDIR)/rom/b7.bin $(@D)/opr-13871.b7
	cp $(OUTDIR)/rom/b3.bin $(@D)/opr-13867.b3
	cp $(OUTDIR)/rom/b8.bin $(@D)/opr-13872.b8
	cp $(OUTDIR)/rom/b4.bin $(@D)/opr-13868.b4
	cp $(OUTDIR)/rom/b10.bin $(@D)/opr-13873.b10
	cp $(OUTDIR)/rom/a1.bin $(@D)/opr-13852.a1
	cp $(OUTDIR)/rom/b11.bin $(@D)/opr-13874.b11
	cp $(OUTDIR)/rom/a2.bin $(@D)/opr-13853.a2
	cp $(OUTDIR)/rom/b12.bin $(@D)/opr-13894.b12
	cp $(OUTDIR)/rom/a3.bin $(@D)/opr-13891.a3
	cp $(OUTDIR)/rom/b13.bin $(@D)/opr-13876.b13
	cp $(OUTDIR)/rom/a4.bin $(@D)/opr-13855.a4
# sound CPU
	cp $(OUTDIR)/rom/a10.bin $(@D)/epr-13892.a10
	cp $(OUTDIR)/rom/a11.bin $(@D)/opr-13893.a11
# pld
	cp $(OUTDIR)/rom/b9.bin $(@D)/315-5298.b9

mame: $(OUTDIR)/$(SAI_S16B_TESTROM_NAME)/bootleg_epr-13921a.a7
	mame $(SAI_S16B_TESTROM_NAME) -debug -r 1280x896 -prescale 4 -rompath $(shell pwd)/$(OUTDIR)
