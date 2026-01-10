#ifndef __INTERFACE_TB6612_H__
#define __INTERFACE_TB6612_H__

#include "stm32f10x.h"
#include "DRIVER_TIM.h"

#define GO 0
#define BACK 1
#define STOP 2

#define AIN1_L         GPIOB->ODR &= ~GPIO_ODR_ODR14; // AIN1 置低
#define AIN1_H         GPIOB->ODR |= GPIO_ODR_ODR14;  // AIN1 置高

#define AIN2_L         GPIOB->ODR &= ~GPIO_ODR_ODR15; // AIN2 置低
#define AIN2_H         GPIOB->ODR |= GPIO_ODR_ODR15;  // AIN2 置高

#define BIN1_L         GPIOB->ODR &= ~GPIO_ODR_ODR13; // BIN1 置低
#define BIN1_H         GPIOB->ODR |= GPIO_ODR_ODR13;  // BIN1 置高

#define BIN2_L         GPIOB->ODR &= ~GPIO_ODR_ODR12; // BIN2 置低
#define BIN2_H         GPIOB->ODR |= GPIO_ODR_ODR12;  // BIN2 置高


void Interface_TB6612_Init(void);

void Interface_TB6612_SetPWM(int pwma, int pwmb);



#endif
