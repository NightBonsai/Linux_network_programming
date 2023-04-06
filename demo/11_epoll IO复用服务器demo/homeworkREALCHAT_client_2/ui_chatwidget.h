/********************************************************************************
** Form generated from reading UI file 'chatwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATWIDGET_H
#define UI_CHATWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChatWidget
{
public:
    QListWidget *friendList;
    QLineEdit *chatEdit;
    QTextEdit *chatWidget;
    QPushButton *send;

    void setupUi(QWidget *ChatWidget)
    {
        if (ChatWidget->objectName().isEmpty())
            ChatWidget->setObjectName(QStringLiteral("ChatWidget"));
        ChatWidget->resize(580, 400);
        friendList = new QListWidget(ChatWidget);
        friendList->setObjectName(QStringLiteral("friendList"));
        friendList->setGeometry(QRect(30, 20, 121, 331));
        chatEdit = new QLineEdit(ChatWidget);
        chatEdit->setObjectName(QStringLiteral("chatEdit"));
        chatEdit->setGeometry(QRect(180, 280, 371, 71));
        chatWidget = new QTextEdit(ChatWidget);
        chatWidget->setObjectName(QStringLiteral("chatWidget"));
        chatWidget->setGeometry(QRect(180, 20, 371, 251));
        send = new QPushButton(ChatWidget);
        send->setObjectName(QStringLiteral("send"));
        send->setGeometry(QRect(180, 360, 89, 25));

        retranslateUi(ChatWidget);

        QMetaObject::connectSlotsByName(ChatWidget);
    } // setupUi

    void retranslateUi(QWidget *ChatWidget)
    {
        ChatWidget->setWindowTitle(QApplication::translate("ChatWidget", "chatWidget", Q_NULLPTR));
        send->setText(QApplication::translate("ChatWidget", "\345\217\221\351\200\201", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ChatWidget: public Ui_ChatWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATWIDGET_H
