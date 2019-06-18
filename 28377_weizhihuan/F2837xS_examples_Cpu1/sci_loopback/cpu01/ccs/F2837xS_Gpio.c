//###########################################################################
// FILE:	F2837xS_Gpio.c
// TITLE:	GPIO module support functions
// AUTHOR:	Adam Haun
// NOTES:
//###########################################################################
// $TI Release: F2837xS Support Library v190 $
// $Release Date: Mon Feb  1 16:59:09 CST 2016 $
// $Copyright: Copyright (C) 2014-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "F2837xS_device.h"
#include "F2837xS_Examples.h"

//Low-level functions for GPIO configuration
//Sets all pins to be muxed to GPIO in input mode with pull-ups enabled.
//Also disables open drain and polarity inversion and sets the qualification 
//to synchronous. Also unlocks all GPIOs
void InitGpio()
{
    volatile Uint32 *gpioBaseAddr;
    Uint16 regOffset;

    //Disable pin locks
    EALLOW;
    GpioCtrlRegs.GPALOCK.all = 0x00000000;
    GpioCtrlRegs.GPBLOCK.all = 0x00000000;
    GpioCtrlRegs.GPCLOCK.all = 0x00000000;
    GpioCtrlRegs.GPDLOCK.all = 0x00000000;
    GpioCtrlRegs.GPELOCK.all = 0x00000000;
    GpioCtrlRegs.GPFLOCK.all = 0x00000000;

    //Fill all registers with zeros. Writing to each register separately
    //for six GPIO modules would make this function *very* long. Fortunately,
    //we'd be writing them all with zeros anyway, so this saves a lot of space.
    gpioBaseAddr = (Uint32 *)&GpioCtrlRegs;
    for (regOffset = 0; regOffset < sizeof(GpioCtrlRegs)/2; regOffset++)
    {
        //Hack to avoid enabling pull-ups on all pins. GPyPUD is offset
        //0x0C in each register group of 0x40 words. Since this is a
        //32-bit pointer, the addresses must be divided by 2.
        if (regOffset % (0x40/2) != (0x0C/2))
            gpioBaseAddr[regOffset] = 0x00000000;
    }

    gpioBaseAddr = (Uint32 *)&GpioDataRegs;
    for (regOffset = 0; regOffset < sizeof(GpioDataRegs)/2; regOffset++)
    {
        gpioBaseAddr[regOffset] = 0x00000000;
    }
    EDIS;
}

