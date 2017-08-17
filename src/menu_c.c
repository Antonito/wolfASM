#include "cdefs.h"
#include <dirent.h>

// Wrapper for struct dirent
extern char const *wolfasm_read_dir(DIR *d) __asm__("wolfasm_read_dir");
char const *wolfasm_read_dir(DIR *d) {
  static struct dirent *dir = NULL;
  dir = readdir(d);
  if (dir) {
    return dir->d_name;
  }
  return NULL;
}

//
// Menu events
//
extern void wolfasm_menu_event_quit(void) __asm__("wolfasm_menu_event_quit");
extern void
wolfasm_menu_event_key_down(void) __asm__("wolfasm_menu_event_key_down");
extern void
wolfasm_menu_event_key_up(void) __asm__("wolfasm_menu_event_key_up");
extern void
wolfasm_menu_event_key_left(void) __asm__("wolfasm_menu_event_key_left");
extern void
wolfasm_menu_event_key_right(void) __asm__("wolfasm_menu_event_key_right");
extern void
wolfasm_menu_event_key_tab(void) __asm__("wolfasm_menu_event_key_tab");
extern void
wolfasm_menu_event_key_enter(void) __asm__("wolfasm_menu_event_key_enter");
extern void wolfasm_menu_event_key_backspace(void) __asm__(
    "wolfasm_menu_event_key_backspace");
extern void wolfasm_menu_event_textinput(char const *) __asm__(
    "wolfasm_menu_event_textinput");

extern void
wolfasm_menu_events_cwrapper(void) __asm__("wolfasm_menu_events_cwrapper");
void wolfasm_menu_events_cwrapper(void) {
  SDL_Event event;

  // Handle events
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      wolfasm_menu_event_quit();
      break;

    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      // Menu controls
      case SDLK_LEFT:
        wolfasm_menu_event_key_left();
        break;
      case SDLK_RIGHT:
        wolfasm_menu_event_key_right();
        break;
      case SDLK_UP:
        wolfasm_menu_event_key_up();
        break;
      case SDLK_DOWN:
        wolfasm_menu_event_key_down();
        break;
      case SDLK_RETURN:
        wolfasm_menu_event_key_enter();
        break;
      case SDLK_BACKSPACE:
        wolfasm_menu_event_key_backspace();
        break;
      case SDLK_TAB:
        wolfasm_menu_event_key_tab();
        break;
      case SDLK_ESCAPE:
        wolfasm_menu_event_quit();
        break;
      default:
        break;
      }
      break;

    case SDL_TEXTINPUT:
      wolfasm_menu_event_textinput(event.text.text);
      break;

    default:
      break;
    }
  }
}
