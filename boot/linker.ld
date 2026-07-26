ENTRY(start)

SECTIONS
{
  /* Load at 0x7C00 for BIOS boot */
  . = 0x7C00;

  /* Place multiboot headers first */
  .multiboot:
  {
    *(.multiboot_headers)
  }

  /* Code and data */
  .text :
  {
    *(.text)
  }

  .rodata :
  {
    *(.rodata)
  }

  .data :
  {
    *(.data)
  }

  .bss :
  {
    *(.bss COMMON)
  }
}
