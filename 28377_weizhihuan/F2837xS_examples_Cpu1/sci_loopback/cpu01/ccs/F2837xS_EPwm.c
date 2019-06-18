//###########################################################################
//
// FILE:   F2837xS_EPwm.c
//
// TITLE:  F2837xS EPwm Initialization & Support Functions.
//
//###########################################################################
// $TI Release: F2837xS Support Library v190 $
// $Release Date: Mon Feb  1 16:59:09 CST 2016 $
// $Copyright: Copyright (C) 2014-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "F2837xS_device.h"     // F2837xS Headerfile Include File
#include "F2837xS_Examples.h"   // F2837xS Examples Include File
#include "Common_Definitions.h"     // Common Definitions Headerfile Include File

void InitEPwmGpio(void)
{
	InitEPwm1Gpio();
//	InitEPwm2Gpio();
//	InitEPwm3Gpio();
//	InitEPwm4Gpio();
//	InitEPwm5Gpio();
	InitEPwm6Gpio();
	InitEPwm7Gpio();
//	InitEPwm8Gpio();
//	InitEPwm9Gpio();
//	InitEPwm10Gpio();
//	InitEPwm11Gpio();
//	InitEPwm12Gpio();
}

void InitEPwm(void)
{
	EALLOW;
	CpuSysRegs.PCLKCR0.bit.TBCLKSYNC =0;

	EDIS;

	InitEPwm1();
	InitEPwm6();
	InitEPwm7();

	EALLOW;
	CpuSysRegs.PCLKCR0.bit.TBCLKSYNC =1;

	EDIS;
}

