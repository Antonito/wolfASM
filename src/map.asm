        [bits 64]

        %include "map.inc"
        %include "items.inc"

        %define O_RDONLY  0x00

        global wolfasm_map_init, wolfasm_map_deinit,        \
        wolfasm_map_width, wolfasm_map_height,              \
        wolfasm_map, wolfasm_items, wolfasm_items_nb

        extern wolfasm_player_refill_ammo,                  \
        wolfasm_player_refill_life

        ;; LibC symbols
        extern _free, _puts, _exit, _open, _close, _read,   \
        _calloc

        section .text
wolfasm_map_init:
        push      rbp
        mov       rbp, rsp

        ;; Open file
        mov       rsi, O_RDONLY
        call      _open

        ;; Check if opened
        cmp       eax, -1
        je        .err

        ;; Save file descriptor
        push      rax
        push      rdi

        ;; Read map header
        mov       edi, eax
        lea       rsi, [rel wolfasm_map_header]
        mov       edx, WOLFASM_MAP_HEADER_SIZE
        call      _read

        ;; Check for any error
        cmp       eax, -1
        je        .err

        ;; Check magic number
        lea       rsi, [rel wolfasm_map_header]
        mov       r8w, [rsi + wolfasm_map_header_t.magic]
        cmp       r8w, WOLFASM_MAP_MAGIC
        jne       .err

        ;; Fill datas
        mov       r8d, [rsi + wolfasm_map_header_t.width]
        mov       dword [rel wolfasm_map_width], r8d
        mov       r9d, [rsi + wolfasm_map_header_t.height]
        mov       dword [rel wolfasm_map_height], r9d

        ;; Alloc map
        xor       rdx, rdx
        mov       eax, WOLFASM_MAP_CASE_SIZE
        mul       r8d
        mul       r9d
        mov       edi, eax
        mov       esi, 1

        sub       rsp, 8
        push      rdi
        call      _calloc

        pop       rdi
        add       rsp, 8

        ;; Check any error
        cmp       rax, 0
        je        .err
        mov       [rel wolfasm_map], rax

        ;; Read map datas
        mov       rdx, rdi
        mov       rsi, [rel wolfasm_map]
        mov       rdi, [rsp + 8]
        call      _read

        ;; Check any error
        cmp       eax, -1
        je        .err

        ;; Read items
        mov       rdi, [rsp + 8]
        lea       rsi, [rel wolfasm_map_items_header]
        mov       rdx, WOLFASM_MAP_ITEM_HEADER_SIZE
        call      _read

        ;; Check errors
        cmp       eax, -1
        je        .err

        ;; Save header's datas
        lea       r8, [rel wolfasm_map_items_header]
        mov       r8d, [r8 + wolfasm_map_items_header_t.nb_items]
        mov       dword [rel wolfasm_items_nb], r8d

        ;; Allocate item's data
        mov       eax, WOLFASM_ITEM_SIZE
        mul       r8d
        mov       edi, eax
        mov       rsi, 1
        call      _calloc

        ;; Check any error
        cmp       rax, 0
        je        .err

        ;; Store pointer
        mov       qword [rel wolfasm_items], rax
        xor       rcx, rcx
        mov       edi, [rel wolfasm_items_nb]
