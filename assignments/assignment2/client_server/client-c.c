/*****************************************************************************
 * client-c.c                                                                 
 * Name:
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>

#define SEND_BUFFER_SIZE 2048


/*
 * main()
*/
int main(int argc, char **argv) {
  char *URI;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s [absolute URI]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  URI= argv[1];

  /*
   * TODO: Put your code here
   */

  return 0;
}
