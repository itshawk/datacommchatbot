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

// -lpulse-simple -lpulse

// playback atm is aplay -r 192000 -c 5 -f S32_BE recordingtest211 it makes MASSIVE files lol
// nvm its aplay -r 44100 -c 2 -f S24_3BE recordingtest3 cause way smaller and more reasonable files
//
// prob need to encode here somehow cause hot damn
// instead of write to file fd we will do write to socket fd, probably put it in
// "audio" mode until a certain text thing is written as hangup
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ConnectUi c;
    c.show();

    a.exec();
}
