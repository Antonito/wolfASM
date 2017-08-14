struc wolfasm_map_case
  .value        resd  1  ;; uint32_t
  .padding      resd  1  ;; padding to 16 bytes
  .item         resq  1  ;; pointer
  .enemy        resq  1  ;; pointer
  .padding_ptr  resq  1  ;; pointer
size:
endstruc

%define WOLFASM_MAP_CASE_SIZE  16
