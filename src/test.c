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
static int32_t enemy_animation[] = {11, 12, 13};

struct wolfasm_item_s wolfasm_items[] __asm__("wolfasm_items") = {
    {9, 4, 3, 2, 2, 64.0, 0, 0, 1, NULL, 5, ITEM_AMMO,
     &wolfasm_player_refill_ammo},
    {9, 2, 3, 1, 1, 64.0, 0, 0, 1, NULL, 5, ITEM_AMMO, &wolfasm_refill_ammo},
    {11, 4, 4, 1, 1, 1.00, 0, 0, 30, NULL, -1, ITEM_ENEMY, NULL},
    {10, 4, 2, 2, 2, 64.0, 0, 0, 1, NULL, 5, ITEM_MEDIKIT,
     &wolfasm_player_refill_life}};
int32_t wolfasm_items_nb __asm__("wolfasm_items_nb") = sizeof(wolfasm_items) /
                                                       sizeof(wolfasm_items[0]);

extern double wolfasm_z_buffer[] __asm__("wolfasm_z_buffer");
void c_init() {
  // You can initialize stuff before the game loop
  memset(wolfasm_z_buffer, 0, sizeof(double) * window_width);

  // Initialize items
  for (int32_t i = 0; i < wolfasm_items_nb; ++i) {
    map[wolfasm_items[i].pos_y * map_width + wolfasm_items[i].pos_x].item =
        &wolfasm_items[i];
  }
  wolfasm_items[2].nb_anim =
      sizeof(enemy_animation) / sizeof(enemy_animation[0]);
  wolfasm_items[2].texture_table = enemy_animation;
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

static void combSort(int *order, double *dist, int amount) {
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

void display_sprites(void);
void display_sprites(void) {
  int wolfasm_sprite_order[wolfasm_items_nb];
  double wolfasm_sprite_distance[wolfasm_items_nb];

  for (int i = 0; i < wolfasm_items_nb; i++) {
    wolfasm_sprite_order[i] = i;
    wolfasm_sprite_distance[i] =
        ((game_player.pos_x - wolfasm_items[i].pos_x) *
             (game_player.pos_x - wolfasm_items[i].pos_x) +
         (game_player.pos_y - wolfasm_items[i].pos_y) *
             (game_player.pos_y -
              wolfasm_items[i].pos_y)); // sqrt not taken, unneeded
  }
  combSort(wolfasm_sprite_order, wolfasm_sprite_distance, wolfasm_items_nb);

  // after sorting the sprites, do the projection and draw them
  for (int i = 0; i < wolfasm_items_nb; i++) {

    if (wolfasm_items[wolfasm_sprite_order[i]].stock) {

      // translate sprite position to relative to camera
      int h = window_height;
      int w = window_width;

      double spriteX =
          wolfasm_items[wolfasm_sprite_order[i]].pos_x - game_player.pos_x;
      double spriteY =
          wolfasm_items[wolfasm_sprite_order[i]].pos_y - game_player.pos_y;

      double invDet = 1.0 / (game_player.plane_x * game_player.dir_y -
                             game_player.dir_x * game_player.plane_y);

      double transformX =
          invDet * (game_player.dir_y * spriteX - game_player.dir_x * spriteY);
      double transformY = invDet * (-game_player.plane_y * spriteX +
                                    game_player.plane_x * spriteY);
      int vMoveScreen =
          (int)(wolfasm_items[wolfasm_sprite_order[i]].height_move /
                transformY);

      int spriteScreenX = (int)((w / 2) * (1 + transformX / transformY));

      // calculate height of the sprite on screen
      int spriteHeight = abs((int)(h / (transformY))) /
                         wolfasm_items[wolfasm_sprite_order[i]].height_div;

      int drawStartY = -spriteHeight / 2 + h / 2 + vMoveScreen;
      if (drawStartY < 0)
        drawStartY = 0;
      int drawEndY = spriteHeight / 2 + h / 2 + vMoveScreen;
      if (drawEndY >= h)
        drawEndY = h - 1;

      // calculate width of the sprite
      int spriteWidth = abs((int)(h / (transformY)) /
                            wolfasm_items[wolfasm_sprite_order[i]].width_div);
      int drawStartX = -spriteWidth / 2 + spriteScreenX;
      if (drawStartX < 0)
        drawStartX = 0;
      int drawEndX = spriteWidth / 2 + spriteScreenX;
      if (drawEndX >= w)
        drawEndX = w - 1;

      for (int stripe = drawStartX; stripe < drawEndX; stripe++) {
        int texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) *
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
            uint32_t color =
                wolfasm_texture[wolfasm_items[wolfasm_sprite_order[i]].texture]
                               [64 * texY + texX];
            if (color & 0xFF000000)
              wolfasm_put_pixel(stripe, y, color);
          }
      }

      // Sprite animation
      if (wolfasm_items[wolfasm_sprite_order[i]].texture_table) {
        ++wolfasm_items[wolfasm_sprite_order[i]].current_anim;
        int anim = wolfasm_items[wolfasm_sprite_order[i]].current_anim /
                   wolfasm_items[wolfasm_sprite_order[i]].anim_rate;
        if (anim >= wolfasm_items[wolfasm_sprite_order[i]].nb_anim) {
          wolfasm_items[wolfasm_sprite_order[i]].current_anim = 0;
          anim = 0;
        }
        wolfasm_items[wolfasm_sprite_order[i]].texture =
            wolfasm_items[wolfasm_sprite_order[i]].texture_table[anim];
      }
    }
  }
}

// Game logic
void game_logic_cwrapper(void);
void game_logic_cwrapper(void) {
  struct wolfasm_item_s *item =
      map[(int32_t)(game_player.pos_y * map_width + game_player.pos_x)].item;

  if (item) {
    if (item->callback) {
      item->callback();
    }
    if (item->stock > 0) {
      --item->stock;
      if (!item->stock) {
        map[(int32_t)(game_player.pos_y * map_width + game_player.pos_x)].item =
            NULL;
      }
    }
  }
}
