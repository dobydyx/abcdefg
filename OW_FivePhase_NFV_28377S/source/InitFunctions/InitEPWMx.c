/*
 * InitEPWMx.c
 *
 *  Created on: 2018年1月4日
 *      Author: Ezreal, Doby
 */

/************************************************************
Description:初始化ePWM3, 4, 6, 7, 8, 9包括对应176脚封装的GPIO口初始化
************************************************************/

#include "F28x_Project.h"
#include "CtrlParams.h"

/*
 * board1 A相驱动-EPWM1
 */
void InitEPwm1(void)
{
    InitEPwm1Gpio();                                //初始化PWMx的GPIO

    //时基模块工作模式
    EPwm1Regs.TBPRD = EPWM_PRD;                      // 设置三角载波周期
    EPwm1Regs.TBPHS.bit.TBPHS = 0x0000;             // Phase is 0
    EPwm1Regs.TBCTR = 0x0000;                       // Clear counter
    EPwm1Regs.TBCTL.bit.CTRMODE = 0x2;              // 递增递减双向模式 Up-down count mode
    EPwm1Regs.TBCTL.bit.PHSEN = 0x0;                // Disable phase loading
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0x0;            // Clock ratio to SYSCLKOUT  TBCLK = EPWMCLK / (HSPCLKDIV x CLKDIV)
    EPwm1Regs.TBCTL.bit.CLKDIV = 0x0;               // Slow just to observe on the scope

    // 计数比较模块CMPA初始值（可删）
    EPwm1Regs.CMPA.bit.CMPA = 0;

    // 影子寄存器设置
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = 0;  //Shadow mode
    EPwm1Regs.CMPCTL.bit.LOADAMODE = 0;             // Load on CTR = Zero

    // 动作模块，高电平有效
    EPwm1Regs.AQCTLA.bit.CAU = 2;            // Set PWMxA on 1 Action When TBCTR = CMPA on Up Count，10: Set: force EPWMxA output high.
    EPwm1Regs.AQCTLA.bit.CAD = 1;            // Set PWMxA on 0 Action When TBCTR = CMPA on Down Count，01: Clear: force EPWMxA output low.

    // 死区产生模块
    EPwm1Regs.DBCTL.bit.OUT_MODE = 0x3; //死区输出模式，RED和FED都使能
    EPwm1Regs.DBCTL.bit.POLSEL = 0x2;   //死区设置在B路PWM上
    EPwm1Regs.DBCTL.bit.IN_MODE = 0x0;  //死区输入模式控制
    EPwm1Regs.DBRED.bit.DBRED = DB_TIME;    //上升沿死区
    EPwm1Regs.DBFED.bit.DBFED = DB_TIME;    //下降沿死区

    // PWM中断设置
    EPwm1Regs.ETSEL.bit.INTSEL = 0x1;   //Enable event time-base counter equal to zero.
    EPwm1Regs.ETPS.bit.INTPRD = 0x1;    //Generate INT on 1st event
//    EPwm1Regs.ETSEL.bit.INTEN = 1;      //打开EPWM中断使能
}

/*
 * board1 B相驱动-EPWM2
 */
void InitEPwm2(void)
{
    InitEPwm2Gpio();                                //初始化PWMx的GPIO

    //时基模块工作模式
    EPwm2Regs.TBPRD = EPWM_PRD;                      // 设置三角载波周期
    EPwm2Regs.TBPHS.bit.TBPHS = 0x0000;             // Phase is 0
    EPwm2Regs.TBCTR = 0x0000;                       // Clear counter
    EPwm2Regs.TBCTL.bit.CTRMODE = 0x2;              // 递增递减双向模式
    EPwm2Regs.TBCTL.bit.PHSEN = 0x0;                // Disable phase loading
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0x0;            // Clock ratio to SYSCLKOUT
    EPwm2Regs.TBCTL.bit.CLKDIV = 0x0;               // Slow just to observe on the scope

    // 计数比较模块CMPA初始值（可删）
    EPwm2Regs.CMPA.bit.CMPA = 0;

    // 影子寄存器设置
    EPwm2Regs.CMPCTL.bit.SHDWAMODE = 0;
    EPwm2Regs.CMPCTL.bit.LOADAMODE = 0;             // 计数器为0时，载入比较值

    // 动作模块，高电平有效
    EPwm2Regs.AQCTLA.bit.CAU = 2;            // Set PWMxA on 1
    EPwm2Regs.AQCTLA.bit.CAD = 1;            // Set PWMxA on 0

    // 死区产生模块
    EPwm2Regs.DBCTL.bit.OUT_MODE = 0x3; //死区输出模式，RED和FED都使能
    EPwm2Regs.DBCTL.bit.POLSEL = 0x2;   //死区设置在B路PWM上
    EPwm2Regs.DBCTL.bit.IN_MODE = 0x0;  //死区输入模式控制
    EPwm2Regs.DBRED.bit.DBRED = DB_TIME;    //上升沿死区
    EPwm2Regs.DBFED.bit.DBFED = DB_TIME;    //下降沿死区
}
/*
 * board1 C相驱动-EPWM3
 */
