struc wolfasm_sprite_s
  .texture         resq 1  ;; pointer to SDL_Texture
  .sprite          resq 1  ;; pointer to SDL_Rect
  .width           resd 1
  .height          resd 1
  .nb_sprites      resw 1  ;; number of animation frame
  .current_anim    resw 1  ;; current frame of animation
  .trigger         resd 1  ;; shall animate if == 1
  .size:
endstruc

%define SPRITE_PISTOL       0
%define SPRITE_SHOTGUN      1
%define SPRITE_BARREL       2
%define NB_WOLFASM_SPRITES  3

%define SPRITE_WEAPON_WIDTH       62 * 4
%define SPRITE_WEAPON_HEIGHT      SPRITE_WEAPON_WIDTH
%define SPRITE_PISTOL_NB_ANIM     5
%define SPRITE_SHOTGUN_NB_ANIM    4
%define SPRITE_BARREL_NB_ANIM     4
