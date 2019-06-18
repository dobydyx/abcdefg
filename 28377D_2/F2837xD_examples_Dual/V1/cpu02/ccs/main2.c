//###########################################################################
// $TI Release: F2837xD Support Library v190 $
// $Release Date: Mon Feb  1 16:51:57 CST 2016 $
// $Copyright: Copyright (C) 2013-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "F28x_Project.h"     // Device Headerfile and Examples Include File
#include "F2837xD_Ipc_drivers.h"

// Global counts used in this example
volatile struct TX_FRAME_REG TX_FRAME, TX_UPLOAD;
volatile struct RX_FRAME_REG RX_FRAME;
volatile struct COM_VAR McomData;

#define CPU01TOCPU02_PASSMSG  0x0003FFF4     // CPU01 to CPU02 MSG RAM offsets
#define GS0SARAM_START        0xC000         // Start of GS0 SARAM
volatile tIpcController g_sIpcController1;
volatile tIpcController g_sIpcController2;
float usCPU02Buffer[64];

Uint16 Timer_200us = 0;        //通讯处理
Uint16 Timer_SCI = 0;          //2ms通讯发送
Uint16 Timer_50us = 0;         //控制
Uint16 Timer_100us = 0;        //编码器一个齿轮
Uint16 Timer_850us = 0;        //编码器解算
Uint16 Timer_500ms = 0;        //接收帧计数反馈

const float pi = 3.141592654;  // 圆周率
const float Lq = 0.56e-3;	   // 交轴电感，单位H
const float Rs = 0.029;		   // 电枢电阻，单位ohm

unsigned int Hall_status = 0x8, TX_flag = 0, RX_flag = 0, turns = 0;
float omega = 0.0,  omega_g = 0.0, omega_tmp = 0.0, omega_r = 0.0, omega_n = 0.0;
float IA_tmp = 0, IB_tmp = 0, IC_tmp = 0, ID_tmp = 0, I_main_tmp = 0, Usys_tmp = 0;
float i_A = 0, i_B = 0, i_C = 0, i_D = 0;
float IA = 0, IB = 0, IC = 0, ID = 0;
float Ai = 0, Bi = 0, Ci = 0, Di = 0;
float V_A = 0, V_B = 0, V_C = 0;
float U_sci = 0;
float I_main = 0;
float theta = 0.0;
float T1 = 0, T2 = 0, T3 = 0;
float U_FBKd = 0, U_FBKq = 0, U_FFV = 0, U_LFFq = 0, U_RFFq = 0;
float jiaodu = 0, theta_encoder = 0.0, lTheta_dot = 0.0;
float theta_dq = 0;
int dir = 1;    //转速方向为正

