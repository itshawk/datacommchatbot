#include "Network.h"

#include<QEventLoop>

void* Network::receiver()
{
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
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (!setup)
        {

            str.sprintf("Received %zd bytes: %s\n", nread, buf);
            emit recv(str);

            //printf("Received %zd bytes: %s\n", nread, buf);
            setup = 1;
        }
        else
        {
            //fprintf(stderr, "%s", buf);
            char *msg = strtok(buf, "_");
            char *name = strtok(NULL, "_");
            str.sprintf("%s: %s", name, msg);
            emit recv(str);
       
            printf("%s: %s\n", name, msg);
            fflush(stdout);
        }
    }
}
void Network::sender(QString in)
{
    char msg[200];
    int len;

    if(in.size() > MSG_LENGTH)
        return;

    strcpy(msg,in.toLocal8Bit().constData());
    //QLineEdit *lineEdit = w->findChild<QLineEdit *>("lineEdit");

    //printf("Enter your Client Name as the first msg:\n");
    //QEventLoop loop;
    //loop.connect(lineEdit, SIGNAL(returnPressed()), SLOT(quit()));
    //while (loop.exec() == 0)
    //{
        //printf("msg: %s\n", msg);
        //QByteArray ba = lineEdit->displayText().toLocal8Bit();
        //strcpy(msg, ba.constData());
       // lineEdit->clear();
        //printf("msg: %s\n", msg);

        if (strcmp(msg, "exit\n") == 0)
        {
           return;
        }
        printf("\033[A\33[2KT\r");
        printf("*");
        fflush(stdout);
        if (strcmp(msg, "exit\n") == 0)
        {
            
        }
        //msg[strlen(msg)] = 0;
        len = strlen(msg) + 1;
        /* +1 for terminating null byte */

        if (len > BUF_SIZE)
        {
            fprintf(stderr,
                    "Ignoring long message in argument\n");
        }

        if (write(sfd, msg, len) != len)
        {
            fprintf(stderr, "partial/failed write\n");
            exit(EXIT_FAILURE);
        }
    //}
}

 void Network::initSocket(const char* addr, const char* port)
 {
    /*  if (signal(SIGSEGV, sig_handler) == SIG_ERR || signal(SIGINT, sig_handler) == SIG_ERR)
    {
        fputs("An error occurred while setting a signal handler.\n", stderr);
        exit(EXIT_FAILURE);
    } */
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
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sfd = socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (::connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break; /* Success */

        close(sfd);
    }

    freeaddrinfo(result); /* No longer needed */

    if (rp == NULL)
    { /* No address succeeded */
        fprintf(stderr, "Could not connect\n");
        exit(EXIT_FAILURE);
    }
    
 }
