//###########################################################################
// AUTHOR:          Ezreal
// DESCRIPTION:     Motor Controlled by TMS320F28377S
// DATE:            2018/06/27
// VERSION:         1.5.0-alpha
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

//  临时变量定义
int counter = 0;
float ob1=0,ob2=0,ob3=0,ob4=0,ob5=0,ob6=0,ob7=0;
float udcob[2] = {0};
float ImgTheta = 0;
float x = -1;
float y1=0, y2=0;
int DAC_a=0, DAC_b=0;
float iabc[6] = {0};
float iab0[6] = {0};
float idq0[6] = {0};
//  临时变量定义结束

//***************************************************全局变量***************************************************
//电机状态变量
char MotorRunFlag = '0';                                //电机启停标志位
float Velocity = 0;                                     //速度(rpm, mps)
float EstVelocity = 0;
float ElecTheta = 0;                             //电角度(rad)
float EstElecTheta = _2PI / 2;
float Torque = 0;                                       //转矩(N`m, N)
float IOriSCS[PHASE] = {0};                             //原始静止坐标系电流(A)
float I2pSCS[PHASE] = {0};                              //两相静止坐标系电流(A)
float I2pRCS[PHASE] = {0};                              //两相旋转坐标系电流(A)
float UOriSCS[PWM_NUM] = {0};                           //原始静止坐标系电压(V)
float E2pSCS[PHASE] = {0};                              //两相静止坐标系电压(V)
float Psi2pSCS[PHASE] = {0};                            //两相静止坐标系磁链(V)
float Psi2pRCS[PHASE] = {0};                            //两相旋转坐标系磁链(V)

//  系统状态变量
float Udc[2] = {60, 60};                                     //两侧母线电压
float WindingTemp[PHASE] = {0};                         //绕组温度
float DutyCycle[PWM_NUM] = {0};                         //桥臂占空比

//  PID控制器变量
PID VelocityPID = {0, 0, 0};
PID IdPID = {0, 0, 0};
PID IqPID = {0, 0, 0};                      //结构体参数{iacc, lasterr, pidout}
PID IxPID = {0, 0, 0};
PID IyPID = {0, 0, 0};
PID TorquePID = {0, 0, 0};
PID CapPID = {0, 0, 0};
float VelPIDParas[5] = {GIVEN_VEL, VEL_KP, VEL_KI, VEL_UPLIM, VEL_DNLIM};
float IdPIDParas[5] = {GIVEN_ID, ID_KP, ID_KI, ID_UPLIM, ID_DNLIM};
float IqPIDParas[5] = {GIVEN_IQ, IQ_KP, IQ_KI, IQ_UPLIM, IQ_DNLIM};//PID参数数组
float IxPIDParas[5] = {GIVEN_IX, IX_KP, IX_KI, IX_UPLIM, IX_DNLIM};
float IyPIDParas[5] = {GIVEN_IY, IY_KP, IY_KI, IY_UPLIM, IY_DNLIM};
int VelCtrlCNT = 1;                                     //速度控制器分频计数

//  SCI上位机通信变量
ValuePart SendValue;                                    //SCI发送数据共用体
ValuePart RecValue;                                     //SCI接收数据共用体
char HandShakeFlag = '0';                               //握手标志位
char SendFrameFlag = '0';                               //发送帧标志位
char RecFrameFlag = '0';                                //接收帧标志位
char RecFrameErr = '0';                                 //接收帧错误位
char RecFrame[5] = {0};                                 //接收数据帧
int RecFrameCNT = 0;                                    //接收帧计数
int SendFrameCNT = 0;                                   //发送帧计数
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
    PieVectTable.EPWM3_INT = &EPWM3_ISR;        //程序使用ePWM3中断
    PieVectTable.EQEP1_INT = &EQEP1_ISR;        //程序使用eQEP1中断
    PieVectTable.SCIB_RX_INT = &SCIB_RX_ISR;    //程序使用Scib_RX中断
    EDIS;

//  初始化外围设备
    InitADCs();
    InitDACs();
    InitEPwms();
    InitEQep1();
//    InitSpib();
    InitScib();
//    InitCana();
//    InitECap1(50);
//    InitAD2S1210();

//  使能程序用到的中断
    IER |= (M_INT3 | M_INT5 | M_INT9);
    PieCtrlRegs.PIEIER3.bit.INTx3 = 1;          //程序使用ePWM3中断
    PieCtrlRegs.PIEIER5.bit.INTx1 = 1;          //程序使用eQEP1中断
    PieCtrlRegs.PIEIER9.bit.INTx3 = 1;          //程序使用Scib_RX中断

//  Enable global Interrupts and higher priority real-time debug events:
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM
//  初始化结束

