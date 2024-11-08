%include "const.inc"

; --------------------------------------------------
; Make symbols below visible to the linker

global idt_load

global _exception3
global _exception8

global _irq5

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

_exception3:
    push    3
    jmp     exception_wrapper

_exception8:
    push    0  ; dummy error code
    push    8
    jmp     exception_wrapper

; --------------------------------------------------
; First part of low level hardware interrupt handlers

_irq5:
    push    0  ; dummy error code
    push    5
    jmp     irq_wrapper

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
