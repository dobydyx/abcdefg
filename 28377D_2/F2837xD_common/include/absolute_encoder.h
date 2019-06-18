/*
 *  file:absolute_encoder.c
 *
 *  Created on: 2016年4月4日
 *
 *  Description: AVAGO--AEAT-84AD-LBSFO
 *
 *  Author: jiang qing wang
 */

#ifndef ABSOLUTE_ENCODER_H_
#define ABSOLUTE_ENCODER_H_


#define SET_MTMUX0_0	  GpioDataRegs.GPACLEAR.bit.GPIO8 = 1;//GpioDataRegs.GPADAT.bit.GPIO8 = 0;               //output 0
#define SET_MTMUX0_1	  GpioDataRegs.GPASET.bit.GPIO8 = 1;//GpioDataRegs.GPADAT.bit.GPIO8 = 1;               //output 1
#define SET_MTMUX1_0	  GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;//GpioDataRegs.GPADAT.bit.GPIO9 = 0;               //output 0
#define SET_MTMUX1_1	  GpioDataRegs.GPASET.bit.GPIO9 = 1;//GpioDataRegs.GPADAT.bit.GPIO9 = 1;               //output 1
#define SET_MTMUX2_0	  GpioDataRegs.GPACLEAR.bit.GPIO16 = 1;//GpioDataRegs.GPADAT.bit.GPIO16 = 0;              //output 0
#define SET_MTMUX2_1	  GpioDataRegs.GPASET.bit.GPIO16 = 1;//GpioDataRegs.GPADAT.bit.GPIO16 = 1;              //output 1


#define DATA_MTDAT0       GpioDataRegs.GPADAT.bit.GPIO17;                  //read DAT0
#define DATA_MTDAT1       GpioDataRegs.GPADAT.bit.GPIO27;                  //read DAT1
#define DATA_MTDAT2       GpioDataRegs.GPADAT.bit.GPIO26;                  //read DAT2


extern unsigned int num_turns;     // 圈数
extern unsigned int last_turns;     // 圈数
extern unsigned int num_teeth;     // 圈数
extern unsigned int sel[7];        // 进位位，sel0来自于旋变对前半圈和后半圈的判断
extern unsigned int turn[7];       // 7个编码轮的圈数
extern unsigned int MTDAT0[7];     // MTDAT0
extern unsigned int MTDAT1[7];     // MTDAT1
extern unsigned int MTDAT2[7];     // MTDAT2
extern unsigned int D1[7];         // D1-Gray
extern unsigned int D2[7];         // D2-Gray
extern unsigned int V2[7];         // V2-Gray


extern void delayus ( unsigned long num );
extern void read_absolute_encoder(void);
extern void get_real_data_absolute_encoder(void);
extern void analyze_absolute_encoder(void);


#endif /* ABSOLUTE_ENCODER_H_ */