void InitEPwm3(void)
{
    InitEPwm3Gpio();                                //初始化PWMx的GPIO

    //时基模块工作模式
    EPwm3Regs.TBPRD = EPWM_PRD;                      // 设置三角载波周期
    EPwm3Regs.TBPHS.bit.TBPHS = 0x0000;             // Phase is 0
    EPwm3Regs.TBCTR = 0x0000;                       // Clear counter
    EPwm3Regs.TBCTL.bit.CTRMODE = 0x2;              // 递增递减双向模式
    EPwm3Regs.TBCTL.bit.PHSEN = 0x0;                // Disable phase loading
    EPwm3Regs.TBCTL.bit.HSPCLKDIV = 0x0;            // Clock ratio to SYSCLKOUT
    EPwm3Regs.TBCTL.bit.CLKDIV = 0x0;               // Slow just to observe on the scope

    // 计数比较模块CMPA初始值（可删）
    EPwm3Regs.CMPA.bit.CMPA = 0;

    // 影子寄存器设置
    EPwm3Regs.CMPCTL.bit.SHDWAMODE = 0;
    EPwm3Regs.CMPCTL.bit.LOADAMODE = 0;             // 计数器为0时，载入比较值

    // 动作模块，高电平有效
    EPwm3Regs.AQCTLA.bit.CAU = 2;            // Set PWMxA on 1
    EPwm3Regs.AQCTLA.bit.CAD = 1;            // Set PWMxA on 0

    // 死区产生模块
    EPwm3Regs.DBCTL.bit.OUT_MODE = 0x3; //死区输出模式，RED和FED都使能
    EPwm3Regs.DBCTL.bit.POLSEL = 0x2;   //死区设置在B路PWM上
    EPwm3Regs.DBCTL.bit.IN_MODE = 0x0;  //死区输入模式控制
    EPwm3Regs.DBRED.bit.DBRED = DB_TIME;    //上升沿死区
    EPwm3Regs.DBFED.bit.DBFED = DB_TIME;    //下降沿死区
}

/*
 * board2 A相驱动-EPWM4
 */
void InitEPwm4(void)
{
    InitEPwm4Gpio();                                //初始化PWMx的GPIO

    //时基模块工作模式
    EPwm4Regs.TBPRD = EPWM_PRD;                      // 设置三角载波周期
    EPwm4Regs.TBPHS.bit.TBPHS = 0x0000;             // Phase is 0
    EPwm4Regs.TBCTR = 0x0000;                       // Clear counter
    EPwm4Regs.TBCTL.bit.CTRMODE = 0x2;              // 递增递减双向模式
    EPwm4Regs.TBCTL.bit.PHSEN = 0x0;                // Disable phase loading
    EPwm4Regs.TBCTL.bit.HSPCLKDIV = 0x0;            // Clock ratio to SYSCLKOUT
    EPwm4Regs.TBCTL.bit.CLKDIV = 0x0;               // Slow just to observe on the scope

    // 计数比较模块CMPA初始值（可删）
    EPwm4Regs.CMPA.bit.CMPA = 0;

    // 影子寄存器设置
    EPwm4Regs.CMPCTL.bit.SHDWAMODE = 0;
    EPwm4Regs.CMPCTL.bit.LOADAMODE = 0;             // 计数器为0时，载入比较值

    // 动作模块，高电平有效
    EPwm4Regs.AQCTLA.bit.CAU = 2;            // Set PWMxA on 1
    EPwm4Regs.AQCTLA.bit.CAD = 1;            // Set PWMxA on 0

    // 死区产生模块
    EPwm4Regs.DBCTL.bit.OUT_MODE = 0x3; //死区输出模式，RED和FED都使能
    EPwm4Regs.DBCTL.bit.POLSEL = 0x2;   //死区设置在B路PWM上
    EPwm4Regs.DBCTL.bit.IN_MODE = 0x0;  //死区输入模式控制
    EPwm4Regs.DBRED.bit.DBRED = DB_TIME;    //上升沿死区
    EPwm4Regs.DBFED.bit.DBFED = DB_TIME;    //下降沿死区
}

