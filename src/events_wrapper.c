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
  switch (events->button.button) {
  default:
    break;
  }
}

// C-Wrapper for SDL_MOUSEMOTION events
void wolfasm_events_mouse_motion_cwrapper(SDL_Event const *events) {
  assert(events == &game_events);
  assert(events->type == SDL_MOUSEMOTION);
  int32_t mouse_x = events->motion.xrel;

  double const tmp_speed = game_player.rotation_speed;
  game_player.rotation_speed = 0.015; // Decrease speed
  if (mouse_x > 0) {
    // Mouse moved right
    while (mouse_x) {
      wolfasm_player_rotate_right();
      --mouse_x;
    }
  } else if (mouse_x < 0) {
    // Mouse moved left
    mouse_x = -mouse_x;
    while (mouse_x) {
      wolfasm_player_rotate_left();
      --mouse_x;
    }
  }
  game_player.rotation_speed = tmp_speed; // Restore speed
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
