#include "network.h"
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

// Network utilities
int32_t wolfasm_network_read(tcp_socket_t const sock, void *buf, size_t len) {
  int32_t ret = -1;
  char *buff = buf;

  assert(sock > -1 && buff);
  assert(len > 0);
  errno = EINTR;
  do {
    ret = (int32_t)read(sock, buff, (size_t)len);
  } while (ret == -1 && errno == EINTR);
  return (ret);
}

int32_t wolfasm_network_write(tcp_socket_t const sock, void const *buf,
                              size_t len) {
  int32_t ret = -1;
  int32_t total = 0;
  char const *buff = buf;

  assert(sock > -1 && buff);
  assert(len > 0);
  while (total != (int32_t)len) {
    do {
      ret = (int32_t)write(sock, buff + total, (size_t)len - (size_t)total);
    } while (ret == -1 && errno == EAGAIN);
    if (ret == -1 || ret == 0) {
      return (ret);
    }
    total += ret;
    assert(total <= len);
  }
  return (total);
}

// Serialization
void wolfasm_network_tcp_pck_header_deserialize(
    struct wolfasm_tcp_packet_header *header) {
  assert(header);
  header->magic = ntohs(header->magic);
  header->version = ntohs(header->version);
  header->type = ntohs(header->type);
  header->checksum = ntohs(header->checksum);
}

void wolfasm_network_tcp_pck_header_serialize(
    struct wolfasm_tcp_packet_header *header) {
  assert(header);
  header->magic = htons(header->magic);
  header->version = htons(header->version);
  header->type = htons(header->type);
  header->checksum = htons(header->checksum);
}

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
static struct wolfasm_network_client clients[WOLFASM_SRV_MAX_CLIENTS + 1];
static int32_t nb_clients = 0;

//
// Callbacks
//

// Read a PCK_HELLO packet
static void
wolfasm_network_cb_read_hello(struct wolfasm_network_client *cli,
                              struct wolfasm_tcp_packet_header const *pck) {
  assert(cli);
  assert(pck);
  assert(pck->type == PCK_HELLO);

  // TODO: Check checksum
  if (cli->state == CLI_AUTHENTICATING) {
    cli->state = CLI_MAP;
  }
}

// Read a PCK_CONNECTED packet
static void
wolfasm_network_cb_read_connected(struct wolfasm_network_client *cli,
                                  struct wolfasm_tcp_packet_header const *pck) {
  assert(cli);
  assert(pck);
  assert(pck->type == PCK_CONNECTED);

  // TODO: Check checksum
  if (cli->state == CLI_MAP) {
    cli->state = CLI_PLAYING;
  }
}

static void (*callback_read_treat_pck[])(
    struct wolfasm_network_client *cli,
    struct wolfasm_tcp_packet_header const *pck) = {
    &wolfasm_network_cb_read_hello, NULL, &wolfasm_network_cb_read_connected,
    NULL};

static void
wolfasm_server_disconnect_client(struct wolfasm_network_client *cli) {
  assert(cli);
  if (cli->sock != -1) {
    close(cli->sock);
  }
  cli->sock = -1;
  --nb_clients;

  assert(nb_clients >= 0);
}

static void cli_callback_read(struct wolfasm_network_client *cli) {
  assert(cli);

  // Read header
  struct wolfasm_tcp_packet_header header;

  int32_t rc = wolfasm_network_read(cli->sock, &header, sizeof(header));
  if (rc == 0 || rc == -1) {
    wolfasm_server_disconnect_client(cli);
    return;
  }

  // Analyze header
  wolfasm_network_tcp_pck_header_deserialize(&header);
  if (header.magic != WOLFASM_PCK_MAGIC) {
    return;
  }
  if (header.version != WOLFASM_PCK_VER) {
    return;
  }
  if (callback_read_treat_pck[header.type]) {
    callback_read_treat_pck[header.type](cli, &header);
  }
}

static void cli_callback_write(struct wolfasm_network_client *cli) {
  assert(cli);
}

static void cli_callback_except(struct wolfasm_network_client *cli) {
  assert(cli);
  wolfasm_server_disconnect_client(cli);
}

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
      tcp_socket_t max_fd = sock;

      // Reset fd_sets
      FD_ZERO(&readfds);
      FD_ZERO(&writefds);
      FD_ZERO(&exceptfds);

      FD_SET(sock, &readfds);

      // Monitor clients too
      for (int32_t i = 0; i < WOLFASM_SRV_MAX_CLIENTS; ++i) {
        if (clients[i].sock != -1) {
          FD_SET(clients[i].sock, &readfds);
          FD_SET(clients[i].sock, &exceptfds);
          if (clients[i].can_write) {
            FD_SET(clients[i].sock, &writefds);
          }
          if (clients[i].sock > max_fd) {
            max_fd = clients[i].sock;
          }
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
        clients[nb_clients].sock = -1;
        do {
          socklen_t len = sizeof(clients[nb_clients].addr);

          memset(&clients[nb_clients], 0, sizeof(*clients));
          clients[nb_clients].sock =
              accept(sock, (struct sockaddr *)&clients[nb_clients].addr, &len);
        } while (clients[nb_clients].sock && errno == EAGAIN);

        // Client was accepted
        if (clients[nb_clients].sock != -1) {
          if (nb_clients == WOLFASM_SRV_MAX_CLIENTS) {
            close(clients[nb_clients].sock);
            clients[nb_clients].sock = -1;
          } else {
            printf("Adding client %d\n", nb_clients);
            clients[nb_clients].state = CLI_AUTHENTICATING;
            ++nb_clients;
          }
        } else {
          perror("accept");
        }
      }
      for (int32_t i = 0; i < WOLFASM_SRV_MAX_CLIENTS; ++i) {
        if (clients[i].sock != -1) {
          if (FD_ISSET(clients[i].sock, &writefds)) {
            cli_callback_write(&clients[i]);
          }
          if (FD_ISSET(clients[i].sock, &exceptfds)) {
            cli_callback_except(&clients[i]);
          }
          if (clients[i].sock && FD_ISSET(clients[i].sock, &readfds)) {
            cli_callback_read(&clients[i]);
          }
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

// TODO: rm
int __attribute__((weak)) main() {
  wolfasm_server(NULL, 12345);
  return EXIT_SUCCESS;
}
