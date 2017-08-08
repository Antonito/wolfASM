// This files permits me to try things in C, before implementing them in
// assembly

#include "cdefs.h"
#include <SDL2/SDL_Image.h>
#include <SDL2/SDL_Mixer.h>
#include <assert.h>

enum wolfasm_weapon_type { WOLFASM_PISTOL };

typedef struct wolfasm_weapon_s {
  enum wolfasm_weapon_type type;
  Mix_Chunk *sound;
} wolfasm_weapon_t;

void c_deinit(void);
void c_init(void);
void init_textures(void);

void init_sprites(void);
void deinit_sprites(void);
void display_sprites_cwrapper(void);

void init_sound(void);
void deinit_sound(void);

void c_init() {
  init_sprites();
  init_sound();
}

void c_deinit() {
  deinit_sprites();
  deinit_sound();
}

//
// Sound
//
static Mix_Chunk *wolfasm_sounds[NB_WOLFASM_SOUNDS];

static char const *wolfasm_sound_file[] = {"./resources/sounds/pistol.wav"};

_Static_assert(sizeof(wolfasm_sound_file) / sizeof(wolfasm_sound_file[0]) ==
                   NB_WOLFASM_SOUNDS,
               "Invalid number of sounds");

void play_sound(enum wolfasm_sounds sound) {
  assert(sound >= SOUND_PISTOL && sound < NB_WOLFASM_SOUNDS);
  int32_t rc = Mix_PlayChannel(SOUND_CHANNEL, wolfasm_sounds[sound], 0);

  if (rc == -1) {
    printf("%s\n", SDL_GetError());
    exit(1);
  }
}

void init_sound(void) {
  printf("Loading sounds..\n");
  int32_t rc = Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

  if (rc == -1) {
    goto err;
  }

  for (int32_t i = 0; i < NB_WOLFASM_SOUNDS; ++i) {
    wolfasm_sounds[i] = Mix_LoadWAV(wolfasm_sound_file[i]);
    if (!wolfasm_sounds[i]) {
      goto err;
    }
  }

  printf("Sounds loaded\n");
  return;
err:
  printf("%s\n", SDL_GetError());
  exit(1);
}

void deinit_sound(void) {
  for (int32_t i = 0; i < NB_WOLFASM_SOUNDS; ++i) {
    Mix_FreeChunk(wolfasm_sounds[i]);
    wolfasm_sounds[i] = NULL;
  }
  Mix_CloseAudio();
}

//
// Sprites
//
typedef struct wolfasm_sprite {
  SDL_Texture *texture;
  SDL_Rect *sprite;
  int32_t width;
  int32_t height;
  uint32_t nb_sprites;
} wolfasm_sprite_t;

enum wolfasm_sprites { SPRITE_PISTOL = 0, NB_WOLFASM_SPRITES };
static char const *wolfasm_sprite_file[] = {"./resources/sprites/weapons.png"};
_Static_assert(sizeof(wolfasm_sprite_file) / sizeof(wolfasm_sprite_file[0]) ==
                   NB_WOLFASM_SPRITES,
               "Invalid sprites number");
wolfasm_sprite_t wolfasm_sprite[NB_WOLFASM_SPRITES];

#define PISTOL_SPRITES_ANIM 5
SDL_Rect pistol_sprite[PISTOL_SPRITES_ANIM];

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
  sprite[0].y = 222;
  sprite[0].w = 62;
  sprite[0].h = 62;

  sprite[1].x = 62;
  sprite[1].y = 202;
  sprite[1].w = 82;
  sprite[1].h = 82;

  sprite[2].x = 144;
  sprite[2].y = 203;
  sprite[2].w = 70;
  sprite[2].h = 81;

  sprite[3].x = 214;
  sprite[3].y = 203;
  sprite[3].w = 64;
  sprite[3].h = 80;

  sprite[4].x = 278;
  sprite[4].y = 181;
  sprite[4].w = 81;
  sprite[4].h = 103;

  return;
err:
  printf("%s\n", SDL_GetError());
  exit(1);
}

void deinit_sprites(void) {
  for (int32_t i = 0; i < NB_WOLFASM_SPRITES; ++i) {
    SDL_DestroyTexture(wolfasm_sprite[SPRITE_PISTOL].texture);
    free(wolfasm_sprite[SPRITE_PISTOL].sprite);
    wolfasm_sprite[SPRITE_PISTOL].sprite = NULL;
  }
}

static void render_sprite(wolfasm_sprite_t *sprite, int x, int y,
                          SDL_Rect *clip) {
  SDL_Rect renderQuad = {x, y, sprite->width, sprite->height};
  SDL_RenderCopy(window_renderer, sprite->texture, clip, &renderQuad);
}

void display_sprites_cwrapper(void) {
  static int32_t current_anim = 0;

  render_sprite(&wolfasm_sprite[0], window_width / 2 - 30 * 4,
                window_height - wolfasm_sprite[0].height,
                &wolfasm_sprite[0].sprite[current_anim]);
}
