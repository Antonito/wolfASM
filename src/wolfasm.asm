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
        wolfasm_deinit_sprites, wolfasm_items_init,             \
        wolfasm_map_init, wolfasm_map_deinit

        ;; TODO: rm
        extern _spawn_player

;; This function starts a window, and calls the game loop
wolfasm:
        push  rbp
        mov   rbp, rsp

        sub   rsp, 8
        push  rdi
.game_data_init:
        ;; Initialize weapons
        call  wolfasm_init_weapon

        ;; Initialize last graphic elements
        call  wolfasm_init_texture

        ;; Load map
        pop   rdi
        add   rsp, 8
        call  wolfasm_map_init

        call  wolfasm_items_init

.game_loop:
        ;; Starts the game loop
        call  _c_init   ;; TODO: rm
        call  _spawn_player
        call  game_loop
        call  _c_deinit ;; TODO: rm

        ;; Unload textures
        call  wolfasm_deinit_texture
        call  wolfasm_map_deinit

        ;; Leave program, everything went right
        mov   rax, 0
        jmp   .exit
.exit_fail:
        mov   rax, 1

.exit:

        mov   rsp, rbp
        pop   rbp
        ret