/*
 * board2 B相驱动-EPWM5
 */
void InitEPwm5(void)
{
    InitEPwm5Gpio();                                //初始化PWMx的GPIO

    //时基模块工作模式
    EPwm6Regs.TBPRD = EPWM_PRD;                      // 设置三角载波周期
    EPwm6Regs.TBPHS.bit.TBPHS = 0x0000;             // Phase is 0
    EPwm6Regs.TBCTR = 0x0000;                       // Clear counter
    EPwm6Regs.TBCTL.bit.CTRMODE = 0x2;              // 递增递减双向模式
    EPwm6Regs.TBCTL.bit.PHSEN = 0x0;                // Disable phase loading
    EPwm6Regs.TBCTL.bit.HSPCLKDIV = 0x0;            // Clock ratio to SYSCLKOUT
    EPwm6Regs.TBCTL.bit.CLKDIV = 0x0;               // Slow just to observe on the scope

    // 计数比较模块CMPA初始值（可删）
    EPwm6Regs.CMPA.bit.CMPA = 0;

    // 影子寄存器设置
    EPwm6Regs.CMPCTL.bit.SHDWAMODE = 0;
    EPwm6Regs.CMPCTL.bit.LOADAMODE = 0;             // 计数器为0时，载入比较值

    // 动作模块，高电平有效
    EPwm6Regs.AQCTLA.bit.CAU = 2;            // Set PWMxA on 1
    EPwm6Regs.AQCTLA.bit.CAD = 1;            // Set PWMxA on 0

    // 死区产生模块
    EPwm6Regs.DBCTL.bit.OUT_MODE = 0x3; //死区输出模式，RED和FED都使能
    EPwm6Regs.DBCTL.bit.POLSEL = 0x2;   //死区设置在B路PWM上
    EPwm6Regs.DBCTL.bit.IN_MODE = 0x0;  //死区输入模式控制
    EPwm6Regs.DBRED.bit.DBRED = DB_TIME;    //上升沿死区
    EPwm6Regs.DBFED.bit.DBFED = DB_TIME;    //下降沿死区
}

/*
 * board2 C相驱动-EPWM6
 */
void InitEPwm6(void)
{
    InitEPwm6Gpio();                                //初始化PWMx的GPIO

    //时基模块工作模式
    EPwm7Regs.TBPRD = EPWM_PRD;                      // 设置三角载波周期
    EPwm7Regs.TBPHS.bit.TBPHS = 0x0000;             // Phase is 0
    EPwm7Regs.TBCTR = 0x0000;                       // Clear counter
    EPwm7Regs.TBCTL.bit.CTRMODE = 0x2;              // 递增递减双向模式
    EPwm7Regs.TBCTL.bit.PHSEN = 0x0;                // Disable phase loading
    EPwm7Regs.TBCTL.bit.HSPCLKDIV = 0x0;            // Clock ratio to SYSCLKOUT
    EPwm7Regs.TBCTL.bit.CLKDIV = 0x0;               // Slow just to observe on the scope

    // 计数比较模块CMPA初始值（可删）
    EPwm7Regs.CMPA.bit.CMPA = 0;

    // 影子寄存器设置
    EPwm7Regs.CMPCTL.bit.SHDWAMODE = 0;
    EPwm7Regs.CMPCTL.bit.LOADAMODE = 0;             // 计数器为0时，载入比较值

    // 动作模块，高电平有效
    EPwm7Regs.AQCTLA.bit.CAU = 2;            // Set PWMxA on 1
    EPwm7Regs.AQCTLA.bit.CAD = 1;            // Set PWMxA on 0

    // 死区产生模块
    EPwm7Regs.DBCTL.bit.OUT_MODE = 0x3; //死区输出模式，RED和FED都使能
    EPwm7Regs.DBCTL.bit.POLSEL = 0x2;   //死区设置在B路PWM上
    EPwm7Regs.DBCTL.bit.IN_MODE = 0x0;  //死区输入模式控制
    EPwm7Regs.DBRED.bit.DBRED = DB_TIME;    //上升沿死区
    EPwm7Regs.DBFED.bit.DBFED = DB_TIME;    //下降沿死区
}