//  test code
//    GPIO_SetupPinMux(59, 0, 0);//SPIB的GPIO59未初始化
//    GPIO_SetupPinOptions(59, 1, 0);
//  CPLD IO控制
    GPIO_SetupPinMux(94, 0, 0);
    GPIO_SetupPinOptions(94, 1, 0);
    GPIO_SetupPinMux(93, 0, 0);
    GPIO_SetupPinOptions(93, 1, 0);
    GPIO_WritePin(93, 1);
    GPIO_SetupPinMux(92, 0, 0);
    GPIO_SetupPinOptions(92, 1, 0);
    GPIO_SetupPinMux(91, 0, 0);
    GPIO_SetupPinOptions(91, 1, 0);
    GPIO_SetupPinMux(90, 0, 0);
    GPIO_SetupPinOptions(90, 1, 0);
    GPIO_SetupPinMux(89, 0, 0);
    GPIO_SetupPinOptions(89, 1, 0);
    GPIO_SetupPinMux(88, 0, 0);
    GPIO_SetupPinOptions(88, 1, 0);

    GPIO_WritePin(94, 0);
    DELAY_US(1000*10);
    GPIO_WritePin(94, 1);   //复位

//    GPIO_WritePin(93, 0);
//    DELAY_US(1000*10);
//    GPIO_WritePin(93, 1);
//  CPLD IO控制 end

//  按键初始化
    GPIO_SetupPinMux(81, 0, 0);
    GPIO_SetupPinOptions(81, 0, 0);     //启动
    GPIO_SetupPinMux(82, 0, 0);
    GPIO_SetupPinOptions(82, 0, 0);     //停止
//  按键初始化end

//  test code end

    while(1)
    {
        /************************************************************
        Description:死循环
                                                  通信程序
        ************************************************************/
//  SCI(上位机)通信
        if(HandShakeFlag == '1' && SendFrameFlag == '1')
        {
            SendValue.float32 = VelPIDParas[0];
            ScibSendFrame(0x01, &SendValue, 4);     //GivenVel
            SendValue.float32 = Velocity;
            ScibSendFrame(0x02, &SendValue, 4);     //Velocity

            SendValue.float32 = ElecTheta;
            ScibSendFrame(0x03, &SendValue, 4);     //ElecTheta

            SendValue.float32 = IdPIDParas[0];
            ScibSendFrame(0x04, &SendValue, 4);     //GivenId
            SendValue.float32 = I2pRCS[0];
            ScibSendFrame(0x05, &SendValue, 4);     //Id

            SendValue.float32 = IqPIDParas[0];
            ScibSendFrame(0x06, &SendValue, 4);     //GivenIq
            SendValue.float32 = I2pRCS[1];
            ScibSendFrame(0x07, &SendValue, 4);     //Iq

            for(SendFrameCNT = 0; SendFrameCNT < PWM_NUM; SendFrameCNT++)
            {
                SendValue.float32 = DutyCycle[SendFrameCNT];
                ScibSendFrame(0x08 + SendFrameCNT, &SendValue, 4);
            }                                       //DutyCycle

            for(SendFrameCNT = 0; SendFrameCNT < PHASE; SendFrameCNT++)
            {
                SendValue.float32 = IOriSCS[SendFrameCNT];
                ScibSendFrame(0x0E + SendFrameCNT, &SendValue, 4);
            }                                       //I

            SendValue.float32 = Udc[0];
            ScibSendFrame(0x14, &SendValue, 4);     //Udc1
            SendValue.float32 = Udc[1];
            ScibSendFrame(0x15, &SendValue, 4);     //Udc2

            for(SendFrameCNT = 0; SendFrameCNT < PHASE; SendFrameCNT++)
            {
                SendValue.int16 = WindingTemp[SendFrameCNT];
                ScibSendFrame(0x16 + SendFrameCNT, &SendValue, 2);
            }                                       //Twinding

            SendValue.int16 = IcTempSamp();
            ScibSendFrame(0x1C, &SendValue, 2);     //Tic
        }

//  电机启停控制
        if(EPwm3Regs.ETSEL.bit.INTEN == 0 && (MotorRunFlag == '1' || GpioDataRegs.GPCDAT.bit.GPIO81 == 1))
        {
            MotorRunFlag = '1';
            GPIO_WritePin(93, 0);
            EPwm3Regs.ETSEL.bit.INTEN = 1;      //打开EPWM中断使能
        }                                       //电机启动状态不主动采样
        if(MotorRunFlag == '0' || GpioDataRegs.GPCDAT.bit.GPIO82 == 1)
        {
            MotorRunFlag = '0';
            LineCurSamp(IOriSCS);
            ElecTheta = ElecThetaCal(ElecTheta);
            ClarkTrans(IOriSCS, I2pSCS);
            ParkTrans(I2pSCS, ElecTheta, I2pRCS);
            Velocity = VelocityCal(Velocity);
            BusVoltSamp(Udc);
            WindingTempSamp(WindingTemp);
        }                                       //电机停止状态主动采样

/*can通讯
        CanMsgObj sTXCANMessage;
        unsigned char ucTXMsgData[4];

        *(unsigned long *)ucTXMsgData = 0;
        sTXCANMessage.MsgID = 1;                      // CAN message ID - use 1
        sTXCANMessage.MsgIDMask = 0;                  // no mask needed for TX
        sTXCANMessage.Flags = 0x00000001;  // enable interrupt on TX
        sTXCANMessage.MsgLen = 8;//sizeof(ucTXMsgData);   // size of message is 4
        sTXCANMessage.pMsgData = ucTXMsgData;           // ptr to message content

        CanaMessageSet(1, &sTXCANMessage, Tx);
        DELAY_US(1000*1000);

        (*(unsigned long *)ucTXMsgData)++;
*/

/*DA输出心形
        if(x<0)
            y1 = pow(-x,2.0/3)+sqrt(1-pow(-x,2))-1;//0-0.52
        else
            y1 = pow(x,2/3.0)+sqrt(1-pow(x,2))-1;//0-0.52

        if(x<0)
            y2 = pow(-x,2.0/3)-sqrt(1-pow(-x,2))+1;//0-2
        else
            y2 = pow(x,2/3.0)-sqrt(1-pow(x,2))+1;//0-2


        DAC_a=0x00FF & (int)(y1*200) | 0x0000;
        DAC_b=0x00FF & (int)(y2*127) | 0x0200;

        DAC_a <<= 5;
        DAC_b <<= 5;

        GPIO_WritePin(59, 1);

        SpibOut(DAC_a);
        DELAY_US(40);
        GPIO_WritePin(59, 0);
        GPIO_WritePin(59, 1);

        SpibOut(DAC_b);
        DELAY_US(40);
        GPIO_WritePin(59, 0);
        GPIO_WritePin(59, 1);
*/

//        iabc[0] = sin(ImgTheta);
//        iabc[1] = sin(ImgTheta-1.047);
//        iabc[2] = sin(ImgTheta-2.094);
//        iabc[3] = sin(ImgTheta-3.142);
//        iabc[4] = sin(ImgTheta-4.189);
//        iabc[5] = sin(ImgTheta-5.236);
//
//        ClarkTrans(iabc, iab0);
//        ParkTrans(iab0, ImgTheta, idq0);
//
//        ob1 = idq0[0];
//        ob2 = idq0[1];
//        ob3 = idq0[2];
//        ob4 = idq0[3];
//        ob5 = idq0[4];
//        ob6 = idq0[5];
//        ScibOut(0x55);
//        ScibOut(0xAA);

//        ElecThetaEst(1);
//        ob1 = VelocityEst(1);

//        EmfEst(Udc, DutyCycle, I2pSCS);

//        PhaseVoltSamp(UOriSCS);
        DELAY_US(10);
    }
}

