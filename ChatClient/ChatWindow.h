#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <Network.h>

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = 0,Network* network = nullptr);
    void closeEvent(QCloseEvent *event);
    ~ChatWindow();

private:

    Network* network_;
    QMenu* menu_;
    Ui::ChatWindow *ui;

public slots:
    //Appends text to textEdit
    void insertText(QString txt);
    void HandleWhisperAction();
    void OpenMenu(QPoint pos);
    void Whisper();

};

#endif // MAINWINDOW_H
