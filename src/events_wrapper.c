#include <SDL2/SDL.h>
#include <assert.h>
#include <stdint.h>

#define MAP_WIDTH 640
#define MAP_HEIGHT 480

// ASM Bindings
void wolfasm_player_move_forward(void) __asm__("wolfasm_player_move_forward");
void wolfasm_player_move_backward(void) __asm__("wolfasm_player_move_backward");
void wolfasm_player_rotate_right(void) __asm__("wolfasm_player_rotate_right");
void wolfasm_player_rotate_left(void) __asm__("wolfasm_player_rotate_left");

void wolfasm_put_pixel(int32_t x, int32_t y,
                       int32_t argb) __asm__("wolfasm_put_pixel"); // TODO: rm

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

void wolfasm_raycast_pix_crwapper(void) __asm__(
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
  case SDLK_d: {
    // Update direction
    wolfasm_player_rotate_right();
    double const old_dir_x = game_player.dir_x;
    game_player.dir_x = game_player.dir_x * cos(-game_player.rotation_speed) -
                        game_player.dir_y * sin(-game_player.rotation_speed);
    game_player.dir_y = old_dir_x * sin(-game_player.rotation_speed) +
                        game_player.dir_y * cos(-game_player.rotation_speed);

    // Update camera plan
    double const old_plane_x = game_player.plane_x;
    game_player.plane_x =
        game_player.plane_x * cos(-game_player.rotation_speed) -
        game_player.plane_y * sin(-game_player.rotation_speed);
    game_player.plane_y =
        old_plane_x * sin(-game_player.rotation_speed) +
        game_player.plane_y * cos(-game_player.rotation_speed);
  } break;

  // Rotate left
  case SDLK_a: {
    // Update direction
    wolfasm_player_rotate_left();
    double const old_dir_x = game_player.dir_x;
    game_player.dir_x = game_player.dir_x * cos(game_player.rotation_speed) -
                        game_player.dir_y * sin(game_player.rotation_speed);
    game_player.dir_y = old_dir_x * sin(game_player.rotation_speed) +
                        game_player.dir_y * cos(game_player.rotation_speed);

    // Update camera plan
    double const old_plane_x = game_player.plane_x;
    game_player.plane_x =
        game_player.plane_x * cos(game_player.rotation_speed) -
        game_player.plane_y * sin(game_player.rotation_speed);
    game_player.plane_y = old_plane_x * sin(game_player.rotation_speed) +
                          game_player.plane_y * cos(game_player.rotation_speed);
  } break;
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

// C raycasting, for testing purpose
// Taken from: http://lodev.org/cgtutor/raycasting.html
void wolfasm_raycast_pix_crwapper(void) {
  for (int32_t x = 0; x < MAP_WIDTH; ++x) {
    double cameraX =
        2.0 * x / (double)(MAP_WIDTH)-1.0; // x-coordinate in camera space
    double rayPosX = game_player.pos_x;
    double rayPosY = game_player.pos_y;
    double rayDirX = game_player.dir_x + game_player.plane_x * cameraX;
    double rayDirY = game_player.dir_y + game_player.plane_y * cameraX;

    // which box of the map we're in
    int mapX = (int)rayPosX;
    int mapY = (int)rayPosY;

    // length of ray from current position to next x or y-side
    double sideDistX;
    double sideDistY;

    // length of ray from one x or y-side to next x or y-side
    double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
    double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
    double perpWallDist;

    // what direction to step in x or y-direction (either +1 or -1)
    int stepX;
    int stepY;

    int hit = 0;  // was there a wall hit?
    int side = 0; // was a NS or a EW wall hit?

    // calculate step and initial sideDist
    if (rayDirX < 0) {
      stepX = -1;
      sideDistX = (rayPosX - mapX) * deltaDistX;
    } else {
      stepX = 1;
      sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
    }
    if (rayDirY < 0) {
      stepY = -1;
      sideDistY = (rayPosY - mapY) * deltaDistY;
    } else {
      stepY = 1;
      sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
    }

    while (hit == 0) {
      // jump to next map square, OR in x-direction, OR in y-direction
      if (sideDistX < sideDistY) {
        sideDistX += deltaDistX;
        mapX += stepX;
        side = 0;
      } else {
        sideDistY += deltaDistY;
        mapY += stepY;
        side = 1;
      }
      // Check if ray has hit a wall
      if (map[mapY * map_width + mapX] > 0)
        hit = 1;
    }

    // Calculate distance projected on camera direction (oblique distance will
    // give fisheye effect!)
    if (side == 0)
      perpWallDist = (mapX - rayPosX + (1 - stepX) / 2) / rayDirX;
    else
      perpWallDist = (mapY - rayPosY + (1 - stepY) / 2) / rayDirY;

    // Calculate height of line to draw on screen
    int h = 480;
    int lineHeight = (int)(h / perpWallDist);

    // calculate lowest and highest pixel to fill in current stripe
    int drawStart = -lineHeight / 2 + h / 2;
    if (drawStart < 0)
      drawStart = 0;
    int drawEnd = lineHeight / 2 + h / 2;
    if (drawEnd >= h)
      drawEnd = h - 1;

    // choose wall color
    int32_t color;
    assert(mapY * map_width + mapX < map_width * map_height);
    switch (map[mapY * map_width + mapX]) {
    case 1:
      color = 0xC81543;
      break; // red
    case 2:
      color = 0xA8E4B1;
      break; // green
    case 3:
      color = 0x4AA3BA;
      break; // blue
    case 4:
      color = 0xFFE9EC;
      break; // white
    default:
      color = 0xF1E694;
      break; // yellow
    }

    // give x and y sides different brightness
    if (side == 1) {
      color = color / 2;
    }

    for (int32_t i = drawStart; i < drawEnd; ++i) {
      wolfasm_put_pixel(x, i, color);
    }
  }
}
