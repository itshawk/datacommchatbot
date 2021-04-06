#include "ConnectUi.h"
#include "ui_Connect.h"
#include <thread>
#include <iostream>

ConnectUi::ConnectUi(QWidget *parent) : QWidget(parent),
                                        ui(new Ui::ConnectUi)
{
    ui->setupUi(this);
    ui->errorLabel->setVisible(false);
    mainUi = new MainWindow();
}

ConnectUi::~ConnectUi()
{
    delete ui;
}

void ConnectUi::on_connectButton_pressed()
{
    //clear scuffed bool
    ui->errorLabel->setVisible(false);

    //Create the network and add error handling before starting
    network_ = new Network();
    connect(network_,&Network::error,
            this,&ConnectUi::showErrorLabel);

    //init the socket
    network_->Start(ui->addressLine->displayText().toLocal8Bit().constData(),
                    ui->portLine->displayText().toLocal8Bit().constData());

    //Check scuffed bool
    if(ui->errorLabel->isVisible())
        return;

    connect(network_,&Network::recv,
            mainUi, &MainWindow::uwu);

    network_->sender(ui->usernameLine->text());
    mainUi->show();


    auto mwle = mainUi->findChild<QLineEdit*>("lineEdit");
    connect(mwle,&QLineEdit::returnPressed,
            network_,[=]{network_->sender(mwle->text());mwle->clear();});

    std::thread t1(&Network::receiver, std::ref(network_));
    t1.detach();
    hide();

   

}

void ConnectUi::showErrorLabel()
{
    ui->errorLabel->setVisible(true);
}
