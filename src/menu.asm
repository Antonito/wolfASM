        [bits 64]

        %include "sdl.inc"
        %include "menu.inc"

        global wolfasm_menu_render_button

        extern gui_font, wolfasm_display_text, window_height,   \
        window_width, wolfasm, wolfasm_menu_play_music

        ;; SDL functions
        extern _TTF_SetFontStyle, _Mix_HaltMusic,               \
        _SDL_StartTextInput

        ;; LibC functions
        extern _strncpy, _strncat, _snprintf

        ;; TODO: rm
        global selected_text_field_max_len, selected_text_field_len, selected_text_field, wolfasm_connect, wolfasm_connect_len, wolfasm_port, wolfasm_port_len, callbacks_main_menu, menu, selected_button, wolfasm_menu_nb_buttons, running, callback_sm_solo, wolfasm_selected_map, callbacks_multiplayer_menu
        extern _wolfasm_load_maps

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

        ;;
        ;; Main menu functions
        ;;

wolfasm_main_play:
        push    rbp
        mov     rbp, rsp

        mov     dword [rel menu], MENU_SELECT_MAP_SOLO
        mov     dword [rel wolfasm_menu_nb_buttons], NB_BUTTON_SELECT_MAP_SOLO
        mov     dword [rel selected_button], 0
        call    _wolfasm_load_maps

        mov     rsp, rbp
        pop     rbp
        ret

wolfasm_main_multiplayer:
        push    rbp
        mov     rbp, rsp

        mov     dword [rel menu], MENU_MULTIPLAYER
        mov     dword [rel wolfasm_menu_nb_buttons], NB_BUTTON_MULTIPLAYER
        mov     dword [rel selected_button], 0

        mov     rsp, rbp
        pop     rbp
        ret

wolfasm_main_exit:
        push    rbp
        mov     rbp, rsp

        mov     byte [rel running], 0

        mov     rsp, rbp
        pop     rbp
        ret

wolfasm_main_buttons:
        push    rbp
        mov     rbp, rsp

        ;; Display "Play" button
        lea     rdi, [rel play_txt]
        mov     esi, [rel window_width]
        shr     esi, 1
        mov     edx, [rel window_height]
        shr     edx, 1
        mov     ecx, [rel window_width]
        shr     ecx, 4
        xor     r8, r8
        mov     r9, 1
        cmp     dword [rel selected_button], BUTTON_PLAY
        cmove   r8w, r9w
        call    wolfasm_menu_render_button

        ;; Display "Multiplayer" button
        lea     rdi, [rel multiplayer_txt]
        mov     esi, [rel window_width]
        shr     esi, 1
        mov     edx, [rel window_height]
        shr     edx, 1 ;; / 2
        mov     r8d, [rel window_height]
        shr     r8d, 3 ;; /8
        add     edx, r8d
        mov     ecx, [rel window_width]
        shr     ecx, 2
        xor     r8, r8
        mov     r9, 1
        cmp     dword [rel selected_button], BUTTON_MULTIPLAYER
        cmove   r8w, r9w
        call    wolfasm_menu_render_button


        ;; Display "Exit" button
        lea     rdi, [rel exit_txt]
        mov     esi, [rel window_width]
        shr     esi, 1
        mov     edx, [rel window_height]
        shr     edx, 1 ;; / 2
        mov     r8d, [rel window_height]
        shr     r8d, 3 ;; /8
        shl     r8d, 1 ;; * 2
        add     edx, r8d
        mov     ecx, [rel window_width]
        shr     ecx, 4
        xor     r8, r8
        mov     r9, 1
        cmp     dword [rel selected_button], BUTTON_EXIT
        cmove   r8w, r9w
        call    wolfasm_menu_render_button

        mov     rsp, rbp
        pop     rbp
        ret

        ;;
        ;; Select Map solo menu functions
        ;;
