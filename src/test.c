// This files permits me to try things in C, before implementing them in
// assembly

#include "cdefs.h"
#include <SDL2/SDL_Image.h>
#include <assert.h>

//
// Prototypes
//
void c_deinit(void);
void c_init(void);
void init_textures(void);

void display_sprites_cwrapper(void);

void init_weapons(void);

void c_init() {
  // You can initialize stuff before the game loop
}

void c_deinit() {
  // You can initialize stuff after the game loop
}

//
// Sprites
//
extern void render_sprite(wolfasm_sprite_t *sprite, int x, int y,
                          SDL_Rect *clip) __asm__("wolfasm_render_sprite");

void display_sprites_cwrapper(void) {
  if (game_player.weapon) {

    wolfasm_sprite_t *weapon_sprite = game_player.weapon->sprite;

    render_sprite(weapon_sprite, window_width / 2 - 30 * 4,
                  window_height - weapon_sprite->height,
                  &weapon_sprite->sprite[weapon_sprite->current_anim / 8]);

    if (weapon_sprite->trigger) {
      ++weapon_sprite->current_anim;
      if (weapon_sprite->current_anim == weapon_sprite->nb_sprites * 8) {
        weapon_sprite->current_anim = 0;
        weapon_sprite->trigger = 0;
      }
    }
  }
}

void draw_floor(int32_t x) {

  extern void wolfasm_put_pixel(int32_t x, int32_t y,
                                uint32_t color) __asm__("wolfasm_put_pixel");
  extern double rayDirX __asm__("ray_dir_x");
  extern double rayDirY __asm__("ray_dir_y");
  extern double posX __asm__("ray_pos_x");
  extern double posY __asm__("ray_pos_y");
  extern int32_t side __asm__("side");
  extern int32_t mapX __asm__("map_x");
  extern int32_t mapY __asm__("map_y");
  extern int32_t drawStart __asm__("draw_start");
  extern int32_t drawEnd __asm__("draw_end");

  extern double perpWallDist __asm__("wall_dist");
#define TEXTURE_NB 9
  extern uint32_t texture[TEXTURE_NB][64 * 64] __asm__("wolfasm_texture");
  extern double wallX __asm__("wall_hit_x");

  int32_t h = window_height;
  int32_t texWidth = 64;
  int32_t texHeight = 64;

  // FLOOR CASTING
  double floorXWall,
      floorYWall; // x, y position of the floor texel at the bottom of the
                  // wall

  // 4 different wall directions possible
  if (side == 0 && rayDirX > 0) {
    floorXWall = mapX;
    floorYWall = mapY + wallX;
  } else if (side == 0 && rayDirX < 0) {
    floorXWall = mapX + 1.0;
    floorYWall = mapY + wallX;
  } else if (side == 1 && rayDirY > 0) {
    floorXWall = mapX + wallX;
    floorYWall = mapY;
  } else {
    floorXWall = mapX + wallX;
    floorYWall = mapY + 1.0;
  }

  double distWall, distPlayer, currentDist;

  distWall = perpWallDist;
  distPlayer = 0.0;

  if (drawEnd < 0)
    drawEnd = h; // becomes < 0 when the integer overflows

  // draw the floor from drawEnd to the bottom of the screen
  for (int y = drawEnd + 1; y < h; y++) {
    currentDist =
        h /
        (2.0 * y - h); // you could make a small lookup table for this instead

    double weight = (currentDist - distPlayer) / (distWall - distPlayer);

    double currentFloorX = weight * floorXWall + (1.0 - weight) * posX;
    double currentFloorY = weight * floorYWall + (1.0 - weight) * posY;

    int floorTexX, floorTexY;
    floorTexX = (int)(currentFloorX * texWidth) % texWidth;
    floorTexY = (int)(currentFloorY * texHeight) % texHeight;

    // floor
    uint32_t color =
        (texture[8][texWidth * floorTexY + floorTexX] >> 1) & 8355711;
    wolfasm_put_pixel(x, y, color);

    // ceiling (symmetrical!)
    // color = (texture[8][texWidth * floorTexY + floorTexX] >> 1) & 8355711;
  }
}
