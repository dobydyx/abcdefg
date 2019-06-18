//###########################################################################
//
// FILE:   F2837xS_Spi.c
//
// TITLE:  F2837xS SPI Initialization & Support Functions.
//
//###########################################################################
// $TI Release: F2837xS Support Library v190 $
// $Release Date: Mon Feb  1 16:59:09 CST 2016 $
// $Copyright: Copyright (C) 2014-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "F2837xS_device.h"     // F2837xS Headerfile Include File
#include "F2837xS_Examples.h"   // F2837xS Examples Include File

//---------------------------------------------------------------------------
// Example: InitSpiGpio:
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as SPI pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.
//
// Caution:
// For each SPI peripheral
// Only one GPIO pin should be enabled for SPISOMO operation.
// Only one GPIO pin should be enabled for SPISOMI operation.
// Only one GPIO pin should be enabled for SPICLK  operation.
// Only one GPIO pin should be enabled for SPISTE  operation.
// Comment out other unwanted lines.

void InitSpiGpio()
{
   InitSpiaGpio();
//   InitSpibGpio();
}

void InitSpi()
{
   InitSpia();
//   InitSpib();
}

void InitSpiaGpio()
{

   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPBPUD.bit.GPIO58 = 0;   // Enable pull-up on GPIO58 (SPISIMOA)
    GpioCtrlRegs.GPBPUD.bit.GPIO59 = 0;   // Enable pull-up on GPIO59 (SPISOMIA)
    GpioCtrlRegs.GPBPUD.bit.GPIO60 = 0;   // Enable pull-up on GPIO60 (SPICLKA)
    GpioCtrlRegs.GPBPUD.bit.GPIO61 = 0;   // Enable pull-up on GPIO61 (SPISTEA)

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPBQSEL2.bit.GPIO58 = 3; // Asynch input GPIO58 (SPISIMOA)
    GpioCtrlRegs.GPBQSEL2.bit.GPIO59 = 3; // Asynch input GPIO59 (SPISOMIA)
    GpioCtrlRegs.GPBQSEL2.bit.GPIO60 = 3; // Asynch input GPIO60 (SPICLKA)
    GpioCtrlRegs.GPBQSEL2.bit.GPIO61 = 3; // Asynch input GPIO61 (SPISTEA)

/* Configure SPI-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SPI functional pins.
// Comment out other unwanted lines.

//  GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 15; // Configure GPIO58 as SPISIMOA
//  GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 15; // Configure GPIO59 as SPISOMIA
//  GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 15; // Configure GPIO60 as SPICLKA
//  GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 15; // Configure GPIO61 as SPISTEA

	GPIO_SetupPinMux(58, GPIO_MUX_CPU1, 15);
    GPIO_SetupPinMux(59, GPIO_MUX_CPU1, 15);
    GPIO_SetupPinMux(60, GPIO_MUX_CPU1, 15);
    GPIO_SetupPinMux(61, GPIO_MUX_CPU1, 15);

    EDIS;
}

void InitSpibGpio()
{

   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;   // Enable pull-up on GPIO63 (SPISIMOA)
    GpioCtrlRegs.GPCPUD.bit.GPIO64 = 0;   // Enable pull-up on GPIO64 (SPISOMIA)
    GpioCtrlRegs.GPCPUD.bit.GPIO65 = 0;   // Enable pull-up on GPIO65 (SPICLKA)
    GpioCtrlRegs.GPCPUD.bit.GPIO66 = 0;   // Enable pull-up on GPIO66 (SPISTEA)

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPBQSEL2.bit.GPIO63 = 3; // Asynch input GPIO63 (SPISIMOA)
    GpioCtrlRegs.GPCQSEL1.bit.GPIO64 = 3; // Asynch input GPIO64 (SPISOMIA)
    GpioCtrlRegs.GPCQSEL1.bit.GPIO65 = 3; // Asynch input GPIO65 (SPICLKA)
    GpioCtrlRegs.GPCQSEL1.bit.GPIO66 = 3; // Asynch input GPIO66 (SPISTEA)

/* Configure SPI-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SPI functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 15; // Configure GPIO63 as SPISIMOA
    GpioCtrlRegs.GPCMUX1.bit.GPIO64 = 15; // Configure GPIO64 as SPISOMIA
    GpioCtrlRegs.GPCMUX1.bit.GPIO65 = 15; // Configure GPIO65 as SPICLKA
    GpioCtrlRegs.GPCMUX1.bit.GPIO66 = 15; // Configure GPIO66 as SPISTEA

    EDIS;
}

//---------------------------------------------------------------------------
// InitSPI:
//---------------------------------------------------------------------------
// This function initializes the SPI(s) to a known state.
//
void InitSpia(void)
{
   // Initialize SPI-A
   SpiaRegs.SPICCR.bit.SPISWRESET=0; // Reset SPI

   SpiaRegs.SPICCR.all = 0x0007;       // 8-bit character,output on the rising edge,input data is latched on the falling edge
   SpiaRegs.SPICTL.all = 0x0006;       // Enable master mode, normal phase, enable talk, and disable SPI int.
   SpiaRegs.SPISTS.all = 0x0000;
   SpiaRegs.SPIBRR.all = 0x0007;       // 10MHz Baud rate

   SpiaRegs.SPICCR.bit.SPISWRESET=1;   // Enable SPI

}

//===========================================================================
// End of file.
//===========================================================================