wolfasm_sm_solo_map:
        push    rbp
        mov     rbp, rsp

        ;; Put path in buffer
        lea     rdi, [rel filename]
        lea     rsi, [rel filename_path]
        mov     edx, 512 - 1
        call    _strncpy

        ;; Add filename to path
        lea     rdi, [rel filename]
        mov     rsi, [rel wolfasm_selected_map]
        mov     edx, 512 - 1
        call    _strncat

        call    _Mix_HaltMusic
        lea     rdi, [rel filename]
        call    wolfasm
        call    wolfasm_menu_play_music

        mov     rsp, rbp
        pop     rbp
        ret

wolfasm_sm_solo_back:
        push    rbp
        mov     rbp, rsp

        mov     dword [rel menu], MENU_MAIN
        mov     dword [rel wolfasm_menu_nb_buttons], NB_BUTTON_MAIN
        mov     dword [rel selected_button], 0

        mov     rsp, rbp
        pop     rbp
        ret

wolfasm_sm_solo_buttons:
        push    rbp
        mov     rbp, rsp

        lea     rdi, [rel buff_file]
        mov     esi, 255 + 1 + 4
        lea     rdx, [rel file_fmt]
        mov     rcx, [rel wolfasm_selected_map]
        call    _snprintf

        ;; Render file button
        lea     rdi, [rel buff_file]
        mov     esi, [rel window_width]
        shr     esi, 1
        mov     edx, [rel window_height]
        shr     edx, 1
        mov     ecx, [rel window_width]
        shr     ecx, 3
        xor     r8, r8
        mov     r9d, 1
        cmp     dword [rel selected_button], BUTTON_SM_SOLO_MAP
        cmove   r8d, r9d
        call    wolfasm_menu_render_button

        ;; Render "Back" button
        lea     rdi, [rel back_txt]
        mov     esi, [rel window_width]
        shr     esi, 1
        mov     edx, [rel window_height]
        shr     edx, 1
        mov     r8d, [rel window_height]
        shr     r8d, 3
        add     edx, r8d
        mov     ecx, [rel window_width]
        shr     ecx, 3
        xor     r8, r8
        mov     r9d, 1
        cmp     dword [rel selected_button], BUTTON_SM_SOLO_BACK
        cmove   r8d, r9d
        call    wolfasm_menu_render_button

        mov     rsp, rbp
        pop     rbp
        ret

        ;;
        ;; Multiplayer menu functions
        ;;
wolfasm_multiplayer_host:
        push    rbp
        mov     rbp, rsp

        mov     dword [rel menu], MENU_MULTIPLAYER_HOST
        mov     dword [rel wolfasm_menu_nb_buttons], NB_BUTTON_MULTIPLAYER_HOST
        mov     dword [rel selected_button], 0
        mov     dword [rel wolfasm_port_len], 0
        lea     rdi, [rel wolfasm_port_len]
        mov     qword [rel selected_text_field_len], rdi
        mov     dword [rel selected_text_field_max_len], 6  ;; sizeof("65535")
        lea     rdi, [rel wolfasm_port]
        mov     qword [rel selected_text_field], rdi

        ;; Load maps
        call    _wolfasm_load_maps
        ;; Start handling text input
        call    _SDL_StartTextInput

        mov     rsp, rbp
        pop     rbp
        ret

wolfasm_multiplayer_connect:
        push    rbp
        mov     rbp, rsp

        mov     dword [rel menu], MENU_MULTIPLAYER_CONNECT
        mov     dword [rel wolfasm_menu_nb_buttons], NB_BUTTON_MULTIPLAYER_CONNECT
        mov     dword [rel selected_button], 0
        mov     dword [rel wolfasm_connect_len], 0
        mov     dword [rel wolfasm_port_len], 0
        lea     rdi, [rel wolfasm_connect_len]
        mov     qword [rel selected_text_field_len], rdi
        mov     dword [rel selected_text_field_max_len], 255
        lea     rdi, [rel wolfasm_connect]
        mov     qword [rel selected_text_field], rdi

        ;; Start handling text input
        call    _SDL_StartTextInput

        mov     rsp, rbp
        pop     rbp
        ret

