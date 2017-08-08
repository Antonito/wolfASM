        [bits 64]

        %include "texture.inc"
        %include "sdl.inc"

        global  wolfasm_init_texture, wolfasm_deinit_texture, wolfasm_texture

        ;; SDL2 functions
        extern _IMG_Init, _IMG_Quit, _IMG_Load, _SDL_FreeSurface,  \
        _SDL_GetError

        ;; LibC functions
        extern _exit, _puts

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

        ;; TODO: Check size of surface

        inc       rcx
        jmp       .loop

.texture_load_err:
        ;; Get SDL Error
        call       _SDL_GetError
        mov        rdi, rax
        call       _puts

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
wolfasm_texture_0:        dd  "./resources/img/stone.png", 0x00
wolfasm_texture_1:        dd  "./resources/img/stonebrick_mossy.png", 0x00
wolfasm_texture_2:        dd  "./resources/img/log_jungle.png", 0x00
wolfasm_texture_3:        dd  "./resources/img/brick.png", 0x00
wolfasm_texture_4:        dd  "./resources/img/stonebrick_cracked.png", 0x00
wolfasm_texture_5:        dd  "./resources/img/end_stone.png", 0x00
wolfasm_texture_6:        dd  "./resources/img/planks_oak.png", 0x00
wolfasm_texture_7:        dd  "./resources/img/sand.png", 0x00
wolfasm_texture_name:     dq  wolfasm_texture_0, \
                              wolfasm_texture_1, \
                              wolfasm_texture_2, \
                              wolfasm_texture_3, \
                              wolfasm_texture_4, \
                              wolfasm_texture_5, \
                              wolfasm_texture_6, \
                              wolfasm_texture_7

        section .data
wolfasm_texture_nb:       dd    TEXTURE_NB
wolfasm_texture_size:     dd    TEXTURE_SIZE

        section .bss
wolfasm_texture_surface:  resq  TEXTURE_NB

;; The actual buffer: wolfasm_texture[TEXTURE_NB][TEXTURE_SIZE * TEXTURE_SIZE]
wolfasm_texture:          resd  TEXTURE_NB * TEXTURE_SIZE * TEXTURE_SIZE