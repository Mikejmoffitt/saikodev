OUTPUT_ARCH(m68k)
SEARCH_DIR(.)
__DYNAMIC = 0;

MEMORY
{
	rom :	ORIGIN = 0x00000000, LENGTH = 0x00100000
	ram :	ORIGIN = 0x00100000, LENGTH = 0x0000F300
	sysram :	ORIGIN = 0x0010FD80, LENGTH = 0x00000280
}

PROVIDE (__stack = 0x0010F300);

SECTIONS
{
	.text ORIGIN(rom) :
	{
		__text_start = .;
		KEEP(*(.text.keepboot))
		. = 0x182;
		KEEP(*(.text.security));
		. = ALIGN(0x2);
		*(.text .text.*)
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

	.data ORIGIN(ram) :
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

	__rom_size = 0x100000;

	__bss_size = SIZEOF (.bss);

	.sysram (NOLOAD) :
	{
		__sysram_start = .;
		*(.sysram .sysram.*)
		. = ALIGN(0x2);
		__sysram_end = .;
	} > sysram
}
