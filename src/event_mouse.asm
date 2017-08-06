        [bits 64]

        %include "sdl.mac"

        section .text
        global wolfasm_events_mouse_down, wolfasm_events_mouse_up, wolfasm_event_mouse_motion

;; Handle events when a mouse button goes up
wolfasm_events_mouse_up:
          push  rbp
          mov   rbp, rsp

          mov   rsp, rbp
          pop  rbp
          ret

;; Handle events when a mouse button goes down
wolfasm_events_mouse_down:
          push  rbp
          mov   rbp, rsp

          mov   rsp, rbp
          pop  rbp
          ret

;; Handle events when the mouse moves
wolfasm_event_mouse_motion:
          push  rbp
          mov   rbp, rsp

          mov   rsp, rbp
          pop  rbp
          ret
