// Temporary file, will be removed when I have a map loader

#include "cdefs.h"
#include <stdint.h>

#define MAP_NUMBER 1

extern int32_t const map_width;
extern int32_t const map_height;
extern wolfasm_map_case_t map[];

#if MAP_NUMBER == 1

#define MAP_WIDTH (8)
#define MAP_HEIGHT (7)

// clang-format off
wolfasm_map_case_t  map[] __asm__("map") = {
    {1, NULL}, {1, NULL}, {1, NULL}, {1, NULL}, {1, NULL}, {1, NULL}, {1, NULL}, {1, NULL},
    {1, NULL}, {0, NULL}, {0, NULL}, {0, NULL}, {0, NULL}, {0, NULL}, {0, NULL}, {1, NULL},
    {1, NULL}, {0, NULL}, {1, NULL}, {0, NULL}, {0, NULL}, {1, NULL}, {0, NULL}, {1, NULL},
    {1, NULL}, {0, NULL}, {0, NULL}, {0, NULL}, {0, NULL}, {1, NULL}, {0, NULL}, {1, NULL},
    {1, NULL}, {0, NULL}, {1, NULL}, {0, NULL}, {0, NULL}, {1, NULL}, {0, NULL}, {1, NULL},
    {1, NULL}, {0, NULL}, {0, NULL}, {0, NULL}, {0, NULL}, {0, NULL}, {0, NULL}, {1, NULL},
    {1, NULL}, {1, NULL}, {1, NULL}, {1, NULL}, {1, NULL}, {1, NULL}, {1, NULL}, {1, NULL}};
//clang-format on

#elif MAP_NUMBER == {2, NULL}

#define MAP_WIDTH ({2, NULL}{4, NULL}
#define MAP_HEIGHT ({2, NULL}{4, NULL}

// clang-format off
wolfasm_map_case_t  map[] __asm__("map") = {
  {4, NULL},{4, NULL}, {7, NULL},{7, NULL},{7, NULL},{7, NULL},{7, NULL},{7, NULL},{7, NULL},{7, NULL},
  {4, NULL}{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{7, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{7, NULL},
  {4, NULL}{0, NULL},{1, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{7, NULL},
  {4, NULL}{0, NULL},{2, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{7, NULL},
  {4, NULL}{0, NULL},{3, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{7, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{7, NULL},
  {4, NULL}{0, NULL},{4, NULL}{0, NULL},{0, NULL},{0, NULL},{0, NULL},{5, NULL},{5, NULL},{5, NULL},{5, NULL},{5, NULL},{5, NULL},{5, NULL},{5, NULL},{5, NULL},{7, NULL},{7, NULL},{0, NULL},{7, NULL},{7, NULL},{7, NULL},{7, NULL},{7, NULL},
  {4, NULL}{0, NULL},{5, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{5, NULL},{0, NULL},{5, NULL},{0, NULL},{5, NULL},{0, NULL},{5, NULL},{0, NULL},{5, NULL},{7, NULL},{0, NULL},{0, NULL},{0, NULL},{7, NULL},{7, NULL},{7, NULL},{1, NULL},
  {4, NULL}{0, NULL},{6, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{5, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{5, NULL},{7, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{8, NULL},
  {4, NULL}{0, NULL},{7, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{7, NULL},{7, NULL},{7, NULL},{1, NULL},
  {4, NULL}{0, NULL},{8, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{5, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{5, NULL},{7, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{8, NULL},
  {4, NULL}{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{5, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{5, NULL},{7, NULL},{0, NULL},{0, NULL},{0, NULL},{7, NULL},{7, NULL},{7, NULL},{1, NULL},
  {4, NULL}{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{5, NULL},{5, NULL},{5, NULL},{5, NULL},{0, NULL},{5, NULL},{5, NULL},{5, NULL},{5, NULL},{7, NULL},{7, NULL},{7, NULL},{7, NULL},{7, NULL},{7, NULL},{7, NULL},{1, NULL},
  {6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{0, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},
  {8, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{4, NULL}
  {6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{0, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{0, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},{6, NULL},
  {4, NULL}{0, NULL},{4, NULL}{6, NULL},{0, NULL},{6, NULL},{2, NULL},{2, NULL},{2, NULL},{2, NULL},{2, NULL},{2, NULL},{2, NULL},{3, NULL},{3, NULL},{3, NULL},{3, NULL},
  {4, NULL}{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{4, NULL}{6, NULL},{0, NULL},{6, NULL},{2, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{2, NULL},{0, NULL},{0, NULL},{0, NULL},{2, NULL},
  {4, NULL}{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{6, NULL},{2, NULL},{0, NULL},{0, NULL},{5, NULL},{0, NULL},{0, NULL},{2, NULL},{0, NULL},{0, NULL},{0, NULL},{2, NULL},
  {4, NULL}{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{4, NULL}{6, NULL},{0, NULL},{6, NULL},{2, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{2, NULL},{2, NULL},{0, NULL},{2, NULL},{2, NULL},
  {4, NULL}{0, NULL},{6, NULL},{0, NULL},{6, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{4, NULL}{6, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{5, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{2, NULL},
  {4, NULL}{0, NULL},{0, NULL},{5, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{4, NULL}{6, NULL},{0, NULL},{6, NULL},{2, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{2, NULL},{2, NULL},{0, NULL},{2, NULL},{2, NULL},
  {4, NULL}{0, NULL},{6, NULL},{0, NULL},{6, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{4, NULL}{6, NULL},{0, NULL},{6, NULL},{2, NULL},{0, NULL},{0, NULL},{5, NULL},{0, NULL},{0, NULL},{2, NULL},{0, NULL},{0, NULL},{0, NULL},{2, NULL},
  {4, NULL}{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{4, NULL}{6, NULL},{0, NULL},{6, NULL},{2, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{0, NULL},{2, NULL},{0, NULL},{0, NULL},{0, NULL},{2, NULL},
  {4, NULL}{1, NULL},{1, NULL},{1, NULL},{2, NULL},{2, NULL},{2, NULL},{2, NULL},{2, NULL},{2, NULL},{3, NULL},{3, NULL},{3, NULL},{3, NULL},{3, NULL}
};
// clang-format on
#endif

int32_t const map_width __asm__("map_width") = MAP_WIDTH;
int32_t const map_height __asm__("map_height") = MAP_HEIGHT;

_Static_assert(sizeof(map) == sizeof(map[0]) * MAP_WIDTH * MAP_HEIGHT,
               "Invalid map size");
