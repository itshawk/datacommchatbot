#include "Network.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QApplication>
#include <time.h>

void Network::logAction(const char *loggedAction)
{
    if (logptr == NULL)
    {
        logptr = fopen("ChatClient.log", "a");
    }
    char timeout[1000];
    time_t t = time(NULL);
    struct tm *p = localtime(&t);
    strftime(timeout, 1000, "%F-%H-%M-%S", p);

    printf("%s: %s\n", timeout, loggedAction);
    fprintf(logptr, "%s: %s\n", timeout, loggedAction);
    fflush(logptr);
}

void Network::Start(const char *addr, const char *port)
{
    initSocket(addr, port);
    logptr = fopen("ChatClient.log", "a");
}

void Network::receiver()
{

    if (!running)
        return;

    char buf[BUF_SIZE];
    ssize_t nread;
    bool setup = 0;

    //QPlainTextEdit *plainText = w->findChild<QPlainTextEdit *>("plainTextEdit");
    QString str;

    while (1)
    {
        bzero(buf, BUF_SIZE);
        nread = ::recv(sfd, buf, sizeof(buf), 0);
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

            exit(EXIT_SUCCESS);
        }
        if (!setup)
        {

            str.sprintf("mLogged in as: %s", buf);
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
            str.remove(0, 1);
            this->logAction(str.toLocal8Bit().constData());
        }
    }
}
void Network::sender(QString in)
{

    if (!running)
        return;
    char msg[200];
    int len;

    if (in.size() > MSG_LENGTH)
        return;

    strcpy(msg, in.toLocal8Bit().constData());

    // First attempt at Voice Chat
    // if (msg[1] == 'v' && msg[0] == '/')
    // {
    //     strtok(msg, " ");
    //     char *name = strtok(NULL, " ");
    //     std::thread t1(&Network::audioSender, this, name);
    //     t1.detach();
    //     return;
    // }

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

// First attempt at Voice Chat Broken on Server Side I think
// #define BUFSIZE 512

// /* A simple routine calling UNIX write() in a loop */
// ssize_t Network::loop_write(int fd, const void *data, size_t size, char *name)
// {
//     ssize_t ret = 0;
//     uint8_t buf[BUFSIZE + 100];
//     memmove(buf + strlen(name) + 2, data, size);
//     buf[0] = '/';
//     buf[1] = 'v';
//     for (int i = 0; i < strlen(name); i++)
//     {
//         buf[i + 2] = name[i];
//     }

//     printf("%s", buf);
//     if ((ret = write(fd, buf, size)) < 0)
//     {
//         return ret;
//     }

//     return 0;
// }

// void Network::audioSender(char *name)
// {
//     // this isnt reopening files not sure why O_TRUNC didnt work either
//     int fd = open("recordingtest10", O_CREAT | O_WRONLY, 777);
//     /* The sample type to use */
//     static const pa_sample_spec ss = {
//         .format = PA_SAMPLE_S24BE,
//         .rate = 44100,
//         .channels = 2};
//     pa_simple *s = NULL;
//     int error;

//     /* Create the recording stream */
//     if (!(s = pa_simple_new(NULL, "ChatClient", PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error)))
//     {
//         fprintf(stderr, __FILE__ ": pa_simple_new() failed: %s\n", pa_strerror(error));
//         goto finish;
//     }

//     for (;;)
//     {
//         uint8_t buf[BUFSIZE + 15];

//         /* Record some data ... */
//         // this is blocking until size is reached
//         if (pa_simple_read(s, buf, BUFSIZE, &error) < 0)
//         {
//             fprintf(stderr, __FILE__ ": pa_simple_read() failed: %s\n", pa_strerror(error));
//             goto finish;
//         }
//         /* And write it to fd */
//         if (loop_write(fd, buf, sizeof(buf), name) != -1)
//         {
//             fprintf(stderr, __FILE__ ": write() failed: %s\n", strerror(errno));
//             //goto finish;
//         }
//     }

// finish:

//     if (s)
//         pa_simple_free(s);
// }

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
