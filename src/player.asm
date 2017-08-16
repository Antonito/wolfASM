        [bits 64]

        %include "player.inc"
        %include "map.inc"
        %include "weapon.inc"

        section .text

        global wolfasm_player_move_forward, wolfasm_player_move_backward, \
        wolfasm_player_rotate_right, wolfasm_player_rotate_left,          \
        wolfasm_player_refill_ammo, wolfasm_player_refill_life
        global game_player

        extern wolfasm_map_width, wolfasm_map

        ;; Lib C functions
        extern _cos, _sin

wolfasm_player_refill_ammo:
        push      rbp
        mov       rbp, rsp

        mov       rdi, [rel game_player + wolfasm_player.weapon]
        mov       ax, [rdi + wolfasm_weapon_s.max_ammo]
        mov       [rdi + wolfasm_weapon_s.ammo], ax

        mov       rsp, rbp
        pop       rbp
        ret

wolfasm_player_refill_life:
        push      rbp
        mov       rbp, rsp

        mov       rax, 100
        mov       [rel game_player + wolfasm_player.life], eax

        mov       rsp, rbp
        pop       rbp
        ret

wolfasm_player_move_forward:
        push      rbp
        mov       rbp, rsp

.update_x:
        ;; Compute x
        movsd     xmm1, [rel game_player + wolfasm_player.pos_x]
        movsd     xmm0, [rel game_player + wolfasm_player.dir_x]
        movsd     xmm2, [rel game_player + wolfasm_player.movement_speed]
        mulsd     xmm0, xmm2  ;; dir_x * movement_speed
        addsd     xmm0, xmm1  ;; + pos_x
        movsd     xmm4, xmm0  ;; Save for later
        cvttsd2si edi, xmm0   ;; Convert double to integer

        movsd     xmm0, [rel game_player + wolfasm_player.pos_y]
        cvttsd2si esi, xmm0

        ;; Compute map index (y * map_width + x)
        mov       eax, [rel wolfasm_map_width]
        mul       esi
        add       eax, edi
        mov       ecx, eax

        ;; Get map index
        mov       rax, [rel wolfasm_map]
        shl       rcx, 5            ;; WOLFASM_MAP_CASE_SIZE
        mov       byte dl, [rax + rcx]

        cmp       dl, 0
        jne       .update_y
        movsd     [rel game_player + wolfasm_player.pos_x], xmm4

.update_y:
        ;; Compute y
        movsd     xmm1, [rel game_player + wolfasm_player.pos_y]
        movsd     xmm0, [rel game_player + wolfasm_player.dir_y]
        movsd     xmm2, [rel game_player + wolfasm_player.movement_speed]
        mulsd     xmm0, xmm2  ;; dir_y * movement_speed
        addsd     xmm0, xmm1  ;; + pos_y
        movsd     xmm4, xmm0  ;; Save for later
        cvttsd2si rsi, xmm0   ;; Convert double to integer

        movsd     xmm0, [rel game_player + wolfasm_player.pos_x]
        cvttsd2si rdi, xmm0

        ;; Compute map index (y * map_width + x)
        mov       rax, [rel wolfasm_map_width]
        mul       esi
        add       rax, rdi
        mov       rcx, rax

        ;; Get map index
        mov       rax, [rel wolfasm_map]
        shl       rcx, 5            ;; WOLFASM_MAP_CASE_SIZE
        mov       byte dl, [rax + rcx]

        cmp       dl, 0
        jne       .end
        movsd     [rel game_player + wolfasm_player.pos_y], xmm4

.end:
        mov       rsp, rbp
        pop       rbp
        emms
        ret

wolfasm_player_move_backward:
        push      rbp
        mov       rbp, rsp

.update_x:
        ;; Compute x
        movsd     xmm1, [rel game_player + wolfasm_player.pos_x]
        movsd     xmm0, [rel game_player + wolfasm_player.dir_x]
        movsd     xmm2, [rel game_player + wolfasm_player.movement_speed]
        mulsd     xmm0, xmm2  ;; dir_x * movement_speed
        subsd     xmm1, xmm0  ;; pos_x - xmm0
        movsd     xmm4, xmm1  ;; Save for later
        cvttsd2si edi, xmm1   ;; Convert double to integer

        movsd     xmm0, [rel game_player + wolfasm_player.pos_y]
        cvttsd2si esi, xmm0


        ;; Compute map index (y * map_width + x)
        mov       eax, [rel wolfasm_map_width]
        mul       esi
        add       eax, edi
        mov       ecx, eax

        ;; Get map index
        mov       rax, [rel wolfasm_map]
        shl       rcx, 5            ;; WOLFASM_MAP_CASE_SIZE
        mov       byte dl, [rax + rcx]

        ;; Check that case is empty
        cmp       dl, 0
        jne       .update_y
        movsd     [rel game_player + wolfasm_player.pos_x], xmm4

