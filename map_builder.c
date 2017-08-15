//
// Compile with : clang -std=c11 -Weverything -I./include map_builder.c -o
// map_builder
//

#include "cdefs.h"
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//
// You must compile this file with your own map.c
// Your file must declare the following symbols
//
extern uint32_t const wolfasm_map_width;
extern uint32_t const wolfasm_map_height;
extern wolfasm_map_case_t const *wolfasm_map;

#define WOLFASM_MAP_MAGIC 0xCAFE

typedef struct wolfasm_map_header {
  uint16_t magic;
  uint32_t width;
  uint32_t height;
} __attribute__((packed)) wolfasm_map_header_t;

typedef struct wolfasm_map_items_header {
  uint32_t nb_items;
} __attribute__((packed)) wolfasm_map_items_header;

//
// Create the file
//
static int32_t create_map(char const *output_file) {
  // Create file
  int32_t rc = 0;
  int32_t fd = open(output_file, O_WRONLY | O_TRUNC | O_CREAT, 0664);

  if (fd == -1) {
    goto err;
  }

  // Write map header
  {
    wolfasm_map_header_t header = {WOLFASM_MAP_MAGIC, wolfasm_map_width,
                                   wolfasm_map_height};
    rc = (int32_t)write(fd, &header, sizeof(header));
    if (rc == -1) {
      goto err;
    }
  }

  // Check map
  for (uint32_t x = 0; x < wolfasm_map_width; ++x) {
    for (uint32_t y = 0; y < wolfasm_map_height; ++y) {
      assert(map[y * wolfasm_map_width + x].item == NULL);
      assert(map[y * wolfasm_map_width + x].enemy == NULL);
      assert(map[y * wolfasm_map_width + x].padding == NULL);
    }
  }
  // Write map
  rc = (int32_t)write(fd, map, sizeof(map));

  // Write items header
  {
    wolfasm_map_items_header header = {0};
    rc = (int32_t)write(fd, &header, sizeof(header));
    if (rc == -1) {
      goto err;
    }
  }
  // Write items
  //
  // TODO
  //

  // We're done with generating the map
  rc = close(fd);
  if (rc == -1) {
    goto err;
  }

  return EXIT_SUCCESS;

// In case of error
err:
  fprintf(stderr, "[Err] Cannot create map '%s': %s\n", output_file,
          strerror(errno));
  return EXIT_FAILURE;
}

int main(int ac, char **av) {
  int32_t ret = EXIT_SUCCESS;

  if (ac != 2) {
    fprintf(stderr, "Usage: %s output_file\n", *av);
    ret = EXIT_FAILURE;
  } else {
    ret = create_map(*(av + 1));
  }
  return ret;
}
