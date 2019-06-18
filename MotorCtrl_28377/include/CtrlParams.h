/*
 * CtrlParams.h
 *
 *  Created on: 2018年1月5日
 *      Author: Ezreal
 *      Modify: Doby(2018.10.9)
 */

/************************************************************
Description:系统参数定义，电机参数定义，控制参数定义，常量定义,控制器参数定义
************************************************************/

#ifndef INCLUDE_SYSHEADER_CTRLPARAMS_H_
#define INCLUDE_SYSHEADER_CTRLPARAMS_H_

//系统参数
//#define _FLASH                                        // FLASH烧写，若烧RAM注释该行
#define SYS_FREQ 200000                                 // 系统主频200000k
#define SWICH_FREQ 10.0                                 // 开关频率10k
#define TS 1.0 / SWICH_FREQ / 1000                      // 采样周期 1/10k
#define EPWM_PRD SYS_FREQ / (4 * SWICH_FREQ)            // 无需修改，自动计算得到。ePWM模块频率(100000k)默认为1/2主频(200000k)，三角载波周期为2*EPWMPRD
#define DB_TIME 200                                     // 死区时间DBTIME/ePWM模块频率，200M下，400->4us

//控制参数
#define LOOP_FLAG 2                                     // 0：速度，电流开环；1：速度开环，电流闭环；2：速度，电流闭环
#define GIVEN_VEL 0.4                                   // LOOP_FLAG   2状态          给定速度
#define GIVEN_TORQUE 80                                 // LOOP_FLAG 1,2状态          给定Torque
#define GIVEN_ID 0                                      // LOOP_FLAG 1,2状态          给定id
#define GIVEN_IQ 1                                      // LOOP_FLAG   1状态          给定iq
#define GIVEN_IX 0                                      // LOOP_FLAG 1,2状态          给定ix
#define GIVEN_IY 0                                      // LOOP_FLAG 1,2状态          给定iy
#define GIVEN_UD 0                                      // LOOP_FLAG   0状态          给定ud
#define GIVEN_UQ 10                                     // LOOP_FLAG   0状态          给定uq

//传感器参数
#define SENSOR 2                                        // 传感器类型，0：光电编码器；1：旋转变压器；2：磁栅尺
#define CODER_NUM 2940                                  // 旋转编码器线数，直线电机改为一对极下的磁栅尺信号数
#define CODER_LEN 0.005                                 // 磁栅尺信号间距(mm)

//电机参数
#define POLE_PAIRS 1                                    // 旋转电机极对数，直线电机改为1
#define POLE_PITCH 0.0147                               // 直线电机磁极距
#define PHASE 5                                         // 电机相数
#define PWM_NUM 31                                      // PWM信号对数
#define MOTOR_RS 0.5                                    // 电机定子电阻(Ω)
#define MOTOR_LS 0.00595                                // 电机定子电感(H)
#define MOTOR_LD 0.0851                                 // 电机d轴电感(H)
#define MOTOR_LQ 0.0852                                 // 电机q轴电感(H)
#define MOTOR_PSI 0.0248                                // 电机永磁磁链(Wb)

//控制器参数
#define VEL_KP 4.0                                      //速度控制器 kp 0.5
#define VEL_KI 160.0                                    //速度控制器 ki 400
#define VEL_UPLIM 3                                     //速度控制器 上限幅 20
#define VEL_DNLIM -3                                    //速度控制器 下限幅 -20

#define Torque_KP 0.000002                              //转矩控制器 kp 0.5
#define Torque_KI 0.0008                                //转矩控制器 ki 400
#define Torque_UPLIM 0.04                               //转矩控制器 上限幅 20
#define Torque_DNLIM -0.04                              //转矩控制器 下限幅 -20

#define ID_KP 5                                         //id控制器 kp 3
#define ID_KI 150                                       //id控制器 ki 200
#define ID_UPLIM 100                                    //id控制器 上限幅 150
#define ID_DNLIM -100                                   //id控制器 下限幅 -150

#define IQ_KP 5                                         //iq控制器 kp 3
#define IQ_KI 200                                       //iq控制器 ki 100
#define IQ_UPLIM 100                                    //iq控制器 上限幅 150
#define IQ_DNLIM -100                                   //iq控制器 下限幅 -150

#define IX_KP 20                                        //ix控制器 kp 20
#define IX_KI 200                                       //ix控制器 ki 100
#define IX_UPLIM 150                                    //ix控制器 上限幅 150
#define IX_DNLIM -150                                   //ix控制器 下限幅 -150

#define IY_KP 20                                        //iy控制器 kp 20
#define IY_KI 200                                       //iy控制器 ki 100
#define IY_UPLIM 150                                    //iy控制器 上限幅 150
#define IY_DNLIM -150                                   //iy控制器 下限幅 -150

//常量
#define SQRT3     1.73205080                            // √3
#define SQRT2     1.41421356                            // √2
#define SQRT2TO2  0.70710678                            // √2/2
#define SQRT3TO2  0.86602540                            // √3/2
#define SQRT2TO3  0.47140452                            // √2/3
#define SQRT3TO3  0.57735027                            // √3/3
#define _2TO3     0.66666667                            // 2/3
#define _1TO3     0.33333333                            // 1/3
#define _2PI      6.28318531                            // 2*pi
#define COS1A     0.30901699                            // cos(2*pi/5)
#define SIN1A     0.95105652                            // sin(2*pi/5)
#define COS2A    -0.80901699                            // cos(4*pi/5)
#define SIN2A     0.58778525                            // sin(4*pi/5)
#define COS1B     0.86602540                            // cos(pi/3)

//求最值函数
#define MAX(a,b) ((a)>(b))?(a):(b)
#define MIN(a,b) ((a)<(b))?(a):(b)

//PID结构体
typedef struct
{
    float lasterr, lastout;
    float pidout;
} PID;

//SCI ValuePart 共用体
typedef union
{
    char char8;
    int int16;
    float float32;
    long long32;
} ValuePart;

//CAN Message Object 配置结构体
typedef struct
{
    //! The CAN message identifier used for 11 or 29 bit identifiers.
    uint32_t MsgID;

    //! The message identifier mask used when identifier filtering is enabled.
    uint32_t MsgIDMask;

    //! This value holds various status flags and settings specified by
    //! tCANObjFlags.
    uint32_t Flags;

    //! This value is the number of bytes of data in the message object.
    uint32_t MsgLen;

    //! This is a pointer to the message object's data.
    unsigned char *pMsgData;
} CanMsgObj;

//CAN MsgObj 模式类型枚举
typedef enum
{
        //! Transmit message object.
        Tx,

        //! Transmit remote request message object
        TxRemote,

        //! Receive message object.
        Rx,

        //! Receive remote request message object.
        RxRemote,

        //! Remote frame receive remote, with auto-transmit message object.
        RxTxRemote
} MsgObjType;

#endif /* INCLUDE_SYSHEADER_CTRLPARAMS_H_ */
