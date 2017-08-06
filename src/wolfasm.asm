        [bits 64]

        %include "sdl.mac"
        %include "window.mac"

        section .text
        global wolfasm

        ;; SDL functions
        extern _SDL_Init, _SDL_CreateWindow, _SDL_Quit, _exit

        ;; wolfasm functions
        extern game_loop

;; This function starts a window, and calls the game loop
wolfasm:
        push  rbp
        ;; Initialize SDL
        mov   rdi, SDL_INIT_VIDEO
        call  _SDL_Init

        ;; Start the window
        mov   rdi,  window_name ;; Window title
        mov   rsi, SDL_WINDOWPOS_UNDEFINED ;; X position
        mov   rdx, SDL_WINDOWPOS_UNDEFINED ;; Y Position
        mov   rcx, WIN_WIDTH ;; Width
        mov   r8, WIN_HEIGHT ;; Height
        mov   r9, SDL_WINDOW_SHOWN ;; Flags
        call  _SDL_CreateWindow
        mov qword [rel window_ptr], rax

        ;; Check that is non zero
        cmp   qword [rel window_ptr], 0x0
        je    .exit_fail

        ;; Starts the game loop
        call  game_loop

        ;; Leave program, everything went right
        mov   rax, 0
        jmp   .exit
.exit_fail:
        mov   rax, 1

.exit:
        ;; Cleanup before leaving
        sub     rsp, 8
        push  rax
        call  _SDL_Quit
        pop   rax
        add     rsp, 8
        pop   rbp
        ret

        section .data
window_ptr dq 1

        section .rodata
window_name db "WolfASM - bache_a", 0x00
