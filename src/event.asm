        [bits 64]

        %include "sdl.inc"

        section .text
        global wolfasm_events

        ;; SDL functions
        extern _SDL_PollEvent

        ;; wolfasm symbols
        extern game_running, game_events

        ;; wolfasm functions
        extern wolfasm_events_keyboard_down, wolfasm_events_keyboard_up,  \
        wolfasm_events_mouse_down, wolfasm_events_mouse_up,               \
        wolfasm_event_mouse_motion

;; This function process the events
wolfasm_events:
        push  rbp
        mov   rbp, rsp

.poll:
        mov   rdi, game_events
        call  _SDL_PollEvent

        ;; Check if we have any event to process
        cmp   rax, 0
        je    .end_poll
        mov   rdi, game_events  ;; 'event' is passed to events handler

        cmp   dword [rel game_events], SDL_QUIT
        jne   .check_keydown
        ;; If the cross is cliked, stop the game
        mov   dword [rel game_running], 0
        jmp   .poll

.check_keydown:
        cmp   dword [rel game_events], SDL_KEYDOWN
        jne   .check_keyup
        call  wolfasm_events_keyboard_down
        jmp   .poll

.check_keyup:
        cmp   dword [rel game_events], SDL_KEYUP
        jne   .check_mousedown
        call  wolfasm_events_keyboard_up
        jmp   .poll

.check_mousedown:
        cmp   dword [rel game_events], SDL_MOUSEBUTTONDOWN
        jne   .check_mouseup
        call  wolfasm_events_mouse_down
        jmp   .poll

.check_mouseup:
        cmp   dword [rel game_events], SDL_MOUSEBUTTONUP
        jne   .check_mousemotion
        call  wolfasm_events_mouse_up
        jmp   .poll

.check_mousemotion:
        cmp   dword [rel game_events], SDL_MOUSEMOTION
        jne   .poll
        call  wolfasm_event_mouse_motion
        jmp   .poll

.end_poll:
        mov   rsp, rbp
        pop   rbp
        ret
