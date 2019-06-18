// TI File $Revision: /main/8 $
// Checkin $Date: November 16, 2011   10:42:30 $
//###########################################################################
//
// FILE:   Common_Definitions.h
//
// TITLE:  DSP280x Common Definitions.
//
//###########################################################################
// $TI Release: DSP280x Header Files V1.60 $
// $Release Date:  November 16, 2011   10:42:30 $
//###########################################################################
// ECCTL1 ( ECAP Control Reg 1)
//==========================
// CAPxPOL bits
#define EC_RISING 0x0

#define EC_FALLING 0x1
// CTRRSTx bits
#define EC_ABS_MODE 0x0
#define EC_DELTA_MODE 0x1
// PRESCALE bits
#define EC_BYPASS 0x0
#define EC_DIV1 0x0
#define EC_DIV2 0x1
#define EC_DIV4 0x2
#define EC_DIV6 0x3
#define EC_DIV8 0x4
#define EC_DIV10 0x5
// ECCTL2 ( ECAP Control Reg 2)
//==========================
// CONT/ONESHOT bit
#define EC_CONTINUOUS 0x0
#define EC_ONESHOT 0x1
// STOPVALUE bit
#define EC_EVENT1 0x0
#define EC_EVENT2 0x1
#define EC_EVENT3 0x2
#define EC_EVENT4 0x3
// RE-ARM bit
#define EC_ARM 0x1
// TSCTRSTOP bit
#define EC_FREEZE 0x0
#define EC_RUN 0x1

// SYNCO_SEL bit
#define EC_SYNCIN 0x0
#define EC_CTR_PRD 0x1
#define EC_SYNCO_DIS 0x2
// CAP/APWM mode bit
#define EC_CAP_MODE 0x0
#define EC_APWM_MODE 0x1
// APWMPOL bit
#define EC_ACTV_HI 0x0
#define EC_ACTV_LO 0x1
// Generic
#define EC_DISABLE 0x0
#define EC_ENABLE 0x1
#define EC_FORCE 0x1


// Configure the period for each timer
#define EPWM1_TIMER_TBPRD  2000  // Period register, 20KHz
#define EPWM1_MAX_CMPA     1000
#define EPWM1_MIN_CMPA     1000
#define EPWM1_MAX_CMPB     1000
#define EPWM1_MIN_CMPB     1000

#define EPWM2_TIMER_TBPRD  2000  // Period register
#define EPWM2_MAX_CMPA     1000
#define EPWM2_MIN_CMPA     1000
#define EPWM2_MAX_CMPB     1000
#define EPWM2_MIN_CMPB     1000

#define EPWM3_TIMER_TBPRD  2000  // Period register
#define EPWM3_MAX_CMPA     1000
#define EPWM3_MIN_CMPA     1000
#define EPWM3_MAX_CMPB     1000
#define EPWM3_MIN_CMPB     1000

#define EPWM4_TIMER_TBPRD  2500  // Period register
#define EPWM4_MAX_CMPA     1250
#define EPWM4_MIN_CMPA     1250
#define EPWM4_MAX_CMPB     1250
#define EPWM4_MIN_CMPB     1250

// To keep track of which way the compare value is moving
#define EPWM_CMP_UP   1
#define EPWM_CMP_DOWN 0


typedef struct
{
   volatile struct EPWM_REGS *EPwmRegHandle;
   Uint16 EPwm_CMPA_Direction;
   Uint16 EPwm_CMPB_Direction;
   Uint16 EPwmTimerIntCount;
   Uint16 EPwmMaxCMPA;
   Uint16 EPwmMinCMPA;
   Uint16 EPwmMaxCMPB;
   Uint16 EPwmMinCMPB;   
}EPWM_INFO;

#define CPU_FREQ 	160E6
#define LSPCLK_FREQ CPU_FREQ/2
#define SCI_FREQ 	115200
#define SCI_PRD 	(LSPCLK_FREQ/(SCI_FREQ*8))-1
//#define FLASH_MEMCOPY 0

//SCI Communication frame structure
struct TX_FRAME_REG
{
	Uint16 SYNC[2];
	Uint16 FUNC;
	Uint16 DATA[12];
	Uint16 CSUM;

};

struct RX_FRAME_REG
{
	Uint16 FUNC;
	Uint16 DATA[4];		// Data of frame
	Uint16 CSUM;			// Checksum of frame
};

struct COM_VAR
{
	unsigned int frame_cnt;
	unsigned int speed_cmd;
	unsigned int speed_fbk;
	unsigned int sys_voltage;
	unsigned int sys_current;
	unsigned int PhA_current;
	unsigned int PhB_current;
	unsigned int PhC_current;
	unsigned int PhD_current;
	unsigned int PhAC_current;
	unsigned int PhBD_current;
	int IA_d;
	int IA_q;
	int IB_d;
	int IB_q;
	int IC_d;
	int IC_q;
	int ID_d;
	int ID_q;
	int I_PhA[10];
	int I_PhB[10];
	int I_PhC[10];
	int I_PhD[10];
	unsigned int sys_status;
};

