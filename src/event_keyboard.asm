        [bits 64]

        %include "sdl.mac"

        section .text
        global wolfasm_events_keyboard_down, wolfasm_events_keyboard_up

        ;; wolfasm symbols
        extern game_running

        ;; C wrapper
        extern wolfasm_events_keyboard_up_cwrapper, wolfasm_events_keyboard_down_cwrapper

;; Handle events when a key goes up
wolfasm_events_keyboard_up:
        push  rbp
        mov   rbp, rsp
        call  wolfasm_events_keyboard_up_cwrapper
        mov   rsp, rbp
        pop  rbp
        ret

;; Handle events when a key goes down
wolfasm_events_keyboard_down:
        push  rbp
        mov   rbp, rsp
        call  wolfasm_events_keyboard_down_cwrapper
        mov   rsp, rbp
        pop  rbp
        ret
