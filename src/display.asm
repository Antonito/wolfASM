        [bits 64]

        %include "window.inc"

        global wolfasm_display, wolfasm_put_pixel

        ;; SDL functions
        extern _SDL_UpdateWindowSurface, _SDL_LockSurface,   \
        _SDL_UnlockSurface, _SDL_RenderPresent

        ;; wolfasm symbols
        extern window_surface, window_ptr, window_renderer,   \
        window_width, window_height

        ;; wolfasm functions
        extern wolfasm_raycast, wolfasm_display_gui

        section .text
;; This functions handle the graphic part of the game
wolfasm_display:
        push  rbp
        mov   rbp, rsp

        ;; Process graphics
        call  wolfasm_set_sky
        call  wolfasm_set_ground
        call  wolfasm_raycast

        call  wolfasm_display_gui

        ;; Display graphics
        mov   rdi, [rel window_ptr]
        call  _SDL_UpdateWindowSurface

        mov   rsp, rbp
        pop   rbp
        ret

;; void wolfasm_put_pixel(int32_t x, int32_t y, int32_t argb)
wolfasm_put_pixel:
        push  rbp
        mov   rbp, rsp

        ;; Save arguments
        push  rdi
        push  rsi
        push  rdx
        mov   rdi, [rel window_surface]
        sub   rsp, 8
        call  _SDL_LockSurface
        add   rsp, 8
        pop   rdx
        pop   rsi
        pop   rdi

        ;; Save color
        mov   r9, rdx

        ;; Offset in array: (y * WIN_WIDTH + x) * 4
        ;; Compute offset in rcx : rcx = (rsi * WIN_WIDTH + rdi)
        mov   eax, [rel window_width]
        mul   esi       ;; y * WIN_WIDTH
        add   eax, edi
        shl   eax, 2    ;; multiply by 4
        mov   ecx, eax

        ;; Store pixel
        mov   r8, [rel window_surface]
        mov   r8, [r8 + 32]
        mov   rax, r9
        mov   dword [r8 + rcx], eax

        mov   rdi, [rel window_surface]
        call  _SDL_UnlockSurface

        mov   rsp, rbp
        pop   rbp
        ret

;; Draw the sky
wolfasm_set_sky:
        push  rbp
        mov   rbp, rsp

        xor   rsi, rsi
        mov   r8, [rel window_height]
        shr   r8, 1 ;; window_height / 2
.loop_y:
        cmp    rsi, r8
        je    .end_loop_y

        xor   rdi, rdi

.loop_x:
        cmp   rdi, [rel window_width]
        je    .end_loop_x

        push  rdi
        push  rsi
        push  r8
        sub   rsp, 8
        mov   edx, SKY_COLOR
        call  wolfasm_put_pixel
        add   rsp, 8
        pop   r8
        pop   rsi
        pop   rdi

        inc   rdi
        jmp   .loop_x
.end_loop_x:

        inc   rsi
        jmp   .loop_y

.end_loop_y:
        mov   rsp, rbp
        pop   rbp
        ret

;; Draw the ground
wolfasm_set_ground:
        push  rbp
        mov   rbp, rsp

        xor   rsi, rsi
        mov   rsi, [rel window_height]
        shr   rsi, 1    ;; window_height / 2
.loop_y:
        cmp    rsi, [rel window_height]
        je    .end_loop_y

        xor   rdi, rdi

.loop_x:
        cmp   rdi, [rel window_width]
        je    .end_loop_x

        push  rdi
        push  rsi
        mov   edx, GROUND_COLOR
        call  wolfasm_put_pixel
        pop   rsi
        pop   rdi

        inc   rdi
        jmp   .loop_x
.end_loop_x:

        inc   rsi
        jmp   .loop_y

.end_loop_y:
        mov   rsp, rbp
        pop   rbp
        ret
