//###########################################################################
// AUTHOR:          Ezreal
// DESCRIPTION:     Motor Controlled by TMS320F28377S
// DATE:            2018/06/27
// VERSION:         1.5.0-alpha
// last modified:   Doby 2019.6.18
//###########################################################################
// Copyright © 2017-2018 Lab.106 All Rights Reserved.
// School of Electrical and Information Engineering
// JiangSu University
// CHINA
//###########################################################################

/*****************************************************************************************************
Instructions for use:
1. 修改 CtrlParams.h 中各参数
2. 修改CtrlAlgo.c 中控制算法
3. 注释无需使用的PID控制器
*****************************************************************************************************/

#include "F28x_Project.h"
#include "string.h"
#include "CtrlParams.h"
#include "UserFuncs.h"

//***************************************************全局变量***************************************************
//电机状态变量
char MotorRunFlag = '0';                                //电机启停标志位
float Velocity = 0;                                     //速度(rpm, mps)
float ElecTheta = 0;                             //电角度(rad)
float IOriSCS[PHASE] = {0};                             //原始静止坐标系电流(A)
float I2pSCS[PHASE] = {0};                              //两相静止坐标系电流(A)
float I2pRCS[PHASE] = {0};                              //两相旋转坐标系电流(A)
int     out1;
int     out2;
//  系统状态变量
float DutyCycle[PWM_NUM] = {0};                         //桥臂占空比
float Udc[2] = {200,0};
//  PID控制器变量
PID VelocityPID = {0, 0, 0};
PID IdPID = {0, 0, 0};
PID IqPID = {0, 0, 0};                      //结构体参数{iacc, lasterr, pidout}
PID IxPID = {0, 0, 0};
PID IyPID = {0, 0, 0};
int VelCtrlCNT = 1;                                     //速度控制器分频计数


//************************************************************************************************************
int main(void)
{
//  Initialize System Control:PLL, WatchDog, enable Peripheral Clocks
    InitSysCtrl();
//  Initialize GPIO:
    InitGpio();
//  Clear all interrupts and initialize PIE vector table:Disable CPU interrupts
    DINT;
//  Initialize the PIE control registers to their default state.
    InitPieCtrl();
//  Disable CPU interrupts and clear all CPU interrupt flags:
    IER = 0x0000;
    IFR = 0x0000;

//  Initialize the PIE vector table with pointers to the shell Interrupt Service Routines (ISR).
    InitPieVectTable();
    EALLOW;
    PieVectTable.EPWM1_INT = &EPWM1_ISR;        //程序使用ePWM3中断
    PieVectTable.EQEP1_INT = &EQEP1_ISR;        //程序使用eQEP1中断
//    PieVectTable.SCIB_RX_INT = &SCIB_RX_ISR;    //程序使用Scib_RX中断
    EDIS;

//  初始化外围设备
    InitADCs();
    InitDACs();
    InitEPwms();
    InitEQep1();
//    InitScib();
    InitKey();
//    InitCPLD();

//  使能程序用到的中断
    IER |= (M_INT3 | M_INT5 | M_INT9);
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;          //程序使用ePWM1中断
    PieCtrlRegs.PIEIER5.bit.INTx1 = 1;          //程序使用eQEP1中断
//    PieCtrlRegs.PIEIER9.bit.INTx3 = 1;          //程序使用Scib_RX中断

//  Enable global Interrupts and higher priority real-time debug events:
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM


//  test code end
    while(1)
    {
//  电机启停控制
//        CtrlAlgo(0, 4, 0, 0 , Udc , 0 , DutyCycle,out1,out2);
        if(EPwm3Regs.ETSEL.bit.INTEN == 0 && (MotorRunFlag == '1' || GpioDataRegs.GPADAT.bit.GPIO25 == 1))
        {
            MotorRunFlag = '1';
            EPwm1Regs.ETSEL.bit.INTEN = 1;      //打开EPWM中断使能
        }                                       //电机启动状态不主动采样
        if(MotorRunFlag == '0' || GpioDataRegs.GPADAT.bit.GPIO24 == 1)
        {
            MotorRunFlag = '0';
            LineCurSamp(IOriSCS);
            ElecTheta = ElecThetaCal(ElecTheta);
            ClarkTrans(IOriSCS, I2pSCS);
            ParkTrans(I2pSCS, ElecTheta, I2pRCS);
            Velocity = VelocityCal(Velocity);
            //设置DACA输出
                SetDACaValue(0);
            //设置DACB输出
                SetDACbValue(0);
            //设置DACC输出
                SetDACcValue(0);
        }                                       //电机停止状态主动采样
    }
}

