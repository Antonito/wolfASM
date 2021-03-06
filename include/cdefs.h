#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include <stdint.h>

// ASM Bindings
void wolfasm_player_move_forward(void) __asm__("wolfasm_player_move_forward");
void wolfasm_player_move_backward(void) __asm__("wolfasm_player_move_backward");
double wolfasm_player_rotate_right(void) __asm__("wolfasm_player_rotate_right");
void wolfasm_player_rotate_left(void) __asm__("wolfasm_player_rotate_left");

// Events
void wolfasm_events_keyboard_up_cwrapper(SDL_Event const *events) __asm__(
    "wolfasm_events_keyboard_up_cwrapper");
void wolfasm_events_keyboard_down_cwrapper(SDL_Event const *events) __asm__(
    "wolfasm_events_keyboard_down_cwrapper");
void wolfasm_events_mouse_up_cwrapper(SDL_Event const *events) __asm__(
    "wolfasm_events_mouse_up_cwrapper");
void wolfasm_events_mouse_down_cwrapper(SDL_Event const *events) __asm__(
    "wolfasm_events_mouse_down_cwrapper");
void wolfasm_events_mouse_motion_cwrapper(SDL_Event const *events) __asm__(
    "wolfasm_events_mouse_motion_cwrapper");
void wolfasm_event_window_cwrapper(SDL_Event const *events) __asm__(
    "wolfasm_event_window_cwrapper");
void wolfasm_events_exec_cwrapper(void) __asm__("wolfasm_events_exec_cwrapper");

// Weapons
void *wolfasm_change_weapon(int32_t weapon) __asm__("wolfasm_change_weapon");

// Handle game events
extern SDL_Event game_events;
SDL_Event game_events __asm__("game_events");

typedef struct wolfasm_weapon_s wolfasm_weapon_t;

#if defined __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

// Player informations
struct wolfasm_player {
  double pos_x, pos_y;
  double dir_x, dir_y;
  double plane_x, plane_y;
  double movement_speed;
  double rotation_speed;
  wolfasm_weapon_t *weapon;
  int32_t life;
};
extern struct wolfasm_player game_player __asm__("game_player");

// Map informations
typedef struct wolfasm_map_case {
  uint32_t value;
  struct wolfasm_item_s *item;
  struct wolfasm_enemy_s *enemy;
  void *padding;
} wolfasm_map_case_t;
_Static_assert(sizeof(wolfasm_map_case_t) == 32, "Invalid map case size");

#if defined __clang__
#pragma clang diagnostic pop
#endif

// Window informations
extern SDL_Window *window_ptr __asm__("window_ptr");
extern SDL_Surface *window_surface __asm__("window_surface");
extern SDL_Renderer *window_renderer __asm__("window_renderer");
extern int32_t window_width __asm__("window_width");
extern int32_t window_height __asm__("window_height");

//
// Audio
//
enum wolfasm_sounds {
  SOUND_PISTOL = 0,
  SOUND_SHOTGUN,
  SOUND_BARREL,
  SOUND_NO_AMMO,
  NB_WOLFASM_SOUNDS
};

//
// TODO: rm
// Sprites
//
typedef struct wolfasm_sprite {
  SDL_Texture *texture;
  SDL_Rect *sprite;
  int32_t width;
  int32_t height;
  uint16_t nb_sprites;
  uint16_t current_anim;
  int32_t trigger;
} wolfasm_sprite_t;
enum wolfasm_sprites {
  SPRITE_PISTOL = 0,
  SPRITE_SHOTGUN,
  SPRITE_BARREL,
  NB_WOLFASM_SPRITES
};
extern wolfasm_sprite_t
    wolfasm_sprite[NB_WOLFASM_SPRITES] __asm__("wolfasm_sprite");

//
// Weapons
//
enum wolfasm_weapon_type {
  WOLFASM_PISTOL = 0,
  WOLFASM_SHOTGUN,
  WOLFASM_BARREL,
  NB_WOLFASM_WEAPON
};

struct wolfasm_weapon_s {
  wolfasm_sprite_t *sprite;
  enum wolfasm_sounds sound;
  enum wolfasm_weapon_type type;
  int16_t ammo;
  int16_t const max_ammo;
  int32_t const damage;
};

//
// Items
//
enum wolfasm_item_type { ITEM_AMMO, ITEM_MEDIKIT, ITEM_ENEMY };

#if defined __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpacked"
#endif

struct __attribute__((packed)) wolfasm_item_s {
  int32_t texture;
  int32_t pos_x;
  int32_t pos_y;
  int32_t width_div;
  int32_t height_div;
  double height_move;

  int32_t current_anim;
  int32_t nb_anim;
  int32_t anim_rate;
  int32_t *texture_table;

  int32_t stock;
  enum wolfasm_item_type type;
  void (*callback)(void);
};

#if defined __clang__
#pragma clang diagnostic pop
#endif

_Static_assert(sizeof(struct wolfasm_item_s) == 64, "Invalid item size");

//
// Enemy
//
enum wolfasm_enemy_state {
  ENEMY_HIT,
  ENEMY_DIE,
  ENEMY_MOVE,
  ENEMY_SHOOT,
  ENEMY_STILL
};

struct wolfasm_enemy_s {
  struct wolfasm_item_s *item;
  int32_t life;
  enum wolfasm_enemy_state state;
};
