#include "cdefs.h"
#include <assert.h>

// C-Wrapper for SDL_KEYUP events
void wolfasm_events_keyboard_up_cwrapper(SDL_Event const *events) {
  assert(events == &game_events);
  assert(events->type == SDL_KEYUP);
  switch (events->key.keysym.sym) {
  default:
    break;
  }
}

// C-Wrapper for SDL_KEYDOWN events
void wolfasm_events_keyboard_down_cwrapper(SDL_Event const *events) {
  extern int game_running __asm__("game_running");
  assert(events == &game_events);
  assert(events->type == SDL_KEYDOWN);
  switch (events->key.keysym.sym) {
  case SDLK_ESCAPE:
    game_running = 0;
    break;

  // Move forward if no wall in front of you
  case SDLK_w:
    wolfasm_player_move_forward();
    break;

  // Move backwards if no wall behind
  case SDLK_s:
    wolfasm_player_move_backward();
    break;

  // Rotate right
  case SDLK_d:
    wolfasm_player_rotate_right();
    break;

  // Rotate left
  case SDLK_a:
    wolfasm_player_rotate_left();
    break;

  //
  // Change weapons
  //
  case SDLK_1:
    wolfasm_change_weapon(WOLFASM_PISTOL);
    break;

  case SDLK_2:
    wolfasm_change_weapon(WOLFASM_SHOTGUN);
    break;

  case SDLK_3:
    wolfasm_change_weapon(WOLFASM_BARREL);
    break;

  default:
    break;
  }
}

// C-Wrapper for SDL_MOUSEBUTTONUP events
void wolfasm_events_mouse_up_cwrapper(SDL_Event const *events) {
  assert(events == &game_events);
  assert(events->type == SDL_MOUSEBUTTONUP);
  switch (events->button.button) {
  default:
    break;
  }
}

// C-Wrapper for SDL_MOUSEBUTTONDOWN events
void wolfasm_events_mouse_down_cwrapper(SDL_Event const *events) {
  assert(events == &game_events);
  assert(events->type == SDL_MOUSEBUTTONDOWN);
  extern void wolfasm_play_sound(int) __asm__("wolfasm_play_sound");

  if (!game_player.weapon) {
    return;
  }

  switch (events->button.button) {
  case SDL_BUTTON_LEFT:
    if (game_player.weapon->sprite->trigger == 0) {

      // Check that enough ammo
      if (game_player.weapon->ammo) {
        wolfasm_play_sound(game_player.weapon->sound);
        --game_player.weapon->ammo;
      } else {
        wolfasm_play_sound(SOUND_NO_AMMO);
      }

      game_player.weapon->sprite->trigger = 1;
    }
    break;
  default:
    break;
  }
}

// C-Wrapper for SDL_MOUSEMOTION events
void wolfasm_events_mouse_motion_cwrapper(SDL_Event const *events) {
  extern void wolfasm_event_mouse_motion_handle(int32_t rel_x) __asm__(
      "wolfasm_event_mouse_motion_handle");

  assert(events == &game_events);
  assert(events->type == SDL_MOUSEMOTION);

  wolfasm_event_mouse_motion_handle(events->motion.xrel);
}

// C-Wrapper for SDL_WINDOWEVENT events
void wolfasm_event_window_cwrapper(SDL_Event const *events) {
  assert(events == &game_events);
  assert(events->type == SDL_WINDOWEVENT);
  switch (events->window.event) {

  // Handle resize of window
  // TODO: Fix, seems not to work on OSX. Is it working on Linux ?
  case SDL_WINDOWEVENT_RESIZED: {
    extern SDL_Window *window_ptr __asm__("window_ptr");
    extern int32_t window_width __asm__("window_width");
    extern int32_t window_height __asm__("window_height");

    printf("Window resized to %d x %d\n", events->window.data1,
           events->window.data2);
    window_width = events->window.data1;
    window_height = events->window.data2;
    SDL_SetWindowSize(window_ptr, window_width, window_height);
  } break;
  default:
    break;
  }
}
