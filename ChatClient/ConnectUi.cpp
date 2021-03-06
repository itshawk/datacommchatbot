#include "ConnectUi.h"
#include "ui_Connect.h"
#include <thread>
#include <iostream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

ConnectUi::ConnectUi(QWidget *parent) : QWidget(parent),
                                        ui(new Ui::ConnectUi)
{
    ui->setupUi(this);
    ui->errorLabel->setVisible(false);
    mainUi = new ChatWindow();

    //Create the network and add error handling before starting
    network_ = new Network();
    network_->logAction("\n\nStartup");

    connect(network_, &Network::error,
            this, &ConnectUi::showErrorLabel);

    connect(network_, &Network::error,
            network_, [=] { char s[100] = "Connection Error: ";        
            strcat(s, ui->errorLabel->text().toLocal8Bit().constData());
            network_->logAction(s); });

    //Handle enter on username tb
    connect(ui->usernameLine, &QLineEdit::returnPressed,
            this, &ConnectUi::on_connectButton_pressed);

    //Load last connection
    if (loadLast())
    {
        ui->addressLine->setText(loginDetails.Address);
        ui->portLine->setText(loginDetails.Port);
        ui->usernameLine->setText(loginDetails.Username);
        loaded = true;
        network_->logAction("Load Saved Credentials");
    }
}

ConnectUi::~ConnectUi()
{
    delete ui;
}

// why the fuck does this happen twice occasionally
// inits a thing twice i think? i dunno
void ConnectUi::on_connectButton_pressed()
{
    if (connected)
        return;
    connected = true;
    //clear scuffed bool
    ui->errorLabel->setVisible(false);

    //if user changed lines update file
    if (ui->addressLine->isModified() || ui->portLine->isModified() || ui->usernameLine->isModified())
    {
        loginDetails.Address = ui->addressLine->displayText();
        loginDetails.Port = ui->portLine->displayText();
        loginDetails.Username = ui->usernameLine->displayText();
        network_->logAction("Update Saved Credentials");
    }

    //Save last login to disk
    saveLast();

    //init the socket
    network_->logAction("Starting the Network...");

    network_->Start(loginDetails.Address.toLocal8Bit().constData(),
                    loginDetails.Port.toLocal8Bit().constData());
    //Check scuffed bool
    if (ui->errorLabel->isVisible())
        return;
    network_->logAction("Started the Network");

    //Setup reciver handler
    connect(network_, &Network::recv,
            mainUi, &ChatWindow::insertText);

    //Scuffed username handling
    char usernameLog[100] = "Attempt Login with Username==";
    strcat(usernameLog, loginDetails.Username.toLocal8Bit().constData());
    network_->logAction(usernameLog);
    network_->sender(loginDetails.Username.toLocal8Bit().constData());
    network_->logAction("Login Successful");

    auto mwle = mainUi->findChild<QLineEdit *>("lineEdit");
    connect(mwle, &QLineEdit::returnPressed,
            network_, [=] {     char s[100] = "Sending Message: ";
        strcat(s, mwle->text().toLocal8Bit().constData());
        network_->logAction(s);
        network_->sender(mwle->text());
        mwle->clear(); });

    connect(QApplication::instance(), SIGNAL(aboutToQuit()), this, SLOT(sendExit()));

    //printf("here");
    //Check scuffed bool
    if (ui->errorLabel->isVisible())
        return;

    // this actually helps im honestly not sure why
    // assumption is race condition in the receiver thread? but really unsure
    // investigate later
    sleep(1); //sleep for 1 just to see if that fixes extra spawns it shouldnt but whatever
    std::thread t1(&Network::receiver, std::ref(network_));
    t1.detach();

    hide();
    mainUi->show();
}

void ConnectUi::saveLast()
{
    QJsonObject obj;
    obj["Address"] = loginDetails.Address;
    obj["Port"] = loginDetails.Port;
    obj["Username"] = loginDetails.Username;

    QFile saveFile(fileName);
    saveFile.open(QIODevice::WriteOnly);
    saveFile.write(QJsonDocument(obj).toJson());
    saveFile.close();
}

bool ConnectUi::loadLast()
{
    QFile loadfile(fileName);
    loadfile.open(QIODevice::ReadOnly);

    auto fileIn = loadfile.readAll();
    if (fileIn.size() <= 0)
        return false;

    auto loadJDoc = QJsonDocument::fromJson(fileIn);
    auto jobj = loadJDoc.object();

    loginDetails.Address = jobj["Address"].toString();
    loginDetails.Port = jobj["Port"].toString();
    loginDetails.Username = jobj["Username"].toString();

    return true;
}

void ConnectUi::showErrorLabel(QString err)
{
    ui->errorLabel->setText(err);
    ui->errorLabel->setVisible(true);
    connected = false;
}

void ConnectUi::sendExit()
{
    network_->logAction("Exit");
    network_->sender(QString("exit\n"));
}