void InitEPwm1Gpio(void)
{
   EALLOW;

/* Disable internal pull-up for the selected output pins
   for reduced power consumption */
// Pull-ups can be enabled or disabled by the user.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;    // Disable pull-up on GPIO1 (EPWM1B)
// 	GpioCtrlRegs.GPEPUD.bit.GPIO145 = 1;    // Disable pull-up on GPIO145 (EPWM1A)
// 	GpioCtrlRegs.GPEPUD.bit.GPIO146 = 1;    // Disable pull-up on GPIO146 (EPWM1B)
	

/* Configure EPWM-1 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be EPWM1 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO1 as EPWM1B
//	GpioCtrlRegs.GPEMUX2.bit.GPIO145 = 1;   // Configure GPIO145 as EPWM1A
//	GpioCtrlRegs.GPEMUX2.bit.GPIO146 = 1;   // Configure GPIO0146 as EPWM1B

    EDIS;
}

void InitEPwm2Gpio(void)
{
   EALLOW;

/* Disable internal pull-up for the selected output pins
   for reduced power consumption */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;    // Disable pull-up on GPIO2 (EPWM2A)
    GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;    // Disable pull-up on GPIO3 (EPWM2B)
// 	GpioCtrlRegs.GPEPUD.bit.GPIO147 = 1;    // Disable pull-up on GPIO147 (EPWM2A)
// 	GpioCtrlRegs.GPEPUD.bit.GPIO148 = 1;    // Disable pull-up on GPIO148 (EPWM2B)

/* Configure EPwm-2 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be EPWM2 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO2 as EPWM2A
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO3 as EPWM2B
//	GpioCtrlRegs.GPEMUX2.bit.GPIO147 = 1;   // Configure GPIO147 as EPWM2A
//	GpioCtrlRegs.GPEMUX2.bit.GPIO148 = 1;   // Configure GPIO148 as EPWM2B

    EDIS;
}

void InitEPwm3Gpio(void)
{
   EALLOW;

/* Disable internal pull-up for the selected output pins
   for reduced power consumption */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;    // Disable pull-up on GPIO4 (EPWM3A)
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;    // Disable pull-up on GPIO5 (EPWM3B)
// 	GpioCtrlRegs.GPEPUD.bit.GPIO149 = 1;    // Disable pull-up on GPIO149 (EPWM3A)
// 	GpioCtrlRegs.GPEPUD.bit.GPIO150 = 1;    // Disable pull-up on GPIO150 (EPWM3B)

/* Configure EPwm-3 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be EPWM3 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO4 as EPWM3A
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO5 as EPWM3B
//	GpioCtrlRegs.GPEMUX2.bit.GPIO149 = 1;   // Configure GPIO149 as EPWM3A
//	GpioCtrlRegs.GPEMUX2.bit.GPIO150 = 1;   // Configure GPIO150 as EPWM3B

    EDIS;
}

void InitEPwm4Gpio(void)
{
   EALLOW;
/* Disable internal pull-up for the selected output pins
   for reduced power consumption */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;    // Disable pull-up on GPIO6 (EPWM4A)
    GpioCtrlRegs.GPAPUD.bit.GPIO7 = 1;    // Disable pull-up on GPIO7 (EPWM4B)
// 	GpioCtrlRegs.GPEPUD.bit.GPIO151 = 1;    // Disable pull-up on GPIO151 (EPWM4A)
// 	GpioCtrlRegs.GPEPUD.bit.GPIO152 = 1;    // Disable pull-up on GPIO152 (EPWM4B)

/* Configure EPWM-4 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be EPWM4 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;   // Configure GPIO6 as EPWM4A
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;   // Configure GPIO7 as EPWM4B
//	GpioCtrlRegs.GPEMUX2.bit.GPIO151 = 1;   // Configure GPIO151 as EPWM4A
//	GpioCtrlRegs.GPEMUX2.bit.GPIO152 = 1;   // Configure GPIO152 as EPWM4B

    EDIS;
}

void InitEPwm5Gpio(void)
{
   EALLOW;
/* Disable internal pull-up for the selected output pins
   for reduced power consumption */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 1;    // Disable pull-up on GPIO8 (EPWM5A)
    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 1;    // Disable pull-up on GPIO9 (EPWM5B)
// 	GpioCtrlRegs.GPEPUD.bit.GPIO153 = 1;    // Disable pull-up on GPIO153 (EPWM5A)
// 	GpioCtrlRegs.GPEPUD.bit.GPIO154 = 1;    // Disable pull-up on GPIO154 (EPWM5B)

/* Configure EPWM-5 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be EPWM5 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;   // Configure GPIO8 as EPWM5A
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;   // Configure GPIO9 as EPWM5B
//	GpioCtrlRegs.GPEMUX2.bit.GPIO153 = 1;   // Configure GPIO153 as EPWM5A
//	GpioCtrlRegs.GPEMUX2.bit.GPIO154 = 1;   // Configure GPIO0154 as EPWM5B

    EDIS;
}

void InitEPwm6Gpio(void)
{
   EALLOW;
/* Disable internal pull-up for the selected output pins
   for reduced power consumption */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO10 = 1;    // Disable pull-up on GPIO10 (EPWM6A)
    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 1;    // Disable pull-up on GPIO11 (EPWM6B)
// 	GpioCtrlRegs.GPEPUD.bit.GPIO155 = 1;    // Disable pull-up on GPIO155 (EPWM6A)
// 	GpioCtrlRegs.GPEPUD.bit.GPIO156 = 1;    // Disable pull-up on GPIO156 (EPWM6B)

/* Configure EPWM-6 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be EPWM6 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;   // Configure GPIO10 as EPWM6A
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;   // Configure GPIO11 as EPWM6B
	//	GpioCtrlRegs.GPEMUX2.bit.GPIO155 = 1;   // Configure GPIO155 as EPWM6A
	//	GpioCtrlRegs.GPEMUX2.bit.GPIO156 = 1;   // Configure GPIO156 as EPWM6B

    EDIS;
}

void InitEPwm7Gpio(void)
{
   EALLOW;
/* Disable internal pull-up for the selected output pins
   for reduced power consumption */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO12 = 1;    // Disable pull-up on GPIO12 (EPWM7A)
    GpioCtrlRegs.GPAPUD.bit.GPIO13 = 1;    // Disable pull-up on GPIO13 (EPWM7B)
// 	GpioCtrlRegs.GPEPUD.bit.GPIO157 = 1;    // Disable pull-up on GPIO157 (EPWM7A)
// 	GpioCtrlRegs.GPEPUD.bit.GPIO158 = 1;    // Disable pull-up on GPIO158 (EPWM7B)

/* Configure EPWM-6 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be EPWM6 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;   // Configure GPIO12 as EPWM7A
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;   // Configure GPIO13 as EPWM7B
	//	GpioCtrlRegs.GPEMUX2.bit.GPIO157 = 1;   // Configure GPIO157 as EPWM7A
	//	GpioCtrlRegs.GPEMUX2.bit.GPIO158 = 1;   // Configure GPIO158 as EPWM7B

    EDIS;
}

void InitEPwm8Gpio(void)
{
   EALLOW;
/* Disable internal pull-up for the selected output pins
   for reduced power consumption */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO14 = 1;    // Disable pull-up on GPIO14 (EPWM8A)
    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 1;    // Disable pull-up on GPIO15 (EPWM8B)
// 	GpioCtrlRegs.GPEPUD.bit.GPIO159 = 1;    // Disable pull-up on GPIO159 (EPWM8A)
//  GpioCtrlRegs.GPFPUD.bit.GPIO160 = 1;    // Disable pull-up on GPIO160 (EPWM8B)

/* Configure EPWM-6 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be EPWM6 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1;   // Configure GPIO14 as EPWM8A
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1;   // Configure GPIO15 as EPWM8B
	//	GpioCtrlRegs.GPEMUX2.bit.GPIO159 = 1;   // Configure GPIO159 as EPWM8A
	//	GpioCtrlRegs.GPFMUX1.bit.GPIO160 = 1;   // Configure GPIO160 as EPWM8B

    EDIS;
}

void InitEPwm9Gpio(void)
{
   EALLOW;
/* Disable internal pull-up for the selected output pins
   for reduced power consumption */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPFPUD.bit.GPIO161 = 1;    // Disable pull-up on GPIO161 (EPWM9A)
    GpioCtrlRegs.GPFPUD.bit.GPIO162 = 1;    // Disable pull-up on GPIO162 (EPWM9B)

/* Configure EPWM-6 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be EPWM6 functional pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPFMUX1.bit.GPIO161 = 1;   // Configure GPIO161 as EPWM9A
	GpioCtrlRegs.GPFMUX1.bit.GPIO162 = 1;   // Configure GPIO162 as EPWM9B

    EDIS;
}

void InitEPwm10Gpio(void)
{
   EALLOW;
/* Disable internal pull-up for the selected output pins
   for reduced power consumption */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPFPUD.bit.GPIO163 = 1;    // Disable pull-up on GPIO163 (EPWM10A)
    GpioCtrlRegs.GPFPUD.bit.GPIO164 = 1;    // Disable pull-up on GPIO164 (EPWM10B)

/* Configure EPWM-6 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be EPWM6 functional pins.
// Comment out other unwanted lines.

   GpioCtrlRegs.GPFMUX1.bit.GPIO163 = 1;   // Configure GPIO163 as EPWM10A
   GpioCtrlRegs.GPFMUX1.bit.GPIO164 = 1;   // Configure GPIO164 as EPWM10B

    EDIS;
}

void InitEPwm11Gpio(void)
{
   EALLOW;
/* Disable internal pull-up for the selected output pins
   for reduced power consumption */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPFPUD.bit.GPIO165 = 1;    // Disable pull-up on GPIO165 (EPWM11A)
    GpioCtrlRegs.GPFPUD.bit.GPIO166 = 1;    // Disable pull-up on GPIO166 (EPWM11B)

/* Configure EPWM-6 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be EPWM6 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPFMUX1.bit.GPIO165 = 1;   // Configure GPIO165 as EPWM11A
    GpioCtrlRegs.GPFMUX1.bit.GPIO166 = 1;   // Configure GPIO166 as EPWM11B

    EDIS;
}

void InitEPwm12Gpio(void)
{
   EALLOW;
/* Disable internal pull-up for the selected output pins
   for reduced power consumption */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPFPUD.bit.GPIO167 = 1;    // Disable pull-up on GPIO167 (EPWM12A)
    GpioCtrlRegs.GPFPUD.bit.GPIO168 = 1;    // Disable pull-up on GPIO168 (EPWM12B)

/* Configure EPWM-6 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be EPWM6 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPFMUX1.bit.GPIO167 = 1;   // Configure GPIO167 as EPWM12A
    GpioCtrlRegs.GPFMUX1.bit.GPIO168 = 1;   // Configure GPIO168 as EPWM12B

    EDIS;
}

void InitEPwm1()
{

   // Setup TBCLK(10nS)
   EPwm1Regs.TBPRD = EPWM3_TIMER_TBPRD;           // Set timer period 5000 TBCLKs =50uS, DIV=1
   EPwm1Regs.TBPHS.bit.TBPHS = 0;                 // Phase is 0
   EPwm1Regs.TBCTR = 0x0000;                      // Clear counter

   // Set Compare values
   EPwm1Regs.CMPA.bit.CMPA = EPWM3_MIN_CMPA;     // Set compare A value
   EPwm1Regs.CMPB.bit.CMPB = EPWM3_MAX_CMPB;               // Set Compare B value

   // Setup counter mode
   EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; 		// Count up
   EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
//   EPwm1Regs.TBCTL.bit.PHSDIR = TB_UP;
   EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;
   EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; // Sync down-stream module
   EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
   EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;


   // Setup shadowing
   EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
   EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
   EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

   // Set actions
   EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;             // Set PWM1A on event A, up count
   EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;           // Clear PWM1A on event A, UP count
//   EPwm1Regs.AQCTLA.bit.ZRO = AQ_SET;             // Set PWM1A on event A, up count

//   EPwm1Regs.AQCTLB.bit.ZRO = AQ_SET;          // Clear PWM1B on event B, up count
//   EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;            // Set PWM1B on event B, UP count

   // Interrupt where we will change the Compare Values
//   EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;      // Select INT on Zero event
//   EPwm1Regs.ETSEL.bit.INTEN = 1;                 // Enable INT
//   EPwm1Regs.ETPS.bit.INTPRD = ET_3RD;            // Generate INT on 3rd event

   // Set Dead-band
   EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;	// enable Dead-band module
   EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; 		// Active Hi complementary
   EPwm1Regs.DBFED.all = 80; 							// FED = 100 TBCLKs initially
   EPwm1Regs.DBRED.all = 80; 							// RED = 100 TBCLKs initially

   // Information this example uses to keep track
   // of the direction the CMPA/CMPB values are
   // moving, the min and max allowed values and
   // a pointer to the correct ePWM registers
/*
   epwm1_info.EPwm_CMPA_Direction = EPWM_CMP_UP;   // Start by increasing CMPA &
   epwm1_info.EPwm_CMPB_Direction = EPWM_CMP_DOWN; // decreasing CMPB
   epwm1_info.EPwmTimerIntCount = 0;               // Zero the interrupt counter
   epwm1_info.EPwmRegHandle = &EPwm1Regs;          // Set the pointer to the ePWM module
   epwm1_info.EPwmMaxCMPA = EPWM1_MAX_CMPA;        // Setup min/max CMPA/CMPB values
   epwm1_info.EPwmMinCMPA = EPWM1_MIN_CMPA;
   epwm1_info.EPwmMaxCMPB = EPWM1_MAX_CMPB;
   epwm1_info.EPwmMinCMPB = EPWM1_MIN_CMPB;
*/
}

