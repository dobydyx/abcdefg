/********************************************************************************
 Author : CAST (China Applications Support Team) 

 Date :   April, 2011

 File name :   AD2S1210.h

 Description :	 Use the GPIO to simulate the SPI communication of AD2S1210

 Hardware plateform : 	ADuC7026 and AD2S1210
********************************************************************************/

#ifndef AD2S1210_H
#define AD2S1210_H

// add the header file here

//Mode Select
#define POSITION	0
#define	VELOCITY	1
#define CONFIG		2  


//Register Map 
#define	POSITIONMSB		0x8000	
#define	POSITIONLSB		0x8100
#define	VELOCITYMSB		0x8200
#define	VELOCITYLSB		0x8300
#define	LOSTHRES		0x8800	//Loss of Signal
#define	DOSORTHRES		0x8900	//degradation of Signal
#define	DOSMISTHRES		0x8A00
#define	DOSRSTMXTHRES	0x8B00
#define	DOSRSTMITHRES	0x8C00
#define	LOTHITHRES		0x8D00	//Loss of Tracking
#define	LOTLOTHRES		0x8E00
#define	EXFREQUENCY		0x9100	//Excitation Frequency
#define	CONTROL_RDC		0x9200
#define	SOFTRESET		0xF000
#define	FAULT			0xFF00
#define POS_VEL			0x5500  //void register for normal read address


//GPIO Define
//#define SET_CS()		GpioDataRegs.GPBDAT.bit.GPIO33 = 1		//GPIO33->CS
//#define CLR_CS()		GpioDataRegs.GPBDAT.bit.GPIO33 = 0
/*
#define	SET_SCLK()		GpioDataRegs.GPADAT.bit.GPIO14 = 1		//GPIO14->SCLK
#define	CLR_SCLK()		GpioDataRegs.GPADAT.bit.GPIO14 = 0

#define SET_SDI()		GpioDataRegs.GPADAT.bit.GPIO12 = 1		//GPIO12->SDI
#define CLR_SDI()		GpioDataRegs.GPADAT.bit.GPIO12 = 0
*/
//#define	SET_A0()		GpioDataRegs.GPBSET.bit.GPIO43 = 1		//GPIO25->A0_A
//#define	CLR_A0()		GpioDataRegs.GPBCLEAR.bit.GPIO43 = 1
//
//#define SET_A1()		GpioDataRegs.GPBSET.bit.GPIO42 = 1		//GPIO50->A1_A
//#define CLR_A1()		GpioDataRegs.GPBCLEAR.bit.GPIO42 = 1
//
//#define SET_WR()		GpioDataRegs.GPBSET.bit.GPIO40 = 1      //GpioDataRegs.GPBDAT.bit.GPIO40 = 1		//GPIO52->WR_A
//#define CLR_WR()		GpioDataRegs.GPBCLEAR.bit.GPIO40 = 1    //GpioDataRegs.GPBDAT.bit.GPIO40 = 0
//
//#define SET_SPL()		GpioDataRegs.GPBSET.bit.GPIO41 = 1		//GPIO8->SPL_A
//#define CLR_SPL()		GpioDataRegs.GPBCLEAR.bit.GPIO41 = 1
//
//#define SET_RESET()		GpioDataRegs.GPBSET.bit.GPIO47 = 1		//GPIO12->RESET_A
//#define CLR_RESET()		GpioDataRegs.GPBCLEAR.bit.GPIO47 = 1

#define	SET_A0()		GpioDataRegs.GPBDAT.bit.GPIO43 = 1		//GPIO25->A0_A
#define	CLR_A0()		GpioDataRegs.GPBDAT.bit.GPIO43 = 0

#define SET_A1()		GpioDataRegs.GPBDAT.bit.GPIO42 = 1		//GPIO50->A1_A
#define CLR_A1()		GpioDataRegs.GPBDAT.bit.GPIO42 = 0

#define SET_WR()		GpioDataRegs.GPBSET.bit.GPIO40 = 1      //GpioDataRegs.GPBDAT.bit.GPIO40 = 1		//GPIO52->WR_A
#define CLR_WR()		GpioDataRegs.GPBCLEAR.bit.GPIO40 = 1    //GpioDataRegs.GPBDAT.bit.GPIO40 = 0

#define SET_SPL()		GpioDataRegs.GPBDAT.bit.GPIO41 = 1		//GPIO8->SPL_A
#define CLR_SPL()		GpioDataRegs.GPBDAT.bit.GPIO41 = 0

#define SET_RESET()		GpioDataRegs.GPBDAT.bit.GPIO47 = 1		//GPIO12->RESET_A
#define CLR_RESET()		GpioDataRegs.GPBDAT.bit.GPIO47 = 0


//#define SET_RD()		GpioDataRegs.GPBDAT.bit.GPIO50 = 1		//GPIO9->RD_A  for parallel
//#define CLR_RD()		GpioDataRegs.GPBDAT.bit.GPIO50 = 0
 


extern void InitAD2S1210(void);
extern void AD2S1210SelectMode(unsigned int mode);
extern void AD2S1210SoftReset(void);
extern void WriteToAD2S1210(unsigned int address, unsigned int data);
extern void ReadFromAD2S1210(unsigned int mode, unsigned int address, unsigned int * buf);
extern void delayns(unsigned long num);
extern void AD2S1210FaultClear(void);

#endif



//===========================================================================
// End of file.
//===========================================================================