long int i=0;
interrupt void EPWM1_ISR(void)
{
    /************************************************************
    Description:EPWM1中断子程序
                                        整体控制程序，采样频率=控制频率=PWM频率
    ************************************************************/
    LineCurSamp(IOriSCS);                    //电流采样

    ElecTheta = ElecThetaCal(ElecTheta);     //电角度计算
    ClarkTrans(IOriSCS, I2pSCS);
    ParkTrans(I2pSCS, ElecTheta, I2pRCS);    //坐标变换

    Velocity = VelocityCal(Velocity);        //速度计算

    if(VelCtrlCNT++ == 50)
    {
        PIDCtrl(&VelocityPID, GIVEN_VEL - Velocity, VEL_KP, VEL_KI, VEL_UPLIM, VEL_DNLIM);
        VelCtrlCNT = 1;
    }                                                      //速度控制，50倍的电流控制周期，判断顺序待调整
    PIDCtrl(&IdPID, GIVEN_ID - I2pRCS[0], ID_KP, ID_KI, ID_UPLIM, ID_DNLIM);
    PIDCtrl(&IqPID, VelocityPID.pidout - I2pRCS[1], IQ_KP, IQ_KI, IQ_UPLIM, IQ_DNLIM);
    CtrlAlgo(IdPID.pidout, IqPID.pidout, 0,0,Udc, ElecTheta, DutyCycle,&out1,&out2);
////    CtrlAlgo(0, 4, 0, 0 , Udc , 0, DutyCycle,out1,out2);
//    for (i=0 ; i<12 ; i++)
//    {
//        DutyCycle[i]=0.05*i;
//    }
    SetDACaValue(out1*400);
    SetDACbValue(out2*400);
    SetCMP(DutyCycle);
//-----------------------------------------------
//  电机启停控制
    if(MotorRunFlag == '0' || GpioDataRegs.GPADAT.bit.GPIO24 == 1)
    {
        MotorRunFlag = '0';
        memset(&VelocityPID, 0, sizeof(VelocityPID));
        memset(&IdPID, 0, sizeof(IdPID));
        memset(&IqPID, 0, sizeof(IqPID));
        memset(&IxPID, 0, sizeof(IxPID));
        memset(&IyPID, 0, sizeof(IyPID));
        memset(DutyCycle, 0, sizeof(DutyCycle));
        SetCMP(DutyCycle);
        EPwm1Regs.ETSEL.bit.INTEN = 0;      //关闭EPWM中断使能
    }                                       //电机停转

    EPwm1Regs.ETCLR.bit.INT = 1;            //清除中断标志位
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3; //允许下次中断进入
}

interrupt void EQEP1_ISR(void)
{
    /************************************************************
    Description:EQEP1中断子程序
                Z信号产生时进入中断，用于清除编码器累计误差
    ************************************************************/
#if (SENSOR == 0)
    ElecTheta = 5.767;          //1.767145867;//1.7197188942232836736510610992797
//    ob1++;
#endif

#if (SENSOR == 2)

#endif

    EQep1Regs.QCLR.bit.IEL = 1;
    EQep1Regs.QCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP5;
}


// End of file
