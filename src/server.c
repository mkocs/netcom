#include "server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BUF 1024

int main(int argc, char * argv[])
{
  int conn_socket, new_socket;
  struct sockaddr_in address;
  socklen_t addrlen;
  char *buffer = malloc(BUF);
  ssize_t size;
  const int y = 1;

  if((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("Could not create socket.\n");
    return -1;
  }
  setsockopt(conn_socket, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int));
  // address configuration
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr("127.0.0.1");
  address.sin_port = htons(PORT);
  // bind the socket to address and port
  if(bind(conn_socket, (struct sockaddr *) &address, sizeof(address)) < 0)
  {
    printf("Could not bind socket. Port may be in use.\n");
    return -1;
  }
  // start listening on the socket
  if(listen(conn_socket, 5/*accept 5 connections*/) < 0)
  {
    printf("Could not start listening.\n");
    return -1;
  }
  printf("Listening on port %d...\n", PORT);
  // store the length of an address
  addrlen = sizeof(struct sockaddr_in);
  while(1){
    new_socket = accept(conn_socket, (struct sockaddr *) &address, &addrlen);
    if(new_socket > 0)
    {
      printf("Client (%s) connected!\n", inet_ntoa(address.sin_addr));
      do
      {
        //size = recv(new_socket, buffer, BUF-1, 0);
        size = read(new_socket, buffer, BUF-1);
        if(size > 0)
          buffer[size] = '\0';
          printf("%s wrote: %s", inet_ntoa(address.sin_addr), buffer);
      } while (strcmp(buffer, "/q\n") != 0/*as long as buffer is not /q\n */);
      printf("%s disconnected.", inet_ntoa(address.sin_addr));
      close(new_socket);
    }
  }
  if(close(conn_socket) < 0)
  {
    printf("Could not close socket.\n");
    return -1;
  }
  return 0;
}
