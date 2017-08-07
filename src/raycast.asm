        [bits 64]

        %include "window.inc"
        %include "player.inc"

        section .text
        global wolfasm_raycast

        ;; wolfasm symbols
        extern game_player, map, map_width, wolfasm_put_pixel

        ;; C function TODO: rm
        extern wolfasm_raycast_pix_crwapper

        ;; LibC functions
        extern _sqrt

;; Loop through the whole the screen and compute each pixel's ray
wolfasm_raycast:
        push      rbp
        mov       rbp, rsp

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
        sub       rsp, 8
        push      rdi               ;; Save counter
        call      _sqrt
        pop       rdi               ;; Restore counter
        add       rsp, 8
        movsd     [rel delta_dist_x], xmm0
        movdqu    xmm3, oword [rsp] ;; get -rotation_speed
        add       rsp, 16
        movdqu    xmm2, oword [rsp] ;; get -rotation_speed
        add       rsp, 16
        movdqu    xmm1, oword [rsp] ;; get -rotation_speed
        add       rsp, 16

        movsd     xmm0, xmm2
        divsd     xmm0, xmm1
        addsd     xmm0, xmm3
        sub       rsp, 8
        push      rdi               ;; Save counter
        call      _sqrt
        pop       rdi               ;; Restore counter
        add       rsp, 8
        movsd     [rel delta_dist_y], xmm0

.compute_steps:
        pxor      xmm0, xmm0
        movsd     xmm2, [rel delta_dist_x]
        mov       eax, [rel map_x]
        cvtsi2sd  xmm3, eax
        movsd     xmm4, [rel ray_pos_x]
.compute_step_x:
        movsd     xmm1, [rel ray_dir_x]
        ucomisd   xmm0, xmm1
        jbe       .compute_step_x_ge
        ;; xmm1 < 0
        mov       dword [rel step_x], -1
        subsd     xmm4, xmm3
        movsd     xmm1, xmm4
        jmp       .compute_step_x_end

.compute_step_x_ge:
        ;; xmm1 >= 0
        mov       dword [rel step_x], 1
        subsd     xmm3, xmm4
        mov       rax, 1
        cvtsi2sd  xmm1, rax
        addsd     xmm1, xmm3

.compute_step_x_end:
        mulsd     xmm2, xmm1
        movsd     [rel side_dist_x], xmm2

.compute_step_y:
        pxor      xmm0, xmm0
        movsd     xmm2, [rel delta_dist_y]
        mov       eax, [rel map_y]
        cvtsi2sd  xmm3, eax
        movsd     xmm4, [rel ray_pos_y]

        movsd     xmm1, [rel ray_dir_y]
        ucomisd   xmm0, xmm1
        jbe       .compute_step_y_ge
        ;; xmm1 < 0
        mov       dword [rel step_y], -1
        subsd     xmm4, xmm3
        movsd     xmm1, xmm4

        jmp       .compute_step_y_end
.compute_step_y_ge:
        ;; xmm1 >= 0
        mov       dword [rel step_y], 1
        subsd     xmm3, xmm4
        mov       rax, 1
        cvtsi2sd  xmm1, rax
        addsd     xmm1, xmm3

.compute_step_y_end:
        mulsd     xmm2, xmm1
        movsd     [rel side_dist_y], xmm2

        ;; Compute hit distance
        xor       rdx, rdx
.hit_loop:
        movsd     xmm0, [rel side_dist_x]
        movsd     xmm1, [rel side_dist_y]
        ucomisd   xmm1, xmm0
        jbe       .hit_loop_y

.hit_loop_x:
        ;; Increment side distance
        movsd     xmm0, [rel side_dist_x]
        addsd     xmm0, [rel delta_dist_x]
        movsd     [rel side_dist_x], xmm0

        ;; Increment ray position
        mov       eax, [rel step_x]
        add       [rel map_x], eax

        ;; dl is 'side'
        mov       dl, 0

        jmp       .hit_loop_check
.hit_loop_y:
        ;; Increment side distance
        movsd     xmm0, [rel side_dist_y]
        addsd     xmm0, [rel delta_dist_y]
        movsd     [rel side_dist_y], xmm0

        ;; Increment ray position
        mov       eax, [rel step_y]
        add       [rel map_y], eax

        ;; dl is 'side'
        mov       dl, 1

.hit_loop_check:

        ;; Check if ray hit a wall
        ;; Get index
        push      rdx
        xor       rcx, rcx
        mov       eax, [rel map_width]
        mov       esi, [rel map_y]
        mul       esi
        mov       rcx, rax
        add       cx, [rel map_x]
        pop       rdx

        ;; map[mapY * map_width + mapX] > 0
        lea       rax, [rel map]
        mov       byte cl, [rax + rcx]
        cmp       byte cl, 0
        ;; We hit a hall, stop iterating
        jne       .compute_wall_distance

        ;; Keep iterating
        jmp       .hit_loop

        ;; Calculate distance projected on camera direction
