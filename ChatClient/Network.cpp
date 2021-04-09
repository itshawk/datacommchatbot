#include "Network.h"

#include <QEventLoop>

void Network::Start(const char *addr, const char *port)
{
    initSocket(addr,port);
}

void Network::receiver()
{
    if(!running)
        return ;

    char buf[BUF_SIZE];
    ssize_t nread;
    bool setup = 0;

    //QPlainTextEdit *plainText = w->findChild<QPlainTextEdit *>("plainTextEdit");
    QString str;

    while (1)
    {
        struct sockaddr_storage peer_addr;
        socklen_t peer_addr_len;
        bzero(buf, BUF_SIZE);
        nread = recvfrom(sfd, buf, BUF_SIZE, 0,
                         (struct sockaddr *)&peer_addr, &peer_addr_len);
        if (nread == -1)
        {
            //TODO: This need better handling
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (!setup)
        {

            str.sprintf("Logged in as: %s", buf);
            emit recv(str);

            setup = 1;
        }
        else
        {
            printf("%c\n", buf[0]);
            if (buf[0] == 'm')
            {
                //fprintf(stderr, "%s", buf);
                char *tmpbuf = strchr(buf, 'm') + 1;
                printf("tmpbuf_m: %s\n", tmpbuf);
                char *msg = strtok(tmpbuf, "_");
                char *name = strtok(nullptr, "_");
                str.sprintf("%s: %s", name, msg);
                printf("%s: %s\n", name, msg);
                fflush(stdout);
            }
            else if (buf[0] == 'w')
            {
                char *msg = strchr(buf, 'w') + 1;
                printf("msg: %s\n", msg);

                str.sprintf("%s", msg);
                printf("%s\n", msg);
            }
            emit recv(str);
        }
    }
}
void Network::sender(QString in)
{

    if(!running)
        return;
    char msg[200];
    int len;

    if (in.size() > MSG_LENGTH)
        return;

    strcpy(msg, in.toLocal8Bit().constData());

    if (strcmp(msg, "exit\n") == 0)
    {
        return;
    }
    //printf("\033[A\33[2KT\r");
    //printf("*");
    fflush(stdout);
    len = strlen(msg) + 1;
    /* +1 for terminating nullptrptr byte */

    if (len > BUF_SIZE)
    {
        fprintf(stderr,
                "Ignoring long message in argument\n");
    }

    if (write(sfd, msg, len) != len)
    {
        emit error("partial/failed write");
    }
    //}
}

void Network::initSocket(const char *addr, const char *port)
{

    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int s;

    /*    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s host port msg...\n", argv[0]);
        exit(EXIT_FAILURE);
    } */

    /* Obtain address(es) matching host/port. */

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0; /* Any protocol */

    /* getaddrinfo() returns a list of address structures.
              Try each address until we successfully connect(2).
              If socket(2) (or connect(2)) fails, we (close the socket
              and) try the next address. */
    s = getaddrinfo(addr, port, &hints, &result);
    if (s != 0)
    {
        //fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        emit error(gai_strerror(s));
        return;
    }

    for (rp = result; rp != nullptr; rp = rp->ai_next)
    {
        sfd = socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (::connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
        {
            running = true;
            break; /* Success */
        }

        close(sfd);
    }

    freeaddrinfo(result); /* No longer needed */

    if (rp == nullptr)
    { /* No address succeeded */
        emit error("No address succeeded");
        //exit(EXIT_FAILURE);
    }


}
