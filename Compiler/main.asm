default rel

section .data
    txt1 db "Hello, World!", 10
    len1 equ $ - txt1

    num1 dd 5
    num2 dd 3

section .bss
    bufor resb 12
section .text
    global _start

; === intToText =======================================================
; Input:  EAX - Number
; Output: RSI - Text
; Output: RDX - Text Length
; =====================================================================
intToText:
    mov rcx, 0
    mov ebx, 10
.ittLoop:
    mov edx, 0
    div ebx
    add edx, 48
    push rdx
    inc rcx
    cmp eax, 0
    jne .ittLoop

    lea rdi, [rel bufor]
    mov rdx, rcx
    lea rsi, [rel bufor]
.ittLoopWrite:
    pop rax
    mov [rdi], al
    inc rdi
    loop .ittLoopWrite
    mov byte [rdi], 10
    inc rdx
    ret

; === prt =============================================================
; Input: RSI - Text
; Input: RDX - Text Lenght
; =====================================================================
prt:
    mov rax, 1
    mov rdi, 1
    syscall
    ret

; =====================================================================
_start:
    lea rsi, [rel txt1]
    mov rdx, len1
    call prt

    mov eax, [rel num1]
    add eax, [rel num2]

    call intToText

    call prt

    mov rax, 60
    mov rdi, 0
    syscall
