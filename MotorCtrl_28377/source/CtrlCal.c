/*
 * CtrlCal.c
 *
 *  Created on: 2018年1月8日
 *      Author: Ezreal
 */

#include "F28x_Project.h"
#include "CtrlParams.h"

void ModeSelAD2S1210(int mode);
void GetPosVelAD2S1210(int *buf);

float ElecThetaCal(float theta)
{
    /************************************************************
    Description:电角度计算，0-2pi
    Input:theta
    Output:theta
    ************************************************************/
#if (SENSOR == 0 | SENSOR == 2)                               //编码器,磁栅尺采集信号
    static int tposition = 0;
    int deltaposition = 0, cnt = 0;

    cnt = EQep1Regs.QPOSCNT;

    if(EQep1Regs.QEPSTS.bit.QDF == 1)
    {
        if(cnt >= tposition)
            deltaposition = cnt - tposition;
        else
            deltaposition = cnt + CODER_NUM - tposition;
    }
    else
    {
        if(cnt <= tposition)
            deltaposition = cnt - tposition;
        else
            deltaposition = cnt - CODER_NUM - tposition;
    }

    theta += _2PI * deltaposition / CODER_NUM * POLE_PAIRS;         //角度=两次采样线数差/编码器总线数*2π*极对数

    tposition = cnt;
#endif

#if (SENSOR == 1)                               //旋转变压器采集信号
    int posbuff[3] = {0};
    unsigned int position = 0;

    ModeSelAD2S1210(0);
    GetPosVelAD2S1210(posbuff);

    position = posbuff[0]<<8 | (posbuff[1] & 0xFC);
    theta = position / 65535.0 * _2PI * POLE_PAIRS - 4.5823;
#endif

//    theta = fmod(theta, _2PI);
    while(theta >= _2PI)
    {
        theta -= _2PI;
    }
    while(theta < 0)
    {
        theta += _2PI;
    }

    return theta;
}

float VelocityCal(float velocity)
{
    /************************************************************
    Description:速度计算（低速模式，20r/min - 700r/min）
    Input:velocity
    Output:velocity
    ************************************************************/
#if (SENSOR == 0)                               //编码器采集信号
    float deltatime = 0;

    if(EQep1Regs.QEPSTS.bit.UPEVNT == 1)
    {
        if(EQep1Regs.QEPSTS.bit.COEF == 0)
        {
            deltatime = EQep1Regs.QCPRD * 64.0/200000000;                     //Δt=单位线数内计时*模块时基/200MHz
            if(EQep1Regs.QEPSTS.bit.QDF == 1)
            {
                velocity = 32.0 / CODER_NUM / deltatime * 60;                            //单位线数值/编码器总线数/Δt*60
            }                                                                           //正方向
            else
            {
                velocity = 32.0 / CODER_NUM / deltatime * 60 * -1;                            //单位线数值/编码器总线数/Δt*60
            }                                                                           //负方向

        }
        EQep1Regs.QEPSTS.bit.UPEVNT = 1;
    }

    if(EQep1Regs.QEPSTS.bit.COEF == 1)
    {
        velocity = 0;
    }
#endif

#if (SENSOR == 1)                               //旋转变压器采集信号
    int velbuff[3] = {0};

    ModeSelAD2S1210(1);
    GetPosVelAD2S1210(velbuff);

    velocity = (velbuff[0]<<8 | (velbuff[1] & 0xFC)) / 32767.0 * 125 * 60 * -1;
#endif

#if (SENSOR == 2)                               //磁栅尺采集信号
    float deltatime = 0;

    if(EQep1Regs.QEPSTS.bit.UPEVNT == 1)
    {
        if(EQep1Regs.QEPSTS.bit.COEF == 0)
        {
            deltatime = EQep1Regs.QCPRD * 64.0/200000000;                     //Δt=单位线数内计时*模块时基/200MHz
            if(EQep1Regs.QEPSTS.bit.QDF == 1)
            {
                velocity = 32.0 * CODER_LEN / deltatime / 1000;                            //单位线数值/编码器总线数/Δt*60
            }                                                                           //正方向
            else
            {
                velocity = 32.0 * CODER_LEN / deltatime / 1000 * -1;                            //单位线数值/编码器总线数/Δt*60
            }                                                                           //负方向

        }
        EQep1Regs.QEPSTS.bit.UPEVNT = 1;
    }

    if(EQep1Regs.QEPSTS.bit.COEF == 1)
    {
        velocity = 0;
    }
#endif

    return velocity;
//*/
    /*
    static int vposition = 0, vtime = 0;
    int deltaposition = 0, deltatime = 0, cnt, prd;

    cnt = EQep1Regs.QPOSCNT;
    prd = EQep1Regs.QCTMRLAT;

    if(EQep1Regs.QEPSTS.bit.QDF == 1)
    {
        if(cnt >= vposition)
            deltaposition = cnt - vposition;
        else
            deltaposition = cnt + CODER_NUM - vposition;
    }
    else
    {
        if(cnt <= vposition)
            deltaposition = cnt - vposition;
        else
            deltaposition = cnt - CODER_NUM - vposition;
    }

    velocity = deltaposition / ();

    vposition = cnt;
    vtime = prd;

    return velocity;
    */
}

float PIDCtrl(PID *p, float errin, float kp, float ki, float up, float down)
{
    /************************************************************
    Description:PID控制函数
    Input:PID struct pointer, errin, kp, ki, kd, up, down
    Output:pidout
    ************************************************************/
/*
    if(p->pidout == up)                                   //上一次正向饱和
    {
        if(errin < 0)
            p->iacc += errin;                             //正饱和只积分负偏差
    }
    if(p->pidout == down)                                 //上一次负向饱和
    {
        if(errin > 0)
            p->iacc += errin;                             //负饱和只积分正偏差
    }
    if(down < p->pidout && p->pidout < up)
    {
        p->iacc += errin;                                 //非饱和正常积分
    }

    p->pidout = kp * errin + ki * p->iacc;

    if(p->pidout > up)                                    //正向饱和
        p->pidout = up;
    if(p->pidout < down)                                  //负向饱和
        p->pidout = down;

    p->lasterr = errin;
    return p->pidout;
*/

    float b0 = 0, b1 = 0;

    b0 = kp + ki * TS;
    b1 = -1 * kp;

    p->pidout = b0 * errin + b1 * p->lasterr + p->lastout;
    if(p->pidout > up)
    {
        p->pidout = up;
    }
    if(p->pidout < down)
    {
        p->pidout = down;
    }

    p->lasterr = errin;
    p->lastout = p->pidout;

    return p->pidout;
}