void Gpioselect()
{
    EALLOW;

	GpioCtrlRegs.GPCDIR.bit.GPIO66 = 1;    // Configure as the Right CPLD WDT_TOG  GPIO4
	GpioCtrlRegs.GPDDIR.bit.GPIO99 = 1;    // Configure as the Left CPLD WDT_TOG   GPIO3
    GpioCtrlRegs.GPBDIR.bit.GPIO45 = 1;		//output as H_A                        GPIO6
//    GpioCtrlRegs.GPCDIR.bit.GPIO87 = 1;		//output as H_A                        GPIO6


    GpioCtrlRegs.GPADIR.bit.GPIO17 = 0;		//input as DAT1                        PWM8B
    GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 2;
    GpioCtrlRegs.GPADIR.bit.GPIO27 = 0;		//input as DAT2                        GPIO1
    GpioCtrlRegs.GPAQSEL2.bit.GPIO27 = 2;
    GpioCtrlRegs.GPADIR.bit.GPIO26 = 0;		//input as DAT3                        GPIO2
    GpioCtrlRegs.GPAQSEL2.bit.GPIO26 = 2;
    GpioCtrlRegs.GPADIR.bit.GPIO8 = 1;		//output as MUX1                       PWM5A
    GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;		//output as MUX2                       PWM5B
    GpioCtrlRegs.GPADIR.bit.GPIO16 = 1;		//output as MUX3                       PWM8A

//    GpioCtrlRegs.GPCDIR.bit.GPIO90 = 1;		//output as BRAKE
//    GpioDataRegs.GPCDAT.bit.GPIO90 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO7 = 1;		//output as BRAKE
    GpioDataRegs.GPADAT.bit.GPIO7 = 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO44 = 1;		//output as DISABLE3                   GPIO5
    GpioDataRegs.GPBDAT.bit.GPIO44 = 0;		//output as DISABLE改为1
    GpioCtrlRegs.GPCDIR.bit.GPIO91 = 1;		//output as /RE_A
    GpioDataRegs.GPCDAT.bit.GPIO91 = 0;
    GpioCtrlRegs.GPCDIR.bit.GPIO93 = 1;		//output as DE_A
    GpioDataRegs.GPCDAT.bit.GPIO93 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO24 = 1;		//output as DE_B
    GpioDataRegs.GPADAT.bit.GPIO24 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO25 = 1;		//output as /RE_B
    GpioDataRegs.GPADAT.bit.GPIO25 = 0;

/*
    GpioCtrlRegs.GPBDIR.bit.GPIO58 = 1;		//output as ALE锁存
    GpioCtrlRegs.GPADIR.bit.GPIO8 = 1;		//output as A0  A0=0&&A1=0输入FLT
    GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;		//output as A1  A0=1&&A1=1输入RDY，否则输入0

    GpioCtrlRegs.GPADIR.bit.GPIO15 = 0;		//input DAT1
    GpioCtrlRegs.GPADIR.bit.GPIO19 = 0;		//DAT2
    GpioCtrlRegs.GPADIR.bit.GPIO27 = 0;		//DAT3
    GpioCtrlRegs.GPBDIR.bit.GPIO41 = 0;		//DAT4
    GpioCtrlRegs.GPBDIR.bit.GPIO39 = 0;		//DAT5
    GpioCtrlRegs.GPBDIR.bit.GPIO52 = 0;		//DAT6
    GpioCtrlRegs.GPBDIR.bit.GPIO53 = 0;		//DAT7
    GpioCtrlRegs.GPBDIR.bit.GPIO54 = 0;		//DAT8
    GpioCtrlRegs.GPBDIR.bit.GPIO55 = 0;		//DAT9
    GpioCtrlRegs.GPBDIR.bit.GPIO57 = 0;		//DAT10
    GpioCtrlRegs.GPBDIR.bit.GPIO42 = 0;		//DAT11
    GpioCtrlRegs.GPBDIR.bit.GPIO43 = 0;		//DAT12

    GpioCtrlRegs.GPADIR.bit.GPIO24 = 1;		//output as DISABLE1
    GpioDataRegs.GPADAT.bit.GPIO24 = 0;		//output as DISABLE改为1
    GpioCtrlRegs.GPADIR.bit.GPIO25 = 1;		//output as DISABLE2
    GpioDataRegs.GPADAT.bit.GPIO25 = 0;		//output as DISABLE改为1
    GpioCtrlRegs.GPADIR.bit.GPIO26 = 1;		//output as DISABLE3
    GpioDataRegs.GPADAT.bit.GPIO26 = 0;		//output as DISABLE改为1
    GpioCtrlRegs.GPBDIR.bit.GPIO40 = 1;		//output as DISABLE4
    GpioDataRegs.GPBDAT.bit.GPIO40 = 1;		//output as DISABLE改为1
*/

    GpioCtrlRegs.GPBDIR.bit.GPIO40 = 1;		//output as WR_A to AD2S1210
    GpioCtrlRegs.GPBDIR.bit.GPIO41 = 1;		//output as SAMPLEn_A to AD2S1210
    GpioCtrlRegs.GPBDIR.bit.GPIO47 = 1;		//output as RESET_A to AD2S1210
    GpioCtrlRegs.GPBDIR.bit.GPIO43 = 1;		//output as A0_A to AD2S1210
    GpioCtrlRegs.GPBDIR.bit.GPIO42 = 1;		//output as A1_A to AD2S1210

    EDIS;
}