extern volatile struct TX_FRAME_REG TX_FRAME,TX_UPLOAD;
extern volatile struct RX_FRAME_REG RX_FRAME;
extern volatile struct COM_VAR McomData;
//extern volatile g_sIpcController1;
//extern volatile g_sIpcController2;

#define ENABLE1 GpioDataRegs.GPACLEAR.bit.GPIO27
#define ENABLE2 GpioDataRegs.GPACLEAR.bit.GPIO26
#define ENABLE3 GpioDataRegs.GPBCLEAR.bit.GPIO44
#define DISABLE1 GpioDataRegs.GPASET.bit.GPIO27
#define DISABLE2 GpioDataRegs.GPASET.bit.GPIO26
#define DISABLE3 GpioDataRegs.GPBSET.bit.GPIO44

//#define DAT1 GpioDataRegs.GPADAT.bit.GPIO15
//#define DAT2 GpioDataRegs.GPADAT.bit.GPIO19
//#define DAT3 GpioDataRegs.GPADAT.bit.GPIO27
//#define DAT4 GpioDataRegs.GPBDAT.bit.GPIO41
//#define DAT5 GpioDataRegs.GPBDAT.bit.GPIO39
//#define DAT6 GpioDataRegs.GPBDAT.bit.GPIO52
//#define DAT7 GpioDataRegs.GPBDAT.bit.GPIO53
//#define DAT8 GpioDataRegs.GPBDAT.bit.GPIO54
//#define DAT9 GpioDataRegs.GPBDAT.bit.GPIO55
//#define DAT10 GpioDataRegs.GPBDAT.bit.GPIO57
//#define DAT11 GpioDataRegs.GPBDAT.bit.GPIO42
//#define DAT12 GpioDataRegs.GPBDAT.bit.GPIO43
#define BRAKE GpioDataRegs.GPADAT.bit.GPIO16
#define DE_B GpioDataRegs.GPADAT.bit.GPIO24
#define RE_B GpioDataRegs.GPADAT.bit.GPIO25
#define DE_A GpioDataRegs.GPCDAT.bit.GPIO93
#define RE_A GpioDataRegs.GPCDAT.bit.GPIO91
//#define H_A GpioDataRegs.GPBDAT.bit.GPIO45
#define H_A GpioDataRegs.GPCDAT.bit.GPIO87
//#define PV GpioDataRegs.GPADAT.bit.GPIO27
//#define Test_IO_TOG GpioDataRegs.GPATOGGLE.bit.GPIO5
//#define WDT_TOG GpioDataRegs.GPATOGGLE.bit.GPIO14
#define WDT_TOG_Right GpioDataRegs.GPCTOGGLE.bit.GPIO66
#define WDT_TOG_Left  GpioDataRegs.GPDTOGGLE.bit.GPIO99

#define LIM_omega 20000
#define xor(a,b) ((~a && b) || (a && ~b))

// Prototype statements for functions found within this file.
interrupt void cpu_timer0_isr(void);
interrupt void cpu_timer1_isr(void);
__interrupt void CPU02toCPU01IPC0IntHandler(void);
__interrupt void CPU02toCPU01IPC1IntHandler(void);
__interrupt void CPU01toCPU02IPC0IntHandler(void);
__interrupt void CPU01toCPU02IPC1IntHandler(void);

void SCIA_TX(void);
void SCIA_RX(void);
void seq_adc(void);
void seq_SDFM(void);
float ReadTheta( void );
float ReadTheta_turn( void );
float ReadVelocity( void );
float APR( float, float );
float ASR( float, float );
float AYR( float, float );
float ATR( float, float, float );
float FirstOrderFilter_psi(float);
float FirstOrderFilter_dphi(float);
float FirstOrderFilter_Usys(float);
float FirstOrderFilter_Isys(float);
float SecOrderFilter_IX(float);
float SecOrderFilter_IA(float);
float SecOrderFilter_IB(float);
float SecOrderFilter_IC(float);
float SecOrderFilter_VA(float);
float SecOrderFilter_VB(float);
float SecOrderFilter_VC(float);
float SecOrderFilter_ID(float);
float SecOrderFilter_Theta_cmd(float);
float SecOrderFilter_omega(float);
int fsin( long );

extern unsigned int rdataA[64], tdataA[16];					// Receive buffer for SCI-A, SCI-B
extern unsigned int rdataA_head,rdataA_tail,TX_flag,RX_flag,turns;	// Receive buffer pointer
extern unsigned int Hall_status;
extern float IA, IB, IC, ID, U_sci, I_main, V_A, V_B, V_C, Ai, Bi, Ci, Di;
extern float IA_tmp, IB_tmp, IC_tmp, Usys_tmp, I_main_tmp;
extern float omega_g, omega, omega_tmp, omega_r, theta_encoder;
extern float T1, T2, T3,jiaodu,theta_dq;

//===========================================================================
// End of file.
//===========================================================================
