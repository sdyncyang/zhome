#include "sunraise.h"

//sunraise::sunraise()
//{

//}

int sunraise::CalcJulianDay(int iMonth, int iDay, bool bLeapYr)
{
    int iJulDay = 0;

    if(bLeapYr)
    {
        for (int i = 0; i < iMonth - 1 ; i++)
            iJulDay += leapList[i];
    }
    else
    {
        for (int i = 0; i < iMonth - 1; i++)
            iJulDay += monthList[i];

    }

    iJulDay += iDay;

    return iJulDay;
}

//	The hour angle returned below is only for sunrise/sunset, i.e. when the solar zenith angle is 90.8 degrees.
// the reason why it's not 90 degrees is because we need to account for atmoshperic refraction.

double sunraise::CalcHourAngle(double dLat, double dSolarDec, bool bTime)
{

    /*
    var latRad = degToRad(lat);
    if (time) 		//	ii true, then calculationg for sunrise

    return (Math.acos(Math.cos(degToRad(90.833))/(Math.cos(latRad)*Math.cos(solarDec))-Math.tan(latRad) * Math.tan(solarDec)));

    else

    return -(Math.acos(Math.cos(degToRad(90.833))/(Math.cos(latRad)*Math.cos(solarDec))-Math.tan(latRad) * Math.tan(solarDec)));
    */


    double dLatRad = dDegToRad(dLat);
    if(bTime)  //Sunrise
    {
        /*		double dTemp = cos(dDegToRad(90.833));
        //		dTemp /= (cos(dLatRad)*cos(dSolarDec));
        dTemp /= ((cos(dLatRad)*cos(dSolarDec)) - (tan(dLatRad) * tan(dSolarDec)));
        //		dTemp -= (tan(dLatRad) * tan(dSolarDec));
        dTemp = acos(dTemp);
        return dTemp;
        */
        return  (acos(cos(dDegToRad(90.833))/(cos(dLatRad)*cos(dSolarDec))-tan(dLatRad) * tan(dSolarDec)));
    }
    else
    {
        /*		double dTemp = cos(dDegToRad(90.833));
        dTemp /= ((cos(dLatRad)*cos(dSolarDec)) - (tan(dLatRad) * tan(dSolarDec)));
        //dTemp -= (tan(dLatRad) * tan(dSolarDec));
        dTemp = acos(dTemp);
        return -dTemp;
        */
        return -(acos(cos(dDegToRad(90.833))/(cos(dLatRad)*cos(dSolarDec))-tan(dLatRad) * tan(dSolarDec)));
    }

}


double sunraise::calcSunsetGMT(int iJulDay, double dLatitude, double dLongitude)
{
    // First calculates sunrise and approx length of day

    double dGamma = CalcGamma(iJulDay + 1);
    double eqTime = CalcEqofTime(dGamma);
    double solarDec = CalcSolarDec(dGamma);
    double hourAngle = CalcHourAngle(dLatitude, solarDec, 0);
    double delta = dLongitude - dRadToDeg(hourAngle);
    double timeDiff = 4 * delta;
    double setTimeGMT = 720 + timeDiff - eqTime;

    // first pass used to include fractional day in gamma calc

    double gamma_sunset = CalcGamma2(iJulDay, (int)setTimeGMT/60);
    eqTime = CalcEqofTime(gamma_sunset);


    solarDec = CalcSolarDec(gamma_sunset);

    hourAngle = CalcHourAngle(dLatitude, solarDec, false);
    delta = dLongitude - dRadToDeg(hourAngle);
    timeDiff = 4 * delta;
    setTimeGMT = 720 + timeDiff - eqTime; // in minutes

    return setTimeGMT;
}

double sunraise::calcSunriseGMT(int iJulDay, double dLatitude, double dLongitude)
{
    // *** First pass to approximate sunrise

    double gamma = CalcGamma(iJulDay);
    double eqTime = CalcEqofTime(gamma);
    double solarDec = CalcSolarDec(gamma);
    double hourAngle = CalcHourAngle(dLatitude, solarDec, true);
    double delta = dLongitude - dRadToDeg(hourAngle);
    double timeDiff = 4 * delta;
    double timeGMT = 720 + timeDiff - eqTime;

    // *** Second pass includes fractional jday in gamma calc

    double gamma_sunrise = CalcGamma2(iJulDay, (int)timeGMT/60);
    eqTime = CalcEqofTime(gamma_sunrise);
    solarDec = CalcSolarDec(gamma_sunrise);
    hourAngle = CalcHourAngle(dLatitude, solarDec, 1);
    delta = dLongitude - dRadToDeg(hourAngle);
    timeDiff = 4 * delta;
    timeGMT = 720 + timeDiff - eqTime; // in minutes

    return timeGMT;
}

double  sunraise::calcSolNoonGMT(int iJulDay, double dLongitude)
{
    // Adds approximate fractional day to julday before calc gamma

    double gamma_solnoon = CalcGamma2(iJulDay, 12 + (int)(dLongitude/15));
    double eqTime = CalcEqofTime(gamma_solnoon);
    double solarNoonDec = CalcSolarDec(gamma_solnoon);
    double solNoonGMT = 720 + (dLongitude * 4) - eqTime; // min

    return solNoonGMT;
}


