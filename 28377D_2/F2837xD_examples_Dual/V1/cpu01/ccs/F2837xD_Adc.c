//###########################################################################
// FILE:   F2837xD_Adc.c
// TITLE:  F2837xD Adc Support Functions.
//###########################################################################
// $TI Release: F2837xD Support Library v190 $
// $Release Date: Mon Feb  1 16:51:57 CST 2016 $
// $Copyright: Copyright (C) 2013-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "F2837xD_device.h"     // F2837xD Headerfile Include File
#include "F2837xD_Examples.h"   // F2837xD Examples Include File
#include "Common_Definitions.h"     // Common Definitions Headerfile Include File

//Write ADC configurations and power up the ADC for both ADC A and ADC B
void InitAdc(void)
{
	EALLOW;

	//write configurations
	AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
	AdcbRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
	AdccRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
	AdcdRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4

    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCD, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

	//Set pulse positions to late
	AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;
	AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;
	AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;
	AdcdRegs.ADCCTL1.bit.INTPULSEPOS = 1;

	//power up the ADC
	AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
	AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;
	AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;
	AdcdRegs.ADCCTL1.bit.ADCPWDNZ = 1;

	//delay for 1ms to allow ADC time to power up
	DELAY_US(1000);

	EDIS;

	SetupADC();
}

/* 
* Set the resolution and signalmode for a given ADC. This will ensure that
* the correct trim is loaded. 
*/
void AdcSetMode(Uint16 adc, Uint16 resolution, Uint16 signalmode)
{
	Uint16 adcOffsetTrimOTPIndex; //index into OTP table of ADC offset trims
	Uint16 adcOffsetTrim; //temporary ADC offset trim
	
	//re-populate INL trim
	CalAdcINL(adc);
	
	if(0xFFFF != *((Uint16*)GetAdcOffsetTrimOTP)){
		//offset trim function is programmed into OTP, so call it

		//calculate the index into OTP table of offset trims and call
		//function to return the correct offset trim
		adcOffsetTrimOTPIndex = 4*adc + 2*resolution + 1*signalmode;
		adcOffsetTrim = (*GetAdcOffsetTrimOTP)(adcOffsetTrimOTPIndex);
	}
	else {
		//offset trim function is not populated, so set offset trim to 0
		adcOffsetTrim = 0;
	}

	//Apply the resolution and signalmode to the specified ADC.
	//Also apply the offset trim and, if needed, linearity trim correction.
	switch(adc){
		case ADC_ADCA:
			AdcaRegs.ADCCTL2.bit.RESOLUTION = resolution;
			AdcaRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
			AdcaRegs.ADCOFFTRIM.all = adcOffsetTrim;
			if(ADC_RESOLUTION_12BIT == resolution){

				//12-bit linearity trim workaround
				AdcaRegs.ADCINLTRIM1 &= 0xFFFF0000;
				AdcaRegs.ADCINLTRIM2 &= 0xFFFF0000;
				AdcaRegs.ADCINLTRIM4 &= 0xFFFF0000;
				AdcaRegs.ADCINLTRIM5 &= 0xFFFF0000;
			}
		break;
		case ADC_ADCB:
			AdcbRegs.ADCCTL2.bit.RESOLUTION = resolution;
			AdcbRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
			AdcbRegs.ADCOFFTRIM.all = adcOffsetTrim;
			if(ADC_RESOLUTION_12BIT == resolution){

				//12-bit linearity trim workaround
				AdcbRegs.ADCINLTRIM1 &= 0xFFFF0000;
				AdcbRegs.ADCINLTRIM2 &= 0xFFFF0000;
				AdcbRegs.ADCINLTRIM4 &= 0xFFFF0000;
				AdcbRegs.ADCINLTRIM5 &= 0xFFFF0000;
			}
		break;
		case ADC_ADCC:
			AdccRegs.ADCCTL2.bit.RESOLUTION = resolution;
			AdccRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
			AdccRegs.ADCOFFTRIM.all = adcOffsetTrim;
			if(ADC_RESOLUTION_12BIT == resolution){

				//12-bit linearity trim workaround
				AdccRegs.ADCINLTRIM1 &= 0xFFFF0000;
				AdccRegs.ADCINLTRIM2 &= 0xFFFF0000;
				AdccRegs.ADCINLTRIM4 &= 0xFFFF0000;
				AdccRegs.ADCINLTRIM5 &= 0xFFFF0000;
			}
		break;
		case ADC_ADCD:
			AdcdRegs.ADCCTL2.bit.RESOLUTION = resolution;
			AdcdRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
			AdcdRegs.ADCOFFTRIM.all = adcOffsetTrim;
			if(ADC_RESOLUTION_12BIT == resolution){

				//12-bit linearity trim workaround
				AdcdRegs.ADCINLTRIM1 &= 0xFFFF0000;
				AdcdRegs.ADCINLTRIM2 &= 0xFFFF0000;
				AdcdRegs.ADCINLTRIM4 &= 0xFFFF0000;
				AdcdRegs.ADCINLTRIM5 &= 0xFFFF0000;
			}
		break;
	}
}

