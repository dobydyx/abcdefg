/*
 * InitAD2S1210.c
 *
 *  Created on: 2018年6月10日
 *      Author: Ezreal, Doby
 */

/************************************************************
Description:初始化AD2S1210芯片，包括对应176脚封装的GPIO口初始化
************************************************************/

#include "F28x_Project.h"

void SpibOut(int a);

void InitAD2S1210Gpio(void)
{
    GPIO_SetupPinMux(66, 0, 0);
    GPIO_SetupPinOptions(66, 1, 0);         //Pin WR/FSYNC
    GPIO_WritePin(66, 1);

    GPIO_SetupPinMux(44, 0, 0);
    GPIO_SetupPinOptions(44, 1, 0);         //Pin SAMPLE
    GPIO_WritePin(44, 1);

    GPIO_SetupPinMux(64, 0, 0);
    GPIO_SetupPinOptions(64, 1, 0);         //Pin RESET
    GPIO_WritePin(64, 1);

    GPIO_SetupPinMux(65, 0, 0);
    GPIO_SetupPinOptions(65, 1, 0);         //Pin CS
    GPIO_WritePin(64, 0);

    GPIO_SetupPinMux(62, 0, 0);
    GPIO_SetupPinOptions(62, 1, 0);         //Pin A0

    GPIO_SetupPinMux(63, 0, 0);
    GPIO_SetupPinOptions(63, 1, 0);         //Pin A1
}

void ConfPinFSYNC(int value)
{
    GPIO_WritePin(66, value);
}

void ConfPinSAMPLE(int value)
{
    GPIO_WritePin(44, value);
}

void ConfPinRESET(int value)
{
    GPIO_WritePin(64, value);
}

void ConfPinCS(int value)
{
    GPIO_WritePin(65, value);
}

void ConfPinA0(int value)
{
    GPIO_WritePin(62, value);
}

void ConfPinA1(int value)
{
    GPIO_WritePin(63, value);
}

void ModeSelAD2S1210(int mode)
{

    switch(mode)
    {
        case 0:
        {
            ConfPinA0(0);
            ConfPinA1(0);       //Normal Mode position output
        }break;
        case 1:
        {
            ConfPinA0(0);
            ConfPinA1(1);       //Normal Mode velocity output
        }break;
        case 2:
        {
            ConfPinA0(1);
            ConfPinA1(1);       //Configuration Mode
        }break;
    }
}

void WriteAD2S1210(int addr, int data)
{
    ConfPinFSYNC(0);
    SpibOut(addr<<8);        //write control register address
    ConfPinFSYNC(1);

    ConfPinFSYNC(0);
    SpibOut(data<<8);        //write control register data
    ConfPinFSYNC(1);
}

int ReadAD2S1210(int addr)
{
    ConfPinSAMPLE(0);      //拉高采样信号
    ConfPinSAMPLE(1);      //拉低采样信号

    ConfPinFSYNC(0);
    SpibOut(addr<<8);       //write control register address
    DELAY_US(10);
    ConfPinFSYNC(1);
    ConfPinFSYNC(0);
    SpibOut(addr<<8);       //write control register address
    DELAY_US(10);
    ConfPinFSYNC(1);

    return SpibRegs.SPIRXBUF;                         // 接收数据
}

void GetPosVelAD2S1210(int *buf)
{
    int i = 0;
    ConfPinSAMPLE(0);      //拉高采样信号
    ConfPinSAMPLE(1);      //拉低采样信号

    ConfPinFSYNC(0);
    for(i = 0; i < 3; i++)
    {
        SpibOut(0xFF00);       //write control register address
        DELAY_US(10);
        buf[i] = SpibRegs.SPIRXBUF;                         // 接收数据
    }
    ConfPinFSYNC(1);
}


void FaultRegClrAD2S1210(void)
{
    ConfPinSAMPLE(0);      //拉高采样信号
    ConfPinSAMPLE(1);      //拉低采样信号

    ConfPinFSYNC(0);
    SpibOut(0xFF00);
    DELAY_US(10);
    ConfPinFSYNC(1);

    ConfPinSAMPLE(0);      //拉高采样信号
    ConfPinSAMPLE(1);      //拉低采样信号
}

void InitAD2S1210(void)
{
    InitAD2S1210Gpio();

    ConfPinRESET(0);
    DELAY_US(60000);
    ConfPinRESET(1);
    DELAY_US(60000);

    ModeSelAD2S1210(2);
    FaultRegClrAD2S1210();

    WriteAD2S1210(0x91, 0x14);      //5KHz Excitation
    WriteAD2S1210(0x92, 0x7F);      //16位精度
}

