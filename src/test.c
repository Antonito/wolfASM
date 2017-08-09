// This files permits me to try things in C, before implementing them in
// assembly

#include "cdefs.h"
#include <SDL2/SDL_Image.h>
#include <assert.h>

//
// Prototypes
//
void c_deinit(void);
void c_init(void);
void init_textures(void);

void init_sprites(void);
void deinit_sprites(void);
void display_sprites_cwrapper(void);

void init_weapons(void);

void c_init() { init_sprites(); }

void c_deinit() { deinit_sprites(); }

//
// Sprites
//
static char const *wolfasm_sprite_file[] = {"./resources/sprites/pistol.png",
                                            "./resources/sprites/shotgun.png",
                                            "./resources/sprites/barell.png"};
_Static_assert(sizeof(wolfasm_sprite_file) / sizeof(wolfasm_sprite_file[0]) ==
                   NB_WOLFASM_SPRITES,
               "Invalid sprites number");

wolfasm_sprite_t wolfasm_sprite[NB_WOLFASM_SPRITES] __asm__("wolfasm_sprite");

extern SDL_Texture *window_texture;
SDL_Texture *window_texture __asm__("window_texture");

void init_sprites(void) {
  printf("Loading sprites..\n");
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
  window_texture = SDL_CreateTexture(window_renderer, SDL_PIXELFORMAT_BGRA32,
                                     SDL_TEXTUREACCESS_STREAMING, window_width,
                                     window_height);
  if (!window_texture) {
    goto err;
  }

  for (int32_t i = 0; i < NB_WOLFASM_SPRITES; ++i) {
    memset(&wolfasm_sprite[i], 0, sizeof(wolfasm_sprite[i]));
    SDL_Surface *surface = IMG_Load(wolfasm_sprite_file[i]);
    if (!surface) {
      goto err;
    }
    SDL_SetColorKey(surface, 1, SDL_MapRGB(surface->format, 0, 0xFF, 0xFF));
    wolfasm_sprite[i].texture =
        SDL_CreateTextureFromSurface(window_renderer, surface);
    wolfasm_sprite[i].width = surface->w;
    wolfasm_sprite[i].height = surface->h;
    SDL_FreeSurface(surface);
    if (!wolfasm_sprite[i].texture) {
      goto err;
    }
  }
  printf("Sprites loaded..\n");

  // Pistol
  wolfasm_sprite[SPRITE_PISTOL].height = 62 * 4;
  wolfasm_sprite[SPRITE_PISTOL].width = 62 * 4;
  wolfasm_sprite[SPRITE_PISTOL].nb_sprites = 5;

  wolfasm_sprite[SPRITE_PISTOL].sprite =
      malloc(sizeof(*wolfasm_sprite[SPRITE_PISTOL].sprite) *
             wolfasm_sprite[SPRITE_PISTOL].nb_sprites);
  if (!wolfasm_sprite[SPRITE_PISTOL].sprite) {
    goto err;
  }
  SDL_Rect *sprite = wolfasm_sprite[SPRITE_PISTOL].sprite;

  sprite[0].x = 0;
  sprite[0].y = 42;
  sprite[0].w = 60;
  sprite[0].h = 62;

  sprite[1].x = 60;
  sprite[1].y = 22;
  sprite[1].w = 82;
  sprite[1].h = 82;

  sprite[2].x = 142;
  sprite[2].y = 23;
  sprite[2].w = 70;
  sprite[2].h = 81;

  sprite[3].x = 212;
  sprite[3].y = 23;
  sprite[3].w = 64;
  sprite[3].h = 81;

  sprite[4].x = 276;
  sprite[4].y = 1;
  sprite[4].w = 80;
  sprite[4].h = 103;

  // Shotgun
  wolfasm_sprite[SPRITE_SHOTGUN].height = 62 * 4;
  wolfasm_sprite[SPRITE_SHOTGUN].width = 62 * 4;
  wolfasm_sprite[SPRITE_SHOTGUN].nb_sprites = 4;
  wolfasm_sprite[SPRITE_SHOTGUN].sprite =
      malloc(sizeof(*wolfasm_sprite[SPRITE_SHOTGUN].sprite) *
             wolfasm_sprite[SPRITE_SHOTGUN].nb_sprites);
  if (!wolfasm_sprite[SPRITE_SHOTGUN].sprite) {
    goto err;
  }
  sprite = wolfasm_sprite[SPRITE_SHOTGUN].sprite;

  sprite[0].x = 0;
  sprite[0].y = 91;
  sprite[0].w = 79;
  sprite[0].h = 60;

  sprite[1].x = 82;
  sprite[1].y = 30;
  sprite[1].w = 119;
  sprite[1].h = 121;

  sprite[2].x = 204;
  sprite[2].y = 0;
  sprite[2].w = 87;
  sprite[2].h = 151;

  sprite[3].x = 294;
  sprite[3].y = 20;
  sprite[3].w = 113;
  sprite[3].h = 130;

  // Barrel
  wolfasm_sprite[SPRITE_BARREL].height = 62 * 4;
  wolfasm_sprite[SPRITE_BARREL].width = 62 * 4;
  wolfasm_sprite[SPRITE_BARREL].nb_sprites = 4;
  wolfasm_sprite[SPRITE_BARREL].sprite =
      malloc(sizeof(*wolfasm_sprite[SPRITE_BARREL].sprite) *
             wolfasm_sprite[SPRITE_BARREL].nb_sprites);
  if (!wolfasm_sprite[SPRITE_BARREL].sprite) {
    goto err;
  }
  sprite = wolfasm_sprite[SPRITE_BARREL].sprite;

  sprite[0].x = 0;
  sprite[0].y = 75;
  sprite[0].w = 59;
  sprite[0].h = 55;

  sprite[1].x = 62;
  sprite[1].y = 27;
  sprite[1].w = 83;
  sprite[1].h = 102;

  sprite[2].x = 148;
  sprite[2].y = 0;
  sprite[2].w = 121;
  sprite[2].h = 130;

  sprite[3].x = 272;
  sprite[3].y = 50;
  sprite[3].w = 81;
  sprite[3].h = 80;

  return;
err:
  printf("%s\n", SDL_GetError());
  exit(1);
}

void deinit_sprites(void) {
  for (int32_t i = 0; i < NB_WOLFASM_SPRITES; ++i) {
    SDL_DestroyTexture(wolfasm_sprite[i].texture);
    free(wolfasm_sprite[i].sprite);
    wolfasm_sprite[i].sprite = NULL;
  }
}

static void render_sprite(wolfasm_sprite_t *sprite, int x, int y,
                          SDL_Rect *clip) {
  SDL_Rect renderQuad = {x, y, sprite->width, sprite->height};
  SDL_RenderCopy(window_renderer, sprite->texture, clip, &renderQuad);
}

void display_sprites_cwrapper(void) {
  if (!game_player.weapon) {
    return;
  }
  wolfasm_sprite_t *weapon_sprite = game_player.weapon->sprite;

  render_sprite(weapon_sprite, window_width / 2 - 30 * 4,
                window_height - weapon_sprite->height,
                &weapon_sprite->sprite[weapon_sprite->current_anim / 8]);

  if (weapon_sprite->trigger) {
    ++weapon_sprite->current_anim;
    if (weapon_sprite->current_anim == weapon_sprite->nb_sprites * 8) {
      weapon_sprite->current_anim = 0;
      weapon_sprite->trigger = 0;
    }
  }
}
