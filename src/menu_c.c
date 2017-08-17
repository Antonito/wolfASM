#include "cdefs.h"
#include <assert.h>
#include <dirent.h>
#include <time.h>

extern void render_sprite(wolfasm_sprite_t *sprite, int x, int y,
                          SDL_Rect *clip) __asm__("wolfasm_render_sprite");
extern void wolfasm(char const *map) __asm__("wolfasm");
extern void render_button(char const *text, int32_t const x, int32_t y,
                          int32_t width,
                          bool selected) __asm__("wolfasm_menu_render_button");
extern void wolfasm_menu_play_music(void) __asm__("wolfasm_menu_play_music");

enum wolfasm_menus {
  MENU_MAIN,
  MENU_MULTIPLAYER,
  MENU_SELECT_MAP_SOLO,
  MENU_MULTIPLAYER_CONNECT,
  NB_MENUS
};

enum wolfasm_buttons_main {
  BUTTON_PLAY = 0,
  BUTTON_MULTIPLAYER,
  BUTTON_EXIT,
  NB_BUTTON_MAIN
};

enum wolfasm_buttons_mutliplayer {
  BUTTON_MP_HOST = 0,
  BUTTON_MP_CONNECT,
  BUTTON_MP_BACK,
  NB_BUTTON_MULTIPLAYER
};

enum wolfasm_buttons_select_map_solo {
  BUTTON_SM_SOLO_MAP = 0,
  BUTTON_SM_SOLO_BACK,
  NB_BUTTON_SELECT_MAP_SOLO
};

enum wolfasm_buttons_mutliplayer_connect {
  BUTTON_MP_CO_CONNECT = 0,
  BUTTON_MP_CO_BACK,
  NB_BUTTON_MULTIPLAYER_CONNECT
};

//
// Menu
//
static bool running = true;
static enum wolfasm_menus menu = MENU_MAIN;
static int32_t wolfasm_menu_nb_buttons = NB_BUTTON_MAIN;
static int32_t selected_button = BUTTON_PLAY;

static char const *wolfasm_selected_map = NULL;
static char const *wolfasm_maps[255] = {0};
static int32_t wolfasm_current_map = 0;
static int32_t wolfasm_nb_maps = 0;

static char wolfasm_connect[255];
static int32_t wolfasm_connect_len = 0;
static char wolfasm_port[sizeof("65535")];
static int32_t wolfasm_port_len = 0;
static char *wolfasm_text_field[] = {wolfasm_connect, wolfasm_port};
static int32_t wolfasm_selected_text_field = 0;

static int32_t selected_text_field_max_len = 0;
static int32_t *selected_text_field_len = NULL;
static char *selected_text_field = NULL;

// Main Menu
static void wolfasm_load_maps() {
  DIR *d = opendir("./resources/map/");
  struct dirent *dir = NULL;

  wolfasm_nb_maps = 0;
  if (!d) {
    goto err;
  }
  while ((dir = readdir(d))) {
    if (dir->d_name[0] != '.') {
      if (wolfasm_nb_maps == 255) {
        goto err;
      }
      free(wolfasm_maps[wolfasm_nb_maps]);
      wolfasm_maps[wolfasm_nb_maps] = strdup(dir->d_name);
      if (!wolfasm_maps[wolfasm_nb_maps]) {
        goto err;
      }
      ++wolfasm_nb_maps;
    }
  }
  closedir(d);
  if (wolfasm_nb_maps) {
    wolfasm_current_map = 0;
    wolfasm_selected_map = wolfasm_maps[wolfasm_current_map];
  }
  return;

err:
  fprintf(stderr, "Cannot load maps\n");
  exit(1);
}

//
// Main menu
//
static void wolfasm_main_play() {
  menu = MENU_SELECT_MAP_SOLO;
  wolfasm_menu_nb_buttons = NB_BUTTON_SELECT_MAP_SOLO;
  selected_button = 0;
  wolfasm_load_maps();
}
static void wolfasm_main_multiplayer() {
  menu = MENU_MULTIPLAYER;
  wolfasm_menu_nb_buttons = NB_BUTTON_MULTIPLAYER;
  selected_button = 0;
}
static void wolfasm_main_exit() { running = false; }
static void wolfasm_main_buttons() {
  render_button("Play", window_width / 2, window_height / 2, window_width / 16,
                selected_button == BUTTON_PLAY);
  render_button("Multiplayer", window_width / 2,
                window_height / 2 + window_height / 8, window_width / 4,
                selected_button == BUTTON_MULTIPLAYER);
  render_button("Exit", window_width / 2,
                window_height / 2 + 2 * (window_height / 8), window_width / 16,
                selected_button == BUTTON_EXIT);
}

