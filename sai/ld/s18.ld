OUTPUT_ARCH(m68k)
SEARCH_DIR(.)
__DYNAMIC = 0;

MEMORY
{
	rom :	ORIGIN = 0x00000000, LENGTH = 0x00100000
	ram :	ORIGIN = 0x00E00000, LENGTH = 0x00004000
}

PROVIDE (__stack = 0x00E04000);

SECTIONS
{
	.text 0x00000000 :
	{
		KEEP(*(.text.keepboot)) *(.text .text.*)
		. = ALIGN(0x2);
		_etext = .;
		. = ALIGN(0x2);
		*(.rodata .rodata.*)
		. = ALIGN(0x2);
		*(.lit)
	} > rom

	_stext = SIZEOF (.text);

	.data 0x00E00000 :
	AT ( ADDR (.text) + SIZEOF (.text) )
	{
		. = ALIGN(0x2);
		*(.data .data.*)
		. = ALIGN(0x2);
		_edata = .;
	} > ram
	_sdata = SIZEOF (.data);

	.bss 0x00E00000 + SIZEOF (.data) :
	{
		. = ALIGN(0x2);
		_start = . ;
		*(.bss .bss.*)
		. = ALIGN(0x2);
		_bend = . ;
		end = . ;
	} > ram
}
