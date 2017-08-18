#include "network.h"
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

//
// /!\ This game server can be heavily optimized.
//     It is only provided in order to allow 2 players to play together.
//     We could easily allow several players, or teams, to play together
//     with a some changes.
//

static int32_t wolfasm_server_init_socket(uint16_t const port) {
  int32_t sock = -1;
  struct sockaddr_in addr;

  // Create socket TCP
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    perror("socket");
    goto err;
  }

  memset(&addr, 0, sizeof(addr));
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);

  if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("bind");
    goto err;
  }

  if (listen(sock, 2) == -1) {
    perror("listen");
    goto err;
  }
  return sock;

err:
  if (sock >= 0) {
    close(sock);
  }
  return -1;
}

#define WOLFASM_SRV_MAX_CLIENTS 2
static struct wolfasm_network_client clients[WOLFASM_SRV_MAX_CLIENTS];
static int32_t nb_clients = 0;
static void (*read_callback[])() const = {};

void wolfasm_server(char const *map_file, uint16_t const port) {
  // Initialize TCP socket
  int32_t sock = wolfasm_server_init_socket(port);
  int32_t rc = 0;

  if (sock == -1) {
    fprintf(stderr, "Cannot initialize TCP socket\n");
    return;
  }
  memset(clients, 0, sizeof(clients));

  // Connection loop
  while (1) {
    fd_set readfds, writefds, exceptfds;
    // Multiplex here
    do {
      int32_t max_fd = sock;

      // Reset fd_sets
      FD_ZERO(&readfds);
      FD_ZERO(&writefds);
      FD_ZERO(&exceptfds);

      FD_SET(sock, &readfds);

      // Monitor clients too
      for (int32_t i = 0; i < nb_clients; ++i) {
        FD_SET(clients[i].sock, &readfds);
        FD_SET(clients[i].sock, &exceptfds);
        if (clients[i].can_write) {
          FD_SET(clients[i].sock, &writefds);
        }
        if (clients[i].sock > max_fd) {
          max_fd = clients[i].sock;
        }
      }
      rc = select(max_fd + 1, &readfds, &writefds, &exceptfds, NULL);
    } while (rc == -1 && errno == EAGAIN);

    if (rc == -1) {
      perror("select");
      break;
    } else if (rc) {
      // Treat IO here
      if (FD_ISSET(sock, &readfds)) {
        // Add new client
      }
      for (int32_t i = 0; i < WOLFASM_SRV_MAX_CLIENTS; ++i) {
        if (FD_ISSET(clients[i].sock, &readfds)) {
        }
        if (FD_ISSET(clients[i].sock, &writefds)) {
        }
        if (FD_ISSET(clients[i].sock, &exceptfds)) {
        }
      }
    }
  }

  // Stop connection
  rc = close(sock);
  if (rc == -1) {
    perror("close");
  }
}
