#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QCloseEvent>
#include <Qt>
namespace Ui
{
    class ChatWindow;
}

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = 0);
    void closeEvent(QCloseEvent *event);
    ~ChatWindow();

private:
    QMenu *menu_;
    Ui::ChatWindow *ui;

public slots:
    //Appends text to textEdit
    void insertText(QString txt);
    void OpenMenu(QPoint pos);
};

#endif // MAINWINDOW_H
