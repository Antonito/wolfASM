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