/*
 * board3 A相驱动-EPWM7
 */
void InitEPwm7(void)
{
    InitEPwm7Gpio();                                //初始化PWMx的GPIO

    //时基模块工作模式
    EPwm8Regs.TBPRD = EPWM_PRD;                      // 设置三角载波周期
    EPwm8Regs.TBPHS.bit.TBPHS = 0x0000;             // Phase is 0
    EPwm8Regs.TBCTR = 0x0000;                       // Clear counter
    EPwm8Regs.TBCTL.bit.CTRMODE = 0x2;              // 递增递减双向模式
    EPwm8Regs.TBCTL.bit.PHSEN = 0x0;                // Disable phase loading
    EPwm8Regs.TBCTL.bit.HSPCLKDIV = 0x0;            // Clock ratio to SYSCLKOUT
    EPwm8Regs.TBCTL.bit.CLKDIV = 0x0;               // Slow just to observe on the scope

    // 计数比较模块CMPA初始值（可删）
    EPwm8Regs.CMPA.bit.CMPA = 0;

    // 影子寄存器设置
    EPwm8Regs.CMPCTL.bit.SHDWAMODE = 0;
    EPwm8Regs.CMPCTL.bit.LOADAMODE = 0;             // 计数器为0时，载入比较值

    // 动作模块，高电平有效
    EPwm8Regs.AQCTLA.bit.CAU = 2;            // Set PWMxA on 1
    EPwm8Regs.AQCTLA.bit.CAD = 1;            // Set PWMxA on 0

    // 死区产生模块
    EPwm8Regs.DBCTL.bit.OUT_MODE = 0x3; //死区输出模式，RED和FED都使能
    EPwm8Regs.DBCTL.bit.POLSEL = 0x2;   //死区设置在B路PWM上
    EPwm8Regs.DBCTL.bit.IN_MODE = 0x0;  //死区输入模式控制
    EPwm8Regs.DBRED.bit.DBRED = DB_TIME;    //上升沿死区
    EPwm8Regs.DBFED.bit.DBFED = DB_TIME;    //下降沿死区
}

/*
 * board3 B相驱动-EPWM8
 */
