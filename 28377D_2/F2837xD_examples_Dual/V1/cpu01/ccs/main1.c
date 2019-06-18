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

#define CPU01TOCPU02_PASSMSG  0x0003FFF4      // CPU01 to CPU02 MSG RAM offsets for passing address
volatile tIpcController g_sIpcController1;    // IPC����
volatile tIpcController g_sIpcController2;    // IPC����
float usCPU01Buffer[64];                   // ���ջ���
float usCPU01Buffer1[64];                  // ���ջ���
uint16_t counter1;

Uint16 Timer_200us = 0;        //ͨѶ����
Uint16 Timer_SCI = 0;          //2msͨѶ����
Uint16 Timer_50us = 0;         //����
Uint16 Timer_100us = 0;        //������һ������
Uint16 Timer_850us = 0;        //����������
Uint16 Timer_500ms = 0;        //����֡��������

const float pi = 3.141592654;  // Բ����
const float Lq = 0.56e-3;	   // �����У���λH
const float Rs = 0.029;		   // ������裬��λohm

unsigned int Hall_status = 0x8, TX_flag = 0, RX_flag = 0, turns = 0;
float omega = 0.0,  omega_g = 0.0, omega_tmp = 0.0, omega_r = 0.0, omega_n = 0.0;
float IA_tmp = 0, IB_tmp = 0, IC_tmp = 0, ID_tmp = 0, I_main_tmp = 0, Usys_tmp = 0;
float i_A = 0, i_B = 0, i_C = 0, i_D = 0;
float IA = 0, IB = 0, IC = 0, ID = 0;
float Ai = 0, Bi = 0, Ci = 0, Di = 0;
float V_A = 0, V_B = 0, V_C = 0;
float U_sci = 0;
float I_main = 0;
float T1 = 0, T2 = 0, T3 = 0;
float U_FBKd = 0, U_FBKq = 0, U_FFV = 0, U_LFFq = 0, U_RFFq = 0;
float jiaodu = 0, theta_encoder = 0.0, lTheta_dot = 0.0;
float theta = 0.0, theta_dq = 0;
int dir = 1;    //ת�ٷ���Ϊ��

void main(void)
{
// Declare all local variables
    const int LIM_OUT = 960;                                        //��������ѹ�޷�ֵ
	const float LIM_ufbk = 300.0;                                   //���ջ���ѹ�޷�
	const float LIM_Uq = 280.0;
	const float LIM_Ud = 280.0;
	const float LIM_ULFFq = 260.0;

	const float Ke = 0.016 * sqrt(3.0 / 2.0);                       // ǰ��ϵ��������ϵ��, V/rpm
	const float dq_tran = sqrt(2.0 / 3.0);                          // ʸ���任ϵ��
//	unsigned int i = 0;

	float theta_pha = 0.0, theta_tran = 0.0, theta_g = 0.0, theta_turn = 0.0, theta_last = 0, theta_ref = 0;

	float Ig = 0.0;                                                 //����������
	float Id = 0.0, Iq = 0.0, Ud = 0.0, Uq = 0.0, Uaf = 0, Ubt = 0;

	float ctrl_uA = 0.0, ctrl_uB = 0.0, ctrl_uC = 0.0;              //���Ƶ�ѹ
	float Ctrl_Duty_A = 0.0, Ctrl_Duty_B = 0.0, Ctrl_Duty_C = 0.0;  // ���Ƶ�ѹռ�ձ�
	float inv_U_sys = 0.0, U_main = 270.0;

	int UA = 0, UB = 0, UC = 0;
	unsigned int TC_cnt = 0, encoder_flag = 5, turns_flag = 0;
	unsigned long turns_start = 0, turns_resolver = 0, resolver_last = 0;

    uint32_t *pulMsgRam;
    uint16_t counter;

// Step 1. Initialize System Control:��ʼ��

    // PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the F2837xD_SysCtrl.c file.
	InitSysCtrl();

#ifdef _STANDALONE
#ifdef _FLASH
	// Send boot command to allow the CPU2 application to begin execution
	IPCBootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_FLASH);
#else
	// Send boot command to allow the CPU2 application to begin execution
	IPCBootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_RAM);
#endif
#endif

// Step 2. Initialize GPIO:��ʼ��GPIO

	InitGpio();  
    Gpioselect();
    InitSciGpio();
    InitEPwmGpio();
    InitSpiGpio();
    
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
	PieVectTable.TIMER0_INT = &cpu_timer0_isr;
//	PieVectTable.TIMER1_INT = &cpu_timer1_isr;
	EDIS;    // This is needed to disable write to EALLOW protected registers

    EALLOW;
    DevCfgRegs.CPUSEL5.bit.SCI_A = 1;
    EDIS;

    EALLOW;  // This is needed to write to EALLOW protected registers
    PieVectTable.IPC0_INT = &CPU02toCPU01IPC0IntHandler;
    PieVectTable.IPC1_INT = &CPU02toCPU01IPC1IntHandler;
    EDIS;    // This is needed to disable write to EALLOW protected registers

// Step 4. Initialize all the Device Peripherals to a known state:  �������豸�����ʼ��Ϊ��֪״̬

    // This function is found in F2837xS_InitPeripherals.c
	InitCpuTimers();   // For this example, only initialize the Cpu Timers

// Configure CPU-Timer 0, 1, and 2 to interrupt every second:
// 200MHz CPU Freq, 25u second Period (in uSeconds)
	ConfigCpuTimer(&CpuTimer0, 160, 25);
