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
#include <ConnectUi.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    ConnectUi c;
    c.show();

  /*   MainWindow w;
    w.show();
    c.ui->addressLine->displayText().toLocal8Bit().constData() */
  

    a.exec();
}
