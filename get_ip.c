#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char *argv[]) 
{
    struct addrinfo hints, *res, *p;
    char * ipVersion;
    void * address;
    struct sockaddr_in *ipv4;
    struct sockaddr_in6 *ipv6;
    struct sockaddr *ipv;
    char ip_str[INET_ADDRSTRLEN];
   
    if (argc != 2) {
        fprintf(stderr, "Need to set the hostname !");
        return 1;
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = 0;

    if (getaddrinfo(argv[1], NULL, &hints, &res) != 0) {
        fprintf(stderr, "Can't get information about domain!\n");
    }

    for (p = res; p != NULL; p = p->ai_next) {
        ipv = (struct sockaddr *) p->ai_addr;
        //IP4
        if (p->ai_family == AF_INET) {
            ipVersion = (char *) "IPv4";
            ipv4 = (struct sockaddr_in *) p->ai_addr;
            address = &(ipv4->sin_addr);
        //IP6
        } else {
            ipVersion = (char *) "IPv6";
            ipv6 = (struct sockaddr_in6 *) p->ai_addr;
            address = &(ipv6->sin6_addr);
        }
        
        inet_ntop(p->ai_family, address, ip_str, sizeof ip_str);
        printf("%s : %s\n\n", ip_str, ipVersion); 
    }
}
