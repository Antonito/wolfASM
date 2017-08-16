        [bits 64]

        %include "items.inc"
        %include "map.inc"
        %include "enemy.inc"
        %include "player.inc"

        global wolfasm_items_init, wolfasm_items,             \
        wolfasm_items_nb, wolfasm_display_items_and_mobs

        extern map, map_width, wolfasm_player_refill_ammo,    \
        wolfasm_player_refill_life, game_player,              \
        wolfasm_put_pixel, window_width, window_height,       \
        wolfasm_z_buffer, wolfasm_texture

        extern enemy_animation_shoot, enemy_animation_shoot_nb

        ;; LibC functions
        extern _abs

        ;; TODO: rm
        extern _comb_sort

        section .text

wolfasm_items_init:
        push      rbp
        mov       rbp, rsp

        xor       ecx, ecx
.loop:
        cmp       dword ecx, [rel wolfasm_items_nb]
        je        .end_loop

        ;; Get element offset
        lea       rdi, [rel wolfasm_items]
        mov       eax, 64
        mov       edx, ecx
        mul       edx
        mov       r9, rdi
        lea       rdi, [rdi + rax]

        ;; Compute map index (y * map_width + x)
        mov       esi, [rdi + wolfasm_item_s.pos_y]
        mov       r8d, [rdi + wolfasm_item_s.pos_x]
        mov       eax, [rel map_width]
        mul       esi
        add       eax, r8d
        mov       edx, eax

        ;; If enemy, don't store it
        mov       eax, ITEM_ENEMY
        cmp       eax, [rdi + wolfasm_item_s.type]
        je        .next_loop

        ;; Get map index
        mov       rax, [rel map]
        shl       rdx, 4            ;; WOLFASM_MAP_CASE_SIZE
        lea       rax, [rax + rdx + wolfasm_map_case.item]
        mov       [rax], rdi

.next_loop:
        inc       ecx
        jmp       .loop

.end_loop:
        mov       rsp, rbp
        pop       rbp
        ret

wolfasm_display_items_and_mobs:
        push      rbp
        mov       rbp, rsp

        xor       rcx, rcx
        mov       edi, [rel wolfasm_items_nb]
.fill_order_loop:
        cmp       ecx, edi
        je        .fill_order_end_loop

        ;; Fill index
        lea       rsi, [rel wolfasm_sprite_order]
        mov       r8, rcx
        shl       r8, 2
        add       esi, r8d
        mov       [rsi], ecx

        ;; Calculate distance
        lea       r9, [rel wolfasm_items]
        mov       eax, 64
        mov       edx, ecx
        mul       edx
        lea       r10, [r9 + rax]

        ;; (a - b) * (a - b)
        mov       r9d, [r10 + wolfasm_item_s.pos_x]
        cvtsi2sd  xmm1, r9
        movsd     xmm0, [rel game_player + wolfasm_player.pos_x]
        subsd     xmm0, xmm1
        mulsd     xmm0, xmm0

        ;; (c - d) * (c - d)
        mov       r9d, [r10 + wolfasm_item_s.pos_y]
        cvtsi2sd  xmm2, r9
        movsd     xmm1, [rel game_player + wolfasm_player.pos_y]
        subsd     xmm1, xmm2
        mulsd     xmm1, xmm1

        ;; (a - b) * (a - b) + (c - d) * (c - d)
        addsd     xmm0, xmm1

        ;; Fill distance
        lea       rsi, [rel wolfasm_sprite_distance]
        shl       r8, 1 ;; 2 * 2 * 2 == 8
        add       esi, r8d
        movsd     [rsi], xmm0

        inc       rcx
        jmp       .fill_order_loop
.fill_order_end_loop:
        mov       rdi, wolfasm_sprite_order
        mov       rsi, wolfasm_sprite_distance
        mov       edx, [rel wolfasm_items_nb]
        call      _comb_sort

        xor       rcx, rcx
        mov       edi, [rel wolfasm_items_nb]
.item_loop:
        cmp       ecx, edi
        je        .item_loop_end

        ;; Get index
        lea       r8, [rel wolfasm_sprite_order]
        mov       r8d, [r8 + rcx * 4]
        ;; Get item
        lea       r9, [rel wolfasm_items]
        mov       eax, 64
        mul       r8d
        lea       r8, [r9 + rax]
        mov       [rel wolfasm_item_current], r8

        cmp       dword [r8 + wolfasm_item_s.stock], 0
        je        .no_stock

