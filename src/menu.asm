        [bits 64]

        %include "sdl.inc"
        %include "menu.inc"
        %include "sprite.inc"

        global wolfasm_menu_render_button,                      \
        wolfasm_regulate_framerate, wolfasm_menu

        ;; events
        global wolfasm_menu_event_quit,                          \
        wolfasm_menu_event_key_up, wolfasm_menu_event_key_down,  \
        wolfasm_menu_event_key_left,                             \
        wolfasm_menu_event_key_right,                            \
        wolfasm_menu_event_key_tab,                              \
        wolfasm_menu_event_key_enter,                           \
        wolfasm_menu_event_key_backspace,                       \
        wolfasm_menu_event_textinput

        extern gui_font, wolfasm_display_text, window_height,   \
        window_width, wolfasm, wolfasm_menu_play_music,         \
        wolfasm_read_dir, wolfasm_menu_events_cwrapper,         \
        window_renderer, wolfasm_render_sprite, wolfasm_sprite

        ;; SDL functions
        extern _TTF_SetFontStyle, _Mix_HaltMusic,               \
        _SDL_StartTextInput, _SDL_StopTextInput,                \
        _SDL_GetTicks, _SDL_Delay, _SDL_RenderClear,            \
        _SDL_RenderPresent

        ;; LibC functions
        extern _strncpy, _strncat, _snprintf, _strtol, _memset, \
        _strlen, _printf, _exit, _opendir, _free,               \
        _strdup, _closedir, _srand, _time, _strlen

        ;; TODO: rm
        global selected_text_field, selected_text_field_len, selected_text_field_max_len
        extern _wolfasm_join_game, _wolfasm_host_game, _print_dir

        section .text
wolfasm_menu:
        push      rbp
        mov       rbp, rsp

        ;; Update rand seed
        xor       rdi, rdi
        call      _time
        mov       rdi, rax
        call      _srand

        call      wolfasm_menu_play_music

.loop:
        cmp       byte [rel running], 0
        je        .loop_end

        ;; Handle events
        call      wolfasm_menu_events_cwrapper

        ;; Update display
        mov       rdi, [rel window_renderer]
        call      _SDL_RenderClear

        ;; Display background
        lea       rdi, [rel wolfasm_sprite]
        mov       eax, wolfasm_sprite_s.size
        mov       ecx, 3
        mul       ecx
        lea       rdi, [rdi + rax]
        xor       rsi, rsi
        xor       rdx, rdx
        xor       rcx, rcx
        call      wolfasm_render_sprite

        ;; Render buttons
        lea       rdi, [rel callbacks]
        mov       eax, [rel menu]
        mov       rdi, [rdi + rax * 8]
        mov       eax, [rel wolfasm_menu_nb_buttons]
        mov       rdi, [rdi + rax * 8]
        call      rdi

        mov       rdi, [rel window_renderer]
        call      _SDL_RenderPresent

        call      wolfasm_regulate_framerate

        jmp       .loop
.loop_end:

        mov       rsp, rbp
        pop       rbp
        ret

wolfasm_regulate_framerate:
        push      rbp
        mov       rbp, rsp

        movsd     xmm0, [rel cur_time]
        movsd     [rel old_time], xmm0

        ;; Update cur_time
        call      _SDL_GetTicks
        cvtsi2sd  xmm0, rax
        movsd     [rel cur_time], xmm0

        movsd     xmm1, [rel old_time]
        subsd     xmm0, xmm1
        xor       rdx, rdx
        mov       eax, 1000
        mov       edi, 60
        div       edi
        cvtsi2sd  xmm1, eax
        ucomisd   xmm0, xmm1
        jae       .no_sleep

        subsd     xmm1, xmm0
        cvttsd2si rdi, xmm1
        call      _SDL_Delay

.no_sleep:
        mov       rsp, rbp
        pop       rbp
        emms
        ret

wolfasm_load_maps:
        push      rbp
        mov       rbp, rsp

        lea       rdi, [rel filename_path]
        call      _opendir
        mov       dword [rel wolfasm_nb_maps], 0

        cmp       rax, 0
        je        .err
        mov       rdi, rax

        sub       rsp, 8
        push      rdi
