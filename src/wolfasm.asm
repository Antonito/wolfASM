        [bits 64]

        %include "window.inc"

        section .text
        global wolfasm

        extern window_ptr, window_surface, window_renderer,       \
        window_width, window_height, window_texture

        ;; TODO: rm
        extern _c_init, _c_deinit

        ;; Syscalls
        extern _exit

        ;; wolfasm functions
        extern game_loop,                                       \
        wolfasm_init_texture, wolfasm_deinit_texture,           \
        wolfasm_init_weapon, wolfasm_init_sprites,              \
        wolfasm_deinit_sprites, wolfasm_items_init

;; This function starts a window, and calls the game loop
wolfasm:
        push  rbp
        mov   rbp, rsp

        ;; TODO: Load map here

.game_data_init:
        ;; Initialize weapons
        call  wolfasm_init_weapon

        ;; Initialize last graphic elements
        call  wolfasm_init_texture
        call  wolfasm_items_init

.game_loop:
        ;; Starts the game loop
        call  _c_init   ;; TODO: rm
        call  game_loop
        call  _c_deinit ;; TODO: rm

        ;; Unload textures
        call  wolfasm_deinit_texture

        ;; Leave program, everything went right
        mov   rax, 0
        jmp   .exit
.exit_fail:
        mov   rax, 1

.exit:

        mov   rsp, rbp
        pop   rbp
        ret
