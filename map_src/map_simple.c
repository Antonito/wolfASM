#include "cdefs.h"
#include "map_binary.h"

extern wolfasm_map_case_t wolfasm_map[];
extern uint32_t const wolfasm_map_width;
extern uint32_t const wolfasm_map_height;
extern char const wolfasm_map_name[255];
extern wolfasm_map_item_t wolfasm_items[];
extern uint32_t const wolfasm_items_nb;

#define MAP_WIDTH (8u)
#define MAP_HEIGHT (7u)

// clang-format off
wolfasm_map_case_t  wolfasm_map[] = {
    {1, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {1, NULL, NULL, NULL},
    {1, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {1, NULL, NULL, NULL},
    {1, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {1, NULL, NULL, NULL},
    {1, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {1, NULL, NULL, NULL},
    {1, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {1, NULL, NULL, NULL},
    {1, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {1, NULL, NULL, NULL},
    {1, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {1, NULL, NULL, NULL}};
// clang-format on

uint32_t const wolfasm_map_width = MAP_WIDTH;
uint32_t const wolfasm_map_height = MAP_HEIGHT;

_Static_assert(sizeof(wolfasm_map) ==
                   MAP_WIDTH * MAP_HEIGHT * sizeof(wolfasm_map[0]),
               "Invalid map size");

char const wolfasm_map_name[255] = "Simple Map";

wolfasm_map_item_t wolfasm_items[] = {
    {11, 4, 4, 1, 1, 1.0, 0, 3, 10, TABLE_ENEMY_ANIMATION_SHOOT, -1, ITEM_ENEMY,
     0},
    {9, 4, 3, 2, 2, 64.0, 0, 0, 1, 0, 5, ITEM_AMMO, CALLBACK_AMMO},
    {9, 2, 3, 1, 1, 64.0, 0, 0, 1, 0, -1, ITEM_AMMO, CALLBACK_AMMO},
    {10, 3, 2, 2, 2, 64.0, 0, 0, 1, 0, -1, ITEM_MEDIKIT, CALLBACK_LIFE}};

uint32_t const wolfasm_items_nb =
    sizeof(wolfasm_items) / sizeof(wolfasm_items[0]);
