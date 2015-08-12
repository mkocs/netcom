#include <stdio.h>
#include "net.h"

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    printf("Not enough arguments provided. Usage: %s <server address>\n", argv[0]);
    return -1;
  }
  start_reader();
  start_sender(argv[1]);
  return 0;
}
