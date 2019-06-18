/*
 * CtrlAlgo.c
 *
 *  Created on: 2018年1月23日
 *      Author: Ezreal
 */

/************************************************************
Description:用户定义控制算法函数
************************************************************/

#include "F28x_Project.h"
#include "CtrlParams.h"

    extern float ob2, ob3;

#if PHASE == 3
void ClarkTrans(float abc[], float ab0[]);
void IClarkTrans(float ab0[], float abc[]);
void ParkTrans(float ab0[], float theta, float dq0[]);
void IParkTrans(float dq0[], float theta, float ab0[]);
void CtrlAlgo(float ud, float uq, float ucap, float idq0[], float udc[], float theta, float duty[])            //DTC-OEW-UP
//void CtrlAlgo(float ud, float uq, float udc[], float theta, float duty[])   //SVPWM
//void CtrlAlgo(float iq, float iabc[], float theta, float duty[])            //Bang-Bang
{
    /************************************************************
    Description:三相FOC_SVPWM
    Input:ud, uq, theta
    Output:duty[]
    ************************************************************/
/*SVPWM
    float udq0[3] = {ud, uq, 0}, uab0[3] = {0}, uabc[3] = {0};
    float udcavg = 0.5 * (udc[0] + udc[1]);
    float umax = 0, umin = 0;

    IParkTrans(udq0, theta, uab0);
    IClarkTrans(uab0, uabc);

    umax = MAX(uabc[0], MAX(uabc[1], uabc[2]));
    umin = MIN(uabc[0], MIN(uabc[1], uabc[2]));

    duty[0] = (uabc[0] - 0.5 * (umax + umin)) / udcavg + 0.5;
    duty[1] = (uabc[1] - 0.5 * (umax + umin)) / udcavg + 0.5;
    duty[2] = (uabc[2] - 0.5 * (umax + umin)) / udcavg + 0.5;

    int i = 0;
    for(i = 0; i < 3; i++)
    {
        if(duty[i] > 1)
            duty[i] = 1;
        else if(duty[i] < 0)
            duty[i] = 0;
    }
*/

///*SVPWM 开绕组
    float udq0[3] = {ud, uq, 0}, uab0[3] = {0}, uabc[3] = {0};
    float umax = 0, umin = 0, u0 = 0;
    float power = 0, itemp = 0;
    float udq0temp[3] = {0};


    power = udq0[0] * idq0[0] + udq0[1] * idq0[1];
    itemp = idq0[0] * idq0[0] + idq0[1] * idq0[1];

//*/

// ABC三边占空比计算
//    udq0[0] = 0.5 * udq0[0];
//    udq0[1] = 0.5 * udq0[1];
//    udq0[2] = 0;

//    IParkTrans(udq0, theta, uab0);
    udq0temp[0] = idq0[0] / itemp * power + ucap;
    udq0temp[1] = idq0[1] / itemp * power + ucap;


    IParkTrans(udq0temp, theta, uab0);
    IClarkTrans(uab0, uabc);

    umax = MAX(uabc[0], MAX(uabc[1], uabc[2]));
    umin = MIN(uabc[0], MIN(uabc[1], uabc[2]));
//    u0 = -0.5 * (umax + umin);
    if((umax + umin) >= 0)
    {
        u0 = udc[0] - umax;
    }
    else
    {
        u0 = -udc[0] - umin;
    }

    duty[0] = (uabc[0] + u0) / udc[0] + 0.5;
    duty[1] = (uabc[1] + u0) / udc[0] + 0.5;
    duty[2] = (uabc[2] + u0) / udc[0] + 0.5;

// DEF三边占空比计算
//    udq0[0] = -1 * udq0[0];
//    udq0[1] = -1 * udq0[1];
//    udq0[2] = 0;

//    IParkTrans(udq0, theta, uab0);
//    uab0temp[0] = uab0temp[0] - uab0[0];
//    uab0temp[1] = uab0temp[1] - uab0[1];

    udq0temp[0] = udq0temp[0] - udq0[0];
    udq0temp[1] = udq0temp[1] - udq0[1];
    udq0temp[2] = 0;

    IParkTrans(udq0temp, theta, uab0);
    IClarkTrans(uab0, uabc);

    umax = MAX(uabc[0], MAX(uabc[1], uabc[2]));
    umin = MIN(uabc[0], MIN(uabc[1], uabc[2]));
//    u0 = -0.5 * (umax + umin);
    if((umax + umin) >= 0)
    {
        u0 = -udc[1] - umin;
    }
    else
    {
        u0 = udc[1] - umax;
    }

    duty[3] = (uabc[0] + u0) / udc[1] + 0.5;
    duty[4] = (uabc[1] + u0) / udc[1] + 0.5;
    duty[5] = (uabc[2] + u0) / udc[1] + 0.5;

    int i = 0;
    for(i = 0; i < 6; i++)
    {
        if(duty[i] > 1)
            duty[i] = 1;
        else if(duty[i] < 0)
            duty[i] = 0;
    }
//*/

/*Bang-Bang
    float idq0ref[3] = {GIVEN_ID, iq, 0}, iabcref[3] = {0}, iab0ref[3] = {0};

    IParkTrans(idq0ref, theta, iab0ref);
    IClarkTrans(iab0ref, iabcref);

    int i = 0;
    for(i = 0; i < 3; i++)
    {
        if(iabcref[i] > iabc[i])
            duty[i] = 1;
        else
            duty[i] = 0;
    }
*/

/*
    float ualpha, ubeta;
    float x, y, z, t1, t2;
    int sector;

//    udc = BusVoltSamp();
    ualpha = uab0temp[0];//ud * cos(theta) - uq * sin(theta);
    ubeta = uab0temp[1];//ud * sin(theta) + uq * cos(theta);

    // 扇区判断
    if (ubeta >= 0 && (ubeta - SQRT3 * ualpha) < 0)
        sector = 1;
    else if ((ubeta - SQRT3 * ualpha) >= 0 && (ubeta + SQRT3 * ualpha) > 0)
        sector = 2;
    else if ((ubeta + SQRT3 * ualpha) <= 0 && ubeta > 0)
        sector = 3;
    else if (ubeta <= 0 && (ubeta - SQRT3 * ualpha) > 0)
        sector = 4;
    else if ((ubeta - SQRT3 * ualpha) <= 0 && (ubeta + SQRT3 * ualpha) < 0)
        sector = 5;
    else if ((ubeta + SQRT3 * ualpha) >= 0 && ubeta < 0)
        sector = 6;
    else
        sector = 1;
    //扇区判断(132645)
//    float ua, ub, uc;
//    int a, b, c, n;
//
//    ua = ualpha * _2TO3;
//    ub = -ualpha * _1TO3 + ubeta * SQRT3TO3;
//    uc = -ualpha * _1TO3 - ubeta * SQRT3TO3;
//
//    if(ua >= 0)
//        a = 1;
//    else
//        a = 0;
//
//    if(ub >= 0)
//        b = 1;
//    else
//        b = 0;
//
//    if(uc >= 0)
//        c = 1;
//    else
//        c = 0;
//    n = a + 2 * b + 4 * c;
    //end

    x = SQRT3TO2 / udc[0] * 2 * ubeta;
    y = SQRT3TO2 / udc[0] * (SQRT3 * ualpha - ubeta);
    z = SQRT3TO2 / udc[0] * (SQRT3 * ualpha + ubeta);

    switch (sector)
    {
        case 1:
            t1 = y;
            t2 = x;

            duty[2] = (1 - t1 - t2) / 2;
            duty[1] = duty[2] + t2 / 2;
            duty[0] = duty[1] + t1 / 2;
            break;
        case 2:
            t1 = -y;
            t2 = z;

            duty[2] = (1 - t1 - t2) / 2;
            duty[0] = duty[2] + t2 / 2;
            duty[1] = duty[0] + t1 / 2;
            break;
        case 3:
            t1 = x;
            t2 = -z;

            duty[0] = (1 - t1 - t2) / 2;
            duty[2] = duty[0] + t2 / 2;
            duty[1] = duty[2] + t1 / 2;
            break;
        case 4:
            t1 = -x;
            t2 = -y;

            duty[0] = (1 - t1 - t2) / 2;
            duty[1] = duty[0] + t2 / 2;
            duty[2] = duty[1] + t1 / 2;
            break;
        case 5:
            t1 = -z;
            t2 = y;

            duty[1] = (1 - t1 - t2) / 2;
            duty[0] = duty[1] + t2 / 2;
            duty[2] = duty[0] + t1 / 2;
            break;
        case 6:
            t1 = z;
            t2 = -x;

            duty[1] = (1 - t1 - t2) / 2;
            duty[2] = duty[1] + t2 / 2;
            duty[0] = duty[2] + t1 / 2;
            break;
    }

    uab0temp[0] = uab0temp[0] - uab0[0];
    uab0temp[1] = uab0temp[1] - uab0[1];
    uab0temp[2] = 0;

    ualpha = uab0temp[0];//ud * cos(theta) - uq * sin(theta);
    ubeta = uab0temp[1];//ud * sin(theta) + uq * cos(theta);

    // 扇区判断
    if (ubeta >= 0 && (ubeta - SQRT3 * ualpha) < 0)
        sector = 1;
    else if ((ubeta - SQRT3 * ualpha) >= 0 && (ubeta + SQRT3 * ualpha) > 0)
        sector = 2;
    else if ((ubeta + SQRT3 * ualpha) <= 0 && ubeta > 0)
        sector = 3;
    else if (ubeta <= 0 && (ubeta - SQRT3 * ualpha) > 0)
        sector = 4;
    else if ((ubeta - SQRT3 * ualpha) <= 0 && (ubeta + SQRT3 * ualpha) < 0)
        sector = 5;
    else if ((ubeta + SQRT3 * ualpha) >= 0 && ubeta < 0)
        sector = 6;
    else
        sector = 1;
    //扇区判断(132645)
//    float ua, ub, uc;
//    int a, b, c, n;
//
//    ua = ualpha * _2TO3;
//    ub = -ualpha * _1TO3 + ubeta * SQRT3TO3;
//    uc = -ualpha * _1TO3 - ubeta * SQRT3TO3;
//
//    if(ua >= 0)
//        a = 1;
//    else
//        a = 0;
//
//    if(ub >= 0)
//        b = 1;
//    else
//        b = 0;
//
//    if(uc >= 0)
//        c = 1;
//    else
//        c = 0;
//    n = a + 2 * b + 4 * c;
    //end

    x = SQRT3TO2 / udc[1] * 2 * ubeta;
    y = SQRT3TO2 / udc[1] * (SQRT3 * ualpha - ubeta);
    z = SQRT3TO2 / udc[1] * (SQRT3 * ualpha + ubeta);

    switch (sector)
    {
        case 1:
            t1 = y;
            t2 = x;

            duty[5] = (1 - t1 - t2) / 2;
            duty[4] = duty[2] + t2 / 2;
            duty[3] = duty[1] + t1 / 2;
            break;
        case 2:
            t1 = -y;
            t2 = z;

            duty[5] = (1 - t1 - t2) / 2;
            duty[3] = duty[2] + t2 / 2;
            duty[4] = duty[0] + t1 / 2;
            break;
        case 3:
            t1 = x;
            t2 = -z;

            duty[3] = (1 - t1 - t2) / 2;
            duty[5] = duty[0] + t2 / 2;
            duty[4] = duty[2] + t1 / 2;
            break;
        case 4:
            t1 = -x;
            t2 = -y;

            duty[3] = (1 - t1 - t2) / 2;
            duty[4] = duty[0] + t2 / 2;
            duty[5] = duty[1] + t1 / 2;
            break;
        case 5:
            t1 = -z;
            t2 = y;

            duty[4] = (1 - t1 - t2) / 2;
            duty[3] = duty[1] + t2 / 2;
            duty[5] = duty[0] + t1 / 2;
            break;
        case 6:
            t1 = z;
            t2 = -x;

            duty[4] = (1 - t1 - t2) / 2;
            duty[5] = duty[1] + t2 / 2;
            duty[3] = duty[2] + t1 / 2;
            break;
    }

*/

/*
    float ualpha, ubeta;
    float Vas, Vbs, Vcs;
    float Tas, Tbs, Tcs, Tmax, Tmin, Teff, T0, Toffset;

    ualpha = ud * cos(theta) - uq * sin(theta);
    ubeta = ud * sin(theta) + uq * cos(theta);
    Vas = ualpha * _2TO3;
    Vbs = (-ualpha + ubeta * SQRT3) / 3;
    Vcs = (-ualpha - ubeta * SQRT3) / 3;

    Tas = Vas / UDC;
    Tbs = Vbs / UDC;
    Tcs = Vcs / UDC;

//    Tas = 2 * Vas / UDC;
//    Tbs = 2 * Vbs / UDC;
//    Tcs = 2 * Vcs / UDC;

    Tmax = Tas;
    Tmin = Tas;
    if(Tbs > Tmax)
    {
        Tmax = Tbs;
    }
    if(Tbs < Tmin)
    {
        Tmin = Tbs;
    }
    if(Tcs > Tmax)
    {
        Tmax = Tcs;
    }
    if(Tcs < Tmin)
    {
        Tmin = Tcs;
    }

    Teff = Tmax - Tmin;
    T0 = 1 - Teff;
    Toffset = 0.5 * T0 - Tmin;

    if(T0 < 0)
    {
        Tas = Tas / Teff;
        Tbs = Tbs / Teff;
        Tcs = Tcs / Teff;
        Toffset = -Tmin / Teff;
    }

    duty[0] = Tas + Toffset;
    duty[1] = Tbs + Toffset;
    duty[2] = Tcs + Toffset;
*/
}
#endif

