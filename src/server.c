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

// Constant quit string
// which indicates the end of a connection
// if received
const char quit_str[4] = "/q\n";
// Constant version number
const char v_num[] = "0.1";

int main(int argc, char * argv[])
{
  // Check if there are 2 or more arguments.
  // If there are, check if the 2nd argument is
  // -v or --version and print the version number.
  if (argc >= 2) {
    if (strncmp(argv[1], "-v", sizeof(&argv[1])) == 0 || strncmp(argv[1], "--version", sizeof(&argv[1])) == 0) {
      printf("Version number: %s\n", v_num);
      return 0;
    }
  }
  // 2 Socket connections represented by integer values
  // The first one is the overall connection the server listens
  // on for new connecting clients.
  // The second one is assigned, when a user connects,
  // so the server is able to listen to the user's communication.
  int conn_socket, new_socket;
  // Address struct, storing addressing information
  struct sockaddr_in address;
  socklen_t addrlen;
  // The buffer contains the data sent by the user
  // it's a small memory area, which can be filled,
  // if needed
  char *buffer = malloc(BUF);
  // Size stores the size/length of the
  // received data, to check if there has been any
  // new data sent by the user
  ssize_t size;
  const int y = 1;

  // Create the socket
  // Configure to use the internet protocol (IP) family
  // and TCP (SOCK_STREAM).
  // The third argument (0) sets the transport protocol to use from
  // the SOCK_STREAM protocols. 0 sets TCP
  // If the resulting integer, which is inserted into conn_socket
  // is smaller than 0, the socket could not be created and the program
  // will exit.
  // If successful, the socket functions assigns an integer value to
  // conn_socket, which will represent the socket.
  if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Could not create socket.\n");
    return -1;
  }
  // "The setsockopt() function shall set the option
  // specified by the option_name argument, at the protocol
  // level specified by the level argument, to the value
  // pointed to by the option_value argument for the socket
  // associated with the file descriptor specified by the
  // socket argument."
  setsockopt(conn_socket, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int));
  // Address configuration
  // Set address family to IP
  // Set port by transforming the PORT
  // from host format to network format (short)
  // and set the host address to INADDR_ANY
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);
  // Bind conn_socket to address and port
  // If a negative value is returned, an
  // error will be printed and the program will exit.
  if (bind(conn_socket, (struct sockaddr *) &address, sizeof(address)) < 0) {
    printf("Could not bind socket. Port may be in use.\n");
    return -1;
  }
  // Start listening on the connection socket
  // and await connecting clients
  if (listen(conn_socket, 5/*accept up to 5 connections*/) < 0) {
    printf("Could not start listening.\n");
    return -1;
  }
  printf("Listening on port %d...\n", PORT);
  // store the length of an address
  addrlen = sizeof(struct sockaddr_in);
  // Infinite loop to always await connections
  // and listen.
  // Each time going through the loop, a new socket will be created
  // and if there is a value bigger than 0 assigned to it, there
  // is a new connection on this socket, which is represented by this integer value.
  while (1) {
    new_socket = accept(conn_socket, (struct sockaddr *) &address, &addrlen);
    if (new_socket > 0) {
      printf("Client (%s) connected!\n", inet_ntoa(address.sin_addr));
      // This inner loop sort of represents the
      // connection to the user on the new_socket.
      // As long as the server doesn't receive a quit string (/q\n)
      // from the user, this inner loop will execute.
      do {
        // The length of the (possibly) received data
        // returned by the read() function is assigned to
        // the size variable.
        // If the size if bigger than 0, data/a message has
        // arrived and will be printed (with a few exceptions)
        size = read(new_socket, buffer, BUF-1);
        if (size > 0) {
          // Add a \0 termination string at the end of
          // the buffer.
          buffer[size] = '\0';
          // There are a few exceptions on which messages are printed and which
          // are not. Empty messages and quit strings are ignored. Quit strings
          // will instead be printed as a disconnect message after the loop
          // has ended.
          // This is necessary bceause the size check doesn't prevent empty
          // messages to be printed.
          if (strncmp(buffer, quit_str, sizeof(&buffer)) != 0 &&
            strncmp(buffer, "", sizeof(&buffer)) != 0 &&
            strncmp(buffer, "\n", sizeof(&buffer)) != 0) {
            printf("%s wrote: %s", inet_ntoa(address.sin_addr), buffer);
          }
        }
      } while (strcmp(buffer, quit_str) != 0/*as long as buffer is not /q\n */);
      printf("%s disconnected.\n", inet_ntoa(address.sin_addr));
      close(new_socket);
    }
  }
  return 0;
}
