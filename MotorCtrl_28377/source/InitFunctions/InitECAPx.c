/*
 * InitECAPx.c
 *
 *  Created on: 2018年5月21日
 *      Author: Ezreal
 */

/************************************************************
Description:初始化eCAP1,2,3,4,5,6，包括对应176脚封装的GPIO口初始化
************************************************************/

#include "F28x_Project.h"

void InitECap1(int pin)
{
    // Initialize GPIO
    InitECap1Gpio(pin);
    GPIO_SetupPinOptions(pin, GPIO_INPUT, GPIO_ASYNC);

    ECap1Regs.ECEINT.all = 0x0000;          // Disable all capture __interrupts
    ECap1Regs.ECCLR.all = 0xFFFF;           // Clear all CAP __interrupt flags
    ECap1Regs.ECCTL1.bit.CAPLDEN = 0;       // Disable CAP1-CAP4 register loads
    ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0;     // Make sure the counter is stopped

    //
    // Configure peripheral registers
    //
    ECap1Regs.ECCTL2.bit.CONT_ONESHT = 0;   // One-shot
    ECap1Regs.ECCTL2.bit.STOP_WRAP = 3;     // Stop at 4 events
    ECap1Regs.ECCTL1.bit.CAP1POL = 1;       // Falling edge
    ECap1Regs.ECCTL1.bit.CAP2POL = 0;       // Rising edge
    ECap1Regs.ECCTL1.bit.CAP3POL = 1;       // Falling edge
    ECap1Regs.ECCTL1.bit.CAP4POL = 0;       // Rising edge
    ECap1Regs.ECCTL1.bit.CTRRST1 = 1;       // Difference operation
    ECap1Regs.ECCTL1.bit.CTRRST2 = 1;       // Difference operation
    ECap1Regs.ECCTL1.bit.CTRRST3 = 1;       // Difference operation
    ECap1Regs.ECCTL1.bit.CTRRST4 = 1;       // Difference operation
    ECap1Regs.ECCTL2.bit.SYNCI_EN = 0;      // Disable sync in
    ECap1Regs.ECCTL2.bit.SYNCO_SEL = 2;     // Disable sync out signal
    ECap1Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable capture units

    ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1;     // Start Counter
    ECap1Regs.ECCTL2.bit.REARM = 1;         // arm one-shot
    ECap1Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable CAP1-CAP4 register loads
//    ECap1Regs.ECEINT.bit.CEVT4 = 1;         // 4 events = __interrupt
}

