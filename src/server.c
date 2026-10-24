#include "./include/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

server_t server_Constructor(int domain, int port, int service, int protocol,
                            int backlog, u_long interface,
                            void (*launch)(server_t *server)) {
  server_t server;

  server.domain = domain;
  server.service = service;
  server.port = port;
  server.protocol = protocol;
  server.backlog = backlog;

  server.address.sin_family = domain;
  server.address.sin_port = htons(port);
  server.address.sin_addr.s_addr = htonl(interface);

  server.socket = socket(domain, service, protocol);
  if (server.socket < 0) {
    perror("Server Log: Failed to initialize/connect to socket\n");
    exit(EXIT_FAILURE);
  }

  if (bind(server.socket, (struct sockaddr *)&server.address,
           sizeof(server.address)) < 0) {
    perror("Server Log: Failed to bind socket\n");
    exit(EXIT_FAILURE);
  }

  if (listen(server.socket, server.backlog) < 0) {
    perror("Server Log: Failed to start listening\n");
    exit(EXIT_FAILURE);
  }

  server.launch = launch;
  return server;
}

void launch(server_t *server) {
  char buffer[BUFFER_SIZE];
  while (1) {
    printf("Server Log: Waiting for a connection\n");
    int addrlen = sizeof(server->address);
    int new_socket = accept(server->socket, (struct sockaddr *)&server->address,
                            (socklen_t *)&addrlen);
    ssize_t bytesRead = read(new_socket, buffer, BUFFER_SIZE - 1);
    if (bytesRead >= 0) {
      buffer[bytesRead] = '\0';
      puts(buffer);
    } else {
      perror("Server Log: Error reading buffer\n");
    }
    char *response = "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                     "<!DOCTYPE html>\r\n"
                     "<html>\r\n"
                     "<head>\r\n"
                     "<title>Testing Basic HTTP-SERVER</title>\r\n"
                     "</head>\r\n"
                     "<body>\r\n"
                     "Hello World!\r\n"
                     "</body>\r\n"
                     "</html>\r\n";
    write(new_socket, response, strlen(response));
    close(new_socket);
  }
}
