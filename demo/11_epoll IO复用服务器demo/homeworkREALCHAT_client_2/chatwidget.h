#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>

namespace Ui {
class ChatWidget;
}

class ChatWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWidget(QWidget *parent = nullptr);
    ~ChatWidget();

private:
    Ui::ChatWidget *ui; //UI
signals:

public slots:
};

#endif // CHATWIDGET_H
