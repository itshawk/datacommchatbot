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

class Network : public QObject
{
    Q_OBJECT
private:
    int BUF_SIZE = 500;
    int MSG_LENGTH = 200;

public:
    Network(const char *addr, const char *port)
    {
        initSocket(addr, port);
    }
    void *receiver();

private:
    int sfd;
    void initSocket(const char *addr, const char *port);

public slots:
    void sender(QString in);
signals:
    void recv(QString in);
};
