        [bits 64]
        section .text
        global wolfasm_logic

wolfasm_logic:
        push  rbp
        mov   rbp, rsp

        ;; TODO: Implement game logic here

        mov   rsp, rbp
        pop   rbp
        ret