//Set the peripheral muxing for the specified pin. The appropriate
//parameters can be found in the GPIO Muxed Pins table(4.4) in the datasheet.
//Use the GPIO index row (0 to 15) to select a muxing option for the GPIO.
void GPIO_SetupPinMux(Uint16 pin, Uint16 cpu, Uint16 peripheral)
{
    volatile Uint32 *gpioBaseAddr;
    volatile Uint32 *mux, *gmux, *csel;
    Uint16 pin32, pin16, pin8;

    pin32 = pin % 32;
    pin16 = pin % 16;
    pin8 = pin % 8;
    gpioBaseAddr = (Uint32 *)&GpioCtrlRegs + (pin/32)*GPY_CTRL_OFFSET;

    //Sanity check for valid cpu and peripheral values
    if (cpu > GPIO_MUX_CPU2CLA || peripheral > 0xF)
        return;

    //Create pointers to the appropriate registers. This is a workaround
    //for the way GPIO registers are defined. The standard definition
    //in the header file makes it very easy to do named accesses of one
    //register or bit, but hard to do arbitrary numerical accesses. It's
    //easier to have an array of GPIO modules with identical registers,
    //including arrays for multi-register groups like GPyCSEL1-4. But
    //the header file doesn't define anything we can turn into an array,
    //so manual pointer arithmetic is used instead.
    mux = gpioBaseAddr + GPYMUX + pin32/16;
    gmux = gpioBaseAddr + GPYGMUX + pin32/16;
    csel = gpioBaseAddr + GPYCSEL + pin32/8;

    //Now for the actual function
    EALLOW;

    //To change the muxing, set the peripheral mux to 0/GPIO first to avoid
    //glitches, then change the group mux, then set the peripheral mux to
    //its target value. Finally, set the CPU select. This procedure is
    //described in the TRM. Unfortunately, since we don't know the pin in
    //advance we can't hardcode a bitfield reference, so there's some tricky
    //bit twiddling here.
    *mux &= ~(0x3UL << (2*pin16));
    *gmux &= ~(0x3UL << (2*pin16));
    *gmux |= (Uint32)((peripheral >> 2) & 0x3UL) << (2*pin16);
    *mux |= (Uint32)(peripheral & 0x3UL) << (2*pin16);

    *csel &= ~(0x3L << (4*pin8));
    *csel |= (Uint32)(cpu & 0x3L) << (4*pin8);

    //WARNING: This code does not touch the analog mode select registers,
    //which are needed to give the USB module control of its IOs.

    EDIS;
}


