#pragma once

#include <SDL2/SDL.h>
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

// Handle game events
extern SDL_Event game_events;
SDL_Event game_events __asm__("game_events");

// Player informations
struct wolfasm_player {
  double pos_x, pos_y;
  double dir_x, dir_y;
  double plane_x, plane_y;
  double movement_speed;
  double rotation_speed;
};
extern struct wolfasm_player game_player __asm__("game_player");

// Map informations
extern int32_t const map_width __asm__("map_width");
extern int32_t const map_height __asm__("map_height");
extern uint8_t const map[] __asm__("map");

// Window informations
extern SDL_Window *window_ptr __asm__("window_ptr");
extern SDL_Surface *window_surface __asm__("window_surface");
extern SDL_Renderer *window_renderer __asm__("window_renderer");
extern int32_t window_width __asm__("window_width");
extern int32_t window_height __asm__("window_height");

//
// TODO: RM
// SOUND
//
enum wolfasm_audio_channel { SOUND_CHANNEL = 0 };

enum wolfasm_sounds { SOUND_PISTOL = 0, NB_WOLFASM_SOUNDS };
void play_sound(enum wolfasm_sounds sound);
