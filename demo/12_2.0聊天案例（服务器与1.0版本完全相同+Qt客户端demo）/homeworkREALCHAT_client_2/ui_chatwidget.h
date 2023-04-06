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
    QPushButton *send;
    QTextEdit *textEdit;

    void setupUi(QWidget *ChatWidget)
    {
        if (ChatWidget->objectName().isEmpty())
            ChatWidget->setObjectName(QStringLiteral("ChatWidget"));
        ChatWidget->resize(580, 400);
        friendList = new QListWidget(ChatWidget);
        QIcon icon;
        icon.addFile(QStringLiteral("photo/1.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(friendList);
        __qlistwidgetitem->setIcon(icon);
        QIcon icon1;
        icon1.addFile(QStringLiteral("photo/2.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem1 = new QListWidgetItem(friendList);
        __qlistwidgetitem1->setIcon(icon1);
        QIcon icon2;
        icon2.addFile(QStringLiteral("photo/3.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem2 = new QListWidgetItem(friendList);
        __qlistwidgetitem2->setIcon(icon2);
        QIcon icon3;
        icon3.addFile(QStringLiteral("photo/4.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem3 = new QListWidgetItem(friendList);
        __qlistwidgetitem3->setIcon(icon3);
        QIcon icon4;
        icon4.addFile(QStringLiteral("photo/5.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem4 = new QListWidgetItem(friendList);
        __qlistwidgetitem4->setIcon(icon4);
        friendList->setObjectName(QStringLiteral("friendList"));
        friendList->setGeometry(QRect(30, 20, 121, 331));
        chatEdit = new QLineEdit(ChatWidget);
        chatEdit->setObjectName(QStringLiteral("chatEdit"));
        chatEdit->setGeometry(QRect(180, 280, 371, 71));
        send = new QPushButton(ChatWidget);
        send->setObjectName(QStringLiteral("send"));
        send->setGeometry(QRect(180, 360, 89, 25));
        textEdit = new QTextEdit(ChatWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(180, 20, 371, 251));

        retranslateUi(ChatWidget);

        QMetaObject::connectSlotsByName(ChatWidget);
    } // setupUi

    void retranslateUi(QWidget *ChatWidget)
    {
        ChatWidget->setWindowTitle(QApplication::translate("ChatWidget", "chatWidget", Q_NULLPTR));

        const bool __sortingEnabled = friendList->isSortingEnabled();
        friendList->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = friendList->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("ChatWidget", "1001", Q_NULLPTR));
        QListWidgetItem *___qlistwidgetitem1 = friendList->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("ChatWidget", "1002", Q_NULLPTR));
        QListWidgetItem *___qlistwidgetitem2 = friendList->item(2);
        ___qlistwidgetitem2->setText(QApplication::translate("ChatWidget", "1003", Q_NULLPTR));
        QListWidgetItem *___qlistwidgetitem3 = friendList->item(3);
        ___qlistwidgetitem3->setText(QApplication::translate("ChatWidget", "1004", Q_NULLPTR));
        QListWidgetItem *___qlistwidgetitem4 = friendList->item(4);
        ___qlistwidgetitem4->setText(QApplication::translate("ChatWidget", "1005", Q_NULLPTR));
        friendList->setSortingEnabled(__sortingEnabled);

        send->setText(QApplication::translate("ChatWidget", "\345\217\221\351\200\201", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ChatWidget: public Ui_ChatWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATWIDGET_H