.loop_items:
        cmp       ecx, edi
        je        .loop_items_end

        push      rdi
        push      rcx

        ;; Read current chunk of data
        mov       rdi, [rsp + 24]
        lea       rsi, [rel wolfasm_map_item]
        mov       edx, WOLFASM_MAP_ITEM_SIZE
        call      _read

        ;; Check for error
        cmp       eax, -1
        je        .err

        pop       rcx
        pop       rdi

        ;; Get data containers
        mov       eax, WOLFASM_ITEM_SIZE
        mul       ecx
        mov       r8, [rel wolfasm_items]
        add       r8, rax
        lea       r9, [rel wolfasm_map_item]

        ;; Fill datas here
        mov       eax, [r9 + wolfasm_map_item_t.texture]
        mov       dword [r8 + wolfasm_item_s.texture], eax
        mov       eax, [r9 + wolfasm_map_item_t.pos_x]
        mov       dword [r8 + wolfasm_item_s.pos_x], eax
        mov       eax, [r9 + wolfasm_map_item_t.pos_y]
        mov       dword [r8 + wolfasm_item_s.pos_y], eax
        mov       eax, [r9 + wolfasm_map_item_t.width_div]
        mov       dword [r8 + wolfasm_item_s.width_div], eax
        mov       eax, [r9 + wolfasm_map_item_t.height_div]
        mov       dword [r8 + wolfasm_item_s.height_div], eax
        movsd       xmm0, [r9 + wolfasm_map_item_t.height_move]
        movsd       [r8 + wolfasm_item_s.height_move], xmm0
        mov       eax, [r9 + wolfasm_map_item_t.current_anim]
        mov       dword [r8 + wolfasm_item_s.current_anim], eax
        mov       eax, [r9 + wolfasm_map_item_t.nb_anim]
        mov       dword [r8 + wolfasm_item_s.nb_anim], eax
        mov       eax, [r9 + wolfasm_map_item_t.anim_rate]
        mov       dword [r8 + wolfasm_item_s.anim_rate], eax
        mov       eax, [r9 + wolfasm_map_item_t.stock]
        mov       dword [r8 + wolfasm_item_s.stock], eax
        mov       eax, [r9 + wolfasm_map_item_t.type]
        mov       dword [r8 + wolfasm_item_s.type], eax

        mov       eax, [r9 + wolfasm_map_item_t.texture_table]
        lea       r10, [rel item_animation_table]
        shl       eax, 2        ;; * 4
        add       r10, rax
        mov       eax, [r10]
        mov       dword [r8 + wolfasm_item_s.texture_table], eax

        mov       eax, [r9 + wolfasm_map_item_t.callback]
        lea       r10, [rel item_callback_table]
        shl       eax, 3       ;; * 8
        add       r10, rax
        mov       rax, [r10]
        mov       qword [r8 + wolfasm_item_s.callback], rax

        inc       rcx
        jmp       .loop_items
.loop_items_end:

        pop       rdi
        pop       rax

        ;; Close file
        mov       rdi, rax
        call      _close

        ;; Check if closed
        cmp       eax, -1
        je        .err

        mov       rsp, rbp
        pop       rbp
        emms
        ret
.err:
        mov       rdi, error_msg
        call       _puts
        mov       rdi, 1
        call      _exit

wolfasm_map_deinit:
        push      rbp
        mov       rbp, rsp

        ;; Free allocated datas
        mov       rdi, [rel wolfasm_map]
        call      _free
        mov       rdi, [rel wolfasm_items]
        call      _free

        ;; Reset datas
        mov       dword [rel wolfasm_map_width], 0
        mov       dword [rel wolfasm_map_height], 0
        mov       dword [rel wolfasm_items_nb], 0
        mov       qword [rel wolfasm_map], 0
        mov       qword [rel wolfasm_items], 0

        mov       rsp, rbp
        pop       rbp
        ret

        section .rodata
error_msg:            db    "Cannot load map", 0x00
item_animation_table: dd    NO_TABLE, TABLE_ENEMY_ANIMATION_SHOOT
item_callback_table:  dq    wolfasm_player_refill_life, wolfasm_player_refill_ammo

        section .data
wolfasm_map_width:    dd    0
wolfasm_map_height:   dd    0
wolfasm_items_nb:     dd    0
wolfasm_map:          dq    0
wolfasm_items:        dq    0

wolfasm_map_header:
istruc wolfasm_map_header_t
  at   wolfasm_map_header_t.magic,              dw    0
  at   wolfasm_map_header_t.width,              dd    0
  at   wolfasm_map_header_t.height,             dd    0
  at   wolfasm_map_header_t.name,   times 255   db    0
iend

wolfasm_map_items_header:
istruc wolfasm_map_items_header_t
  at   wolfasm_map_items_header_t.nb_items,     dd    0
iend

wolfasm_map_item:
istruc wolfasm_map_item_t
iend
