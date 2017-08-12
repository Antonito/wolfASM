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

void wolfasm_player_refill_ammo() __asm__("wolfasm_player_refill_ammo");
void wolfasm_player_refill_life() __asm__("wolfasm_player_refill_life");
extern struct wolfasm_item_s wolfasm_items[] __asm__("wolfasm_items");
extern int32_t wolfasm_items_nb __asm__("wolfasm_items_nb");

extern double wolfasm_z_buffer[] __asm__("wolfasm_z_buffer");
void c_init() {
  // You can initialize stuff before the game loop
  // memset(wolfasm_z_buffer, 0, sizeof(double) * (size_t)window_width);
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

extern uint32_t wolfasm_texture[10][64 * 64] __asm__("wolfasm_texture");
extern void wolfasm_put_pixel(int32_t x, int32_t y,
                              uint32_t color) __asm__("wolfasm_put_pixel");

void combSort(int *order, double *dist, int amount) {
  int gap = amount;
  bool swapped = false;
  while (gap > 1 || swapped) {
    // shrink factor 1.3
    gap = (gap * 10) / 13;
    if (gap == 9 || gap == 10)
      gap = 11;
    if (gap < 1)
      gap = 1;
    swapped = false;
    for (int i = 0; i < amount - gap; i++) {
      int j = i + gap;
      if (dist[i] < dist[j]) {
        {
          double tmp;

          tmp = dist[i];
          dist[i] = dist[j];
          dist[j] = tmp;
        }
        {
          int tmp;

          tmp = order[i];
          order[i] = order[j];
          order[j] = tmp;
        }
        swapped = true;
      }
    }
  }
}

void display_sprites(int i);
void display_sprites(int i) {
  extern int wolfasm_sprite_order[] __asm__("wolfasm_sprite_order");

  // after sorting the sprites, do the projection and draw them
  struct wolfasm_item_s *item = &wolfasm_items[wolfasm_sprite_order[i]];

  extern double const transformX __asm__("wolfasm_item_transform_x");
  extern double const transformY __asm__("wolfasm_item_transform_y");
  extern int const vMoveScreen __asm__("wolfasm_item_vmove_screen");
  extern int const spriteScreenX __asm__("wolfasm_item_sprite_screen_x");
  extern int const spriteHeight __asm__("wolfasm_item_sprite_height");

  int const w = window_width;
  assert(spriteScreenX == (int)((w / 2) * (1 + transformX / transformY)));

  int const h = window_height;

  int drawStartY = -spriteHeight / 2 + h / 2 + vMoveScreen;
  if (drawStartY < 0)
    drawStartY = 0;
  int drawEndY = spriteHeight / 2 + h / 2 + vMoveScreen;
  if (drawEndY >= h)
    drawEndY = h - 1;

  // calculate width of the sprite
  int const spriteWidth = abs((int)(h / (transformY)) / item->width_div);
  int drawStartX = -spriteWidth / 2 + spriteScreenX;
  if (drawStartX < 0)
    drawStartX = 0;
  int drawEndX = spriteWidth / 2 + spriteScreenX;
  if (drawEndX >= w)
    drawEndX = w - 1;

  for (int stripe = drawStartX; stripe < drawEndX; stripe++) {
    int const texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) *
                           64 / spriteWidth) /
                     256;

    if (transformY > 0 && stripe > 0 && stripe < w &&
        transformY < wolfasm_z_buffer[stripe])
      for (int y = drawStartY; y < drawEndY;
           y++) // for every pixel of the current stripe
      {
        int d = (y - vMoveScreen) * 256 - h * 128 +
                spriteHeight * 128; // 256 and 128 factors to avoid floats
        int texY = ((d * 64) / spriteHeight) / 256;
        uint32_t color = wolfasm_texture[item->texture][64 * texY + texX];
        if (color & 0xFF000000)
          wolfasm_put_pixel(stripe, y, color);
      }
  }
}

// Game logic
void game_logic_cwrapper(void);
void game_logic_cwrapper(void) {
  // Implement game logic elements here
}