.loop:
        call      wolfasm_read_dir
        cmp       rax, 0
        je        .loop_end

        cmp       byte [rax], '.'
        je        .next_loop

        ;; Check that we can still read files
        mov       ecx, [rel wolfasm_nb_maps]
        cmp       ecx, 255
        jge       .err

        sub       rsp, 8
        push      rax

        ;; Free current string
        lea       rdi, [rel wolfasm_maps]
        mov       rdi, [rdi + rcx * 8]
        call      _free

        pop       rax
        add       rsp, 8

        lea       rdi, [rax]
        call      _strdup

        ;; Check if string is correct
        cmp       rax, 0
        je        .err

        ;; Save string
        lea       rdi, [rel wolfasm_maps]
        mov       ecx, [rel wolfasm_nb_maps]
        mov       qword [rdi + rcx * 8], rax

        inc       ecx
        mov       dword [rel wolfasm_nb_maps], ecx
.next_loop:
        mov       rdi, [rsp]
        jmp       .loop

.loop_end:
        pop       rdi
        add       rsp, 8

        call      _closedir

        mov       eax, [rel wolfasm_nb_maps]
        cmp       eax, 0
        je        .no_update

        mov       dword [rel wolfasm_current_map], 0
        lea       rdi, [rel wolfasm_maps]
        mov       rax, [rdi]
        mov       qword [rel wolfasm_selected_map], rax


.no_update:
        mov       rsp, rbp
        pop       rbp
        ret
.err:
        lea       rdi, [rel loading_error_msg]
        call      _printf
        mov       rdi, 1
        call      _exit

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
        call    wolfasm_load_maps

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
        call    wolfasm_load_maps
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
wolfasm_multiplayer_connect_connect:
        push    rbp
        mov     rbp, rsp

        ;; Prepare game
        call    _SDL_StopTextInput
        call    _Mix_HaltMusic

        ;; Get port
        lea     rdi, [rel wolfasm_port]
        xor     rsi, rsi
        mov     edx, 10
        call    _strtol

        ;; Connect to distant game
        mov     rsi, rax
        lea     rdi, [rel wolfasm_connect]
        call    _wolfasm_join_game

        ;; Back to menu
        call    wolfasm_menu_play_music
        call    _SDL_StartTextInput

        mov     rsp, rbp
        pop     rbp
        ret

wolfasm_multiplayer_connect_back:
        push    rbp
        mov     rbp, rsp

        mov     dword [rel menu], MENU_MULTIPLAYER
        mov     dword [rel wolfasm_menu_nb_buttons], NB_BUTTON_MULTIPLAYER
        mov     dword [rel selected_button], 0
        mov     dword [rel wolfasm_connect_len], 0
        mov     dword [rel wolfasm_port_len], 0
        mov     dword [rel selected_text_field_max_len], 0
        mov     qword [rel selected_text_field_len], 0
        mov     qword [rel selected_text_field], 0

        ;; Reset address info
        lea     rdi, [rel wolfasm_connect]
        xor     esi, esi
        mov     edx, 255
        call    _memset

        ;; Reset port info
        lea     rdi, [rel wolfasm_port]
        xor     esi, esi
        mov     edx, 6
        call    _memset

        call    _SDL_StopTextInput

        mov     rsp, rbp
        pop     rbp
        ret

wolfasm_multiplayer_connect_buttons:
        push    rbp
        mov     rbp, rsp

        ;; Render "Host" button
        lea     rdi, [rel host_in_txt]
        mov     esi, [rel window_width]
        shr     esi, 1
        mov     edx, [rel window_height]
        shr     edx, 1
        mov     ecx, [rel window_width]
        shr     ecx, 4
        xor     r8, r8
        call    wolfasm_menu_render_button

        ;; Check if any address to render
        lea     rdi, [rel wolfasm_connect]
        cmp     byte [rdi], 0
        je      .render_port

        ;; Render address
        call    _strlen
        mov     r8d, 15
        mul     r8d

        lea     rdi, [rel wolfasm_connect]
        mov     esi, [rel window_width]
        shr     esi, 1
        mov     r8d, [rel window_width]
        shr     r8d, 4
        add     esi, r8d
        mov     edx, [rel window_height]
        shr     edx, 1
        mov     ecx, eax
        xor     r8, r8
        call    wolfasm_menu_render_button

