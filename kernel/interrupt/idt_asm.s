%include "const.inc"

; --------------------------------------------------
; MACRO

; generate global function
%macro gen_global_sym 3
%assign i %2
%rep (%3 - %2 + 1)

    global _%1%+ i

%assign i i+1

%endrep
%endmacro

; generate interrupt/exception that don't have an error code [start, end]
%macro gen_int_no_err 3
%assign i %2
%rep (%3 - %2 + 1)

    _%1%+ i:
        push 0                      ; dummy error code
        push i                      ; number exception
        jmp %1 %+ _wrapper

%assign i i+1

%endrep
%endmacro

; generate interrupt/exception that already have an error code [start, end]
%macro gen_int_err 3
%assign i %2
%rep (%3 - %2 + 1)

    _%1%+ i:
        push i                     ; number exception
        jmp %1 %+ _wrapper

%assign i i+1

%endrep
%endmacro

; --------------------------------------------------
; Make symbols below visible to the linker

global idt_load

gen_global_sym exception, 0, 20
gen_global_sym irq, 0, 14

section .text                      ; start of the text (code) section
align 4                            ; the code must be 4 byte aligned

; --------------------------------------------------
; Loads the IDT specified in argument.
; void idt_load(idt_ptr_t *idt_ptr);
idt_load:
    mov     eax,[esp+4]  ; Get the pointer to the IDT, passed as a parameter. 
    lidt    [eax]        ; Load the IDT pointer.
    ret

; --------------------------------------------------
; First part of low level exception handlers

gen_int_no_err exception, 0, 7
gen_int_err exception, 8, 8
gen_int_no_err exception, 9, 9
gen_int_err exception, 10, 14
gen_int_no_err exception, 15, 16
gen_int_err exception, 17, 17
gen_int_err exception, 18, 20

; --------------------------------------------------
; First part of low level hardware interrupt handlers

gen_int_no_err irq, 0, 14

; --------------------------------------------------
; Second part of low level exception handler
; Calls the high level handler (C function)

; High level handler for exceptions (C function) 
extern exception_handler

exception_wrapper:
    ; Save all registers
    push    eax
    push    ebx
    push    ecx
    push    edx
    push    esi
    push    edi
    push    ebp
    push    ds
    push    es
    push    fs
    push    gs

    ; Load kernel data descriptor into all segments
    mov     ax,GDT_KERNEL_DATA_SELECTOR
    mov     ds,ax
    mov     es,ax
    mov     fs,ax
    mov     gs,ax

    ; Pass the stack pointer (which gives the CPU context) to the C function
    mov     eax,esp
    push    eax
    call    exception_handler
    pop     eax  ; only here to balance the "push eax" done before the call

    ; Restore all registers
    pop     gs
    pop     fs
    pop     es
    pop     ds
    pop     ebp
    pop     edi
    pop     esi
    pop     edx
    pop     ecx
    pop     ebx
    pop     eax

	; Fix the stack pointer due to the 2 push done before the call
	; to exception_wrapper: error code and exception number
    add     esp,8
    iret

extern irq_handler

irq_wrapper:
    ; Save all registers
    push    eax
    push    ebx
    push    ecx
    push    edx
    push    esi
    push    edi
    push    ebp
    push    ds
    push    es
    push    fs
    push    gs

    ; Load kernel data descriptor into all segments
    mov     ax,GDT_KERNEL_DATA_SELECTOR
    mov     ds,ax
    mov     es,ax
    mov     fs,ax
    mov     gs,ax

    ; Pass the stack pointer (which gives the CPU context) to the C function
    mov     eax,esp
    push    eax
    call    irq_handler
    pop     eax  ; only here to balance the "push eax" done before the call

    ; Restore all registers
    pop     gs
    pop     fs
    pop     es
    pop     ds
    pop     ebp
    pop     edi
    pop     esi
    pop     edx
    pop     ecx
    pop     ebx
    pop     eax

	; Fix the stack pointer due to the 2 push done before the call
	; to irq_wrapper: error code and exception number
    add     esp,8
    iret
