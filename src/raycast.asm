        [bits 64]

        %include "window.inc"
        %include "player.inc"

        section .text
        global wolfasm_raycast, wolfasm_z_buffer

        ;; wolfasm symbols
        extern game_player, map, map_width, wolfasm_put_pixel,  \
        window_width, window_height, wolfasm_texture

        ;; C function TODO: rm
        extern wolfasm_raycast_pix_crwapper, _get_color

        ;; LibC functions
        extern _sqrt, _floor

;; Loop through the whole the screen and compute each pixel's ray
wolfasm_raycast:
        push      rbp
        mov       rbp, rsp

        xor       rdi, rdi
.loop_x:
        cmp       rdi, [rel window_width]
        je        .loop_x_end

.initialization:
        ;; Initialize camera_x
        mov       rax, rdi
        shl       rax, 1      ;; Multiply by 2
        cvtsi2sd  xmm0, rax
        mov       rax, [rel window_width]
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
        shl       rcx, 4            ;; WOLFASM_MAP_CASE_SIZE
        mov       byte cl, [rax + rcx]
        cmp       byte cl, 0
        ;; We hit a hall, stop iterating
        jne       .compute_wall_distance

        ;; Keep iterating
        jmp       .hit_loop

        ;; Calculate distance projected on camera direction
.compute_wall_distance:
        cmp       dl, 0
        mov       [rel side], edx
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
        movsd     [rel wall_dist], xmm1
        mov       rax, [rel window_height]
        cvtsi2sd  xmm0, rax
        divsd     xmm0, xmm1
        ;; Convert result to integer
        cvttsd2si esi, xmm0  ;; line_height is esi
        mov       [rel line_height], esi

.set_zbuffer:
        lea       r8, [rel wolfasm_z_buffer]
        movsd     [r8 + rdi * 8], xmm1

        ;; Compute line's limits
.compute_limits:
.compute_limits_start:
        xor       r8d, r8d
        mov       edx, [rel window_height]
        shr       edx, 1      ;; window_height / 2
        mov       ecx, esi
        shr       esi, 1      ;; line_height / 2
        mov       ecx, esi
        neg       ecx         ;; -line_height / 2
        add       ecx, edx
        cmp       ecx, 0
        cmovl     ecx, r8d

.compute_limits_end:
        mov       r8d, [rel window_height]
        dec       r8d       ;; window_height - 1
        mov       edx, [rel window_height]
        shr       edx, 1    ;; window_height / 2
        add       esi, edx
        cmp       esi, [rel window_height]
        cmovge    esi, r8d

        ;; Determine texture to print
.compute_texture_coordinate:
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
        shl       rcx, 4            ;; WOLFASM_MAP_CASE_SIZE
        mov       byte dl, [rax + rcx]
        dec       dl      ;; Decrement so texture[0] can be used
        mov       dword [rel tex_num], 0
        mov       byte [rel tex_num], dl

        movsd     xmm0, [rel wall_dist]
        cmp       byte [rel side], 1
        je        .compute_texture_side_1
.compute_texture_side_0:
        movsd     xmm1, [rel ray_pos_y]
        movsd     xmm2, [rel ray_dir_y]
        mulsd     xmm0, xmm2  ;; perp_wall_dist * ray_dir_y
        addsd     xmm1, xmm0  ;; ray_pos_y + perp_wall_dist * ray_dir_y
        jmp       .compute_texture_wall_x_floor

.compute_texture_side_1:
        movsd     xmm1, [rel ray_pos_x]
        movsd     xmm2, [rel ray_dir_x]
        mulsd     xmm0, xmm2  ;; perp_wall_dist * ray_dir_x
        addsd     xmm1, xmm0  ;; ray_pos_x + perp_wall_dist * ray_dir_x

.compute_texture_wall_x_floor:
        movsd     xmm0, xmm1
        call      _floor
        subsd     xmm1, xmm0   ;; xmm1 -= floor(xmm1)
        movsd     [rel wall_hit_x], xmm1

.compute_texture_x_coord:
        mov       rax, 64      ;; texWidth
        cvtsi2sd  xmm0, rax
        mulsd     xmm0, xmm1   ;; xmm0 = (xmm1 * 64.0)
        cvttsd2si rax, xmm0    ;; cast to integer

        cmp       byte [rel side], 1
        je        .compute_texture_x_coord_1

