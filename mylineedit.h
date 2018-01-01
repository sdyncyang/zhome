#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H
#include <QLineEdit>
#include<keyboard/widgetKeyBoard.h>
class MyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    MyLineEdit(QWidget *parent=0);
    ~MyLineEdit();
private:
    widgetKeyBoard  *myKeyboard;
protected:
       virtual void focusInEvent(QFocusEvent *e);
       virtual void focusOutEvent(QFocusEvent *e);
};

#endif // MYLINEEDIT_H
