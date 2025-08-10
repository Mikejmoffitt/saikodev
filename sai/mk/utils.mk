
CC_HOST := cc
HOSTCFLAGAS := -O2 -std=gnu23 -Werror -Wall -Wpedantic

SAI_UTILDIR := $(realpath $(SAI)/../util)

# Utilities packed in with Saikodev
BIN2S := $(SAI_UTILDIR)/core/bin2s
BIN2H := $(SAI_UTILDIR)/core/bin2h
BINPAD := $(SAI_UTILDIR)/core/binpad
BSPLIT := $(SAI_UTILDIR)/core/bsplit
VELELLA := $(SAI_UTILDIR)/velella/velella
YMZTOOL := $(SAI_UTILDIR)/ymztool/ymztool

$(BIN2S): $(SAI_UTILDIR)/core/bin2s.c
	@$(CC_HOST) $^ -o $@ $(HOSTCFLAGS)

$(BIN2H): $(SAI_UTILDIR)/core/bin2h.c
	@$(CC_HOST) $^ -o $@ $(HOSTCFLAGS)

$(BINPAD): $(SAI_UTILDIR)/core/binpad.c
	@$(CC_HOST) $^ -o $@ $(HOSTCFLAGS)

$(BSPLIT): $(SAI_UTILDIR)/core/bsplit.c
	@$(CC_HOST) $^ -o $@ $(HOSTCFLAGS)

.PHONY: $(VELELLA)
$(VELELLA):
	make -C $(@D)

.PHONY: $(YMZTOOL)
$(YMZTOOL):
	make -C $(@D)
