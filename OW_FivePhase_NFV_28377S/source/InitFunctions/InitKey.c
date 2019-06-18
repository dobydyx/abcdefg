/*
 * InitKey.c
 *
 *  Created on: 2019年4月25日
 *      Author: 16213
 */
#include "F28x_Project.h"

void InitKey(void)
{
    GPIO_SetupPinMux(25, 0, 0);
    GPIO_SetupPinOptions(25, 0, 0);     //启动
    GPIO_SetupPinMux(25, 0, 0);
    GPIO_SetupPinOptions(24, 0, 0);     //停止
    GPIO_SetupPinMux(24, 0, 0);
    GPIO_SetupPinOptions(24, 0, 0);     //速度切换
}

