OUTPUT_ARCH(m68k)
SEARCH_DIR(.)
__DYNAMIC = 0;

MEMORY
{
	rom :	ORIGIN = 0x00000000, LENGTH = 0x00080000
	ram :	ORIGIN = 0x00100000, LENGTH = 0x00010000
}

PROVIDE (__stack = 0x00110000);

SECTIONS
{
	.text 0x00000000 :
	{
		__text_start = .;
		KEEP(*(.text.keepboot)) *(.text .text.*)
		. = ALIGN(0x2);
		__text_end = .;
		. = ALIGN(0x2);
		__rodata_start = .;
		*(.rodata .rodata.*)
		. = ALIGN(0x2);
		__rodata_end = .;
		*(.lit)
	} > rom

	__text_size = SIZEOF (.text);
	__data_src = .;

	.data 0x00100000 :
	AT ( ADDR (.text) + SIZEOF (.text) )
	{
		__data_start = .;
		*(.data .data.*)
		. = ALIGN(0x2);
		__data_end = .;
	} > ram

	__data_size = SIZEOF (.data);

	.bss (NOLOAD) :
	{
		. = ALIGN(0x2);
		__bss_start = .;
		*(.bss .bss.*)
		. = ALIGN(0x2);
		__bss_end = .;
	} > ram

	__bss_size = SIZEOF (.bss);
}
