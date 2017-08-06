        [bits 64]

        %include "sdl.mac"

        section .text
        global wolfasm_events

        ;; SDL functions
        extern _SDL_PollEvent

        ;; wolfasm symbols
        extern game_running

;; This function process the events
wolfasm_events:
        push  rbp
        mov   rbp, rsp

.poll:
        mov   rdi, event
        call  _SDL_PollEvent

        ;; Check if we have any event to process
        cmp   rax, 0
        je    .end_poll

        cmp   dword [rel event], SDL_QUIT
        jne   .check_keydown
        ;; Handle SQL_QUIT here
        mov   dword [rel game_running], 0
        jmp   .poll

.check_keydown:
        cmp   dword [rel event], SDL_KEYDOWN
        jne   .check_keyup
        ;; Handle SQL_KEYDOWN here
        jmp   .poll

.check_keyup:
        cmp   dword [rel event], SDL_KEYUP
        jne   .check_mousedown
        ;; Handle SQL_KEYUP here
        jmp   .poll

.check_mousedown:
        cmp   dword [rel event], SDL_MOUSEBUTTONDOWN
        jne   .check_mouseup
        ;; Handle SDL_MOUSEBUTTONDOWN here
        jmp   .poll

.check_mouseup:
        cmp   dword [rel event], SDL_MOUSEBUTTONUP
        jne   .check_mousemotion
        ;; Handle SDL_MOUSEBUTTONUP hereSDL_MOUSEMOTION
        jmp   .poll

.check_mousemotion:
        cmp   dword [rel event], SDL_MOUSEMOTION
        jne   .poll
        ;; Handle SDL_MOUSEMOTION here

        jmp   .poll

.end_poll:
        mov   rsp, rbp
        pop   rbp
        ret

        section .data
        event istruc sdl_event
          at sdl_event.type, dd 0
        iend
