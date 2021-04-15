#include "mainwindow.h"
#include "ui_mainwindow.h"
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
        ui->listWidget->addItem(new QListWidgetItem(txt));
        return;
    }
    ui->textEdit->insertPlainText(txt + "\n");
}
