#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>


int main(int argc, char *argv[]) 
{
    struct addrinfo hints, *res, *p;
   
    if (argc != 2) {
        fprintf(stderr, "Need to set the hostname !");
        return 1;
    }

    memset(&hints, 0, sizeof hints);
}