#if PHASE == 5
void ClarkTrans(float abcde[], float abxy0[]);
void IClarkTrans(float abxy0[], float abcde[]);
void ParkTrans(float abxy0[], float theta, float dqxy0[]);
void IParkTrans(float dqxy0[], float theta, float abxy0[]);
void CtrlAlgo(float ud, float uq, float ux, float uy, float udc[], float theta, float duty[])
{
    /************************************************************
    Description:五相FOC_SVPWM
    Input:ud, uq, theta
    Output:duty[]
    ************************************************************/
    float udqxy0[5] = {ud, uq, ux, uy, 0}, uabxy0[5] = {0}, uabcde[5] = {0};
    float udcavg = 0.5 * (udc[0] + udc[1]);
    float umax = 0, umin = 0;

    IParkTrans(udqxy0, theta, uabxy0);
    IClarkTrans(uabxy0, uabcde);

    int i = 0;
    for(i = 0; i < 5; i++)
    {
        if(uabcde[i] < umin)
        {
            umin = uabcde[i];
        }
        if(uabcde[i] > umax)
        {
            umax = uabcde[i];
        }
    }

    duty[0] = (uabcde[0] - 0.5 * (umax + umin)) / udcavg + 0.5;
    duty[1] = (uabcde[1] - 0.5 * (umax + umin)) / udcavg + 0.5;
    duty[2] = (uabcde[2] - 0.5 * (umax + umin)) / udcavg + 0.5;
    duty[3] = (uabcde[3] - 0.5 * (umax + umin)) / udcavg + 0.5;
    duty[4] = (uabcde[4] - 0.5 * (umax + umin)) / udcavg + 0.5;

    for(i = 0; i < 5; i++)
    {
        if(duty[i] > 1)
            duty[i] = 1;
        else if(duty[i] < 0)
            duty[i] = 0;
    }

}
#endif

#if PHASE == 6
void ClarkTrans(float abcdef[], float abxymn[]);
void IClarkTrans(float abxymn[], float abcdef[]);
void ParkTrans(float abxymn[], float theta, float dqxymn[]);
void IParkTrans(float dqxymn[], float theta, float abxymn[]);
void CtrlAlgo(float iq, float iabcdef[], float theta, float duty[])
{
    /************************************************************
    Description:六相FOC_SVPWM
    Input:ud, uq, theta
    Output:duty[]
    ************************************************************/
///*Bang-Bang
    float idqxymnref[6] = {GIVEN_ID, iq, GIVEN_IX, GIVEN_IY, 0, 0}, iabcdefref[6] = {0}, iabxymnref[6] = {0};

    IParkTrans(idqxymnref, theta, iabxymnref);
    IClarkTrans(iabxymnref, iabcdefref);

    int i = 0;
    for(i = 0; i < 6; i++)
    {
        if(iabcdefref[i] > iabcdef[i])
            duty[i] = 1;
        else
            duty[i] = 0;
    }
//*/
}
#endif
