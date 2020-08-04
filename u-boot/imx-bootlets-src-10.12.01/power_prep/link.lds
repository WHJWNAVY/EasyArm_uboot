OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	. = 0x00000000;
	. = ALIGN(4);
	.text : { *(.text) }
	.data : { *(.data) }
	.bss  : { *(.bss) }
}


