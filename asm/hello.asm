section .data
    msg db "Hello, world!"

section .text
    global _start

_start:
    mov     rax, 1      ; Syscall: sys_write.
    mov     rdi, 1      ; 1st arg: file descriptor = stdout.
    mov     rsi, msg    ; 2nd arg: const char* buf.
    mov     rdx, 13     ; 3rd arg: buf count.
    syscall

    mov     rax, 60     ; Syscall (sys_exit)
    mov     rdi, 0      ; Exit code.
    syscall

