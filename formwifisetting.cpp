#include "formwifisetting.h"
#include "ui_formwifisetting.h"
#include <QFile>
#include <QDebug>
#include <QString>
#include <QRegExp>
#include <mylineedit.h>
#include <QGridLayout>
#include <QLineEdit>
FormWifiSetting::FormWifiSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormWifiSetting)
{
    ui->setupUi(this);
    this->setGeometry(270,0,760,600);
    //ui->scrollArea->setFrameShape(QFrame::NoFrame);
    ui->label_2->hide();
    ui->wifissidLabel->hide();
//    ui->wifipassword->hide();
   lineEdit1=new MyLineEdit(this);
   lineEdit1->setGeometry(400,150,300,50);
   lineEdit1->hide();
//   MyLineEdit lineEdit2=new MyLineEdit(this);
//   QGridLayout gridLayout=new QGridLayout;
//         gridLayout->addWidget(lineEdit1,0,0);
//         gridLayout->addWidget(lineEdit2,1,0);
//         setLayout(gridLayout);


    showConnectWifi();
    listScanResult();

//    QFile file("/home/pi/wifiScanTemp");
//    if(!file.open(QIODevice::ReadOnly| QIODevice::Text))
//       {
//          qDebug()<<file.errorString();
//       }
//    QString wifiScaninfo = file.readAll();
//    QStringList list = wifiScaninfo.split("BSS");

//    qDebug()<<list;
}

FormWifiSetting::~FormWifiSetting()
{
    delete ui;
}

void FormWifiSetting::on_pushButton_2_clicked()
{
    this->close();
}

void FormWifiSetting::showConnectWifi()
{
    system("iw dev wlan0 link > /home/qt_project/tempConnectedwifi");
    QFile templinkedfile("/home/qt_project/tempConnectedwifi");
        if(!templinkedfile.open(QIODevice::ReadOnly| QIODevice::Text))
           {
              qDebug()<<templinkedfile.errorString();
           }
    QString wifiConnectedinfo = templinkedfile.readAll();
//    qDebug()<<wifiConnectedinfo;
    wifiInfo info;
    QRegExp rx("(SSID:)(.*)(freq)");
    rx.indexIn(wifiConnectedinfo);
//    qDebug()<<rx.cap(0);
//    qDebug()<<rx.cap(1);
    info.SSID = rx.cap(2).trimmed();
    ui->linkedWifi->setText(info.SSID);
}

void FormWifiSetting::listScanResult()
{
   system("sudo iw dev wlan0 scan |grep -e 'SSID' -e 'signal' > /home/qt_project/wifiScanTemp");
   QFile tempScanfile("/home/qt_project/wifiScanTemp");
       if(!tempScanfile.open(QIODevice::ReadOnly| QIODevice::Text))
          {
             qDebug()<<tempScanfile.errorString();
          }
   QString tempRouterInfo = "";
   int i = 0;

   while(!tempScanfile.atEnd())
   {
        QString wifiScanResult = tempScanfile.readLine();
//        tempRouterInfo = tempRouterInfo.trimmed()+" "+wifiScanResult.trimmed();
        i++;
        if(i==2)
        {
          tempRouterInfo=wifiScanResult.trimmed();
          tempRouterInfo = tempRouterInfo.mid(5);
//        qDebug()<<tempRouterInfo;
//        QPushButton *btn = new QPushButton(this);
//        btn->setGeometry(50,j,300,30);
            ui->listWidget->addItem(tempRouterInfo);

        /*need add pushbutton*/
            tempRouterInfo = "";
            i=0;
        }

//        if(i==2)
//        {
//            i = 0;
//            qDebug()<<wifiScanResult;
//            tempRouterInfo = "";
//        }

   }
}

void FormWifiSetting::on_listWidget_clicked(const QModelIndex &index)
{
//    qDebug()<<index.row();
//    qDebug()<<ui->listWidget->item(index.row())->text();
      ui->wifissidLabel->show();
      ui->label_2->show();
//      ui->wifipassword->show();
      ui->wifissidLabel->setText(ui->listWidget->item(index.row())->text());
      lineEdit1->show();
      lineEdit1->setText("");
}

