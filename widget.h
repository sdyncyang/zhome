#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QNetworkReply>
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
    void showWifiSignal();
private slots:
    void showTime();
    void showWeahter();
    void finishedSlot(QNetworkReply *reply);
    void on_pushButton_clicked();
    void on_wifiPushButton_clicked();
};

#endif // WIDGET_H
