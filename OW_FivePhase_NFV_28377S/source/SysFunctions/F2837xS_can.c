//###########################################################################
//
// FILE:   F2837xS_can.c
//
// TITLE:  F2837xS CAN Support Functions.
//
//###########################################################################
// $TI Release: F2837xS Support Library v210 $
// $Release Date: Tue Nov  1 15:35:23 CDT 2016 $
// $Copyright: Copyright (C) 2014-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

//
// Included Files
//
#include "F2837xS_device.h"
#include "F2837xS_Examples.h"

//
// InitCANGpio - Initialize all CAN modules' GPIOs
//
void InitCANGpio(void)
{
    InitCanaGpio();
    InitCanbGpio();
}

void InitCanaGpio(void)
{
    EALLOW;
    //
    // Enable internal pull-up for the selected pins
    //
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPBPUD.bit.GPIO36 = 0;  // Enable pull-up on GPIO36 (CAN-A RX)
    GpioCtrlRegs.GPBPUD.bit.GPIO37 = 0;  // Enable pull-up on GPIO37 (CAN-A TX)
    //
    // Set qualification for selected pins to asynch only
    //
    // This will select asynch (no qualification) for the selected pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPBQSEL1.bit.GPIO36 = 3; // Asynch input GPIO36 (CAN-A RX)
    GpioCtrlRegs.GPBQSEL1.bit.GPIO37 = 3; // Asynch input GPIO37 (CAN-A TX)
    //
    //Configure CAN-A pins using GPIO regs
    //
    // This specifies which of the possible GPIO pins will be CAN functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPBGMUX1.bit.GPIO36 = 1; // Configure GPIO36 as CAN-A RX
    GpioCtrlRegs.GPBGMUX1.bit.GPIO37 = 1; // Configure GPIO37 as CAN-A TX

    GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 2; // Configure GPIO36 as CAN-A RX
    GpioCtrlRegs.GPBMUX1.bit.GPIO37 = 2; // Configure GPIO37 as CAN-A TX

    EDIS;
}

void InitCanbGpio(void)
{
    EALLOW;
    //
    // Enable internal pull-up for the selected pins
    //
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPBPUD.bit.GPIO38 = 0;  // Enable pull-up on GPIO38 (CAN-B TX)
    GpioCtrlRegs.GPBPUD.bit.GPIO39 = 0;  // Enable pull-up on GPIO39 (CAN-B RX)
    //
    // Set qualification for selected pins to asynch only
    //
    // This will select asynch (no qualification) for the selected pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPBQSEL1.bit.GPIO38 = 3; // Asynch input GPIO38 (CAN-B TX)
    GpioCtrlRegs.GPBQSEL1.bit.GPIO39 = 3; // Asynch input GPIO39 (CAN-B RX)
    //
    //Configure CAN-B pins using GPIO regs
    //
    // This specifies which of the possible GPIO pins will be CAN functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPBGMUX1.bit.GPIO38 = 1; // Configure GPIO38 as CAN-B TX
    GpioCtrlRegs.GPBGMUX1.bit.GPIO39 = 1; // Configure GPIO39 as CAN-B RX

    GpioCtrlRegs.GPBMUX1.bit.GPIO38 = 2; // Configure GPIO38 as CAN-B TX
    GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 2; // Configure GPIO39 as CAN-B RX

    EDIS;
}

//
// InitCAN - Initializes the CAN-A controller after reset.
//
void InitCAN(void)
{
    int16_t iMsg;

    //
    // Place CAN controller in init state, regardless of previous state.  This
    // will put controller in idle, and allow the message object RAM to be
    // programmed.
    //
    CanaRegs.CAN_CTL.bit.Init = 1;
    CanaRegs.CAN_CTL.bit.SWR = 1;

    //
    // Wait for busy bit to clear
    //
    while(CanaRegs.CAN_IF1CMD.bit.Busy)
    {
    }

    //
    // Clear the message value bit in the arbitration register.  This indicates
    // the message is not valid and is a "safe" condition to leave the message
    // object.  The same arb reg is used to program all the message objects.
    //
    CanaRegs.CAN_IF1CMD.bit.DIR = 1;
    CanaRegs.CAN_IF1CMD.bit.Arb = 1;
    CanaRegs.CAN_IF1CMD.bit.Control = 1;

    CanaRegs.CAN_IF1ARB.all = 0;

    CanaRegs.CAN_IF1MCTL.all = 0;

    CanaRegs.CAN_IF2CMD.bit.DIR = 1;
    CanaRegs.CAN_IF2CMD.bit.Arb = 1;
    CanaRegs.CAN_IF2CMD.bit.Control = 1;

    CanaRegs.CAN_IF2ARB.all = 0;

    CanaRegs.CAN_IF2MCTL.all = 0;

    //
    // Loop through to program all 32 message objects
    //
    for(iMsg = 1; iMsg <= 32; iMsg+=2)
    {
        //
        // Wait for busy bit to clear
        //
        while(CanaRegs.CAN_IF1CMD.bit.Busy)
        {
        }

        //
        // Initiate programming the message object
        //
        CanaRegs.CAN_IF1CMD.bit.MSG_NUM = iMsg;

        //
        // Wait for busy bit to clear
        //
        while(CanaRegs.CAN_IF2CMD.bit.Busy)
        {
        }

        //
        // Initiate programming the message object
        //
        CanaRegs.CAN_IF2CMD.bit.MSG_NUM = iMsg + 1;
    }

    //
    // Acknowledge any pending status interrupts.
    //
    volatile uint32_t discardRead = CanaRegs.CAN_ES.all;

}

//
// End of file
//
