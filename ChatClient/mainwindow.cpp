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

void MainWindow::uwu(QString fuckywucky)
{
    ui->textEdit->insertPlainText(fuckywucky + "\n");
}
