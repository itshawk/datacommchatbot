/********************************************************************************
** Form generated from reading UI file 'Connect.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECT_H
#define UI_CONNECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
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
            ConnectUi->setObjectName(QStringLiteral("ConnectUi"));
        ConnectUi->resize(334, 117);
        verticalLayout = new QVBoxLayout(ConnectUi);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        addressLine = new QLineEdit(ConnectUi);
        addressLine->setObjectName(QStringLiteral("addressLine"));

        horizontalLayout->addWidget(addressLine);

        portLine = new QLineEdit(ConnectUi);
        portLine->setObjectName(QStringLiteral("portLine"));

        horizontalLayout->addWidget(portLine);


        verticalLayout->addLayout(horizontalLayout);

        usernameLine = new QLineEdit(ConnectUi);
        usernameLine->setObjectName(QStringLiteral("usernameLine"));

        verticalLayout->addWidget(usernameLine);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        connectButton = new QPushButton(ConnectUi);
        connectButton->setObjectName(QStringLiteral("connectButton"));

        horizontalLayout_2->addWidget(connectButton);

        errorLabel = new QLabel(ConnectUi);
        errorLabel->setObjectName(QStringLiteral("errorLabel"));
        errorLabel->setStyleSheet(QStringLiteral("QLabel { color : red; }"));
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
        ConnectUi->setWindowTitle(QApplication::translate("ConnectUi", "Connect", Q_NULLPTR));
        addressLine->setInputMask(QString());
        addressLine->setText(QString());
        addressLine->setPlaceholderText(QApplication::translate("ConnectUi", "Address", Q_NULLPTR));
        portLine->setPlaceholderText(QApplication::translate("ConnectUi", "Port", Q_NULLPTR));
        usernameLine->setPlaceholderText(QApplication::translate("ConnectUi", "Username", Q_NULLPTR));
        connectButton->setText(QApplication::translate("ConnectUi", "Connect", Q_NULLPTR));
        errorLabel->setText(QApplication::translate("ConnectUi", "Failed to connect!", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ConnectUi: public Ui_ConnectUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECT_H
