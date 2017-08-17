#include "cdefs.h"
#include <assert.h>
#include <dirent.h>
#include <time.h>

enum wolfasm_menus {
  MENU_MAIN,
  MENU_MULTIPLAYER,
  MENU_SELECT_MAP_SOLO,
  MENU_MULTIPLAYER_CONNECT,
  MENU_MULTIPLAYER_HOST,
  NB_MENUS
};

//
// Menu
//
extern enum wolfasm_menus menu __asm__("menu");
extern int32_t selected_button __asm__("selected_button");

extern int32_t
    selected_text_field_max_len __asm__("selected_text_field_max_len");
extern int32_t *selected_text_field_len __asm__("selected_text_field_len");
extern char *selected_text_field __asm__("selected_text_field");

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

extern void (**callbacks[])() __asm__("callbacks");

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
wolfasm_menu_events_cwrapper(void) __asm__("wolfasm_menu_events_cwrapper");
void wolfasm_menu_events_cwrapper(void) {
  SDL_Event event = {};

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
        (callbacks[menu][selected_button])();
        break;
      case SDLK_BACKSPACE:
        if (menu == MENU_MULTIPLAYER_CONNECT || menu == MENU_MULTIPLAYER_HOST) {
          assert(selected_text_field_len);
          if (*selected_text_field_len > 0) {
            selected_text_field[*selected_text_field_len - 1] = '\0';
            --(*selected_text_field_len);
          }
        }
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
      assert(selected_text_field_len);
      assert(selected_text_field);
      assert(menu == MENU_MULTIPLAYER_CONNECT || menu == MENU_MULTIPLAYER_HOST);
      {
        size_t const cur_len = strlen(event.text.text);
        size_t real_len = cur_len;
        if (*selected_text_field_len + cur_len >=
            selected_text_field_max_len - 1) {
          real_len = selected_text_field_max_len - 1 - *selected_text_field_len;
        }
        if (real_len) {
          strncpy(selected_text_field + *selected_text_field_len,
                  event.text.text, real_len);
        }
        *selected_text_field_len += real_len;
      }
      break;

    default:
      break;
    }
  }
}
