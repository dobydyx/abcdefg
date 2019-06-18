/*
 * InitADCx.c
 *
 *  Created on: 2018年1月7日
 *      Author: Ezreal, Doby
 */

/************************************************************
Description:初始化ADC
                              线电流：A4, C2, C4, D0, B2, 14
                              相电压：A3, A5, C3, B3, B0, 15
                              绕组温度：A0, A1, B1, D2, D3, D4
                              母线电压：A2, D1
                              片内温度传感器：A13
************************************************************/

#include "F28x_Project.h"
#include "CtrlParams.h"

void ConfigureADC(void)
{
    EALLOW;

//  ADCA
    //write configurations
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    //Set pulse positions to late
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    //power up the ADCs
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    //delay for 1ms to allow ADC time to power up
    DELAY_US(1000);

//  ADCB
    //write configurations
    AdcbRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    //Set pulse positions to late
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    //power up the ADCs
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    //delay for 1ms to allow ADC time to power up
    DELAY_US(1000);

//  ADCC
    //write configurations
    AdccRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    //Set pulse positions to late
    AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    //power up the ADCs
    AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    //delay for 1ms to allow ADC time to power up
    DELAY_US(1000);

//  ADCD
    //write configurations
    AdcdRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdcSetMode(ADC_ADCD, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    //Set pulse positions to late
    AdcdRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    //power up the ADCs
    AdcdRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    //delay for 1ms to allow ADC time to power up
    DELAY_US(1000);

    EDIS;
}

void SetupADCContinuous(void)
{
    EALLOW;

//  ADCA
    AdcaRegs.ADCSOC9CTL.bit.CHSEL = 0;      //Winding Temperature 1
    AdcaRegs.ADCSOC10CTL.bit.CHSEL = 1;     //Winding Temperature 2
    AdcaRegs.ADCSOC11CTL.bit.CHSEL = 2;     //Bus Voltage 1
    AdcaRegs.ADCSOC12CTL.bit.CHSEL = 3;     //Phase Voltage A
    AdcaRegs.ADCSOC13CTL.bit.CHSEL = 4;     //Line Current A
    AdcaRegs.ADCSOC14CTL.bit.CHSEL = 5;     //Phase Voltage B
    AdcaRegs.ADCSOC15CTL.bit.CHSEL = 13;    //IC Temperature

    AdcaRegs.ADCSOC9CTL.bit.ACQPS = 14;     //sample window is 15 SYSCLK cycles
    AdcaRegs.ADCSOC10CTL.bit.ACQPS = 14;    //sample window is 15 SYSCLK cycles
    AdcaRegs.ADCSOC11CTL.bit.ACQPS = 14;    //sample window is 15 SYSCLK cycles
    AdcaRegs.ADCSOC12CTL.bit.ACQPS = 14;    //sample window is 15 SYSCLK cycles
    AdcaRegs.ADCSOC13CTL.bit.ACQPS = 14;    //sample window is 15 SYSCLK cycles
    AdcaRegs.ADCSOC14CTL.bit.ACQPS = 14;    //sample window is 15 SYSCLK cycles
    AdcaRegs.ADCSOC15CTL.bit.ACQPS = 139;   //sample window is 140 SYSCLK cycles

    AdcaRegs.ADCINTSOCSEL2.bit.SOC9 = 1;    //ADCINT1 will trigger SOC9
    AdcaRegs.ADCINTSOCSEL2.bit.SOC10 = 1;   //ADCINT1 will trigger SOC10
    AdcaRegs.ADCINTSOCSEL2.bit.SOC11 = 1;   //ADCINT1 will trigger SOC11
    AdcaRegs.ADCINTSOCSEL2.bit.SOC12 = 1;   //ADCINT1 will trigger SOC12
    AdcaRegs.ADCINTSOCSEL2.bit.SOC13 = 1;   //ADCINT1 will trigger SOC13
    AdcaRegs.ADCINTSOCSEL2.bit.SOC14 = 1;   //ADCINT1 will trigger SOC14
    AdcaRegs.ADCINTSOCSEL2.bit.SOC15 = 1;   //ADCINT1 will trigger SOC15

    AdcaRegs.ADCINTSEL1N2.bit.INT1CONT = 1; //Enable Continuous Mode
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 15; //End of SOC15 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;    //Enable ADCINT1

    AdcaRegs.ADCSOCFRC1.all = 0xFE00;       //Start the ADCA

//ADCB
    AdcbRegs.ADCSOC12CTL.bit.CHSEL = 0;     //Phase Voltage E
    AdcbRegs.ADCSOC13CTL.bit.CHSEL = 1;     //Winding Temperature 3
    AdcbRegs.ADCSOC14CTL.bit.CHSEL = 2;     //Line Current E
    AdcbRegs.ADCSOC15CTL.bit.CHSEL = 3;     //Phase Voltage D

    AdcbRegs.ADCSOC12CTL.bit.ACQPS = 14;    //sample window is 15 SYSCLK cycles
    AdcbRegs.ADCSOC13CTL.bit.ACQPS = 14;    //sample window is 15 SYSCLK cycles
    AdcbRegs.ADCSOC14CTL.bit.ACQPS = 14;    //sample window is 15 SYSCLK cycles
    AdcbRegs.ADCSOC15CTL.bit.ACQPS = 14;    //sample window is 15 SYSCLK cycles

    AdcbRegs.ADCINTSOCSEL2.bit.SOC12 = 1;   //ADCINT1 will trigger SOC12
    AdcbRegs.ADCINTSOCSEL2.bit.SOC13 = 1;   //ADCINT1 will trigger SOC13
    AdcbRegs.ADCINTSOCSEL2.bit.SOC14 = 1;   //ADCINT1 will trigger SOC14
    AdcbRegs.ADCINTSOCSEL2.bit.SOC15 = 1;   //ADCINT1 will trigger SOC15

    AdcbRegs.ADCINTSEL1N2.bit.INT1CONT = 1; //Enable Continuous Mode
    AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = 15; //End of SOC15 will set INT1 flag
    AdcbRegs.ADCINTSEL1N2.bit.INT1E = 1;    //Enable ADCINT1

    AdcbRegs.ADCSOCFRC1.all = 0xF000;       //Start the ADCB

//ADCC
    AdccRegs.ADCSOC11CTL.bit.CHSEL = 2;     //Line Current B
    AdccRegs.ADCSOC12CTL.bit.CHSEL = 3;     //Phase Voltage C
    AdccRegs.ADCSOC13CTL.bit.CHSEL = 4;     //Line Current C
    AdccRegs.ADCSOC14CTL.bit.CHSEL = 14;    //Line Current F
    AdccRegs.ADCSOC15CTL.bit.CHSEL = 15;    //Phase Voltage F

    AdccRegs.ADCSOC11CTL.bit.ACQPS = 14;    //sample window is 15 SYSCLK cycles
    AdccRegs.ADCSOC12CTL.bit.ACQPS = 14;    //sample window is 15 SYSCLK cycles
    AdccRegs.ADCSOC13CTL.bit.ACQPS = 14;    //sample window is 15 SYSCLK cycles
    AdccRegs.ADCSOC14CTL.bit.ACQPS = 14;    //sample window is 15 SYSCLK cycles
    AdccRegs.ADCSOC15CTL.bit.ACQPS = 14;    //sample window is 15 SYSCLK cycles

    AdccRegs.ADCINTSOCSEL2.bit.SOC11 = 1;   //ADCINT1 will trigger SOC11
    AdccRegs.ADCINTSOCSEL2.bit.SOC12 = 1;   //ADCINT1 will trigger SOC12
    AdccRegs.ADCINTSOCSEL2.bit.SOC13 = 1;   //ADCINT1 will trigger SOC13
    AdccRegs.ADCINTSOCSEL2.bit.SOC14 = 1;   //ADCINT1 will trigger SOC14
    AdccRegs.ADCINTSOCSEL2.bit.SOC15 = 1;   //ADCINT1 will trigger SOC15

    AdccRegs.ADCINTSEL1N2.bit.INT1CONT = 1; //Enable Continuous Mode
    AdccRegs.ADCINTSEL1N2.bit.INT1SEL = 15; //End of SOC15 will set INT1 flag
    AdccRegs.ADCINTSEL1N2.bit.INT1E = 1;    //Enable ADCINT1

    AdccRegs.ADCSOCFRC1.all = 0xF800;       //Start the ADCC

//ADCD
    AdcdRegs.ADCSOC11CTL.bit.CHSEL = 0;     //Line Current D
    AdcdRegs.ADCSOC12CTL.bit.CHSEL = 1;     //Bus Voltage 2
    AdcdRegs.ADCSOC13CTL.bit.CHSEL = 2;     //Winding Temperature 4
    AdcdRegs.ADCSOC14CTL.bit.CHSEL = 3;     //Winding Temperature 5
    AdcdRegs.ADCSOC15CTL.bit.CHSEL = 4;     //Winding Temperature 6

    AdcdRegs.ADCSOC11CTL.bit.ACQPS = 14;    //sample window is 15 SYSCLK cycles
    AdcdRegs.ADCSOC12CTL.bit.ACQPS = 14;    //sample window is 15 SYSCLK cycles
    AdcdRegs.ADCSOC13CTL.bit.ACQPS = 14;    //sample window is 15 SYSCLK cycles
    AdcdRegs.ADCSOC14CTL.bit.ACQPS = 14;    //sample window is 15 SYSCLK cycles
    AdcdRegs.ADCSOC15CTL.bit.ACQPS = 14;    //sample window is 15 SYSCLK cycles

    AdcdRegs.ADCINTSOCSEL2.bit.SOC11 = 1;   //ADCINT1 will trigger SOC11
    AdcdRegs.ADCINTSOCSEL2.bit.SOC12 = 1;   //ADCINT1 will trigger SOC12
    AdcdRegs.ADCINTSOCSEL2.bit.SOC13 = 1;   //ADCINT1 will trigger SOC13
    AdcdRegs.ADCINTSOCSEL2.bit.SOC14 = 1;   //ADCINT1 will trigger SOC14
    AdcdRegs.ADCINTSOCSEL2.bit.SOC15 = 1;   //ADCINT1 will trigger SOC15

    AdcdRegs.ADCINTSEL1N2.bit.INT1CONT = 1; //Enable Continuous Mode
    AdcdRegs.ADCINTSEL1N2.bit.INT1SEL = 15; //End of SOC5 will set INT1 flag
    AdcdRegs.ADCINTSEL1N2.bit.INT1E = 1;    //Enable ADCINT1

    AdcdRegs.ADCSOCFRC1.all = 0xF800;       //Start the ADCD

    EDIS;
}

void InitADCs(void)
{
    ConfigureADC();
    SetupADCContinuous();
    InitTempSensor(3.0);
}

void LineCurSamp(float i[])
{
    /************************************************************
    Description:计算线电流采样值
    Input:none
    Output:i[]
    ************************************************************/
#if (PHASE == 3 | PHASE == 5 | PHASE == 6)                  //三相电流采样
    i[0] = ((int)AdcaResultRegs.ADCRESULT13 - 2003) / 20.075;
    i[1] = ((int)AdccResultRegs.ADCRESULT11 - 2012) / 20.075;
    i[2] = ((int)AdccResultRegs.ADCRESULT13 - 2010) / 20.075;    //(RESULT - 2047.5[偏置]) / (4095 / 200[量程(-100A~100A)])
#endif

#if (PHASE == 5 | PHASE == 6)                               //五相电流采样
    i[3] = ((int)AdcdResultRegs.ADCRESULT11 - 2028) / 20.075;
    i[4] = ((int)AdcbResultRegs.ADCRESULT14 - 2004) / 20.075;
#endif

#if (PHASE == 6)                                            //六相电流采样
    i[5] = ((int)AdccResultRegs.ADCRESULT14 - 1994) / 20.075;
#endif

//    extern float ob5,ob6,ob7;
//    ob5 = AdcaResultRegs.ADCRESULT13;
//    ob6 = AdccResultRegs.ADCRESULT11;
//    ob7 = AdccResultRegs.ADCRESULT13;
}

void PhaseVoltSamp(float u[])
{
    /************************************************************
    Description:计算相电压采样值
    Input:none
    Output:u[]
    ************************************************************/
#if (PWM_NUM == 3 | PWM_NUM == 5 | PWM_NUM == 6)            //三相电压采样
    u[0] = ((int)AdcaResultRegs.ADCRESULT12) * 0.173557;
    u[1] = ((int)AdcaResultRegs.ADCRESULT14) * 0.173557;
    u[2] = ((int)AdccResultRegs.ADCRESULT12) * 0.173557;           //(每单位1等于0.173557V)
#endif

#if (PWM_NUM == 5 | PWM_NUM == 6)                           //五相电压采样
    u[3] = ((int)AdcbResultRegs.ADCRESULT15) * 0.173557;
    u[4] = ((int)AdcbResultRegs.ADCRESULT12) * 0.173557;
#endif

#if (PWM_NUM == 6)                                          //六相电压采样
    u[5] = ((int)AdccResultRegs.ADCRESULT15) * 0.173557;
#endif
}

void BusVoltSamp(float udc[])
{
    /************************************************************
    Description:计算母线电压采样
    Input:none
    Output:udc[]
    ************************************************************/
    udc[0] = ((int)AdcaResultRegs.ADCRESULT11) * 0.173557 - 3;
    udc[1] = ((int)AdcdResultRegs.ADCRESULT12) * 0.173557 + 7;     //(每单位1等于0.173557V)
}

void WindingTempSamp(float t[])
{
    /************************************************************
    Description:计算绕组温度采样
    Input:none
    Output:t[]
    ************************************************************/
#if (PHASE == 3 | PHASE == 5 | PHASE == 6)                  //三相绕组温度采样
    t[0] = ((int)AdcaResultRegs.ADCRESULT9 - 1971) / 20.475;
    t[1] = ((int)AdcaResultRegs.ADCRESULT10 - 1977) / 20.475;
    t[2] = ((int)AdcbResultRegs.ADCRESULT13 - 1977) / 20.475;    //(系数未修改)
#endif

#if (PHASE == 5 | PHASE == 6)                               //五相绕组温度采样
    t[3] = ((int)AdcdResultRegs.ADCRESULT13 - 2048) / 20.475;
    t[4] = ((int)AdcdResultRegs.ADCRESULT14 - 2048) / 20.475;
#endif

#if (PHASE == 6)                                            //六相绕组温度采样
    t[5] = ((int)AdcdResultRegs.ADCRESULT14 - 2048) / 20.475;
#endif
}

int IcTempSamp(void)
{
    /************************************************************
    Description:片内温度传感器采样
    Input:none
    Output:TemperatureC
    ************************************************************/
    return (GetTemperatureC((int)AdcaResultRegs.ADCRESULT15));   //返回摄氏温度
}