.render_port:
        ;; Render "Port" button
        lea     rdi, [rel port_in_txt]
        mov     esi, [rel window_width]
        shr     esi, 1
        mov     edx, [rel window_height]
        shr     edx, 1
        mov     r8d, [rel window_height]
        shr     r8d, 3
        add     edx, r8d
        mov     ecx, [rel window_width]
        shr     ecx, 4
        xor     r8, r8
        call    wolfasm_menu_render_button

        ;; Check if any port to render
        lea     rdi, [rel wolfasm_port]
        cmp     byte [rdi], 0
        je      .render_connect

        ;; Render port
        call    _strlen
        mov     r8d, 15
        mul     r8d

        lea     rdi, [rel wolfasm_port]
        mov     esi, [rel window_width]
        shr     esi, 1
        mov     r8d, [rel window_width]
        shr     r8d, 4
        add     esi, r8d
        mov     edx, [rel window_height]
        shr     edx, 1
        mov     r8d, [rel window_height]
        shr     r8d, 3
        add     edx, r8d
        mov     ecx, eax
        xor     r8, r8
        call    wolfasm_menu_render_button

.render_connect:
        ;; Render "Connect" button
        lea     rdi, [rel connect_txt]
        mov     esi, [rel window_width]
        shr     esi, 1
        mov     edx, [rel window_height]
        shr     edx, 1
        mov     r8d, [rel window_height]
        shr     r8d, 3
        shl     r8d, 1
        add     edx, r8d
        mov     ecx, [rel window_width]
        shr     ecx, 3
        xor     r8, r8
        mov     r9d, 1
        cmp     dword [rel selected_button], BUTTON_MP_CO_CONNECT
        cmove   r8d, r9d
        call    wolfasm_menu_render_button

        ;; Render "Back" button
        lea     rdi, [rel back_txt]
        mov     esi, [rel window_width]
        shr     esi, 1
        mov     r8d, [rel window_height]
        shr     r8d, 3
        mov     eax, 3
        mul     r8d
        mov     edx, [rel window_height]
        shr     edx, 1
        add     edx, eax
        mov     ecx, [rel window_width]
        shr     ecx, 4
        xor     r8, r8
        mov     r9d, 1
        cmp     dword [rel selected_button], BUTTON_MP_CO_BACK
        cmove   r8d, r9d
        call    wolfasm_menu_render_button

        mov     rsp, rbp
        pop     rbp
        ret


        ;;
        ;; Multiplayer Host menu functions
        ;;
wolfasm_multiplayer_host_map:
        push    rbp
        mov     rbp, rsp

        ;; Prepare game
        call    _Mix_HaltMusic
        call    _SDL_StopTextInput

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

        ;; Host game
        lea     rdi, [rel filename]
        mov     rsi, rax
        call    _wolfasm_host_game

        ;; Back to menu
        call    wolfasm_menu_play_music
        call    _SDL_StartTextInput

        mov     rsp, rbp
        pop     rbp
        ret

wolfasm_multiplayer_host_back:
        push    rbp
        mov     rbp, rsp

        mov     dword [rel menu], MENU_MULTIPLAYER
        mov     dword [rel wolfasm_menu_nb_buttons], NB_BUTTON_MULTIPLAYER
        mov     dword [rel selected_button], 0
        mov     dword [rel wolfasm_port_len], 0
        mov     dword [rel selected_text_field_max_len], 0
        mov     qword [rel selected_text_field_len], 0
        mov     qword [rel selected_text_field], 0

        ;; Reset port buffer
        lea     rdi, [rel wolfasm_port]
        xor     rsi, rsi
        mov     edx, 6
        call    _memset

        call    _SDL_StopTextInput

        mov     rsp, rbp
        pop     rbp
        ret

wolfasm_multiplayer_host_buttons:
        push    rbp
        mov     rbp, rsp

        ;; Get current map
        lea     rdi, [rel buff_file]
        mov     esi, 255 + 1 + 4
        lea     rdx, [rel file_fmt]
        mov     rcx, [rel wolfasm_selected_map]
        call    _snprintf

        ;; Render "Port" button
        lea     rdi, [rel port_in_txt]
        mov     esi, [rel window_width]
        shr     esi, 1
        mov     edx, [rel window_height]
        shr     edx, 1
        mov     ecx, [rel window_width]
        shr     ecx, 4
        xor     r8, r8
        call    wolfasm_menu_render_button

        lea     rdi, [rel wolfasm_port]
        cmp     byte [rdi], 0
        je      .render_map

        ;; Render port value
        call    _strlen
        mov     edi, 15
        mul     edi

        lea     rdi, [rel wolfasm_port]
        mov     esi, [rel window_width]
        shr     esi, 1
        mov     r8d, [rel window_width]
        shr     r8d, 4
        add     esi, r8d
        mov     edx, [rel window_height]
        shr     edx, 1
        mov     ecx, eax
        xor     r8, r8
        call    wolfasm_menu_render_button

