#include "formsetting.h"
#include "ui_formsetting.h"
#include <QTimer>
#include <QFile>
#include <QDebug>

FormSetting::FormSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormSetting)
{
    ui->setupUi(this);
    this->setGeometry(270,0,760,600);
    ui->backButton->setStyleSheet("border-image: url(:/picture/Back.png);outline: none;");

//    this->setStyleSheet("QWidget{border: 0px;background-image: url(:/picture/day.png);}");
//    ui->backButton->setGeometry(30,30,100,30);
    temptimer= new QTimer(this);
    connect(temptimer,SIGNAL(timeout()),this,SLOT(updateTemp()));
    temptimer->start (5000);        //每1000ms刷新一次，即1秒
    updateTemp();
}

FormSetting::~FormSetting()
{

    delete ui;
}

void FormSetting::on_backButton_clicked()
{
    temptimer->stop();
    delete temptimer;
    this->close();
}

void FormSetting::updateTemp()
{
           QFile file("/sys/class/thermal/thermal_zone0/temp");
           if(!file.open(QIODevice::ReadOnly| QIODevice::Text))
            {
                qDebug()<<file.errorString();
            }
          ui->label_systemp->setText(file.readAll());
          file.close();

}
