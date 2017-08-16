#pragma once

#if defined __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpacked"
#endif

//
// Note: This file should be synced with map.inc
//

#define WOLFASM_MAP_MAGIC 0xCAFE

typedef struct wolfasm_map_header {
  uint16_t magic;
  uint32_t width;
  uint32_t height;
  char name[255];
} __attribute__((packed)) wolfasm_map_header_t;

_Static_assert(sizeof(wolfasm_map_header_t) == 2 + 4 + 4 + 255,
               "Invalid Map Header Size");

typedef struct wolfasm_map_items_header {
  uint32_t nb_items;
} __attribute__((packed)) wolfasm_map_items_header;

enum wolfasm_item_callbacks { CALLBACK_LIFE = 0, CALLBACK_AMMO };

enum wolfasm_item_sprite_table { NO_TABLE = 0, TABLE_ENEMY_ANIMATION_SHOOT };

typedef struct wolfasm_map_item {
  int32_t texture;
  int32_t pos_x;
  int32_t pos_y;
  int32_t width_div;
  int32_t height_div;
  double height_move;

  int32_t current_anim;
  int32_t nb_anim;
  int32_t anim_rate;
  enum wolfasm_item_sprite_table texture_table;

  int32_t stock;
  uint32_t type;
  enum wolfasm_item_callbacks callback;
} __attribute__((packed)) wolfasm_map_item_t;

#if defined __clang__
#pragma clang diagnostic pop
#endif
