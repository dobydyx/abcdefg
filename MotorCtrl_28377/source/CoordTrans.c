/*
 * CoordTrans.c
 *
 *  Created on: 2018年1月8日
 *      Author: Ezreal
 */

/************************************************************
Description:坐标变换函数
            CLARK变换                                         ClarkTrans
            CLARK逆变换                                     IClarkTrans
            PARK变换                                           ParkTrans
            PARK逆变换                                       IParkTrans
WARNING:    采用等幅值变换
************************************************************/

#include "F28x_Project.h"
#include "CtrlParams.h"

#if PHASE==3
/************************************************************
三相坐标变换
************************************************************/
void ClarkTrans(float abc[], float ab0[])
{
    /************************************************************
    Description:CLARK变换,3s/2s变换
                ┌                   ┐
                │2/3    -1/3    -1/3│
                │ 0     √3/3   -√3/3│
                │ 1/3    1/3     1/3│
                └                   ┘
    Input:abc[]
    Output:ab0[]
    ************************************************************/
    ab0[0] = _2TO3 * abc[0] - _1TO3 * abc[1] - _1TO3 * abc[2];
    ab0[1] = SQRT3TO3 * (abc[1] - abc[2]);
    ab0[2] = _1TO3 * (abc[0] + abc[1] + abc[2]);
}

void IClarkTrans(float ab0[], float abc[])
{
    /************************************************************
    Description:CLARK逆变换,2s/3s变换
                ┌                  ┐
                │  1       0      1│
                │-1/2    √3/2     1│
                │-1/2   -√3/2     1│
                └                  ┘
    Input:ab0[]
    Output:abc[]
    ************************************************************/
    abc[0] = ab0[0] + ab0[2];
    abc[1] = -0.5 * ab0[0] + SQRT3TO2 * ab0[1] + ab0[2];
    abc[2] = -0.5 * ab0[0] - SQRT3TO2 * ab0[1] + ab0[2];
}

void ParkTrans(float ab0[], float theta, float dq0[])
{
    /************************************************************
    Description:PARK变换,2s/2r变换
                ┌                        ┐
                │ cos(theta)   sin(theta)│
                │-sin(theta)   cos(theta)│
                └                        ┘
    Input:ab0[], theta
    Output:dq0[]
    ************************************************************/
    dq0[0] = cos(theta) * ab0[0] + sin(theta) * ab0[1];
    dq0[1] = -sin(theta) * ab0[0] + cos(theta) * ab0[1];
    dq0[2] = ab0[2];
}

void IParkTrans(float dq0[], float theta, float ab0[])
{
    /************************************************************
    Description:PARK逆变换,2r/2s变换
                ┌                        ┐
                │cos(theta)   -sin(theta)│
                │sin(theta)    cos(theta)│
                └                        ┘
    Input:dq0[], theta
    Output:ab0[]
    ************************************************************/
    ab0[0] = cos(theta) * dq0[0] - sin(theta) * dq0[1];
    ab0[1] = sin(theta) * dq0[0] + cos(theta) * dq0[1];
    ab0[2] = dq0[2];
}
#endif

#if PHASE==5
/************************************************************
五相坐标变换
************************************************************/
void ClarkTrans(float abcde[], float abxy0[])
{
    /************************************************************
    Description:CLARK变换,5s/2s变换
                ┌                                                   ┐
                │  1     cos(1a)    cos(2a)     cos(2a)     cos(1a) │
                │  0     sin(1a)    sin(2a)    -sin(2a)    -sin(1a) │
            2/5*│  1     cos(2a)    cos(1a)     cos(1a)     cos(2a) │
                │  0    -sin(2a)    sin(1a)    -sin(1a)     sin(2a) │
                │ 1/2      1/2        1/2         1/2         1/2   │
                └                                                   ┘
    PS:a = 2pi/5
    Input:abcde[]
    Output:abxy0[]
    ************************************************************/
    abxy0[0] = 0.4 * (abcde[0] + COS1A * abcde[1] + COS2A * abcde[2] + COS2A * abcde[3] + COS1A * abcde[4]);
    abxy0[1] = 0.4 * (SIN1A * abcde[1] + SIN2A * abcde[2] - SIN2A * abcde[3] - SIN1A * abcde[4]);
    abxy0[2] = 0.4 * (abcde[0] + COS2A * abcde[1] + COS1A * abcde[2] + COS1A * abcde[3] + COS2A * abcde[4]);
    abxy0[3] = 0.4 * (-SIN2A * abcde[1] + SIN1A * abcde[2] - SIN1A * abcde[3] + SIN2A * abcde[4]);
    abxy0[4] = 0.4 * (abcde[0]+ abcde[1]+ abcde[2]+ abcde[3]+ abcde[4]);
}

