#ifndef server_h
#define server_h

#include <netinet/in.h>

#define BUFFER_SIZE 16000

typedef struct Server {
  int domain;
  int port;
  int service;
  int protocol;
  int backlog;
  u_long interface;

  int socket;
  struct sockaddr_in address;

  void (*launch)(struct Server *server);
} server_t;

server_t server_Constructor(int domain, int port, int service, int protocol,
                            int backlog, u_long interface,
                            void (*launch)(server_t *server));
void launch(server_t *server);

#endif
