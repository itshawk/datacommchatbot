#include "Network.h"

#include <QApplication>
void Network::Start(const char *addr, const char *port)
{
    initilizeSocket(addr, port);
}

void Network::receiver()
{

    if (!connected_)
        return;

    char buf[BUF_SIZE];
    ssize_t nread;
    bool setup = 0;

    QString str;

    while (1)
    {
        bzero(buf, BUF_SIZE);
        nread = ::recv(socket_, buf, sizeof(buf), 0);
        printf("buf: %s\n", buf);
        if (nread == -1)
        {
            //TODO: This need better handling
            perror("read");
            exit(EXIT_FAILURE);
        }
        // 0 bytes returned on tcp means server shutdown orderly
        if (nread == 0)
        {
            perror("we outtie server died on purpose");
            // this sigsegv's occasionally probably cause threads were doing stuff
            // qt shutdown or something mybe? doesnt matter since we wanan die here anyway
            exit(EXIT_SUCCESS);
        }
        if (!setup)
        {

            str.asprintf("mLogged in as: %s", buf);
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
                str.sprintf("m%s: %s", name, msg);
                printf("%s: %s\n", name, msg);
                fflush(stdout);
            }
            else if (buf[0] == 'w')
            {
                char *msg = strchr(buf, 'w') + 1;
                printf("msg: %s\n", msg);

                str.sprintf("w%s", msg);
                printf("%s\n", msg);
            }
            else if (buf[0] == 'c')
            {
                printf("msg: %s\n", buf);

                str.sprintf("%s", buf);
            }
            else if (buf[0] == 'r')
            {
                printf("msg: %s\n", buf);

                str.sprintf("%s", buf);
            }
            emit recv(str);
        }
    }
}
void Network::send(QString in)
{

    if (!connected_)
        return;

    char msg[200];
    int len;

    if (in.size() > MSG_LENGTH)
        return;

    strcpy(msg, in.toLocal8Bit().constData());

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

    if (write(socket_, msg, len) != len)
    {
        emit error("partial/failed write");
    }
    if (strcmp(msg, "exit\n") == 0)
    {
        QApplication::quit();
        exit(EXIT_SUCCESS);
    }
    // if (strcmp(msg, "exit") == 0)
    // {
    //     close(sfd);
    // }
    //}
}

void Network::initilizeSocket(const char *addr, const char *port)
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
    hints.ai_family = AF_UNSPEC;     /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* tcp */
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
        socket_ = socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol);
        if (socket_ == -1)
            continue;

        if (::connect(socket_, rp->ai_addr, rp->ai_addrlen) != -1)
        {
            connected_ = true;
            break; /* Success */
        }

        close(socket_);
    }

    freeaddrinfo(result); /* No longer needed */

    if (rp == nullptr)
    { /* No address succeeded */
        emit error("No address succeeded");
        //exit(EXIT_FAILURE);
    }
}
