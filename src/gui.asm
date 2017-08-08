        [bits 64]

        global wolfasm_gui_init, wolfasm_gui_deinit, wolfasm_display_gui

        ;; SDL2 functions
        extern _TTF_Init, _TTF_Quit, _TTF_OpenFont, _TTF_CloseFont, \
        _SDL_GetError, _TTF_RenderText_Solid, _SDL_UpperBlit,       \
        _SDL_FreeSurface, _SDL_CreateTextureFromSurface,            \
        _SDL_RenderCopy, _SDL_DestroyTexture, _SDL_RenderPresent

        ;; LibC functions
        extern _exit, _puts, _snprintf

        ;; wolfasm_symbol
        extern frame_time, window_surface, window_renderer

        section .text
;; Initialize GUI display
wolfasm_gui_init:
        push      rbp
        mov       rbp, rsp

        ;; Init SDL_TTF
        call      _TTF_Init
        cmp       rax, -1
        je        .init_error

        ;; Load texture
        mov       rdi, fps_font_path
        mov       rsi, [rel fps_font_size]
        call      _TTF_OpenFont

        cmp       rax, 0
        je        .init_error

        mov       [rel fps_font], rax

        jmp       .init_end
.init_error:
        ;; Get error, display it
        call      _SDL_GetError
        mov       rdi, rax
        call      _puts

        ;; Exit program TODO: Find a better way
        mov       rdi, 1
        call      _exit

.init_end:
        mov       rsp, rbp
        pop       rbp
        ret

;; De-initialize GUI display
wolfasm_gui_deinit:
        push      rbp
        mov       rbp, rsp

        ;; Destroy FPS counter font
        mov       rdi, [rel fps_font]
        call      _TTF_CloseFont

        ;; Stop SDL_TTF
        call      _TTF_Quit

        mov       rsp, rbp
        pop       rbp
        ret

wolfasm_display_gui:
        push      rbp
        mov       rbp, rsp

.display_fps:
        ;; Display FPS counter
        movsd     xmm1, [rel frame_time]
        mov       rax, 1
        cvtsi2sd  xmm0, rax
        divsd     xmm0, xmm1  ;; 1.0 / frame_time

        ;; Create string
        mov       rdi, fps_buff
        mov       rsi, 128    ;; fps_buff_len
        mov       rdx, fps_fmt_str
        call      _snprintf

        ;; Create SDL_Surface from string
        mov       rdi, [rel fps_font]
        mov       rsi, fps_buff
        mov       rdx, [rel fps_text_color]
        call      _TTF_RenderText_Solid
        mov       [rel fps_surface], rax
        cmp       rax, 0
        je        .err

        ;; Create SDL_Texture
        mov       rdi, [rel window_renderer]
        mov       rsi, rax
        call      _SDL_CreateTextureFromSurface
        cmp       rax, 0
        je        .err

        mov       [rel fps_texture], rax

        ;; Blit SDL_Texture
        mov       rdi, [rel window_renderer]
        mov       rsi, rax
        xor       rdx, rdx
        lea       rcx, [rel fps_text_pos]
        call      _SDL_RenderCopy

        ;; Destroy texture
        mov       rdi, [rel fps_texture]
        call      _SDL_DestroyTexture


.end_display_fps:
        ;; Free SDL_Surface (ptr already in rdi)
        mov       rdi, [rel fps_surface]
        call      _SDL_FreeSurface


        mov       rsp, rbp
        pop       rbp
        ret
.err:
        call      _SDL_GetError
        mov       rdi, rax
        call      _puts
        mov       rdi, 1
        call      _exit

        section .rodata
fps_font_path     db    "./resources/brig.ttf", 0x00
fps_font_size     dd    24
fps_fmt_str       db    "FPS: %3.0lf", 0x00
fps_text_color    db    255, 255, 255, 255
fps_text_pos      dd    0, 0, 200, 30

        section .bss
fps_buff          resb  128
fps_font          resq  1
fps_surface       resq  1
fps_texture       resq  1