.render_map:
        ;; Render map
        lea     rdi, [rel buff_file]
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
        cmp     dword [rel selected_button], BUTTON_MP_HOST_MAP
        cmove   r8d, r9d
        call    wolfasm_menu_render_button

        ;; Render "Back button"
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
        cmp     dword [rel selected_button], BUTTON_MP_HOST_BACK
        cmove   r8d, r9d
        call    wolfasm_menu_render_button

        mov     rsp, rbp
        pop     rbp
        ret

        ;;
        ;; Menu events
        ;;
wolfasm_menu_event_quit:
        push    rbp
        mov     rbp, rsp

        mov     byte [rel running], 0

        mov     rsp, rbp
        pop     rbp
        ret

wolfasm_menu_event_key_up:
        push    rbp
        mov     rbp, rsp

        mov     eax, [rel selected_button]
        cmp     eax, 0
        jne     .end_button

        mov     eax, [rel wolfasm_menu_nb_buttons]
.end_button:
        dec     eax
        mov     dword [rel selected_button], eax

        mov     rsp, rbp
        pop     rbp
        ret

wolfasm_menu_event_key_down:
        push    rbp
        mov     rbp, rsp

        mov     eax, [rel selected_button]
        inc     eax
        cmp     eax, [rel wolfasm_menu_nb_buttons]
        jne     .end_button
        xor     eax, eax
.end_button:
        mov     dword [rel selected_button], eax

        mov     rsp, rbp
        pop     rbp
        ret

wolfasm_menu_event_key_left:
        push    rbp
        mov     rbp, rsp

        mov     eax, [rel menu]
        cmp     eax, MENU_SELECT_MAP_SOLO
        je      .ok

        cmp     eax, MENU_MULTIPLAYER_HOST
        jne     .end

.ok:
        mov     eax, [rel wolfasm_current_map]
        dec     eax
        cmp     eax, 0
        jge     .store

        mov     eax, [rel wolfasm_nb_maps]
        dec     eax

.store:
        mov     dword [rel wolfasm_current_map], eax
        lea     rdi, [rel wolfasm_maps]
        mov     rax, [rdi + rax * 8]
        mov     qword [rel wolfasm_selected_map], rax
.end:
        mov     rsp, rbp
        pop     rbp
        ret

wolfasm_menu_event_key_right:
        push    rbp
        mov     rbp, rsp

        mov     eax, [rel menu]
        cmp     eax, MENU_SELECT_MAP_SOLO
        je      .ok

        cmp     eax, MENU_MULTIPLAYER_HOST
        jne     .end

.ok:
        mov     eax, [rel wolfasm_current_map]
        inc     eax
        cmp     eax, [rel wolfasm_nb_maps]
        jl     .store

        xor     eax, eax
.store:
        mov     dword [rel wolfasm_current_map], eax
        lea     rdi, [rel wolfasm_maps]
        mov     rax, [rdi + rax * 8]
        mov     qword [rel wolfasm_selected_map], rax
.end:

        mov     rsp, rbp
        pop     rbp
        ret

wolfasm_menu_event_key_tab:
        push    rbp
        mov     rbp, rsp

        mov     eax, [rel menu]
        cmp     eax, MENU_MULTIPLAYER_CONNECT
        jne     .end

        mov     rax, [rel selected_text_field]
        lea     rdi, [rel wolfasm_connect]
        cmp     rax, rdi
        jne     .turn_to_connect

.turn_to_port:
        lea     rax, [rel wolfasm_port]
        mov     qword [rel selected_text_field], rax
        mov     dword [rel selected_text_field_max_len], 6
        lea     rax, [rel wolfasm_port_len]
        mov     qword [rel selected_text_field_len], rax
        jmp     .end
.turn_to_connect:
        lea     rax, [rel wolfasm_connect]
        mov     qword [rel selected_text_field], rax
        mov     dword [rel selected_text_field_max_len], 255
        lea     rax, [rel wolfasm_connect_len]
        mov     qword [rel selected_text_field_len], rax
.end:
        mov     rsp, rbp
        pop     rbp
        ret

