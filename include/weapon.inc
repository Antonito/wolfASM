struc wolfasm_weapon_s
  .sprite          resq 1  ;; pointer to wolfasm_sprite
  .sound           resd 1  ;; sound id
  .type            resd 1  ;; weapon id
  .size:
endstruc

%define WOLFASM_PISTOL      0
%define WOLFASM_SHOTGUN     1
%define WOLFASM_BARREL      2
