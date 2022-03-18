/*****************************************************************************
 * server-c.c                                                                 
 * Name: Tachun Lin
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

#include <sys/stat.h>

#define QUEUE_LENGTH 10
#define MAX_SIZE 2048
#define HTTP_PORT 80

/*
 * For simplicity, we assume that the HTTP request is
 *     GET abs_path HTTP/1.1\r\n
 *     Host: hostname\r\n
 *     \r\n
 * so, we only need to get the abs_path and verify its validity
 */
  
void handle_http_request(char *request, int sockfd, char *filename) {
    int size;
    char buf[MAX_SIZE];
    struct stat file_stat;
    FILE *file;
    char *abs_path;
    char *file_to_open;
    char good_http_msg[] = "HTTP/1.1 200\r\n\r\n";
    char error_msg[] = "HTTP/1.1 404 Not Found\r\n\r\n<html><body><h1>404 Not Found</h1></body></html>\r\n";

    // method
    strtok(request, " ");
    // abs_path
    abs_path = strtok(NULL, " ");

    if(strlen(abs_path) == 1) {
      file_to_open = filename;
    } else {
      file_to_open = abs_path+1;
    }
    // open and return the default file
    if(stat(file_to_open, &file_stat) == 0 && (file_stat.st_mode & S_IFREG)== S_IFREG) {
      file = fopen(file_to_open, "r");
      send(sockfd, good_http_msg, sizeof good_http_msg, 0);
      while( (size = fread(buf, 1, MAX_SIZE, file)) > 0) {
        send(sockfd, buf, size, 0);
      }
      fclose(file);
    }
    else {
      send(sockfd, error_msg, strlen(error_msg), 0);
      perror("stat");
    }
}

/*
* main():
 */
int main(int argc, char **argv) {
    char *filename;
    struct sockaddr_in sin;
    char buf[MAX_SIZE];
    int buf_len;
    int yes = 1;
    socklen_t addr_len;
    int s, new_s;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s [default web content file]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    filename = argv[1];

    /* build address data structure */
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(HTTP_PORT);

    /* setup passive open */
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    // the following overcomes the bind() error "address already in use"
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      perror("setsockopt");
      exit(-1);
    }

    if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
        perror("bind");
        exit(1);
    }
    listen(s, QUEUE_LENGTH);

    /* wait for connection, then receive and print text */
    while(1) {
        if ((new_s = accept(s, (struct sockaddr *)&sin, &addr_len)) < 0) {
            fprintf(stderr, "error: %s accept\n", argv[0]);
            exit(1);
        }
        if ( (buf_len = recv(new_s, buf, sizeof(buf), 0)) ) {
            write(1, buf, buf_len);
            fflush(stdout);

            // analyze the request
            handle_http_request(buf, new_s, filename);
        }

        close(new_s);
    }
    close(s);

    return 0;
}
