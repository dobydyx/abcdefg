/*
 * InitEQEPx.c
 *
 *  Created on: 2018年1月7日
 *      Author: Ezreal
 */

/************************************************************
Description:初始化eQEP1,包括对应176脚封装的GPIO口初始化
************************************************************/

#include "F28x_Project.h"
#include "CtrlParams.h"
///*
void InitEQep1(void)
{
    InitEQep1Gpio();

//    EQep1Regs.QUPRD=2000000;            // Unit Timer for 100Hz at 200 MHz SYSCLKOUT
    EQep1Regs.QDECCTL.bit.QSRC = 0;      // QEP quadrature count mode
//    EQep1Regs.QEPCTL.bit.FREE_SOFT = 2;
    EQep1Regs.QEPCTL.bit.PCRM = 1;       // PCRM=01 mode - QPOSCNT reset on maximum position
//    EQep1Regs.QEPCTL.bit.UTE = 1;         // Unit Timeout Enable
//    EQep1Regs.QEPCTL.bit.QCLM = 1;        // Latch on unit time out
    EQep1Regs.QPOSMAX = CODER_NUM -1;               //maximum position
    EQep1Regs.QEPCTL.bit.QPEN = 1;        // QEP enable
    EQep1Regs.QCAPCTL.bit.UPPS = 5;       // 1/32 for unit position
    EQep1Regs.QCAPCTL.bit.CCPS = 6;       // 1/64 for CAP clock
    EQep1Regs.QCAPCTL.bit.CEN = 1;        // QEP Capture Enable

    EQep1Regs.QEPCTL.bit.IEL = 1;
    EQep1Regs.QEINT.bit.IEL = 1;            //QEP Index Interrupt Enable
//    EQep1Regs.QEINT.bit.SEL = 1;
//    EQep1Regs.QEPCTL.bit.IEL = 2;

    EQep1Regs.QDECCTL.bit.QAP = 1;
    EQep1Regs.QDECCTL.bit.QBP = 1;
    EQep1Regs.QDECCTL.bit.QIP = 1;
    EQep1Regs.QDECCTL.bit.QSP = 1;          //编码器极性
}
//*/

