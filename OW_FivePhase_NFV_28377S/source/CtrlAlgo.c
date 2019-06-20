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

extern float ob1, ob2, ob3,ob4,ob5;


#if PHASE == 3 //三相电机CBPWM
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

#if (PHASE == 5 && PWM_NUM == 5)//五相电机CBPWM
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

#if (PHASE == 5 && PWM_NUM == 10)//开绕组五相电机SVPWM
void ClarkTrans(float abcde[], float abxy0[]);
void IClarkTrans(float abxy0[], float abcde[]);
void ParkTrans(float abxy0[], float theta, float dqxy0[]);
void IParkTrans(float dqxy0[], float theta, float abxy0[]);
void NTV_SVPWM(float abxy0[],float Udc,float duty[],int *out);
void NTV_SVPWM(float abxy0[],float Udc,float duty[],int *out)
{
    float x=abxy0[1];   //beta轴
    float y=abxy0[0];   //alpha轴
    int   n=Udc;        //母线电压
    float a=0.2764;
    float SIN1=0.587785252;
    float SIN2=0.951056516;
    float COS1=0.809016994;
    float COS2=0.309016994;
    float value1,value2,value3,value4;
    float T1,T2,T3,T4;
    float duty_a,duty_b,duty_c,duty_d,duty_e;
    value1 = y*SIN1 - x*COS1;
    value2 = y*SIN2 - x*COS2;
    value3 = y*SIN2 + x*COS2;
    value4 = y*SIN1 + x*COS1;

    if (y > 0.0)
    {
        if (x > 0.0)
        {
            if (y > 1.3764*x)
            {
              *out=1;
              T1  = __divf32((a*value1),(0.4*n*SIN1));
              T2  = __divf32(((1 - a)*x),(0.6472*n*SIN1));
              T3  = __divf32(((1 - a)*value1),(0.6472*n*SIN1));
              T4  = __divf32((a*x),(0.4*n*SIN1));
              duty_d = 0.5*(1.0 - T1 - T2 - T3 - T4);
              duty_a = 1.0 - duty_d;
              duty_c = duty_d + T4;
              duty_e = duty_c + T3;
              duty_b = duty_e + T2;
            }
            else  if (y < 0.3249*x)
            {
             *out=3;
             T1  = a*value3/(0.4*n*SIN1);
             T2  = (1 - a)*(-value2)/(0.6472*n*SIN1);
             T3  = (1 - a)*value3/(0.6472*n*SIN1);
             T4  = a*(-value2)/(0.4*n*SIN1);
             duty_e = 0.5*(1.0 - T1 - T2 - T3 - T4);
             duty_b = 1.0 - duty_e;
             duty_d = duty_e + T4;
             duty_a = duty_d + T3;
             duty_c = duty_a + T2;
             }
             else
             {
              *out=2;
              T1  = a*(-value1)/(0.4*n*SIN1);
              T2  = (1 - a)*value2/(0.6472*n*SIN1);
              T3  = (1 - a)*(-value1)/(0.6472*n*SIN1);
              T4  = a*value2/(0.4*n*SIN1);
              duty_d = 0.5*(1.0 - T1 - T2 - T3 - T4);
              duty_b = 1.0 - duty_d;
              duty_e = duty_d + T4;
              duty_c = duty_e + T3;
              duty_a = duty_c + T2;
             }
        }
        else //x <= 0.0
        {
            if (y > -1.3764*x)
            {
              *out=10;
              T1  = a*value4 /(0.4*n*SIN1);
              T2  = (1 - a)*(-x)/(0.6472*n*SIN1);
              T3  = (1 - a)*value4/(0.6472*n*SIN1);
              T4  = a*(-x)/(0.4*n*SIN1);
              duty_c = 0.5*(1.0 - T1 - T2 - T3 - T4);
              duty_a = 1.0 - duty_c;
              duty_d = duty_c + T4;
              duty_b = duty_d + T3;
              duty_e = duty_b + T2;
            }
            else if (y < -0.3249*x)
            {
             *out=8;
             T1  = a*value2/(0.4*n*SIN1);
             T2  = (1 - a)*(-value3)/(0.6472*n*SIN1);
             T3  = (1 - a)*value2/(0.6472*n*SIN1);
             T4  = a*(-value3)/(0.4*n*SIN1);
             duty_b = 0.5*(1.0 - T1 - T2 - T3 - T4);
             duty_e = 1.0 - duty_b;
             duty_c = duty_b + T4;
             duty_a = duty_c + T3;
             duty_d = duty_a + T2;
            }
            else
            {
            *out=9;
            T1  = a*(-value4)/(0.4*n*SIN1);
            T2  = (1 - a)*value3/(0.6472*n*SIN1);
            T3  = (1 - a)*(-value4)/(0.6472*n*SIN1);
            T4  = a*value3/(0.4*n*SIN1);
            duty_c = 0.5*(1.0 - T1 - T2 - T3 - T4);
            duty_e = 1.0 - duty_c;
            duty_b = duty_c + T4;
            duty_d = duty_b + T3;
            duty_a = duty_d + T2;
            }
        }
    }
    else   //y <= 0.0
    {
        if (x > 0.0)
        {
          if (y < -1.3764*x)
          {
              *out=5;
              T1  = a*x/(0.4*n*SIN1);
              T2  = (1 - a)*(-value4)/(0.6472*n*SIN1);
              T3  = (1 - a)*x/(0.6472*n*SIN1);
              T4  = a*(-value4)/(0.4*n*SIN1);
              duty_a = 0.5*(1.0 - T1 - T2 - T3 - T4);
              duty_c = 1.0 - duty_a;
              duty_e = duty_a + T4;
              duty_b = duty_e + T3;
              duty_d = duty_b + T2;
          }
          else  if (y > -0.3249*x)
          {
                 *out=3;
                 T1  = a*value3/(0.4*n*SIN1);
                 T2  = (1 - a)*(-value2)/(0.6472*n*SIN1);
                 T3  = (1 - a)*value3/(0.6472*n*SIN1);
                 T4  = a*(-value2)/(0.4*n*SIN1);
                 duty_e = 0.5*(1.0 - T1 - T2 - T3 - T4);
                 duty_b = 1.0 - duty_e;
                 duty_d = duty_e + T4;
                 duty_a = duty_d + T3;
                 duty_c = duty_a + T2;
          }
          else
          {
                 *out=4;
                 T1  = a*(-value3)/(0.4*n*SIN1);
                 T2  = (1 - a)*value4/(0.6472*n*SIN1);
                 T3  = (1 - a)*(-value3)/(0.6472*n*SIN1);
                 T4  = a*value4/(0.4*n*SIN1);
                 duty_e = 0.5*(1.0 - T1 - T2 - T3 - T4);
                 duty_c = 1.0 - duty_e;
                 duty_a = duty_e + T4;
                 duty_d = duty_a + T3;
                 duty_b = duty_d + T2;
          }
        }
        else //x <= 0.0
        {
            if (y < 1.3764*x)
            {
                *out=6;
                T1  = a*(-x)/(0.4*n*SIN1);
                T2  = (1 - a)*(-value1)/(0.6472*n*SIN1);
                T3  = (1 - a)*(-x)/(0.6472*n*SIN1);
                T4  = a*(-value1)/(0.4*n*SIN1);
                duty_a = 0.5*(1.0 - T1 - T2 - T3 - T4);
                duty_d = 1.0 - duty_a;
                duty_b = duty_a + T4;
                duty_e = duty_b + T3;
                duty_c = duty_e + T2;
            }
            else   if (y > 0.3249*x)
            {
                   *out=8;
                   T1  = a*value2/(0.4*n*SIN1);
                   T2  = (1 - a)*(-value3)/(0.6472*n*SIN1);
                   T3  = (1 - a)*value2/(0.6472*n*SIN1);
                   T4  = a*(-value3)/(0.4*n*SIN1);
                   duty_b = 0.5*(1.0 - T1 - T2 - T3 - T4);
                   duty_e = 1.0 - duty_b;
                   duty_c = duty_b + T4;
                   duty_a = duty_c + T3;
                   duty_d = duty_a + T2;
            }

                else
                {
                   *out=7;
                   T1  = a*(-value2)/(0.4*n*SIN1);
                   T2  = (1 - a)*value1/(0.6472*n*SIN1);
                   T3  = (1 - a)*(-value2)/(0.6472*n*SIN1);
                   T4  = a*value1/(0.4*n*SIN1);
                   duty_b = 0.5*(1.0 - T1 - T2 - T3 - T4);
                   duty_d = 1.0 - duty_b;
                   duty_a = duty_b + T4;
                   duty_c = duty_a + T3;
                   duty_e = duty_c + T2;
                }
        }
    }
    duty[0]=duty_a;
    duty[1]=duty_b;
    duty[2]=duty_c;
    duty[3]=duty_d;
    duty[4]=duty_e;
}
void CtrlAlgo(float ud, float uq, float ux, float uy, float udc[], float theta, float duty[],int *out1,int *out2)
{
    /************************************************************
    Description:五相FOC_SVPWM
    Input:ud, uq, theta
    Output:duty[]
    ************************************************************/
    float Uabxy0_OW[5]; //静止坐标系
    float Uabxy0_I1[5]={0,0,0,0,0}; //静止坐标系 第一个逆变器
    float Uabxy0_I2[5]={0,0,0,0,0}; //静止坐标系 第二个逆变器
    float Udqxy0[5]={ud, uq, ux, uy, 0};
    float duty_I1[5]={0,0,0,0,0};
    float duty_I2[5]={0,0,0,0,0};
    int i=0;

    IParkTrans(Udqxy0,theta,Uabxy0_OW);
//    IClarkTrans(Uabxy0_OW,UABCDE);
    for(i = 0; i < 5; i++)
        {
            Uabxy0_I1[i]= 0.5 * Uabxy0_OW[i];
            Uabxy0_I2[i]=-0.5 * Uabxy0_OW[i];
        }
    NTV_SVPWM(Uabxy0_I1,udc[0],duty_I1,out1);
    NTV_SVPWM(Uabxy0_I2,udc[0],duty_I2,out2);
    for(i = 0; i < 10; i++)
    {
        if( i < 5 )
        duty[i]=duty_I1[i];
        else
        duty[i]=duty_I2[i-5];
    }

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
