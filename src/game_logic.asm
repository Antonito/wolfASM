        [bits 64]
        section .text
        global wolfasm_logic

        ;; TODO: rm
        extern _game_logic_cwrapper

wolfasm_logic:
        push  rbp
        mov   rbp, rsp

        ;; TODO: Implement game logic here
        call  _game_logic_cwrapper

        mov   rsp, rbp
        pop   rbp
        ret
