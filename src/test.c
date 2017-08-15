// This files permits me to try things in C, before implementing them in
// assembly

#include "cdefs.h"
#include <SDL2/SDL_Image.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <time.h>

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
  srand((unsigned int)time(NULL));
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
void enemy_init_single(struct wolfasm_enemy_s *enemy);
void spawn_enemy(struct wolfasm_enemy_s *enemy);

void game_logic_cwrapper(void);
void game_logic_cwrapper(void) {
  extern int32_t enemy_animation_shoot[] __asm__("enemy_animation_shoot");

  // Implement game logic elements here
  for (int32_t i = 0; i < wolfasm_enemies_nb; ++i) {
    if (enemies[i].item) {
      if (!enemies[i].item->stock) {
        spawn_enemy(&enemies[i]);
      }
      switch (enemies[i].state) {
      case ENEMY_DIE:
        if (enemies[i].item->current_anim / enemies[i].item->anim_rate ==
            enemies[i].item->nb_anim - 1) {
          enemies[i].item->stock = 0;
          map[enemies[i].item->pos_y * map_width + enemies[i].item->pos_x]
              .enemy = NULL;
        }
        break;
      case ENEMY_MOVE:
        break;
      case ENEMY_SHOOT:
        break;
      case ENEMY_STILL:
        break;
      case ENEMY_HIT:
        if (enemies[i].item->current_anim / enemies[i].item->anim_rate ==
            enemies[i].item->nb_anim - 1) {
          enemies[i].state = ENEMY_STILL;
          enemies[i].item->texture_table = enemy_animation_shoot;
          enemies[i].item->nb_anim = 3;
          enemies[i].item->current_anim = 0;
        }
        break;
      }
    }
  }
}

// Enemy
void spawn_enemy(struct wolfasm_enemy_s *enemy) {
  int32_t x = 0;
  int32_t y = 0;

  do {
    x = rand() % map_width;
    y = rand() % map_height;
  } while (map[y * map_width + x].value);
  enemy->item->pos_x = x;
  enemy->item->pos_y = y;
  enemy_init_single(enemy);
}

void enemy_init_single(struct wolfasm_enemy_s *enemy) {
  extern int32_t enemy_animation_shoot[] __asm__(
      "enemy_animation_shoot"); // TODO: Change table

  enemy->state = ENEMY_STILL;
  enemy->life = 100;
  map[enemy->item->pos_y * map_width + enemy->item->pos_x].enemy = enemy;
  enemy->item->stock = -1;
  enemy->item->texture_table = enemy_animation_shoot;
  enemy->item->nb_anim = 3;
  enemy->item->current_anim = 0;
}

