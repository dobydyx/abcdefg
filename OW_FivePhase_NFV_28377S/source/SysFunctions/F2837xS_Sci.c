//###########################################################################
//
// FILE:   F2837xS_Sci.c
//
// TITLE:  F2837xS SCI Initialization & Support Functions.
//
//###########################################################################
// $TI Release: F2837xS Support Library v210 $
// $Release Date: Tue Nov  1 15:35:23 CDT 2016 $
// $Copyright: Copyright (C) 2014-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "F2837xS_device.h"     // F2837xS Headerfile Include File
#include "F2837xS_Examples.h"   // F2837xS Examples Include File

void InitSciaGpio(void)
{
   EALLOW;

    //
    // Enable internal pull-up for the selected pins
    //
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPBPUD.bit.GPIO42 = 0;  // Enable pull-up on GPIO42 (SCITXDA)
    GpioCtrlRegs.GPBPUD.bit.GPIO43 = 0;  // Enable pull-up on GPIO43 (SCIRXDA)

    //
    // Set qualification for selected pins to asynch only
    //
    // This will select asynch (no qualification) for the selected pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPBQSEL1.bit.GPIO42 = 3; // Asynch input GPIO42 (SCITXDA)
    GpioCtrlRegs.GPBQSEL1.bit.GPIO43 = 3; // Asynch input GPIO43 (SCIRXDA)

    //
    //Configure SCI-a pins using GPIO regs
    //
    // This specifies which of the possible GPIO pins will be SCI functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPBGMUX1.bit.GPIO42 = 3; // Configure GPIO42 as SCITXDA
    GpioCtrlRegs.GPBGMUX1.bit.GPIO43 = 3; // Configure GPIO43 as SCIRXDA

    GpioCtrlRegs.GPBMUX1.bit.GPIO42 = 3; // Configure GPIO42 as SCITXDA
    GpioCtrlRegs.GPBMUX1.bit.GPIO43 = 3; // Configure GPIO43 as SCIRXDA

    EDIS;
}

void InitScibGpio(void)
{
   EALLOW;

    //
    // Enable internal pull-up for the selected pins
    //
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
//    GpioCtrlRegs.GPBPUD.bit.GPIO54 = 0;  // Enable pull-up on GPIO54 (SCITXDB)
//    GpioCtrlRegs.GPBPUD.bit.GPIO55 = 0;  // Enable pull-up on GPIO55 (SCIRXDB)

    GpioCtrlRegs.GPCPUD.bit.GPIO86 = 0;  // Enable pull-up on GPIO54 (SCITXDB)
    GpioCtrlRegs.GPCPUD.bit.GPIO87 = 0;  // Enable pull-up on GPIO55 (SCIRXDB)

    //
    // Set qualification for selected pins to asynch only
    //
    // This will select asynch (no qualification) for the selected pins.
    // Comment out other unwanted lines.
    //
//    GpioCtrlRegs.GPBQSEL2.bit.GPIO54 = 3; // Asynch input GPIO54 (SCITXDB)
//    GpioCtrlRegs.GPBQSEL2.bit.GPIO55 = 3; // Asynch input GPIO55 (SCIRXDB)

    GpioCtrlRegs.GPCQSEL2.bit.GPIO86 = 3; // Asynch input GPIO54 (SCITXDB)
    GpioCtrlRegs.GPCQSEL2.bit.GPIO87 = 3; // Asynch input GPIO55 (SCIRXDB)

    //
    //Configure SCI-a pins using GPIO regs
    //
    // This specifies which of the possible GPIO pins will be SCI functional
    // pins.
    // Comment out other unwanted lines.
    //
//    GpioCtrlRegs.GPBGMUX2.bit.GPIO54 = 1; // Configure GPIO54 as SCITXDB
//    GpioCtrlRegs.GPBGMUX2.bit.GPIO55 = 1; // Configure GPIO55 as SCIRXDB

    GpioCtrlRegs.GPCGMUX2.bit.GPIO86 = 1; // Configure GPIO54 as SCITXDB
    GpioCtrlRegs.GPCGMUX2.bit.GPIO87 = 1; // Configure GPIO55 as SCIRXDB

//    GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 2; // Configure GPIO54 as SCITXDB
//    GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 2; // Configure GPIO55 as SCIRXDB

    GpioCtrlRegs.GPCMUX2.bit.GPIO86 = 1; // Configure GPIO54 as SCITXDB
    GpioCtrlRegs.GPCMUX2.bit.GPIO87 = 1; // Configure GPIO55 as SCIRXDB

    EDIS;
}

void InitScicGpio(void)
{
   EALLOW;

    //
    // Enable internal pull-up for the selected pins
    //
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPBPUD.bit.GPIO56 = 0;  // Enable pull-up on GPIO56 (SCITXDC)
    GpioCtrlRegs.GPBPUD.bit.GPIO57 = 0;  // Enable pull-up on GPIO57 (SCIRXDC)

    //
    // Set qualification for selected pins to asynch only
    //
    // This will select asynch (no qualification) for the selected pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPBQSEL2.bit.GPIO56 = 3; // Asynch input GPIO54 (SCITXDC)
    GpioCtrlRegs.GPBQSEL2.bit.GPIO57 = 3; // Asynch input GPIO55 (SCIRXDC)

    //
    //Configure SCI-a pins using GPIO regs
    //
    // This specifies which of the possible GPIO pins will be SCI functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPBGMUX2.bit.GPIO56 = 1; // Configure GPIO54 as SCITXDB
    GpioCtrlRegs.GPBGMUX2.bit.GPIO57 = 1; // Configure GPIO55 as SCIRXDB

    GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 2; // Configure GPIO54 as SCITXDB
    GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 2; // Configure GPIO55 as SCIRXDB

    EDIS;
}
//
// End of file
//
