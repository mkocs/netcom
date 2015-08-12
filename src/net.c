#include "net.h"
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int start_reader()
{
  int socket_welcome, socket_new;
  char buffer[1024];
  struct sockaddr_in server_addr;
  struct sockaddr_storage server_storage;
  socklen_t addr_size;

  // create socket
  socket_welcome = socket(
    AF_INET /*address family (ipv4)*/,
    SOCK_STREAM /*set as tcp socket*/,
    0 /*default layer 4 protocol (TCP in this case)*/
  );

  // server address struct configuration
  // address family (ipv4/ipv6)
  server_addr.sin_family = AF_INET;
  // set port number
  // htons (host to network short) makes sure bytes are stored in network byte order (most significant bit first)
  server_addr.sin_port = htons(PORT);
  // set ip
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  // set all bits of the padding field to 0
  memset(server_addr.sin_zero, '\0', sizeof server_addr.sin_zero);

  // bind the address struct to the socket
  bind(socket_welcome, (struct sockaddr *) &server_addr, sizeof(server_addr));

  if(listen(socket_welcome, 1/*maximum connection count*/) == 0)
  {
    printf("Listening on port %i...\n", PORT);
  }
  else
  {
    printf("An error occured. Could not start listening...\n");
    return -1;
  }

  // store address length/size
  addr_size = sizeof server_storage;
  // create a new socket for the incoming connection
  socket_new = accept(socket_welcome, (struct sockaddr *)&server_storage, &addr_size);

  // send messages to the socket of the new connection
  strcpy(buffer, "hello, netcom client");
  send(socket_new, buffer, 13, 0);

  return 0;
}

int start_sender(char * addr[])
{
  int socket_client;
  char buffer[1024];
  struct sockaddr_in server_addr;
  socklen_t addr_size;

  // create the socket
  socket_client = socket(
    AF_INET, /*address family (ipv4)*/
    SOCK_STREAM, /*set as tcp socket*/
    0 /*default layer 4 protocol (0 = tcp)*/
  );

  // configure server address struct
  // address family
  server_addr.sin_family = AF_INET;
  // set port number
  server_addr.sin_port = htons(PORT);
  // set address
  server_addr.sin_addr.s_addr = inet_addr(*addr);
  // set all fields of the padding field to 0
  memset(server_addr.sin_zero, '\0', sizeof server_addr.sin_zero);

  // connect to the server using address struct
  addr_size = sizeof server_addr;
  connect(socket_client, (struct sockaddr *) &server_addr, addr_size);

  // read the message from the server into the buffer
  recv(socket_client, buffer, 1024, 0);
  // print content of buffer
  printf("Received: %s", buffer);

  return 0;
}
