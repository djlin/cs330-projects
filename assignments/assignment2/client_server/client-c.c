/*****************************************************************************
 * client-c.c                                                                 
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

#define SEND_BUFFER_SIZE 2048
#define HTTP_PORT 80


/*
 * main()
*/
int main(int argc, char **argv) {
    char *URI;
    char *host, *path; 
    int s;
    size_t len;
    struct hostent *hp;
    struct sockaddr_in sin;
    char buf[SEND_BUFFER_SIZE];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s [absolute URI]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    URI= argv[1];
    len = strlen(URI);

    // parse the URI and get the host (or IP)
    // and the abs_path (default /)
    host = strtok(URI, "/");
    path = strtok(NULL, "");
    //printf("%s\n", host);
    //printf("%s\n", path); 

    // prepare HTTP request
    if(path)
        sprintf(buf, "GET /%s HTTP/1.1\r\nHost: %s\r\n\r\n",
            path, host);
    else
        sprintf(buf, "GET / HTTP/1.1\r\nHost: %s\r\n\r\n",
            host);
    printf("%s\n", buf);

    // prepare socket (same code as project#1)
    /* translate host name into peer's IP address */
    hp = gethostbyname(host);
    if (!hp) {
        fprintf(stderr, "%s: unknown host\n", host);
        exit(1);
    }

    /* build address data structure */
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
    sin.sin_port = htons(HTTP_PORT);

    /* active open */
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("simplex-talk: socket");
        exit(1);
    }
    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        perror("simplex-talk: connect");
        close(s);
        exit(1);
    }

    int size = strlen(buf);
    if(send(s, buf, size, 0) < 0) {
        perror("client: send");
        exit(EXIT_FAILURE);
    }

    while( (size = recv(s, buf, sizeof buf, 0)) ) {
        write(1, buf, size);     
    }
    fflush(stdout);

    // Done, close the s socket descriptor
    close(s);


    return 0;
}
