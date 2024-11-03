MEMORY
{
  flash (rx) : ORIGIN = 0x8000000, LENGTH = 32K
  ram (rwx) : ORIGIN = 0x20000000, LENGTH = 6K
}

_eram = ORIGIN(ram) + LENGTH(ram);
_stack = _eram; /* Stack starts at end of RAM and grows downwards */

SECTIONS
{
  .text : ALIGN(4)
  {
    KEEP(*(.ivt)) /* Interrupt vector table */
    *(.text*)
    . = ALIGN(4);
    _etext = .;
  } > flash

  .init_array : ALIGN(4)
  {
    __init_array_start = .;
    KEEP(*(.init_array*))
    __init_array_end = .;
    . = ALIGN(4);
  } > flash

  /* Initial (constant) values for data is placed in flash after the program code */
  _idata = LOADADDR(.data);

  .data : ALIGN(4)
  {
    _data = .;
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } > ram AT > flash

  .bss : ALIGN(4)
  {
    _bss = .;
    *(.bss*)
    . = ALIGN(4);
    _ebss = .;
    PROVIDE(_end = .);
  } > ram
}