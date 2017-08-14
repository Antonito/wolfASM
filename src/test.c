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
void enemy_init(void);
void enemy_deinit(void);

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
  enemy_init();
}

void c_deinit() {
  // You can initialize stuff after the game loop
  enemy_deinit();
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
static int32_t wolfasm_enemies_nb = 0;
static struct wolfasm_enemy_s *enemies = NULL;

void game_logic_cwrapper(void);
void game_logic_cwrapper(void) {
  // Implement game logic elements here
  for (int32_t i = 0; i < wolfasm_enemies_nb; ++i) {
    switch (enemies[i].state) {
    case ENEMY_DIE:
      if (!enemies[i].item->current_anim) {
        enemies[i].item->stock = 0;
        map[enemies[i].item->pos_y * map_width + enemies[i].item->pos_x].enemy =
            NULL;
      }
      break;
    default:
      break;
    }
  }
}

// Enemy

void enemy_init(void) {
  // Init enemies here
  enemies = calloc(sizeof(*enemies) * wolfasm_items_nb, 1);
  if (!enemies) {
    perror("calloc");
    exit(1);
  }
  for (int32_t i = 0; i < wolfasm_items_nb; ++i) {
    if (wolfasm_items[i].type == ITEM_ENEMY) {
      enemies[wolfasm_enemies_nb].state = ENEMY_STILL;
      enemies[wolfasm_enemies_nb].item = &wolfasm_items[i];
      enemies[wolfasm_enemies_nb].life = 100;
      map[wolfasm_items[i].pos_y * map_width + wolfasm_items[i].pos_x].enemy =
          &enemies[wolfasm_enemies_nb];
      ++wolfasm_enemies_nb;
    }
  }
}

void enemy_deinit(void) {
  // De-init enemies here
  free(enemies);
}

void enemy_kill(struct wolfasm_enemy_s *const enemy);
void enemy_kill(struct wolfasm_enemy_s *const enemy) {
  extern int32_t enemy_animation_die[] __asm__("enemy_animation_die");
  assert(enemy->life <= 0);

  enemy->state = ENEMY_DIE;
  enemy->item->texture_table = enemy_animation_die;
  enemy->item->nb_anim = 7;
}

void player_shoot(void);
void player_shoot(void) {
  extern void wolfasm_play_sound(int) __asm__("wolfasm_play_sound");

  if (game_player.weapon->sprite->trigger == 0) {

    // Check that enough ammo
    if (game_player.weapon->ammo) {
      wolfasm_play_sound(game_player.weapon->sound);
      --game_player.weapon->ammo;

      // Detect if any enemy there
      {
        double const inc_base = 0.01;
        int32_t pos = 0;
        double inc = inc_base;
        do {
          int32_t const pos_x =
              game_player.pos_x + (int)((double)game_player.dir_x * inc);
          int32_t const pos_y =
              game_player.pos_y + (int)((double)game_player.dir_y * inc);
          pos = pos_y * map_width + pos_x;
          if (map[pos].enemy) {
            assert(map[pos].value == 0);
            map[pos].enemy->life -= game_player.weapon->damage;
            printf("Enemy Life: %d [-%d]\n", map[pos].enemy->life,
                   game_player.weapon->damage);
            if (map[pos].enemy->life <= 0) {
              enemy_kill(map[pos].enemy);
            }
            break;
          }
          inc += inc_base;
        } while (!map[pos].value);
      }

    } else {
      wolfasm_play_sound(SOUND_NO_AMMO);
    }

    game_player.weapon->sprite->trigger = 1;
  }
}
