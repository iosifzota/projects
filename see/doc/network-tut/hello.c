#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define MYPORT "3490"  // the port users will be connecting to
#define HOST "localhost"
#define BACKLOG 10     // how many pending connections queue will hold

int main(void)
{
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int sockfd, new_fd,  status;

    // !! don't forget your error checking for these calls !!

    // first, load up address structs with getaddrinfo():

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    // hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    if ((status = getaddrinfo(HOST, MYPORT, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    // make a socket, bind it, and listen on it:

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(sockfd, res->ai_addr, res->ai_addrlen);
    listen(sockfd, BACKLOG);

    // now accept an incoming connection:
    addr_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size); // blocking

    puts("Connection !! :)");

#define recv_len 4096
    char recv_bytes[recv_len];
    status = recv(new_fd, recv_bytes, recv_len, 0); // blocking
    recv_bytes[recv_len-1] = '\0';
    printf("Received(%d): %s\n", status, recv_bytes);

    // ready to communicate on socket descriptor new_fd!
    char msg[] = "<h1>Hello there!</h1>";
    status = send(new_fd, msg, strlen(msg), 0);  // 
    printf("Send: %d\n", status);

    close(new_fd);
    close(sockfd);
    freeaddrinfo(res);
}
