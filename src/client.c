#include "client.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BUF 1024
int main (int argc, char **argv) {
  if(argc < 2){
     printf("Usage: %s <server address>\n", *argv);
     return -1;
  }
  if(strcmp(argv[1],"--help") == 0 || strcmp(argv[1], "-h") == 0)
  {
    printf("Usage: %s <server address>\n", *argv);
    printf("Commands:\n\t/q ... quit\n");
    return 0;
  }

  int conn_socket;
  char *buffer = malloc (BUF);
  struct sockaddr_in address;
  int size;
  if ((conn_socket=socket (AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("Could not create socket.\n");
    return -1;
  }
  address.sin_family = AF_INET;
  address.sin_port = htons (PORT);
  inet_aton (argv[1], &address.sin_addr);
  if (connect(conn_socket,(struct sockaddr *) &address, sizeof (address)) == 0)
    printf ("Connected to (%s) ...\n", inet_ntoa (address.sin_addr));
  do {
    printf ("> ");
    fgets (buffer, BUF, stdin);
    send(conn_socket, buffer, strlen (buffer), 0);
  } while (strcmp (buffer, "/q\n") != 0);
  close (conn_socket);
  return 0;
}
