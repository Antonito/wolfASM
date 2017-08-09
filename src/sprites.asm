        [bits 64]

        %include "sprite.inc"

        global wolfasm_init_sprites, wolfasm_deinit_sprites,        \
        wolfasm_display_sprites, wolfasm_sprite

        ;; wolfasm symbols
        extern window_renderer

        ;; SDL functions
        extern _SDL_DestroyTexture, _SDL_RenderCopy, _SDL_GetError, \
        _IMG_Load, _SDL_FreeSurface, _SDL_MapRGB, _SDL_SetColorKey, \
        _SDL_CreateTextureFromSurface

        ;; LibC functions
        extern _exit, _puts

        ;;TODO: rm
        global wolfasm_render_sprite

        ;; TODO: rm
        extern _display_sprites_cwrapper

        section .text

wolfasm_init_sprites:
        push      rbp
        mov       rbp, rsp

        xor       rcx, rcx
        mov       edi, NB_WOLFASM_SPRITES

.loop:
        cmp       rcx, rdi
        je        .loop_end

        push      rcx   ;; Save counter
        push      rdi   ;; Save end

        ;; Get surface
        lea       rdi, [rel wolfasm_sprite_file]
        mov       rdi, [rdi + rcx * 8]
        call      _IMG_Load

        ;; Check if NULL
        cmp       rax, 0
        je        .err

        ;; Setup the SDL to treat the CYAN pixel as a transparent pixel
.set_transparency_pixel:
        mov       rdi, rax
        push      rdi     ;; Save surface
        sub       rsp, 8  ;; Align stack to 16 bytes

        mov       rdi, [rdi + 8] ;; Format offset
        xor       rsi, rsi        ;; 0x00
        mov       rdx, 0xFF
        mov       rcx, 0xFF
        call      _SDL_MapRGB

        add       rsp, 8
        pop       rdi     ;; Restore surface
        push      rdi     ;; Save it again for later use
        sub       rsp, 8
        mov       esi, 1
        mov       rdx, rax
        call      _SDL_SetColorKey

        add       rsp, 8
        pop       rdi
        push      rdi
        sub       rsp, 8

.create_texture:
        mov       rsi, rdi
        mov       rdi, [rel window_renderer]
        call      _SDL_CreateTextureFromSurface

        ;; Check if not NULL
        cmp       rax, 0
        je        .err

        add       rsp, 8
        pop       rdi
        push      rax
        sub       rsp, 8

        ;; Free old surface
        call      _SDL_FreeSurface

        add       rsp, 8
        pop       rax
        pop       rdi   ;; Restore end
        pop       rcx   ;; Restore counter

.store_texture:
        ;; Store texture
        mov       r9, rax
        mov       eax, wolfasm_sprite_s.size
        lea       r8, [rel wolfasm_sprite]
        mul       ecx
        add       r8, rax
        mov       [r8 + wolfasm_sprite_s.texture], r9

        inc       rcx
        jmp       .loop

.loop_end:
        mov       rsp, rbp
        pop       rbp
        ret
.err:
        call      _SDL_GetError
        mov       rdi, rax
        call      _puts

        mov       rdi, 1
        call      _exit

wolfasm_deinit_sprites:
        push      rbp
        mov       rbp, rsp

        xor       rcx, rcx
        mov       edi, NB_WOLFASM_SPRITES
.loop:
        cmp       rcx, rdi
        je        .end_loop

        push      rcx   ;; Save counter
        push      rdi   ;; Save end

        ;; Calculate offset
        mov       eax,  wolfasm_sprite_s.size
        mul       ecx

        ;; Get sprite
        lea       rdi, [rel wolfasm_sprite]
        add       rdi, rax

        ;; Get texture
        mov       rdi, [rdi + wolfasm_sprite_s.texture]
        call      _SDL_DestroyTexture

        pop       rdi   ;; Restore end
        pop       rcx   ;; Restore counter

        inc       rcx
        jmp       .loop
.end_loop:
        mov       rsp, rbp
        pop       rbp
        ret

;; void
;; wolfasm_render_sprite(wolfasm_sprite_s *sprite, int x, int y, SDL_Rect *clip)
wolfasm_render_sprite:
        push      rbp
        mov       rbp, rsp

