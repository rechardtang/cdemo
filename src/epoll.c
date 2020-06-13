#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <string.h>

#define MAX_EVENTS 5
#define READ_SIZE 10

int main()
{

    int running = 1, event_count, i;

    size_t bytes_read;
    char read_buffer[READ_SIZE + 1];

    struct epoll_event event, events[MAX_EVENTS];

    // add file descripter for epoll to watch
    // the next thing is tell epoll what fds to watch and what kinds of events to watch for

    int epfd = epoll_create1(0);
    if (epfd == -1)
    {
        fprintf(stderr, "Failed to create epfd\n");
        return 1;
    }

    event.events = EPOLLIN;
    event.data.fd = 0;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, 0, &event))
    {
        fprintf(stderr, "Failed to add file descriptor to epoll\n");
        close(epfd);
        return 1;
    }

    while (running)
    {
        printf("\nPolling for input.\n");
        event_count = epoll_wait(epfd, events, MAX_EVENTS, 30000);

        printf("%d ready events\n", event_count);

        for (i = 0; i < event_count; i++)
        {
            printf("Reading file descriptor '%d' -- ", events[i].data.fd);
            bytes_read = read(events[i].data.fd, read_buffer, READ_SIZE);
            printf("%zd bytes read.\n", bytes_read);
            read_buffer[bytes_read] = '\0';
            printf("Read '%s'", read_buffer);
            if (!strncmp(read_buffer, "stop\n", 5))
            {
                running = 0;
            }
        }
    }

    if (close(epfd))
    {
        fprintf(stderr, "Failed to close epfd\n");
        return 1;
    }

    return 0;
}