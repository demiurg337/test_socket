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
#define MAX_PENDING_CONNECT 10

enum LogLvl {Info, Warn, Err};
/*
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
*/
void log(const char* msg, const LogLvl lvl)
{
    printf("INFO: %s\n", msg);
}

int main(int argc, char** argv)
{
    log("start ...", Info);
    int port_num = 1500;

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0) {
       log("Error on creation of socket !", Err); 
    }

    int val {1};
    // for preventing "this addres alredy in use", when close previous server
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0) {
    //if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0) { //c99
        log("Can't set option for socket !", Err);
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    //??????????
    //INADDR_ANY - 0.0.0.0.0
    server_addr.sin_addr.s_addr = INADDR_ANY;
    //The port number field in struct sockaddr_in is stored in network byte order. This means that you must use htons()
    server_addr.sin_port = htons(port_num);

    if (bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        log("Can't bind !", Err);
        exit(1);
    }

    if (listen(socket_fd, MAX_PENDING_CONNECT) < 0) {
        log("Can't start listen !", Err);
        exit(1);
    } 

    /*
    //for receiving signals
    struct sigaction sigAction;
    int socket_fd, 
        //id to accepted socket
        acceptSock_fd;
    struct addrinfo hints, *servInfo, *p;
    socklen_t sin_size;
    //socket address information for IPv4 or IPv6
    struct sockaddr_storage their_addr;
    char connHostName[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    //flag for receiving connections
    hints.ai_flags = AI_PASSIVE;
    int op_ok = 1;

    if (getaddrinfo(NULL, PORT, &hints, &servInfo) != 0) {
        fprintf(stderr, "Can't gent information !!!!\n");
    }

    for (p = servInfo; p != NULL; p = p->ai_next) {
    }

    if ((socket_fd = socket(servInfo->ai_family, servInfo->ai_socktype, servInfo->ai_protocol)) == -1) {
        perror("Can't create socker !");
        exit(1);
    }

    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &op_ok, sizeof(int)) == -1) {
        perror("Can't set the option !");
        exit(2);
    }

    //bind address to socket
    if (bind(socket_fd, servInfo->ai_addr, servInfo->ai_addrlen) == -1) {
        perror("Can't bind the address !");
    }

    freeaddrinfo(servInfo);

    if (listen(socket_fd, MAX_CONNECT) == -1) {
        perror("Can't enable listenning !");
    }

    //for removing all dead processes
    sigAction.sa_handler = sig_handler;
    //to hear only this signals
    sigemptyset(&sigAction.sa_mask);
    //set flags for specific behaviour
    sigAction.sa_flags = SA_RESTART;

    //SIGCHLD - when child process has finished his life
    if (sigaction(SIGCHLD, &sigAction, NULL) != 0) {
        perror("Can't bind the address !");
        exit(1);
    }

    printf("Waiting for connections ...");
    while(1) {
        acceptSock_fd = accept(socket_fd, (struct sockaddr *)&their_addr, &sin_size);
        if (acceptSock_fd == -1) {
            perror("Error on aceppting the socket !");
        }
        
        inet_ntop(
            their_addr.ss_family, 
            get_in_addr_of_spec_type((struct sockaddr *)&their_addr),
            connHostName,
            sizeof connHostName
        );

        printf("Was connected %s", connHostName);
        //Creation of child process
        
        //Like I understood forked process 
        //start his executing from the same place where
        //was father process
        
        if (!fork()) {
            //This is code for which will be executed for child process
            
            close(socket_fd);
            if (send(acceptSock_fd, "Message to client !", 13, 0) != 0) {
                perror("Error of sending !");
            } 
            close(acceptSock_fd);
            //after message child process will be finished
            exit(0);
        }
        //and after first message close father socket
        close(socket_fd);
    }
    */
    return 0;
}
