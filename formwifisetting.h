#ifndef FORMWIFISETTING_H
#define FORMWIFISETTING_H

#include <QWidget>
#include <QString>
namespace Ui {
class FormWifiSetting;
}

class FormWifiSetting : public QWidget
{
    Q_OBJECT

public:
    explicit FormWifiSetting(QWidget *parent = 0);
    ~FormWifiSetting();

private slots:
    void on_pushButton_2_clicked();

    void on_listWidget_clicked(const QModelIndex &index);

private:
    Ui::FormWifiSetting *ui;
    void showConnectWifi();
    void listScanResult();
};

class wifiInfo
{
public:
    QString macAddress ;
    QString SSID;
    QString freq;
    QString SSIDvalue;

};

#endif // FORMWIFISETTING_H
