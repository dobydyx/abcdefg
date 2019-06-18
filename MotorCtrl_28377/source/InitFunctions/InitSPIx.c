/*
 * InitSPIx.c
 *
 *  Created on: 2018年1月20日
 *      Author: Ezreal
 */

/************************************************************
Description:初始化SPIb,包括对应176脚封装的GPIO口初始化
************************************************************/

#include "F28x_Project.h"

void InitSpib(void)
{
    InitSpibGpio();
    // Initialize SPI-b

    // Set reset low before configuration changes
    SpibRegs.SPICCR.bit.SPISWRESET = 0;

    SpibRegs.SPICTL.bit.MASTER_SLAVE = 1;           //Master mode

    SpibRegs.SPICCR.bit.CLKPOLARITY = 0;
    SpibRegs.SPICTL.bit.CLK_PHASE = 0;

    // Set the baud rate
    SpibRegs.SPIBRR.bit.SPI_BIT_RATE = 49;          //SPI CLK freq = 1 MHz

    SpibRegs.SPICCR.bit.SPICHAR = 7;               //Word length:(7+1)bit

    SpibRegs.SPICTL.bit.SPIINTENA = 0;

    SpibRegs.SPICTL.bit.TALK = 1;

    //Clear Flags
    SpibRegs.SPISTS.all = 0x0000;

    // Release the SPI from reset
    SpibRegs.SPICCR.bit.SPISWRESET = 1;
}

void SpibOut(int a)
{
    SpibRegs.SPITXBUF = a;
    while(SpibRegs.SPISTS.bit.BUFFULL_FLAG == 1)
    {
    }
}
