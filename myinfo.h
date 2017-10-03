#ifndef MYINFO_H
#define MYINFO_H
#include "QString"
#include <QHash>
class myinfo
{
public:
    myinfo();
    QString mycity;
    double dLat;
    double dLon;
    QHash<QString, QString> weekday;


};

#endif // MYINFO_H
