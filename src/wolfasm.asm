        [bits 64]

        %include "sdl.inc"
        %include "window.inc"

        section .text
        global wolfasm
        global window_ptr, window_surface, window_renderer,                 \
        window_width, window_height, window_texture

        ;; TODO: rm
        extern _c_init, _c_deinit

        ;; SDL functions
        extern _SDL_Init, _SDL_CreateWindow, _SDL_Quit, _SDL_DestroyWindow, \
        _SDL_GetWindowSurface, _SDL_GetRenderer, _SDL_SetRelativeMouseMode, \
        _SDL_GetRenderer, _SDL_CreateTexture

        ;; Syscalls
        extern _exit

        ;; wolfasm functions
        extern game_loop, wolfasm_gui_init, wolfasm_gui_deinit, \
        wolfasm_init_texture, wolfasm_deinit_texture,           \
        wolfasm_init_audio, wolfasm_deinit_audio,               \
        wolfasm_init_weapon, wolfasm_init_sprites,              \
        wolfasm_deinit_sprites, wolfasm_items_init

;; This function starts a window, and calls the game loop
wolfasm:
        push  rbp
        mov   rbp, rsp

.sdl_init:
        ;; Initialize SDL
        mov   rdi, SDL_INIT_VIDEO
        call  _SDL_Init

.window_init:
        ;; Start the window
        mov   rdi,  window_name ;; Window title
        mov   rsi, SDL_WINDOWPOS_UNDEFINED ;; X position
        mov   rdx, SDL_WINDOWPOS_UNDEFINED ;; Y Position
        mov   rcx, [rel window_width] ;; Width
        mov   r8, [rel window_height] ;; Height
        mov   r9, SDL_WINDOW_SHOWN | SDL_WINDOW_MOUSE_CAPTURE;; Flags
;;        mov   r9, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE ;; Flags
        call  _SDL_CreateWindow
        mov qword [rel window_ptr], rax

        ;; Check that is non NULL
        cmp   qword [rel window_ptr], 0x0
        je    .exit_fail

        ;; Get the window's surface
        mov   rdi, [rel window_ptr]
        call  _SDL_GetWindowSurface

        ;; Check that is non NULL
        cmp   rax, 0x0
        je    .exit_fail
        mov   qword [rel window_surface], rax

        ;; Get window's renderer
        mov   rdi, [rel window_ptr]
        call  _SDL_GetRenderer

        ;; Check that is non NULL
        cmp   rax, 0x0
        je    .exit_fail
        mov   qword [rel window_renderer], rax

        ;; Create window_texture
        mov   rdi, [rel window_renderer]
        mov   rsi, SDL_PIXELFORMAT_BGRA32
        mov   rdx, SDL_TEXTUREACCESS_STREAMING
        mov   rcx, [rel window_width]
        mov   r8, [rel window_height]
        call  _SDL_CreateTexture

        ;; Check if NULL
        cmp   rax, 0x00
        je    .exit_fail
        mov   qword [rel window_texture], rax

.device_init:
        ;; Initialize mouse support
        mov   rdi, 1
        call  _SDL_SetRelativeMouseMode

.sdl_plugins_init:
        ;; Initialize SDL_ttf
        call  wolfasm_gui_init

        ;; Initialize SDL_Mixer
        call  wolfasm_init_audio

        ;; TODO: Load map here

.game_data_init:
        ;; Initialize weapons
        call  wolfasm_init_weapon

        ;; Initialize last graphic elements
        call  wolfasm_init_texture
        call  wolfasm_init_sprites
        call  wolfasm_items_init

.game_loop:
        ;; Starts the game loop
        call  _c_init   ;; TODO: rm
        call  game_loop
        call  _c_deinit ;; TODO: rm

        ;; Clean sounds
        call  wolfasm_deinit_audio

        ;; Unload sprites
        call  wolfasm_deinit_sprites

        ;; Clean SDL_ttf
        call  wolfasm_gui_deinit
        call  wolfasm_deinit_texture

        ;; Destroy window
        mov   qword rdi, [rel window_ptr]
        call  _SDL_DestroyWindow

        ;; Leave program, everything went right
        mov   rax, 0
        jmp   .exit
.exit_fail:
        mov   rax, 1

.exit:
        ;; Cleanup before leaving
        push  rax
        sub   rsp, 8
        call  _SDL_Quit
        add   rsp, 8
        pop   rax

        mov   rsp, rbp
        pop   rbp
        ret

        section .data
window_width    dq WIN_WIDTH
window_height   dq WIN_HEIGHT
window_ptr      dq 1
window_surface  dq 1
window_renderer dq 1
window_texture dq 1

        section .rodata
window_name     db "WolfASM - bache_a", 0x00