void InitEPwm8(void)
{
    InitEPwm8Gpio();                                //初始化PWMx的GPIO

    //时基模块工作模式
    EPwm9Regs.TBPRD = EPWM_PRD;                      // 设置三角载波周期
    EPwm9Regs.TBPHS.bit.TBPHS = 0x0000;             // Phase is 0
    EPwm9Regs.TBCTR = 0x0000;                       // Clear counter
    EPwm9Regs.TBCTL.bit.CTRMODE = 0x2;              // 递增递减双向模式
    EPwm9Regs.TBCTL.bit.PHSEN = 0x0;                // Disable phase loading
    EPwm9Regs.TBCTL.bit.HSPCLKDIV = 0x0;            // Clock ratio to SYSCLKOUT
    EPwm9Regs.TBCTL.bit.CLKDIV = 0x0;               // Slow just to observe on the scope

    // 计数比较模块CMPA初始值（可删）
    EPwm9Regs.CMPA.bit.CMPA = 0;

    // 影子寄存器设置
    EPwm9Regs.CMPCTL.bit.SHDWAMODE = 0;
    EPwm9Regs.CMPCTL.bit.LOADAMODE = 0;             // 计数器为0时，载入比较值

    // 动作模块，高电平有效
    EPwm9Regs.AQCTLA.bit.CAU = 2;            // Set PWMxA on 1
    EPwm9Regs.AQCTLA.bit.CAD = 1;            // Set PWMxA on 0

    // 死区产生模块
    EPwm9Regs.DBCTL.bit.OUT_MODE = 0x3; //死区输出模式，RED和FED都使能
    EPwm9Regs.DBCTL.bit.POLSEL = 0x2;   //死区设置在B路PWM上
    EPwm9Regs.DBCTL.bit.IN_MODE = 0x0;  //死区输入模式控制
    EPwm9Regs.DBRED.bit.DBRED = DB_TIME;    //上升沿死区
    EPwm9Regs.DBFED.bit.DBFED = DB_TIME;    //下降沿死区
}

/*
 * board3 C相驱动-EPWM9
 */
void InitEPwm9(void)
{
    InitEPwm9Gpio();                                //初始化PWMx的GPIO

    //时基模块工作模式
    EPwm10Regs.TBPRD = EPWM_PRD;                      // 设置三角载波周期
    EPwm10Regs.TBPHS.bit.TBPHS = 0x0000;             // Phase is 0
    EPwm10Regs.TBCTR = 0x0000;                       // Clear counter
    EPwm10Regs.TBCTL.bit.CTRMODE = 0x2;              // 递增递减双向模式
    EPwm10Regs.TBCTL.bit.PHSEN = 0x0;                // Disable phase loading
    EPwm10Regs.TBCTL.bit.HSPCLKDIV = 0x0;            // Clock ratio to SYSCLKOUT
    EPwm10Regs.TBCTL.bit.CLKDIV = 0x0;               // Slow just to observe on the scope

    // 计数比较模块CMPA初始值（可删）
    EPwm10Regs.CMPA.bit.CMPA = 0;

    // 影子寄存器设置
    EPwm10Regs.CMPCTL.bit.SHDWAMODE = 0;
    EPwm10Regs.CMPCTL.bit.LOADAMODE = 0;             // 计数器为0时，载入比较值

    // 动作模块，高电平有效
    EPwm10Regs.AQCTLA.bit.CAU = 2;            // Set PWMxA on 1
    EPwm10Regs.AQCTLA.bit.CAD = 1;            // Set PWMxA on 0

    // 死区产生模块
    EPwm10Regs.DBCTL.bit.OUT_MODE = 0x3; //死区输出模式，RED和FED都使能
    EPwm10Regs.DBCTL.bit.POLSEL = 0x2;   //死区设置在B路PWM上
    EPwm10Regs.DBCTL.bit.IN_MODE = 0x0;  //死区输入模式控制
    EPwm10Regs.DBRED.bit.DBRED = DB_TIME;    //上升沿死区
    EPwm10Regs.DBFED.bit.DBFED = DB_TIME;    //下降沿死区
}

/*
 * board4 A相驱动-EPWM10
 */