//	ConfigCpuTimer(&CpuTimer1, 160, 25);
	CpuTimer0Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit = 0
//	CpuTimer1Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit = 0

    IPCInitialize(&g_sIpcController1, IPC_INT0, IPC_INT0);
    IPCInitialize(&g_sIpcController2, IPC_INT1, IPC_INT1);
//	InitSci();
	InitEPwm();
	InitSpi();
	InitAdc();

// Enable CPU and PIE __interrupts
// This example function is found in the F2837xS_PieCtrl.c file.
	EnableInterrupts();

// Step 5.User specific functions, Reassign vectors (optional), Enable Interrupts �û��ض��Ĺ��ܣ����·��䣬ʹ���ж�����:

	InitAD2S1210();
	AD2S1210SelectMode(CONFIG);
	WriteToAD2S1210(EXFREQUENCY, 0x2800);	  	//10KHz Excitation
	WriteToAD2S1210(CONTROL_RDC, 0x7D00);		//Set resolution: 14bit

    for(counter = 0; counter < 64; counter++)
    {
        usCPU01Buffer[counter] = 0;
    }

    // Point array to address in CPU02 TO CPU01 MSGRAM for passing variable locations
    pulMsgRam = (void *)CPU01TOCPU02_PASSMSG;
    pulMsgRam[0] = (uint32_t)&usCPU01Buffer[0];
    IpcRegs.IPCSET.bit.IPC16 = 1;

// Spin here until CPU02 is ready
	while (!IPCRtoLFlagBusy(IPC_FLAG17));
	IPCRtoLFlagAcknowledge(IPC_FLAG17);
//
//	state = GpioDataRegs.GPADAT.bit.GPIO14;

