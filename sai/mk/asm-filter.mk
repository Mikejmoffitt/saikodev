# A long gawk command that does some brute filtering of assembly sources so that
# we may write 68000 assembly like normal humans, instead of the weird business
# that m68k-gcc-as wants.

# This command replaces the call to $(AS).
# TODO: This is boorish and the awk script should really be its own file.
define filteras
	awk '{gsub(/;/,";#"); printf("%s", $$0 RT)}' RS='"[^"]*"' $< | awk '{gsub(/\$$/,"0x"); printf("%s", $$0 RT)}' RS='"[^"]*"' | $(AS) $(ASFLAGS) -o $@ -c -
endef