void InitEPwm6()
{


   // Setup TBCLK(10nS)
   EPwm6Regs.TBPRD = EPWM1_TIMER_TBPRD;           // Set timer period 5000 TBCLKs
   EPwm6Regs.TBPHS.bit.TBPHS = 1333.33;                 // Phase is 0
   EPwm6Regs.TBCTR = 0x0000;                      // Clear counter

   // Set Compare values
   EPwm6Regs.CMPA.bit.CMPA = EPWM1_MIN_CMPA;     // Set compare A value
   EPwm6Regs.CMPB.bit.CMPB = EPWM1_MIN_CMPB;               // Set Compare B value

   // Setup counter mode
   EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count updown
   EPwm6Regs.TBCTL.bit.PHSEN = TB_ENABLE;        // Disable phase loading
   EPwm6Regs.TBCTL.bit.PHSDIR = TB_DOWN;
   EPwm6Regs.TBCTL.bit.PRDLD = TB_SHADOW;
   EPwm6Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN; // sync flow-through
   EPwm6Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
   EPwm6Regs.TBCTL.bit.CLKDIV = TB_DIV1;

   // Setup shadowing
   EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
   EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;  // Load on Zero
   EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;


   // Set actions
   EPwm6Regs.AQCTLA.bit.CAD = AQ_SET;             // Set PWM1A on event A, up count
   EPwm6Regs.AQCTLA.bit.CAU = AQ_CLEAR;           // Clear PWM1A on event A, down count
//   EPwm2Regs.AQCTLA.bit.ZRO = AQ_SET;             // Set PWM1A on event A, up count

//   EPwm2Regs.AQCTLB.bit.ZRO = AQ_SET;           // Clear PWM1B on event B, up count
//   EPwm2Regs.AQCTLB.bit.CAU = AQ_CLEAR;          	  // Set PWM1B on event B, down count


   // Interrupt where we will change the Compare Values
//   EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
//   EPwm2Regs.ETSEL.bit.INTEN = 1;                // Enable INT
//   EPwm2Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event

   // Set Dead-band
   EPwm6Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;	// enable Dead-band module
   EPwm6Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; 		// Active Hi complementary
   EPwm6Regs.DBFED.all = 80; 							// FED = 100 TBCLKs initially
   EPwm6Regs.DBRED.all = 80;							// RED = 100 TBCLKs initially

   // Information this example uses to keep track
   // of the direction the CMPA/CMPB values are
   // moving, the min and max allowed values and
   // a pointer to the correct ePWM registers
/*
   epwm2_info.EPwm_CMPA_Direction = EPWM_CMP_UP;   // Start by increasing CMPA &
   epwm2_info.EPwm_CMPB_Direction = EPWM_CMP_DOWN;   // decreasing CMPB
   epwm2_info.EPwmTimerIntCount = 0;               // Zero the interrupt counter
   epwm2_info.EPwmRegHandle = &EPwm2Regs;          // Set the pointer to the ePWM module
   epwm2_info.EPwmMaxCMPA = EPWM2_MAX_CMPA;        // Setup min/max CMPA/CMPB values
   epwm2_info.EPwmMinCMPA = EPWM2_MIN_CMPA;
   epwm2_info.EPwmMaxCMPB = EPWM2_MAX_CMPB;
   epwm2_info.EPwmMinCMPB = EPWM2_MIN_CMPB;
*/
}

