        [bits 64]
        section .text

        %include "window.mac"

        global wolfasm_display, wolfasm_put_pixel

        ;; SDL functions
        extern _SDL_UpdateWindowSurface, _SDL_LockSurface, _SDL_UnlockSurface

        ;; wolfasm symbols
        extern window_surface, window_ptr

;; This functions handle the graphic part of the game
wolfasm_display:
        push  rbp
        mov   rbp, rsp

        ;; Process graphics
        call  wolfasm_display_clean

        ;; Display graphics
        mov   rdi, [rel window_ptr]
        call  _SDL_UpdateWindowSurface

        mov   rsp, rbp
        pop   rbp
        ret

wolfasm_display_clean:
        push  rbp
        mov   rbp, rsp

        ;; Reset y to 0
        xor   rsi, rsi
.y_loop:
        cmp   rsi, WIN_HEIGHT - 1
        je    .end_y_loop

        ;; Reset x to 0
        xor   rdi, rdi
.x_loop:
        cmp   rdi, WIN_WIDTH - 1
        je    .end_x_loop

        ;; Set color, as rdi and rsi are already set
        mov   rdx, 0x0
        push  rsi
        push  rdi
        call  wolfasm_put_pixel
        pop   rdi
        pop   rsi

        ;; Increment x counter
        inc   rdi
        jmp   .x_loop

.end_x_loop:
        ;; Increment y counter
        inc   rsi
        jmp   .y_loop

.end_y_loop:
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
        mov   eax, WIN_WIDTH
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
