
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>
#include <stdbool.h>

// use linked list instead of array

FILE *logptr;

#define BUF_SIZE 500
#define BACKLOG 10 /* Passed to listen() */
#define MAX_CONNECTIONS 100

struct connectionnamepair
{
    int *socket;
    char name[50];
};

struct connectionnamepair connections[MAX_CONNECTIONS];
int numConnections = 0;

void sendToAll(char *buf, int mode) // mode 1 == message, 2 == remove from list, 3 == client log
{
    char tmpbuf[BUF_SIZE];
    if (mode == 1)
    {
        sprintf(tmpbuf, "m%s", buf);
    }
    else if (mode == 2)
    {
        sprintf(tmpbuf, "r%s", buf);
    }
    else if (mode == 3)
    {
        sprintf(tmpbuf, "c%s", buf);
    }
    for (int i = 0; i < numConnections; i++)
    {

        if (!send(*connections[i].socket, tmpbuf, sizeof(tmpbuf), 0))
        {
            fprintf(stderr, "Error sending response\n");
        }
    }
    fprintf(logptr, "Send to all: %s\n", tmpbuf);
    fflush(logptr);
}

void *handle(void *con)
{
    /* send(), recv(), close() */
    char buf[BUF_SIZE];

    struct connectionnamepair *connection = (struct connectionnamepair *)con;

    if (!recv(*connection->socket, buf, sizeof(buf), 0))
    {
        perror("receive");
    }

    strcpy(connection->name, buf);
    fprintf(logptr, "Received Name for Client: %s\n", connection->name);
    if (!send(*connection->socket, buf, sizeof(buf), 0))
    {
        fprintf(stderr, "Error sending response\n");
    }

    sendToAll(buf, 3);

    // how the fuck is this wrong, but yea
    // this is supposed to send all existing clients to this guy
    // it sucks tho
    // numconnections - 1 cause the newest one is me :)
    // why is this sending this persons name
    // and not the other one ???

    // correction this works, i dont know what i changhed im scared to
    // ctrl-z to find the difference so we are keeping it like this
    // also theoretically need to lock pthread access on numconnections
    // and connections when editing them and using them
    // struct with mutex in it should be enough
    // for (int i = 0; i < numConnections - 1; i++)
    // {
    //     sprintf(buf, "c%s", connections[i].name);
    //     if (*connections[i].socket != -1)
    //     {
    //         fprintf(stderr, "existing username %d: %s\n", i, buf);
    //         if (!send(*connection->socket, buf, sizeof(buf), 0))
    //         {
    //             fprintf(stderr, "Error sending response\n");
    //         }
    //     }
    // }
    while (1)
    {
        // 0 is orderly shutdown so clean if 0
        // if (recv(*connection->socket, buf, sizeof(buf), 0))
        // {
        //     perror("receive");
        // }

        // send msg to everyone to remove here prob
        // recv each byte and build string until exit/end character then go and come back after
        if (!recv(*connection->socket, buf, sizeof(buf), 0) || strcmp(buf, "exit") == 0)
        {
            sendToAll(connection->name, 2);

            for (int i = 0; i < numConnections; i++)
            {

                if (*connection->socket == *connections[i].socket)
                {
                    close(*connection->socket);

                    *connections[i].socket = -1;

                    return 0;
                }
            }
        }
        char tmpbuf[BUF_SIZE];
        printf("%s\n", buf);
        if (buf[0] == '/')
        {
            strtok(buf, " ");

            if (buf[1] == 'w')
            {
                char *name = strtok(NULL, " ");
                bool foundit = 0;
                char tmpbuf[BUF_SIZE];
                for (int i = 0; i < numConnections; i++)
                {

                    if (strcmp(connections[i].name, name) == 0)
                    {
                        char *msg = strtok(NULL, " ");
                        sprintf(tmpbuf, "w(Whisper From %s) %s", name, msg);

                        if (!send(*connections[i].socket, tmpbuf, sizeof(tmpbuf), 0))
                        {
                            fprintf(stderr, "Error sending response\n");
                        }
                        sprintf(tmpbuf, "w(Whisper To %s) %s", name, msg);

                        if (!send(*connection->socket, tmpbuf, sizeof(tmpbuf), 0))
                        {
                            fprintf(stderr, "Error sending response\n");
                        }
                        foundit = 1;
                    }
                }
                if (!foundit)
                {
                    // probably need to make seperate error thingie in client to handle
                    // stuff like this
                    sprintf(tmpbuf, "w%s is not a valid user", name);

                    if (!send(*connection->socket, tmpbuf, sizeof(tmpbuf), 0))
                    {
                        fprintf(stderr, "Error sending response\n");
                    }
                }
            }
            // else if (buf[1] == 'v')
            // {
            //     fprintf(stderr, "got a audio frame in theory\n");
            //     fprintf(stderr, "%s", buf);
            // }
            else
            {
                char nope[] = "mthats like not a real command bro";
                printf("%s\n", buf);
                if (!send(*connection->socket, nope, sizeof(nope), 0))
                {
                    fprintf(stderr, "Error sending response\n");
                }
            }
        }
        else
        {
            sprintf(tmpbuf, "%s_%s", buf, connection->name);
            sendToAll(tmpbuf, 1);
        }
        fprintf(logptr, "Sent Message: %s\n", tmpbuf);
        fflush(logptr);
    }

    return NULL;
}

int main(int argc, char *argv[])
{

    logptr = fopen("ChatServer.log", "a");

    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    pthread_t thread;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    int newsocket;
    char buf[BUF_SIZE];

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;     /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;     /* For wildcard IP address */
    hints.ai_protocol = 0;           /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    fprintf(logptr, "\n\nServer Starting\n");
    s = getaddrinfo(NULL, argv[1], &hints, &result);
    if (s != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    /* getaddrinfo() returns a list of address structures.
              Try each address until we successfully bind(2).
              If socket(2) (or bind(2)) fails, we (close the socket
              and) try the next address. */

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sfd = socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break; /* Success */

        close(sfd);
    }

    freeaddrinfo(result); /* No longer needed */

    if (rp == NULL)
    { /* No address succeeded */
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }

    if (listen(sfd, BACKLOG) == -1)
    {
        perror("listen");
        return 0;
    }
    fprintf(logptr, "Server Started and Listening\n");

    while (1)
    {
        peer_addr_len = sizeof(peer_addr);
        newsocket = accept(sfd, (struct sockaddr *)&peer_addr, &peer_addr_len);
        fprintf(logptr, "Server Accepted Client\n");

        if (newsocket == -1)
        {
            perror("accept");
            continue; /* Ignore failed request */
        }

        int *safesock = (int *)(malloc(sizeof(int)));
        if (safesock)
        {
            *safesock = newsocket;
            connections[numConnections].socket = safesock;

            if (pthread_create(&thread, NULL, handle, (void *)&connections[numConnections++]))
            {
                fprintf(stderr, "Failed to create thread\n");
            }
            printf("numConnections: %d\n", numConnections);
            fprintf(logptr, "numConnections: %d\n", numConnections);
        }
        else
        {
            perror("malloc");
        }
        fflush(logptr);
    }
}