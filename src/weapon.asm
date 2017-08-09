        [bits 64]

        %include "audio.inc"
        %include "player.inc"
        %include "weapon.inc"
        %include "sprite.inc"

        global wolfasm_init_weapon, wolfasm_change_weapon, wolfasm_weapon

        extern game_player, wolfasm_sprite

        section .text

wolfasm_init_weapon:
        push      rbp
        mov       rbp, rsp

        ;; Initialize weapon's data
.pistol:
        mov       edi, SPRITE_PISTOL
        mov       esi, WOLFASM_PISTOL
        call      wolfasm_init_single_weapon

.shotgun:
        mov       edi, SPRITE_SHOTGUN
        mov       esi, WOLFASM_SHOTGUN
        call      wolfasm_init_single_weapon

.barrel:
        mov       edi, SPRITE_BARREL
        mov       esi, WOLFASM_BARREL
        call      wolfasm_init_single_weapon

.player:
        ;; Initialize player's data
        mov       edi, WOLFASM_PISTOL
        call      wolfasm_change_weapon

.end:
        mov       rsp, rbp
        pop       rbp
        ret

wolfasm_init_single_weapon:
        push      rbp
        mov       rbp, rsp

        mov       r9, rdi   ;; sprite
        mov       r10, rsi  ;; weapon

        ;; Get sprite addr
        mov       edi, r9d
        mov       eax, wolfasm_sprite_s.size
        mul       edi
        lea       r8, [rel wolfasm_sprite]
        add       r8, rax

        ;; Get weapon's sprite
        mov       edi, r10d
        mov       eax, wolfasm_weapon_s.size
        mul       edi

        ;; Set sprite in weapon's memory
        lea       rdi, [rel wolfasm_weapon]
        add       rdi, rax
        mov       [rdi + wolfasm_weapon_s.sprite], r8

        mov       rsp, rbp
        pop       rbp
        ret

;; void wolfasm_change_weapon(int weapon)
wolfasm_change_weapon:
        push      rbp
        mov       rbp, rsp

        ;; Multiply edi by wolfasm_weapon_s.size
        mov       eax, wolfasm_weapon_s.size
        mul       edi

        ;; Update weapon slot
        lea       rdi, [rel wolfasm_weapon]
        add       rdi, rax
        mov       [rel game_player + wolfasm_player.weapon], rdi

        ;; Reinitialize animation status to 0
.reinit_frame:
        mov       rax, [rel game_player + wolfasm_player.weapon]
        mov       rax, [rax + wolfasm_weapon_s.sprite]
        mov       word [rax + wolfasm_sprite_s.current_anim], 0
        mov       dword [rax + wolfasm_sprite_s.trigger], 0

        mov       rsp, rbp
        pop       rbp
        ret

        section .data

wolfasm_weapon:
istruc wolfasm_weapon_s
      at wolfasm_weapon_s.sprite,   dq    0
      at wolfasm_weapon_s.sound,    dd    SOUND_PISTOL
      at wolfasm_weapon_s.type,     dd    WOLFASM_PISTOL
      at wolfasm_weapon_s.ammo,     dw    WOLFASM_PISTOL_MAX_AMMO / 2
      at wolfasm_weapon_s.max_ammo, dw    WOLFASM_PISTOL_MAX_AMMO
iend
istruc wolfasm_weapon_s
      at wolfasm_weapon_s.sprite,   dq    0
      at wolfasm_weapon_s.sound,    dd    SOUND_SHOTGUN
      at wolfasm_weapon_s.type,     dd    WOLFASM_SHOTGUN
      at wolfasm_weapon_s.ammo,     dw    WOLFASM_SHOTGUN_MAX_AMMO / 2
      at wolfasm_weapon_s.max_ammo, dw    WOLFASM_SHOTGUN_MAX_AMMO
iend
istruc wolfasm_weapon_s
      at wolfasm_weapon_s.sprite,   dq    0
      at wolfasm_weapon_s.sound,    dd    SOUND_BARREL
      at wolfasm_weapon_s.type,     dd    WOLFASM_BARREL
      at wolfasm_weapon_s.ammo,     dw    WOLFASM_BARREL_MAX_AMMO / 2
      at wolfasm_weapon_s.max_ammo, dw    WOLFASM_BARREL_MAX_AMMO
iend
