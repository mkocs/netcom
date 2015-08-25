#include "server.h"
#include "client.h"
#include "net.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

const char netcom_usage_string[] = "Usage: ./netcom [-c <destination address>] [-s] [-v] [--version] [-h]\n";
const char netcom_cmd_string[] = "Possible commands during communication: /q ... Quit\n";
const char netcom_ver_string[] = "Version: 0.1\n";

static void *srv_thread_init(void *arg)
{
  srv_init();
  return NULL;
}

int main(int argc, char **argv)
{
  int run_srv, run_cli;

  if (argc < 2) {
    printf("Not enough arguments provided.\n%s", netcom_usage_string);
    return -1;
  }

  char *host_addr;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-h") == 0) {
      printf("NETCOM HELP\n%s%s", netcom_usage_string, netcom_cmd_string);
      return 0;
    } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
      printf("%s", netcom_ver_string);
      return 0;
    } else if (strcmp(argv[i], "-c") == 0) {
      if (i < argc-1) {
        host_addr = argv[i+1];
        run_cli = 1;
      } else {
        printf("Not enough arguments to call -c. You need to enter a destination address.\n");
        return -1;
      }
    } else if (strcmp(argv[i], "-s") == 0) {
      run_srv = 1;
    }
  }
  if (run_srv && !run_cli) {
    srv_init();
  } else if (run_srv && run_cli) {
    pthread_t srv_thread;
    if (pthread_create(&srv_thread, NULL, &srv_thread_init, &srv_thread) != 0) {
      printf("Server thread could not be created.\n");
      return -1;
    }
  }
  if (run_cli) {
    if (cli_init(host_addr) != 0) {
      srv_close();
      return -1;
    }
  }
  return 0;
}
