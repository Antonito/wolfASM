#include "cdefs.h"
#include "map_binary.h"

extern wolfasm_map_case_t wolfasm_map[];
extern uint32_t const wolfasm_map_width;
extern uint32_t const wolfasm_map_height;
extern char const wolfasm_map_name[255];
extern wolfasm_map_item_t wolfasm_items[];
extern uint32_t const wolfasm_items_nb;

#define MAP_WIDTH (24u)
#define MAP_HEIGHT (24u)

// clang-format off
wolfasm_map_case_t  wolfasm_map[] = {
  {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {
  4, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {
  4, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {
  4, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {
  4, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {3, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {
  4, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {
  4, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {
  4, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {8, NULL, NULL, NULL}, {
  4, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {
  4, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {8, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {8, NULL, NULL, NULL}, {
  4, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {
  4, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {7, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {
  6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {
  8, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {
  6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {
  4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {3, NULL, NULL, NULL}, {3, NULL, NULL, NULL}, {3, NULL, NULL, NULL}, {3, NULL, NULL, NULL}, {
  4, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {
  4, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {
  4, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {
  4, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {
  4, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {
  4, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {5, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {
  4, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {6, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {0, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {
  4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {1, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {3, NULL, NULL, NULL}, {3, NULL, NULL, NULL}, {3, NULL, NULL, NULL}, {3, NULL, NULL, NULL}, {3, NULL, NULL, NULL}
};
// clang-format on

uint32_t const wolfasm_map_width = MAP_WIDTH;
uint32_t const wolfasm_map_height = MAP_HEIGHT;

_Static_assert(sizeof(wolfasm_map) ==
                   MAP_WIDTH * MAP_HEIGHT * sizeof(wolfasm_map[0]),
               "Invalid map size");

char const wolfasm_map_name[255] = "Medium Map";

wolfasm_map_item_t wolfasm_items[] = {};

uint32_t const wolfasm_items_nb =
    sizeof(wolfasm_items) / sizeof(wolfasm_items[0]);
