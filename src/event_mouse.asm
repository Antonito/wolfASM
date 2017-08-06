        [bits 64]

        %include "sdl.inc"

        section .text
        global wolfasm_events_mouse_down, wolfasm_events_mouse_up, wolfasm_event_mouse_motion

        ;; C wrapper
        extern wolfasm_events_mouse_up_cwrapper, wolfasm_events_mouse_down_cwrapper, wolfasm_events_mouse_motion_cwrapper

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