.compute_wall_distance:
        cmp       dl, 0
        push      rdx                   ;; Save side for later
        jne       .compute_wall_distance_y
.compute_wall_distance_x:
        mov       eax, [rel step_x]
        mov       dword edx,  1
        sub       edx, eax              ;; 1 - step_x
        shr       edx, 1                ;; (1 - step_x) / 2
        movsd     xmm0, [rel ray_pos_x]
        cvtsi2sd  xmm1, [rel map_x]
        subsd     xmm1, xmm0            ;; map_x - ray_pos_x
        cvtsi2sd  xmm0, edx
        addsd     xmm1, xmm0 ;; map_x - ray_pos_x + (1 - step_x) / 2
        movsd     xmm0, [rel ray_dir_x]
        divsd     xmm1, xmm0

        jmp       .compute_line_height
.compute_wall_distance_y:
        mov       eax, [rel step_y]
        mov       dword edx,  1
        sub       edx, eax              ;; 1 - step_y
        shr       edx, 1                ;; (1 - step_y) / 2
        movsd     xmm0, [rel ray_pos_y]
        cvtsi2sd  xmm1, [rel map_y]
        subsd     xmm1, xmm0            ;; map_y - ray_pos_y
        cvtsi2sd  xmm0, edx
        addsd     xmm1, xmm0 ;; map_y - ray_pos_y + (1 - step_y) / 2
        movsd     xmm0, [rel ray_dir_y]
        divsd     xmm1, xmm0

        ;; Calculate line to draw
.compute_line_height:
        mov       rax, WIN_HEIGHT
        cvtsi2sd  xmm0, rax
        divsd     xmm0, xmm1
        ;; Convert result to integer
        cvttsd2si esi, xmm0  ;; line_height is r8

        ;; Compute line's limits
.compute_limits:
.compute_limits_start:
        xor       r8d, r8d
        mov       edx, WIN_HEIGHT / 2
        mov       ecx, esi
        shr       esi, 1      ;; line_height / 2
        mov       ecx, esi
        neg       ecx         ;; -line_height / 2
        add       ecx, edx
        cmp       ecx, 0
        cmovl     ecx, r8d

.compute_limits_end:
        mov       r8d, WIN_HEIGHT - 1
        mov       edx, WIN_HEIGHT / 2
        add       esi, edx
        cmp       esi, WIN_HEIGHT
        cmovge    esi, r8d

        ;; Determine wall color
        ;; TODO: Find a cleaner / faster way to do it
.compute_color:
        ;; Get index
        push      rcx   ;; Save draw_start
        push      rsi   ;; Save draw_end
        xor       rcx, rcx
        mov       eax, [rel map_width]
        mov       esi, [rel map_y]
        mul       esi
        mov       rcx, rax
        add       cx, [rel map_x]

        ;; map[mapY * map_width + mapX]
        lea       rax, [rel map]
        mov       byte dl, [rax + rcx]

.compute_color_red:
        cmp       byte dl, 1
        jne       .compute_color_green
        mov       eax, 0xC81543 ;; Red color
        jmp       .check_shadow_color

.compute_color_green:
        cmp       byte dl, 2
        jne       .compute_color_blue
        mov       eax, 0xA8E4B1 ;; Green color
        jmp       .check_shadow_color

.compute_color_blue:
        cmp       byte dl, 3
        jne       .compute_color_white
        mov       eax, 0x4AA3BA ;; Blue color
        jmp       .check_shadow_color

.compute_color_white:
        cmp       byte dl, 4
        jne       .compute_color_default
        mov       eax, 0xFFE9EC ;; White color
        jmp       .check_shadow_color

.compute_color_default:
        mov       eax, 0xF1E694 ;; Yellow color

        ;; Simulate shadow
.check_shadow_color:
        pop       rsi      ;; Restore draw_end
        pop       rcx;     ;; Restore draw_start
        pop       rdx      ;; Get back 'side'
        cmp       byte dl, 1
        jne       .draw_scene_pre
        shr       eax, 1    ;; color / 2

        ;; TODO: Optimize
        ;; Draw line to the screen
.draw_scene_pre:
        ;; Swap draw_start and draw_end for conveniance
        ;; (simpler to call wolfasm_put_pixel)
        mov       rdx, rsi
        mov       rsi, rcx
        mov       rcx, rdx

        ;; Same that comment above
        mov       edx, eax

.draw_scene:
        cmp       rsi, rcx
        je        .draw_scene_end

        sub       rsp, 8
        push      rsi     ;; Save current_state
        sub       rsp, 8
        push      rcx     ;; Save draw_end
        sub       rsp, 8
        push      rdi     ;; Save loop counter
        sub       rsp, 8
        push      rdx     ;; Save color
        call      wolfasm_put_pixel
        pop       rdx
        add       rsp, 8
        pop       rdi
        add       rsp, 8
        pop       rcx
        add       rsp, 8
        pop       rsi
        add       rsp, 8

        inc       rsi
        jmp       .draw_scene

.draw_scene_end:

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
