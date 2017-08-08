        [bits 64]

        global wolfasm_display_sprites

        ;; TODO: rm
        extern _display_sprites_cwrapper

        section .text

wolfasm_display_sprites:
        push      rbp
        mov       rbp, rsp

        call      _display_sprites_cwrapper

        mov       rsp, rbp
        pop       rbp
        ret
