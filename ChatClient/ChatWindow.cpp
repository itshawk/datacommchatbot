#include "ChatWindow.h"
#include "ui_Chatwindow.h"
#include <Qt>
#include <QCloseEvent>
#include <QMessageBox>
#include <iostream>

void ChatWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Chat Client",
                                                               tr("Are you sure?\n"),
                                                               QMessageBox::No | QMessageBox::Yes,
                                                               QMessageBox::Yes);

    if (resBtn != QMessageBox::Yes)
    {
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

/*
    Move whisper to own ui file
*/


ChatWindow::ChatWindow(QWidget *parent,Network* network) : QMainWindow(parent),
                                          ui(new Ui::ChatWindow)
{
    network_ = network;
    ui->setupUi(this);
    ui->textEdit->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);

    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    //Right-click menu handling
    connect(ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(OpenMenu(QPoint)));
    //Send message
    connect(ui->lineEdit, &QLineEdit::returnPressed,
            network, [=] {network->send(ui->lineEdit->text());ui->lineEdit->clear(); });
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::Whisper()
{
    disconnect(ui->lineEdit,&QLineEdit::returnPressed,nullptr,nullptr);


    auto name = ui->listWidget->selectedItems()[0]->text();
    network_->send("/w "+ name + " " +ui->lineEdit->text());

    ui->lineEdit->clear();
    ui->lineEdit->setPlaceholderText(" ");

    connect(ui->lineEdit, &QLineEdit::returnPressed,
            network_, [=] {network_->send(ui->lineEdit->text());ui->lineEdit->clear(); });
}

void ChatWindow::HandleWhisperAction()
{

    disconnect(ui->lineEdit,&QLineEdit::returnPressed,nullptr,nullptr);
    connect(ui->lineEdit,&QLineEdit::returnPressed,this,&ChatWindow::Whisper);
    ui->lineEdit->setPlaceholderText("Whispering " + ui->listWidget->selectedItems()[0]->text());

}
void ChatWindow::OpenMenu(QPoint pos)
{
    menu_ = new QMenu;
    menu_->addAction("Whisper");
    connect(menu_,SIGNAL(triggered(QAction*)),SLOT(HandleWhisperAction()));

    if (ui->listWidget->itemAt(pos))

    if(ui->listWidget->itemAt(pos))
        menu_->popup(ui->listWidget->viewport()->mapToGlobal(pos));
}

void ChatWindow::insertText(QString txt)
{
    fprintf(stderr, "Received msg: %s\n", txt.toLocal8Bit().constData());

    if (txt[0] == 'c')
    {
        txt.remove(0, 1);

        // it was server side i want death
        // DUMBASS THEY ARE POINTERS
        // sometimes we get doubled ups on this, dont know why
        // when 4+ users connect it starts duping stuff

        ui->listWidget->addItem(new QListWidgetItem(txt));

        return;
    }
    else if (txt[0] == 'r')
    {
        txt.remove(0, 1);
        for (int i = 0; i < ui->listWidget->count(); i++)
        {
            if (ui->listWidget->item(i)->text() == txt)
            {
                ui->listWidget->takeItem(i);
            }
        }
    }
    else
    {
        txt.remove(0, 1);
        ui->textEdit->insertPlainText(txt + "\n");
    }
}
