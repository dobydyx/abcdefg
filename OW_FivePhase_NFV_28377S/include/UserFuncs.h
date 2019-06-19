/*
 * UserFuncs.h
 *
 *  Created on: 2018年1月7日
 *      Author: Ezreal
 */

/************************************************************
Description:用户定义函数声明
************************************************************/

#ifndef INCLUDE_USERFUNCS_H_
#define INCLUDE_USERFUNCS_H_

//初始化函数
void InitEPwms(void);
void InitADCs(void);
void InitDACs(void);
void InitEQep1(void);
void InitSpib(void);
void InitScib(void);
void InitScis(void);
void InitCana(void);
void InitECap1(int pin);
void InitAD2S1210(void);
void InitKey(void);
void InitCPLD(void);

//坐标变换函数
#if PHASE == 3 //三相
void ClarkTrans(float abc[], float ab0[]);
void IClarkTrans(float ab0[], float abc[]);
void ParkTrans(float ab0[], float theta, float dq0[]);
void IParkTrans(float dq0[], float theta, float ab0[]);
#endif

#if PHASE == 5 //五相
void ClarkTrans(float abcde[], float abxy0[]);
void IClarkTrans(float abxy0[], float abcde[]);
void ParkTrans(float abxy0[], float theta, float dqxy0[]);
void IParkTrans(float dqxy0[], float theta, float abxy0[]);
void FTClarkTrans(float abxy0[], float abcde[]);
void FTParkTrans(float abxy0[], float theta, float dqxy0[]);
#endif

#if PHASE == 6 //六相
void ClarkTrans(float abcdef[], float abxymn[]);
void IClarkTrans(float abxymn[], float abcdef[]);
void ParkTrans(float abxymn[], float theta, float dqxymn[]);
void IParkTrans(float dqxymn[], float theta, float abxymn[]);
#endif

//控制计算函数
float ElecThetaCal(float theta);
float VelocityCal(float velocity);
void EmfEstCal(float udc[], float duty[], float i2pscs[]);
float ElecThetaEstCal(float theta);
float VelocityEstCal(float velocity);
float PIDCtrl(PID *p, float errin, float kp, float ki, float up, float down);
void CtrlAlgo(float ud, float uq, float udc, float theta, float duty[],int out1,int out2);
//void CtrlAlgo(float giventorque, float deltatheta, float ucap, float psiab0[], float iab0[], float udc[], float theta, float duty[]);            //DTC-OEW-UP
//void CtrlAlgo(float ud, float uq, float ucap, float idq0[], float udc[], float theta, float duty[]);    //XU DeZhi
//void CtrlAlgo(float ud, float uq, float ux, float uy, float udc[], float theta, float duty[]);
//通信
//Spi
void SpibOut(int a);
//Sci
void ScibOut(int a);
void ScibSendFrame(int var, void *val, int size);
void ScicOut(int a);
//CAN
void CanaMessageSet(int ObjID, CanMsgObj *pMsgObj, MsgObjType eMsgType);

//ADC&DAC
void LineCurSamp(float i[]);
void PhaseVoltSamp(float u[]);
void BusVoltSamp(float udc[]);
void WindingTempSamp(float t[]);
int IcTempSamp(void);
void SetDACaValue(int val);
void SetDACbValue(int val);
void SetDACcValue(int val);

//AD2S1210旋变解码
void FaultRegClrAD2S1210();
void ModeSelAD2S1210(int mode);
void WriteAD2S1210(int addr, int data);
int ReadAD2S1210(int addr);
void GetPosVelAD2S1210(int *buf);

//其他
void SetCMP(float *duty);
void InitFivePaseVVV(void);
void ModelPredictiveControl(void);
#endif /* INCLUDE_USERFUNCS_H_ */