//setup the ADC to continuously convert on one channel
void SetupADC(void)
{
	Uint16 acqps;

	//determine minimum acquisition window (in SYSCLKS) based on resolution
	if(ADC_RESOLUTION_12BIT == AdcaRegs.ADCCTL2.bit.RESOLUTION){
		acqps = 14; //75ns
	}
	else { //resolution is 16-bit
		acqps = 63; //320ns
	}

    EALLOW;
    //顺序采样
//    AdcaRegs.ADCSAMPLEMODE.all = 0x0000;
    //软件启动
    AdcaRegs.ADCINTSOCSEL1.all = 0x0000;
    AdcaRegs.ADCINTSOCSEL2.all = 0x0000;
    AdcbRegs.ADCINTSOCSEL1.all = 0x0000;
    AdcbRegs.ADCINTSOCSEL2.all = 0x0000;
    AdccRegs.ADCINTSOCSEL1.all = 0x0000;
    AdccRegs.ADCINTSOCSEL2.all = 0x0000;
    AdcdRegs.ADCINTSOCSEL1.all = 0x0000;
    AdcdRegs.ADCINTSOCSEL2.all = 0x0000;
/*
    AdcaRegs.ADCSOC0CTL.all= 0x003F;
    AdcaRegs.ADCSOC1CTL.all= 0x007F;
    AdcaRegs.ADCSOC2CTL.all= 0x00BF;
    AdcaRegs.ADCSOC3CTL.all= 0x00FF;
    AdcaRegs.ADCSOC4CTL.all= 0x013F;
    AdcaRegs.ADCSOC5CTL.all= 0x017F;
    AdcaRegs.ADCSOC6CTL.all= 0x01BF;
    AdcaRegs.ADCSOC7CTL.all= 0x01FF;
    AdcaRegs.ADCSOC8CTL.all= 0x023F;
    AdcaRegs.ADCSOC9CTL.all= 0x027F;
    AdcaRegs.ADCSOC10CTL.all= 0x02BF;
    AdcaRegs.ADCSOC11CTL.all= 0x02FF;
    AdcaRegs.ADCSOC12CTL.all= 0x033F;
    AdcaRegs.ADCSOC13CTL.all= 0x037F;
    AdcaRegs.ADCSOC14CTL.all= 0x03BF;
    AdcaRegs.ADCSOC15CTL.all= 0x03FF;
*/
    //通道选择
    // Main_U measure   // VA3 Measure   // VA4 measure // Main_I measur // VA1 measure // VA2 measure
    AdcaRegs.ADCSOC0CTL.bit.CHSEL= 0;
    AdcaRegs.ADCSOC1CTL.bit.CHSEL= 1;
    AdcaRegs.ADCSOC2CTL.bit.CHSEL= 2;
    AdcaRegs.ADCSOC3CTL.bit.CHSEL= 3;
    AdcaRegs.ADCSOC4CTL.bit.CHSEL= 4;
    AdcaRegs.ADCSOC5CTL.bit.CHSEL= 5;
    AdcaRegs.ADCSOC14CTL.bit.CHSEL= 14;
    AdcaRegs.ADCSOC15CTL.bit.CHSEL= 15;
    AdcbRegs.ADCSOC0CTL.bit.CHSEL= 0;
    AdcbRegs.ADCSOC1CTL.bit.CHSEL= 1;
    AdcbRegs.ADCSOC2CTL.bit.CHSEL= 2;
    AdcbRegs.ADCSOC3CTL.bit.CHSEL= 3;
    AdccRegs.ADCSOC2CTL.bit.CHSEL= 2;
    AdccRegs.ADCSOC3CTL.bit.CHSEL= 3;
    AdccRegs.ADCSOC4CTL.bit.CHSEL= 4;
    AdcdRegs.ADCSOC0CTL.bit.CHSEL= 0;
    AdcdRegs.ADCSOC1CTL.bit.CHSEL= 1;
    AdcdRegs.ADCSOC2CTL.bit.CHSEL= 2;
    AdcdRegs.ADCSOC3CTL.bit.CHSEL= 3;
    AdcdRegs.ADCSOC4CTL.bit.CHSEL= 4;
    //采样窗
    AdcaRegs.ADCSOC0CTL.bit.ACQPS= acqps;
    AdcaRegs.ADCSOC1CTL.bit.ACQPS= acqps;
    AdcaRegs.ADCSOC2CTL.bit.ACQPS= acqps;
    AdcaRegs.ADCSOC3CTL.bit.ACQPS= acqps;
    AdcaRegs.ADCSOC4CTL.bit.ACQPS= acqps;
    AdcaRegs.ADCSOC5CTL.bit.ACQPS= acqps;
    AdcaRegs.ADCSOC14CTL.bit.ACQPS= acqps;
    AdcaRegs.ADCSOC15CTL.bit.ACQPS= acqps;
    AdcbRegs.ADCSOC0CTL.bit.ACQPS= acqps;
    AdcbRegs.ADCSOC1CTL.bit.ACQPS= acqps;
    AdcbRegs.ADCSOC2CTL.bit.ACQPS= acqps;
    AdcbRegs.ADCSOC3CTL.bit.ACQPS= acqps;
    AdccRegs.ADCSOC2CTL.bit.ACQPS= acqps;
    AdccRegs.ADCSOC3CTL.bit.ACQPS= acqps;
    AdccRegs.ADCSOC4CTL.bit.ACQPS= acqps;
    AdcdRegs.ADCSOC0CTL.bit.ACQPS= acqps;
    AdcdRegs.ADCSOC1CTL.bit.ACQPS= acqps;
    AdcdRegs.ADCSOC2CTL.bit.ACQPS= acqps;
    AdcdRegs.ADCSOC3CTL.bit.ACQPS= acqps;
    AdcdRegs.ADCSOC4CTL.bit.ACQPS= acqps;

    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL= 0;
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL= 0;
    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL= 0;
    AdcaRegs.ADCSOC3CTL.bit.TRIGSEL= 0;
    AdcaRegs.ADCSOC4CTL.bit.TRIGSEL= 0;
    AdcaRegs.ADCSOC5CTL.bit.TRIGSEL= 0;
    AdcaRegs.ADCSOC14CTL.bit.TRIGSEL= 0;
    AdcaRegs.ADCSOC15CTL.bit.TRIGSEL= 0;
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL= 0;
    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL= 0;
    AdcbRegs.ADCSOC2CTL.bit.TRIGSEL= 0;
    AdcbRegs.ADCSOC3CTL.bit.TRIGSEL= 0;
    AdccRegs.ADCSOC2CTL.bit.TRIGSEL= 0;
    AdccRegs.ADCSOC3CTL.bit.TRIGSEL= 0;
    AdccRegs.ADCSOC4CTL.bit.TRIGSEL= 0;
    AdcdRegs.ADCSOC0CTL.bit.TRIGSEL= 0;
    AdcdRegs.ADCSOC1CTL.bit.TRIGSEL= 0;
    AdcdRegs.ADCSOC2CTL.bit.TRIGSEL= 0;
    AdcdRegs.ADCSOC3CTL.bit.TRIGSEL= 0;
    AdcdRegs.ADCSOC4CTL.bit.TRIGSEL= 0;

    EDIS;
}

