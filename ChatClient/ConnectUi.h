#ifndef CONNECT_H
#define CONNECT_H

#include <QWidget>
#include <ui_Connect.h>
#include "Network.h"
#include <mainwindow.h>
namespace Ui {
    class ConnectUi;
    }

struct LoginDetails
{
    QString Address;
    QString Port;
    QString Username;
};

class ConnectUi : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectUi(QWidget *parent = nullptr);
    Network* GetNetwork(){return network_;}
    ~ConnectUi();
    
private:
    void saveLast();
    bool loadLast();
    LoginDetails loginDetails;
    QString fileName = "lastLogin.dat";
    bool loaded = false;
    Ui::ConnectUi *ui;
    MainWindow *mainUi;
    Network *network_;

public slots:
    void on_connectButton_pressed();
    void showErrorLabel(QString err);
};

#endif // MAINWINDOW_H
