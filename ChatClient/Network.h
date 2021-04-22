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
    Network(){}
    void Start(const char *addr, const char *port);
    void receiver();

private:
    int socket_;
    void initilizeSocket(const char *addr, const char *port);
    bool connected_ = false;

public slots:
    void send(QString in);
signals:
    void recv(QString in);
    void error(QString err);
};
