
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

#define BUF_SIZE 500

struct hostservicepair
{
    char service[32];
    char host[1025];
    char name[50];
    struct sockaddr_storage addr;
    socklen_t addr_len;

} hostservicepair;

int main(int argc, char *argv[])
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    char buf[BUF_SIZE];

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

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

    /* Read datagrams and echo them back to sender. */

    struct hostservicepair logged_addr[100];
    int numlogged = 0;
    for (;;)
    {
        peer_addr_len = sizeof(peer_addr);
        nread = recvfrom(sfd, buf, BUF_SIZE, 0,
                         (struct sockaddr *)&peer_addr, &peer_addr_len);

        if (nread == -1)
            continue; /* Ignore failed request */

        char host[NI_MAXHOST], service[NI_MAXSERV];

        s = getnameinfo((struct sockaddr *)&peer_addr,
                        peer_addr_len, host, NI_MAXHOST,
                        service, NI_MAXSERV, NI_NUMERICSERV);

        int found = -1;
        for (int i = 0; i <= numlogged; i++)
        {
            if (strcmp(logged_addr[i].host, host) == 0 && strcmp(logged_addr[i].service, service) == 0)
            {
                printf("Already Logged this one\n");
                found = i;
                break;
            }
        }

        if (found == -1)
        {
            if (numlogged < 100)
            {
                strcpy(logged_addr[numlogged].host, host);
                strcpy(logged_addr[numlogged].service, service);
                strcpy(logged_addr[numlogged].name, buf);
                logged_addr[numlogged].name[nread - 1] = '\0';

                logged_addr[numlogged].addr = peer_addr;
                logged_addr[numlogged].addr_len = peer_addr_len;
            }
            else
            {
                numlogged = 0;
                strcpy(logged_addr[numlogged].host, host);
                strcpy(logged_addr[numlogged].service, service);
                strcpy(logged_addr[numlogged].name, buf);
                logged_addr[numlogged].addr = peer_addr;
                logged_addr[numlogged].name[nread - 1] = '\0';
                logged_addr[numlogged].addr_len = peer_addr_len;
            }
            if (s == 0)
                printf("Received %zd bytes from %s:%s, %s\n",
                       nread, logged_addr[numlogged].host, logged_addr[numlogged].service, logged_addr[numlogged].name);
            else
                fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));

            sprintf(buf, "Client Logged as %s", logged_addr[numlogged].name);
            if (sendto(sfd, buf, strlen(buf), 0,
                       (struct sockaddr *)&peer_addr,
                       peer_addr_len) != strlen(buf))
                fprintf(stderr, "Error sending response\n");

            numlogged++;
            continue;
        }

        if (s == 0)
        {
            //printf("Received %zd bytes from known addr %s:%s, %s\n",
            //       nread, logged_addr[found].host, logged_addr[found].service, logged_addr[found].name);
            printf("%s: %s", logged_addr[found].name, buf);
        }
        else
        {
            fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
        }
        buf[nread - 1] = '\0';

        for (int i = 0; i < numlogged; i++)
        {
            char tmpbuf[500];

            sprintf(tmpbuf, "%s_%s", buf, logged_addr[found].name);

            if (sendto(sfd, tmpbuf, strlen(tmpbuf), 0, (struct sockaddr *)&logged_addr[i].addr, logged_addr[i].addr_len) != strlen(tmpbuf))
            {
                fprintf(stderr, "Error sending response\n");
            }
        }
    }
}