void IClarkTrans(float abxy0[], float abcde[])
{
    /************************************************************
    Description:CLARK逆变换,2s/5s变换
                ┌                                                        ┐
                │    1            0            1             0         1 │
                │ cos(1a)      sin(1a)      cos(2a)      -sin(2a)      1 │
                │ cos(2a)      sin(2a)      cos(1a)       sin(1a)      1 │
                │ cos(2a)     -sin(2a)      cos(1a)      -sin(1a)      1 │
                │ cos(1a)     -sin(1a)      cos(2a)       sin(2a)      1 │
                └                                                        ┘
    Input:abxy0[]
    Output:abced[]
    ************************************************************/
    abcde[0] = abxy0[0] + abxy0[2] + abxy0[4];
    abcde[1] = COS1A * abxy0[0] + SIN1A * abxy0[1] + COS2A * abxy0[2] - SIN2A * abxy0[3] + abxy0[4];
    abcde[2] = COS2A * abxy0[0] + SIN2A * abxy0[1] + COS1A * abxy0[2] + SIN1A * abxy0[3] + abxy0[4];
    abcde[3] = COS2A * abxy0[0] - SIN2A * abxy0[1] + COS1A * abxy0[2] - SIN1A * abxy0[3] + abxy0[4];
    abcde[4] = COS1A * abxy0[0] - SIN1A * abxy0[1] + COS2A * abxy0[2] + SIN2A * abxy0[3] + abxy0[4];
}

void ParkTrans(float abxy0[], float theta, float dqxy0[])
{
    /************************************************************
    Description:PARK变换,2s/2r变换
                ┌                                 ┐
                │ cos(theta)   sin(theta)  0  0  0│
                │-sin(theta)   cos(theta)  0  0  0│
                │     0            0       1  0  0│
                │     0            0       0  1  0│
                │     0            0       0  0  1│
                └                                 ┘
    Input:abxy0[], theta
    Output:dqxy0[]
    ************************************************************/
    dqxy0[0] = cos(theta) * abxy0[0] + sin(theta) * abxy0[1];
    dqxy0[1] = -sin(theta) * abxy0[0] + cos(theta) * abxy0[1];
    dqxy0[2] = abxy0[2];
    dqxy0[3] = abxy0[3];
    dqxy0[4] = abxy0[4];
}

void IParkTrans(float dqxy0[], float theta, float abxy0[])
{
    /************************************************************
    Description:PARK逆变换,2r/2s变换
                ┌                                 ┐
                │ cos(theta)  -sin(theta)  0  0  0│
                │ sin(theta)   cos(theta)  0  0  0│
                │     0            0       1  0  0│
                │     0            0       0  1  0│
                │     0            0       0  0  1│
                └                                 ┘
    Input:dqxy0[], theta
    Output:abxy0[]
    ************************************************************/
    abxy0[0] = cos(theta) * dqxy0[0] - sin(theta) * dqxy0[1];
    abxy0[1] = sin(theta) * dqxy0[0] + cos(theta) * dqxy0[1];
    abxy0[2] = dqxy0[2];
    abxy0[3] = dqxy0[3];
    abxy0[4] = dqxy0[4];
}
#endif

