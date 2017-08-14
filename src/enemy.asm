        [bits 64]

        global enemy_animation_shoot, enemy_animation_shoot_nb

        ;; TODO: Make it local
        global enemy_animation_die, enemy_animation_hit

        section .text

        section .rodata
enemy_animation_hit:      dd    11, 19, 19, 19
enemy_animation_die:      dd    14, 15, 16, 17, 18, 18, 18
enemy_animation_shoot:    dd    11, 12, 13
