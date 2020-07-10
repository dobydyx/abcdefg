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
void MPCC_V();
void CBPWM();
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
float Udc[2] = {75,0};
//  PID控制器变量
PID VelocityPID = {0, 0, 0};
PID IdPID = {0, 0, 0};
PID IqPID = {0, 0, 0};                      //结构体参数{iacc, lasterr, pidout}
PID IxPID = {0, 0, 0};
PID IyPID = {0, 0, 0};
int VelCtrlCNT = 1;                                     //速度控制器分频计数
float INV1_duty[10][5]={{1,0.618,0,0,0.618},//1
                       {1,1,0.382,0,0.382},//2
                       {0.618,1,0.618,0,0},//3
                       {0.382,1,1,0.382,0},//4
                       {0,0.618,1,0.618,0},//5
                       {0,0.382,1,1,0.382},//6
                       {0,0,0.618,1,0.618},//7
                       {0.382,0,0.382,1,1},//8
                       {0.618,0,0,0.618,1},//9
                       {1,0.382,0,0.382,1}};//10
float INV2_duty[10][5]={{0,0.382,1,1,0.382},//1
                        {0,0,0.618,1,0.618},//2
                        {0.382,0,0.382,1,1},//3
                        {0.618,0,0,0.618,1},//4
                        {1,0.382,0,0.382,1},//5
                        {1,0.618,0,0,0.618},//6
                        {1,1,0.382,0,0.382},//7
                        {0.618,1,0.618,0,0},//8
                        {0.382,1,1,0.382,0},//9
                        {0,0.618,1,0.618,0}};//10
float INV_VV_Real_Unit[10]={0.5527696,0.4472,0.1708152,-0.1708152,-0.4472,-0.5527696,-0.4472,-0.1708152,0.1708152,0.4472};
float INV_VV_Imag_Unit[10]={0,0.3248954,0.5256918,0.5256918,0.3248954,0,-0.3248954,-0.5256918,-0.5256918,-0.3248954};
//************************************************************************************************************
float INV_VV_Real[10]={0};
float INV_VV_Imag[10]={0};
float Ualbek[5]={0};
float Ualbek1[5]={0};
float Udqk[5]={0};
float Udqk1[5]={0};
int index_V1=0;
float OmegaeE=0;
float EMFdk;
float EMFqk;
int ii=0;
float Idk1=0;
float Iqk1=0;
float Idk2=0;
float Iqk2=0;
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

    for(ii=0;ii<10;ii++)
    {
        INV_VV_Real[ii]=Udc[0]*INV_VV_Real_Unit[ii]*2;
        INV_VV_Imag[ii]=Udc[0]*INV_VV_Imag_Unit[ii]*2;
    }
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
    OmegaeE=__divf32(Velocity,9.5493)*POLE_PAIRS;
    if(VelCtrlCNT++ == 50)
    {
        PIDCtrl(&VelocityPID, GIVEN_VEL - Velocity, VEL_KP, VEL_KI, VEL_UPLIM, VEL_DNLIM);
        VelCtrlCNT = 1;
    }                                                      //速度控制，50倍的电流控制周期，判断顺序待调整
//    CBPWM();

    MPCC_V();
//    for (i=0 ; i<10 ; i++)
//    {
//        DutyCycle[i]=0.05*i+0.25;
//    }
////    DutyCycle[4]=1;
////    DutyCycle[9]=0;
//    SetCMP(DutyCycle);

//          SetDACaValue(out1*200);
//          SetDACbValue(out2*200);
//      SetDACaValue(I2pRCS[4]*400+2047);
      SetDACbValue(Velocity*4.55);
//        SetDACaValue(I2pSCS[0]*200+2047);
//        SetDACbValue(I2pSCS[1]*200+2047);
//    SetDACaValue(I2pSCS[2]*200+2047);
//    SetDACbValue(I2pSCS[3]*200+2047);
//        SetDACaValue(I2pRCS[0]*200+2047);
//        SetDACbValue(I2pRCS[1]*200+2047);
//            SetDACaValue(I2pRCS[2]*200+2047);
//            SetDACbValue(I2pRCS[3]*200+2047);
//    SetDACaValue(Velocity * 10);
//    SetDACbValue(200 * 10);
//        SetDACaValue(ElecTheta * 500);
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
    ElecTheta = 0;          //1.767145867;//1.7197188942232836736510610992797
//    ob1++;
#endif

#if (SENSOR == 2)

#endif

    EQep1Regs.QCLR.bit.IEL = 1;
    EQep1Regs.QCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP5;
}
#if (AlgorithMode==0)
void CBPWM()
{
    PIDCtrl(&IdPID, GIVEN_ID - I2pRCS[0], ID_KP, ID_KI, ID_UPLIM, ID_DNLIM);
    PIDCtrl(&IqPID, VelocityPID.pidout - I2pRCS[1], IQ_KP, IQ_KI, IQ_UPLIM, IQ_DNLIM);
    CtrlAlgo(IdPID.pidout, IqPID.pidout, IxPID.pidout,IyPID.pidout,Udc, ElecTheta, DutyCycle,&out1,&out2);
    SetCMP(DutyCycle);
}
#endif
#if (AlgorithMode==1)
void MPCC_V()
{
    DutyCycle[0]=INV1_duty[index_V1][0];
    DutyCycle[1]=INV1_duty[index_V1][1];
    DutyCycle[2]=INV1_duty[index_V1][2];
    DutyCycle[3]=INV1_duty[index_V1][3];
    DutyCycle[4]=INV1_duty[index_V1][4];
    DutyCycle[5]=INV2_duty[index_V1][0];
    DutyCycle[6]=INV2_duty[index_V1][1];
    DutyCycle[7]=INV2_duty[index_V1][2];
    DutyCycle[8]=INV2_duty[index_V1][3];
    DutyCycle[9]=INV2_duty[index_V1][4];
    SetCMP(DutyCycle);
    EMFdk=-OmegaeE*MOTOR_LQ*I2pRCS[1];
    EMFqk=OmegaeE*MOTOR_LD*I2pRCS[0]+OmegaeE*MOTOR_PSI;
    Ualbek[0]=INV_VV_Real[index_V1];
    Ualbek[1]=INV_VV_Imag[index_V1];
    ParkTrans(Ualbek, ElecTheta, Udqk);
    Idk1=I2pRCS[0]+TS*(__divf32((Udqk[0]-MOTOR_RS*I2pRCS[0]-EMFdk),MOTOR_LD));
    Iqk1=I2pRCS[1]+TS*(__divf32((Udqk[1]-MOTOR_RS*I2pRCS[1]-EMFqk),MOTOR_LQ));
    index_V1=0;
    int g[10]={0};
    for(ii=0;ii<10;ii++)
    {
        Ualbek1[0]=INV_VV_Real[ii];
        Ualbek1[1]=INV_VV_Imag[ii];
        ParkTrans(Ualbek1, ElecTheta, Udqk1);
        Idk2=Idk1+TS*(__divf32((Udqk1[0]-MOTOR_RS*Idk1-EMFdk),MOTOR_LD));
        Iqk2=Iqk1+TS*(__divf32((Udqk1[1]-MOTOR_RS*Iqk1-EMFqk),MOTOR_LQ));
        g[ii]=(GIVEN_ID-Idk2)*(GIVEN_ID-Idk2)+(VelocityPID.pidout-Iqk2)*(VelocityPID.pidout-Iqk2);
        if(g[index_V1]>g[ii])
        {
            index_V1=ii;
        }
    }
}
#endif

// End of file