wolfasm_menu_event_key_enter:
        push    rbp
        mov     rbp, rsp

        lea     rdi, [rel callbacks]
        mov     eax, [rel menu]
        mov     rdi, [rdi + rax * 8]
        mov     eax, [rel selected_button]
        mov     rdi, [rdi + rax * 8]
        call    rdi

        mov     rsp, rbp
        pop     rbp
        ret

wolfasm_menu_event_key_backspace:
        push    rbp
        mov     rbp, rsp

        cmp     dword [rel menu], MENU_MULTIPLAYER_CONNECT
        je      .ok
        cmp     dword [rel menu], MENU_MULTIPLAYER_HOST
        jne     .end

.ok:
        mov     rax, [rel selected_text_field_len]
        cmp     dword [rax], 0
        jle     .end

        mov     rdi, [rel selected_text_field]
        mov     dword r8d, [rax]
        dec     r8d
        mov     byte [rdi + r8], 0

        mov     edi, [eax]
        dec     edi
        mov     dword [rax], edi

.end:
        mov     rsp, rbp
        pop     rbp
        ret

;; str in rdi
wolfasm_menu_event_textinput:
        push    rbp
        mov     rbp, rsp

        sub     rsp, 8
        push    rdi

        call    _strlen
        mov     r10, [rel selected_text_field_len]
        mov     r8d, [r10]
        add     r8d, eax

        mov     r9d, [rel selected_text_field_max_len]
        sub     r9d, 1

        cmp     r8d, r9d
        jl      .copy

        sub     r9d, [r10]
        mov     eax, r9d
.copy:
        cmp     eax, 0
        jle     .done

        push    r10
        push    rax

        mov     rdi, [rel selected_text_field]
        add     rdi, [r10]
        mov     rsi, [rsp + 16]
        mov     edx, eax
        call    _strncpy

        pop     rax
        pop     r10

        add     eax, [r10]
        mov     [r10], eax
.done:
        pop     rdi
        add     rsp, 8

        mov     rsp, rbp
        pop     rbp
        ret

        section .rodata
;; Text strings
play_txt:               db      "Play", 0x00
multiplayer_txt:        db      "Multiplayer", 0x00
exit_txt:               db      "Exit", 0x00
back_txt:               db      "Back", 0x00
host_txt:               db      "Host", 0x00
connect_txt:            db      "Connect", 0x00
host_in_txt:            db      "Host: ", 0x00
port_in_txt:            db      "Port: ", 0x00
filename_path:          db      "./resources/map/", 0x00
file_fmt:               db      "< %s >", 0x00
loading_error_msg:      db      "Cannot load maps", 0x0A, 0x00

;; Callbacks
callbacks_main_menu:    dq      wolfasm_main_play,                    \
                                wolfasm_main_multiplayer,             \
                                wolfasm_main_exit,                    \
                                wolfasm_main_buttons
callback_sm_solo:       dq      wolfasm_sm_solo_map,                  \
                                wolfasm_sm_solo_back,                 \
                                wolfasm_sm_solo_buttons
callbacks_multiplayer_menu:                                           \
                        dq      wolfasm_multiplayer_host,             \
                                wolfasm_multiplayer_connect,          \
                                wolfasm_multiplayer_back,             \
                                wolfasm_multiplayer_buttons
callback_mp_co:        dq       wolfasm_multiplayer_connect_connect,  \
                                wolfasm_multiplayer_connect_back,     \
                                wolfasm_multiplayer_connect_buttons
callback_mp_host:      dq       wolfasm_multiplayer_host_map,         \
                                wolfasm_multiplayer_host_back,        \
                                wolfasm_multiplayer_host_buttons

callbacks:             dq       callbacks_main_menu,                  \
                                callbacks_multiplayer_menu,           \
                                callback_sm_solo,                     \
                                callback_mp_co,                       \
                                callback_mp_host

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

cur_time:                         dq      0.0
old_time:                         dq      0.0

wolfasm_nb_maps:                  dd      0
wolfasm_current_map:              dd      0
wolfasm_maps:           times 255 dq      0

        section .bss
wolfasm_button_rect:  resd   4
wolfasm_connect:      resb   255
wolfasm_connect_len:  resd   1
wolfasm_port:         resb   6        ;; sizeof("65535")
wolfasm_port_len:     resd   1
filename:             resb   512
buff_file:            resb   255 + 1 + 4
