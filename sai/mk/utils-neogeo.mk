
SAI_TESTROM_NAME := mslug2

.PHONY: rom
rom: $(OUTDIR)/rom/241-p1.p1

$(OUTDIR)/rom/241-p1.p1: $(PROJECT_OUTPUT) $(BSPLIT) $(BINPAD)
	mkdir -p $(@D)
	mkdir -p $(WRKDIR)
# Program data
	$(BINPAD) $(PROJECT_OUTPUT) 0x100000
	$(BSPLIT) x $(PROJECT_OUTPUT) $@
# Upper bank (TODO)
	cp $(PROJECT_OUTPUT) $(@D)/241-p2.sp2
# FIX data
	cp $(RESDIR)/fix.s1 $(@D)/241-s1.s1
	$(BINPAD) $(@D)/241-s1.s1 0x20000
# Z80 program TODO
	dd if=/dev/zero of=$(@D)/241-m1.m1 bs=128K count=1
# ADPCM Data TODO
	dd if=/dev/zero of=$(@D)/241-v1.v1 bs=1M count=4
	dd if=/dev/zero of=$(@D)/241-v2.v2 bs=1M count=4
# Sprite data TODO
	cp $(RESDIR)/chr.c1 $(@D)/241-c1.c1
	cp $(RESDIR)/chr.c2 $(@D)/241-c2.c2
	cp $(RESDIR)/chr.c1 $(@D)/241-c3.c3
	cp $(RESDIR)/chr.c2 $(@D)/241-c4.c4

$(OUTDIR)/$(SAI_TESTROM_NAME)/241-p1.p1: $(OUTDIR)/rom/241-p1.p1 $(BSPLIT) $(BINPAD)
	mkdir -p $(@D)
	cp $(OUTDIR)/rom/* $(@D)/

mame: $(OUTDIR)/$(SAI_TESTROM_NAME)/241-p1.p1
	mame $(SAI_TESTROM_NAME) -debug -r 1280x896 -prescale 4 -rompath $(shell pwd)/$(OUTDIR)
