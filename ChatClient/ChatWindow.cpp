#include "ChatWindow.h"
#include "ui_Chatwindow.h"
#include <Qt>

ChatWindow::ChatWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    ui->textEdit->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);

    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget,SIGNAL(customContextMenuRequested(QPoint)),
                            SLOT(OpenMenu(QPoint)));
}

ChatWindow::~ChatWindow()
{
    delete ui;
}


void ChatWindow::OpenMenu(QPoint pos)
{
    menu_ = new QMenu;
    menu_->addAction("Whisper");

    if(ui->listWidget->itemAt(pos))
        menu_->popup(ui->listWidget->viewport()->mapToGlobal(pos));

}

void ChatWindow::insertText(QString txt)
{
    fprintf(stderr, "insert text qstring txt: %s\n", txt.toLocal8Bit().constData());

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
