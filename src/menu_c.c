#include "cdefs.h"
#include <assert.h>
#include <dirent.h>
#include <time.h>

extern void render_sprite(wolfasm_sprite_t *sprite, int x, int y,
                          SDL_Rect *clip) __asm__("wolfasm_render_sprite");
extern void wolfasm_menu_play_music(void) __asm__("wolfasm_menu_play_music");
extern void
wolfasm_regulate_framerate(void) __asm__("wolfasm_regulate_framerate");

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
extern bool running __asm__("running");
extern enum wolfasm_menus menu __asm__("menu");
extern int32_t wolfasm_menu_nb_buttons __asm__("wolfasm_menu_nb_buttons");
extern int32_t selected_button __asm__("selected_button");

extern char const *wolfasm_selected_map __asm__("wolfasm_selected_map");
extern char const *wolfasm_maps[255] __asm__("wolfasm_maps");
extern int32_t wolfasm_current_map __asm__("wolfasm_current_map");
extern int32_t wolfasm_nb_maps __asm__("wolfasm_nb_maps");

extern char wolfasm_connect[255] __asm__("wolfasm_connect");
extern int32_t wolfasm_connect_len __asm__("wolfasm_connect_len");

extern char wolfasm_port[sizeof("65535")] __asm__("wolfasm_port");
extern int32_t wolfasm_port_len __asm__("wolfasm_port_len");

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

// Main Menu
extern void (**callbacks[])() __asm__("callbacks");

int wolfasm_menu(void) {
  srand((unsigned int)time(NULL));
  wolfasm_menu_play_music();
  while (running) {
    SDL_Event event = {};

    // Handle events
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        running = false;
        break;

      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {

        // Menu controls
        case SDLK_LEFT:
          if (menu == MENU_SELECT_MAP_SOLO || menu == MENU_MULTIPLAYER_HOST) {
            --wolfasm_current_map;
            if (wolfasm_current_map < 0) {
              assert(wolfasm_nb_maps != 0);
              wolfasm_current_map = wolfasm_nb_maps - 1;
            }
            wolfasm_selected_map = wolfasm_maps[wolfasm_current_map];
          }
          break;
        case SDLK_RIGHT:
          if (menu == MENU_SELECT_MAP_SOLO || menu == MENU_MULTIPLAYER_HOST) {
            ++wolfasm_current_map;
            if (wolfasm_current_map == wolfasm_nb_maps) {
              wolfasm_current_map = 0;
            }
            wolfasm_selected_map = wolfasm_maps[wolfasm_current_map];
          }
          break;
        case SDLK_UP:
          if (selected_button == 0) {
            selected_button = wolfasm_menu_nb_buttons - 1;
          } else {
            --selected_button;
          }
          break;
        case SDLK_DOWN:
          ++selected_button;
          if (selected_button == wolfasm_menu_nb_buttons) {
            selected_button = 0;
          }
          break;
        case SDLK_RETURN:
          (callbacks[menu][selected_button])();
          break;
        case SDLK_BACKSPACE:
          if (menu == MENU_MULTIPLAYER_CONNECT ||
              menu == MENU_MULTIPLAYER_HOST) {
            assert(selected_text_field_len);
            if (*selected_text_field_len > 0) {
              selected_text_field[*selected_text_field_len - 1] = '\0';
              --(*selected_text_field_len);
            }
          }
          break;
        case SDLK_TAB:
          if (menu == MENU_MULTIPLAYER_CONNECT) {
            if (selected_text_field == wolfasm_connect) {
              selected_text_field = wolfasm_port;
              selected_text_field_max_len = sizeof(wolfasm_port);
              selected_text_field_len = &wolfasm_port_len;
            } else {
              selected_text_field = wolfasm_connect;
              selected_text_field_max_len = sizeof(wolfasm_connect);
              selected_text_field_len = &wolfasm_connect_len;
            }
          }
          break;

        case SDLK_ESCAPE:
          running = false;
          break;

        default:
          break;
        }
        break;

      case SDL_TEXTINPUT:
        assert(selected_text_field_len);
        assert(selected_text_field);
        assert(menu == MENU_MULTIPLAYER_CONNECT ||
               menu == MENU_MULTIPLAYER_HOST);
        {
          size_t const cur_len = strlen(event.text.text);
          size_t real_len = cur_len;
          if (*selected_text_field_len + cur_len >=
              selected_text_field_max_len - 1) {
            real_len =
                selected_text_field_max_len - 1 - *selected_text_field_len;
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

    // Display  Menu
    SDL_RenderClear(window_renderer);
    render_sprite(&wolfasm_sprite[3], 0, 0, NULL);

    // Render buttons
    (callbacks[menu][wolfasm_menu_nb_buttons])();

    SDL_RenderPresent(window_renderer);
    wolfasm_regulate_framerate();
  }
  return EXIT_SUCCESS;
}