.sprite_position:
        ;; Calculate spriteX position
        mov       eax, [r8 + wolfasm_item_s.pos_x]
        cvtsi2sd  xmm0, eax
        movsd     xmm1, [rel game_player + wolfasm_player.pos_x]
        subsd     xmm0, xmm1

        ;; Calculate spriteY position
        mov       eax, [r8 + wolfasm_item_s.pos_y]
        cvtsi2sd  xmm1, eax
        movsd     xmm2, [rel game_player + wolfasm_player.pos_y]
        subsd     xmm1, xmm2

        ;; Calculate inversion det
        movsd     xmm2, [rel game_player + wolfasm_player.plane_x]
        movsd     xmm3, [rel game_player + wolfasm_player.dir_y]
        mulsd     xmm2, xmm3
        movsd     xmm3, [rel game_player + wolfasm_player.dir_x]
        movsd     xmm4, [rel game_player + wolfasm_player.plane_y]
        mulsd     xmm3, xmm4
        subsd     xmm2, xmm3
        mov       eax, 1
        cvtsi2sd  xmm3, rax
        divsd     xmm3, xmm2
        movsd     xmm2, xmm3

        ;; Compute transformation matrix
.sprite_transform:
        ;; Compute X transform
        movsd     xmm3, [rel game_player + wolfasm_player.dir_y]
        mulsd     xmm3, xmm0
        movsd     xmm4, [rel game_player + wolfasm_player.dir_x]
        mulsd     xmm4, xmm1
        subsd     xmm3, xmm4
        mulsd     xmm3, xmm2
        movsd     [rel wolfasm_item_transform_x], xmm3

        ;; Compute Y transform
        movsd     xmm4, [rel game_player + wolfasm_player.plane_y]
        mov       rax, -1
        cvtsi2sd  xmm5, rax
        mulsd     xmm4, xmm5    ;; * -1
        mulsd     xmm4, xmm0
        movsd     xmm5, [rel game_player + wolfasm_player.plane_x]
        mulsd     xmm5, xmm1
        addsd     xmm4, xmm5
        mulsd     xmm4, xmm2
        movsd     [rel wolfasm_item_transform_y], xmm4

        ;; Compute Vertical move on screen
        movsd     xmm0, [r8 + wolfasm_item_s.height_move]
        divsd     xmm0, xmm4
        cvttsd2si eax, xmm0
        mov       dword [rel wolfasm_item_vmove_screen], eax

        ;; Compute wolfasm_item_sprite_screen_x
        movsd     xmm0, xmm3
        divsd     xmm0, xmm4
        mov       eax, 1
        cvtsi2sd  xmm1, rax
        addsd     xmm0, xmm1
        mov       eax, [rel window_width]
        shr       rax, 1          ;; Divide by 2
        cvtsi2sd  xmm1, rax
        mulsd     xmm1, xmm0
        cvttsd2si eax, xmm1
        mov       dword [rel wolfasm_item_sprite_screen_x], eax

.sprite_height:
        ;; Compute height of the sprite on screen
        mov       eax, [rel window_height]
        cvtsi2sd  xmm0, eax
        divsd     xmm0, xmm4

        sub       rsp, 8
        push      r8
        push      rcx
        push      rdi

        cvttsd2si rdi, xmm0
        call      _abs
        xor       rdx, rdx
        mov       r8, [rsp + 16]
        mov       esi, [r8 + wolfasm_item_s.height_div]
        div       esi
        mov       [rel wolfasm_item_sprite_height], eax

.sprite_width:
        ;; Compute width of the sprite on the screen
        mov       eax, [rel window_width]
        cvtsi2sd  xmm0, eax
        divsd     xmm0, xmm4
        cvttsd2si rdi, xmm0
        call      _abs
        xor       rdx, rdx
        mov       r8, [rsp + 16]
        mov       esi, [r8 + wolfasm_item_s.width_div]
        div       esi
        mov       [rel wolfasm_item_sprite_width], eax

.draw_start_y:
        mov       eax, [rel window_height]
        shr       rax, 1                  ;; Divide by two
        mov       edi, [rel wolfasm_item_vmove_screen]
        add       eax, edi
        mov       edi, [rel wolfasm_item_sprite_height]
        shr       edi, 1
        neg       edi
        add       edi, eax
        cmp       edi, 0
        jge       .store_draw_start_y
        xor       edi, edi
.store_draw_start_y:
        mov       [rel wolfasm_item_draw_start_y], edi

.draw_end_y:
        mov       edi, [rel wolfasm_item_sprite_height]
        shr       edi, 1
        add       edi, eax
        cmp       dword edi, [rel window_height]
        jl        .store_draw_end_y
        mov       edi, [rel window_height]
        sub       edi, 1
.store_draw_end_y:
        mov       [rel wolfasm_item_draw_end_y], edi