.update_y:
        ;; Compute y
        movsd     xmm1, [rel game_player + wolfasm_player.pos_y]
        movsd     xmm0, [rel game_player + wolfasm_player.dir_y]
        movsd     xmm2, [rel game_player + wolfasm_player.movement_speed]
        mulsd     xmm0, xmm2  ;; dir_y * movement_speed
        subsd     xmm1, xmm0  ;; - pos_y
        movsd     xmm4, xmm1  ;; Save for later
        cvttsd2si edi, xmm1   ;; Convert double to integer

        movsd     xmm0, [rel game_player + wolfasm_player.pos_x]
        cvttsd2si esi, xmm0

        ;; Compute map index (y * map_width + x)
        mov       eax, [rel wolfasm_map_width]
        mul       edi
        add       eax, esi
        mov       ecx, eax

        ;; Get map index
        mov       rax, [rel wolfasm_map]
        shl       rcx, 5            ;; WOLFASM_MAP_CASE_SIZE
        mov       byte dl, [rax + rcx]

        ;; Check that case is empty
        cmp       dl, 0
        jne       .end
        movsd     [rel game_player + wolfasm_player.pos_y], xmm4
.end:
        mov       rsp, rbp
        pop       rbp
        emms
        ret

wolfasm_player_rotate_right:
        push      rbp
        mov       rbp, rsp

.update_direction:
.update_direction_x:
        movsd     xmm0, [rel game_player + wolfasm_player.rotation_speed]
        ;; Negate xmm0 TODO: There's probably a faster way with XOR
        mov       rax, -1
        cvtsi2sd  xmm1, rax
        mulsd     xmm0, xmm1
        sub       rsp, 16           ;; Allocate space
        movdqu    oword [rsp], xmm0 ;; save -rotation_speed
        call      _cos
        movdqu    xmm3, oword [rsp] ;; get -rotation_speed
        add       rsp, 16           ;; De-allocate space
        movsd     xmm5, xmm0        ;; Save cos(-rotation_speed)
        mulsd     xmm0, [rel game_player + wolfasm_player.dir_x]
        movsd     xmm2, xmm0        ;; dir_x * cos(-rotation_speed)

        movsd     xmm0, xmm3
        sub       rsp, 16           ;; Allocate space
        movdqu    oword [rsp], xmm2 ;; save xmm2
        sub       rsp, 16           ;; Allocate space
        movdqu    oword [rsp], xmm5 ;; save xmm5
        call      _sin
        movdqu    xmm5, oword [rsp] ;; get cos(-rotation_speed)
        add       rsp, 16           ;; Allocate space
        movdqu    xmm2, oword [rsp] ;; get dir_x * cos(-rotation_speed)
        add       rsp, 16           ;; De-allocate space
        movsd     xmm6, xmm0        ;; Save sin(-rotation_speed)
        mulsd     xmm0,  [rel game_player + wolfasm_player.dir_y]        ;; dir_y * sin(-rotation_speed)

        subsd     xmm2, xmm0
        movsd     xmm0, xmm2
        movsd     xmm2, [rel game_player + wolfasm_player.dir_x]
        movsd     [rel game_player + wolfasm_player.dir_x], xmm0

.update_direction_y:
        mulsd     xmm2, xmm6        ;; dir_x * sin(-rotation_speed)
        movsd     xmm1, [rel game_player + wolfasm_player.dir_y]
        mulsd     xmm1, xmm5        ;; dir_y * cos(-rotation_speed)
        addsd     xmm2, xmm1
        movsd     [rel game_player + wolfasm_player.dir_y], xmm2
        movsd     xmm0, xmm2

        ;; At this point xmm6 -> sin(-rotation_speed)
        ;;               xmm5 -> cos(-rotation_speed)
.update_camera_plane:
.update_camera_plane_x:
        movsd     xmm0, [rel game_player + wolfasm_player.plane_x]
        mulsd     xmm0, xmm5
        movsd     xmm1, [rel game_player + wolfasm_player.plane_y]
        mulsd     xmm1, xmm6
        subsd     xmm0, xmm1
        ;; Backup plane_x in xmm2
        movsd     xmm2, [rel game_player + wolfasm_player.plane_x]
        movsd     [rel game_player + wolfasm_player.plane_x], xmm0

