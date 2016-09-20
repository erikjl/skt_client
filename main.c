#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void stream_ipv4_client(const char* ip, const char* port)
{
    int sockfd, n, retCode;

    char buffer[256];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }

    struct addrinfo hints, *svrInfo, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    retCode = getaddrinfo(ip, port, &hints, &svrInfo);

    if(retCode != 0)
    {
        fprintf(stderr, "getaddrinto: %s", gai_strerror((retCode)));
        exit(1);
    }

    for(p = svrInfo; p != NULL; p=p->ai_next)
    {
        if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("socket");
            continue;
        }

        if(connect(sockfd, p->ai_addr, p->ai_addrlen) ==-1)
        {
            perror("connect");
            close(sockfd);
            continue;
        }
        break; //connect success!
    }

    while(1)
    {
        printf("Please enter the message: ");
        bzero(buffer,256);
        fgets(buffer,255,stdin);
        n = write(sockfd,buffer,strlen(buffer));

        if (n < 0)
        {
             error("ERROR writing to socket");
        }

        bzero(buffer,256);
        n = read(sockfd,buffer,255);

        if (n < 0)
        {
             error("ERROR reading from socket");
        }
        printf("%s\n",buffer);
    }
    close(sockfd); //wont get hit
}

void datagram_ipv4_client(const char* ip, const char* port)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(ip, port, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("talker: socket");
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        fprintf(stderr, "talker: failed to create socket\n");
        exit(2);
    }

    char* input[256];

    while(1)
    {
        printf("Enter message:\n");
        bzero(input, 256);
        fgets(input, 255, stdin);

        if ((numbytes = sendto(sockfd, input, strlen(input), 0, p->ai_addr, p->ai_addrlen)) == -1)
        {
            perror("talker: sendto");
            exit(1);
        }


        printf("talker: sent %d bytes to %s\n", numbytes, "127.0.0.1");
    }

    freeaddrinfo(servinfo); //wont get hit
    close(sockfd);
}

void datagram_ipv6_client(const char* ip, const char* port)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(ip, port, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("talker: socket");
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        fprintf(stderr, "talker: failed to create socket\n");
        exit(2);
    }

    char* input[256];

    while(1)
    {
        printf("Enter message:\n");
        bzero(input, 256);
        fgets(input, 255, stdin);

        if ((numbytes = sendto(sockfd, input, sizeof(input), 0, p->ai_addr, p->ai_addrlen)) == -1)
        {
            perror("talker: sendto");
            exit(1);
        }

        printf("talker: sent %d bytes to %s\n", numbytes, "::1");
    }

    freeaddrinfo(servinfo); //wont get hit
    close(sockfd); //wont get hit
}

int main(int argc, char *argv[])
{
    datagram_ipv6_client("fd00::4053:f807:d82a:54d8", "3000");
}
