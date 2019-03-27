#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>

#include <wait.h>
#include <signal.h>
#include <unistd.h>

#define PORT "3940"
#define MAX_MSG 100

void sig_handler(int s)
{
    // we don't need to know potential errorno from waitpid
    int dump_errno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0); 
    errno = dump_errno;
}

//for getting internet address
void *get_in_addr_of_spec_type(struct sockaddr *sa)
{
    if (sa->sa_family = AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
    //for receiving signals
    struct sigaction sigAction;
    int socket_fd, 
        //id to accepted socket
        acceptSock_fd;
    struct addrinfo hints, *servInfo, *p;
    socklen_t sin_size;
    //socket address information for IPv4 or IPv6
    struct sockaddr_storage their_addr;
    char 
        connHostName[INET6_ADDRSTRLEN],
        msg_buf[MAX_MSG];


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    //flag for receiving connections
    hints.ai_flags = AI_PASSIVE;
    int op_ok = 1;


    if (argc != 2) {
        perror("Format of command \"<command> host\"");
    }

    if (getaddrinfo(argv[1], PORT, &hints, &servInfo) != 0) {
        fprintf(stderr, "Can't gent information !!!!\n");
    }


    if ((socket_fd = socket(servInfo->ai_family, servInfo->ai_socktype, servInfo->ai_protocol)) == -1) {
        perror("Can't create socker !");
        exit(1);
    }


    if (connect(socket_fd, servInfo->ai_addr, servInfo->ai_addrlen) == -1) {
        close(socket_fd);
        perror("Can't bind the address !");
    }



    printf("Waiting for connections to server ...");
        
    inet_ntop(
        servInfo->ai_family, 
        get_in_addr_of_spec_type((struct sockaddr *)&servInfo->ai_addr),
        connHostName,
        sizeof connHostName
    );

    freeaddrinfo(servInfo);

    int numBytes;
    if ((numBytes = recv(socket_fd, msg_buf, MAX_MSG-1, 0)) == -1) {
        perror("Error when receiving message");
        exit(2);
    }
    msg_buf[numBytes] = '\0';

    printf("Received: %s", msg_buf);
    close(socket_fd);

    return 0;
}
