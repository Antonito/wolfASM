struc wolfasm_map_case
  .value        resd  1  ;; uint32_t
  .padding      resd  1  ;; padding to 16 bytes
  .item         resq  1  ;; pointer
  .enemy        resq  1  ;; pointer
  .padding_ptr  resq  1  ;; pointer
size:
endstruc

%define WOLFASM_MAP_CASE_SIZE  32

;;
;; Map loader
;;
;; Note: This file should be synced with map_binary.h
;;
%define WOLFASM_MAP_MAGIC   0xCAFE

struc wolfasm_map_header_t
  .magic        resw  1
  .width        resd  1
  .height       resd  1
  .name         resb  255
endstruc

%define WOLFASM_MAP_HEADER_SIZE (2 + 4 + 4 + 255)

struc wolfasm_map_items_header_t
  .nb_items     resd  1
endstruc
%define WOLFASM_MAP_ITEM_HEADER_SIZE 4

struc wolfasm_map_item_t
  .texture                resd    1
  .pos_x                  resd    1
  .pos_y                  resd    1
  .width_div              resd    1
  .height_div             resd    1
  .height_move            resq    1
  .current_anim           resd    1
  .nb_anim                resd    1
  .anim_rate              resd    1
  .texture_table          resd    1
  .stock                  resd    1
  .type                   resd    1
  .callback               resd    1
endstruc
%define WOLFASM_MAP_ITEM_SIZE        4 * 12 + 8

;; Item animation table
%define NO_TABLE                      0
%define TABLE_ENEMY_ANIMATION_SHOOT   1

;; Item callbacks table
%define CALLBACK_LIFE                 0
%define CALLBACK_AMMO                 1
