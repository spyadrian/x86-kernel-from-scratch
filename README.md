# Yoctos

A small **x86 (32-bit) operating system** written from scratch in C and assembly – hepia, Advanced Systems Programming, 2024-2025.

## Features

- Boots with **GRUB** (Multiboot), BIOS or UEFI
- **GDT / IDT**, interrupt and IRQ handling, PIC and **timer**
- **Paging** and physical frame allocator
- **Graphics driver** (framebuffer) with font rendering and `printf`
- **PS/2 keyboard** driver with a Swiss-French keymap
- **Tasks** (TSS, task creation / loading / freeing) and **system calls**
- Minimal user-space C library (`ulibc`) and test applications loaded as GRUB modules

## Build & run

Requirements: `gcc` (with 32-bit support), `nasm`, `grub-mkrescue`, `xorriso`, `qemu-system-i386`.

```bash
make run                 # build the ISO and boot it in QEMU
make run SYSTEM=BIOS     # BIOS instead of UEFI
make debug               # run with GDB support
make help                # all targets
```

## Layout

| Folder | Content |
|---|---|
| `kernel/` | boot, memory (GDT, paging, frames), interrupts, drivers, tasks, syscalls |
| `common/` | code shared by kernel and user space (string, stdio, mem) |
| `user/` | user-space library and applications |
| `grub/` | GRUB configuration |

## Tech

C · x86 assembly · GRUB / Multiboot · QEMU · GDB · Make
