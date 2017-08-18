#pragma once

//
// This file declares every data structure used during network exchanges
//

#include <stdbool.h>
#include <stdint.h>

//
// Packets datas
//

#define WOLFASM_PCK_MAGIC 0xB007
#define WOLFASM_PCK_VER 0x0001

enum wolfasm_udp_event { LAST_EVENT };

#if defined __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpacked"
#endif

struct __attribute__((packed)) wolfasm_udp_packet {
  double pos_x, pos_y;
  double dir_x, dir_y;
  uint32_t event;
};

#if defined __clang__
#pragma clang diagnostic pop
#endif

_Static_assert(sizeof(struct wolfasm_udp_packet) == 36,
               "Invalid UDP Packet size");

enum wolfasm_tcp_pck_type {
  PCK_HELLO = 0,
  PCK_UDP_PORT,
  PCK_CONNECTED,
  PCK_END,
  PCK_LAST_TYPE // This event should NOT be used
};

_Static_assert(PCK_LAST_TYPE < 0xFFFF, "Too many TCP packet type");

struct wolfasm_tcp_packet_header {
  uint16_t magic;
  uint16_t version;
  uint16_t type;
  uint16_t checksum;
};

_Static_assert(sizeof(struct wolfasm_tcp_packet_header) == 8,
               "Invalid TCP Packet size");

//
// Implementation datas
//
enum wolfasm_network_client_state {
  CLI_AUTHENTICATING = 0,
  CLI_MAP,
  CLI_PLAYING
};

#if defined __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

struct wolfasm_network_client {
  int32_t sock;
  enum wolfasm_network_client_state state;
  bool can_write;
};

#if defined __clang__
#pragma clang diagnostic pop
#endif