.fill_quad:
        mov       [rel wolfasm_render_quad], esi        ;; x
        mov       [rel wolfasm_render_quad + 4], edx    ;; y
        mov       r8d, [rdi + wolfasm_sprite_s.width]
        mov       [rel wolfasm_render_quad + 8], r8d    ;; w
        mov       r8d, [rdi + wolfasm_sprite_s.height]
        mov       [rel wolfasm_render_quad + 12], r8d   ;; h

.render:
        mov       rsi, [rdi + wolfasm_sprite_s.texture]
        mov       rdi, [rel window_renderer]
        mov       rdx, rcx
        mov       rcx, wolfasm_render_quad
        call      _SDL_RenderCopy

        mov       rsp, rbp
        pop       rbp
        ret

wolfasm_display_sprites:
        push      rbp
        mov       rbp, rsp

        call      _display_sprites_cwrapper

        mov       rsp, rbp
        pop       rbp
        ret

        section .rodata
;; Sprite files
wolfasm_sprite_file_0:  db  "./resources/sprites/pistol.png", 0x00
wolfasm_sprite_file_1:  db  "./resources/sprites/shotgun.png", 0x00
wolfasm_sprite_file_2:  db  "./resources/sprites/barrel.png", 0x00
wolfasm_sprite_file:    dq  wolfasm_sprite_file_0,  \
                            wolfasm_sprite_file_1,  \
                            wolfasm_sprite_file_2

;; SPRITE_PISTOL             x      y     w     h
sprite_pistol_0:       dd    0,    42,   60,   62
sprite_pistol_1:       dd    60,   22,   82,   82
sprite_pistol_2:       dd    142,  23,   70,   81
sprite_pistol_3:       dd    212,  23,   64,   81
sprite_pistol_4:       dd    276,   1,   80,  103

;; SPRITE_SHOTGUN
sprite_shotgun_0:      dd    0,    91,   79,   60
sprite_shotgun_1:      dd    82,   30,  119,  121
sprite_shotgun_2:      dd    204,   0,   87,  151
sprite_shotgun_3:      dd    294,  20,  113,  130

;; SPRITE_BARELL
sprite_barrel_0:       dd    0,   75,   59,   55
sprite_barrel_1:       dd    62,  27,   83,  102
sprite_barrel_2:       dd    148,  0,  121,  130
sprite_barrel_3:       dd    272, 50,   81,   80

        section .data
wolfasm_sprite:
;; SPRITE_PISTOL
istruc wolfasm_sprite_s
        at wolfasm_sprite_s.texture,        dq    0
        at wolfasm_sprite_s.sprite,         dq    sprite_pistol_0
        at wolfasm_sprite_s.width,          dd    SPRITE_WEAPON_WIDTH
        at wolfasm_sprite_s.height,         dd    SPRITE_WEAPON_HEIGHT
        at wolfasm_sprite_s.nb_sprites,     dw    SPRITE_PISTOL_NB_ANIM
        at wolfasm_sprite_s.current_anim,   dw    0
        at wolfasm_sprite_s.trigger,        dd    0
iend
;; SPRITE_SHOTGUN
istruc wolfasm_sprite_s
        at wolfasm_sprite_s.texture,        dq    0
        at wolfasm_sprite_s.sprite,         dq    sprite_shotgun_0
        at wolfasm_sprite_s.width,          dd    SPRITE_WEAPON_WIDTH
        at wolfasm_sprite_s.height,         dd    SPRITE_WEAPON_HEIGHT
        at wolfasm_sprite_s.nb_sprites,     dw    SPRITE_SHOTGUN_NB_ANIM
        at wolfasm_sprite_s.current_anim,   dw    0
        at wolfasm_sprite_s.trigger,        dd    0
iend
;; SPRITE_BARREL
istruc wolfasm_sprite_s
        at wolfasm_sprite_s.texture,        dq    0
        at wolfasm_sprite_s.sprite,         dq    sprite_barrel_0
        at wolfasm_sprite_s.width,          dd    SPRITE_WEAPON_WIDTH
        at wolfasm_sprite_s.height,         dd    SPRITE_WEAPON_HEIGHT
        at wolfasm_sprite_s.nb_sprites,     dw    SPRITE_BARREL_NB_ANIM
        at wolfasm_sprite_s.current_anim,   dw    0
        at wolfasm_sprite_s.trigger,        dd    0
iend

      section .bss
wolfasm_render_quad:    resd  4
