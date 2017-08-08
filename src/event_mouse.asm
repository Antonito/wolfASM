        [bits 64]

        %include "player.inc"
        %include "sdl.inc"

        section .text
        global wolfasm_events_mouse_down, wolfasm_events_mouse_up, wolfasm_event_mouse_motion, wolfasm_event_mouse_motion_handle

        ;; C wrapper
        extern wolfasm_events_mouse_up_cwrapper,  \
        wolfasm_events_mouse_down_cwrapper,       \
        wolfasm_events_mouse_motion_cwrapper

        ;; wolfasm functions
        extern wolfasm_player_rotate_right, wolfasm_player_rotate_left

        ;; wolfasm symbols
        extern game_player

;; Handle events when a mouse button goes up
wolfasm_events_mouse_up:
          push  rbp
          mov   rbp, rsp
          call  wolfasm_events_mouse_up_cwrapper
          mov   rsp, rbp
          pop  rbp
          ret

;; Handle events when a mouse button goes down
wolfasm_events_mouse_down:
          push  rbp
          mov   rbp, rsp
          call  wolfasm_events_mouse_down_cwrapper
          mov   rsp, rbp
          pop  rbp
          ret

;; Handle events when the mouse moves
wolfasm_event_mouse_motion:
          push  rbp
          mov   rbp, rsp
          call  wolfasm_events_mouse_motion_cwrapper
          mov   rsp, rbp
          pop  rbp
          ret

;; Handle the mouse's movement
;; void wolfasm_event_mouse_motion_handle(int32_t rel_x);
wolfasm_event_mouse_motion_handle:
          push      rbp
          mov       rbp, rsp

          ;; Save current rotation speed
          movsd     xmm0, [rel game_player + wolfasm_player.rotation_speed]
          movsd     [rel wolfasm_mouse_rot_speed_save], xmm0

          ;; Change rotation speed
          movsd     xmm0, [rel wolfasm_mouse_rot_speed]
          movsd     [rel game_player + wolfasm_player.rotation_speed], xmm0

          cmp       edi, 0
          jl        .left_rotation

.right_rotation:
          ;; Loop for smooth movement
.right_rotation_loop:
          cmp       edi, 0
          je        .end_rotation

          sub       rsp, 8
          push      rdi
          call      wolfasm_player_rotate_right
          pop       rdi
          add       rsp, 8

          dec       edi
          jmp       .right_rotation_loop

.left_rotation:
          ;; mouse_x = -mouse_x
          neg       edi

          ;; Loop for smooth movement
.left_rotation_loop:
          cmp       edi, 0
          je        .end_rotation

          sub       rsp, 8
          push      rdi
          call      wolfasm_player_rotate_left
          pop       rdi
          add       rsp, 8

          dec       edi
          jmp       .left_rotation_loop

.end_rotation:
          ;; Restore rotation speed
          movsd     xmm0, [rel wolfasm_mouse_rot_speed_save]
          movsd     [rel game_player + wolfasm_player.rotation_speed], xmm0

          mov       rsp, rbp
          pop       rbp
          ret

          section .data
wolfasm_mouse_rot_speed:       dq    0.015

          section .bss
wolfasm_mouse_rot_speed_save: reso  1
