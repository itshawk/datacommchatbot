#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <utility>
#include <signal.h>
#include <chrono>
#include <pthread.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>

#define BUF_SIZE 500

bool blowup = false;

void sig_handler(int signo)
{
    if (signo == SIGSEGV)
    {
        printf("sigsegv\n");
        sleep(5);
    }
    else if (signo == SIGINT)
    {
        exit(0);
    }
    return;
}

void receiver(QString in)
{
    
}

void *sender(int sfd, MainWindow *w)
{
    char msg[200];
    int len;

    QLineEdit *lineEdit = w->findChild<QLineEdit *>("lineEdit");

    printf("Enter your Client Name as the first msg:\n");
    QEventLoop loop;
    loop.connect(lineEdit, SIGNAL(returnPressed()), SLOT(quit()));
    while (loop.exec() == 0)
    {
        //printf("msg: %s\n", msg);
        QByteArray ba = lineEdit->displayText().toLocal8Bit();
        strcpy(msg, ba.constData());
        lineEdit->clear();
        //printf("msg: %s\n", msg);
        if (strcmp(msg, "exit\n") == 0)
        {
            blowup = 1;
            break;
        }
        printf("\033[A\33[2KT\r");
        printf("*");
        fflush(stdout);
        if (strcmp(msg, "exit\n") == 0)
        {
            blowup = 1;
        }
        //msg[strlen(msg)] = 0;
        len = strlen(msg) + 1;
        /* +1 for terminating null byte */

        if (len > BUF_SIZE)
        {
            fprintf(stderr,
                    "Ignoring long message in argument\n");
            continue;
        }

        if (write(sfd, msg, len) != len)
        {
            fprintf(stderr, "partial/failed write\n");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}

int initSocket(const char* addr, const char* port)
{
     if (signal(SIGSEGV, sig_handler) == SIG_ERR || signal(SIGINT, sig_handler) == SIG_ERR)
    {
        fputs("An error occurred while setting a signal handler.\n", stderr);
        return EXIT_FAILURE;
    }
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;

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
    return sfd;

}

