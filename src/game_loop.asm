        [bits 64]
        section .text
        global game_loop, game_running

        ;; SDL2 functions
        extern _SDL_Delay

        ;; wolfasm functions
        extern wolfasm_events

game_loop:
        push  rbp
        mov   rbp, rsp
        mov   dword [rel game_running], 1

.loop:
        cmp   dword [rel game_running], 0
        je    .end_loop

        ;; Treat events
        call  wolfasm_events

        ;; Process game logic

        ;; Update display

        ;; Tick to 60fps
        mov   rdi, 1
        call  _SDL_Delay

        ;; Go back to loop
        jmp   .loop
.end_loop:
        mov   rsp, rbp
        pop   rbp
        ret

        section .data
game_running dd 0