interrupt void EPWM3_ISR(void)
{
    /************************************************************
    Description:EPWM1中断子程序
                                        整体控制程序，采样频率=控制频率=PWM频率
    ************************************************************/
    LineCurSamp(IOriSCS);                                      //电流采样
    BusVoltSamp(Udc);

    ElecTheta = ElecThetaCal(ElecTheta);                            //电角度计算

    ClarkTrans(IOriSCS, I2pSCS);
    ParkTrans(I2pSCS, ElecTheta, I2pRCS);                  //坐标变换

    Velocity = VelocityCal(Velocity);                               //速度计算，语句位置与函数待优化，计算模式待修改

//  磁链估计
//    Psi2pRCS[0] = MOTOR_LD * I2pRCS[0] + MOTOR_PSI;
//    Psi2pRCS[1] = MOTOR_LQ * I2pRCS[1];
//
//    IParkTrans(Psi2pRCS, ElecTheta, Psi2pSCS);
//    Torque = 3 * 3.14 / POLE_PITCH * (Psi2pSCS[0] * I2pSCS[1] - Psi2pSCS[1] * I2pSCS[0]);
//
//  磁链估计结束

//    EmfEstCal(Udc, DutyCycle, I2pSCS);
//    EstVelocity = VelocityEstCal(EstVelocity);
//    EstElecTheta = ElecThetaEstCal(EstElecTheta);

    if(VelCtrlCNT++ == 100)
    {
//        Velocity = VelocityCal(Velocity);
        if(LOOP_FLAG == 2)
        {
            PIDCtrl(&VelocityPID, VelPIDParas[0] - Velocity, VelPIDParas[1], VelPIDParas[2], VelPIDParas[3], VelPIDParas[4]);
        }
        VelCtrlCNT = 1;
//        SetDACcValue(Velocity * 3000);
    }                                                      //速度控制，100倍的电流控制周期，判断顺序待调整


//    PIDCtrl(&CapPID, 50 - Udc[1], 0.2, 0.1, 20, -20);


    switch(LOOP_FLAG)
    {
        case 2:                                            //id*=GIVEN_ID, iq*=VelocityPIDOut
        {
//            PIDCtrl(&TorquePID, VelocityPID.pidout - Torque, Torque_KP, Torque_KI, Torque_UPLIM, Torque_DNLIM);
            PIDCtrl(&IdPID, GIVEN_ID - I2pRCS[0], ID_KP, ID_KI, ID_UPLIM, ID_DNLIM);
            PIDCtrl(&IqPID, VelocityPID.pidout - I2pRCS[1], IQ_KP, IQ_KI, IQ_UPLIM, IQ_DNLIM);
//            PIDCtrl(&IxPID, GIVEN_IX - I2pRCS[2], IX_KP, IX_KI, IX_UPLIM, IX_DNLIM);
//            PIDCtrl(&IyPID, GIVEN_IY - I2pRCS[3], IY_KP, IY_KI, IY_UPLIM, IY_DNLIM);
        }break;
        case 1:                                            //id*=GIVEN_ID, iq*=GIVEN_IQ
        {
//            PIDCtrl(&TorquePID, GIVEN_TORQUE - Torque, Torque_KP, Torque_KI, Torque_UPLIM, Torque_DNLIM);
            PIDCtrl(&IdPID, GIVEN_ID - I2pRCS[0], ID_KP, ID_KI, ID_UPLIM, ID_DNLIM);
            PIDCtrl(&IqPID, GIVEN_IQ - I2pRCS[1], IQ_KP, IQ_KI, IQ_UPLIM, IQ_DNLIM);
        }break;
        case 0:                                            //ud*=GIVEN_UD, uq*=GIVEN_UQ
        {
            IdPID.pidout = GIVEN_UD;
            IqPID.pidout = GIVEN_UQ;
        }break;
    }                                                      //id,iq电流控制

    Udc[0] = 35;
    Udc[1] = 25;

    CtrlAlgo(IdPID.pidout, IqPID.pidout, 0, I2pRCS, Udc, ElecTheta, DutyCycle);            //DTC-OEW-UP
//    CtrlAlgo(IdPID.pidout, IqPID.pidout, IxPID.pidout, IyPID.pidout, Udc, ElecTheta, DutyCycle);                //五相SVPWM
//    CtrlAlgo(IdPID.pidout, IqPID.pidout, Udc, ElecTheta, DutyCycle);                //三相SVPWM
//    CtrlAlgo(VelocityPID.pidout, IOriSCS, ElecTheta, DutyCycle);                  //三相Bang-Bang
                                                                                    //控制算法函数，输入User-defined 输出DutyCycle
//    DutyCycle[0] = 0.8;
//    DutyCycle[1] = 0.8;
//    DutyCycle[2] = 0.8;
//    DutyCycle[3] = 0.8;
//    DutyCycle[4] = 0.8;
//    DutyCycle[5] = 0.8;
    SetCMP(DutyCycle);                                     //更新cmp

//    SetDACaValue(udcob[0] * 500);
//    SetDACaValue(Psi2pSCS[0] * 3000 + 1250);
//    SetDACaValue(GIVEN_TORQUE * 30);
//    SetDACaValue(VelocityPID.pidout * 1000);
//    SetDACaValue(Velocity * 3000);
//    SetDACaValue(E2pSCS[0] * 100 + 2000);
//    SetDACaValue(0 * 1500 + 2000);
//    SetDACaValue(1000);
//    SetDACaValue(I2pRCS[1] * 1000);
//    SetDACaValue(I2pRCS[2] * 1000 + 2000);
//    SetDACaValue(I2pRCS[3] * 1000 + 2000);
//    SetDACaValue(ob6 * 500 + 1000);
    SetDACaValue(DutyCycle[0] * 2000);
//    SetDACaValue(EQep1Regs.QPOSCNT / 3);
//    SetDACaValue(iOriSCS[0] * 140 + 2048);
//    SetDACaValue(ob2 * 500 + 2048);
//    SetDACaValue(ElecTheta * 400);
//    SetDACaValue(VelocityPID.pidout * 2000);
//    SetDACaValue(Psi2pSCS[0] * 2000 + 2000);
//    SetDACaValue(VelPIDParas[0] * 3000);
//    SetDACaValue(0);

//    SetDACbValue(I2pRCS[0] * 1000 + 1000);
//    SetDACbValue(Udc[0] * 400);
//    SetDACbValue(Velocity * 5000);
//    SetDACbValue(Psi2pSCS[1] * 3000 + 1250);
//    SetDACbValue(Torque * 30);
//    SetDACbValue(ElecTheta * 500);
//    SetDACbValue(VelPIDParas[0] * 3000);
//    SetDACbValue(I2pRCS[1] * 1000);
    SetDACbValue(DutyCycle[3] * 2000);
//    SetDACbValue(0 * 500 + 1000);
//    SetDACbValue(GIVEN_ID * 1000 + 2000);
//    SetDACbValue(GIVEN_IQ * 1000 + 300);
//    SetDACbValue(I2pRCS[0] * 1000 + 2000);
//    SetDACbValue(2000);
//    SetDACbValue(GIVEN_IX * 1000 + 2000);
//    SetDACbValue(GIVEN_IY * 1000 + 2000);
//    SetDACbValue(E2pSCS[1] * 100 + 2000);
//    SetDACbValue(1000);

//    SetDACcValue(DutyCycle[2] * 4000);                                //DAC输出
//    SetDACcValue(Velocity * 5000);
//    SetDACcValue(I2pRCS[0] * 2000 + 1000);
//    SetDACcValue(Udc[1] * 400);
//    SetDACcValue(2000);

    //test code start
//    ImgTheta += 0.0001;
//    ImgTheta = fmod(ImgTheta, _2PI);

//    x += 0.001;
//    if (x>1)
//    {
//        x = -1;
//        counter++;
//    }
//    if(counter >= 100)
//        MotorRunFlag = '0';
    //test code end

//  电机启停控制
    if(MotorRunFlag == '0' || GpioDataRegs.GPCDAT.bit.GPIO82 == 1)
    {
        MotorRunFlag = '0';
        GPIO_WritePin(93, 1);
        memset(&VelocityPID, 0, sizeof(VelocityPID));
        memset(&IdPID, 0, sizeof(IdPID));
        memset(&IqPID, 0, sizeof(IqPID));
        memset(&IxPID, 0, sizeof(IxPID));
        memset(&IyPID, 0, sizeof(IyPID));
        memset(&TorquePID, 0, sizeof(TorquePID));
        memset(&CapPID, 0, sizeof(CapPID));
        memset(DutyCycle, 0, sizeof(DutyCycle));
        SetCMP(DutyCycle);
        EPwm3Regs.ETSEL.bit.INTEN = 0;      //关闭EPWM中断使能
    }                                       //电机停转

    EPwm3Regs.ETCLR.bit.INT = 1;            //清除中断标志位
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3; //允许下次中断进入
}

