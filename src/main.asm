        [bits 64]
        section .text
        global start

        ;; Syscall
        extern _exit

        ;; Functions
        extern wolfasm

start:
  mov   rbp,  rsp

  ;; Start the game
  call wolfasm

  ;; Exit the game
  mov rdi, rax
  call _exit
