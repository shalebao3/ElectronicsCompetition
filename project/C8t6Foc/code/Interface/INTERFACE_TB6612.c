#include "INTERFACE_TB6612.h"

/**
 * @description: TB6612接口初始化
 * @return {*}
 */
void Interface_TB6612_Init(void)
{
    /* 
    原理图中，
    PWM (PWMA)：负责控制速度（也就是油门大小）。
    AIN1 / AIN2：负责控制方向（也就是挂前进挡、倒挡还是刹车）。 
    */
    /* 初始化定时器 */
    Dri_TIM4_Init();
    /* 初始化GPIO*/
    /* 配置GPIOB12,13,14,15为推挽输出，50Mhz */
    GPIOB->CRH &= ~(GPIO_CRH_CNF12 | GPIO_CRH_CNF13 | GPIO_CRH_CNF14 | GPIO_CRH_CNF15);
    /* 配置GPIOB12,13,14,15为输出模式 */
    GPIOB->CRH |= (GPIO_CRH_MODE12 | GPIO_CRH_MODE13 | GPIO_CRH_MODE14 | GPIO_CRH_MODE15);
}




/**
 * @description: TB6612控制电机A的方向
 * @param {uint8_t} direction 方向：GO前进，BACK后退，STOP刹车
 * @return {*}
 */
void Interface_TB6612_MotorA(uint8_t direction)
{
    if (direction == GO)
    {
        /* 前进: AIN1置高，AIN2置低 */
        AIN1_H;
        AIN2_L;
    }
    else if (direction == BACK)
    {
        /* 后退: AIN1置低，AIN2置高 */
        AIN1_L;
        AIN2_H;
    }
    else if (direction == STOP)
    {
        /* 刹车：AIN1置高，AIN2置高 */
        AIN1_H;
        AIN2_H;
    }
}

/**
 * @description: TB6612控制电机B的方向
 * @param {uint8_t} direction 方向：GO前进，BACK后退，STOP刹车
 * @return {*}
 */
void Interface_TB6612_MotorB(uint8_t direction)
{
    if (direction == GO)
    {
        /* 前进: BIN1置高，BIN2置低 */
        BIN1_H;
        BIN2_L;
    }
    else if (direction == BACK)
    {
        /* 后退: BIN1置低，BIN2置高 */
        BIN1_L;
        BIN2_H;
    }
    else if (direction == STOP)
    {
        /* 刹车: BIN1置高，BIN2置高 */
        BIN1_H;
        BIN2_H;
    } 
}

/**
 * @description: 根据带符号的pwm值设置转速和方向
 * @param {int} pwma 电机A的PWM占空比
 * @param {int} pwmb 电机B的PWM占空比
 * @return {*}
 */
void Interface_TB6612_SetPWM(int pwma, int pwmb)
{
    /*根据带符号的pwm值：1、处理方向；2、设置pwm占空比*/
    /*1.处理pwma*/
    /*1.1根据符号处理方向*/
    if (pwma >= 0)
    {
        /*前进*/
        Interface_TB6612_MotorA(GO);
    }
    else if (pwma < 0)
    {
        /*后退*/
        Interface_TB6612_MotorA(BACK);
        /*取绝对值*/
        pwma = -pwma;
    }
    else
    {
        /*刹车*/
        Interface_TB6612_MotorA(STOP);
    }
    /*1.2设置对应的pwm值=》对应定时器通道的ccr值*/
    TIM4->CCR4 = pwma;


    /*2.处理pwmb*/
    if (pwmb >= 0)
    {
        /*前进*/
        Interface_TB6612_MotorB(GO);
    }
    else if (pwmb < 0)
    {
        /*后退*/
        Interface_TB6612_MotorB(BACK);
        /*取绝对值*/
        pwmb = -pwmb;
    }
    else
    {
        /*刹车*/
        Interface_TB6612_MotorB(STOP);
    }
    /*2.2设置对应的pwm值=》对应定时器通道的ccr值*/
    TIM4->CCR3 = pwmb;
}


