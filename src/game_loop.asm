        [bits 64]
        section .text
        global game_loop

game_loop:
        push  rbp
        mov   byte [rel game_running], 1
.loop:
        cmp   byte [rel game_running], 0
        je    .end_loop

        ;; Go back to loop
        jmp   .loop
.end_loop:
        pop   rbp
        ret

        section .data
game_running db 1
