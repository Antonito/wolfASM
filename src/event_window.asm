        [bits 64]

        global wolfasm_event_window

        ;; C Wrapper
        extern wolfasm_event_window_cwrapper

        section .text
wolfasm_event_window:
        push      rbp
        mov       rbp, rsp
        call      wolfasm_event_window_cwrapper
        mov       rsp, rbp
        pop       rbp
        ret
