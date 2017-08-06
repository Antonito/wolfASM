        [bits 64]

        %include "sdl.mac"
        %include "window.mac"

        section .text
        global wolfasm
        global window_ptr, window_surface

        ;; SDL functions
        extern _SDL_Init, _SDL_CreateWindow, _SDL_Quit, _SDL_DestroyWindow, _SDL_GetWindowSurface

        ;; Syscalls
        extern _exit

        ;; wolfasm functions
        extern game_loop

;; This function starts a window, and calls the game loop
wolfasm:
        push  rbp
        mov   rbp, rsp

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

        ;; Check that is non NULL
        cmp   qword [rel window_ptr], 0x0
        je    .exit_fail

        ;; Get the window's surface
        mov   rdi, [rel window_ptr]
        call  _SDL_GetWindowSurface

        ;; Check that is non NULL
        cmp   rax, 0x0
        je    .exit_fail
        mov   qword [rel window_surface], rax

        ;; TODO: Load map here

        ;; Starts the game loop
        call  game_loop

        ;; Destroy window
        mov   qword rdi, [rel window_ptr]
        call  _SDL_DestroyWindow

        ;; Leave program, everything went right
        mov   rax, 0
        jmp   .exit
.exit_fail:
        mov   rax, 1

.exit:
        ;; Cleanup before leaving
        push  rax
        sub   rsp, 8
        call  _SDL_Quit
        add   rsp, 8
        pop   rax

        mov   rsp, rbp
        pop   rbp
        ret

        section .data
window_ptr dq 1
window_surface dq 1

        section .rodata
window_name db "WolfASM - bache_a", 0x00
