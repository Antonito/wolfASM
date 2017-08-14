struc wolfasm_weapon_s
  .sprite          resq 1  ;; pointer to wolfasm_sprite
  .sound           resd 1  ;; sound id
  .type            resd 1  ;; weapon id
  .ammo            resw 1  ;; current ammount of ammunitions
  .max_ammo        resw 1  ;; maximum ammount of ammunitions you can have
  .damage          resd 1   ;; Damage the weapon deals
  .size:
endstruc

%define WOLFASM_PISTOL      0
%define WOLFASM_SHOTGUN     1
%define WOLFASM_BARREL      2

%define WOLFASM_PISTOL_MAX_AMMO   32
%define WOLFASM_SHOTGUN_MAX_AMMO  12
%define WOLFASM_BARREL_MAX_AMMO   2

%define WOLFASM_PISTOL_DMG        10
%define WOLFASM_SHOTGUN_DMG       25
%define WOLFASM_BARREL_DMG        70
