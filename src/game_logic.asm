        [bits 64]

        %include "player.inc"
        %include "map.inc"
        %include "items.inc"

        global wolfasm_logic

        extern wolfasm_map_width, wolfasm_map, game_player

        ;; TODO: rm
        extern _game_logic_cwrapper

        section .text
wolfasm_logic:
        push      rbp
        mov       rbp, rsp

.collectable:
        ;; Compute map index (y * map_width + x)
        movsd     xmm0, [rel game_player + wolfasm_player.pos_y]
        mov       eax, [rel wolfasm_map_width]
        cvtsi2sd  xmm1, rax
        mulsd     xmm0, xmm1
        movsd     xmm1, [rel game_player + wolfasm_player.pos_x]
        addsd     xmm0, xmm1
        cvttsd2si rcx, xmm0

        ;; Get map index
        mov       rax, [rel wolfasm_map]
        shl       rcx, 4            ;; WOLFASM_MAP_CASE_SIZE
        lea       rdi, [rax + rcx]
        lea       r8, [rdi + wolfasm_map_case.item]
        mov       rdi, [rdi + wolfasm_map_case.item]

        ;; Check if any item on this case
        cmp       rdi, 0
        je        .no_collectable

        mov       rsi, [rdi + wolfasm_item_s.callback]
        cmp       rsi, 0
        je        .no_collectable_callback

        ;; Call callback
        push      rdi
        push      r8
        call      rsi
        pop       r8
        pop       rdi

.no_collectable_callback:
        ;; Check stock
        mov       esi, [rdi + wolfasm_item_s.stock]
        cmp       esi, 0
        jle       .no_collectable_stock

        ;; Consume one item (dec stock)
        sub       esi, 1
        mov       [rdi + wolfasm_item_s.stock], esi

        ;; if stock == 0, we should rm this item
        cmp       esi, 0
        jne       .no_collectable_stock
        mov       qword [r8], 0

.no_collectable_stock:
.no_collectable:
        call      _game_logic_cwrapper

        mov       rsp, rbp
        pop       rbp
        emms
        ret
