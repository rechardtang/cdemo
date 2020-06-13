#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080

int main()
{
    int serverfd, newsocket, valread;
    struct sockaddr_in addr;
    int addrlen = sizeof(addr);
    int opt = 1;
    char buffer[1024] = {0};

    char *hello = "Hello from server!";

    serverfd = socket(AF_INET, SOCK_STREAM, 0);

    if (serverfd < 0)
    {
        perror("socket failed.");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(serverfd, (struct sockaddr *)&addr, addrlen) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverfd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((newsocket = accept(serverfd, (struct sockaddr *)&addr, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // read data from new socket
    valread = read(newsocket, buffer, 1024);

    printf("%s\n", buffer);

    send(newsocket, hello, strlen(hello), 0);

    printf("Hello message sent!");
    return 0;
}