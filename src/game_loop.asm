        [bits 64]

        %include "player.inc"

        section .text
        global game_loop, game_running, frame_time

        ;; SDL2 functions
        extern _SDL_GetTicks

        ;; wolfasm functions
        extern wolfasm_events, wolfasm_display, wolfasm_logic, \
        wolfasm_display_clean

        ;; wolfasm symbols
        extern game_player

        ;; TODO: rm
        extern _wolfasm_regulate_framerate

game_loop:
        push      rbp
        mov       rbp, rsp
        mov       dword [rel game_running], 1

.loop:
        cmp       dword [rel game_running], 0
        je        .end_loop

        ;; Treat events
        call      wolfasm_events

        ;; Process game logic
        call      wolfasm_logic

        ;; Update display
        call      wolfasm_display

        ;; Handle tick
        call      wolfasm_ticks
        mov       rdi, 60
        call      _wolfasm_regulate_framerate

        ;; Go back to loop
        jmp       .loop
.end_loop:
        mov       rsp, rbp
        pop       rbp
        ret

;; Tick handling happens here
wolfasm_ticks:
        push      rbp
        mov       rbp, rsp

        ;; Update old_time
        movsd     xmm0, [rel cur_time]
        movsd     [rel old_time], xmm0

        ;; Update cur_time
        call      _SDL_GetTicks
        cvtsi2sd  xmm0, rax
        movsd     [rel cur_time], xmm0

        ;; Calculate frame time
        subsd     xmm0, [rel old_time]
        mov       rax, 1000
        cvtsi2sd  xmm1, rax
        divsd     xmm0, xmm1
        movsd     [rel frame_time], xmm0

        ;; Calculate new movement speed
        mov       rax, PLAYER_MODIFIER_MOV_SPEED
        cvtsi2sd  xmm1, rax
        mulsd     xmm1, xmm0
        movsd     [rel game_player + wolfasm_player.movement_speed], xmm1

        ;; Calculate new rotation speed
        mov       rax, PLAYER_MODIFIER_ROT_SPEED
        cvtsi2sd  xmm1, rax
        mulsd     xmm1, xmm0
        movsd     [rel game_player + wolfasm_player.rotation_speed], xmm1

        mov       rsp, rbp
        pop       rbp
        emms
        ret

        section .data
game_running  dd 0
cur_time      do 0.0
old_time      do 0.0
frame_time    do 0.0
