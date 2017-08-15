#include "cdefs.h"
#include <time.h>

extern void render_sprite(wolfasm_sprite_t *sprite, int x, int y,
                          SDL_Rect *clip) __asm__("wolfasm_render_sprite");
extern void wolfasm(void) __asm__("wolfasm");
extern void render_button(char const *text, int32_t const x, int32_t y,
                          int32_t width,
                          bool selected) __asm__("wolfasm_menu_render_button");
extern void wolfasm_menu_play_music(void) __asm__("wolfasm_menu_play_music");

enum wolfasm_menus { MENU_MAIN, MENU_MULTIPLAYER, NB_MENUS };

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

//
// Menu
//
static bool running = true;
static enum wolfasm_menus menu = MENU_MAIN;
static int32_t wolfasm_menu_nb_buttons = NB_BUTTON_MAIN;
static int32_t selected_button = BUTTON_PLAY;

// Main Menu
static void wolfasm_main_play() {
  Mix_HaltMusic();
  wolfasm();
  wolfasm_menu_play_music();
  SDL_Delay(120);
}
static void wolfasm_main_multiplayer() {
  menu = MENU_MULTIPLAYER;
  wolfasm_menu_nb_buttons = NB_BUTTON_MULTIPLAYER;
  selected_button = 0;
  SDL_Delay(120);
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

// Multiplayer
static void wolfasm_multiplayer_host() { SDL_Delay(120); }
static void wolfasm_multiplayer_connect() { SDL_Delay(120); }
static void wolfasm_multiplayer_back() {
  menu = MENU_MAIN;
  wolfasm_menu_nb_buttons = NB_BUTTON_MAIN;
  selected_button = 0;
  SDL_Delay(120);
}
static void wolfasm_multiplayer_buttons() {
  render_button("Host", window_width / 2, window_height / 2, window_width / 16,
                selected_button == BUTTON_PLAY);
  render_button("Connect", window_width / 2,
                window_height / 2 + window_height / 8, window_width / 8,
                selected_button == BUTTON_MULTIPLAYER);
  render_button("Back", window_width / 2,
                window_height / 2 + 2 * (window_height / 8), window_width / 16,
                selected_button == BUTTON_EXIT);
}

static void (*callbacks_main_menu[])() = {
    wolfasm_main_play, wolfasm_main_multiplayer, wolfasm_main_exit,
    wolfasm_main_buttons};
static void (*callbacks_multiplayer_menu[])() = {
    wolfasm_multiplayer_host, wolfasm_multiplayer_connect,
    wolfasm_multiplayer_back, wolfasm_multiplayer_buttons};

static void (**callbacks[])() = {&callbacks_main_menu,
                                 &callbacks_multiplayer_menu};

int wolfasm_menu(void) {
  srand((unsigned int)time(NULL));
  wolfasm_menu_play_music();
  while (running) {
    SDL_Event event;

    // Handle events
    if (!SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        running = false;
        break;

      case SDL_KEYUP:
        switch (event.key.keysym.sym) {

        // Menu controls
        case SDLK_LEFT:
          break;
        case SDLK_RIGHT:
          break;
        case SDLK_UP:
          SDL_Delay(10);
          break;
        case SDLK_DOWN:
          SDL_Delay(10);
          break;
        case SDLK_RETURN:
          break;
        }
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {

        // Menu controls
        case SDLK_LEFT:
          break;
        case SDLK_RIGHT:
          break;
        case SDLK_UP:
          if (selected_button == 0) {
            selected_button = wolfasm_menu_nb_buttons - 1;
          } else {
            --selected_button;
          }
          SDL_Delay(120);
          break;
        case SDLK_DOWN:
          ++selected_button;
          if (selected_button == wolfasm_menu_nb_buttons) {
            selected_button = 0;
          }
          SDL_Delay(120);
          break;
        case SDLK_RETURN:
          (callbacks[menu][selected_button])();
          break;

        case SDLK_ESCAPE:
          running = false;
          break;

        default:
          break;
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
