        [bits 64]

        %include "window.inc"
        %include "player.inc"

        section .text
        global wolfasm_raycast

        ;; wolfasm symbols
        extern game_player

        ;; C function TODO: rm
        extern wolfasm_raycast_pix_crwapper

        ;; LibC functions
        extern _sqrt

;; Loop through the whole the screen and compute each pixel's ray
wolfasm_raycast:
        push      rbp
        mov       rbp, rsp

        call      wolfasm_raycast_pix_crwapper
        mov       rsp, rbp
        pop       rbp
        ret

        xor       rdi, rdi
.loop_x:
        cmp       rdi, WIN_WIDTH
        je        .loop_x_end

.initialization:
        ;; Initialize camera_x
        mov       rax, rdi
        shl       rax, 1      ;; Multiply by 2
        cvtsi2sd  xmm0, rax
        mov       rax, WIN_WIDTH
        cvtsi2sd  xmm1, rax
        divsd     xmm0, xmm1
        mov       rax, 1
        cvtsi2sd  xmm1, rax
        subsd     xmm0, xmm1
        movsd     [rel camera_x], xmm0


        ;; Initialize ray pos
        movsd     xmm0, [rel game_player + wolfasm_player.pos_x]
        movsd     [rel ray_pos_x], xmm0
        movsd     xmm0, [rel game_player + wolfasm_player.pos_y]
        movsd     [rel ray_pos_y], xmm0

        ;; Initialize ray dir
        movsd     xmm0, [rel game_player + wolfasm_player.plane_x]
        mulsd     xmm0, [rel camera_x]
        addsd     xmm0, [rel game_player + wolfasm_player.dir_x]
        movsd     [rel ray_dir_x], xmm0
        movsd     xmm0, [rel game_player + wolfasm_player.plane_y]
        mulsd     xmm0, [rel camera_x]
        addsd     xmm0, [rel game_player + wolfasm_player.dir_y]
        movsd     [rel ray_dir_y], xmm0

        ;; Get map indexes
        cvttsd2si rax, [rel ray_pos_x];
        mov       [rel map_x], rax
        cvttsd2si rax, [rel ray_pos_y];
        mov       [rel map_y], rax

        ;; Calculate deltas
        movsd     xmm1, [rel ray_dir_y]
        movsd     xmm2, [rel ray_dir_x]
        mulsd     xmm1, xmm1  ;; ray_dir_y * ray_dir_y
        mulsd     xmm2, xmm2  ;; ray_dir_x * ray_dir_x
        mov       rax, 1
        cvtsi2sd  xmm3, rax

        movsd     xmm0, xmm1
        divsd     xmm0, xmm2
        sub       rsp, 16
        movdqu    oword [rsp], xmm1 ;; save ray_dir_y * ray_dir_y
        sub       rsp, 16
        movdqu    oword [rsp], xmm2 ;; save ray_dir_x * ray_dir_x
        sub       rsp, 16
        movdqu    oword [rsp], xmm3 ;; save 1
        addsd     xmm0, xmm3
        call      _sqrt
        movsd     [rel side_dist_x], xmm0
        movdqu    xmm3, oword [rsp] ;; get -rotation_speed
        add       rsp, 16
        movdqu    xmm2, oword [rsp] ;; get -rotation_speed
        add       rsp, 16
        movdqu    xmm1, oword [rsp] ;; get -rotation_speed
        add       rsp, 16

        movsd     xmm0, xmm2
        divsd     xmm0, xmm1
        addsd     xmm0, xmm3
        call      _sqrt
        movsd     [rel side_dist_y], xmm0

.compute_steps:
        xorpd     xmm0, xmm0
        movsd     xmm2, [rel delta_dist_x]
        mov       rax, [rel map_x]
        cvtsi2sd  xmm3, rax
        movsd     xmm4, [rel ray_pos_x]
.compute_step_x:
        movsd     xmm1, [rel ray_dir_x]
        cmpsd     xmm0, xmm1, 2 ;; Lower or equal
        ;; TODO: conditional jump ???
        ;; xmm1 < 0
        mov       dword [rel step_x], -1
        subsd     xmm4, xmm3
        movsd     xmm1, xmm4

        jmp       .compute_step_x_end
.compute_step_x_le:
        ;; xmm1 >= 0
        mov       dword [rel step_x], 1
        subsd     xmm3, xmm4
        mov       rax, 1
        cvtsi2sd  xmm1, rax
        addsd     xmm1, xmm3

.compute_step_x_end:
        mulsd     xmm0, xmm1
        movsd     [rel side_dist_x], xmm0

.compute_step_y:
        xorpd     xmm0, xmm0
        movsd     xmm2, [rel delta_dist_y]
        mov       rax, [rel map_y]
        cvtsi2sd  xmm3, rax
        movsd     xmm4, [rel ray_pos_y]

        movsd     xmm1, [rel ray_dir_y]
        cmpsd     xmm0, xmm1, 2 ;; Lower or equal
        ;; TODO: conditional jump ???
        ;; xmm1 < 0
        mov       dword [rel step_y], -1
        subsd     xmm4, xmm3
        movsd     xmm1, xmm4

        jmp       .compute_step_x_end
.compute_step_y_le:
        ;; xmm1 >= 0
        mov       dword [rel step_y], 1
        subsd     xmm3, xmm4
        mov       rax, 1
        cvtsi2sd  xmm1, rax
        addsd     xmm1, xmm3

.compute_step_y_end:
        mulsd     xmm0, xmm1
        movsd     [rel side_dist_y], xmm0

.hit_loop:
.compute_wall_distance:
.compute_limits:
.compute_color:
.draw_scene:

        inc       rdi
        jmp       .loop_x
.loop_x_end:
        mov       rsp, rbp
        pop       rbp
        ret

        section .bss
camera_x:       reso  1
ray_pos_x:      reso  1
ray_pos_y:      reso  1
ray_dir_x:      reso  1
ray_dir_y:      reso  1
side_dist_x:    reso  1
side_dist_y:    reso  1
delta_dist_x:   reso  1
delta_dist_y:   reso  1
map_x:          resd  1
map_y:          resd  1
step_x:         resd  1
step_y:         resd  1
