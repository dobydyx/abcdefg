/*
 * InitKey.c
 *
 *  Created on: 2019��4��25��
 *      Author: 16213
 */
#include "F28x_Project.h"

void InitKey(void)
{
    GPIO_SetupPinMux(25, 0, 0);
    GPIO_SetupPinOptions(25, 0, 0);     //����
    GPIO_SetupPinMux(25, 0, 0);
    GPIO_SetupPinOptions(24, 0, 0);     //ֹͣ
    GPIO_SetupPinMux(24, 0, 0);
    GPIO_SetupPinOptions(24, 0, 0);     //�ٶ��л�
}