/* 
* Loads INL trim values from OTP into the trim registers of the specified ADC.
* Use only as part of AdcSetMode function, since linearity trim correction
* is needed for some modes.
*/
void CalAdcINL(Uint16 adc)
{
	switch(adc){
		case ADC_ADCA:
			if(0xFFFF != *((Uint16*)CalAdcaINL)){
				//trim function is programmed into OTP, so call it
				(*CalAdcaINL)();
			}
			else {
				//do nothing, no INL trim function populated
			}
			break;
		case ADC_ADCB:
			if(0xFFFF != *((Uint16*)CalAdcbINL)){
				//trim function is programmed into OTP, so call it
				(*CalAdcbINL)();
			}
			else {
				//do nothing, no INL trim function populated
			}
			break;
		case ADC_ADCC:
			if(0xFFFF != *((Uint16*)CalAdccINL)){
				//trim function is programmed into OTP, so call it
				(*CalAdccINL)();
			}
			else {
				//do nothing, no INL trim function populated
			}
			break;
		case ADC_ADCD:
			if(0xFFFF != *((Uint16*)CalAdcdINL)){
				//trim function is programmed into OTP, so call it
				(*CalAdcdINL)();
			}
			else {
				//do nothing, no INL trim function populated
			}
			break;
	}
}

