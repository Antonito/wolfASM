struc wolfasm_item_s
    .texture        resd    1
    .pos_x          resd    1
    .pos_y          resd    1
    .width_div      resd    1
    .height_div     resd    1
    .height_move    resq    1
    .current_anim   resd    1
    .nb_anim        resd    1
    .anim_rate      resd    1
    .texture_table  resq    1
    .stock          resd    1
    .type           resd    1
    .callback       resq    1
endstruc

%define WOLFASM_MAP_ITEM  (4 * 10 + 8 * 3)

%define ITEM_AMMO       0
%define ITEM_MEDIKIT    1
%define ITEM_ENEMY      2
%define NB_ITEMS        3
