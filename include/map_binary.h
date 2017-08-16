#pragma once

#if defined __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpacked"
#endif

//
// Note: This file should be synced with map.inc
//

//
// Architecture of the file
//
//                 <------ Start
// ----------------------------
// | wolfasm_map_header       |
// ----------------------------
// | wolfasm_map_case * width |
// | * height                 |
// ----------------------------
// | wolfasm_map_items_header |
// ----------------------------
// | wolfasm_map_item_t       |
// | * nb_items               |
// ----------------------------
//                  <------ End
//

// Magic number
#define WOLFASM_MAP_MAGIC 0xCAFE

// Header of the map file
typedef struct wolfasm_map_header {
  uint16_t magic;  // File's magic number
  uint32_t width;  // Map's width
  uint32_t height; // Map's height
  char name[255];  // Map's name
} __attribute__((packed)) wolfasm_map_header_t;

_Static_assert(sizeof(wolfasm_map_header_t) == 2 + 4 + 4 + 255,
               "Invalid Map Header Size");

// Header of the items
typedef struct wolfasm_map_items_header {
  uint32_t nb_items;
} __attribute__((packed)) wolfasm_map_items_header;

// Correspondance table for the callbacks
// as we cannot embbed function's pointers in files
enum wolfasm_item_callbacks { CALLBACK_LIFE = 0, CALLBACK_AMMO };

// Correspondance table for the animations
// as we cannot embbed array's pointer in files
enum wolfasm_item_sprite_table { NO_TABLE = 0, TABLE_ENEMY_ANIMATION_SHOOT };

//
// Slightly modified version of struct wolfasm_item
// `texture_table` and `callback member's` changed
// from pointers to integers
//
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

_Static_assert(sizeof(wolfasm_map_item_t) == 4 * 12 + 8,
               "Invalid Map Item Size");

#if defined __clang__
#pragma clang diagnostic pop
#endif
