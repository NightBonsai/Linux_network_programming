#include <QApplication>
#include "logwidget.h"
#include "chatwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LogWidget lw;
    lw.show();

//    ChatWidget cw;
//    cw.show();

    return a.exec();
}
