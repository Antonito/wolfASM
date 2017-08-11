        [bits 64]

        %include "window.inc"

        global wolfasm_display, wolfasm_put_pixel

        ;; SDL functions
        extern _SDL_LockSurface, _SDL_RenderCopy,             \
        _SDL_UnlockSurface, _SDL_RenderPresent,               \
        _SDL_LockTexture, _SDL_UnlockTexture

        ;; wolfasm symbols
        extern window_surface, window_ptr, window_renderer,   \
        window_width, window_height, window_texture

        ;; wolfasm functions
        extern wolfasm_raycast, wolfasm_display_gui, wolfasm_display_sprites

        ;; LibC functions
        extern _memcpy

        section .text
;; This functions handle the graphic part of the game
wolfasm_display:
        push  rbp
        mov   rbp, rsp

.raycasting:
        ;; Process graphics
        call  wolfasm_raycast

.pre_blit:
        ;; Pre blit to the screen
        mov     rdi, [rel window_texture]
        xor     rsi, rsi
        lea     rdx, [rel texture_arr]
        lea     rcx, [rel texture_pitch]
        call    _SDL_LockTexture

        ;; Update pixels
        mov     rdi, [rel texture_arr]
        lea     rsi, [rel window_pixels]
        mov     edx, [rel window_pixels_size]
        call    _memcpy

        mov     rdi, [rel window_texture]
        call    _SDL_UnlockTexture

        ;; Update the screen
        mov     rdi, [rel window_renderer]
        mov     rsi, [rel window_texture]
        xor     rdx, rdx
        xor     rcx, rcx
        call    _SDL_RenderCopy

.sprites_and_gui:
        call    wolfasm_display_sprites
        call    wolfasm_display_gui

.blit:
        mov     rdi, [rel window_renderer]
        call    _SDL_RenderPresent

        mov   rsp, rbp
        pop   rbp
        ret

;; void wolfasm_put_pixel(int32_t x, int32_t y, int32_t argb)
wolfasm_put_pixel:
        push  rbp
        mov   rbp, rsp

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
        lea   r8, [rel window_pixels]
        mov   rax, r9
        mov   dword [r8 + rcx], eax

        mov   rsp, rbp
        pop   rbp
        ret

        section .rodata
window_pixels_size: dd    WIN_WIDTH * WIN_HEIGHT * 4

        section .bss
texture_arr:        resq  1
texture_pitch:      resd  1
window_pixels:      resd  WIN_WIDTH * WIN_HEIGHT
