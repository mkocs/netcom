#include "client.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#define BUF 1024

const char quit_str[4] = "/q\n";
int conn_socket;

// functions to handle the ctrl-c
// signal
void ctrl_sig_handler (int sig)
{
  // Ask the user for consent
  // If the user wishes to quit, the client
  // will send the quit string (/q\n)
  // and exit with status code 0
  // Otherwise it continues listening
  char c;
  signal(sig, SIG_IGN);
  printf("Do you really want to quit? [y/n] ");
  c = getchar();
  if (c == 'y' || c == 'Y')
  {
    // send the quit string to the server to terminate the connection
    send(conn_socket, quit_str, strlen (quit_str), 0);
    exit(0);
  }
  else
  {
    signal(SIGINT, ctrl_sig_handler);
  }
  getchar(); // Get new line character
}

// Function to print a message to explain
// the usage of the application to the user
void print_usagemsg(char prim_arg[])
{
  printf("Usage: %s [-a <server address>]\nCommands:\n\t/q ... quit\n", prim_arg);
}

int main (int argc, char **argv) {
  char* host_address;
  // signal function sets a function to handle signal i.e.
  // a signal handler with signal number sig.
  //
  // SIGINT = (Signal Illegal Instruction) Invalid function
  // image, such as an illegal instruction. This is generally
  // due to a corruption in the code or to an attempt to
  // execute data.
  signal(SIGINT, ctrl_sig_handler);

  // Check if there are enough arguments to run the
  // client.
  // If it is, continue.
  // Otherwise print a message explaining how
  // to use the client
  if(argc < 2){
     printf("Usage: %s <server address>\n", *argv);
     return -1;
  }
  // Check if the second parameter is --help, -h, -a or empty
  // If it is --help or -h, print the usage text.
  // If it is -a, it expects an address as the next parameter
  if(strcmp(argv[1],"--help") == 0 || strcmp(argv[1], "-h") == 0)
  {
    print_usagemsg(*argv);
    return 0;
  }
  else if(strcmp(argv[1], "-a") == 0 && argc >= 3)
  {
    host_address = argv[2];
  }
  else
  {
    host_address = argv[1];
  }

  // buffer contains the data that is to be
  // sent to the server
  char *buffer = malloc (BUF);
  // address struct, storing addressing information
  struct sockaddr_in address;

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
  if ((conn_socket= socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("Could not create socket.\n");
    return -1;
  }
  // Address configuration
  // Set address family to IP
  // Set port by transforming the PORT
  // from host format to network format (short)
  // and set the host address with inet_aton, which
  // transforms the dotted decimal notation to
  // binary format.
  address.sin_family = AF_INET;
  address.sin_port = htons (PORT);
  inet_aton (host_address, &address.sin_addr);
  // Connect to the server
  // The function takes the socket it shall use,
  // the address (usually it expects type sockaddr, but
  // we can easily transform our sockaddr_in and use it, since they are compatible),
  // and the length/size of the address
  // If the function returns 0, the connection attempt was successful.
  // Otherwise it will return an error.
  if (connect(conn_socket,(struct sockaddr *) &address, sizeof (address)) == 0) {
    printf ("Connected to (%s) ...\n", inet_ntoa (address.sin_addr));
  } else {
    printf("Connection attempt failed. Please check if the address you entered is correct.\n");
    return -1;
  }
  // The loop executes until the user
  // send the quit string (/q\n).
  // It demands user input
  // and sends the data to the server
  // If the user enters the quit string, the program
  // will exit the loop, close the connection and
  // close.
  do {
    printf ("> ");
    fgets (buffer, BUF, stdin);
    send(conn_socket, buffer, strlen (buffer), 0);
  } while (strcmp (buffer, quit_str) != 0);
  close (conn_socket);
  return 0;
}
