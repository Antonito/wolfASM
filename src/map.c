// Temporary file, will be removed when I have a map loader

#include <stdint.h>

#define MAP_WIDTH (8)
#define MAP_HEIGHT (7)

extern int32_t const map_width;
extern int32_t const map_height;
extern uint8_t const map[];

// clang-format off
int32_t const map_width __asm__("map_width") = MAP_WIDTH;
int32_t const map_height __asm__("map_height") = MAP_HEIGHT;
uint8_t const map[] __asm__("map") = {
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 1, 0, 0, 1, 0, 1,
  1, 0, 1, 0, 0, 1, 0, 1,
  1, 0, 1, 0, 0, 1, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 1,
  1, 1, 1, 1, 1, 1, 1, 1
};
// clang-format on
_Static_assert(sizeof(map) == sizeof(map[0]) * MAP_WIDTH * MAP_HEIGHT,
               "Invalid map size");
