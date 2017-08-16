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
