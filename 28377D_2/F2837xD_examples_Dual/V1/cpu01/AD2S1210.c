/********************************************************************************
 Author : Si Binqiang 

 Date :   Febrary, 2013

 File name :   AD2S1210.c

 Description :	 Use the GPIO to simulate the SPI communication of AD2S1210

 Hardware plateform : 	TMS320F2808 and AD2S1210
********************************************************************************/

#include "F2837xS_device.h"     // F2837xS Headerfile Include File
#include "F2837xS_Examples.h"   // F2837xS Examples Include File
#include "Common_Definitions.h"     // Common Definitions Headerfile Include File
#include "AD2S1210.h"

// add the c file here

//---------------------------------
//void WriteToAD2S(unsigned char count,unsigned char *buf);
//---------------------------------
//Function that writes to the AD2S via the SPI port. 

void delayns ( unsigned long num )
{
	static unsigned long k = 0;
	for(k = num; k > 0; k--)
	{
		asm("NOP");
	}
}

void InitAD2S1210(void)
{
//RESET->0 initially  

	CLR_RESET();
	delayns(1);
	SET_SPL();
		
	delayns(1);
	SET_RESET(); 	

	delayns(355555);	//20.912ms

	CLR_SPL();	
		
	delayns(1);
	SET_SPL();
}

void AD2S1210SelectMode(unsigned int mode)
{
	if (mode==POSITION)
	{
		CLR_A0();
		delayns(1);
		CLR_A1();
		delayns(1);		//Normal Mode position output
	}
	else if (mode==VELOCITY)
	{
		CLR_A0();
		delayns(1);
		SET_A1();
		delayns(1);		//Normal Mode velocity output
	}
	else if (mode==CONFIG)
	{
		SET_A0();
		delayns(1);
		SET_A1();
		delayns(1);		//Configuration Mode
	}
}

void WriteToAD2S1210(unsigned int addr, unsigned int data)
{
	
	SET_WR();
	delayns(1);
	CLR_WR();

	//write control register address
	SpiaRegs.SPITXBUF = addr;
	delayns(72);

	SET_WR();
	delayns(18);
	CLR_WR();

	//write control register data
	SpiaRegs.SPITXBUF = data;
	delayns(72);
	SET_WR();
	
}

void AD2S1210SoftReset(void)
{
	unsigned int buf =	SOFTRESET;
	SpiaRegs.SPITXBUF= buf;	  	//soft reset
	delayns(18);
}

void ReadFromAD2S1210(unsigned int mode, unsigned int address, unsigned int * buf)
{
	static unsigned int j;
	
	if (mode==CONFIG)
	{
		SET_WR();
		delayns(1);
		CLR_WR();
		
		//write control register address
		while(SpiaRegs.SPISTS.bit.BUFFULL_FLAG != 0);		//等待发送缓冲器为空
		SpiaRegs.SPITXBUF = address;

		//read 1-byte register		
		while(SpiaRegs.SPISTS.bit.INT_FLAG != 1);			// 等待接收缓冲器为满
		buf[0] = SpiaRegs.SPIRXBUF;							// 接收数据
		SET_WR();			
		//read 1-byte register
	}
	else if (mode==POSITION||mode==VELOCITY)
	{

		CLR_SPL();
		delayns(4);//8
		CLR_WR();

		for(j=0;j<3;j++)
		{
			//send clock
			while(SpiaRegs.SPISTS.bit.BUFFULL_FLAG != 0);	// 等待发送缓冲器为空
			SpiaRegs.SPITXBUF = address;
			
			//read 1st-byte register			
			while(SpiaRegs.SPISTS.bit.INT_FLAG != 1);		// 等待接收缓冲器为满
			buf[j] = SpiaRegs.SPIRXBUF;						// 接收数据
		}
	
		SET_WR();
//		delayns(1);
		SET_SPL();	

	}
}

/*
void AD2S1210FaultClear(void)
{
	unsigned int buf;	
	
	SET_SPL();
	delayns(2);
	CLR_SPL();	
	delayns(4); //hold 6*tck+20ns
	
	AD2S1210SelectMode(CONFIG); 
	ReadFromAD2S1210(CONFIG,FAULT,&buf);		//read fault register		
	
	SET_SPL();
	delayns(2);
	CLR_SPL();
	delayns(2); //hold 2*tck+20ns
		
}
*/

//===========================================================================
// End of file.
//===========================================================================