void main(void)
{
// Declare all variables
    uint16_t counter;
    float *pusCPU01BufferPt;
    float *pusCPU02BufferPt;
    uint32_t *pulMsgRam ;
    unsigned int TC_cnt = 0;
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the F2837xD_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initialize GPIO:
// InitGpio();  // Skipped for this example

// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts and clear all CPU interrupt flags:
	DINT;
	IER = 0x0000;
	IFR = 0x0000;

// Initialize PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the F2837xD_PieCtrl.c file.
	InitPieCtrl();

// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in F2837xD_DefaultIsr.c.
// This function is found in F2837xD_PieVect.c.
	InitPieVectTable();

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.TIMER1_INT = &cpu_timer1_isr;
	EDIS;    // This is needed to disable write to EALLOW protected registers

    EALLOW; // This is needed to write to EALLOW protected registers
    PieVectTable.IPC0_INT = &CPU01toCPU02IPC0IntHandler;
    PieVectTable.IPC1_INT = &CPU01toCPU02IPC1IntHandler;
    EDIS;   // This is needed to disable write to EALLOW protected registers

	EALLOW;
	CpuSysRegs.PCLKCR7.bit.SCI_A = 1;
	EDIS;

    IPCInitialize(&g_sIpcController1, IPC_INT0, IPC_INT0);
    IPCInitialize(&g_sIpcController2, IPC_INT1, IPC_INT1);

// Step 4. Initialize all the Device Peripherals to a known state:
// This function is found in F2837xS_InitPeripherals.c
	InitCpuTimers();   // For this example, only initialize the Cpu Timers

// Configure CPU-Timer 0, 1, and 2 to interrupt every second:
// 200MHz CPU Freq, 25u second Period (in uSeconds)
	ConfigCpuTimer(&CpuTimer1, 160, 25);
	CpuTimer1Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit = 0

	InitSci();

// Enable CPU and PIE __interrupts
// This example function is found in the F2837xS_PieCtrl.c file.
	EnableInterrupts();

    pulMsgRam = (void *)CPU01TOCPU02_PASSMSG;
    pusCPU01BufferPt = (void *)GS0SARAM_START;
    pusCPU02BufferPt = (void *)(GS0SARAM_START + 64);

    while(IpcRegs.IPCSTS.bit.IPC16 != 1)
    {
    }
    IpcRegs.IPCACK.bit.IPC16 = 1;

	IPCLtoRFlagSet(IPC_FLAG17);

	while(1)
	{
       /* 清错误标志，串口发送前进行软复位，防止复位影响发送缓存数据    */
       if( SciaRegs.SCIRXST.bit.RXERROR )
       {
    	   SciaRegs.SCICTL1.bit.SWRESET = 0;
    	   SciaRegs.SCICTL1.bit.SWRESET = 1;
       }

	   /*-----串口接收-----*/
	   SCIA_RX();                                          // 获取 omeg_g 等信号

	   if( RX_flag )
	   {
	       RX_flag = 0;    		                           // 清接收标志位

	       omega_g = (float)McomData.speed_cmd * 0.5;      // 解算转速给定
	       if( omega_g >= LIM_omega )              		   // 速度给定限幅
	    	   omega_g = LIM_omega;

	       /*********** Data Block Writes*************/
	       // Request Memory Access to GS0 SARAM for CPU02, Set bits to let CPU02 own GS0

	       IPCReqMemAccess(&g_sIpcController2, GS0_ACCESS, IPC_GSX_CPU2_MASTER,ENABLE_BLOCKING);

	       while(MemCfgRegs.GSxMSEL.bit.MSEL_GS0 != 1U)
	       {
	       }

	       // Write a block of data from CPU02 to GS0 shared RAM which is then written to an CPU01 address.

	       for(counter = 0; counter < 4; counter++)
	       {
	           pusCPU02BufferPt[0] = omega_g;
	           pusCPU02BufferPt[1] = 1;
	           pusCPU02BufferPt[2] = 2;
	           pusCPU02BufferPt[3] = 3;
	       }

	       IPCLtoRBlockWrite(&g_sIpcController2, pulMsgRam[0],(uint32_t)pusCPU02BufferPt,64, IPC_LENGTH_16_BITS,ENABLE_BLOCKING);

	       TC_cnt = TC_cnt + 1;  	                 	   // 接收帧计数

           EALLOW;
           GpioDataRegs.GPBTOGGLE.bit.GPIO45 = 1;
           EDIS;

	    }

   	    /*-----2ms串口定时发送帧封装-----*/
	    if( Timer_SCI >= 80 )				             // 2ms
	    {
	    	Timer_SCI = 0;	   					         // Clear 2ms SCI Task Flag

		    /*********** Data Block Reads*************/
		    IPCReqMemAccess(&g_sIpcController2, GS0_ACCESS, IPC_GSX_CPU1_MASTER,ENABLE_BLOCKING);

		    IPCLtoRBlockRead(&g_sIpcController2, pulMsgRam[0],(uint32_t)pusCPU01BufferPt,64, ENABLE_BLOCKING,IPC_FLAG16);

		    while(IpcRegs.IPCFLG.bit.IPC16)
		    {
		    }

		    for(counter = 0; counter <64; counter++)
		    {
		        usCPU02Buffer[counter] = pusCPU01BufferPt[counter];
		    }

	    	/* 串口发送数据处理   */
	    	IA = usCPU02Buffer[0] + 30.0;
	    	IB = usCPU02Buffer[1] + 30.0;
	    	IC = usCPU02Buffer[2] + 30.0;
	    	ID = usCPU02Buffer[3] + 30.0;
	    	U_sci = usCPU02Buffer[4] * 57.29577951;		         // 换算为角度 57.3 deg/rad
	    	I_main = usCPU02Buffer[5];

	    	SCIA_TX();                 	   			     // 封装串口发送帧
	    }

		/******************************/
		/*          500ms任务                               */
		/******************************/
		if( Timer_500ms >= 20000 )
		{
		    Timer_500ms = 0;		                     // Clear 500ms Task Flag

		    McomData.frame_cnt = TC_cnt & 0xFF;          // 将每0.5s的接收帧计数存入发送帧
		    TC_cnt = 0;
		}
	}
}

    /*********************************************************/
    /*-----------------1us定时中断服务程序-------------------*/
    /*********************************************************/

    interrupt void cpu_timer1_isr(void)
    {
       Timer_SCI++;
       Timer_500ms++;

       CpuTimer1.InterruptCount++;
    }

    // CPU01toCPU02IPC0IntHandler - Handles writes into CPU01 addresses as a result of read commands to the CPU02.

    __interrupt void CPU01toCPU02IPC0IntHandler (void)
    {
        tIpcMessage sMessage;

        // Continue processing messages as long as CPU01 to CPU02 GetBuffer1 is full

        while(IpcGet(&g_sIpcController1, &sMessage, DISABLE_BLOCKING) != STATUS_FAIL)
        {
            switch (sMessage.ulcommand)
            {
                case IPC_DATA_WRITE:
                    IPCRtoLDataWrite(&sMessage);
                    break;
                default:
//                    ErrorFlag = 1;
                    break;
            }
        }

        // Acknowledge IPC INT0 Flag and PIE to receive more interrupts
        IpcRegs.IPCACK.bit.IPC0 = 1;
        PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    }

    // CPU01toCPU02IPC1IntHandler - Should never reach this ISR. This is an optional placeholder for g_sIpcController2.

    __interrupt void CPU01toCPU02IPC1IntHandler(void)
    {
        // Should never reach here - Placeholder for Debug

        // Acknowledge IPC INT1 Flag and PIE to receive more interrupts
        IpcRegs.IPCACK.bit.IPC1 = 1;
        PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    }

	//===========================================================================
	// No more.
	//===========================================================================

