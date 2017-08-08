// This files permits me to try things in C, before implementing them in
// assembly

#include "cdefs.h"
#include <SDL2/SDL_Image.h>
#include <assert.h>

void c_deinit(void);
void c_init(void);
void init_textures(void);

void c_init() { init_textures(); }

void c_deinit() {}

//
// Textures
//

// Generate a few textures (8)
#define TEXTURE_SIZE 64
#define TEXTURE_NB 8

extern int32_t
    texture[TEXTURE_NB][TEXTURE_SIZE * TEXTURE_SIZE] __asm__("wolfasm_texture");

extern SDL_Surface *
    image_surface[TEXTURE_NB] __asm__("wolfasm_texture_surface");

void init_textures() {
  for (int32_t i = 0; i < TEXTURE_NB; ++i) {
    assert(image_surface[i]->w == image_surface[i]->h);
    assert(image_surface[i]->w == TEXTURE_SIZE);
    int32_t *ptr = image_surface[i]->pixels;
    for (int32_t x = 0; x < TEXTURE_SIZE * TEXTURE_SIZE; ++x) {
      texture[i][x] = ptr[x];
    }
  }
}
