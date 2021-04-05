#include "ConnectUi.h"
#include "ui_Connect.h"
#include <thread>
#include <mainwindow.h>
ConnectUi::ConnectUi(QWidget *parent) : QWidget(parent),
                                        ui(new Ui::ConnectUi)
{
    ui->setupUi(this);
        // connect(ui->connectButton,&QPushButton::pressed,
       //  this,&ConnectUi::on_connectButton_clicked);
}

ConnectUi::~ConnectUi()
{
    delete ui;
}

void ConnectUi::on_connectButton_pressed()
{
    network_ = new Network(ui->addressLine->displayText().toLocal8Bit().constData(),
                            ui->portLine->displayText().toLocal8Bit().constData());
    MainWindow *mw = new MainWindow();

    connect(network_,&Network::recv,
            mw, &MainWindow::uwu);


    mw->show();


    auto mwle = mw->findChild<QLineEdit*>("lineEdit");

    connect(mwle,&QLineEdit::returnPressed,
            network_,[=]{network_->sender(mwle->text());mwle->clear();});

    std::thread t1(&Network::receiver, std::ref(network_));
    t1.detach();
    hide();

    for(;;)
    {
        mw->uwu("Fuckywucky");
    }

}
