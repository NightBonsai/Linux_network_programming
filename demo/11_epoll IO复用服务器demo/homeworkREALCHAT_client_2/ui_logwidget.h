/********************************************************************************
** Form generated from reading UI file 'logwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGWIDGET_H
#define UI_LOGWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LogWidget
{
public:
    QPushButton *logButton;
    QPushButton *registButton;
    QPushButton *exitButton;
    QLabel *accountLabel;
    QLabel *passwordLabel;
    QLineEdit *accountEdit;
    QLineEdit *passwordEdit;

    void setupUi(QWidget *LogWidget)
    {
        if (LogWidget->objectName().isEmpty())
            LogWidget->setObjectName(QStringLiteral("LogWidget"));
        LogWidget->resize(400, 300);
        LogWidget->setMinimumSize(QSize(400, 300));
        logButton = new QPushButton(LogWidget);
        logButton->setObjectName(QStringLiteral("logButton"));
        logButton->setGeometry(QRect(40, 210, 89, 25));
        registButton = new QPushButton(LogWidget);
        registButton->setObjectName(QStringLiteral("registButton"));
        registButton->setGeometry(QRect(150, 210, 89, 25));
        exitButton = new QPushButton(LogWidget);
        exitButton->setObjectName(QStringLiteral("exitButton"));
        exitButton->setGeometry(QRect(260, 210, 89, 25));
        accountLabel = new QLabel(LogWidget);
        accountLabel->setObjectName(QStringLiteral("accountLabel"));
        accountLabel->setGeometry(QRect(40, 80, 67, 17));
        passwordLabel = new QLabel(LogWidget);
        passwordLabel->setObjectName(QStringLiteral("passwordLabel"));
        passwordLabel->setGeometry(QRect(30, 130, 67, 17));
        accountEdit = new QLineEdit(LogWidget);
        accountEdit->setObjectName(QStringLiteral("accountEdit"));
        accountEdit->setGeometry(QRect(130, 80, 231, 25));
        passwordEdit = new QLineEdit(LogWidget);
        passwordEdit->setObjectName(QStringLiteral("passwordEdit"));
        passwordEdit->setGeometry(QRect(130, 130, 231, 25));

        retranslateUi(LogWidget);

        QMetaObject::connectSlotsByName(LogWidget);
    } // setupUi

    void retranslateUi(QWidget *LogWidget)
    {
        LogWidget->setWindowTitle(QApplication::translate("LogWidget", "LogWidget", Q_NULLPTR));
        logButton->setText(QApplication::translate("LogWidget", "Log", Q_NULLPTR));
        registButton->setText(QApplication::translate("LogWidget", "Regist", Q_NULLPTR));
        exitButton->setText(QApplication::translate("LogWidget", "exit", Q_NULLPTR));
        accountLabel->setText(QApplication::translate("LogWidget", "Account", Q_NULLPTR));
        passwordLabel->setText(QApplication::translate("LogWidget", "Password", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class LogWidget: public Ui_LogWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGWIDGET_H
