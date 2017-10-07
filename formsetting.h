#ifndef FORMSETTING_H
#define FORMSETTING_H

#include <QWidget>
#include <QPushButton>
#include <QTimer>
namespace Ui {
class FormSetting;
}

class FormSetting : public QWidget
{
    Q_OBJECT

public:
    explicit FormSetting(QWidget *parent = 0);
    ~FormSetting();


private slots:
    void on_backButton_clicked();
    void updateTemp();
private:
    Ui::FormSetting *ui;
    QTimer * temptimer;
};

#endif // FORMSETTING_H
