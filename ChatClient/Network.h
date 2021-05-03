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
#include <QObject>
#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/gccmacro.h>

class Network : public QObject
{
    Q_OBJECT
private:
    int BUF_SIZE = 500;
    int MSG_LENGTH = 200;

public:
    Network() {}
    void Start(const char *addr, const char *port);
    void receiver();
    void audioSender(char *name);
    void logAction(const char *loggedAction);

private:
    int sfd;
    FILE *logptr;
    void initSocket(const char *addr, const char *port);
    bool running = false;
    ssize_t loop_write(int fd, const void *data, size_t size, char *name);

public slots:
    void sender(QString in);
signals:
    void recv(QString in);
    void error(QString err);
};
