#ifndef WHISPER_H
#define WHISPER_H

#include <QMainWindow>
#include <QObject>
#include "ui_whisper.h"


/*


*/

class Whisper : public QWidget
{


    Q_OBJECT

private:
    Ui_Form *ui;
public:
    explicit Whisper(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // WHISPER_H
