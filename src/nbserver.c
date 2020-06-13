#include <ctype.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <stdlib.h>
#include <fcntl.h>
#include <netinet/in.h>

void build_select_list()
{
    int listnum;
}

void setnonblocking(sock)
{

    int ops;
    ops = fcntl(sock, F_GETFL);
    if (ops < 0)
    {
        perror("fcntl(F_GETFL)");
        exit(EXIT_FAILURE);
    }
    ops = (ops | O_NONBLOCK);
    if (fcntl(sock, F_SETFL, ops) < 0)
    {
        perror("fcntl(F_SETFL)");
        exit(EXIT_FAILURE);
    }
    return;
}

int main()
{
    int sock, highsock, readsocks;
    struct sockaddr_in sockaddr;
    struct timeval timeout;
    int port = 8081;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket failed.");
        exit(EXIT_FAILURE);
    }

    // set non-blocking server
    setnonblocking(sock);
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    sockaddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
    {
        perror("bind");
        close(sock);
        exit(EXIT_FAILURE);
    }

    listen(sock, 5);

    // main loop
    while (1)
    {
        // use select instead of accept
        highsock = sock;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        readsocks = select(highsock + 1, &sock, (fd_set *)0, (fd_set *)0, &timeout);
        if(readsocks < 0){
            perror("readsocks");
            exit(EXIT_FAILURE);
        }

        if(readsocks == 0){
            printf(".");
            fflush(stdout);
        }
    }

    return 0;
}
