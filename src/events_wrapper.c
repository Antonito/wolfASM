#include <SDL2/SDL.h>
#include <assert.h>

// Prototypes
void wolfasm_events_keyboard_up_cwrapper(SDL_Event const *events) __asm__(
    "wolfasm_events_keyboard_up_cwrapper");
void wolfasm_events_keyboard_down_cwrapper(SDL_Event const *events) __asm__(
    "wolfasm_events_keyboard_down_cwrapper");

// Handle game events
SDL_Event game_events __asm__("game_events");

void wolfasm_events_keyboard_up_cwrapper(SDL_Event const *events) {
  assert(events == &game_events);
  assert(events->type == SDL_KEYUP);
  switch (events->key.keysym.sym) {
  default:
    break;
  }
}

void wolfasm_events_keyboard_down_cwrapper(SDL_Event const *events) {
  extern int game_running __asm__("game_running");
  assert(events == &game_events);
  assert(events->type == SDL_KEYDOWN);
  switch (events->key.keysym.sym) {
  case SDLK_ESCAPE:
    game_running = 0;
    break;
  default:
    break;
  }
}