// Step 6. IDLE loop. Just sit and loop forever (optional):ѭ��

	while(1)
	{

	    /******************************/
	    /*           50us����                                       */
	    /******************************/
	    if( Timer_50us >= 2 )
	    {
	        Timer_50us = 0;      		                   // Clear 50us Task Flag

	        WDT_TOG_Left  = 1;
	        WDT_TOG_Right = 1;

			/*       ��ȡת��                         */
			omega_tmp = ReadVelocity();					   // ��ȡ���ٶȣ���λrpm
			omega = SecOrderFilter_omega( omega_tmp );	   // ���ٶȶ����˲�

		    /*-----��ȡת��-----*/
		    theta = ReadTheta();		                   // ��λ: rad,ȡֵ��Χ��[ 0, 2Pi )

		    if( theta >= 2.0 * pi )
		    	theta = theta - 2.0 * pi;
		    else if ( theta < 0 )
		    	theta = theta + 2.0 * pi;

		    /* 0 ~ pi �ߵ�ƽ��pi ~ 2*pi �͵�  */
		     if( theta < pi )
		    	 H_A = 1;
		     else
		    	 H_A = 0;

    /*    ����λ�ý���      */
		    if(turns_flag)
		    {
		    	turns_flag = 0;
		    	turns_resolver = turns_start;
		    }

		    if((jiaodu - resolver_last) < -20)
		    	turns_resolver = turns_resolver + 1;
		    else if((jiaodu - resolver_last) > 20)
		    	turns_resolver = turns_resolver - 1;

		    theta_encoder = jiaodu + 8.0 * pi * turns_resolver;      // ��Ȧ�Ƕ�
		    resolver_last = jiaodu;

	 		 if(( omega_g - omega_r ) > 0.5)                // 10000rps
  		          omega_r += 0.5;
 		     else if(( omega_g - omega_r ) < - 0.5)
		    	  omega_r -= 0.5;
		     else
		    	  omega_r = omega_g;

		     /*-----ת�ٵ������Ͽ���-----*/
		     Ig = ASR( omega_r, omega ); // Iq �ٶȿ����������ֵ

			 theta_pha = -4.2 * atan2( omega, 7.5e4 );      // �˲����ͺ󲹳�,�˲�������*5K

		     theta_tran = theta + theta_pha;         //�Ƕ�

		     if( theta_tran >= 2.0 * pi )
		    	 theta_tran = theta_tran - 2.0 * pi;
		     else if ( theta_tran < 0 )
		    	 theta_tran = theta_tran + 2.0 * pi;

		    /*   ����任      dq_tran = sqrt(2.0 / 3.0)    */
		     Id = dq_tran * ( - cos( theta_tran ) * i_A - cos( theta_tran - 2.0 * pi / 3.0 ) * i_B - cos( theta_tran - 4.0 * pi / 3.0 ) * i_C );
		     Iq = dq_tran * (   sin( theta_tran ) * i_A + sin( theta_tran - 2.0 * pi / 3.0 ) * i_B + sin( theta_tran - 4.0 * pi / 3.0 ) * i_C );

		     U_FFV = omega_g * Ke;//��ѹ������

		     U_FBKq = ATR( Iq, Ig, omega );// Uq ���������������ֵ          ATR�������ֿ�����
		     U_FBKd = AYR( Id, omega );   //Ud

		    /*   �����ջ����Ƶ�ѹ���޷�       */
		     if ( U_FBKq > LIM_ufbk )
		    	U_FBKq = LIM_ufbk;
		     else if ( U_FBKq < -LIM_ufbk )
		    	U_FBKq = -LIM_ufbk;

		     if ( U_FBKd > LIM_ufbk )
		        U_FBKd = LIM_ufbk;
		     else if ( U_FBKd < -LIM_ufbk )
		    	U_FBKd = -LIM_ufbk;

		    /* ������Ƶ�ѹ���ɷ��綯�Ʋ������������ջ��������������ѹ���������   */
		     Uq = U_FFV + U_FBKq;
		     Ud = U_FBKd;

		     if ( Uq > LIM_Uq )
		    	Uq = LIM_Uq;
		     else if ( Uq < -LIM_Uq )
		    	Uq = -LIM_Uq;

		     if ( Ud > LIM_Ud )
		    	Ud = LIM_Ud;
		     else if ( Ud < -LIM_Ud )
		    	Ud = -LIM_Ud;

		    /*   ������任       */
		     ctrl_uA = dq_tran * ( Uq * sin( theta ) - Ud * cos( theta ) );                                  // + U_FFV * sin( theta );
		     ctrl_uB = dq_tran * ( Uq * sin( theta - 2.0 * pi / 3.0 ) - Ud * cos( theta - 2.0 * pi / 3.0 ) );// + U_FFV * sin(theta - 2.0 * pi / 3.0 );
		     ctrl_uC = dq_tran * ( Uq * sin( theta - 4.0 * pi / 3.0 ) - Ud * cos( theta - 4.0 * pi / 3.0 ) );// + U_FFV * sin(theta - 4.0 * pi / 3.0 );

		    /* ����ĸ�ߵ�ѹ�ĵ����������������Ϊ�˷������CPUִ��Ч��  */
		   //inv_U_sys = 1.0 / U_main ;                     // 278.0
		     inv_U_sys = 1.0 / 28.0;

		  	/* �����������Ƶ�ѹռ�ձ�   */
		     Ctrl_Duty_A = ctrl_uA * inv_U_sys;
		     Ctrl_Duty_B = ctrl_uB * inv_U_sys;
		     Ctrl_Duty_C = ctrl_uC * inv_U_sys;

		    /*  ����A��B��C�����ѹ��ӦPWM�Ĵ�������ֵ       */
		     UA = (int)( Ctrl_Duty_A * 960 );              // ռ�ձ���ֵ��Ӧ1125
		     UB = (int)( Ctrl_Duty_B * 960 );              // ռ�ձ���ֵ��Ӧ1125
		     UC = (int)( Ctrl_Duty_C * 960 );              // ռ�ձ���ֵ��Ӧ1125

		    /*-----PWM������ֵ�޷�-----*/
		     if( UA > LIM_OUT)
		    	UA = LIM_OUT;
		     else if( UA < -LIM_OUT )
		    	UA = -LIM_OUT;

		     if( UB > LIM_OUT)
		    	UB = LIM_OUT;
		     else if( UB < -LIM_OUT )
		    	UB = -LIM_OUT;

		     if( UC > LIM_OUT)
		    	UC = LIM_OUT;
		     else if( UC < -LIM_OUT )
		    	UC = -LIM_OUT;

		    /*  ���A��B��C����PWM�Ĵ�������ֵ     */
		     EPwm6Regs.CMPA.bit.CMPA = (unsigned int) (1000 + UA);
		     EPwm7Regs.CMPA.bit.CMPA = (unsigned int) (1000 + UB);
		     EPwm1Regs.CMPA.bit.CMPA = (unsigned int) (1000 + UC);
	    }

	    /******************************/
	    /*           200us����                       */
	    /******************************/
	    if( Timer_200us >= 8 )
	    {
	    	Timer_200us = 0;				               	  // Clear 200us Task Flag

	    	/*  һ�׹����˲�        */
	    	U_main = FirstOrderFilter_Usys( Usys_tmp );
	    	I_main = FirstOrderFilter_Isys( I_main_tmp );

	    	/*  ���ڷ������ݴ���֡���0x3-  */
	    	McomData.I_PhA[Timer_SCI] = (int)(i_A * 2);
	    	McomData.I_PhB[Timer_SCI] = (int)(i_B * 2);
	    	McomData.I_PhC[Timer_SCI] = (int)(i_C * 2);
	    	McomData.I_PhD[Timer_SCI] = (int)(i_D * 2);

	    	Hall_status &= (0x03FF >> (10 - Timer_SCI));     //��λ����ʾHALL�ź�
	    	Hall_status |= (H_A & 0x01) << Timer_SCI;

	    }

		/******************************/
		/*          100us����                               */
		/******************************/
		if( Timer_100us >= 4 )
		{
			Timer_100us = 0;

			read_absolute_encoder();     // ���Ա�����
			num_teeth++;
			if( num_teeth >= 8 )
				num_teeth = 0;
		}

		/******************************/
		/*          850us����                               */
		/******************************/
		if( Timer_850us >= 34 )
		{
			Timer_850us = 0;
		    last_turns = num_turns;
			get_real_data_absolute_encoder();
			analyze_absolute_encoder();

		    if(abs(num_turns-last_turns) < 2)
		    	turns = num_turns;
		    else
		    	turns = last_turns;

		    if(encoder_flag)
		    {
		        encoder_flag = encoder_flag - 1;
		        turns_flag = 1;
		        turns_start = turns;
		    }
		}

		/******************************/
		/*          500ms����                               */
		/******************************/
		if( Timer_500ms >= 20000 )
		{
		    Timer_500ms = 0;		                     // Clear 500ms Task Flag

		    McomData.frame_cnt = TC_cnt & 0xFF;          // ��ÿ0.5s�Ľ���֡�������뷢��֡
		    TC_cnt = 0;
		}

    }
}

    /*************************************************/
	/*       λ�ÿ������� ���뵥λ���������λ��RPM          */
	/*************************************************/
	float APR( float Theta_g, float Theta_m )
	{

//	    const float LIM_Theta = 60000.0;        // λ�ø����޷�

		const float LIM_Thetaerr = 3369.0;	    // λ��ƫ�����528mm
		const float LIM_omeg = 11000.0;         // ת���޷�

		static float kp = 45;
		static float kI = 0;//0.74e-5;

		static float     err = 0.0;             // λ�����
		static float err_sum = 0.0;             // λ��������

		static float n_g;

//		if ( Theta_g > LIM_Theta )
//			Theta_g = LIM_Theta;
//		else if ( Theta_g < -LIM_Theta )
//			Theta_g = -LIM_Theta;

		err = Theta_g - Theta_m;   	            // ����λ�����

		if ( err > LIM_Thetaerr )
			err = LIM_Thetaerr;
		else if (  err < -LIM_Thetaerr )
			err = - LIM_Thetaerr;

		n_g = kp * err;                        // PI ������

		if ( n_g > LIM_omeg )
			n_g = LIM_omeg;
		else if ( n_g < -LIM_omeg )
			n_g = - LIM_omeg;

		return n_g;

	}

	/*************************************************/
	/*       �ٶȿ������� ���뵥λ��RPM�������λ��V              */
	/*************************************************/
	float ASR( float omega_g, float omega )
	{

	    const float LIM_omeg = 11000.0;     // 	�ٶȸ����޷�

		const float LIM_err = 11000.0;	    // ת�����10000rpm
		const float LIM_sum = 6.66e6;		// �ٶ��������޷�

		const float LIM_cur = 65.0;        // 2.4kW������޷�10A��33A, 49A

	//	const float kp= 0.12;				// ��λ��A/RPM  ��ʼ0.05
	//	const float kI = 1.2e-5;			// ��λ��A/RPM
		static float kp = 0.06;//0.05;			   // ��λ��A/RPM  ��ʼ0.05
		static float kI = 1.2e-5;			// ��λ��A/RPM

		static float     err = 0.0;         // �ٶ����
		static float err_sum = 0.0;         // �ٶ�������

		static float u_ig;

		if ( omega_g > LIM_omeg )
			omega_g = LIM_omeg;
		else if ( omega_g < -LIM_omeg )
			omega_g = -LIM_omeg;

		err = omega_g - omega;   	        // �����ٶ����

		if ( err > LIM_err )
			err = LIM_err;
		else if (  err < -LIM_err )
			err = - LIM_err;

		if( abs(omega) > 250 )                   //С��450rpm���ص�ת�ٻ�����
		{
			err_sum = err_sum + err;
		}
		else
		{
			err_sum = 0.9 * err_sum;
		}

		if( err_sum > LIM_sum )
			err_sum = LIM_sum;
		else if ( err_sum < -LIM_sum )
			err_sum = -LIM_sum;

		u_ig = kp * err;// + kI * err_sum;     // PI ������

		if ( u_ig > LIM_cur )
			u_ig = LIM_cur;
		else if ( u_ig < -LIM_cur )
			u_ig = - LIM_cur;

		return u_ig;

	}

	/*************************************************/
	/*       ����Id�������� ���뵥λ��RPM�������λ��V              */
	/*************************************************/
	float AYR( float Id, float omega)
	{
		const float LIM_sum_Id = 300.0;		         // �����޷�

		static float     err_Id = 0.0;               // Id���
		static float err_sum_Id = 0.0;               // Id������

		const float LIM_Ud = 280.0;                    // 2.4kW��ѹ�޷�

		const float Kp_Id = 1.0;                     // ��λ��V/A
	//	const float Ki_Id = 5.0e-5;                  // 5A/3S
//		const float Ki_Id = 5.0e-4;                  // 5A/S
		const float Ki_Id = 1.2e-3;                  // 10A/S

		static float u_Id;

		err_Id = 0.0 - Id;   	                     // ����������
	/*	if ( err_Id > LIM_err_Id )
			err_Id = LIM_err_Id;
		else if (  err_Id < -LIM_err_Id )
			err_Id = - LIM_err_Id;
	*/
		if( abs(omega) > 100 )                            //С��400rpm���ص�����������
		{
			err_sum_Id = err_sum_Id + err_Id;
		}
		else
		{
			err_sum_Id = 0.9 * err_sum_Id;
		}

	/*
		if( err_sum_Id > LIM_sum_Id )
			err_sum_Id = LIM_sum_Id;
		else if ( err_sum_Id < -LIM_sum_Id )
			err_sum_Id = -LIM_sum_Id;
	*/
		u_Id = Kp_Id * err_Id;// + Ki_Id * err_sum_Id;

		if ( u_Id > LIM_Ud )
			u_Id = LIM_Ud;
		else if ( u_Id < -LIM_Ud )
			u_Id = - LIM_Ud;

		return u_Id;
	}

	/*************************************************/
	/*       ����Iq�������� ���뵥λ��RPM�������λ��V              */
	/*************************************************/
	float ATR( float Iq, float Ig, float omega )
	{
		const float LIM_sum_Iq = 600.0;		         // �������޷�

		static float     err_Iq = 0.0;               // Iq���
		static float err_sum_Iq = 0.0;               // Iq������

		const float LIM_Uq = 280.0;                   // 2.4kW������޷�10A��33A, 49A

		const float Kp_Iq = 1.0;                      // ��λ��V/A
		const float Ki_Iq = 1.2e-3;

		static float u_Iq;

		err_Iq = Ig - Iq;   	                     // ����������

		if( abs(omega) > 100 )                            //С��450rpm���ص�����������
		{
			err_sum_Iq = err_sum_Iq + err_Iq;
		}
		else
		{
			err_sum_Iq = 0.9 * err_sum_Iq;
		}

	/*
		if( err_sum_Iq > LIM_sum_Iq )
			err_sum_Iq = LIM_sum_Iq;
		else if ( err_sum_Iq < -LIM_sum_Iq )
			err_sum_Iq = -LIM_sum_Iq;
	*/
		u_Iq = Kp_Iq * err_Iq;// + Ki_Iq * err_sum_Iq;
		if ( u_Iq > LIM_Uq )
			u_Iq = LIM_Uq;
		else if ( u_Iq < -LIM_Uq )
			u_Iq = - LIM_Uq;

		return u_Iq;
	}

	/******************************************************/
	/*-----------------�����˲����˲����� ------------------*/
	/******************************************************/
	// λ�ø��������˲�
	float SecOrderFilter_Theta_cmd( float u )
	{
		/*------�˲�����������------*/
		const float Ts = 50e-6;;				// �������ڣ���λs
		const float omega_n = 2*12* pi;		    // ��Ȼ��Ƶ�ʣ���λrad/s
		const float zeta = 1;					// �����

		/*------��ַ��̲�������------*/
		const float k1 = ( 8 - 2 * omega_n * omega_n * Ts * Ts ) / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );
		const float k2 = ( 4 - 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts ) / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );
		const float k3 = omega_n * omega_n * Ts * Ts / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );

		/*------��ʷ���ֵ, �趨��ֵ------*/
		static float y0 = 0.0;
		static float y1 = 0.0;
		static float y2 = 0.0;

		/*------��ʷ����ֵ���趨��ֵ------*/
		static float u0 = 0.0;
		static float u1 = 0.0;

		y2 = k1 * y1 - k2 * y0 + k3 * ( u + 2 * u1 + u0 ) ;

		lTheta_dot = (y2 - y1) / Ts /10;
		/*
		if(lTheta_dot > 26)
			lTheta_dot = 26;
		else if(lTheta_dot < -26)
			lTheta_dot = -26;
		*/

		y0 = y1;
		y1 = y2;

		u0 = u1;
		u1 = u;
