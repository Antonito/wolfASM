        [bits 64]
        section .text
        global start

        ;; Syscall
        extern _exit

        ;; Functions
        extern wolfasm

start:
  ;; Start the game
  call wolfasm

  ;; Exit the game
  mov rdi, rax
  call _exit
