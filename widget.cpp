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
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

#define wifiSignalUpdateTime 5
#define weatherUpdateTime 1000*60
/*myinfo setup*/
 myinfo *zaddinfo = new myinfo();
 int wifiCount = 0 ;
 QNetworkAccessManager *m_accessManager_weatherdate;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    QPalette palette;
    m_accessManager_weatherdate = new QNetworkAccessManager(this);
    connect(m_accessManager_weatherdate,SIGNAL(finished(QNetworkReply*)),this,SLOT(finishedSlot(QNetworkReply*)));
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

     /*clear weather date background*/
  ui->CityTextEdit->setAttribute(Qt::WA_TranslucentBackground, true);
  ui->DaytemptextEdit->setAttribute(Qt::WA_TranslucentBackground, true);
  ui->NighttemptextEdit->setAttribute(Qt::WA_TranslucentBackground, true);
  ui->weathertextEdit->setAttribute(Qt::WA_TranslucentBackground, true);


    /*show time and date*/
    QTimer * timer = new QTimer(this);
    QTimer * timerweather = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(showTime()));
    connect(timerweather,SIGNAL(timeout()),this,SLOT(showWeahter()));
    timer->start (1000);        //每1000ms刷新一次，即1秒
    timerweather->start(weatherUpdateTime);
    showTime();
    showWeahter();
    showWifiSignal();


}

Widget::~Widget()
{
    delete ui;
    delete m_accessManager_weatherdate;
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
    QNetworkRequest request;
    request.setUrl(QUrl("http://www.for-maker.com/weather"));
    m_accessManager_weatherdate->get(request);
}


void Widget::showWifiSignal()
{

    system("iwconfig wlan0 | grep \"Link Quality\" > /home/qt_project/tempWifiSignal");
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

void Widget::finishedSlot(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError)
         {
             QByteArray weatherBytes = reply->readAll();
             //QString weather_string = QString::fromUtf8(bytes);
             //ui->textBrowser->setText(string.toUtf8());
             QJsonParseError jsonError;
             QJsonDocument parseDoc = QJsonDocument::fromJson(weatherBytes,&jsonError);
             if(jsonError.error == QJsonParseError::NoError)
             {
                 if(parseDoc.isObject())
                   {
                     QJsonObject jsonObj = parseDoc.object();
                     if(jsonObj.contains("city"))
                        {
                          QJsonValue typeValue = jsonObj.take("city");
                          if(typeValue.isString())
                          {
                          QString strValue= typeValue.toString();
                          ui->CityTextEdit->setAttribute(Qt::WA_TranslucentBackground, true);
                          ui->CityTextEdit->setText(strValue);
                         }
                        }
                     if(jsonObj.contains("day_temp"))
                        {
                          QJsonValue typeValue = jsonObj.take("day_temp");
                          if(typeValue.isString())
                          {
                          QString strValue= typeValue.toString();
                          ui->DaytemptextEdit->setAttribute(Qt::WA_TranslucentBackground, true);
                          ui->DaytemptextEdit->setText("白天温度:"+strValue);
                         }
                        }
                     if(jsonObj.contains("night_temp"))
                        {
                          QJsonValue typeValue = jsonObj.take("night_temp");
                          if(typeValue.isString())
                          {
                          QString strValue= typeValue.toString();
                          ui->NighttemptextEdit->setAttribute(Qt::WA_TranslucentBackground, true);
                          ui->NighttemptextEdit->setText("夜晚温度:"+strValue);
                         }
                        }
                     if(jsonObj.contains("weather"))
                        {
                          QJsonValue typeValue = jsonObj.take("weather");
                          if(typeValue.isString())
                          {
                          QString strValue= typeValue.toString();
                          ui->weathertextEdit->setAttribute(Qt::WA_TranslucentBackground, true);
                          ui->weathertextEdit->setText(strValue);
                         }
                        }
                 }
             }
             else
             {
                qDebug()<<"json error";
             }
         }
         else
         {
             qDebug()<<"handle errors here";
             QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
             //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
             qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
             qDebug(qPrintable(reply->errorString()));
         }
         reply->deleteLater();
}
