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

void c_init() {}

void c_deinit() {}

//
// Sprites
//
extern void render_sprite(wolfasm_sprite_t *sprite, int x, int y,
                          SDL_Rect *clip) __asm__("wolfasm_render_sprite");

void display_sprites_cwrapper(void) {
  if (!game_player.weapon) {
    return;
  }
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
