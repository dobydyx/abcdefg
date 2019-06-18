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

void InitEPwm3(void)
{
    //A相驱动-EPWM3
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
    EPwm3Regs.AQCTLA.bit.CAU = 1;            // Set PWMxA on 1
    EPwm3Regs.AQCTLA.bit.CAD = 2;            // Set PWMxA on 0

    // 死区产生模块
    EPwm3Regs.DBCTL.bit.OUT_MODE = 0x3; //死区输出模式，RED和FED都使能
    EPwm3Regs.DBCTL.bit.POLSEL = 0x2;   //死区设置在B路PWM上
    EPwm3Regs.DBCTL.bit.IN_MODE = 0x0;  //死区输入模式控制
    EPwm3Regs.DBRED.bit.DBRED = DB_TIME;    //上升沿死区
    EPwm3Regs.DBFED.bit.DBFED = DB_TIME;    //下降沿死区

    // PWM中断设置
    EPwm3Regs.ETSEL.bit.INTSEL = 0x1;   //设置EPWM产生中断的条件，当前设置为TB等于0时产生中断
    EPwm3Regs.ETPS.bit.INTPRD = 0x1;    //Generate INT on 1st event
//    EPwm3Regs.ETSEL.bit.INTEN = 1;      //打开EPWM中断使能
}

void InitEPwm4(void)
{
    //B相驱动-EPWM4
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
    EPwm4Regs.AQCTLA.bit.CAU = 1;            // Set PWMxA on 1
    EPwm4Regs.AQCTLA.bit.CAD = 2;            // Set PWMxA on 0

    // 死区产生模块
    EPwm4Regs.DBCTL.bit.OUT_MODE = 0x3; //死区输出模式，RED和FED都使能
    EPwm4Regs.DBCTL.bit.POLSEL = 0x2;   //死区设置在B路PWM上
    EPwm4Regs.DBCTL.bit.IN_MODE = 0x0;  //死区输入模式控制
    EPwm4Regs.DBRED.bit.DBRED = DB_TIME;    //上升沿死区
    EPwm4Regs.DBFED.bit.DBFED = DB_TIME;    //下降沿死区
}

void InitEPwm6(void)
{
    //C相驱动-EPWM6
    InitEPwm6Gpio();                                //初始化PWMx的GPIO

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
    EPwm6Regs.AQCTLA.bit.CAU = 1;            // Set PWMxA on 1
    EPwm6Regs.AQCTLA.bit.CAD = 2;            // Set PWMxA on 0

    // 死区产生模块
    EPwm6Regs.DBCTL.bit.OUT_MODE = 0x3; //死区输出模式，RED和FED都使能
    EPwm6Regs.DBCTL.bit.POLSEL = 0x2;   //死区设置在B路PWM上
    EPwm6Regs.DBCTL.bit.IN_MODE = 0x0;  //死区输入模式控制
    EPwm6Regs.DBRED.bit.DBRED = DB_TIME;    //上升沿死区
    EPwm6Regs.DBFED.bit.DBFED = DB_TIME;    //下降沿死区
}

#if PWM_NUM == 5 | PWM_NUM == 6
void InitEPwm7(void)
{
    //D相驱动-EPWM7
    InitEPwm7Gpio();                                //初始化PWMx的GPIO

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
    EPwm7Regs.AQCTLA.bit.CAU = 1;            // Set PWMxA on 1
    EPwm7Regs.AQCTLA.bit.CAD = 2;            // Set PWMxA on 0

    // 死区产生模块
    EPwm7Regs.DBCTL.bit.OUT_MODE = 0x3; //死区输出模式，RED和FED都使能
    EPwm7Regs.DBCTL.bit.POLSEL = 0x2;   //死区设置在B路PWM上
    EPwm7Regs.DBCTL.bit.IN_MODE = 0x0;  //死区输入模式控制
    EPwm7Regs.DBRED.bit.DBRED = DB_TIME;    //上升沿死区
    EPwm7Regs.DBFED.bit.DBFED = DB_TIME;    //下降沿死区
}

void InitEPwm8(void)
{
    //E相驱动-EPWM8
    InitEPwm8Gpio();                                //初始化PWMx的GPIO

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
    EPwm8Regs.AQCTLA.bit.CAU = 1;            // Set PWMxA on 1
    EPwm8Regs.AQCTLA.bit.CAD = 2;            // Set PWMxA on 0

    // 死区产生模块
    EPwm8Regs.DBCTL.bit.OUT_MODE = 0x3; //死区输出模式，RED和FED都使能
    EPwm8Regs.DBCTL.bit.POLSEL = 0x2;   //死区设置在B路PWM上
    EPwm8Regs.DBCTL.bit.IN_MODE = 0x0;  //死区输入模式控制
    EPwm8Regs.DBRED.bit.DBRED = DB_TIME;    //上升沿死区
    EPwm8Regs.DBFED.bit.DBFED = DB_TIME;    //下降沿死区
}
#endif

#if PWM_NUM == 6
void InitEPwm9(void)
{
    InitEPwm9Gpio();                                //初始化PWMx的GPIO

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
    EPwm9Regs.AQCTLA.bit.CAU = 1;            // Set PWMxA on 1
    EPwm9Regs.AQCTLA.bit.CAD = 2;            // Set PWMxA on 0

    // 死区产生模块
    EPwm9Regs.DBCTL.bit.OUT_MODE = 0x3; //死区输出模式，RED和FED都使能
    EPwm9Regs.DBCTL.bit.POLSEL = 0x2;   //死区设置在B路PWM上
    EPwm9Regs.DBCTL.bit.IN_MODE = 0x0;  //死区输入模式控制
    EPwm9Regs.DBRED.bit.DBRED = DB_TIME;    //上升沿死区
    EPwm9Regs.DBFED.bit.DBFED = DB_TIME;    //下降沿死区
}
#endif

void InitEPwms(void)
{
    InitEPwm3();
    InitEPwm4();
    InitEPwm6();

#if PWM_NUM == 5 | PWM_NUM == 6
    InitEPwm7();
    InitEPwm8();
#endif

#if PWM_NUM == 6
    InitEPwm9();
#endif
}

void SetCMP(float duty[])
{
    /************************************************************
    Description:更新EPWM模块的comparer值
    Input:duty[]
    Output:none
    ************************************************************/
    EPwm3Regs.CMPA.bit.CMPA = EPWM_PRD * duty[0];
    EPwm4Regs.CMPA.bit.CMPA = EPWM_PRD * duty[1];
    EPwm6Regs.CMPA.bit.CMPA = EPWM_PRD * duty[2];

#if PWM_NUM == 5 | PWM_NUM == 6
    EPwm7Regs.CMPA.bit.CMPA = EPWM_PRD * duty[3];
    EPwm8Regs.CMPA.bit.CMPA = EPWM_PRD * duty[4];
#endif

#if PWM_NUM == 6
    EPwm9Regs.CMPA.bit.CMPA = EPWM_PRD * duty[5];
#endif
}
