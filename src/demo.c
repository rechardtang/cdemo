#include <stdio.h>
#include <sys/socket.h>
#include <sys/epoll.h> // for epoll_create1()
#include <netinet/in.h>
#include <string.h>

int main()
{
    // int port = 8080;
    // uint16_t p = htons(port);

    char *hello = "hello world";
    char *after;
    after = memset(&hello, '\0', 8);


    getchar();
    return 0;
}