//Setup up the GPIO input/output options for the specified pin.
//The flags are a 16-bit mask produced by ORing together options.
//For input pins, the valid flags are:
//GPIO_PULLUP	Enable pull-up
//GPIO_INVERT	Enable input polarity inversion
//GPIO_SYNC		Synchronize the input latch to PLLSYSCLK (default -- you don't need to specify this)
//GPIO_QUAL3	Use 3-sample qualification
//GPIO_QUAL6	Use 6-sample qualification
//GPIO_ASYNC	Do not use synchronization or qualification
//(Note: only one of SYNC, QUAL3, QUAL6, or ASYNC is allowed)
//
//For output pins, the valid flags are:
//GPIO_OPENDRAIN	Output in open drain mode
//GPIO_PULLUP		If open drain enabled, also enable the pull-up
//and the input qualification flags (SYNC/QUAL3/QUAL6/SYNC) listed above.
//
//With no flags, the default input state is synchronous with no
//pull-up or polarity inversion. The default output state is
//the standard digital output.
void GPIO_SetupPinOptions(Uint16 pin, Uint16 output, Uint16 flags)
{
    volatile Uint32 *gpioBaseAddr;
    volatile Uint32 *dir, *pud, *inv, *odr, *qsel;
    Uint32 pin32, pin16, pinMask, qual;

    pin32 = pin % 32;
    pin16 = pin % 16;
    pinMask = 1UL << pin32;
    gpioBaseAddr = (Uint32 *)&GpioCtrlRegs + (pin/32)*GPY_CTRL_OFFSET;

    //Create pointers to the appropriate registers. This is a workaround
    //for the way GPIO registers are defined. The standard definition
    //in the header file makes it very easy to do named accesses of one
    //register or bit, but hard to do arbitrary numerical accesses. It's
    //easier to have an array of GPIO modules with identical registers,
    //including arrays for multi-register groups like GPyQSEL1-2. But
    //the header file doesn't define anything we can turn into an array,
    //so manual pointer arithmetic is used instead.
    dir = gpioBaseAddr + GPYDIR;
    pud = gpioBaseAddr + GPYPUD;
    inv = gpioBaseAddr + GPYINV;
    odr = gpioBaseAddr + GPYODR;
    qsel = gpioBaseAddr + GPYQSEL + pin32/16;

    EALLOW;

    //Set the data direction
    *dir &= ~pinMask;
    if (output == 1)
    {
        //Output, with optional open drain mode and pull-up
        *dir |= pinMask;

        //Enable open drain if necessary
        if (flags & GPIO_OPENDRAIN)
            *odr |= pinMask;
        else
            *odr &= ~pinMask;

        //Enable pull-up if necessary. Open drain mode must be active.
        if (flags & (GPIO_OPENDRAIN | GPIO_PULLUP))
            *pud &= ~pinMask;
        else
            *pud |= pinMask;
    } else
    {
        //Input, with optional pull-up, qualification, and polarity inversion
        *dir &= ~pinMask;

        //Enable pull-up if necessary
        if (flags & GPIO_PULLUP)
            *pud &= ~pinMask;
        else
            *pud |= pinMask;

        //Invert polarity if necessary
        if (flags & GPIO_INVERT)
            *inv |= pinMask;
        else
            *inv &= ~pinMask;
    }

    //Extract the qualification parameter and load it into the register. This is
    //also needed for open drain outputs, so we might as well do it all the time.
    qual = (flags & GPIO_ASYNC) / GPIO_QUAL3;
    *qsel &= ~(0x3L << (2 * pin16));
    if (qual != 0x0)
        *qsel |= qual << (2 * pin16);

    EDIS;
}

//Enable or disable the GPIO register bit lock for the specified pin.
//The valid flags are:
//GPIO_UNLOCK	Unlock the pin setup register bits for the specified pin
//GPIO_LOCK 	Lock the pin setup register bits for the specified pin
void GPIO_SetupLock(Uint16 pin, Uint16 flags)
{
    volatile Uint32 *gpioBaseAddr;
    volatile Uint32 *lock;
    Uint32 pin32, pinMask;

    pin32 = pin % 32;
    pinMask = 1UL << pin32;
    gpioBaseAddr = (Uint32 *)&GpioCtrlRegs + (pin/32)*GPY_CTRL_OFFSET;

    //Create pointers to the appropriate registers. This is a workaround
    //for the way GPIO registers are defined. The standard definition
    //in the header file makes it very easy to do named accesses of one
    //register or bit, but hard to do arbitrary numerical accesses. It's
    //easier to have an array of GPIO modules with identical registers,
    //including arrays for multi-register groups like GPyQSEL1-2. But
    //the header file doesn't define anything we can turn into an array,
    //so manual pointer arithmetic is used instead.
    lock = gpioBaseAddr + GPYLOCK;


    EALLOW;
    if(flags){
        //Lock the pin
        *lock |= pinMask;
    }else{
        //Unlock the pin
        *lock &= ~pinMask;
    }
    EDIS;
}

//External interrupt setup
void GPIO_SetupXINT1Gpio(Uint16 pin)
{
    EALLOW;
    InputXbarRegs.INPUT4SELECT = pin; 		//Set XINT1 source to GPIO-pin
    EDIS;
}
void GPIO_SetupXINT2Gpio(Uint16 pin)
{
    EALLOW;
    InputXbarRegs.INPUT5SELECT = pin; 		//Set XINT2 source to GPIO-pin
    EDIS;
}
void GPIO_SetupXINT3Gpio(Uint16 pin)
{
    EALLOW;
    InputXbarRegs.INPUT6SELECT = pin; 		//Set XINT3 source to GPIO-pin
    EDIS;
}
void GPIO_SetupXINT4Gpio(Uint16 pin)
{
    EALLOW;
    InputXbarRegs.INPUT13SELECT = pin; 	//Set XINT4 source to GPIO-pin
    EDIS;
}
void GPIO_SetupXINT5Gpio(Uint16 pin)
{
    EALLOW;
    InputXbarRegs.INPUT14SELECT = pin; 	//Set XINT5 source to GPIO-pin
    EDIS;
}


