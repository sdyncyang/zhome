#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    void showWeahter();
    void showWifiSignal();
private slots:
    void showTime();

    void on_pushButton_clicked();
    void on_wifiPushButton_clicked();
};

#endif // WIDGET_H