.draw_start_x:
        mov       edi, [rel wolfasm_item_sprite_width]
        shr       edi, 1
        neg       edi
        mov       eax, [rel wolfasm_item_sprite_screen_x]
        add       edi, eax
        cmp       edi, 0
        jge       .store_draw_start_x
        xor       edi, edi
.store_draw_start_x:
        mov       [rel wolfasm_item_draw_start_x], edi

.draw_end_x:
        mov       edi, [rel wolfasm_item_sprite_width]
        shr       edi, 1
        add       edi, eax
        cmp       dword edi, [rel window_width]
        jl        .store_draw_end_x
        mov       edi, [rel window_width]
        sub       edi, 1

.store_draw_end_x:
        mov       [rel wolfasm_item_draw_end_x], edi

        mov       ecx, [rel wolfasm_item_draw_start_x]
.loop_x:
        cmp       ecx, [rel wolfasm_item_draw_end_x]
        jge       .loop_x_end

        ;; (wolfasm_item_sprite_width / 2) * -1
        mov       eax, [rel wolfasm_item_sprite_width]
        shr       eax, 1    ;; / 2
        neg       rax

        ;; + wolfasm_item_sprite_screen_x
        mov       edx, [rel wolfasm_item_sprite_screen_x]
        add       eax, edx

        ;; ecx - ((wolfasm_item_sprite_width / 2) * -1 +
        ;;         wolfasm_item_sprite_screen_x)
        mov       edx, ecx
        sub       edx, eax
        shl       edx, 14    ;; * 64 * 256

        ;; / wolfasm_item_sprite_width / 256
        mov       eax, edx
        mov       esi, [rel wolfasm_item_sprite_width]
        xor       rdx, rdx
        div       esi
        shr       eax, 8      ;; / 256
        mov       dword [rel wolfasm_item_texture_x], eax


        cmp       ecx, 0
        je        .loop_y_end
        cmp       dword ecx, [rel window_width]
        jge       .loop_y_end
        xor       rax, rax
        cvtsi2sd  xmm0, rax
        movsd     xmm1, [rel wolfasm_item_transform_y]
        ucomisd   xmm1, xmm0
        jbe       .loop_y_end
        lea       r8, [rel wolfasm_z_buffer]
        movsd     xmm0, [r8 + rcx * 8]
        ucomisd   xmm1, xmm0
        jae       .loop_y_end

        mov       r8d, [rel wolfasm_item_draw_start_y]
.loop_y_start:
        cmp       r8d, [rel wolfasm_item_draw_end_y]
        jge       .loop_y_end

        mov       eax, r8d
        sub       eax, [rel wolfasm_item_vmove_screen]
        shl       eax, 8    ;; * 256
        mov       edx, [rel window_height]
        shl       edx, 7    ;; * 128
        sub       eax, edx
        mov       edx, [rel wolfasm_item_sprite_height]
        shl       edx, 7    ;; * 128
        add       eax, edx
        shl       eax, 6    ;; * 64

        mov       esi, [rel wolfasm_item_sprite_height]
        xor       rdx, rdx
        div       esi
        shr       eax, 8    ;; / 256

        ;; Compute ndx
        shl       eax, 6
        add       eax, [rel wolfasm_item_texture_x]

        mov       r10, [rel wolfasm_item_current]
        mov       r10d, [r10 + wolfasm_item_s.texture]
        shl       r10, 12
        lea       r9, [rel wolfasm_texture]
        lea       r9, [r9 + r10 * 4]
        mov       edx, [r9 + rax * 4]
        mov       r9d, edx
        and       r9d, 0xFF000000

        cmp       r9d, 0
        je        .next_y_loop

        push      r8
        push      rcx

        mov       edi, ecx
        mov       esi, r8d
        ;; edx already contains all we need
        call      wolfasm_put_pixel

        pop       rcx
        pop       r8

.next_y_loop:
        add       r8d, 1
        jmp       .loop_y_start
.loop_y_end:

        inc       ecx
        jmp       .loop_x
.loop_x_end:


        pop       rdi
        pop       rcx
        pop       r8
        add       rsp, 8

.animation:
        mov       r9, [r8 + wolfasm_item_s.texture_table]
        cmp       r9, 0
        je        .no_animation

        cmp       dword [r8 + wolfasm_item_s.stock], 0
        je        .no_animation

        ;; Go to next animation
        mov       eax, [r8 + wolfasm_item_s.current_anim]
        add       eax, 1
        mov       [r8 + wolfasm_item_s.current_anim], eax

        xor       edx, edx
        mov       esi, [r8 + wolfasm_item_s.anim_rate]
        div       esi

        cmp       dword eax, [r8 + wolfasm_item_s.nb_anim]
        jl        .switch_anim
        ;; Go back to first animation
        mov       dword [r8 + wolfasm_item_s.current_anim], 0
        xor       eax, eax

