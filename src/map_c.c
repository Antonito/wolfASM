#include "cdefs.h"
#include "map_binary.h"
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

void wolfasm_map_init(char const *name) __asm__("_wolfasm_map_init");

extern uint32_t wolfasm_map_width __asm__("wolfasm_map_width");
extern uint32_t wolfasm_map_height __asm__("wolfasm_map_height");
extern wolfasm_map_case_t *wolfasm_map __asm__("wolfasm_map");
extern uint32_t wolfasm_items_nb __asm__("wolfasm_items_nb");
extern struct wolfasm_item_s *wolfasm_items __asm__("wolfasm_items");

extern void wolfasm_player_refill_life() __asm__("wolfasm_player_refill_life");
extern void wolfasm_player_refill_ammo() __asm__("wolfasm_player_refill_ammo");
extern int32_t enemy_animation_shoot[] __asm__("enemy_animation_shoot");

static void (*item_callbacks[])() = {
    [CALLBACK_LIFE] = &wolfasm_player_refill_life,
    [CALLBACK_AMMO] = &wolfasm_player_refill_ammo};

static int32_t *item_animation_table[] = {[TABLE_ENEMY_ANIMATION_SHOOT] =
                                              enemy_animation_shoot};

void wolfasm_map_init(char const *name) {
  int rc = 0;
  int fd = open(name, O_RDONLY);

  if (fd == -1) {
    goto err;
  }

  // Read header
  {
    wolfasm_map_header_t header;
    rc = (int32_t)read(fd, &header, sizeof(header));
  }

  // Read map
  {
    size_t const map_size =
        wolfasm_map_width * wolfasm_map_height * sizeof(wolfasm_map_case_t);
    char map[map_size];
    rc = (int32_t)read(fd, map, map_size);
  }

  // Read item header
  {
    wolfasm_map_items_header header;
    rc = (int32_t)read(fd, &header, sizeof(header));
  }

  for (uint32_t i = 0; i < wolfasm_items_nb; ++i) {
    wolfasm_map_item_t cur;

    rc = (int32_t)read(fd, &cur, sizeof(cur));
    if (rc == -1) {
      goto err;
    }

    wolfasm_items[i].texture = cur.texture;
    wolfasm_items[i].pos_x = cur.pos_x;
    wolfasm_items[i].pos_y = cur.pos_y;
    wolfasm_items[i].width_div = cur.width_div;
    wolfasm_items[i].height_div = cur.height_div;
    wolfasm_items[i].height_move = cur.height_move;
    wolfasm_items[i].current_anim = cur.current_anim;
    wolfasm_items[i].nb_anim = cur.nb_anim;
    wolfasm_items[i].anim_rate = cur.anim_rate;
    wolfasm_items[i].stock = cur.stock;
    wolfasm_items[i].type = cur.type;

    wolfasm_items[i].texture_table = item_animation_table[cur.texture_table];
    wolfasm_items[i].callback = item_callbacks[cur.callback];
  }

  // Close file
  rc = close(fd);
  if (rc == -1) {
    goto err;
  }
  return;
err:
  fprintf(stderr, "Cannot load map '%s' : %s\n", name, strerror(errno));
  exit(1);
}
