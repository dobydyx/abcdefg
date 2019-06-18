/*
 * InitSCIx.c
 *
 *  Created on: 2018年1月29日
 *      Author: Ezreal
 */

/************************************************************
Description:初始化SCIb,SCIc,包括对应176脚封装的GPIO口初始化
************************************************************/

#include "F28x_Project.h"
#include "CtrlParams.h"

void InitScib(void)
{
    InitScibGpio();
    // Initialize SCI-b

    // 1 stop bit, No loopback, No parity,8 char bits, async mode, idle-line protocol
    ScibRegs.SCICCR.all = 0x0007;

    // enable TX, RX, internal SCICLK, Disable RX ERR, SLEEP, TXWAKE
    ScibRegs.SCICTL1.all = 0x0003;

//    ScibRegs.SCICTL2.bit.TXINTENA = 1;
    ScibRegs.SCICTL2.bit.RXBKINTENA = 1;
    ScibRegs.SCIHBAUD.all = 0x0000;
    ScibRegs.SCILBAUD.all = 0x001A;

    // Relinquish SCI from Reset
    ScibRegs.SCICTL1.bit.SWRESET = 1;
}

void InitScic(void)
{
    InitScicGpio();
    // Initialize SCI-c

    // 1 stop bit, No loopback, No parity,8 char bits, async mode, idle-line protocol
    ScicRegs.SCICCR.all = 0x0007;

    // enable TX, RX, internal SCICLK, Disable RX ERR, SLEEP, TXWAKE
    ScicRegs.SCICTL1.all = 0x0003;

//    ScicRegs.SCICTL2.bit.TXINTENA = 1;
    ScicRegs.SCICTL2.bit.RXBKINTENA = 1;
    ScicRegs.SCIHBAUD.all = 0x0000;
    ScicRegs.SCILBAUD.all = 0x0035;

    // Relinquish SCI from Reset
    ScicRegs.SCICTL1.bit.SWRESET = 1;
}

void InitScis(void)
{
    InitScib();
    InitScic();
}

void ScibOut(int a)
{
    ScibRegs.SCITXBUF.all = a;
    while(ScibRegs.SCICTL2.bit.TXRDY == 0)
    {
    }
}

void ScicOut(int a)
{
    ScicRegs.SCITXBUF.all = a;
    while(ScicRegs.SCICTL2.bit.TXRDY == 0)
    {
    }
}

void ScibSendFrame(int var, ValuePart *val, int size)
{
    int data[4] = {0}, i = 0;

    for(i = 0; i <= 3; i++)
    {
        if(i <= size-1)
        {
            data[i] = val->long32 >> 8 * i;
        }
        else
        {
            data[i] = 0;
        }
    }

    ScibOut(0XAA);
    ScibOut(var);
    ScibOut(data[3]);
    ScibOut(data[2]);
    ScibOut(data[1]);
    ScibOut(data[0]);
    ScibOut(0X55);
}