.compute_texture_x_coord_0:
        movsd     xmm0, [rel ray_dir_x]
        pxor      xmm1, xmm1
        ucomisd   xmm0, xmm1
        jbe       .compute_texture_x_coord_1

        mov       rsi, 64
        sub       rsi, rax
        dec       rsi
        mov       rax, rsi
        jmp       .draw_scene_pre

.compute_texture_x_coord_1:
        movsd     xmm0, [rel ray_dir_y]
        pxor      xmm1, xmm1
        ucomisd   xmm0, xmm1
        jge       .draw_scene_pre

        mov       rsi, 64
        sub       rsi, rax
        dec       rsi
        mov       rax, rsi
        jmp       .draw_scene_pre

        ;; TODO: Optimize
        ;; Draw line to the screen
.draw_scene_pre:
        pop       rsi      ;; Restore draw_end
        pop       rcx;     ;; Restore draw_start
        mov       [rel tex_x], eax
        mov       [rel draw_start], ecx
        mov       [rel draw_end], esi

        ;; Swap draw_start and draw_end for conveniance
        ;; (simpler to call wolfasm_put_pixel)
        mov       rdx, rsi
        mov       rsi, rcx
        mov       rcx, rdx

        ;; Same that comment above
        mov       edx, eax

.draw_scene:
        cmp       rsi, rcx
        jg        .draw_floor

        mov       eax, esi
        shl       eax, 8    ;; y * 256
        mov       r8d, [rel window_height]
        shl       r8d, 7     ;; window_height * 128
        mov       r9d, [rel line_height]
        shl       r9d, 7     ;; line_height * 128

        ;; y * 256 - window_height * 128 + line_height * 128
        sub       eax, r8d
        add       eax, r9d

        ;; tex_y = ((eax * texHeight) / line_height) / 256
        mov       edx, 64 ;; texHeight
        mul       edx
        mov       r10d, [rel line_height]
        div       r10d
        shr       eax, 8

        ;; tex_y * texHieght + tex_x
        mov       edx, 64
        mul       edx
        add       eax, [rel tex_x]

        ;; Get texture from array
        mov       dword r8d, [rel tex_num]
        shl       r8, 12      ;; * 4096 == * texWidth * texHeight
        lea       rdx, [rel wolfasm_texture]
        lea       rdx, [rdx + r8 * 4]
        mov       edx, [rdx + rax * 4]

        cmp       byte [rel side], 0
        shr       edx, 1
        and       edx, 0x7F7F7F
        jne       .draw_scene_print

.draw_scene_print:
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

.draw_floor:
        ;; Set direction
        mov       eax, [rel map_x]
        cvtsi2sd  xmm0, eax
        movsd     [rel floor_x], xmm0
        mov       eax, [rel map_y]
        cvtsi2sd  xmm0, eax
        movsd     [rel floor_y], xmm0

        cmp       byte [rel side], 0
        jne       .draw_floor_set_dir_1

.draw_floor_set_dir_0:
        pxor      xmm1, xmm1
        movsd     xmm0, [rel ray_dir_x]
        ucomisd   xmm1, xmm0
        je        .draw_floor_set_dir_default
        jb        .draw_floor_set_dir_0_above

.draw_floor_set_dir_0_below:
        movsd     xmm0, [rel floor_x]
        mov       rax, 1
        cvtsi2sd  xmm1, rax
        addsd     xmm0, xmm1
        movsd     [rel floor_x], xmm0

.draw_floor_set_dir_0_above:
        movsd     xmm1, [rel wall_hit_x]
        movsd     xmm0, [rel floor_y]
        addsd     xmm0, xmm1
        movsd     [rel floor_y], xmm0
        jmp       .draw_floor_set_end


.draw_floor_set_dir_1:
        pxor      xmm1, xmm1
        movsd     xmm0, [rel ray_dir_y]
        ucomisd   xmm1, xmm0
        jb        .draw_floor_set_dir_1_above

.draw_floor_set_dir_default:
        movsd     xmm0, [rel floor_y]
        mov       rax, 1
        cvtsi2sd  xmm1, rax
        addsd     xmm0, xmm1
        movsd     [rel floor_y], xmm0

.draw_floor_set_dir_1_above:
        movsd     xmm1, [rel wall_hit_x]
        movsd     xmm0, [rel floor_x]
        addsd     xmm0, xmm1
        movsd     [rel floor_x], xmm0


.draw_floor_set_end:
        push      rdi       ;; Save counter
        mov       edi, [rel draw_end]
        cmp       edi, 0
        jge       .draw_floor_prepare_loop
        mov       edi, [rel window_height]