wolfasm_multiplayer_back:
        push    rbp
        mov     rbp, rsp

        mov     dword [rel menu], MENU_MAIN
        mov     dword [rel wolfasm_menu_nb_buttons], NB_BUTTON_MAIN
        mov     dword [rel selected_button], 0

        mov     rsp, rbp
        pop     rbp
        ret

wolfasm_multiplayer_buttons:
        push    rbp
        mov     rbp, rsp

        ;; Render "Host" button
        lea     rdi, [rel host_txt]
        mov     esi, [rel window_width]
        shr     esi, 1
        mov     edx, [rel window_height]
        shr     edx, 1
        mov     ecx, [rel window_width]
        shr     ecx, 4
        xor     r8, r8
        mov     r9d, 1
        cmp     dword [rel selected_button], BUTTON_MP_HOST
        cmove   r8d, r9d
        call    wolfasm_menu_render_button

        ;; Render "Connect" button
        lea     rdi, [rel connect_txt]
        mov     esi, [rel window_width]
        shr     esi, 1
        mov     edx, [rel window_height]
        shr     edx, 1
        mov     r8d, [rel window_height]
        shr     r8d, 3
        add     edx, r8d
        mov     ecx, [rel window_width]
        shr     ecx, 3
        xor     r8, r8
        mov     r9d, 1
        cmp     dword [rel selected_button], BUTTON_MP_CONNECT
        cmove   r8d, r9d
        call    wolfasm_menu_render_button

        ;; Render "Back" button
        lea     rdi, [rel back_txt]
        mov     esi, [rel window_width]
        shr     esi, 1
        mov     edx, [rel window_height]
        shr     edx, 1
        mov     r8d, [rel window_height]
        shr     r8d, 3
        shl     r8d, 1
        add     edx, r8d
        mov     ecx, [rel window_width]
        shr     ecx, 4
        xor     r8, r8
        mov     r9d, 1
        cmp     dword [rel selected_button], BUTTON_MP_BACK
        cmove   r8d, r9d
        call    wolfasm_menu_render_button

        mov     rsp, rbp
        pop     rbp
        ret

        ;;
        ;; Multiplayer Connect menu functions
        ;;

        ;;
        ;; Multiplayer Host menu functions
        ;;

        section .rodata
;; Text strings
play_txt:               db      "Play", 0x00
multiplayer_txt:        db      "Multiplayer", 0x00
exit_txt:               db      "Exit", 0x00
back_txt:               db      "Back", 0x00
host_txt:               db      "Host", 0x00
connect_txt:            db      "Connect", 0x00
filename_path:          db      "./resources/map/", 0x00
file_fmt:               db      "< %s >", 0x00

;; Callbacks
callbacks_main_menu:    dq      wolfasm_main_play,            \
                                wolfasm_main_multiplayer,     \
                                wolfasm_main_exit,            \
                                wolfasm_main_buttons
callback_sm_solo:       dq      wolfasm_sm_solo_map,          \
                                wolfasm_sm_solo_back,         \
                                wolfasm_sm_solo_buttons
callbacks_multiplayer_menu: \
                        dq      wolfasm_multiplayer_host,     \
                                wolfasm_multiplayer_connect,  \
                                wolfasm_multiplayer_back,     \
                                wolfasm_multiplayer_buttons

        section  .data
selected_text_field:              dq      0
selected_text_field_len:          dq      0
selected_text_field_max_len:      dd      0
wolfasm_selected_text_field:      dd      0

wolfasm_selected_map:             dq      0

menu:                             dd      MENU_MAIN
wolfasm_menu_nb_buttons:          dd      NB_BUTTON_MAIN
selected_button:                  dd      BUTTON_PLAY
running:                          db      1

        section .bss
wolfasm_button_rect:  resd   4
wolfasm_connect:      resb   255
wolfasm_connect_len:  resd   1
wolfasm_port:         resb   6        ;; sizeof("65535")
wolfasm_port_len:     resd   1
filename:             resb   512
buff_file:            resb   255 + 1 + 4
