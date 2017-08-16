#include "cdefs.h"
#include "map_binary.h"
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

void wolfasm_map_init(void) __asm__("wolfasm_map_init");
void wolfasm_map_deinit(void) __asm__("wolfasm_map_deinit");

extern uint32_t wolfasm_map_width __asm__("map_width");
extern uint32_t wolfasm_map_height __asm__("map_height");
extern wolfasm_map_case_t *wolfasm_map __asm__("map");

uint32_t wolfasm_map_width = 0;
uint32_t wolfasm_map_height = 0;
wolfasm_map_case_t *wolfasm_map = NULL;

void wolfasm_map_init(void) {
  char const *name = "./resources/map/map_1.bin";

  int rc = 0;
  int fd = open(name, O_RDONLY);

  if (fd == -1) {
    goto err;
  }

  // Read header
  {
    wolfasm_map_header_t header;
    rc = (int32_t)read(fd, &header, sizeof(header));
    if (rc == -1) {
      goto err;
    }
    if (header.magic != WOLFASM_MAP_MAGIC) {
      fprintf(stderr, "Invalid file\n");
      exit(1);
    }
    wolfasm_map_width = header.width;
    wolfasm_map_height = header.height;
  }
  {
    size_t const map_size =
        wolfasm_map_width * wolfasm_map_height * sizeof(wolfasm_map_case_t);
    wolfasm_map = malloc(map_size);
    if (!wolfasm_map) {
      goto err;
    }

    rc = (int32_t)read(fd, wolfasm_map, map_size);
    if (rc == -1) {
      goto err;
    }

    rc = close(fd);
    if (rc == -1) {
      goto err;
    }
    return;
  err:
    fprintf(stderr, "Cannot load map '%s' : %s\n", name, strerror(errno));
    exit(1);
  }
}

void wolfasm_map_deinit(void) {
  free(wolfasm_map);
  wolfasm_map = NULL;
  wolfasm_map_width = 0;
  wolfasm_map_height = 0;
}
