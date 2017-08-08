// This files permits me to try things in C, before implementing them in
// assembly

#include "cdefs.h"
#include <SDL2/SDL_Image.h>
#include <assert.h>

void c_deinit(void);
void c_init(void);
void init_textures(void);
void deinit_textures(void);

void c_init() { init_textures(); }

void c_deinit() { deinit_textures(); }

//
// Textures
//

// Generate a few textures (8)
#define TEXTURE_SIZE 64
#define TEXTURE_NB 8
extern int32_t texture[TEXTURE_NB][TEXTURE_SIZE * TEXTURE_SIZE];
int32_t
    texture[TEXTURE_NB][TEXTURE_SIZE * TEXTURE_SIZE] __asm__("wolfasm_texture");

static SDL_Surface *image_surface[TEXTURE_NB];
static char const *img_name[TEXTURE_NB] = {
    "./resources/img/stone.png",
    "./resources/img/stonebrick_mossy.png",
    "./resources/img/log_jungle.png",
    "./resources/img/brick.png",
    "./resources/img/stonebrick_cracked.png",
    "./resources/img/end_stone.png",
    "./resources/img/planks_oak.png",
    "./resources/img/sand.png"};

void init_textures() {
  IMG_Init(IMG_INIT_PNG);

  for (int32_t i = 0; i < TEXTURE_NB; ++i) {
    image_surface[i] = IMG_Load(img_name[i]);

    if (!image_surface[i]) {
      printf("Cannot load image %s : %s", img_name[i], SDL_GetError());
      exit(1);
    }
    assert(image_surface[i]->w == image_surface[i]->h);
    assert(image_surface[i]->w == TEXTURE_SIZE);
    int32_t *ptr = image_surface[i]->pixels;
    for (int32_t x = 0; x < TEXTURE_SIZE * TEXTURE_SIZE; ++x) {
      texture[i][x] = ptr[x];
    }
  }
}

void deinit_textures(void) {
  for (int32_t i = 0; i < TEXTURE_NB; ++i) {
    SDL_FreeSurface(image_surface[i]);
  }
  IMG_Quit();
}
