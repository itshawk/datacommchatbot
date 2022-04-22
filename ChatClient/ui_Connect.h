/********************************************************************************
** Form generated from reading UI file 'Connect.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECT_H
#define UI_CONNECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConnectUi
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *addressLine;
    QLineEdit *portLine;
    QLineEdit *usernameLine;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *connectButton;
    QLabel *errorLabel;

    void setupUi(QWidget *ConnectUi)
    {
        if (ConnectUi->objectName().isEmpty())
            ConnectUi->setObjectName(QString::fromUtf8("ConnectUi"));
        ConnectUi->resize(334, 117);
        verticalLayout = new QVBoxLayout(ConnectUi);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        addressLine = new QLineEdit(ConnectUi);
        addressLine->setObjectName(QString::fromUtf8("addressLine"));

        horizontalLayout->addWidget(addressLine);

        portLine = new QLineEdit(ConnectUi);
        portLine->setObjectName(QString::fromUtf8("portLine"));

        horizontalLayout->addWidget(portLine);


        verticalLayout->addLayout(horizontalLayout);

        usernameLine = new QLineEdit(ConnectUi);
        usernameLine->setObjectName(QString::fromUtf8("usernameLine"));

        verticalLayout->addWidget(usernameLine);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        connectButton = new QPushButton(ConnectUi);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));

        horizontalLayout_2->addWidget(connectButton);

        errorLabel = new QLabel(ConnectUi);
        errorLabel->setObjectName(QString::fromUtf8("errorLabel"));
        errorLabel->setStyleSheet(QString::fromUtf8("QLabel { color : red; }"));
        errorLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(errorLabel);


        verticalLayout->addLayout(horizontalLayout_2);

        QWidget::setTabOrder(addressLine, portLine);
        QWidget::setTabOrder(portLine, usernameLine);
        QWidget::setTabOrder(usernameLine, connectButton);

        retranslateUi(ConnectUi);

        QMetaObject::connectSlotsByName(ConnectUi);
    } // setupUi

    void retranslateUi(QWidget *ConnectUi)
    {
        ConnectUi->setWindowTitle(QApplication::translate("ConnectUi", "Connect", nullptr));
        addressLine->setInputMask(QString());
        addressLine->setText(QString());
        addressLine->setPlaceholderText(QApplication::translate("ConnectUi", "Address", nullptr));
        portLine->setPlaceholderText(QApplication::translate("ConnectUi", "Port", nullptr));
        usernameLine->setPlaceholderText(QApplication::translate("ConnectUi", "Username", nullptr));
        connectButton->setText(QApplication::translate("ConnectUi", "Connect", nullptr));
        errorLabel->setText(QApplication::translate("ConnectUi", "Failed to connect!", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConnectUi: public Ui_ConnectUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECT_H
