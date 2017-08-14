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

void comb_sort(int *order, double *dist, int amount);
void comb_sort(int *order, double *dist, int amount) {
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

// Game logic
void game_logic_cwrapper(void);
void game_logic_cwrapper(void) {
  // Implement game logic elements here
}