void InitEPwm10(void)
{
    InitEPwm10Gpio();                                //初始化PWMx的GPIO

    //时基模块工作模式
    EPwm11Regs.TBPRD = EPWM_PRD;                      // 设置三角载波周期
    EPwm11Regs.TBPHS.bit.TBPHS = 0x0000;             // Phase is 0
    EPwm11Regs.TBCTR = 0x0000;                       // Clear counter
    EPwm11Regs.TBCTL.bit.CTRMODE = 0x2;              // 递增递减双向模式
    EPwm11Regs.TBCTL.bit.PHSEN = 0x0;                // Disable phase loading
    EPwm11Regs.TBCTL.bit.HSPCLKDIV = 0x0;            // Clock ratio to SYSCLKOUT
    EPwm11Regs.TBCTL.bit.CLKDIV = 0x0;               // Slow just to observe on the scope

    // 计数比较模块CMPA初始值（可删）
    EPwm11Regs.CMPA.bit.CMPA = 0;

    // 影子寄存器设置
    EPwm11Regs.CMPCTL.bit.SHDWAMODE = 0;
    EPwm11Regs.CMPCTL.bit.LOADAMODE = 0;             // 计数器为0时，载入比较值

    // 动作模块，高电平有效
    EPwm11Regs.AQCTLA.bit.CAU = 2;            // Set PWMxA on 1
    EPwm11Regs.AQCTLA.bit.CAD = 1;            // Set PWMxA on 0

    // 死区产生模块
    EPwm11Regs.DBCTL.bit.OUT_MODE = 0x3; //死区输出模式，RED和FED都使能
    EPwm11Regs.DBCTL.bit.POLSEL = 0x2;   //死区设置在B路PWM上
    EPwm11Regs.DBCTL.bit.IN_MODE = 0x0;  //死区输入模式控制
    EPwm11Regs.DBRED.bit.DBRED = DB_TIME;    //上升沿死区
    EPwm11Regs.DBFED.bit.DBFED = DB_TIME;    //下降沿死区
}

/*
 * board4 B相驱动-EPWM11
 */
void InitEPwm11(void)
{
    InitEPwm11Gpio();                                //初始化PWMx的GPIO

    //时基模块工作模式
    EPwm5Regs.TBPRD = EPWM_PRD;                      // 设置三角载波周期
    EPwm5Regs.TBPHS.bit.TBPHS = 0x0000;             // Phase is 0
    EPwm5Regs.TBCTR = 0x0000;                       // Clear counter
    EPwm5Regs.TBCTL.bit.CTRMODE = 0x2;              // 递增递减双向模式
    EPwm5Regs.TBCTL.bit.PHSEN = 0x0;                // Disable phase loading
    EPwm5Regs.TBCTL.bit.HSPCLKDIV = 0x0;            // Clock ratio to SYSCLKOUT
    EPwm5Regs.TBCTL.bit.CLKDIV = 0x0;               // Slow just to observe on the scope

    // 计数比较模块CMPA初始值（可删）
    EPwm5Regs.CMPA.bit.CMPA = 0;

    // 影子寄存器设置
    EPwm5Regs.CMPCTL.bit.SHDWAMODE = 0;
    EPwm5Regs.CMPCTL.bit.LOADAMODE = 0;             // 计数器为0时，载入比较值

    // 动作模块，高电平有效
    EPwm5Regs.AQCTLA.bit.CAU = 2;            // Set PWMxA on 1
    EPwm5Regs.AQCTLA.bit.CAD = 1;            // Set PWMxA on 0

    // 死区产生模块
    EPwm5Regs.DBCTL.bit.OUT_MODE = 0x3; //死区输出模式，RED和FED都使能
    EPwm5Regs.DBCTL.bit.POLSEL = 0x2;   //死区设置在B路PWM上
    EPwm5Regs.DBCTL.bit.IN_MODE = 0x0;  //死区输入模式控制
    EPwm5Regs.DBRED.bit.DBRED = DB_TIME;    //上升沿死区
    EPwm5Regs.DBFED.bit.DBFED = DB_TIME;    //下降沿死区
}

/*
 * board4 C相驱动-EPWM12
 */
