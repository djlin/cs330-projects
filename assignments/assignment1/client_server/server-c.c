/*****************************************************************************
 * server-c.c
 * Name: Tachun Lin
 *****************************************************************************/

//#include <arpa/inet.h> // for inet_ntop()
#include <err.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define QUEUE_LENGTH 10
#define RECV_BUFFER_SIZE 2048

/* TODO: server()
 * Open socket and wait for client to connect
 * Print received message to stdout
 * Return 0 on success, non-zero on failure
 */
int server(char *server_port) {
    // declare variables
    int sock_fd, new_sockfd; // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *serv_info;
    struct sockaddr_in client_addr; // client's address information
    socklen_t length;
    int error;
    int yes = 1;                  // used in setsockopt()
    unsigned char buffer[RECV_BUFFER_SIZE];

    // build address data structure with getaddrinfo()
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    error = getaddrinfo(NULL, server_port, &hints, &serv_info);
    if (error) {
        errx(1, "%s", gai_strerror(error));
    }

    // create a socket. here, s is the new socket descriptor
    if ((sock_fd = socket(serv_info->ai_family, serv_info->ai_socktype,
                          serv_info->ai_protocol)) < 0) {
        // handle the error if failed to create a socket
        perror("server: socket error");
        exit(-1);
    }

    // the following overcomes the bind() error "address already in use"
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(-1);
    }

    // bind the socket to the port we passed in to getaddrinfo()
    if (bind(sock_fd, serv_info->ai_addr, serv_info->ai_addrlen) < 0) {
        // handle the error if failed to bind
        close(sock_fd);
        perror("server: bind error\n");
        exit(-1);
    }
    freeaddrinfo(serv_info);

    if (serv_info == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        exit(-1);
    }

    // prepare the server for incoming requests.
    // you may consider this step as getting the reception ready
    if (listen(sock_fd, QUEUE_LENGTH) < 0) {
        perror("server: listen error");
        exit(-1);
    }

    // wait for connection, then start exchanging messages
    while (1) {
        /*
         * Pay special attention here!!! the accept() call will
         * returns the newly connected socket descriptor new _ s,
         * which will be used between this pair of
         * (server addr, server port, client addr, client port)
         */
        length = sizeof client_addr;
        if ((new_sockfd = accept(sock_fd, (struct sockaddr *)&client_addr,
                                 &length)) < 0) {
            // handle the error if failed to accept }
            perror("server: accept error");
            exit(-1); //continue;
        }

        /*
         * prepare the server for incoming message from the client
         * Note: this is a blocking function meaning that it will wait
         * for I/O to occur
         * Note: Here, we use the new_sockfd descriptor, NOT s!!!
         */

        while((error = recv(new_sockfd, buffer, RECV_BUFFER_SIZE, 0)) > 0) {
            fwrite(buffer, 1, error, stdout);
            fflush(stdout);
        }

        /*
         * Done, close the new _ s descriptor, i.e., release the connection.
         * Note: you should NOT close the socket descriptor s
         */
        close(new_sockfd);
    }
    return 0;
}

/*
 * main():
 * Parse command-line arguments and call server function
 */
int main(int argc, char **argv) {
    char *server_port;

    if (argc != 2) {
        fprintf(stderr, "Usage: ./server-c [server port]\n");
        exit(EXIT_FAILURE);
    }

    server_port = argv[1];
    return server(server_port);
}
