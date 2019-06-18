/*
 * SenlessCal.c
 *
 *  Created on: 2018年7月3日
 *      Author: Ezreal
 */

#include "F28x_Project.h"
#include "CtrlParams.h"

void SetDACaValue(int val);
void SetDACbValue(int val);

void ClarkTrans(float abc[], float ab0[]);

#define OMEGA_1 8.0
#define OMEGA_2 8.0
#define OMEGA_3 1000.0
#define EMF_K 1.0

#define PARAM_KP MOTOR_RS - OMEGA_3 * MOTOR_LS
#define PARAM_KI -1 * OMEGA_2 * OMEGA_3 * MOTOR_LS
#define PARAM_KA (1 - EMF_K) * OMEGA_3 * MOTOR_LS - MOTOR_RS
#define PARAM_KB (OMEGA_2 - EMF_K * OMEGA_1) * OMEGA_3 * MOTOR_LS
#define TRANS_FUN 1 / (MOTOR_LS + TS * MOTOR_RS)
#define PLL_KP 20
#define PLL_KI 2

float e2pscs[PHASE] = {0,0};
float pllout = 0;

void EmfEstCal(float udc[], float duty[], float i2pscs[])
{
    /************************************************************
    Description:反电势估计
    Input:udc[], duty[], i2pscs[]
    Output:none
    ************************************************************/
    float uoriscs[PHASE] = {0}, u2pscs[PHASE] = {0};
    float udcavg = 0.5 * (udc[0] + udc[1]);
    float mid = 0, end[3] = {0};
    static float acc[2][5] = {{0,0,0,0,0}, {0,0,0,0,0}};
    int i = 0;

    float theta = 0;

    for(i = 0; i < PHASE; i++)
    {
        uoriscs[i] = udcavg * duty[i];
    }
    ClarkTrans(uoriscs, u2pscs);

    theta = atan2(-1*e2pscs[0], e2pscs[1]);
    theta = fmod(theta, _2PI);
    while(theta < 0)
    {
        theta += _2PI;
    }

//    SetDACaValue(e2pscs[0] * -3 + 1500);
    SetDACbValue(theta * 400);

    for(i = 0; i  < 2; i++)
    {
        acc[i][0] = (u2pscs[i] - e2pscs[i]) * TRANS_FUN * TS + TRANS_FUN * acc[i][0] * MOTOR_LS;
        mid = i2pscs[i] - acc[i][0];
        acc[i][1] = acc[i][1] + mid * PARAM_KI * TS;
        end[0] = mid * PARAM_KP + acc[i][1];

        acc[i][2] = acc[i][2] + i2pscs[i] * PARAM_KB * TS;
        end[1] = i2pscs[i] * PARAM_KA + acc[i][2];

        acc[i][3] = u2pscs[i] * TRANS_FUN * TS + TRANS_FUN * acc[i][3] * MOTOR_LS;
        acc[i][4] = acc[i][4] + acc[i][3] * PARAM_KB * TS;
        end[2] = acc[i][3] * PARAM_KA + acc[i][4];

        e2pscs[i] = end[0] + end[1] - end[2];
    }
}

float ElecThetaEstCal(float theta)
{
    /************************************************************
    Description:电角度估计，0-2pi
    Input:theta
    Output:theta
    ************************************************************/
    float error = 0;
    static float pllacc = 0;

    error = - e2pscs[0] * cos(theta) - e2pscs[1] * sin(theta);
    pllacc = pllacc + error * PLL_KI;
    pllout = error * PLL_KP + pllacc;

    theta = theta + pllout * TS;

    theta = fmod(theta, _2PI);
    while(theta < 0)
    {
        theta += _2PI;
    }

    return theta;
}

float VelocityEstCal(float velocity)
{
    /************************************************************
    Description:速度估计
    Input:velocity
    Output:velocity
    ************************************************************/
    static float omega = 0;

    omega = 0.995025 * omega + 0.004975 * pllout;
    velocity = omega * 9.55 / 5;

    return velocity;
}
