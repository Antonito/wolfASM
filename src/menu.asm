        [bits 64]

        %include "sdl.inc"

        global wolfasm_menu_render_button

        extern gui_font, wolfasm_display_text, window_height

        ;; SDL functions
        extern _TTF_SetFontStyle

        section .text

;; void render_button(char const *text, int32_t const x, int32_t y, int32_t width, bool selected)
wolfasm_menu_render_button:
        push    rbp
        mov     rbp, rsp

        cmp     r8b, 0
        je      .not_selected

.set_underline_style:
        sub     rsp, 8
        push    rdi
        push    rsi
        push    rdx
        push    rcx
        push    r8

        mov     rdi, [rel gui_font]
        mov     rsi, TTF_STYLE_UNDERLINE
        call    _TTF_SetFontStyle

        pop     r8
        pop     rcx
        pop     rdx
        pop     rsi
        pop     rdi
        add     rsp, 8

.not_selected:
        sub     rsp, 8
        push    rdi
        push    rsi
        push    rdx
        push    rcx
        push    r8

        ;; Fill rect
        lea     r9, [rel wolfasm_button_rect]
        mov     [r9], esi
        mov     [r9 + 1 * 4], edx
        mov     [r9 + 2 * 4], ecx
        mov     ecx, [rel window_height]
        shr     ecx, 4      ;; / 16
        mov     dword [r9 + 3 * 4], ecx

        ;; rdi already contains the text
        mov     rsi, r9
        mov     edx, 0xFFFFFFFF
        call    wolfasm_display_text

        pop     r8
        pop     rcx
        pop     rdx
        pop     rsi
        pop     rdi
        add     rsp, 8

        cmp     r8b, 0
        je      .no_restore_style

.restore_style:

        mov     rdi, [rel gui_font]
        mov     rsi, TTF_STYLE_NORMAL
        call    _TTF_SetFontStyle

.no_restore_style:

        mov     rsp, rbp
        pop     rbp
        ret

        section .rodata
txt: db "Hello", 0x00

        section .bss
wolfasm_button_rect:  resd   4
