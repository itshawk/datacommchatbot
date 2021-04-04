#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <utility>
#include <chrono>
#include <pthread.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#define BUF_SIZE 500

bool blowup = false;

void *receiver(int sfd, MainWindow *w)
{
    char buf[BUF_SIZE];
    ssize_t nread;
    bool setup = 0;

    QPlainTextEdit *plainText = w->findChild<QPlainTextEdit *>("plainTextEdit");
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

            plainText->insertPlainText(str);
            printf("Received %zd bytes: %s\n", nread, buf);
            setup = 1;
        }
        else
        {
            //fprintf(stderr, "%s", buf);
            char *msg = strtok(buf, "_");
            char *name = strtok(NULL, "_");
            str.sprintf("%s: %s\n", name, msg);
            for (int i = 0; i >= 0; i++)
            {
                plainText->insertPlainText(str);
                printf("%d\n", i);
            }
            printf("%s: %s\n", name, msg);
            fflush(stdout);
        }
    }
    return 0;
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
        QByteArray ba = lineEdit->displayText().toLocal8Bit();
        strcpy(msg, ba.constData());
        lineEdit->clear();
        printf("msg: %s\n", msg);
        if (strcmp(msg, "exit\n") == 0)
        {
            blowup = 1;
            break;
        }
        //printf("\033[A\33[2KT\r");
        //printf("*");
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

int main(int argc, char *argv[])
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;

    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s host port msg...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Obtain address(es) matching host/port. */

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0; /* Any protocol */

    s = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (s != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    /* getaddrinfo() returns a list of address structures.
              Try each address until we successfully connect(2).
              If socket(2) (or connect(2)) fails, we (close the socket
              and) try the next address. */

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sfd = socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break; /* Success */

        close(sfd);
    }

    freeaddrinfo(result); /* No longer needed */

    if (rp == NULL)
    { /* No address succeeded */
        fprintf(stderr, "Could not connect\n");
        exit(EXIT_FAILURE);
    }

    /* Send remaining command-line arguments as separate
              datagrams, and read responses from server. */

    //exit(EXIT_SUCCESS);
    QApplication a(argc, argv);
    MainWindow w;

    QPlainTextEdit *plainText = w.findChild<QPlainTextEdit *>("plainTextEdit");

    // std::thread t1(receiver, sfd, &w);
    // std::thread t2(sender, sfd, &w);
    // t1.detach();
    // t2.detach();

    QThread *t1 = QThread::create(receiver, sfd, &w);
    t1->setObjectName("Receiver Thread");
    connect(t1, &QThread::started, gui, &Gui::threadHasStarted);
    t1->start();
    QThread *t2 = QThread::create(sender, sfd, &w);
    t2->setObjectName("Sender Thread");
    connect(t2, &QThread::started, gui, &Gui::threadHasStarted);
    t2->start();

    w.show();
    a.exec();

    printf("after exec");

    while (!blowup)
    {
    }
}
