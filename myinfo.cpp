#include "myinfo.h"

myinfo::myinfo()
{
    mycity = "shanghai";
    dLat =   31.2;//should read from database
    dLon =   121.4;
    weekday.insert("周一","星期一");
    weekday.insert("周二","星期二");
    weekday.insert("周三","星期三");
    weekday.insert("周四","星期四");
    weekday.insert("周五","星期五");
    weekday.insert("周六","星期六");
    weekday.insert("周日","星期日");
}


