
CC_HOST := cc
HOSTCFLAGAS := -O2 -std=gnu23 -Werror -Wall -Wpedantic

UTILDIR := $(realpath $(SAI)/../util)

# Utilities packed in with Saikodev
BIN2S := $(UTILDIR)/core/bin2s
BIN2H := $(UTILDIR)/core/bin2h
BINPAD := $(UTILDIR)/core/binpad
BSPLIT := $(UTILDIR)/core/bsplit

$(BIN2S): $(UTILDIR)/core/bin2s.c
	@$(CC_HOST) $^ -o $@ $(HOSTCFLAGS)

$(BIN2H): $(UTILDIR)/core/bin2h.c
	@$(CC_HOST) $^ -o $@ $(HOSTCFLAGS)

$(BINPAD): $(UTILDIR)/core/binpad.c
	@$(CC_HOST) $^ -o $@ $(HOSTCFLAGS)

$(BSPLIT): $(UTILDIR)/core/bsplit.c
	@$(CC_HOST) $^ -o $@ $(HOSTCFLAGS)