QTime sunraise::GetSunrise(double dLat,double dLon,QDateTime time)
{
    bool bLeap = IsLeapYear( (int)time.date().year());

    int iJulianDay = CalcJulianDay( (int)time.date().month(), (int)time.date().day(), bLeap );

    double timeGMT = calcSunriseGMT(iJulianDay, dLat,dLon);


    // if Northern hemisphere and spring or summer, use last sunrise and next sunset
    if ((dLat > 66.4) && (iJulianDay > 79) && (iJulianDay < 267))
        timeGMT = findRecentSunrise(iJulianDay, dLat, dLon);
    // if Northern hemisphere and fall or winter, use next sunrise and last sunset
    else if ((dLat > 66.4) && ((iJulianDay < 83) || (iJulianDay > 263)))
        timeGMT = findNextSunrise(iJulianDay, dLat, dLon);
    // if Southern hemisphere and fall or winter, use last sunrise and next sunset
    else if((dLat < -66.4) && ((iJulianDay < 83) || (iJulianDay > 263)))
        timeGMT = findRecentSunrise(iJulianDay, dLat, dLon);
    // if Southern hemisphere and spring or summer, use next sunrise and last sunset
    else if((dLat < -66.4) && (iJulianDay > 79) && (iJulianDay < 267))
        timeGMT = findNextSunrise(iJulianDay, dLat, dLon);
    //	else
    //	{
    //("Unaccountable Missing Sunrise!");
    //	}
    timeGMT+=(120-dLon)/360.0*48*60;

    double dHour = timeGMT / 60;
    int iHour = (int)dHour;
    double dMinute = 60 * (dHour - iHour);
    int iMinute = (int)dMinute;
    double dSecond = 60 * (dMinute - iMinute);
    int iSecond = (int)dSecond;

    QTime ret;
//    memcpy( &ret, *time, sizeof( QDateTime ) );
    ret.setHMS(iHour-8,iMinute,iSecond);
//    ret.wMilliseconds = 0;
    return ret;
}

QTime sunraise::GetSunset(double dLat,double dLon,QDateTime time)
{
    bool bLeap = IsLeapYear( time.date().year());

    int iJulianDay = CalcJulianDay( time.date().month(), time.date().day(), bLeap );

    double timeGMT = calcSunsetGMT(iJulianDay, dLat,dLon);

    // if Northern hemisphere and spring or summer, use last sunrise and next sunset
    if ((dLat > 66.4) && (iJulianDay > 79) && (iJulianDay < 267))
        timeGMT = findRecentSunset(iJulianDay, dLat, dLon);
    // if Northern hemisphere and fall or winter, use next sunrise and last sunset
    else if ((dLat > 66.4) && ((iJulianDay < 83) || (iJulianDay > 263)))
        timeGMT = findNextSunset(iJulianDay, dLat, dLon);
    // if Southern hemisphere and fall or winter, use last sunrise and next sunset
    else if((dLat < -66.4) && ((iJulianDay < 83) || (iJulianDay > 263)))
        timeGMT = findRecentSunset(iJulianDay, dLat, dLon);
    // if Southern hemisphere and spring or summer, use next sunrise and last sunset
    else if((dLat < -66.4) && (iJulianDay > 79) && (iJulianDay < 267))
        timeGMT = findNextSunset(iJulianDay, dLat, dLon);
    //	else
    //	{
    //("Unaccountable Missing Sunrise!");
    //	}
    timeGMT+=(120-dLon)/360.0*48*60;

    double dHour = timeGMT / 60;
    int iHour = (int)dHour;
    double dMinute = 60 * (dHour - iHour);
    int iMinute = (int)dMinute;
    double dSecond = 60 * (dMinute - iMinute);
    int iSecond = (int)dSecond;

    QTime ret;
//    CopyMemory( &ret, time, sizeof( SYSTEMTIME ) );
//    ret.wMilliseconds = 0;
//    ret.wHour         = (WORD)iHour - 8; // 8 for BJ time
//    ret.wMinute       = (WORD)iMinute;
//    ret.wSecond       = (WORD)iSecond;
//    ret = time;
//    memcpy( &ret, *time, sizeof( QDateTime ) );
    ret.setHMS(iHour-8,iMinute,iSecond);
    return ret;
}

double sunraise::findRecentSunrise(int iJulDay, double dLatitude, double dLongitude)
{
    int jday = iJulDay;

    double dTime = calcSunriseGMT(jday, dLatitude, dLongitude);

    while(!IsInteger(dTime) )
    {
        jday--;
        if (jday < 1)
            jday = 365;
        dTime = calcSunriseGMT(jday, dLatitude, dLongitude);
    }

    return jday;
}

bool sunraise::IsInteger(double dValue)
{
    int iTemp = (int)dValue;
    double dTemp = dValue - iTemp;
    if(dTemp == 0)
        return true;
    else
        return false;
}

double sunraise::findRecentSunset(int iJulDay, double dLatitude, double dLongitude)
{
    int jday = iJulDay;

    double dTime = calcSunsetGMT(jday, dLatitude, dLongitude);

    while(!IsInteger(dTime) )
    {
        jday--;
        if (jday < 1)
            jday = 365;
        dTime = calcSunsetGMT(jday, dLatitude, dLongitude);
    }

    return jday;
}


double sunraise::findNextSunrise(int iJulDay, double dLatitude, double dLongitude)
{
    int jday = iJulDay;

    double dTime = calcSunriseGMT(jday, dLatitude, dLongitude);

    while(!IsInteger(dTime) )
    {
        jday++;
        if (jday > 366)
            jday = 1;
        dTime = calcSunriseGMT(jday, dLatitude, dLongitude);
    }

    return jday;
}

double sunraise::findNextSunset(int iJulDay, double dLatitude, double dLongitude)
{
    int jday = iJulDay;

    double dTime = calcSunsetGMT(jday, dLatitude, dLongitude);

    while(!IsInteger(dTime) )
    {
        jday++;
        if (jday > 366)
            jday = 1;
        dTime = calcSunsetGMT(jday, dLatitude, dLongitude);
    }

    return jday;
}

