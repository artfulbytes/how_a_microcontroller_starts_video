# How a Microcontroller starts
Accompanying code for the video [How a Microcontroller
starts](https://youtu.be/MhOba73z-dQ) - watch the video for more details.

# Tools
<img src="/nucleo-f042k6.jpg">

* NUCLEO-F042K6
* VS Code with the Cortex-Debug extension

```
sudo apt install gcc-arm-none-eabi
sudo apt install openocd gdb-multiarch
```

# Commands
## Disassemble and size
Utilities for inspecting size and disassembly.

```
arm-none-eabi-size myprogram.elf
arm-none-eabi-objdump -D myprogram.elf | less
```

## Compile myprogram.c
Compile minimal C program.
```
arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -std=c11 -g -O0 -c myprogram_simple.c -o myprogram_simple.o
```

## Link WITH default libc (Newlib), startfiles, and linker script
Link with default behaviour of toolchain including default standard C library,
startfiles and linker script. Insert empty stubs (nosys.specs) for Newlib's
system calls.

```
arm-none-eabi-gcc --specs=nosys.specs myprogram_simple.o -o myprogram.elf
```

## Link WITHOUT libc, startfiles, and default linker script
Discard the default libc, startfiles, and linker script.

```
arm-none-eabi-gcc -nolibc -nostartfiles -T empty.ld -Wl,--verbose myprogram_simple.o -o myprogram.elf
```

## Link with custom startup file and linker script
Compile a custom startup file and link with custom linker script.

```
arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -std=c11 -g -O0 -c myprogram_with_vars.c -o myprogram_with_vars.o
arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -std=c11 -g -O0 -c startup_stm32f042k6.c -o startup_stm32f042k6.o
arm-none-eabi-gcc -nolibc -nostartfiles -T stm32f042k6.ld myprogram_with_vars.o startup_stm32f042k6.o -o myprogram.elf
```

## Link with libc (Newlib)
```
arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -std=c11 -g -O0 -c myprogram_with_libc.c -o myprogram_with_libc.o
arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -std=c11 -g -O0 -c startup_stm32f042k6_with_libc.c -o startup_stm32f042k6_with_libc.o
arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -nostartfiles --specs=nosys.specs --specs=nano.specs -T stm32f042k6.ld myprogram_with_libc.o startup_stm32f042k6_with_libc.o -o myprogram.elf
```