void InitEPwm7()
{

	   // Setup TBCLK(10nS)
	   EPwm7Regs.TBPRD = EPWM2_TIMER_TBPRD;           // Set timer period 5000 TBCLKs
	   EPwm7Regs.TBPHS.bit.TBPHS = 1333.33;           // Phase is 0
	   EPwm7Regs.TBCTR = 0x0000;                      // Clear counter

	   // Set Compare values
	   EPwm7Regs.CMPA.bit.CMPA = EPWM2_MIN_CMPA;     // Set compare A value
	   EPwm7Regs.CMPB.bit.CMPB = EPWM2_MIN_CMPB;               // Set Compare B value

	   // Setup counter mode
	   EPwm7Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count updown
	   EPwm7Regs.TBCTL.bit.PHSEN = TB_ENABLE;         // Disable phase loading
	   EPwm7Regs.TBCTL.bit.PHSDIR = TB_UP;
	   EPwm7Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	   EPwm7Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;     // sync flow-through
	   EPwm7Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
	   EPwm7Regs.TBCTL.bit.CLKDIV = TB_DIV1;

	   // Setup shadowing
	   EPwm7Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	   EPwm7Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	   EPwm7Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;  // Load on Zero
	   EPwm7Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;


	   // Set actions
	   EPwm7Regs.AQCTLA.bit.CAD = AQ_SET;             // Set PWM1A on event A, up count
	   EPwm7Regs.AQCTLA.bit.CAU = AQ_CLEAR;           // Clear PWM1A on event A, down count
//      EPwm3Regs.AQCTLA.bit.ZRO = AQ_SET;             // Set PWM1A on event A, up count

//      EPwm32Regs.AQCTLB.bit.ZRO = AQ_SET;           // Clear PWM1B on event B, up count
//      EPwm3Regs.AQCTLB.bit.CAU = AQ_CLEAR;          	  // Set PWM1B on event B, down count


	   // Interrupt where we will change the Compare Values
//      EPwm3Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
//      EPwm3Regs.ETSEL.bit.INTEN = 1;                // Enable INT
//      EPwm3Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event

	   // Set Dead-band
	   EPwm7Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;	// enable Dead-band module
	   EPwm7Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; 		// Active Hi complementary
	   EPwm7Regs.DBFED.all = 80; 							// FED = 100 TBCLKs initially
	   EPwm7Regs.DBRED.all = 80;							// RED = 100 TBCLKs initially

}