void InitEPwm12(void)
{
    InitEPwm12Gpio();                                //初始化PWMx的GPIO

    //时基模块工作模式
    EPwm12Regs.TBPRD = EPWM_PRD;                      // 设置三角载波周期
    EPwm12Regs.TBPHS.bit.TBPHS = 0x0000;             // Phase is 0
    EPwm12Regs.TBCTR = 0x0000;                       // Clear counter
    EPwm12Regs.TBCTL.bit.CTRMODE = 0x2;              // 递增递减双向模式
    EPwm12Regs.TBCTL.bit.PHSEN = 0x0;                // Disable phase loading
    EPwm12Regs.TBCTL.bit.HSPCLKDIV = 0x0;            // Clock ratio to SYSCLKOUT
    EPwm12Regs.TBCTL.bit.CLKDIV = 0x0;               // Slow just to observe on the scope

    // 计数比较模块CMPA初始值（可删）
    EPwm12Regs.CMPA.bit.CMPA = 0;

    // 影子寄存器设置
    EPwm12Regs.CMPCTL.bit.SHDWAMODE = 0;
    EPwm12Regs.CMPCTL.bit.LOADAMODE = 0;             // 计数器为0时，载入比较值

    // 动作模块，高电平有效
    EPwm12Regs.AQCTLA.bit.CAU = 2;            // Set PWMxA on 1
    EPwm12Regs.AQCTLA.bit.CAD = 1;            // Set PWMxA on 0

    // 死区产生模块
    EPwm12Regs.DBCTL.bit.OUT_MODE = 0x3; //死区输出模式，RED和FED都使能
    EPwm12Regs.DBCTL.bit.POLSEL = 0x2;   //死区设置在B路PWM上
    EPwm12Regs.DBCTL.bit.IN_MODE = 0x0;  //死区输入模式控制
    EPwm12Regs.DBRED.bit.DBRED = DB_TIME;    //上升沿死区
    EPwm12Regs.DBFED.bit.DBFED = DB_TIME;    //下降沿死区
}

void InitEPwms(void)
{
#if (PWM_NUM == 3 || PWM_NUM == 6 || PWM_NUM == 10|| PWM_NUM == 12 )
    InitEPwm1();
    InitEPwm2();
    InitEPwm3();
#endif
#if (PWM_NUM == 6 || PWM_NUM == 10 || PWM_NUM == 12 )
    InitEPwm4();
    InitEPwm5();
    InitEPwm6();
#endif
#if (PWM_NUM == 10 || PWM_NUM == 12 )
    InitEPwm7();
    InitEPwm8();
    InitEPwm9();
    InitEPwm10();
    InitEPwm11();
#endif
#if(PWM_NUM == 12 )
    InitEPwm11();
    InitEPwm12();
#endif
}

void SetCMP(float duty[])
{
    /************************************************************
    Description:更新EPWM模块的comparer值
    Input:duty[]
    Output:none
    ************************************************************/
#if (PWM_NUM == 3 || PWM_NUM == 6 || PWM_NUM == 10 || PWM_NUM == 12)
// * board1
    EPwm1Regs.CMPA.bit.CMPA = EPWM_PRD * (1 - duty[0]); //A phase  epwm1
    EPwm2Regs.CMPA.bit.CMPA = EPWM_PRD * (1 - duty[1]); //B phase  epwm2
    EPwm3Regs.CMPA.bit.CMPA = EPWM_PRD * (1 - duty[2]); //C phase  epwm3
#endif
#if (PWM_NUM == 6 || PWM_NUM == 10 || PWM_NUM == 12)
//* board2
    EPwm4Regs.CMPA.bit.CMPA = EPWM_PRD * (1 - duty[3]); //A phase   epwm4
    EPwm6Regs.CMPA.bit.CMPA = EPWM_PRD * (1 - duty[4]); //B phase   epwm5
//    EPwm7Regs.CMPA.bit.CMPA = EPWM_PRD * (1 - duty[5]); //C phase   epwm6
#endif
#if (PWM_NUM == 12 || PWM_NUM == 10)
//* board3 &* board4
    EPwm8Regs.CMPA.bit.CMPA = EPWM_PRD * (1 - duty[5]); //A phase   epwm7
    EPwm9Regs.CMPA.bit.CMPA = EPWM_PRD * (1 - duty[6]); //B phase   epwm8
    EPwm10Regs.CMPA.bit.CMPA = EPWM_PRD * (1 - duty[7]); //C phase  epwm9
    EPwm11Regs.CMPA.bit.CMPA = EPWM_PRD * (1 - duty[8]); //A phase   epwm10
    EPwm5Regs.CMPA.bit.CMPA = EPWM_PRD * (1 - duty[9]); //B phase   epwm11
#endif
#if (PWM_NUM == 12)
//* board4
    EPwm5Regs.CMPA.bit.CMPA = EPWM_PRD * (1 - duty[10]); //B phase   epwm11
    EPwm12Regs.CMPA.bit.CMPA = EPWM_PRD * (1 - duty[11]); //C phase  epwm12
#endif
}