.draw_floor_prepare_loop:
        inc        edi

.draw_floor_loop:
        cmp       edi, [rel window_height]
        je        .draw_floor_end_loop

.draw_floor_compute_distance:
        mov       eax, edi
        shl       eax, 1      ;; Multiply by 2
        cvtsi2sd  xmm0, eax
        mov       eax, [rel window_height]
        cvtsi2sd  xmm1, eax
        subsd     xmm0, xmm1
        ;; distance = window_height / (2 * y - window_height)
        divsd     xmm1, xmm0

.draw_floor_compute_weight:
        ;; weight = distance / wall_dist
        movsd     xmm0, [rel wall_dist]
        divsd     xmm1, xmm0

.draw_floor_compute_floor_pos_x:
        ;; xmm0 = weight * floor_x
        movsd     xmm0, [rel floor_x]
        mulsd     xmm0, xmm1

        ;; xmm3 = 1 - weight
        mov       rax, 1
        cvtsi2sd  xmm3, rax
        subsd     xmm3, xmm1

        ;; xmm2 = (1 - weight) * pos_x
        movsd     xmm2, [rel ray_pos_x]
        mulsd     xmm2, xmm3

        ;; xmm0 = weight * floor_x + (1 - weight) * pos_x
        addsd     xmm0, xmm2

.draw_floor_compute_floor_pos_y:
        ;; xmm2 = weight * floor_y
        movsd     xmm2, [rel floor_y]
        mulsd     xmm2, xmm1

        ;; xmm4 = (1 - weight) * pos_y
        movsd     xmm4, [rel ray_pos_y]
        mulsd     xmm4, xmm3

        ;; xmm2 = weight * floor_y + (1 - weight) * pos_y
        addsd     xmm2, xmm4

        mov       rax, 64
        cvtsi2sd  xmm3, rax
.draw_floor_compute_floor_texture_x:
        mulsd     xmm0, xmm3
        cvttsd2si rdx, xmm0
        and       rdx, 63         ;; % texture_width

.draw_floor_compute_floor_texture_y:
        mulsd     xmm2, xmm3
        cvttsd2si rax, xmm2
        and       rax, 63         ;; % texture_height

.draw_floor_compute_floor_ndx:
        shl       rax, 6          ;; * texture_width
        add       edx, eax

.draw_floor_get_texture:
        ;; Save current loop counter
        push      rdi

        ;; Save position
        push      rdx

        ;; Get texture's pixel
        mov       dword r8d, 8
        shl       r8, 12      ;; * 4096 == * texWidth * texHeight
        lea       rax, [rel wolfasm_texture]
        lea       rax, [rax + r8 * 4]
        mov       edx, [rax + rdx * 4]

        ;; Make it darker
        shr       edx, 1
        and       edx, 0x7F7F7F

        mov       rsi, rdi
        mov       rdi, [rsp + 16] ;; Get old x counter

        ;; Save parameters for later call
        push      rsi
        call      wolfasm_put_pixel
        pop       rsi

.draw_floor_sky:
        ;; Restore position
        pop       rdx

        ;; Get texture's pixel
        mov       dword r8d, 3
        shl       r8, 12      ;; * 4096 == * texWidth * texHeight
        lea       rax, [rel wolfasm_texture]
        lea       rax, [rax + r8 * 4]
        mov       edx, [rax + rdx * 4]

        ;; Make it darker
        shr       edx, 1
        and       edx, 0x7F7F7F

        ;; Print
        mov       r8d, [rel window_height]
        sub       r8, rsi
        mov       rsi, r8
        mov       rdi, [rsp + 8]

        call      wolfasm_put_pixel

        ;; Restore current loop counter
        pop       rdi

        inc       rdi
        jmp       .draw_floor_loop
.draw_floor_end_loop:
        pop       rdi       ;; Restore counter

        inc       rdi
        jmp       .loop_x
.loop_x_end:
        mov       rsp, rbp
        pop       rbp
        emms
        ret

        section .rodata
tex_width_d:    do    64.0
tex_height_d:   do    64.0

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
wall_dist:      reso  1
wall_hit_x:     reso  1
floor_x:        reso  1
floor_y:        reso  1
map_x:          resd  1
map_y:          resd  1
step_x:         resd  1
step_y:         resd  1
side:           resd  1
tex_x:          resd  1
tex_num:        resd  1
draw_end:       resd  1
draw_start:     resd  1
line_height:    resd  1
wolfasm_z_buffer:       reso  WIN_WIDTH
