#include "widget.h"
#include "ui_widget.h"
//#include "ui_setting.h"
#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QDate>
#include <QToolButton>
#include <sunraise.h>
#include <myinfo.h>
#include <QFile>
#include <QFileInfo>
#include <formsetting.h>
//#include <QRegExp>
#include <formwifisetting.h>
#define wifiSignalUpdateTime 5

/*myinfo setup*/
 myinfo *zaddinfo = new myinfo();
 int wifiCount = 0 ;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QPixmap(":/picture/background.png")));
    this->setPalette(palette);
    this->setWindowFlags(Qt::FramelessWindowHint);
    /*calandar*/
     QToolButton* pPreMonthButton = ui->calendarWidget->findChild<QToolButton*>("qt_calendar_prevmonth");
     if(NULL != pPreMonthButton)
        { QIcon iPrecon;
//         qDebug()<<pPreMonthButton->size();
          iPrecon.addFile(":/picture/left.png", pPreMonthButton->size());
          pPreMonthButton->setIcon(iPrecon);
        }
     QToolButton* pNextMonthButton = ui->calendarWidget->findChild<QToolButton*>("qt_calendar_nextmonth");
     if(NULL != pNextMonthButton)
        { QIcon iPrecon;
          iPrecon.addFile(":/picture/right.png", pNextMonthButton->size());
          pNextMonthButton->setIcon(iPrecon);
        }
//     QToolButton* pMonthButton = ui->calendarWidget->findChild<QToolButton*>("qt_calendar_monthbutton");
//     if(NULL != pMonthButton)
//        { QIcon iPrecon;
//          iPrecon.addFile(":/picture/right.png", pMonthButton->size());
//          pMonthButton->setIcon(iPrecon);
//        }





    /*show time and date*/
    QTimer * timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(showTime()));
    timer->start (1000);        //每1000ms刷新一次，即1秒
    showTime();
    showWifiSignal();


}

Widget::~Widget()
{
    delete ui;
}
void Widget::showTime()
{
        QPalette pal;
        pal.setColor(QPalette::WindowText, QColor(Qt::white));

        /*show time*/
       QTime time = QTime::currentTime ();    //获取系统当前时间
//       QString time_str= time.toString ("hh:mm:ss");     //以时：分：秒 方式显示
       QString time_str= time.toString ("hh:mm");     //以时：分：秒 方式显示
//       ui->lcdNumber->setGeometry (0,0,200,120);
//       ui->lcdNumber->setDigitCount(8);
//       ui->lcdNumber->setLineWidth(0);
//       ui->lcdNumber->setPalette(pal);
//       ui->lcdNumber->setLayoutDirection(Qt::RightToLeft);
//       ui->lcdNumber->display (time_str);     //显示LCD文字
//       ui->label->setText(time_str);
       ui->timePushButton->setText(time_str);
       QDate date = QDate::currentDate();
       QString currentweekday = date.toString("ddd");
//       qDebug()<<currentweekday;
       ui->label_2->setText(date.toString("MM月dd日 ") + zaddinfo->weekday.value(currentweekday));
//        ui->label_2->setText(zaddinfo->weekday.value(currentweekday));
//       QTime comptime;
//       comptime.setHMS(time.hour(),time.minute(),time.second());
       QTime sunr = sunraise::GetSunrise(zaddinfo->dLat,zaddinfo->dLon,QDateTime::currentDateTime());
       QTime suns = sunraise::GetSunset(zaddinfo->dLat,zaddinfo->dLon,QDateTime::currentDateTime());

       /*temp change*/
       if(time>=sunr & time <=suns)
       {
//           qDebug()<<"start";
            ui->frame->setStyleSheet("QFrame{border: 0px;background-image: url(:/picture/day.png);}");
//            QTime sunr = sunraise::GetSunrise(zaddinfo->dLat,zaddinfo->dLon,QDateTime::currentDateTime());
//            qDebug()<<sunr;
       }
       else
       {
           ui->frame->setStyleSheet("QFrame{border: 0px;background-image: url(:/picture/night.png);}");
//           qDebug()<<"night";

//            qDebug()<<suns;
       }
//       else
//       {
//           tempcount = 0;
//       }
       /*show date*/
//       QDate date = QDate::currentDate();
//       QString date_str = date.toString("yyyy-MM"); // 转换成字符串
//       ui->lcdNumber_2->setDigitCount(10);
//       ui->lcdNumber_2->setLineWidth(0);
//       ui->lcdNumber_2->setPalette(pal);
//       ui->lcdNumber_2->display (date_str);     //显示LCD文字

       /*read system temp*/
//       QFile file("/sys/class/thermal/thermal_zone0/temp");
//       if(!file.open(QIODevice::ReadOnly| QIODevice::Text))
//        {
//            qDebug()<<file.errorString();
//        }
//      ui->label_systemp->setText(file.readAll());
//      file.close();
    wifiCount++;
    if(wifiCount>=wifiSignalUpdateTime)
    {
        showWifiSignal();
        wifiCount = 0;
    }
}

void Widget::showWeahter()
{

}


void Widget::showWifiSignal()
{

    system("iwconfig wlan0 | grep \"Link Quality\" > ./tempWifiSignal");
    QFile file("/home/qt_project/tempWifiSignal");
    if(!file.open(QIODevice::ReadOnly| QIODevice::Text))
       {
          qDebug()<<file.errorString();
       }
    QString wifiSignal = file.readAll();
    wifiSignal = wifiSignal.simplified();
    wifiSignal = wifiSignal.mid(13,2);
    int iwifiSignal = wifiSignal.toInt();
//    qDebug()<<iwifiSignal;
    if(iwifiSignal>75)
           {
//             QPixmap pixmapWireless(":/picture/WirelessIcon.png");
             ui->wifiPushButton->setStyleSheet("border-image: url(:/picture/WirelessIcon.png);outline: none;");
            }
    else if(iwifiSignal>50&&iwifiSignal<=75)
            {
//             QPixmap pixmapWireless(":/picture/WirelessIcon1.png");
             ui->wifiPushButton->setStyleSheet("border-image: url(:/picture/WirelessIcon1.png);outline: none;");
            }
    else if(iwifiSignal>25&&iwifiSignal<=50)
            {
 //            QPixmap pixmapWireless(":/picture/WirelessIcon2.png");
             ui->wifiPushButton->setStyleSheet("border-image: url(:/picture/WirelessIcon2.png);outline: none;");
            }
    else if(iwifiSignal>0&&iwifiSignal<=25)
            {
//             QPixmap pixmapWireless(":/picture/WirelessIcon3.png");
            ui->wifiPushButton->setStyleSheet("border-image: url(:/picture/WirelessIcon3.png);outline: none;");
            }
    else
            {
               ui->wifiPushButton->setStyleSheet("border-image: url(:/picture/WirelessIcon4.png);outline: none;");
            }


}
void Widget::on_pushButton_clicked()
{
    qDebug()<<"clicked";
    FormSetting *w2 = new FormSetting();
    w2->show();
}

void Widget::on_wifiPushButton_clicked()
{
    FormWifiSetting *wifiWindown = new FormWifiSetting();
    wifiWindown->show();

}
