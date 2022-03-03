/*****************************************************************************
 * server-c.c                                                                 
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

#define QUEUE_LENGTH 10
#define RECV_BUFFER_SIZE 2048

/*
 * main():
 */
int main(int argc, char **argv) {
  char *server_port;
  char *filename;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s [default web content file]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  server_port = argv[1];
  filename = argv[2];

  /*
   * TODO: Put your code here
   */
  return 0;
}