interrupt void EQEP1_ISR(void)
{
    /************************************************************
    Description:EQEP1中断子程序
                Z信号产生时进入中断，用于清除编码器累计误差
    ************************************************************/
#if (SENSOR == 0)
    ElecTheta = 4.13661666;          //1.767145867;
#endif

#if (SENSOR == 2)
    ob1++;

    if(ob1 == 70)
    {
        VelPIDParas[0] = 0.4;
    }

    if(ob1 == 200)
    {
//        VelPIDParas[0] = -VelPIDParas[0];
        MotorRunFlag = '0';
    }

    if(ob1 == 400)
    {
        MotorRunFlag = '0';
        VelPIDParas[0] = -VelPIDParas[0];
        ob1 = 0;
    }

    EQep1Regs.QCLR.bit.IEL = 1;
    EQep1Regs.QCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP5;
#endif
}

interrupt void SCIB_RX_ISR(void)
{
    /************************************************************
    Description:SCIC接收中断子程序
                                        用于处理上位机发送来的数据
    ************************************************************/
    if(ScibRegs.SCIRXST.bit.RXERROR == 0)                   //波特率，数据位无错，开始接收数据
    {
        char rec = 0;
        int valuecnt = 0;

        rec = ScibRegs.SCIRXBUF.all;

        if(rec == 0x55 && RecFrameCNT == 6)
        {
            RecFrameCNT = 0;
            RecFrameFlag = '0';
        }
        else if(RecFrameFlag == '1')
        {
            if(RecFrameCNT <= 5)
            {
                RecFrame[RecFrameCNT-1] = rec;
            }
            RecFrameCNT++;
        }
        else if(rec == 0xAA && RecFrameCNT == 0)
        {
            RecFrameCNT++;
            RecFrameFlag = '1';
        }                                                   //接收帧处理判断
        else
        {
            RecFrameErr = '1';
        }

        if(RecFrameErr == '0')                                  //帧无错
        {
            if(RecFrameFlag == '0')                         //完整帧接收且，处理帧信息
            {
                if(RecFrame[0] == 0xFF)
                {
                    if(RecFrame[4] == 0x55)
                    {
                        HandShakeFlag = '1';
                        ScibOut(0x55);
                    }
                    else if(RecFrame[4] == 0xAA)
                    {
                        HandShakeFlag = '0';
                        ScibOut(0xAA);
                    }
                }

                if(HandShakeFlag == '1')                    //握手成功
                {
                    switch(RecFrame[0])
                    {
                        case 0x01://GivenVel
                        {
                            for(valuecnt = 0; valuecnt <= 3; valuecnt++)
                            {
                                RecValue.long32 <<= 8;
                                RecValue.long32 |= 0xFF & RecFrame[valuecnt + 1];
                            }
                            VelPIDParas[0] = RecValue.float32;
                        }break;
                        case 0x02://VelKp
                        {
                            for(valuecnt = 0; valuecnt <= 3; valuecnt++)
                            {
                                RecValue.long32 <<= 8;
                                RecValue.long32 |= 0xFF & RecFrame[valuecnt + 1];
                            }
                            VelPIDParas[1] = RecValue.float32;
                        }break;
                        case 0x03://VelKi
                        {
                            for(valuecnt = 0; valuecnt <= 3; valuecnt++)
                            {
                                RecValue.long32 <<= 8;
                                RecValue.long32 |= 0xFF & RecFrame[valuecnt + 1];
                            }
                            VelPIDParas[2] = RecValue.float32;
                        }break;
                        case 0x04://VelUpLim
                        {
                            for(valuecnt = 0; valuecnt <= 3; valuecnt++)
                            {
                                RecValue.long32 <<= 8;
                                RecValue.long32 |= 0xFF & RecFrame[valuecnt + 1];
                            }
                            VelPIDParas[3] = RecValue.float32;
                        }break;
                        case 0x05://VelDnLim
                        {
                            for(valuecnt = 0; valuecnt <= 3; valuecnt++)
                            {
                                RecValue.long32 <<= 8;
                                RecValue.long32 |= 0xFF & RecFrame[valuecnt + 1];
                            }
                            VelPIDParas[4] = RecValue.float32;
                        }break;
                        case 0xFD://数据接收启停
                        {
                            if(RecFrame[4] == 0x55)
                            {
                                SendFrameFlag = '1';
                            }
                            else if(RecFrame[4] == 0xAA)
                            {
                                SendFrameFlag = '0';
                            }
                        }break;
                        case 0xFE://电机启停
                        {
                            if(RecFrame[4] == 0x55)
                            {
                                MotorRunFlag = '1';
                            }
                            else if(RecFrame[4] == 0xAA)
                            {
                                MotorRunFlag = '0';
                            }
                        }break;
                    }
                }
            }
        }
        else                                        //数据帧出错处理
        {
            RecFrameCNT = 0;
            RecFrameFlag = '0';
            RecFrame[0] = 0;
            RecFrameErr = '0';
        }
    }
    else                                            //波特率，数据位出错处理
    {
        ScibRegs.SCICTL1.bit.SWRESET = 0;
        ScibRegs.SCICTL1.bit.SWRESET = 1;
    }

    ScibRegs.SCIFFRX.bit.RXFFOVRCLR = 1;   // Clear Overflow flag
    ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;   // Clear Interrupt flag

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;       // Issue PIE ack
}
// End of file

