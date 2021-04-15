#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Qt>
// right click thingie and then do whisper from that just use same /w front format to allow
// in lineEdit whispers still
//stackoverflow.com/questions/31383519/qt-rightclick-on-qlistwidget-opens-contextmenu-and-delete-item
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::insertText(QString txt)
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
