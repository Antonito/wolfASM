        [bits 64]

        %include "window.inc"

        section .text
        global wolfasm_raycast

        ;; C function TODO: rm
        extern wolfasm_raycast_pix_crwapper

;; Loop through the whole the screen and compute each pixel's ray
wolfasm_raycast:
        push  rbp
        mov   rbp, rsp

        call  wolfasm_raycast_pix_crwapper
        mov   rsp, rbp
        pop   rbp
        ret

        xor   rdi, rdi
.loop_y:
        cmp   rdi, WIN_HEIGHT
        je    .loop_y_end

        xor   rsi, rsi
.loop_x:
        cmp   rsi, WIN_WIDTH
        je    .loop_x_end

        push  rdi
        push  rsi
        call  wolfasm_raycast_pix
        pop   rsi
        pop   rdi

        inc   rsi
        jmp   .loop_x

.loop_x_end:
        inc   rdi
        jmp   .loop_y

.loop_y_end:
        mov   rsp, rbp
        pop   rbp
        ret

;; void wolfasm_raycast_pix(int x, int y)
wolfasm_raycast_pix:
        push  rbp
        mov   rbp, rsp

        mov   rsp, rbp
        pop   rbp
        ret