.switch_anim:
        mov       r9d, [r9 + rax * 4]
        mov       [r8 + wolfasm_item_s.texture], r9d

.no_animation:
.no_stock:

        inc       rcx
        jmp       .item_loop

.item_loop_end:

        mov       rsp, rbp
        pop       rbp
        emms
        ret


        section .data
wolfasm_items:
istruc wolfasm_item_s
      at wolfasm_item_s.texture,        dd    11
      at wolfasm_item_s.pos_x,          dd    4
      at wolfasm_item_s.pos_y,          dd    4
      at wolfasm_item_s.width_div,      dd    1
      at wolfasm_item_s.height_div,     dd    1
      at wolfasm_item_s.height_move,    dq    1.0
      at wolfasm_item_s.current_anim,   dd    0
      at wolfasm_item_s.nb_anim,        dd    ENEMY_ANIMATION_SHOOT_NB
      at wolfasm_item_s.anim_rate,      dd    10
      at wolfasm_item_s.texture_table,  dq    enemy_animation_shoot
      at wolfasm_item_s.stock,          dd    -1
      at wolfasm_item_s.type,           dd    ITEM_ENEMY
      at wolfasm_item_s.callback,       dq    0
iend
istruc wolfasm_item_s
      at wolfasm_item_s.texture,        dd    9
      at wolfasm_item_s.pos_x,          dd    4
      at wolfasm_item_s.pos_y,          dd    3
      at wolfasm_item_s.width_div,      dd    2
      at wolfasm_item_s.height_div,     dd    2
      at wolfasm_item_s.height_move,    dq    64.0
      at wolfasm_item_s.current_anim,   dd    0
      at wolfasm_item_s.nb_anim,        dd    0
      at wolfasm_item_s.anim_rate,      dd    1
      at wolfasm_item_s.texture_table,  dq    0
      at wolfasm_item_s.stock,          dd    5
      at wolfasm_item_s.type,           dd    ITEM_AMMO
      at wolfasm_item_s.callback,       dq    wolfasm_player_refill_ammo
iend
istruc wolfasm_item_s
      at wolfasm_item_s.texture,        dd    9
      at wolfasm_item_s.pos_x,          dd    2
      at wolfasm_item_s.pos_y,          dd    3
      at wolfasm_item_s.width_div,      dd    1
      at wolfasm_item_s.height_div,     dd    1
      at wolfasm_item_s.height_move,    dq    64.0
      at wolfasm_item_s.current_anim,   dd    0
      at wolfasm_item_s.nb_anim,        dd    0
      at wolfasm_item_s.anim_rate,      dd    1
      at wolfasm_item_s.texture_table,  dq    0
      at wolfasm_item_s.stock,          dd    -1 ; 5
      at wolfasm_item_s.type,           dd    ITEM_AMMO
      at wolfasm_item_s.callback,       dq    wolfasm_player_refill_ammo
iend
istruc wolfasm_item_s
      at wolfasm_item_s.texture,        dd    10
      at wolfasm_item_s.pos_x,          dd    4
      at wolfasm_item_s.pos_y,          dd    2
      at wolfasm_item_s.width_div,      dd    2
      at wolfasm_item_s.height_div,     dd    2
      at wolfasm_item_s.height_move,    dq    64.0
      at wolfasm_item_s.current_anim,   dd    0
      at wolfasm_item_s.nb_anim,        dd    0
      at wolfasm_item_s.anim_rate,      dd    1
      at wolfasm_item_s.texture_table,  dq    0
      at wolfasm_item_s.stock,          dd    -1 ; 5
      at wolfasm_item_s.type,           dd    ITEM_MEDIKIT
      at wolfasm_item_s.callback,       dq    wolfasm_player_refill_life
iend
wolfasm_items_nb:                       dd    4

      section .bss
wolfasm_sprite_order:                   resd   4
wolfasm_sprite_distance:                resq   4
wolfasm_item_transform_x:               resq   1
wolfasm_item_transform_y:               resq   1
wolfasm_item_vmove_screen:              resd   1
wolfasm_item_sprite_screen_x:           resd   1
wolfasm_item_sprite_height:             resd   1
wolfasm_item_sprite_width:              resd   1
wolfasm_item_draw_start_y:              resd   1
wolfasm_item_draw_end_y:                resd   1
wolfasm_item_draw_start_x:              resd   1
wolfasm_item_draw_end_x:                resd   1
wolfasm_item_texture_x:                 resd   1
wolfasm_item_current:                   resq   1
