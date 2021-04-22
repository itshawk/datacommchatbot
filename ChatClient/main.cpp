#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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
#include "ChatWindow.h"
#include "ui_Chatwindow.h"
#include <QApplication>
#include <ConnectUi.h>
#include <stdio.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/gccmacro.h>

#define BUFSIZE 1024

/* A simple routine calling UNIX write() in a loop */
static ssize_t loop_write(int fd, const void *data, size_t size)
{
    ssize_t ret = 0;

    while (size > 0)
    {
        ssize_t r;

        if ((r = write(fd, data, size)) < 0)
            return r;

        if (r == 0)
            break;

        ret += r;
        data = (const uint8_t *)data + r;
        size -= (size_t)r;
    }

    return ret;
}

// -lpulse-simple -lpulse

// playback atm is aplay -r 192000 -c 5 -f S32_BE recordingtest211
// it makes MASSIVE files
// prob need to encode here somehow cause hot damn
// instead of write to file fd we will do write to socket fd, probably put it in
// "audio" mode until a certain text thing is written as hangup
int main(int argc, char *argv[])
{

    // this isnt reopening files not sure why O_TRUNC didnt work either
    int fd = open("recordingtest2111", O_CREAT | O_WRONLY, 777);
    /* The sample type to use */
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S32BE,
        .rate = 192000,
        .channels = 5};
    pa_simple *s = NULL;
    int ret = 1;
    int error;

    /* Create the recording stream */
    if (!(s = pa_simple_new(NULL, "ChatClient", PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error)))
    {
        fprintf(stderr, __FILE__ ": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }

    for (int i = 0; i < 10000; i++)
    {
        uint8_t buf[BUFSIZE];

        /* Record some data ... */
        if (pa_simple_read(s, buf, sizeof(buf), &error) < 0)
        {
            fprintf(stderr, __FILE__ ": pa_simple_read() failed: %s\n", pa_strerror(error));
            goto finish;
        }

        /* And write it to fd */
        if (loop_write(fd, buf, sizeof(buf)) != sizeof(buf))
        {
            fprintf(stderr, __FILE__ ": write() failed: %s\n", strerror(errno));
            goto finish;
        }
    }

    ret = 0;

finish:

    if (s)
        pa_simple_free(s);

    QApplication a(argc, argv);

    ConnectUi c;
    c.show();

    a.exec();
}
