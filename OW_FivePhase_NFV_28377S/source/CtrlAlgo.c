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
//void CtrlAlgo(float ud, float uq, float ucap, float idq0[], float udc[], float theta, float duty[])            //DTC-OEW-UP
void CtrlAlgo(float ud, float uq, float udc, float theta, float duty[])   //SVPWM
//void CtrlAlgo(float iq, float iabc[], float theta, float duty[])            //Bang-Bang
{
    /************************************************************
    Description:三相FOC_SVPWM
    Input:ud, uq, theta
    Output:duty[]
    ************************************************************/
    ///*SVPWM
        float udq0[3] = {ud, uq, 0}, uab0[3] = {0}, uabc[3] = {0};
        float umax = 0, umin = 0;

        IParkTrans(udq0, theta, uab0);
        IClarkTrans(uab0, uabc);

        umax = MAX(uabc[0], MAX(uabc[1], uabc[2]));
        umin = MIN(uabc[0], MIN(uabc[1], uabc[2]));

        duty[0] = (uabc[0] - 0.5 * (umax + umin)) / udc + 0.5;
        duty[1] = (uabc[1] - 0.5 * (umax + umin)) / udc + 0.5;
        duty[2] = (uabc[2] - 0.5 * (umax + umin)) / udc + 0.5;

        int i = 0;
        for(i = 0; i < 3; i++)
        {
            if(duty[i] > 1)
                duty[i] = 1;
            else if(duty[i] < 0)
                duty[i] = 0;
        }
}
#endif

#if (PHASE == 5 & LOOP_FLAG !=3)//非MPC算法
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
    float udcavg = udc[0];
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

    duty[0] = __divf32((uabcde[0] - 0.5 * (umax + umin)), udcavg) + 0.5;
    duty[1] = __divf32((uabcde[1] - 0.5 * (umax + umin)), udcavg) + 0.5;
    duty[2] = __divf32((uabcde[2] - 0.5 * (umax + umin)), udcavg) + 0.5;
    duty[3] = __divf32((uabcde[3] - 0.5 * (umax + umin)), udcavg) + 0.5;
    duty[4] = __divf32((uabcde[4] - 0.5 * (umax + umin)), udcavg) + 0.5;

    for(i = 0; i < 5; i++)
    {
        if(duty[i] > 1)
            duty[i] = 1;
        else if(duty[i] < 0)
            duty[i] = 0;
    }

}
#endif

#if (PHASE == 5 & LOOP_FLAG ==3)//MPC算法
void ClarkTrans(float abcde[], float abxy0[]);
void IClarkTrans(float abxy0[], float abcde[]);
void ParkTrans(float abxy0[], float theta, float dqxy0[]);
void IParkTrans(float dqxy0[], float theta, float abxy0[]);
void CtrlAlgo(float iq1ref, float id1ref, float id1, float iq1, float OmegaE, float theta, float index_old,float index,float duty[])
{
    /************************************************************
    Description:五相MPC(单矢量)
    Input:id1, iq1, iq1ref,id1ref,theta，OmegaE,index_old
    Output:duty[]，index
    ************************************************************/

}
#endif

#if PHASE == 6
void ClarkTrans(float abcdef[], float abxymn[]);
void IClarkTrans(float abxymn[], float abcdef[]);
void ParkTrans(float abxymn[], float theta, float dqxymn[]);
void IParkTrans(float dqxymn[], float theta, float abxymn[]);
void CtrlAlgo(float ud, float uq, float udc, float theta, float duty[])   //SVPWM
{
    /************************************************************
    Description:六相FOC_SVPWM
    Input:ud, uq, theta
    Output:duty[]
    ************************************************************/
    float udqxyo1o2[6] = {ud, uq, 0, 0, 0, 0}, uabxyo1o2[6] = {0}, uabcuvw[6] = {0};
    float umax1 = 0, umin1 = 0, umax2=0, umin2=0;

    IParkTrans(udqxyo1o2, theta, uabxyo1o2);
    IClarkTrans(uabxyo1o2, uabcuvw);

    umax1 = MAX(uabcuvw[0], MAX(uabcuvw[1], uabcuvw[2]));
    umin1 = MIN(uabcuvw[0], MIN(uabcuvw[1], uabcuvw[2]));

    umax2 = MAX(uabcuvw[3], MAX(uabcuvw[4], uabcuvw[5]));
    umin2 = MIN(uabcuvw[3], MIN(uabcuvw[4], uabcuvw[5]));

    duty[0] = (uabcuvw[0] - 0.5 * (umax1 + umin1)) / udc + 0.5;
    duty[1] = (uabcuvw[1] - 0.5 * (umax1 + umin1)) / udc + 0.5;
    duty[2] = (uabcuvw[2] - 0.5 * (umax1 + umin1)) / udc + 0.5;
    duty[3] = (uabcuvw[3] - 0.5 * (umax2 + umin2)) / udc + 0.5;
    duty[4] = (uabcuvw[4] - 0.5 * (umax2 + umin2)) / udc + 0.5;
    duty[5] = (uabcuvw[5] - 0.5 * (umax2 + umin2)) / udc + 0.5;

    int i = 0;
    for(i = 0; i < 6; i++)
    {
        if(duty[i] > 1)
            duty[i] = 1;
        else if(duty[i] < 0)
            duty[i] = 0;
    }

}
#endif
