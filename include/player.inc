;; Game player's informations
struc wolfasm_player
  .pos_x          resq 1  ;; Double
  .pos_y          resq 1  ;; Double
  .dir_x          resq 1  ;; Double
  .dir_y          resq 1  ;; Double
  .plane_x        resq 1  ;; Double
  .plane_y        resq 1  ;; Double
  .movement_speed resq 1  ;; Double
  .rotation_speed resq 1  ;; Double
  .weapon         resq 1  ;; Weapon pointer
  .life           resd 1  ;; Life
  .size:
endstruc

%define PLAYER_DEFAULT_POS_X      4.0
%define PLAYER_DEFAULT_POS_Y      3.0
%define PLAYER_DEFAULT_DIR_X      -1.0
%define PLAYER_DEFAULT_DIR_Y      0.00
%define PLAYER_DEFAULT_PLA_X      0.00
%define PLAYER_DEFAULT_PLA_Y      0.66
%define PLAYER_DEFAULT_MOV_SPEED  0.30
%define PLAYER_DEFAULT_ROT_SPEED  0.50

%define PLAYER_MODIFIER_MOV_SPEED 5
%define PLAYER_MODIFIER_ROT_SPEED 12

%define PLAYER_MAX_LIFE           100
