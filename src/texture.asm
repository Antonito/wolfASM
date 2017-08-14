        [bits 64]

        %include "texture.inc"
        %include "sdl.inc"

        global  wolfasm_init_texture, wolfasm_deinit_texture, wolfasm_texture

        ;; SDL2 functions
        extern _IMG_Init, _IMG_Quit, _IMG_Load, _SDL_FreeSurface,  \
        _SDL_GetError

        ;; LibC functions
        extern _exit, _puts, _printf

        ;; TODO: rm
        global wolfasm_texture_surface

        section .text
wolfasm_init_texture:
        push      rbp
        mov       rbp, rsp

        ;; Starts SDL_Img
        mov       rdi, IMG_INIT_PNG
        call      _IMG_Init
        ;; TODO: Check return ?

        xor       rcx, rcx
        mov       eax, [rel wolfasm_texture_nb]
.loop:
        cmp       rcx, rax
        je        .end_loop

        ;; Save loop counters
        sub       rsp, 8
        push      rcx
        sub       rsp, 8
        push      rax

        ;; Load image wolfasm_texture_name[i]
        lea       rdi, [rel wolfasm_texture_name]
        mov       rdi, [rdi + rcx * 8]
        call      _IMG_Load

        ;; Check if texture is loaded
        cmp       rax, 0
        je        .texture_load_err
        mov       r8, rax

        ;; Restore loop counters
        pop       rax
        add       rsp, 8
        pop       rcx
        add       rsp, 8

        ;; texture surface is now in r8
        lea       rsi, [rel wolfasm_texture_surface]
        mov       [rsi + rcx * 8], r8

        ;; Check that size if correct
        mov       r9d, [r8 + 16]  ;; width
        mov       r10d, [r8 + 20]  ;; height

        ;; Check width == height
        cmp       r9d, r10d
        jne       .texture_load_err

        ;; Check width == height == wolfasm_texture_size
        cmp       r9d, [rel wolfasm_texture_size]
        jne       .texture_load_err

        ;; Load buffer
        mov       r8, [r8 + 32]
        xor       rdi, rdi
        mov       esi, [rel wolfasm_texture_full_size]

.map_texture_loop:
        cmp       rdi, rsi
        je        .map_texture_loop_end

        mov       edx, [r8 + rdi * 4]   ;; Current pixel
        mov       r10, rcx
        shl       r10, 12
        lea       r9d, [rel wolfasm_texture]
        lea       r9, [r9 + r10 * 4]
        mov       [r9 + rdi * 4], edx

        inc       rdi
        jmp       .map_texture_loop

.map_texture_loop_end:
        inc       rcx
        jmp       .loop

.texture_load_err:
        mov       rdi, wolfasm_texture_err
        lea       rsi, [rel wolfasm_texture_name]
        mov       rsi, [rsi + rcx * 8]
        call      _printf
        ;; Get SDL Error
        call      _SDL_GetError
        mov       rdi, rax
        call      _puts

.exit_err:
        mov       rdi, 1
        call      _exit

.end_loop:
        mov       rsp, rbp
        pop       rbp
        ret

wolfasm_deinit_texture:
        push      rbp
        mov       rbp, rsp

        xor       rcx, rcx
        mov       eax, [rel wolfasm_texture_nb]

.loop:
        cmp       rcx, rax
        je        .end_loop

        ;; Save counters
        sub       rsp, 8
        push      rcx
        sub       rsp, 8
        push      rax

        ;; Destroy current surface
        lea       rdi, [rel wolfasm_texture_surface]
        mov       rdi, [rdi + rcx * 8]
        call      _SDL_FreeSurface

        ;; Restore counters
        pop       rax
        add       rsp, 8
        pop       rcx
        add       rsp, 8

        inc       rcx
        jmp       .loop

.end_loop:
        ;; Stop SDL_Img
        call      _IMG_Quit

        mov       rsp, rbp
        pop       rbp
        ret

        section .rodata
wolfasm_texture_err:      dd  "Cannot load texture %s", 0x0A, 0x00
wolfasm_texture_0:        dd  "./resources/textures/stone.png", 0x00
wolfasm_texture_1:        dd  "./resources/textures/stonebrick_mossy.png", 0x00
wolfasm_texture_2:        dd  "./resources/textures/log_jungle.png", 0x00
wolfasm_texture_3:        dd  "./resources/textures/brick.png", 0x00
wolfasm_texture_4:        dd  "./resources/textures/stonebrick_cracked.png", 0x00
wolfasm_texture_5:        dd  "./resources/textures/end_stone.png", 0x00
wolfasm_texture_6:        dd  "./resources/textures/planks_oak.png", 0x00
wolfasm_texture_7:        dd  "./resources/textures/sand.png", 0x00
wolfasm_texture_8:        dd  "./resources/textures/floor.png", 0x00
wolfasm_texture_9:        dd  "./resources/textures/ammo.png", 0x00
wolfasm_texture_10:       dd  "./resources/textures/medikit.png", 0x00
wolfasm_texture_11:       dd  "./resources/textures/enemy_shoot_0.png", 0x00
wolfasm_texture_12:       dd  "./resources/textures/enemy_shoot_1.png", 0x00
wolfasm_texture_13:       dd  "./resources/textures/enemy_shoot_2.png", 0x00
wolfasm_texture_14:       dd  "./resources/textures/enemy_dead_0.png", 0x00
wolfasm_texture_15:       dd  "./resources/textures/enemy_dead_1.png", 0x00
wolfasm_texture_16:       dd  "./resources/textures/enemy_dead_2.png", 0x00
wolfasm_texture_17:       dd  "./resources/textures/enemy_dead_3.png", 0x00
wolfasm_texture_18:       dd  "./resources/textures/enemy_dead_4.png", 0x00
wolfasm_texture_19:       dd  "./resources/textures/enemy_hit_0.png", 0x00
wolfasm_texture_name:     dq  wolfasm_texture_0,   \
                              wolfasm_texture_1,   \
                              wolfasm_texture_2,   \
                              wolfasm_texture_3,   \
                              wolfasm_texture_4,   \
                              wolfasm_texture_5,   \
                              wolfasm_texture_6,   \
                              wolfasm_texture_7,   \
                              wolfasm_texture_8,   \
                              wolfasm_texture_9,   \
                              wolfasm_texture_10,  \
                              wolfasm_texture_11,  \
                              wolfasm_texture_12,  \
                              wolfasm_texture_13,  \
                              wolfasm_texture_14,  \
                              wolfasm_texture_15,  \
                              wolfasm_texture_16,  \
                              wolfasm_texture_17,  \
                              wolfasm_texture_18,  \
                              wolfasm_texture_19

        section .data
wolfasm_texture_nb:       dd    TEXTURE_NB
wolfasm_texture_size:     dd    TEXTURE_SIZE
wolfasm_texture_full_size:dd    TEXTURE_SIZE * TEXTURE_SIZE

        section .bss
wolfasm_texture_surface:  resq  TEXTURE_NB

;; The actual buffer: wolfasm_texture[TEXTURE_NB][TEXTURE_SIZE * TEXTURE_SIZE]
wolfasm_texture:          resd  TEXTURE_NB * TEXTURE_SIZE * TEXTURE_SIZE
