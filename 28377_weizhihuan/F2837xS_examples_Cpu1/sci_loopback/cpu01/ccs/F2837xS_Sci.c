//###########################################################################
//
// FILE:   F2837xS_Sci.c
//
// TITLE:  F2837xS SCI Initialization & Support Functions.
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
//#include "absolute_encoder.h"   // absolute_encoder Definitions

unsigned int rdataA[64];    						// Receive Buffer for SCI-A
unsigned int rdataA_head = 0, rdataA_tail = 0; 		// Used for checking the received data
unsigned int tdataA[16];							// Transfer Buffer for SCI-A

void InitSciGpio()
{
   InitSciaGpio();
//   InitScibGpio();
//   InitScicGpio();
//   InitScidGpio();
}

void InitSci()
{
   InitScia();
//   InitScib();
//   InitScic();
//   InitScid();
}

void InitSciaGpio()
{
//    GPIO_SetupPinMux(9, GPIO_MUX_CPU1, 6);
//	  GPIO_SetupPinOptions(9, GPIO_INPUT, GPIO_PUSHPULL);
//	  GPIO_SetupPinMux(8, GPIO_MUX_CPU1, 6);
//    GPIO_SetupPinOptions(8, GPIO_OUTPUT, GPIO_ASYNC);

  EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPCPUD.bit.GPIO85 = 0;    // Enable pull-up for GPIO9 (SCIRXDA)
	GpioCtrlRegs.GPCPUD.bit.GPIO84 = 0;	  // Enable pull-up for GPIO8 (SCITXDA)
//	GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up for GPIO9 (SCIRXDA)
//	GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;	  // Enable pull-up for GPIO8 (SCITXDA)

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPCQSEL2.bit.GPIO85 = 3;  // Asynch input GPIO9 (SCIRXDA)
//	GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO9 (SCIRXDA)

/* Configure SPI-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SPI functional pins.
// Comment out other unwanted lines.

	GPIO_SetupPinMux(85, GPIO_MUX_CPU1, 5);
	GPIO_SetupPinMux(84, GPIO_MUX_CPU1, 5);
//	GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 6;   // Configure GPIO9 for SCIRXDA operation
//	GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 6;   // Configure GPIO8 for SCITXDA operation
//	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;   // Configure GPIO9 for SCIRXDA operation
//	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;   // Configure GPIO8 for SCITXDA operation

//    EDIS;
}

void InitScibGpio()
{

   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;    // Enable pull-up for GPIO23 (SCIRXDB)
	GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;	  // Enable pull-up for GPIO22 (SCITXDB)

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;  // Asynch input GPIO23 (SCIRXDB)

/* Configure SPI-B pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SPI functional pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 3;   // Configure GPIO23 for SCIRXDB operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 3;   // Configure GPIO22 for SCITXDB operation

    EDIS;
}

void InitScicGpio()
{

   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPCPUD.bit.GPIO73 = 0;    // Enable pull-up for GPIO73 (SCIRXDC)
	GpioCtrlRegs.GPCPUD.bit.GPIO72 = 0;	  // Enable pull-up for GPIO72 (SCITXDC)

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPCQSEL1.bit.GPIO73 = 3;  // Asynch input GPIO73 (SCIRXDC)

/* Configure SPI-C pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SPI functional pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPCMUX1.bit.GPIO73 = 6;   // Configure GPIO73 for SCIRXDC operation
	GpioCtrlRegs.GPCMUX1.bit.GPIO72 = 6;   // Configure GPIO72 for SCITXDC operation

    EDIS;
}

void InitScidGpio()
{

   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPBPUD.bit.GPIO46 = 0;    // Enable pull-up for GPIO46 (SCIRXDD)
	GpioCtrlRegs.GPBPUD.bit.GPIO47 = 0;	  // Enable pull-up for GPIO47 (SCITXDD)

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPBQSEL1.bit.GPIO46 = 3;  // Asynch input GPIO46 (SCIRXDD)

/* Configure SPI-D pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SPI functional pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPBMUX1.bit.GPIO46 = 6;   // Configure GPIO46 for SCIRXDD operation
	GpioCtrlRegs.GPBMUX1.bit.GPIO47 = 6;   // Configure GPIO47 for SCITXDD operation

    EDIS;
}

//---------------------------------------------------------------------------
// InitSci:
//---------------------------------------------------------------------------
// This function initializes the SCI(s) to a known state.
//
void InitScia(void)
{
// Initialize SCI-A:
	static int i = 0;
// Initialize SCI-A:
	SciaRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
	                               // No parity,8 char bits,
	                               // async mode, idle-line protocol
	SciaRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
	                                  // Disable RX ERR, SLEEP, TXWAKE
    SciaRegs.SCICTL2.bit.TXINTENA =1;
    SciaRegs.SCICTL2.bit.RXBKINTENA =1;
	SciaRegs.SCIHBAUD.all = 0x0000;
    SciaRegs.SCILBAUD.all = SCI_PRD;
//  SciaRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back
	SciaRegs.SCIFFTX.all=0xC020;
	SciaRegs.SCIFFRX.all=0x0028;
	SciaRegs.SCIFFCT.all=0x00;

	SciaRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset
	SciaRegs.SCIFFTX.bit.TXFIFORESET=1;
	SciaRegs.SCIFFRX.bit.RXFIFORESET=1;

// Initialize frame data
	tdataA[0] = 0xEB;
	tdataA[1] = 0x90;
	tdataA[2] = 0x66;
	for(i=3;i<15;i++)
		tdataA[i] = 0;

	TX_FRAME.SYNC[0] = 0xEB;
	TX_FRAME.SYNC[1] = 0x90;
	TX_FRAME.FUNC = 0x66;
	for(i=0;i<12;i++)
		TX_FRAME.DATA[i] = 0;

	RX_FRAME.FUNC = 0x66;
}

void SCIA_RX(void)
{
	static unsigned int rdata_n = 0;
	static unsigned int rdata_length = 0;
	static unsigned int CheckSum = 0;

	if( rdataA_tail >= rdataA_head )
		rdata_length = rdataA_tail - rdataA_head;
	else
		rdata_length = rdataA_tail + 64 - rdataA_head;

	// 如果接收缓存数据数有8个或更多，则可能有一帧完整数据
	if( rdata_length >= 8 )
	{
		// 如果接收缓存数据有超过15字节,正常情况下应有一帧完整数据，直接从倒数第15字节开始搜寻最新帧
		if( rdata_length > 15 )
		{
			rdataA_head = rdataA_head + rdata_length -15;
			rdataA_head = rdataA_head & 0x3F;
			rdata_length = 15;
		}

		while( rdata_length >= 8 )
		{
			// 如果当前数据和同步帧数据吻合，则判断帧信息是否完整
			if( rdataA[rdataA_head] == 0xEB && rdataA[( rdataA_head + 1 ) & 0x3F] == 0x90 )
			{
				CheckSum = 0;
				for( rdata_n = 2; rdata_n < 8; rdata_n++ )
					CheckSum = CheckSum + (rdataA[(rdataA_head + rdata_n) & 0x3F] & 0x00FF);

				if( ( CheckSum & 0x00FF ) == 0 )
				{
					RX_FRAME.FUNC = rdataA[( rdataA_head + 2 ) & 0x3F];
					for(rdata_n = 0; rdata_n < 4; rdata_n++)
						RX_FRAME.DATA[rdata_n] = rdataA[(rdataA_head + rdata_n + 3) & 0x3F];

					// 获取转速给定数据
					McomData.speed_cmd = (unsigned int)((RX_FRAME.DATA[1] & 0x00FF ) << 8 | (RX_FRAME.DATA[2] & 0x00FF));

					// 接收标志置位
					RX_flag = 1;

					rdataA_head = rdataA_head + 8;
					rdataA_head = rdataA_head & 0x3F;
					rdata_length = rdata_length - 8;
				}
				else
				{
					rdataA_head = rdataA_head + 1;
					rdataA_head = rdataA_head & 0x3F;
					rdata_length = rdata_length - 1;
				}
			}
			else
			{
				rdataA_head = rdataA_head + 1;
				rdataA_head = rdataA_head & 0x3F;
				rdata_length = rdata_length - 1;
			}
		}
	}
}

void SCIA_TX(void)
{
	static unsigned int i_send,CheckSum=0;

	switch(RX_FRAME.FUNC)
	{
			case 0x66:
//				McomData.speed_fbk   = (unsigned)( jiaodu * 57.29577951 * 2 );
//				McomData.speed_fbk   = (unsigned)( theta_encoder * 2 );
//				McomData.speed_fbk   = (unsigned)( theta_dq * 2 );
//				McomData.speed_fbk   = (unsigned)( turns * 2 );
				McomData.speed_fbk   = (unsigned)( omega * 2 );
				McomData.sys_voltage = (unsigned)( U_sci * 8 );
				McomData.sys_current = (unsigned)( I_main * 16 );
//				McomData.sys_current = (unsigned)( turns * 16 );
				McomData.PhA_current = (unsigned)( IA * 64 );
				McomData.PhB_current = (unsigned)( IB * 64 );
				McomData.PhC_current = (unsigned)( IC * 64 );
				McomData.PhD_current = (unsigned)( ID * 64 );

				TX_FRAME.FUNC = 0x66;
				TX_FRAME.DATA[0]  =  McomData.frame_cnt & 0x00FF ;
				TX_FRAME.DATA[1]  = (McomData.speed_fbk & 0xFF00) >> 8;
				TX_FRAME.DATA[2]  =  McomData.speed_fbk & 0x00FF ;
				TX_FRAME.DATA[3]  = (McomData.sys_voltage & 0x0FF0) >> 4 ;
				TX_FRAME.DATA[4]  = (McomData.sys_voltage & 0x000F) << 4 | (McomData.sys_current & 0x0F00) >> 8;
				TX_FRAME.DATA[5]  =  McomData.sys_current & 0x00FF;
				TX_FRAME.DATA[6]  = (McomData.PhA_current & 0x0FF0) >> 4 ;
				TX_FRAME.DATA[7]  = (McomData.PhA_current & 0x000F) << 4 | (McomData.PhB_current & 0x0F00) >> 8;
				TX_FRAME.DATA[8]  =  McomData.PhB_current & 0x00FF;
				TX_FRAME.DATA[9]  = (McomData.PhC_current & 0x0FF0) >> 4 ;
				TX_FRAME.DATA[10] = (McomData.PhC_current & 0x000F) << 4 | (McomData.PhD_current & 0x0F00) >> 8;
				TX_FRAME.DATA[11] =  McomData.PhD_current & 0x00FF;
				break;

			case 0x63:
				McomData.speed_fbk    = (unsigned)( omega * 2 );
				McomData.sys_voltage  = (unsigned)( U_sci * 8 );
				McomData.sys_current  = (unsigned)( I_main * 16 );
				McomData.PhAC_current = (unsigned)( ( IA - IC ) * 32 );
				McomData.PhBD_current = (unsigned)( ( IB - ID ) * 32 );
				McomData.sys_status   = 0;

				TX_FRAME.FUNC = 0x63;
				TX_FRAME.DATA[0]  =  McomData.frame_cnt & 0x00FF ;
				TX_FRAME.DATA[1]  = (McomData.speed_cmd & 0xFF00) >> 8;
				TX_FRAME.DATA[2]  =  McomData.speed_cmd & 0x00FF ;
				TX_FRAME.DATA[3]  = (McomData.speed_fbk & 0xFF00) >> 8;
				TX_FRAME.DATA[4]  =  McomData.speed_fbk & 0x00FF ;
				TX_FRAME.DATA[5]  = (McomData.sys_voltage & 0x0FF0) >> 4 ;
				TX_FRAME.DATA[6]  = (McomData.sys_voltage & 0x000F) << 4 | (McomData.sys_current & 0x0F00) >> 8;
				TX_FRAME.DATA[7]  =  McomData.sys_current & 0x00FF;
				TX_FRAME.DATA[8]  = (McomData.PhAC_current & 0x0FF0) >> 4 ;
				TX_FRAME.DATA[9]  = (McomData.PhAC_current & 0x000F) << 4 | (McomData.PhBD_current & 0x0F00) >> 8;
				TX_FRAME.DATA[10] =  McomData.PhBD_current & 0x00FF;
				TX_FRAME.DATA[11] =  McomData.sys_status & 0x00FF;
				break;

			case 0xC3:
				McomData.speed_fbk   = (unsigned)( omega * 2 );
				McomData.PhA_current = (unsigned)( IA * 1024 );
				McomData.PhB_current = (unsigned)( IB * 1024 );
				McomData.PhC_current = (unsigned)( IC * 1024 );
				McomData.PhD_current = (unsigned)( ID * 1024 );
				McomData.sys_status  = 0;

				TX_FRAME.FUNC = 0xC3;
				TX_FRAME.DATA[0]  =  McomData.frame_cnt & 0x00FF;
				TX_FRAME.DATA[1]  = (McomData.speed_fbk & 0xFF00) >> 8;
				TX_FRAME.DATA[2]  =  McomData.speed_fbk & 0x00FF;
				TX_FRAME.DATA[3]  = (McomData.PhA_current & 0xFF00) >> 8;
				TX_FRAME.DATA[4]  =  McomData.PhA_current & 0x00FF;
				TX_FRAME.DATA[5]  = (McomData.PhB_current & 0xFF00) >> 8;
				TX_FRAME.DATA[6]  =  McomData.PhB_current & 0x00FF;
				TX_FRAME.DATA[7]  = (McomData.PhC_current & 0xFF00) >> 8;
				TX_FRAME.DATA[8]  =  McomData.PhC_current & 0x00FF;
				TX_FRAME.DATA[9]  = (McomData.PhD_current & 0xFF00) >> 8;
				TX_FRAME.DATA[10] =  McomData.PhD_current & 0x00FF;
				TX_FRAME.DATA[11] =  McomData.sys_status & 0x00FF;
				break;
			case 0xC6:
				McomData.speed_fbk    = (unsigned)( omega * 2 );
				McomData.sys_voltage  = (unsigned)( U_sci * 128 );
				McomData.sys_current  = (unsigned)( I_main * 1024 );
				McomData.PhAC_current = (unsigned)( ( IA + IC ) * 1024 );
				McomData.PhBD_current = (unsigned)( ( IB + ID ) * 1024 );
				McomData.sys_status   = 0;

				TX_FRAME.FUNC = 0xC6;
				TX_FRAME.DATA[0]  =  McomData.frame_cnt & 0x00FF;
				TX_FRAME.DATA[1]  = (McomData.speed_fbk & 0xFF00) >> 8;
				TX_FRAME.DATA[2]  =  McomData.speed_fbk & 0x00FF;
				TX_FRAME.DATA[3]  = (McomData.sys_voltage & 0xFF00) >> 8;
				TX_FRAME.DATA[4]  =  McomData.sys_voltage & 0x00FF;
				TX_FRAME.DATA[5]  = (McomData.sys_current & 0xFF00) >> 8;
				TX_FRAME.DATA[6]  =  McomData.sys_current & 0x00FF;
				TX_FRAME.DATA[7]  = (McomData.PhAC_current & 0xFF00) >> 8;
				TX_FRAME.DATA[8]  =  McomData.PhAC_current & 0x00FF;
				TX_FRAME.DATA[9]  = (McomData.PhBD_current & 0xFF00) >> 8;
				TX_FRAME.DATA[10] =  McomData.PhBD_current & 0x00FF;
				TX_FRAME.DATA[11] =  McomData.sys_status & 0x00FF;
				break;

/*			case 0xC9:
				McomData.speed_fbk = (unsigned)( omega * 2 );
				McomData.IA_d = (unsigned)( ia_d * 512 );
				McomData.IA_q = (unsigned)( ia_q * 512 );
				McomData.IB_d = (unsigned)( ib_d * 512 );
				McomData.IB_q = (unsigned)( ib_q * 512 );
				McomData.sys_status = 0;

				TX_FRAME.FUNC = 0xC9;
				TX_FRAME.DATA[0]  =  McomData.frame_cnt & 0x00FF;
				TX_FRAME.DATA[1]  = (McomData.speed_fbk & 0xFF00) >> 8;
				TX_FRAME.DATA[2]  =  McomData.speed_fbk & 0x00FF;
				TX_FRAME.DATA[3]  = (McomData.IA_d & 0xFF00) >> 8;
				TX_FRAME.DATA[4]  =  McomData.IA_d & 0x00FF;
				TX_FRAME.DATA[5]  = (McomData.IA_q & 0xFF00) >> 8;
				TX_FRAME.DATA[6]  =  McomData.IA_q & 0x00FF;
				TX_FRAME.DATA[7]  = (McomData.IB_d & 0xFF00) >> 8;
				TX_FRAME.DATA[8]  =  McomData.IB_d & 0x00FF;
				TX_FRAME.DATA[9]  = (McomData.IB_q & 0xFF00) >> 8;
				TX_FRAME.DATA[10] =  McomData.IB_q & 0x00FF;
				TX_FRAME.DATA[11] =  McomData.sys_status & 0x00FF;
				break;

			case 0xCC:
				McomData.speed_fbk = (unsigned)( omega * 2 );
				McomData.IC_d = (unsigned)( ic_d * 512 );
				McomData.IC_q = (unsigned)( ic_q * 512 );
				McomData.ID_d = (unsigned)( id_d * 512 );
				McomData.ID_q = (unsigned)( id_q * 512 );
				McomData.sys_status = 0;

				TX_FRAME.FUNC = 0xCC;
				TX_FRAME.DATA[0]  =  McomData.frame_cnt & 0x00FF;
				TX_FRAME.DATA[1]  = (McomData.speed_fbk & 0xFF00) >> 8;
				TX_FRAME.DATA[2]  =  McomData.speed_fbk & 0x00FF;
				TX_FRAME.DATA[3]  = (McomData.IC_d & 0xFF00) >> 8;
				TX_FRAME.DATA[4]  =  McomData.IC_d & 0x00FF;
				TX_FRAME.DATA[5]  = (McomData.IC_q & 0xFF00) >> 8;
				TX_FRAME.DATA[6]  =  McomData.IC_q & 0x00FF;
				TX_FRAME.DATA[7]  = (McomData.ID_d & 0xFF00) >> 8;
				TX_FRAME.DATA[8]  =  McomData.ID_d & 0x00FF;
				TX_FRAME.DATA[9]  = (McomData.ID_q & 0xFF00) >> 8;
				TX_FRAME.DATA[10] =  McomData.ID_q & 0x00FF;
				TX_FRAME.DATA[11] =  McomData.sys_status & 0x00FF;
				break;
*/
			case 0x33:
				McomData.sys_status = Hall_status;

				TX_FRAME.FUNC = 0x33;
				TX_FRAME.DATA[0]  = (unsigned int)(( McomData.frame_cnt & 0x003F ) | (McomData.sys_status & 0x0300) >> 2);
				TX_FRAME.DATA[1]  = (unsigned int)( McomData.I_PhA[0] & 0x00FF ) ;
				TX_FRAME.DATA[2]  = (unsigned int)( McomData.I_PhA[1] & 0x00FF ) ;
				TX_FRAME.DATA[3]  = (unsigned int)( McomData.I_PhA[2] & 0x00FF ) ;
				TX_FRAME.DATA[4]  = (unsigned int)( McomData.I_PhA[3] & 0x00FF ) ;
				TX_FRAME.DATA[5]  = (unsigned int)( McomData.I_PhA[4] & 0x00FF ) ;
				TX_FRAME.DATA[6]  = (unsigned int)( McomData.I_PhA[5] & 0x00FF ) ;
				TX_FRAME.DATA[7]  = (unsigned int)( McomData.I_PhA[6] & 0x00FF ) ;
				TX_FRAME.DATA[8]  = (unsigned int)( McomData.I_PhA[7] & 0x00FF ) ;
				TX_FRAME.DATA[9]  = (unsigned int)( McomData.I_PhA[8] & 0x00FF ) ;
				TX_FRAME.DATA[10] = (unsigned int)( McomData.I_PhA[9] & 0x00FF ) ;
				TX_FRAME.DATA[11] = (unsigned int)( McomData.sys_status & 0x00FF);
				break;

			case 0x36:
				McomData.sys_status = Hall_status;

				TX_FRAME.FUNC = 0x36;
				TX_FRAME.DATA[0]  = (unsigned int)(( McomData.frame_cnt & 0x003F ) | (McomData.sys_status & 0x0300) >> 2);
				TX_FRAME.DATA[1]  = (unsigned int)( McomData.I_PhB[0] & 0x00FF ) ;
				TX_FRAME.DATA[2]  = (unsigned int)( McomData.I_PhB[1] & 0x00FF ) ;
				TX_FRAME.DATA[3]  = (unsigned int)( McomData.I_PhB[2] & 0x00FF ) ;
				TX_FRAME.DATA[4]  = (unsigned int)( McomData.I_PhB[3] & 0x00FF ) ;
				TX_FRAME.DATA[5]  = (unsigned int)( McomData.I_PhB[4] & 0x00FF ) ;
				TX_FRAME.DATA[6]  = (unsigned int)( McomData.I_PhB[5] & 0x00FF ) ;
				TX_FRAME.DATA[7]  = (unsigned int)( McomData.I_PhB[6] & 0x00FF ) ;
				TX_FRAME.DATA[8]  = (unsigned int)( McomData.I_PhB[7] & 0x00FF ) ;
				TX_FRAME.DATA[9]  = (unsigned int)( McomData.I_PhB[8] & 0x00FF ) ;
				TX_FRAME.DATA[10] = (unsigned int)( McomData.I_PhB[9] & 0x00FF ) ;
				TX_FRAME.DATA[11] = (unsigned int)( McomData.sys_status & 0x00FF);
				break;

			case 0x39:
				McomData.sys_status = Hall_status;

				TX_FRAME.FUNC = 0x39;
				TX_FRAME.DATA[0]  = (unsigned int)(( McomData.frame_cnt & 0x003F ) | (McomData.sys_status & 0x0300) >> 2);
				TX_FRAME.DATA[1]  = (unsigned int)( McomData.I_PhC[0] & 0x00FF ) ;
				TX_FRAME.DATA[2]  = (unsigned int)( McomData.I_PhC[1] & 0x00FF ) ;
				TX_FRAME.DATA[3]  = (unsigned int)( McomData.I_PhC[2] & 0x00FF ) ;
				TX_FRAME.DATA[4]  = (unsigned int)( McomData.I_PhC[3] & 0x00FF ) ;
				TX_FRAME.DATA[5]  = (unsigned int)( McomData.I_PhC[4] & 0x00FF ) ;
				TX_FRAME.DATA[6]  = (unsigned int)( McomData.I_PhC[5] & 0x00FF ) ;
				TX_FRAME.DATA[7] = (unsigned int)( McomData.I_PhC[6] & 0x00FF ) ;
				TX_FRAME.DATA[8]  = (unsigned int)( McomData.I_PhC[7] & 0x00FF ) ;
				TX_FRAME.DATA[9]  = (unsigned int)( McomData.I_PhC[8] & 0x00FF ) ;
				TX_FRAME.DATA[10] = (unsigned int)( McomData.I_PhC[9] & 0x00FF ) ;
				TX_FRAME.DATA[11] = (unsigned int)( McomData.sys_status & 0x00FF);
				break;

			case 0x3C:
				McomData.sys_status = Hall_status;

				TX_FRAME.FUNC = 0x3C;
				TX_FRAME.DATA[0]  = (unsigned int)(( McomData.frame_cnt & 0x003F ) | (McomData.sys_status & 0x0300) >> 2);
				TX_FRAME.DATA[1]  = (unsigned int)( McomData.I_PhD[0] & 0x00FF ) ;
				TX_FRAME.DATA[2]  = (unsigned int)( McomData.I_PhD[1] & 0x00FF ) ;
				TX_FRAME.DATA[3]  = (unsigned int)( McomData.I_PhD[2] & 0x00FF ) ;
				TX_FRAME.DATA[4]  = (unsigned int)( McomData.I_PhD[3] & 0x00FF ) ;
				TX_FRAME.DATA[5]  = (unsigned int)( McomData.I_PhD[4] & 0x00FF ) ;
				TX_FRAME.DATA[6]  = (unsigned int)( McomData.I_PhD[5] & 0x00FF ) ;
				TX_FRAME.DATA[7]  = (unsigned int)( McomData.I_PhD[6] & 0x00FF ) ;
				TX_FRAME.DATA[8]  = (unsigned int)( McomData.I_PhD[7] & 0x00FF ) ;
				TX_FRAME.DATA[9]  = (unsigned int)( McomData.I_PhD[8] & 0x00FF ) ;
				TX_FRAME.DATA[10] = (unsigned int)( McomData.I_PhD[9] & 0x00FF ) ;
				TX_FRAME.DATA[11] = (unsigned int)( McomData.sys_status & 0x00FF);
				break;

			default:
				break;
	}

	//Count checksum
	CheckSum = TX_FRAME.FUNC;
	for(i_send=0; i_send<12; i_send++)
		CheckSum += TX_FRAME.DATA[i_send];
	TX_FRAME.CSUM = 0x0100 - (CheckSum & 0x00FF);

	SciaRegs.SCIFFTX.bit.TXFIFORESET = 1;
	SciaRegs.SCIFFTX.bit.TXFFINTCLR = 1;					// Clear SCI Interrupt Flag 清串口发送中断标志，发送
}

//===========================================================================
// End of file.
//===========================================================================