#if PHASE==6
/************************************************************
六相坐标变换
************************************************************/
void ClarkTrans(float abcdef[], float abxymn[])
{
    /************************************************************
    Description:CLARK变换,6s/2s变换
                ┌                                         ┐
                │1   cos(b)  -sin(b)  -cos(b)  -sin(b)   0│
                │0   sin(b)   cos(b)   sin(b)  -cos(b)  -1│
            1/3*│1  -cos(b)  -sin(b)   cos(b)  -sin(b)   0│
                │0   sin(b)  -cos(b)   sin(b)   cos(b)  -1│
                │1     0        1        0        1      0│
                │0     1        0        1        0      1│
                └                                         ┘
    Input:abcuvw[]
    Output:abxymn[]
    ************************************************************/
    abxymn[0] = _1TO3 * (abcdef[0] + COS1B * abcdef[1] - 0.5 * abcdef[2] - COS1B * abcdef[3] - 0.5 * abcdef[4]);
    abxymn[1] = _1TO3 * (0.5 * abcdef[1] + COS1B * abcdef[2] + 0.5 * abcdef[3] - COS1B * abcdef[4] - abcdef[5]);
    abxymn[2] = _1TO3 * (abcdef[0] - COS1B * abcdef[1] - 0.5 * abcdef[2] + COS1B * abcdef[3] - 0.5 * abcdef[4]);
    abxymn[3] = _1TO3 * (0.5 * abcdef[1] - COS1B * abcdef[2] + 0.5 * abcdef[3] + COS1B * abcdef[4] - abcdef[5]);
    abxymn[4] = _1TO3 * (abcdef[0] + abcdef[2] + abcdef[4]);
    abxymn[5] = _1TO3 * (abcdef[1] + abcdef[3] + abcdef[5]);
}

void IClarkTrans(float abxymn[], float abcdef[])
{
    /************************************************************
    Description:CLARK逆变换,2s/6s变换
                ┌                                                    ┐
                │   1          0          1          0        1     0│
                │ cos(b)     sin(b)    -cos(b)     sin(b)     0     1│
                │-sin(b)     cos(b)    -sin(b)    -cos(b)     1     0│
                │-cos(b)     sin(b)     cos(b)     sin(b)     0     1│
                │-sin(b)    -cos(b)    -sin(b)     cos(b)     1     0│
                │   0         -1          0         -1        0     1│
                └                                                    ┘
    Input:abxymn[]
    Output:abcdef[]
    ************************************************************/
    abcdef[0] = abxymn[0] + abxymn[2] + abxymn[4];
    abcdef[1] = COS1B * abxymn[0] + 0.5 * abxymn[1] - COS1B * abxymn[2] + 0.5 * abxymn[3] + abxymn[5];
    abcdef[2] = -0.5 * abxymn[0] + COS1B * abxymn[1] - 0.5 * abxymn[2] - COS1B * abxymn[3] + abxymn[4];
    abcdef[3] = -COS1B * abxymn[0] + 0.5 * abxymn[1] + COS1B * abxymn[2] + 0.5 * abxymn[3] + abxymn[5];
    abcdef[4] = -0.5 * abxymn[0] - COS1B * abxymn[1] - 0.5 * abxymn[2] + COS1B * abxymn[3] + abxymn[4];
    abcdef[5] = -abxymn[1] - abxymn[3] + abxymn[5];
}

void ParkTrans(float abxymn[], float theta, float dqxymn[])
{
    /************************************************************
    Description:PARK变换,2s/2r变换
                ┌                                    ┐
                │ cos(theta)   sin(theta)  0  0  0  0│
                │-sin(theta)   cos(theta)  0  0  0  0│
                │     0            0       1  0  0  0│
                │     0            0       0  1  0  0│
                │     0            0       0  0  1  0│
                │     0            0       0  0  0  1│
                └                                    ┘
    Input:abxymn[], theta
    Output:dqxymn[]
    ************************************************************/
    dqxymn[0] = cos(theta) * abxymn[0] + sin(theta) * abxymn[1];
    dqxymn[1] = -sin(theta) * abxymn[0] + cos(theta) * abxymn[1];
    dqxymn[2] = abxymn[2];
    dqxymn[3] = abxymn[3];
    dqxymn[4] = abxymn[4];
    dqxymn[5] = abxymn[5];
}

void IParkTrans(float dqxymn[], float theta, float abxymn[])
{
    /************************************************************
    Description:PARK逆变换,2r/2s变换
                ┌                                    ┐
                │cos(theta)   -sin(theta)  0  0  0  0│
                │sin(theta)    cos(theta)  0  0  0  0│
                │     0            0       1  0  0  0│
                │     0            0       0  1  0  0│
                │     0            0       0  0  1  0│
                │     0            0       0  0  0  1│
                └                                    ┘
    Input:dqxymn[], theta
    Output:abxymn[]
    ************************************************************/
    abxymn[0] = cos(theta) * dqxymn[0] - sin(theta) * dqxymn[1];
    abxymn[1] = sin(theta) * dqxymn[0] + cos(theta) * dqxymn[1];
    abxymn[2] = dqxymn[2];
    abxymn[3] = dqxymn[3];
    abxymn[4] = dqxymn[4];
    abxymn[5] = dqxymn[5];
}
#endif
