        [bits 64]

        %include "window.inc"
        %include "player.inc"
        %include "weapon.inc"

        global wolfasm_gui_init, wolfasm_gui_deinit, wolfasm_display_gui, gui_font

        ;; SDL2 functions
        extern _TTF_Init, _TTF_Quit, _TTF_OpenFont, _TTF_CloseFont, \
        _SDL_GetError, _TTF_RenderText_Solid, _SDL_UpperBlit,       \
        _SDL_FreeSurface, _SDL_CreateTextureFromSurface,            \
        _SDL_RenderCopy, _SDL_DestroyTexture, _SDL_RenderPresent

        ;; LibC functions
        extern _exit, _puts, _snprintf

        ;; wolfasm_symbol
        extern frame_time, window_surface, window_renderer, \
        game_player

        ;; TODO: rm
        extern _display_gui_ammo

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
        mov       rdi, gui_font_path
        mov       rsi, [rel gui_font_size]
        call      _TTF_OpenFont

        cmp       rax, 0
        je        .init_error

        mov       [rel gui_font], rax

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
        mov       rdi, [rel gui_font]
        call      _TTF_CloseFont

        ;; Stop SDL_TTF
        call      _TTF_Quit

        mov       rsp, rbp
        pop       rbp
        ret

;; rdi -> text
;; rsi -> position
;; rdx -> color
wolfasm_display_text:
        push      rbp
        mov       rbp, rsp

        sub       rsp, 8
        push      rsi       ;; Save position

        ;; Create SDL_Surface from string
        mov       rsi, rdi
        mov       rdi, [rel gui_font]
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

        mov       [rel font_texture], rax

        pop       rsi
        add       rsp, 8
        mov       rcx, rsi    ;; Restore position

        ;; Blit SDL_Texture
        mov       rdi, [rel window_renderer]
        mov       rsi, rax
        xor       rdx, rdx
        call      _SDL_RenderCopy

        ;; Destroy texture
        mov       rdi, [rel font_texture]
        call      _SDL_DestroyTexture


.end_display_text:
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
        mov       rdi, text_buff
        mov       rsi, 128    ;; text_buff_len
        mov       rdx, fps_fmt_str
        call      _snprintf

        ;; Print to screen
        mov       rdi, text_buff
        lea       rsi, [rel fps_text_pos]
        mov       rdx, [rel fps_text_color]
        call      wolfasm_display_text

.display_ammo:
        ;; Get weapon's ammo informations
        mov       r8, [rel game_player + wolfasm_player.weapon]
        mov       cx, [r8 + wolfasm_weapon_s.ammo]
        mov       r8w, [r8 + wolfasm_weapon_s.max_ammo]

        ;; Create string
        mov       rdi, text_buff
        mov       rsi, 128    ;; text_buff_len
        mov       rdx, ammo_fmt_str
        call      _snprintf

        ;; Print to screen
        mov       rdi, text_buff
        lea       rsi, [rel ammo_text_pos]
        mov       rdx, [rel fps_text_color]
        call      wolfasm_display_text

;        call      _display_gui_ammo

        mov       rsp, rbp
        pop       rbp
        ret

        section .rodata
gui_font_path     db    "./resources/arial.ttf", 0x00
gui_font_size     dd    24
fps_fmt_str       db    "FPS: %3.0lf", 0x00
fps_text_color    db    255, 255, 255, 255
fps_text_pos      dd    10, 10, 100, 30
ammo_fmt_str      db    "%.2d / %.2d", 0x00
ammo_text_pos     dd    WIN_WIDTH - 110, 10, 100, 30

        section .bss
text_buff         resb  128
gui_font          resq  1
fps_surface       resq  1
font_texture      resq  1
