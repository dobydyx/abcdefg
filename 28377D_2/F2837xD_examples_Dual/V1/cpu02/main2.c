//###########################################################################
// FILE:   GPIO_toggle_cpu01.c
// TITLE:  GPIO Toggle for F2837xD.
//
// This example tests the IPC of the F2837xD.
// CPU1 have controls of one input on GPIO15 and one output on GPIO11.
// CPU2 have controls of one input on GPIO14 and one output on GPIO12.
// Toggling the input on CPU1 will also toggle the output on CPU2 through IPC.
// Toggling the input on CPU2 will also toggle the output on CPU1 through IPC.
//
// \b Watch Variables \b
// - GPIO31 - output on CPU2
// - GPIO11 - input on CPU2
// - GPIO34 - output on CPU1
// - GPIO14 - input on CPU1
// - GPIO12 - square wave output on CPU02
// - GPIO15 - square wave output on CPU01
// Connect the outputs to an oscilloscope
//
//###########################################################################
// $TI Release: F2837xD Support Library v190 $
// $Release Date: Mon Feb  1 16:51:57 CST 2016 $
// $Copyright: Copyright (C) 2013-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "F28x_Project.h"     // Device Headerfile and Examples Include File
#include "F2837xD_Ipc_drivers.h"

void main(void)
{
// Declare all variables
	uint32_t count;
	uint16_t state;

// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the F2837xD_SysCtrl.c file.
//   InitSysCtrl();

// Step 2. Initialize GPIO:
// InitGpio();  // Skipped for this example

// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
	DINT;

// Initialize PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the F2837xD_PieCtrl.c file.
	InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
	IER = 0x0000;
	IFR = 0x0000;

// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in F2837xD_DefaultIsr.c.
// This function is found in F2837xD_PieVect.c.
	InitPieVectTable();

// Enable global Interrupts and higher priority real-time debug events:
	EINT;       // Enable Global interrupt INTM
	ERTM;   // Enable Global realtime interrupt DBGM

	IPCLtoRFlagSet(IPC_FLAG17);

	state = GpioDataRegs.GPADAT.bit.GPIO11;
    
	EALLOW;
	GpioDataRegs.GPATOGGLE.bit.GPIO12 = 1;
	EDIS;

	while(1)
	{
		// Produce a Square Wave on GPIO12. This signal will be used to drive GPIO11 input on CPU2
		if (count++ > 2000000)
		{
			count = 0;
			EALLOW;
			GpioDataRegs.GPATOGGLE.bit.GPIO12 = 1;
			EDIS;
		}

		//Set Flag 11 when GPIO11 input changes
		if (GpioDataRegs.GPADAT.bit.GPIO11 != state)
		{
			state = GpioDataRegs.GPADAT.bit.GPIO11;
			if(IPCRtoLFlagBusy(IPC_FLAG11) == 0)
			{
				IPCLtoRFlagSet(IPC_FLAG11);
			}
		}

		//Toggle GPIO34 output if Flag 10 is set by CPU1
		if (IPCRtoLFlagBusy(IPC_FLAG10) == 1)
		{
			EALLOW;
			GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;
			EDIS;
			IPCRtoLFlagAcknowledge(IPC_FLAG10);
			IPCLtoRFlagClear(IPC_FLAG10);
		}
	}
}
