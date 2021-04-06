#ifndef CONNECT_H
#define CONNECT_H

#include <QWidget>
#include <ui_Connect.h>
#include "Network.h"
#include <mainwindow.h>
namespace Ui {
    class ConnectUi;
    }

class ConnectUi : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectUi(QWidget *parent = 0);
    Network* GetNetwork(){return network_;}
    ~ConnectUi();
    
private:
    Ui::ConnectUi *ui;
    MainWindow *mainUi;
    Network *network_; 
public slots:
    void on_connectButton_pressed();
    void showErrorLabel();
};

#endif // MAINWINDOW_H
