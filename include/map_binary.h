#pragma once

#define WOLFASM_MAP_MAGIC 0xCAFE

typedef struct wolfasm_map_header {
  uint16_t magic;
  uint32_t width;
  uint32_t height;
  char name[255];
} __attribute__((packed)) wolfasm_map_header_t;

typedef struct wolfasm_map_items_header {
  uint32_t nb_items;
} __attribute__((packed)) wolfasm_map_items_header;
