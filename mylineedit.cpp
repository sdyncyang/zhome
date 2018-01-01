#include "mylineedit.h"
#include <QDebug>
#include <keyboard/widgetKeyBoard.h>

MyLineEdit::MyLineEdit(QWidget *parent):QLineEdit(parent),myKeyboard(NULL)
{
    this->myKeyboard = new widgetKeyBoard(false);
    this->myKeyboard->setZoomFacility(true);
    this->myKeyboard->enableSwitchingEcho(false); // enable possibility to change echo through keyboard
    this->myKeyboard->createKeyboard(); // only create keyboard
//    ui->verticalLayout->addWidget(new QLabel(tr("Focus that below without virtual keyboard TAB:")));
//    ui->verticalLayout->addWidget(new exampleMyFocus(this->myKeyboard)); // example how-to voluntary focus (hand-focus or mouse-focus)
//      ui->verticalLayout->addWidget(new exampleMyFocus(this->myKeyboard)); // example how-to voluntary focus (hand-focus or mouse-focus)

}

MyLineEdit::~MyLineEdit()
{   
    delete (this->myKeyboard);
}

void MyLineEdit::focusInEvent(QFocusEvent *e)
{
//       QPalette p=QPalette();
//       p.setColor(QPalette::Base,Qt::green);    //QPalette::Base 对可编辑输入框有效，还有其他类型，具体的查看文档
//       setPalette(p);

//    widgetKeyBoard  *myKeyboard;
    myKeyboard->show(this);
}

void MyLineEdit::focusOutEvent(QFocusEvent *e)
{
//       QPalette p1=QPalette();
//       p1.setColor(QPalette::Base,Qt::white);
//       setPalette(p1);
    myKeyboard->hide(this);
}
