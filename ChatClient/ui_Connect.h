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
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConnectUi
{
public:
    QWidget *layoutWidget;
    QFormLayout *formLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *addressLine;
    QLineEdit *portLine;
    QPushButton *connectButton;
    QLineEdit *usernameLine;

    void setupUi(QWidget *ConnectUi)
    {
        if (ConnectUi->objectName().isEmpty())
            ConnectUi->setObjectName(QStringLiteral("ConnectUi"));
        ConnectUi->resize(334, 117);
        layoutWidget = new QWidget(ConnectUi);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 20, 282, 85));
        formLayout = new QFormLayout(layoutWidget);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        addressLine = new QLineEdit(layoutWidget);
        addressLine->setObjectName(QStringLiteral("addressLine"));

        horizontalLayout->addWidget(addressLine);

        portLine = new QLineEdit(layoutWidget);
        portLine->setObjectName(QStringLiteral("portLine"));

        horizontalLayout->addWidget(portLine);


        formLayout->setLayout(0, QFormLayout::LabelRole, horizontalLayout);

        connectButton = new QPushButton(layoutWidget);
        connectButton->setObjectName(QStringLiteral("connectButton"));

        formLayout->setWidget(2, QFormLayout::LabelRole, connectButton);

        usernameLine = new QLineEdit(layoutWidget);
        usernameLine->setObjectName(QStringLiteral("usernameLine"));

        formLayout->setWidget(1, QFormLayout::SpanningRole, usernameLine);


        retranslateUi(ConnectUi);

        QMetaObject::connectSlotsByName(ConnectUi);
    } // setupUi

    void retranslateUi(QWidget *ConnectUi)
    {
        ConnectUi->setWindowTitle(QApplication::translate("ConnectUi", "Form", Q_NULLPTR));
        addressLine->setInputMask(QString());
        addressLine->setText(QString());
        addressLine->setPlaceholderText(QApplication::translate("ConnectUi", "Address", Q_NULLPTR));
        portLine->setPlaceholderText(QApplication::translate("ConnectUi", "Port", Q_NULLPTR));
        connectButton->setText(QApplication::translate("ConnectUi", "Connect", Q_NULLPTR));
        usernameLine->setPlaceholderText(QApplication::translate("ConnectUi", "Username", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ConnectUi: public Ui_ConnectUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECT_H
