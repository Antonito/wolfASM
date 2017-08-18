        [bits 64]
        section .text
        global _start, _main

        ;; Syscall
        extern _exit

        ;; Functions
        extern wolfasm, wolfasm_init, wolfasm_deinit, wolfasm_menu

_start:
_main:
        push  rbp
        mov   rbp, rsp

        ;; Start the game
        call  wolfasm_init
        call  wolfasm_menu

        sub   rsp, 8
        push  rax

        call  wolfasm_deinit

        pop   rax
        add   rsp, 8

        ;; Exit the game
        mov rdi, rax
        call _exit
