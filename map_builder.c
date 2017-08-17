//
// Compile with : clang -std=c11 -Weverything -I./include map_builder.c -o
// map_builder
//

#include "cdefs.h"
#include "map_binary.h"
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//
// You MUST compile this file with your own map.c
// Your file MUST declare the following symbols
//
extern uint32_t const wolfasm_map_width;
extern uint32_t const wolfasm_map_height;
extern wolfasm_map_case_t const wolfasm_map[];
extern char const wolfasm_map_name[255];
extern wolfasm_map_item_t wolfasm_items[];
extern uint32_t const wolfasm_items_nb;

//
// Create the file
//
static int32_t create_map_header(int32_t const fd) {
  int32_t rc = 0;
  wolfasm_map_header_t header = {
      WOLFASM_MAP_MAGIC, wolfasm_map_width, wolfasm_map_height, {0}};

  strncpy(header.name, wolfasm_map_name, sizeof(header.name) - 1);
  do {
    rc = (int32_t)write(fd, &header, sizeof(header));
  } while (rc == -1 && errno == EAGAIN);
  if (rc == -1) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

#define MAP_CHECK_ERR(member)                                                  \
  if (!wolfasm_map[y * wolfasm_map_width + x].member) {                        \
    fprintf(stderr, "Invalid " #member " at [x:%d][y:%d]", x, y);              \
    return EXIT_FAILURE;                                                       \
  }

static int32_t create_map(int32_t const fd) {
  int32_t rc = 0;

  // Write map header
  if (create_map_header(fd) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }
  // Check map
  for (uint32_t x = 0; x < wolfasm_map_width; ++x) {
    for (uint32_t y = 0; y < wolfasm_map_height; ++y) {
      MAP_CHECK_ERR(item)
      MAP_CHECK_ERR(enemy)
      MAP_CHECK_ERR(padding)
    }
  }
  // Write map
  do {
    rc = (int32_t)write(fd, wolfasm_map,
                        sizeof(wolfasm_map[0]) * wolfasm_map_width *
                            wolfasm_map_height);
  } while (rc == -1 && errno == EAGAIN);
  if (rc == -1) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

#undef MAP_CHECK_ERR

static int32_t create_items(int32_t const fd) {
  int32_t rc = 0;
  // Write items header
  wolfasm_map_items_header header = {wolfasm_items_nb};
  do {
    rc = (int32_t)write(fd, &header, sizeof(header));
  } while (rc == -1 && errno == EAGAIN);
  if (rc == -1) {
    return EXIT_FAILURE;
  }
  // Write items
  do {
    rc = (int32_t)write(fd, wolfasm_items,
                        sizeof(wolfasm_items[0]) * wolfasm_items_nb);
  } while (rc == -1 && errno == EAGAIN);
  if (rc == -1) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

static int32_t create_map_file(char const *output_file) {
  // Create file
  int32_t rc = 0;
  int32_t fd = open(output_file, O_WRONLY | O_TRUNC | O_CREAT, 0664);

  if (fd == -1) {
    goto err;
  }

  // Write map to file
  if (create_map(fd) != EXIT_SUCCESS) {
    goto err;
  }
  // Write items to file
  if (create_items(fd) != EXIT_SUCCESS) {
    goto err;
  }

  // We're done with generating the map
  do {
    rc = close(fd);
  } while (rc == -1 && errno == EAGAIN);
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
    ret = create_map_file(*(av + 1));
  }
  return ret;
}