/********************************************************
                   _ooOoo_
                  o8888888o
                  88" . "88
                  (| -_- |)
                  O\  =  /O
               ____/`---'\____
             .'  \\|     |//  `.
            /  \\|||  :  |||//  \
           /  _||||| -:- |||||-  \
           |   | \\\  -  /// |   |
           | \_|  ''\---/''  |   |
           \  .-\__  `-`  ___/-. /
         ___`. .'  /--.--\  `. . __
      ."" '<  `.___\_<|>_/___.'  >'"".
     | | :  `- \`.;`\ _ /`;.`/ - ` : | |
     \  \ `-.   \_ __\ /__ _/   .-` /  /
======`-.____`-.___\_____/___.-`____.-'======
                   `=---='
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                                      佛祖保佑       永无BUG
********************************************************/
/********************************************************
*
*              x     x
*             0 0   0 0
*          0 0   0 0
*       00000000000000000
*      0.0                0
*      0.0    x    0 0     0
*       0000000       [x]  0
*             0    U       0
*              0          0
*               000000000
*             0          0__
*            /0          0  |
*         __/ 0   0   0  0 _|
*          |\ 00000000000  |\
*                 0   0
*                 0   0
*               000 000
*       小猪佩奇身上纹   掌声送给社会人
********************************************************/
/********************************************************




                                                                                                         .``````````````````````
                                                                                                        :@######################!
                                                                                                        !#######################%.
                                                                                                       `$#######################&`
                                                                                                       :@#######################@:
                                                                      `$#!                             |#########################!                             '$|.
                                                                   ;@#####&'                          `$#########################%.                          .%#####&'
                                                               `$###########|                         ;##########################&`                         ;###########!
                                                            ;@###############&'                       |##########################@:                       '&###############$'
                                                        `%#####################|                '!$##################################&%;`               .%####################@!
                                                     :@#########################@'      `!&##################################################@%:       ;##########################$`
                                                    .%############################%!&###############################################################%!&############################@:
                                                     '&############################################################################################################################;
                                                      ;##############################################@%!:`                    .';|&###############################################!
                                                       |#####################################%:.                                         '|&#####################################|
                                                       `$##############################%'            .:!%&####################@$|;`            .;&##############################%.
                                                        :@########################%`         `!&########################################@%:         .;@########################$`
                                                         !####################|.       `|@#####################################################$:        :&###################@'
                                                          |###############$`       !@###############################################################%`       !@###############:
                                                        !##############|.     .!@#######################################################################$'      '&#############$`
                                                     .%#############|      '&#######################@%;'                        .:|&########################!.     :&############@:
                                                   `$############$`     ;@###################@|`                                        .;$####################%.     ;@###########@;
                            ..                   .$###########@;     :@#################&;                                                    `%##################|.    `$###########@;
                           ;####@!.            .%###########$`    `$################$'                                                            .!@###############@;     !###########@:              '%###%
                          !##########&;       !###########%.    :@##############@:                                                                    `%###############%.    ;###########$`      .;&#########$`
                         |################$';@##########%.    !##############$'                                                                           |##############$`    ;###########|'|@###############&'
                       .%#############################$`    |#############&'                                                                                .%#############&'    !#############################@:
                      `$############################&'    |#############!                                                                                      '&############&`    |#############################;
                     '&############################!    ;############@:                                                                                          .%############%.   `&############################!
                    :@###########################$`   '&###########&`                                                                                               |############!    !############################|
                   ;############################;    |###########&`                                                                                                   !###########&'   `$###########################%.
                  !###########################&'   '@##########@:      ````'$###%`````    :&##%.!##@:;##@:       '&##|          '&#@:!#######$`   .%##$`  :@##$`        |###########|    |###########################$`
                 |###########################%    !###########!      '&###############@: |#####%%########%..%##############!  :@#####%|@#####@:  '%@##$$##########|      `&##########$`   :@##########################&'
                   |########################!   .%##########$.        .::::;$###%:::::`   '|@##%$########$``&##$;|@##$;|@##|  |######@%&#;:##@: `&####&|&########@:        ;##########@:   '&########################$`
                     :@####################;   '&##########;          :@##############|   ;#%%#!;&#######| `&##############|   .%@|%%!&##;:@#@:   `%##&:.%#######$`         `$##########!   `$####################@;
                       .%#################;   :@#########&`            ::```````````:|' `$##&%@#$$#######%.`&##%';@##%`;&##|  '&##|%##%$#;:@#@:    |##&'`$#######@:           !##########|   .$#################|.
                          ;##############;   :@#########%.             :@############!   !@###!!#########@:`$##############|  %####&':@#@::@$;:` ;#####$$###| ;##@:            ;##########|   .$#############&'
                            `&##########;   :@#########!               :@##|.    !###!     !##%%##########; `!%%%$###@%%!|@#|. `!@#@;%##$`:@&&#$:%####$%###&:`%##&'             '&#########|   `$#########@;
                             '@########!   :@#########!                :@############!     !##|   .%##%.         '&#########$`  '@#$$###; :####%      !###@|$####%.              '&#########|   '&########|
                            .$########|   '&#########!                  '|%%%%%%%%%|:      `!|`    '||`           :%$$$$$$$|`    :%;`:|'   `;;`        :%;  `!||;.                '&#########!   :@########;
                            !########$`  .%#########!                                                                                                                              '&#########:   !########$`
                           '@#######@:   |#########|                                                                                                                                :@########&'  .%########|
                           |########|   :@########$`                                                                                                                                 !#########|   :@#######@'
                          :@#######&`  .$########@:           .;%&@#####@&|:.                                `;%&######@$!'                                 '!$@######&%!'           .%#########;   |########|
                          |########!   ;#########!      .!@#####################&;                     `|@#####################$'                     '$######################%`      '&########%.  '&#######@'
                         '&#######&`  `$########&`   !@#############################@;             .%##############################&'             '$##############################%`   !#########;   |########!
                         !########|   ;#########!`%#########$:              .;&#########|.      '&#########%'              `!@########@;       ;@########@|`              '%#########&:'&########%.  :@#######$`
                       `;&#######@:  .%#################|.                       `%########! `$#######@;                        '&#######@; :@#######&:                        ;@################@:  .%########!
        `;|$@####################$`  '&#############@:                               ;@############$'                              .|#############%.                              `$##############!   !################@$|:`
        %########################|   ;############!                                     |#######@:                                    .$#######$`                                    '&###########%.  :@#######################&'
        %########################;   |#############@;                                 !###########&'                                .%###########%.                                `$#############&'  `$#######################&'
        %#######################@:  .%################|                            .%######$';@#####@;                            `&######|`|######&'                            :@######@########@:  .%#######################&'
        %#######################&'  `$########| .%######$`                       `&######|     `$######|                        :@#####@;     :@#####@;                        !######&' :#########;   |#######################&'
        %#######################&'  `$########|    !######&'                   :@#####@;         .|######%.                   !######&'         `$######|                   .|######%.   :@########;   |#######################&'
        %#######################&'  `&########|      :@#####@:               ;######&'              !######&`               |######%.             .%######%.              `$######|      :@########!   |#######################&'
        %#######################&'  `$########|        `&######!           |######%.                  :@#####@:          `$######|                   !######$`          '&#####@;        :@########;   |#######################&'
        %#######################&'  `$########|          .%######%.     `$######|                       '&######!      :&######;                       ;@#####@:      ;######&'          :@########;   |#######################&'
        %#######################@:  .%########%.            |######&' :@######;                           .%######%. !######&'                           '&######| .%######$`            ;########@:  .%#######################&'
        %########################;   |########&'              ;@###########&'                                !############$`                               .%############|               |########&'  `$#######################&'
        %########################|   ;#########:                `&#######$.                                    :@#######|                                     !########;                .%########$.  '@#######################&'
         .:!$@###################$.  '@########|              :&###########&'                                ;@###########%.                               .|############!              '@########|   ;########################$`
                     ';|&########@'  .%########$`          !@#######%:$#######@;                         .%########!;&#######&'                         '$#######@;!@#######%`          !########@:   |#########&%|;'.
                         |########!   !#########;     `|@########$`     '&########@!.                '%#########|.     ;@########&;                 :$########@;      !#########$'     `$########$`  '&#######&`
                         :@#######$`  `&#########@############|.           `%############@$%||%&@###########@;            '&############&$|||$&############&:            ;@###########@##########!   !########|
                         .%########;   !##################%`                   '$#######################@!.                   :&#######################&;                   .!@#################$`  `$#######@:
                          ;########$`  `$##########@%;.                             `;$###########@%:                              '!&###########&|'                              :|&###########;   !########%.
                          .%########!   ;#########%.                                                                                                                                 ;#########%.  '&#######@:
                           :@#######&'   %#########!                                                                                                                                '&########@:   |########|
                            |########%.  `&#########;                                                                                                                              `$#########!   ;########&'
                            `&########!   :@########@:                                                                                                                            .%#########|   '&########!
                             ;#########;   ;#########@:                                                                                                                          .%#########%.  .$########%.
                             ;@########@:   !##########;                                                                                                                        `$#########$`   %#########%.
                          `$############&'   !##########!                  ;&&;             ;@@$'      :@@@@@@@@%:              `$#!      :$#@%`      `%@#&:                   '&#########$`   |############@:
                        |################&'   !##########%                 !##!            :@###$`     ;##@%%%$@##@:           |###|    '@#@%$##%.  :@##%%@#&'                :@#########$`   |################|
                     :@###################&'   ;##########&'               !##!           `&#$$##|     ;##|     ;##!        '&##@##|   .%#@'  !##! `$#$`                     |##########%.   |###################$`
                  `$#######################@:   '&##########!              !##!          .%#@: |##!    ;###@@@@###!         .`  ;##|   '&#$`  ;##| :@#@@###&'              '&##########!   .%######################@;
                '&###########################!   .%##########&'            !##!          !##!  `$##:   ;##@$$$$@###!            ;##|   '&#$`  :##% :###;  !##!            |##########@:   '&##########################%.
                 `$###########################%.   !###########%.          !##!         ;##########&'  ;##|     .%##;           ;##|   `$#&'  ;##! '&#$`  '&#%.         ;###########$`   ;###########################@:
                  .%###########################&'   `$###########|         !##%''''''` '&#&'     ;##$` ;##$:''';$##&` `!%;      ;##|    !##| '&#&'  !##%. |##!        '@###########!    |###########################&'
                    |############################!    ;############!       !#########@!$##;       |##| ;#########@;   '&#%.     ;##|     '&####%.    '&####$`       '&###########$`   '&###########################$`
                     !############################&'   .%############|                                                                                            :@###########@:    |############################%.
                      ;#############################%.   `$############&`                                                                                       |#############;    ;@############################|
                       :@#############################!    '&#############!                                                                                  '&#############!    '&#############################!
                        '&#################@@###########;    `&#############@;                                                                            `$##############!    `$##########@&##################;
                         .$###########$:     `$###########;    .%###############!                                                                      '&##############@:    `$###########;     :&###########@:
                           |####@|`            :@###########!     ;################&'                                                              .|################%`    '&###########|           .!@#####&'
                            ..                   :@###########%.    .%#################&;                                                      `%#################@:     ;@###########%.                 `||`
                                                   ;@###########@:     `$###################&;.                                           '%###################@;     `%############%.
                                                     :@############$`     `%#######################$;`                            .:|&######################@:      !#############|
                                                       `$#############$`      ;@##############################@@&$$%%$$&&@###############################%`      !@#############;
                                                         .%##############&:       ;@#################################################################%`      .|##############@'
                                                         !###################%`       `|########################################################&:        ;@##################|
                                                        :########################%`         :%############################################&!`        .!@#######################;
                                                       '&############################@|`           `;|&##########################@$!'            ;&############################@'
                                                      `$###################################&!`                                            .:$###################################$.
                                                     .%############################################&|:`                          .'!$@###########################################|
                                                     |############################################################################################################################;
                                                    !##############################@##############################################################################################&'
                                                    '$##########################@:     '%@#####################################################&;.    '&###########################!
                                                       .!######################|              '!&######################################@%;.             |######################&:
                                                           '&################$`                      .%##########################@:                      '&#################|.
                                                              .|###########@:                         !##########################$`                        |############&:
                                                                  :&######|                           :@#########################!                          '&#######%.
                                                                     .|#$`                            `&########################@:                            !##@:
                                                                                                      .%########################$.
                                                                                                       !########################!
                                                                                                       :@######################@:
                                                                                                       `&######################%.
                                                                                                        :||||||||||||||||||||||'




********************************************************/