void seq_adc( void )
{
	//static long int ADC_data1 = 0, ADC_data2 = 0, ADC_data3 = 0, ADC_data4 = 0, ADC_data5 = 0, ADC_data6 = 0, ADC_data7 = 0, ADC_data8 = 0;
	static float VA1_tmp = 0, VA2_tmp = 0, VA3_tmp = 0, VA4_tmp = 0, VA5_tmp = 0, VA6_tmp = 0, VA7_tmp = 0, VA8_tmp = 0, VA9_tmp = 0, VA10_tmp = 0, VA11_tmp = 0, VA12_tmp = 0, VA13_tmp = 0, VA14_tmp = 0, VA15_tmp = 0, VA16_tmp = 0, VA17_tmp = 0, VA18_tmp = 0, VA19_tmp = 0, VA20_tmp = 0;
//	static long int RT1_tmp = 0, RT2_tmp = 0, RT3_tmp = 0;
	float VAL_tmp = 0, VAR_tmp = 0, VBL_tmp = 0,VBR_tmp = 0, VCL_tmp = 0, VCR_tmp = 0;

	AdcaRegs.ADCSOCFRC1.all = 0xC03F;  // Force Start SOC0-15 to begin ping-pong sampling
	AdcbRegs.ADCSOCFRC1.all = 0x000F;  // Force Start SOC0-15 to begin ping-pong sampling
	AdccRegs.ADCSOCFRC1.all = 0x001C;  // Force Start SOC0-15 to begin ping-pong sampling
	AdcdRegs.ADCSOCFRC1.all = 0x001F;  // Force Start SOC0-15 to begin ping-pong sampling

	VA1_tmp =  AdcaResultRegs.ADCRESULT0;       // A_V_R
	VA2_tmp =  AdcaResultRegs.ADCRESULT1;       // A_V_L
	VA3_tmp =  AdcaResultRegs.ADCRESULT2;       // C_C
	VA4_tmp =  AdcaResultRegs.ADCRESULT3;       // B_C
	VA5_tmp =  AdcaResultRegs.ADCRESULT4;       // A_C
	VA6_tmp =  AdcaResultRegs.ADCRESULT5;       // VIM
	VA7_tmp =  AdcaResultRegs.ADCRESULT14;      // A_V_R
	VA8_tmp =  AdcaResultRegs.ADCRESULT15;      // B_V_L
	VA9_tmp =  AdcbResultRegs.ADCRESULT0;       // B_V_R
	VA10_tmp = AdcbResultRegs.ADCRESULT1;       // C_V_L
	VA11_tmp = AdcbResultRegs.ADCRESULT2;       // C_V_L
	VA12_tmp = AdcbResultRegs.ADCRESULT3;       // C_V_L
	VA13_tmp = AdccResultRegs.ADCRESULT2;       // C_T
	VA14_tmp = AdccResultRegs.ADCRESULT3;       // B_T
	VA15_tmp = AdccResultRegs.ADCRESULT4;       // A-T
	VA16_tmp = AdcdResultRegs.ADCRESULT0;       // C_V_R
	VA17_tmp = AdcdResultRegs.ADCRESULT1;       // C_V_R
	VA18_tmp = AdcdResultRegs.ADCRESULT2;       // C_V_R
	VA19_tmp = AdcdResultRegs.ADCRESULT3;       // MAINV
	VA20_tmp = AdcdResultRegs.ADCRESULT4;       // MAINV
/*
    // 温度检测计算，单位°
	RT1_tmp = VA15_tmp;
	RT2_tmp = VA14_tmp;
	RT3_tmp = VA13_tmp;

    T1=1182.3/(5-RT1_tmp*3.3/4096)-316.8;
    T2=1182.3/(5-RT2_tmp*3.3/4096)-316.8;
    T3=1182.3/(5-RT3_tmp*3.3/4096)-316.8;
*/
    // 电压检测计算，单位V
    Usys_tmp = (VA19_tmp + VA20_tmp) * 7.558136e-2 * 1.043927;
    VAL_tmp = VA2_tmp * 0.147679 * 1.0332;
    VAR_tmp = (VA1_tmp + VA7_tmp) * 0.147679 * 0.48085;
    VBL_tmp = VA8_tmp * 0.147679 * 0.9851;
    VBR_tmp = VA9_tmp * 0.147679 * 0.9897;
    VCL_tmp = (VA10_tmp + VA11_tmp + VA12_tmp) * 0.147679 * 0.308319;
    VCR_tmp = (VA16_tmp + VA17_tmp + VA18_tmp) * 0.147679 * 0.313533;

    V_A = VAL_tmp - VAR_tmp;
    V_B = VBL_tmp - VBR_tmp;
    V_C = VCL_tmp - VCR_tmp;

	// 电流检测计算，单位A，系数 6.84814453e-2 = 100/2*5.1/3*3.3/4096
    IA_tmp = ( ( VA5_tmp - 1787.0 ) * 6.84814453e-2 ) * 1.0399562 * 0.9969;// * 1.0019;
    IB_tmp = ( ( VA4_tmp - 1793.0 ) * 6.84814453e-2 ) * 1.0303483 * 0.995;// * 0.9953;
    IC_tmp = ( ( VA3_tmp - 1782.0 ) * 6.84814453e-2 ) * 1.051475 * 0.9759;// * 0.9982;
    I_main_tmp = ( (VA6_tmp - 1785.0) * 6.98510742e-2 * 1.0416);

    if( I_main_tmp < 0 )
    	I_main_tmp = 0;

}