//		return y2;
		return lTheta_dot;
	}

	/******************************************************/
	/*-----------------�����˲����˲����� ------------------*/
	/******************************************************/
	// A���ѹ�����˲�
	float SecOrderFilter_VA( float u )
	{
		/*------�˲�����������------*/
		const float Ts = 2000e-6;					// �������ڣ���λs
		const float omega_n = 5;		        // ��Ȼ��Ƶ�ʣ���λrad/s
		const float zeta = 1;				    	// �����

		/*------��ַ��̲�������------*/
		const float k1 = ( 8 - 2 * omega_n * omega_n * Ts * Ts ) / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );
		const float k2 = ( 4 - 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts ) / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );
		const float k3 = omega_n * omega_n * Ts * Ts / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );

		/*------��ʷ���ֵ, �趨��ֵ------*/
		static float y0 = 0.0;
		static float y1 = 0.0;
		static float y2 = 0.0;

		/*------��ʷ����ֵ���趨��ֵ------*/
		static float u0 = 0.0;
		static float u1 = 0.0;

		y2 = k1 * y1 - k2 * y0 + k3 * ( u + 2 * u1 + u0 ) ;

		y0 = y1;
		y1 = y2;

		u0 = u1;
		u1 = u;

		return y2;
	}

	/******************************************************/
	/*-----------------�����˲����˲����� ------------------*/
	/******************************************************/
	// B���ѹ�����˲�
	float SecOrderFilter_VB( float u )
	{
		/*------�˲�����������------*/
		const float Ts = 200e-6;					// �������ڣ���λs
		const float omega_n = 2 * pi * 5e3;		        // ��Ȼ��Ƶ�ʣ���λrad/s
		const float zeta = 1;				    	// �����

		/*------��ַ��̲�������------*/
		const float k1 = ( 8 - 2 * omega_n * omega_n * Ts * Ts ) / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );
		const float k2 = ( 4 - 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts ) / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );
		const float k3 = omega_n * omega_n * Ts * Ts / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );

		/*------��ʷ���ֵ, �趨��ֵ------*/
		static float y0 = 0.0;
		static float y1 = 0.0;
		static float y2 = 0.0;

		/*------��ʷ����ֵ���趨��ֵ------*/
		static float u0 = 0.0;
		static float u1 = 0.0;

		y2 = k1 * y1 - k2 * y0 + k3 * ( u + 2 * u1 + u0 ) ;

		y0 = y1;
		y1 = y2;

		u0 = u1;
		u1 = u;

		return y2;
	}

	/******************************************************/
	/*-----------------�����˲����˲����� ------------------*/
	/******************************************************/
	// C�����ѹ�����˲�
	float SecOrderFilter_VC( float u )
	{
		/*------�˲�����������------*/
		const float Ts = 200e-6;					// �������ڣ���λs
		const float omega_n = 2 * pi * 5e3;		        // ��Ȼ��Ƶ�ʣ���λrad/s
		const float zeta = 1;				    	// �����

		/*------��ַ��̲�������------*/
		const float k1 = ( 8 - 2 * omega_n * omega_n * Ts * Ts ) / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );
		const float k2 = ( 4 - 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts ) / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );
		const float k3 = omega_n * omega_n * Ts * Ts / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );

		/*------��ʷ���ֵ, �趨��ֵ------*/
		static float y0 = 0.0;
		static float y1 = 0.0;
		static float y2 = 0.0;

		/*------��ʷ����ֵ���趨��ֵ------*/
		static float u0 = 0.0;
		static float u1 = 0.0;

		y2 = k1 * y1 - k2 * y0 + k3 * ( u + 2 * u1 + u0 ) ;

		y0 = y1;
		y1 = y2;

		u0 = u1;
		u1 = u;

		return y2;
	}

	/******************************************************/
	/*-----------------�����˲����˲����� ------------------*/
	/******************************************************/
	// A����������˲�
	float SecOrderFilter_IA( float u )
	{
		/*------�˲�����������------*/
		const float Ts = 25e-6;					// �������ڣ���λs
		const float omega_n = 2 * pi * 5e3;		// ��Ȼ��Ƶ�ʣ���λrad/s
		const float zeta = 1;					// �����

		/*------��ַ��̲�������------*/
		const float k1 = ( 8 - 2 * omega_n * omega_n * Ts * Ts ) / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );
		const float k2 = ( 4 - 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts ) / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );
		const float k3 = omega_n * omega_n * Ts * Ts / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );

		/*------��ʷ���ֵ, �趨��ֵ------*/
		static float y0 = 0.0;
		static float y1 = 0.0;
		static float y2 = 0.0;

		/*------��ʷ����ֵ���趨��ֵ------*/
		static float u0 = 0.0;
		static float u1 = 0.0;

		y2 = k1 * y1 - k2 * y0 + k3 * ( u + 2 * u1 + u0 ) ;

		y0 = y1;
		y1 = y2;

		u0 = u1;
		u1 = u;

		return y2;
	}

	// B����������˲�
	float SecOrderFilter_IB( float u )
	{
		/*------�˲�����������------*/
		const float Ts = 25e-6;					// �������ڣ���λs
		const float omega_n = 2 * pi * 5e3;		// ��Ȼ��Ƶ�ʣ���λrad/s
		const float zeta = 1;					// �����

		/*------��ַ��̲�������------*/
		const float k1 = ( 8 - 2 * omega_n * omega_n * Ts * Ts ) / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );
		const float k2 = ( 4 - 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts ) / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );
		const float k3 = omega_n * omega_n * Ts * Ts / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );

		/*------��ʷ���ֵ, �趨��ֵ------*/
		static float y0 = 0.0;
		static float y1 = 0.0;
		static float y2 = 0.0;

		/*------��ʷ����ֵ���趨��ֵ------*/
		static float u0 = 0.0;
		static float u1 = 0.0;

		y2 = k1 * y1 - k2 * y0 + k3 * ( u + 2 * u1 + u0 ) ;

		y0 = y1;
		y1 = y2;

		u0 = u1;
		u1 = u;

		return y2;
	}

	// C����������˲�
	float SecOrderFilter_IC( float u )
	{
		/*------�˲�����������------*/
		const float Ts = 25e-6;					// �������ڣ���λs
		const float omega_n = 2 * pi * 5e3;		// ��Ȼ��Ƶ�ʣ���λrad/s
		const float zeta = 1;					// �����

		/*------��ַ��̲�������------*/
		const float k1 = ( 8 - 2 * omega_n * omega_n * Ts * Ts ) / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );
		const float k2 = ( 4 - 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts ) / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );
		const float k3 = omega_n * omega_n * Ts * Ts / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );

		/*------��ʷ���ֵ, �趨��ֵ------*/
		static float y0 = 0.0;
		static float y1 = 0.0;
		static float y2 = 0.0;

		/*------��ʷ����ֵ���趨��ֵ------*/
		static float u0 = 0.0;
		static float u1 = 0.0;

		y2 = k1 * y1 - k2 * y0 + k3 * ( u + 2 * u1 + u0 ) ;

		y0 = y1;
		y1 = y2;

		u0 = u1;
		u1 = u;

		return y2;
	}

	// D����������˲�
	float SecOrderFilter_ID( float u )
	{
		/*------�˲�����������------*/
		const float Ts = 25e-6;					// �������ڣ���λs
		const float omega_n = 2 * pi * 5e3;		// ��Ȼ��Ƶ�ʣ���λrad/s
		const float zeta = 1;					// �����

		/*------��ַ��̲�������------*/
		const float k1 = ( 8 - 2 * omega_n * omega_n * Ts * Ts ) / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );
		const float k2 = ( 4 - 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts ) / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );
		const float k3 = omega_n * omega_n * Ts * Ts / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );

		/*------��ʷ���ֵ, �趨��ֵ------*/
		static float y0 = 0.0;
		static float y1 = 0.0;
		static float y2 = 0.0;

		/*------��ʷ����ֵ���趨��ֵ------*/
		static float u0 = 0.0;
		static float u1 = 0.0;

		y2 = k1 * y1 - k2 * y0 + k3 * ( u + 2 * u1 + u0 ) ;

		y0 = y1;
		y1 = y2;

		u0 = u1;
		u1 = u;

		return y2;
	}

	// ת�ٶ����˲�
	float SecOrderFilter_omega( float u )
	{
		/*------�˲�����������------*/
		const float Ts = 50e-6;					// �������ڣ���λs
		const float omega_n = 2 * pi * 50;		// ��Ȼ��Ƶ�ʣ���λrad/s
		const float zeta = 1;					// �����

		/*------��ַ��̲�������------*/
		const float k1 = ( 8 - 2 * omega_n * omega_n * Ts * Ts ) / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );
		const float k2 = ( 4 - 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts ) / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );
		const float k3 = omega_n * omega_n * Ts * Ts / ( 4 + 4 * Ts * zeta * omega_n + omega_n * omega_n * Ts * Ts );

		/*------��ʷ���ֵ, �趨��ֵ------*/
		static float y0 = 0.0;
		static float y1 = 0.0;
		static float y2 = 0.0;

		/*------��ʷ����ֵ���趨��ֵ------*/
		static float u0 = 0.0;
		static float u1 = 0.0;

		y2 = k1 * y1 - k2 * y0 + k3 * ( u + 2 * u1 + u0 ) ;

		y0 = y1;
		y1 = y2;

		u0 = u1;
		u1 = u;

		return y2;
	}


	/********************************************************/
	/*----------------һ�׹����˲����˲�����-----------------*/
	/********************************************************/
	// ĸ�ߵ�ѹһ���˲�
	float FirstOrderFilter_Usys( float u )
	{
		/*------�˲�����������------*/
		const float Fb = 4;			// ͨƵ������λHz
		const float Ts = 200e-6;	// �������ڣ���λs

		/*------��ַ��̲�������------*/
		const float a1 = ( 2 - 2 * pi * Fb * Ts ) / ( 2 + 2 * pi * Fb * Ts );
		const float b1 = 2 * pi * Fb * Ts / ( 2 + 2 * pi * Fb * Ts );

		/*------��ʷ�������ֵ�����ֵ ------*/
		static float u0 = 0.0;
		static float y0 = 0.0;
		static float y1 = 0.0;

		y1 =  a1 * y0 + b1 * ( u + u0 );

		y0 = y1;
		u0 = u;

		return y1;
	}

	// ĸ�ߵ���һ���˲�
	float FirstOrderFilter_Isys( float u )
	{
		/*------�˲�����������------*/
		const float Fb = 4;			// ͨƵ������λHz
		const float Ts = 200e-6;	// �������ڣ���λs

		/*------��ַ��̲�������------*/
		const float a1 = ( 2 - 2 * pi * Fb * Ts ) / ( 2 + 2 * pi * Fb * Ts );
		const float b1 = 2 * pi * Fb * Ts / ( 2 + 2 * pi * Fb * Ts );

		/*------��ʷ�������ֵ�����ֵ ------*/
		static float u0 = 0.0;
		static float y0 = 0.0;
		static float y1 = 0.0;

		y1 =  a1 * y0 + b1 * ( u + u0 );

		y0 = y1;
		u0 = u;

		return y1;
	}


	/***********************************************/
	/*------------------�Ƕȼ��--------------------*/
	/***********************************************/
	//Read position data from AD2S1210
	float ReadTheta( void )
	{
		static unsigned int Posbuf[3] = {0,0,0};
		static unsigned int high8b = 0, low8b = 0;
		static unsigned int Theta_tmp = 0
		static float Theta_e = 0, Theta = 0;

		AD2S1210SelectMode(POSITION);
		ReadFromAD2S1210(POSITION, POS_VEL, Posbuf);

		high8b = ( Posbuf[0] << 8 ) & 0xFF00;
		low8b = Posbuf[1] & 0x00FF;

		Theta_tmp = ( high8b | low8b );

//		Theta = (float) Theta_tmp * 2.876213977e-4;			// 14bits�����ֱ���, ����Ϊ�Ƕ�ֵ��2 * pi / 2^16 * 3  = 2.876213977e-4 (rad/LSB)
		Theta = (float) Theta_tmp * 3.8349519697e-4;			// 14bits�����ֱ���, ����Ϊ�Ƕ�ֵ��2 * pi / 2^16 * 4  = 2.876213977e-4 (rad/LSB)
        jiaodu = Theta;

        if( Theta >= 6.0 * pi )
			Theta_e = Theta - 6.0 * pi;
        else if( Theta >= 4.0 * pi )
			Theta_e = Theta - 4.0 * pi;
		else if ( Theta >= 2.0 * pi )
			Theta_e = Theta - 2.0 * pi;
		else
			Theta_e = Theta;

		return Theta_e;

	}

	/***********************************************/
	/*------------------�Ƕȼ��--------------------*/
	/***********************************************/
	//Read position data from AD2S1210
	float ReadTheta_turn( void )
	{
		static unsigned int Posbuf[3] = {0,0,0};
		static unsigned int high8b = 0, low8b = 0;
		static unsigned int Theta_tmp = 0;
		static float Theta_e = 0, Theta = 0;

		AD2S1210SelectMode(POSITION);
		ReadFromAD2S1210(POSITION, POS_VEL, Posbuf);

		high8b = ( Posbuf[0] << 8 ) & 0xFF00;
		low8b = Posbuf[1] & 0x00FF;

		Theta_tmp = ( high8b | low8b );

	//	Theta = (float) Theta_tmp * 2.876213977e-4;			// 14bits�����ֱ���, ����Ϊ�Ƕ�ֵ��2 * pi / 2^16 * 3  = 2.876213977e-4 (rad/LSB)
		Theta = (float) Theta_tmp * 3.8349519697e-4;		// 14bits�����ֱ���, ����Ϊ�Ƕ�ֵ��2 * pi / 2^16 * 4  = 2.876213977e-4 (rad/LSB)
//	    jiaodu = Theta;
//		if( Theta >= 16.0 * pi )
//			Theta_e = Theta - 16.0 * pi;
//		else if ( Theta >= 8.0 * pi )
//			Theta_e = Theta - 8.0 * pi;
//		else
//			Theta_e = Theta;

		return Theta;

	}

	/***********************************************/
	/*-------------------ת�ټ��-------------------*/
	/***********************************************/
	//Read velocity data from AD2S1210
	float ReadVelocity( void )
	{
		const float Omega_LIM = 20000.0;

		static unsigned int Velbuf[3]={0,0,0};
		static unsigned int high8b = 0, low8b = 0;
		static int velocity = 0;
		static float Omega = 0;
//		static float error = 0;

		AD2S1210SelectMode(VELOCITY);
		ReadFromAD2S1210(VELOCITY, POS_VEL, Velbuf);

		high8b = ( Velbuf[0] << 8 ) & 0xFF00;
		low8b = Velbuf[1] & 0x00FF;

		velocity = (int)(high8b | low8b);
//      error = Velbuf[3];

		if ( abs(velocity) < 10 )
			Omega = 0.0;	// ���ٶȻ�����rpm���������500rps�����ֱ���2^13=8192��500*60/8192/4=9.15527344e-1 (rpm/LSB)
		else
			Omega = (float)velocity * 9.15527344e-1;

		if( Omega > Omega_LIM )
			Omega = Omega_LIM;
		else if( Omega < -Omega_LIM )
			Omega = -Omega_LIM;

		return Omega;
	}


	/*********************************************************/
	/*-----------------25us��ʱ�жϷ������-------------------*/
	/*********************************************************/

	interrupt void cpu_timer0_isr(void)
	{
		/*-----��ʱ�����������-----*/
		seq_adc();

		// ��������˲�
		i_A = SecOrderFilter_IA( IA_tmp );
		i_B = SecOrderFilter_IB( IB_tmp );
		i_C = SecOrderFilter_IC( IC_tmp );
		i_D = SecOrderFilter_ID( ID_tmp );

		Timer_200us ++;
		Timer_50us ++;
		Timer_500ms++;
		Timer_100us++;
		Timer_850us ++;

	    CpuTimer0.InterruptCount++;

	    // Acknowledge this interrupt to receive more interrupts from group 1
	    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
	}

	// CPU02toCPU01IPC0IntHandler - Handles Data Word Reads/Writes
	__interrupt void CPU02toCPU01IPC0IntHandler(void)
	{
	    tIpcMessage sMessage;

	    // Continue processing messages as long as CPU02toCPU01 GetBuffer1 is full
	    while(IpcGet(&g_sIpcController1, &sMessage, DISABLE_BLOCKING)!= STATUS_FAIL)
	    {
	        switch (sMessage.ulcommand)
	        {
	            case IPC_SET_BITS:
	                 IPCRtoLSetBits(&sMessage);
	                 break;
	            case IPC_CLEAR_BITS:
	                IPCRtoLClearBits(&sMessage);
	                break;
	            case IPC_DATA_WRITE:
	                IPCRtoLDataWrite(&sMessage);
	                break;
	            case IPC_DATA_READ:
	                IPCRtoLDataRead(&g_sIpcController1, &sMessage, ENABLE_BLOCKING);
	                break;
	            case IPC_FUNC_CALL:
	                IPCRtoLFunctionCall(&sMessage);
	                break;
	            default:
//	                ErrorFlag = 1;
	                break;
	        }
	    }

	    // Acknowledge IPC INT0 Flag and PIE to receive more interrupts
	    IpcRegs.IPCACK.bit.IPC0 = 1;
	    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
	}

	// CPU02toCPU01IPC1IntHandler - Handles Data Block Reads/Writes
	__interrupt void CPU02toCPU01IPC1IntHandler (void)
	{
	    tIpcMessage sMessage;

	    // Continue processing messages as long as CPU02toCPU01 GetBuffer2 is full
	    while(IpcGet(&g_sIpcController2, &sMessage, DISABLE_BLOCKING)!= STATUS_FAIL)
	    {
	        switch (sMessage.ulcommand)
	        {
	            case IPC_SET_BITS_PROTECTED:
	                IPCRtoLSetBits_Protected(&sMessage);       // Processes
	                                                           // IPCReqMemAccess()
	                                                           // function
	                break;
	            case IPC_CLEAR_BITS_PROTECTED:
	                IPCRtoLClearBits_Protected(&sMessage);     // Processes
	                                                           // IPCReqMemAccess()
	                                                           // function
	                break;
	            case IPC_BLOCK_WRITE:
	                IPCRtoLBlockWrite(&sMessage);
	                omega_g = usCPU01Buffer[0];
	        	    for(counter1 = 0; counter1 <64; counter1++)
	        	    {
	        	    	usCPU01Buffer1[counter1] = usCPU01Buffer[counter1];
	        	    }

	                break;
	            case IPC_BLOCK_READ:
	        	    usCPU01Buffer[0] = i_A;
	        	    usCPU01Buffer[1] = i_B;
	        	    usCPU01Buffer[2] = i_C;
	        	    usCPU01Buffer[3] = i_D;
	        	    usCPU01Buffer[4] = theta;
	        	    usCPU01Buffer[5] = omega_g;
	                IPCRtoLBlockRead(&sMessage);
	                break;
	            default:
//	                ErrorFlag = 1;
	                break;
	        }
	    }


	    // Acknowledge IPC INT1 Flag and PIE to receive more interrupts
	    IpcRegs.IPCACK.bit.IPC1 = 1;
	    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
	}

	//
	// End of file
	//



	//===========================================================================
	// No more.
	//===========================================================================
