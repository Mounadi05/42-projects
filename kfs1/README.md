# kfs1

Tiny 32-bit kernel skeleton that boots through GRUB and writes a greeting
directly to the VGA text buffer. This project is a starting point for
experiments with low-level OS development.

## Prerequisites

- `nasm` to assemble the Multiboot-compliant loader
- `gcc` with i386 support (e.g., `-m32`)
- `ld` from binutils for linking
- `grub` for creating the bootable ISO

## Build

```bash
make iso      
```


## Project layout

- `boot.asm` – Multiboot header and hand-off to the C kernel
- `kernel.c` – Minimal kernel that writes to VGA memory
- `linker.ld` – Places sections at the 1 MiB mark as expected by GRUB
- `Makefile` – Build targets for the kernel binary, ISO, and clean up


