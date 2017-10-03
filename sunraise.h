#ifndef SUNRAISE_H
#define SUNRAISE_H
#include <math.h>
#include <QTime>
#include <QDateTime>
#define PI 3.1415926

static int monthList[12] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static int leapList[12] = {
    31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

class sunraise
{
public:
//    sunraise();
    //Returns in GMT
    static QTime GetSunset(double dLat,double dLon,QDateTime time);
    static QTime GetSunrise(double dLat,double dLon,QDateTime time);

private:
    //	Convert radian angle to degrees
    static double dRadToDeg(double dAngleRad)
    {
        return (180 * dAngleRad / PI);
    }

    //	Convert degree angle to radians
    static double dDegToRad(double dAngleDeg)
    {
        return (PI * dAngleDeg / 180);
    }

    static double CalcGamma(int iJulianDay)
    {
        return (2 * PI / 365) * (iJulianDay - 1);
    }

    static double CalcGamma2(int iJulianDay, int iHour)
    {
        return (2 * PI / 365) * (iJulianDay - 1 + (iHour/24));
    }

    static double CalcEqofTime(double dGamma)
    {
        return (229.18 * (0.000075 + 0.001868 * cos(dGamma) - 0.032077 * sin(dGamma)- 0.014615 * cos(2 * dGamma) - 0.040849 *  sin(2 * dGamma)));
    }

    static double CalcSolarDec(double dGamma)
    {
        return (0.006918 - 0.399912 * cos(dGamma) + 0.070257 * sin(dGamma) - 0.006758 * cos(2 * dGamma) + 0.000907 *  sin(2 * dGamma));
    }

    static int CalcDayLength(double dHourAngle)
    {
        //	Return the length of the day in minutes.
        return (int)((2 * abs((int)dRadToDeg(dHourAngle))) / 15);

    }


    static int CalcJulianDay(int iMonth,int iDay, bool bLeapYr);

    static double CalcHourAngle(double dLat, double dSolarDec, bool bTime);

    static double calcSunsetGMT(int iJulDay, double dLatitude, double dLongitude);
    static double calcSunriseGMT(int iJulDay, double dLatitude, double dLongitude);
    static double calcSolNoonGMT(int iJulDay, double dLongitude);

    static double findRecentSunrise(int iJulDay, double dLatitude, double dLongitude);
    static double findRecentSunset(int iJulDay, double dLatitude, double dLongitude);

    static double findNextSunrise(int iJulDay, double dLatitude, double dLongitude);
    static double findNextSunset(int iJulDay, double dLatitude, double dLongitude);

    static bool IsInteger(double dValue);
    static bool IsLeapYear(int iYear)
    {
        return ((iYear % 4 == 0 && iYear % 100 != 0) || iYear % 400 == 0);
    }
};

#endif // SUNRAISE_H
