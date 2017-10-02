#include "widget.h"
#include "ui_widget.h"
#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QDate>
#include <QToolButton>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QPixmap(":/picture/background.png")));
    this->setPalette(palette);
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
    /*show time and date*/
    QTimer * timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(showTime()));
    timer->start (1000);        //每1000ms刷新一次，即1秒
    showTime();


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
       QString time_str= time.toString ("hh:mm:ss");     //以时：分：秒 方式显示
//       ui->lcdNumber->setGeometry (0,0,200,120);
       ui->lcdNumber->setDigitCount(8);
       ui->lcdNumber->setLineWidth(0);
       ui->lcdNumber->setPalette(pal);
       ui->lcdNumber->setLayoutDirection(Qt::RightToLeft);
       ui->lcdNumber->display (time_str);     //显示LCD文字

       /*show date*/
//       QDate date = QDate::currentDate();
//       QString date_str = date.toString("yyyy-MM"); // 转换成字符串
//       ui->lcdNumber_2->setDigitCount(10);
//       ui->lcdNumber_2->setLineWidth(0);
//       ui->lcdNumber_2->setPalette(pal);
//       ui->lcdNumber_2->display (date_str);     //显示LCD文字
}
