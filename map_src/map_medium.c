#include "cdefs.h"

extern uint32_t const wolfasm_map_width;
extern uint32_t const wolfasm_map_height;
extern char const wolfasm_map_name[255];

#define MAP_WIDTH (24u)
#define MAP_HEIGHT (24u)

// clang-format off
wolfasm_map_case_t  map[] = {
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

_Static_assert(sizeof(map) == MAP_WIDTH * MAP_HEIGHT * sizeof(map[0]),
               "Invalid map size");

char const wolfasm_map_name[255] = "Medium Map";