//
// Multiplayer
//
static void wolfasm_multiplayer_host() {}
static void wolfasm_multiplayer_connect() {
  menu = MENU_MULTIPLAYER_CONNECT;
  wolfasm_menu_nb_buttons = NB_BUTTON_MULTIPLAYER_CONNECT;
  selected_button = 0;
  wolfasm_selected_text_field = 0;
  wolfasm_connect_len = 0;
  wolfasm_port_len = 0;
  selected_text_field_len = &wolfasm_connect_len;
  selected_text_field_max_len = sizeof(wolfasm_connect);
  selected_text_field = wolfasm_connect;
  SDL_StartTextInput();
  SDL_SetTextInputRect((SDL_Rect[]){window_width / 2, window_height / 2, 100,
                                    window_width / 16});
}
static void wolfasm_multiplayer_back() {
  menu = MENU_MAIN;
  wolfasm_menu_nb_buttons = NB_BUTTON_MAIN;
  selected_button = 0;
}
static void wolfasm_multiplayer_buttons() {
  render_button("Host", window_width / 2, window_height / 2, window_width / 16,
                selected_button == BUTTON_MP_HOST);
  render_button("Connect", window_width / 2,
                window_height / 2 + window_height / 8, window_width / 8,
                selected_button == BUTTON_MP_CONNECT);
  render_button("Back", window_width / 2,
                window_height / 2 + 2 * (window_height / 8), window_width / 16,
                selected_button == BUTTON_MP_BACK);
}

//
// Multiplayer Connect
//
static void wolfasm_multiplayer_connect_connect() {
  uint16_t port = strtol(wolfasm_port, NULL, 10);
  printf("Going to connect to %s:%d\n", wolfasm_connect, port);
}
static void wolfasm_multiplayer_connect_back() {
  menu = MENU_MULTIPLAYER;
  wolfasm_menu_nb_buttons = NB_BUTTON_MULTIPLAYER;
  selected_button = 0;
  wolfasm_selected_text_field = 0;
  wolfasm_connect_len = 0;
  wolfasm_port_len = 0;
  selected_text_field_max_len = 0;
  selected_text_field_len = NULL;
  selected_text_field = NULL;
  memset(wolfasm_connect, '\0', sizeof(wolfasm_connect));
  memset(wolfasm_port, '\0', sizeof(wolfasm_port));
  SDL_StopTextInput();
}
static void wolfasm_multiplayer_connect_buttons() {
  render_button("Host: ", window_width / 2, window_height / 2,
                window_width / 16, 0);
  if (wolfasm_connect[0]) {
    render_button(wolfasm_connect, window_width / 2 + window_width / 16,
                  window_height / 2, strlen(wolfasm_connect) * 15, 0);
  }
  render_button("Port: ", window_width / 2,
                window_height / 2 + window_height / 8, window_width / 16, 0);
  if (wolfasm_port[0]) {
    render_button(wolfasm_port, window_width / 2 + +window_width / 16,
                  window_height / 2 + window_height / 8,
                  strlen(wolfasm_port) * 15, 0);
  }
  render_button("Connect", window_width / 2,
                window_height / 2 + 2 * (window_height / 8), window_width / 8,
                selected_button == BUTTON_MP_CO_CONNECT);
  render_button("Back", window_width / 2,
                window_height / 2 + 3 * (window_height / 8), window_width / 16,
                selected_button == BUTTON_MP_CO_BACK);
}

//
// Multiplayer host
//

//
// Select Map Solo
//
static void wolfasm_sm_solo_map() {
  char filename[512];

  strncpy(filename, "./resources/map/", sizeof(filename) - 1);
  strncat(filename, wolfasm_selected_map, sizeof(filename) - 1);
  Mix_HaltMusic();
  wolfasm(filename);
  wolfasm_menu_play_music();
}
static void wolfasm_sm_solo_back() {
  menu = MENU_MAIN;
  wolfasm_menu_nb_buttons = NB_BUTTON_MAIN;
  selected_button = 0;
}

static void wolfasm_sm_solo_buttons() {
  char buff[255 + 1 + 4];
  snprintf(buff, sizeof(buff), "< %s >", wolfasm_selected_map);
  render_button(buff, window_width / 2, window_height / 2, window_width / 8,
                selected_button == BUTTON_SM_SOLO_MAP);
  render_button("Back", window_width / 2,
                window_height / 2 + 1 * (window_height / 8), window_width / 8,
                selected_button == BUTTON_SM_SOLO_BACK);
}

static void (*callbacks_main_menu[])() = {
    wolfasm_main_play, wolfasm_main_multiplayer, wolfasm_main_exit,
    wolfasm_main_buttons};
static void (*callbacks_multiplayer_menu[])() = {
    wolfasm_multiplayer_host, wolfasm_multiplayer_connect,
    wolfasm_multiplayer_back, wolfasm_multiplayer_buttons};
static void (*callback_sm_solo[])() = {
    wolfasm_sm_solo_map, wolfasm_sm_solo_back, wolfasm_sm_solo_buttons};
static void (*callback_mp_co[])() = {wolfasm_multiplayer_connect_connect,
                                     wolfasm_multiplayer_connect_back,
                                     wolfasm_multiplayer_connect_buttons};

static void (**callbacks[])() = {&callbacks_main_menu,
                                 &callbacks_multiplayer_menu, &callback_sm_solo,
                                 &callback_mp_co};

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
          if (menu == MENU_SELECT_MAP_SOLO) {
            --wolfasm_current_map;
            if (wolfasm_current_map < 0) {
              assert(wolfasm_nb_maps != 0);
              wolfasm_current_map = wolfasm_nb_maps - 1;
            }
            wolfasm_selected_map = wolfasm_maps[wolfasm_current_map];
          }
          break;
        case SDLK_RIGHT:
          if (menu == MENU_SELECT_MAP_SOLO) {
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
          if (menu == MENU_MULTIPLAYER_CONNECT) {
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
        assert(menu == MENU_MULTIPLAYER_CONNECT); // TODO: Update for Host menu
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
    wolfasm_regulate_framerate(60);
  }
  return EXIT_SUCCESS;
}
