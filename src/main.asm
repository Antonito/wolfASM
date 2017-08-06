        [bits 64]
        section .text
        global _main

        ;; Syscall
        extern _exit

        ;; Functions
        extern wolfasm

_main:
        push  rbp
        mov   rbp, rsp

        ;; Start the game
        call wolfasm

        ;; Exit the game
        mov rdi, rax
        call _exit