//Enable pullups for the unbonded GPIOs on the 176PTP package:
//GPIOs     Grp Bits
//95-132    C   31
//          D   31:0
//          E   4:0
//134-168   E   31:6
//          F   8:0
void GPIO_EnableUnbondedIOPullupsFor176Pin()
{
    EALLOW;
    GpioCtrlRegs.GPCPUD.all = ~0x80000000;  //GPIO 95
    GpioCtrlRegs.GPDPUD.all = ~0xFFFFFFF7;  //GPIOs 96-127
    GpioCtrlRegs.GPEPUD.all = ~0xFFFFFFDF;  //GPIOs 128-159 except for 133
    GpioCtrlRegs.GPFPUD.all = ~0x000001FF;  //GPIOs 160-168
    EDIS;
}

//Enable pullups for the unbonded GPIOs on the 100PZ package:
//GPIOs     Grp Bits
//0-1       A   1:0
//5-9       A   9:5
//22-40     A   31:22
//          B   8:0
//44-57     B   25:12
//67-68     C   4:3
//74-77     C   13:10
//79-83     C   19:15
//93-168    C   31:29
//          D   31:0
//          E   31:0
//          F   8:0
void GPIO_EnableUnbondedIOPullupsFor100Pin()
{
    EALLOW;
    GpioCtrlRegs.GPAPUD.all = ~0xFFC003E3;  //GPIOs 0-1, 5-9, 22-31
    GpioCtrlRegs.GPBPUD.all = ~0x03FFF1FF;  //GPIOs 32-40, 44-57
    GpioCtrlRegs.GPCPUD.all = ~0xE10FBC18;  //GPIOs 67-68, 74-77, 79-83, 93-95
    GpioCtrlRegs.GPDPUD.all = ~0xFFFFFFF7;  //GPIOs 96-127
    GpioCtrlRegs.GPEPUD.all = ~0xFFFFFFFF;  //GPIOs 128-159
    GpioCtrlRegs.GPFPUD.all = ~0x000001FF;  //GPIOs 160-168
    EDIS;
}

//InitSysCtrl would call this function this takes care of stuff.
void GPIO_EnableUnbondedIOPullups()
{
    unsigned char pin_count = ((DevCfgRegs.PARTIDL.all & 0x00000700) >> 8) ; //bits 8-10 have pin count*/
/*
 * 5 = 100 pin
 * 6 = 176 pin
 * 7 = 337 pin
 */

    if(pin_count == 5)
    {
        GPIO_EnableUnbondedIOPullupsFor100Pin();
    }
    else if (pin_count == 6)
    {
        GPIO_EnableUnbondedIOPullupsFor176Pin();
    }
    else
    {
        //do nothing - this is 337 pin package
    }

}

//Read the GPyDAT register bit for the specified pin. Note that this returns
//the actual state of the pin, not the state of the output latch.
Uint16 GPIO_ReadPin(Uint16 pin)
{
	volatile Uint32 *gpioDataReg;
	Uint16 pinVal;

	gpioDataReg = (volatile Uint32 *)&GpioDataRegs + (pin/32)*GPY_DATA_OFFSET;
	pinVal = (gpioDataReg[GPYDAT] >> (pin % 32)) & 0x1;

	return pinVal;
}

//Set the GPyDAT register bit for the specified pin.
void GPIO_WritePin(Uint16 pin, Uint16 outVal)
{
	volatile Uint32 *gpioDataReg;
	Uint32 pinMask;

	gpioDataReg = (volatile Uint32 *)&GpioDataRegs + (pin/32)*GPY_DATA_OFFSET;
	pinMask = 1UL << (pin % 32);

	if (outVal == 0)
		gpioDataReg[GPYCLEAR] = pinMask;
	else
		gpioDataReg[GPYSET] = pinMask;
}
