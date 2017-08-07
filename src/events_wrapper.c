#include <SDL2/SDL.h>
#include <assert.h>
#include <stdint.h>

// ASM Bindings
void wolfasm_player_move_forward(void) __asm__("wolfasm_player_move_forward");
void wolfasm_player_move_backward(void) __asm__("wolfasm_player_move_backward");
double wolfasm_player_rotate_right(void) __asm__("wolfasm_player_rotate_right");
void wolfasm_player_rotate_left(void) __asm__("wolfasm_player_rotate_left");

// Prototypes
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

void wolfasm_raycast_pix_crwapper(int32_t x) __asm__(
    "wolfasm_raycast_pix_crwapper"); // TODO: rm

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
  int32_t mouse_x = events->motion.x;
  int32_t mouse_y = events->motion.y;
  (void)mouse_x, (void)mouse_y;
}
