/*
 * InitDACx.c
 *
 *  Created on: 2018年1月20日
 *      Author: Ezreal
 */

/************************************************************
Description:初始化DAC(DACOUTA(A0), DACOUTB(A1), DACOUTC(B1))
************************************************************/

#include "F28x_Project.h"

void InitDACs(void)
{
    EALLOW;
    //DACOUTA
    DacaRegs.DACCTL.bit.DACREFSEL = 1;                          //VREFHI作为参考电压
    DacaRegs.DACOUTEN.bit.DACOUTEN = 1;                         //使能DACOUT
    DacaRegs.DACVALS.all = 0;                                   //初值为零
    DELAY_US(10);                                               // Delay for buffered DAC to power up

    //DACOUTB
    DacbRegs.DACCTL.bit.DACREFSEL = 1;                          //VREFHI作为参考电压
    DacbRegs.DACOUTEN.bit.DACOUTEN = 1;                         //使能DACOUT
    DacbRegs.DACVALS.all = 0;                                   //初值为零
    DELAY_US(10);                                               // Delay for buffered DAC to power up
}

void SetDACaValue(int val)
{
    //输入0-4095，输出0-3v
    DacaRegs.DACVALS.all = val;
    DELAY_US(2);
}

void SetDACbValue(int val)
{
    //输入0-4095，输出0-3v
    DacbRegs.DACVALS.all = val;
    DELAY_US(2);
}