void enemy_init(void) {
  // Init enemies here
  enemies = calloc(sizeof(*enemies) * (size_t)wolfasm_items_nb, 1);
  if (!enemies) {
    perror("calloc");
    exit(1);
  }
  for (int32_t i = 0; i < wolfasm_items_nb; ++i) {
    if (wolfasm_items[i].type == ITEM_ENEMY) {
      enemies[wolfasm_enemies_nb].item = &wolfasm_items[i];
      enemy_init_single(&enemies[wolfasm_enemies_nb]);
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
  extern int enemy_animation_hit[] __asm__("enemy_animation_hit");

  if (game_player.weapon->sprite->trigger == 0) {

    // Check that enough ammo
    if (game_player.weapon->ammo) {
      wolfasm_play_sound(game_player.weapon->sound);
      --game_player.weapon->ammo;

      // Detect if any enemy there
      {
        double const inc_base = 0.0001;
        int32_t pos = 0;
        double inc = inc_base;
        do {
          int32_t const pos_x =
              (int)(game_player.pos_x + (double)game_player.dir_x * inc);
          int32_t const pos_y =
              (int)(game_player.pos_y + (double)game_player.dir_y * inc);
          pos = pos_y * map_width + pos_x;
          if (map[pos].enemy && map[pos].enemy->life > 0) {
            assert(map[pos].value == 0);
            map[pos].enemy->life -= game_player.weapon->damage;

            map[pos].enemy->state = ENEMY_HIT;
            map[pos].enemy->item->texture_table = enemy_animation_hit;
            map[pos].enemy->item->nb_anim = 4;
            map[pos].enemy->item->current_anim = 0;

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

// Menu
extern void
wolfasm_display_text(char const *text, SDL_Rect const *pos,
                     uint32_t cololor) __asm__("wolfasm_display_text");
void render_button(char const *text, int32_t const x, int32_t y, int32_t width,
                   bool selected) {
  extern TTF_Font *gui_font __asm__("gui_font");
  SDL_Rect const rect = {x, y, width, window_height / 16};

  if (selected) {
    TTF_SetFontStyle(gui_font, TTF_STYLE_UNDERLINE);
  }

  wolfasm_display_text(text, &rect, 0xFFFFFFFF);

  if (selected) {
    TTF_SetFontStyle(gui_font, TTF_STYLE_NORMAL);
  }
}

enum wolfasm_buttons {
  BUTTON_PLAY = 0,
  BUTTON_MULTIPLAYER,
  BUTTON_EXIT,
  NB_BUTTON
};

void wolfasm_regulate_framerate(uint32_t fps) {
  static uint32_t old_ticks = 0;
  uint32_t ticks = SDL_GetTicks();
  uint32_t elapsed = ticks - old_ticks;

  if (elapsed < 1000u / fps) {
    SDL_Delay((1000u / fps) - elapsed);
  }
  old_ticks = ticks;
}

static Mix_Music *wolfasm_music[2] = {};
char const *wolfasm_music_files[] = {"./resources/sounds/menu_0.ogg",
                                     "./resources/sounds/menu_1.ogg"};

void wolfasm_load_music() {
  for (size_t i = 0;
       i < sizeof(wolfasm_music_files) / sizeof(wolfasm_music_files[0]); ++i) {
    char const *file = wolfasm_music_files[i];

    wolfasm_music[i] = Mix_LoadMUS(file);
    if (!wolfasm_music[i]) {
      printf("Error loading %s : %s\n", file, SDL_GetError());
      exit(1);
    }
  }
}

void wolfasm_deinit_music() {
  Mix_HaltMusic();
  for (size_t i = 0; i < sizeof(wolfasm_music) / sizeof(wolfasm_music[0]);
       ++i) {
    Mix_FreeMusic(wolfasm_music[i]);
  }
}

void wolfasm_menu_play_music(void) {
  int ndx = rand() % (sizeof(wolfasm_music) / sizeof(wolfasm_music[0]));
  Mix_PlayMusic(wolfasm_music[ndx], 1);
}

extern void wolfasm(void) __asm__("wolfasm");
int wolfasm_menu(void) {
  bool running = true;

  wolfasm_load_music();
  wolfasm_menu_play_music();
  while (running) {
    static enum wolfasm_buttons selected_button = BUTTON_PLAY;
    static void (*callbacks[NB_BUTTON])() = {&wolfasm, NULL, NULL};
    SDL_Event event;

    // Handle events
    if (!SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        running = false;
        break;

      case SDL_KEYUP:
        switch (event.key.keysym.sym) {

        // Menu controls
        case SDLK_LEFT:
          break;
        case SDLK_RIGHT:
          break;
        case SDLK_UP:
          SDL_Delay(10);
          break;
        case SDLK_DOWN:
          SDL_Delay(10);
          break;
        case SDLK_RETURN:
          break;
        }
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {

        // Menu controls
        case SDLK_LEFT:
          break;
        case SDLK_RIGHT:
          break;
        case SDLK_UP:
          if (selected_button == 0) {
            selected_button = NB_BUTTON - 1;
          } else {
            --selected_button;
          }
          SDL_Delay(120);
          break;
        case SDLK_DOWN:
          ++selected_button;
          if (selected_button == NB_BUTTON) {
            selected_button = 0;
          }
          SDL_Delay(120);
          break;
        case SDLK_RETURN:
          if (selected_button == BUTTON_EXIT) {
            running = false;
          } else if (callbacks[selected_button]) {
            Mix_HaltMusic();
            callbacks[selected_button]();
            wolfasm_menu_play_music();
            SDL_Delay(120);
          }
          break;

        case SDLK_ESCAPE:
          running = false;
          break;

        default:
          break;
        }
        break;

      default:
        break;
      }
    }

    // Display  Menu
    SDL_RenderClear(window_renderer);
    render_sprite(&wolfasm_sprite[3], 0, 0, NULL);
    render_button("Play", window_width / 2, window_height / 2,
                  window_width / 16, selected_button == BUTTON_PLAY);
    render_button("Multiplayer", window_width / 2,
                  window_height / 2 + window_height / 8, window_width / 4,
                  selected_button == BUTTON_MULTIPLAYER);
    render_button("Exit", window_width / 2,
                  window_height / 2 + 2 * (window_height / 8),
                  window_width / 16, selected_button == BUTTON_EXIT);
    SDL_RenderPresent(window_renderer);
    wolfasm_regulate_framerate(60);
  }

  wolfasm_deinit_music();
  return EXIT_SUCCESS;
}
