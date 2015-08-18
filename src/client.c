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

void ctrl_sig_handler (int sig)
{
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

int main (int argc, char **argv) {
  // signal function sets a function to handle signal i.e.
  // a signal handler with signal number sig.
  //
  // SIGINT = (Signal Illegal Instruction) Invalid function
  // image, such as an illegal instruction. This is generally
  // due to a corruption in the code or to an attempt to
  // execute data.
  signal(SIGINT, ctrl_sig_handler);

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

  // conn_socket defined in header
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