.update_camera_plane_y:
        mulsd     xmm2, xmm6
        movsd     xmm0, [rel game_player + wolfasm_player.plane_y]
        mulsd     xmm0, xmm5
        addsd     xmm2, xmm0
        movsd     [rel game_player + wolfasm_player.plane_y], xmm2

        mov       rsp, rbp
        pop       rbp
        emms
        ret

wolfasm_player_rotate_left:
        push      rbp
        mov       rbp, rsp

.update_direction:
.update_direction_x:
        movsd     xmm0, [rel game_player + wolfasm_player.rotation_speed]
        call      _cos
        movsd     xmm5, xmm0        ;; Save cos(rotation_speed)
        mulsd     xmm0, [rel game_player + wolfasm_player.dir_x]
        movsd     xmm2, xmm0        ;; dir_x * cos(rotation_speed)

        movsd     xmm0, [rel game_player + wolfasm_player.rotation_speed]
        sub       rsp, 16           ;; Allocate space
        movdqu    oword [rsp], xmm2 ;; save xmm2
        sub       rsp, 16           ;; Allocate space
        movdqu    oword [rsp], xmm5 ;; save xmm5
        call      _sin
        movdqu    xmm5, oword [rsp] ;; get cos(rotation_speed)
        add       rsp, 16           ;; Allocate space
        movdqu    xmm2, oword [rsp] ;; get dir_x * cos(rotation_speed)
        add       rsp, 16           ;; De-allocate space
        movsd     xmm6, xmm0        ;; Save sin(rotation_speed)
        mulsd     xmm0,  [rel game_player + wolfasm_player.dir_y]        ;; dir_y * sin(rotation_speed)

        subsd     xmm2, xmm0
        movsd     xmm0, xmm2
        movsd     xmm2, [rel game_player + wolfasm_player.dir_x]
        movsd     [rel game_player + wolfasm_player.dir_x], xmm0

.update_direction_y:
        mulsd     xmm2, xmm6        ;; dir_x * sin(rotation_speed)
        movsd     xmm1, [rel game_player + wolfasm_player.dir_y]
        mulsd     xmm1, xmm5        ;; dir_y * cos(rotation_speed)
        addsd     xmm2, xmm1
        movsd     [rel game_player + wolfasm_player.dir_y], xmm2
        movsd     xmm0, xmm2

      ;; At this point xmm6 -> sin(rotation_speed)
      ;;               xmm5 -> cos(rotation_speed)
.update_camera_plane:
.update_camera_plane_x:
        movsd     xmm0, [rel game_player + wolfasm_player.plane_x]
        mulsd     xmm0, xmm5
        movsd     xmm1, [rel game_player + wolfasm_player.plane_y]
        mulsd     xmm1, xmm6
        subsd     xmm0, xmm1
        ;; Backup plane_x in xmm2
        movsd     xmm2, [rel game_player + wolfasm_player.plane_x]
        movsd     [rel game_player + wolfasm_player.plane_x], xmm0

.update_camera_plane_y:
        mulsd     xmm2, xmm6
        movsd     xmm0, [rel game_player + wolfasm_player.plane_y]
        mulsd     xmm0, xmm5
        addsd     xmm2, xmm0
        movsd     [rel game_player + wolfasm_player.plane_y], xmm2

        mov       rsp, rbp
        pop       rbp
        emms
        ret

        section .data
game_player:
istruc wolfasm_player
        at wolfasm_player.pos_x,          dq PLAYER_DEFAULT_POS_X
        at wolfasm_player.pos_y,          dq PLAYER_DEFAULT_POS_Y
        at wolfasm_player.dir_x,          dq PLAYER_DEFAULT_DIR_X
        at wolfasm_player.dir_y,          dq PLAYER_DEFAULT_DIR_Y
        at wolfasm_player.plane_x,        dq PLAYER_DEFAULT_PLA_X
        at wolfasm_player.plane_y,        dq PLAYER_DEFAULT_PLA_Y
        at wolfasm_player.movement_speed, dq PLAYER_DEFAULT_MOV_SPEED
        at wolfasm_player.rotation_speed, dq PLAYER_DEFAULT_ROT_SPEED
        at wolfasm_player.weapon,         dq 0x00 ;; TODO
        at wolfasm_player.life,           dd PLAYER_MAX_LIFE
iend
