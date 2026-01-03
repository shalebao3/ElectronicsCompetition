#include "DRIVER_TIM.h"

/**
 * @description: TIM4初始化，pwm模式
 * @return {*}
 */
void Dri_TIM4_Init()
{
/* 1.  开启时钟*/
    /* 1.1 定时器4的时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    /* 1.2 GPIO的时钟 PB */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

/* 2. 设置GPIO的复用推挽输出 PB8 CNF=10 MODE=11*/
    GPIOB->CRH |= GPIO_CRH_MODE8;
    GPIOB->CRH |= GPIO_CRH_CNF8_1;
    GPIOB->CRH &= ~GPIO_CRH_CNF8_0;
    /* 2. 设置GPIO的复用推挽输出 PB9 CNF=10 MODE=11*/
    GPIOB->CRH |= GPIO_CRH_MODE9;
    GPIOB->CRH |= GPIO_CRH_CNF9_1;
    GPIOB->CRH &= ~GPIO_CRH_CNF9_0;

/* 3. 定时器配置 */
    /* pwm频率=10k； pwm频率= 72M(时钟频率) /(PSC+1)×(ARR+1) */
    /* 3.1 预分频器的配置 */
    TIM4->PSC = 0;
    /* 3.2 自动重装载寄存器的配置 */
    TIM4->ARR = 7199;
    /* 3.3 计数器的计数方向 0=向上 1=向下*/
    //  &= ~ &= ~ (位清除操作): 这一行代码通过“与非”逻辑将 DIR 位清零。 这意味着计数器将以向上计数的方式工作。
    TIM4->CR1 &= ~TIM_CR1_DIR;

/* 4. pwm CH3相关配置 */
    /* 4.1 配置通道3的占空比 */
    TIM4->CCR3 = 0;
    /* 4.2 把通道3配置为输出  CCMR2_CC3S=00 输出*/
    TIM4->CCMR2 &= ~TIM_CCMR2_CC3S;
    /* 4.3 配置通道的输出比较模式 CCMR2_OC3M=110，pwm模式1*/
    TIM4->CCMR2 &= ~TIM_CCMR2_OC3M_0;
    TIM4->CCMR2 |= TIM_CCMR2_OC3M_1;
    TIM4->CCMR2 |= TIM_CCMR2_OC3M_2;
    /* 4.4 使能通道3  CCER_CC3E=1 */
    TIM4->CCER |= TIM_CCER_CC3E;
    /* 4.5 设置通道的极性 0=高电平有效  1=低电平有效 */
    TIM4->CCER &= ~TIM_CCER_CC3P;

/* 5 CH4的相关配置 */
    /* 5.1 配置通道4的占空比 */
    TIM4->CCR4 = 0;
    /* 5.2 把通道4配置为输出  CCMR2_CC4S=00 输出*/
    TIM4->CCMR2 &= ~TIM_CCMR2_CC4S;
    /* 5.3 配置通道的输出比较模式 CCMR2_OC4M=110，pwm模式1*/
    TIM4->CCMR2 |= TIM_CCMR2_OC4M_2;
    TIM4->CCMR2 |= TIM_CCMR2_OC4M_1;
    TIM4->CCMR2 &= ~TIM_CCMR2_OC4M_0;
    /* 5.4 使能通道4  CCER_CC4E=1 */
    TIM4->CCER |= TIM_CCER_CC4E;
    /* 5.5 设置通道的极性 0=高电平有效  1=低电平有效 */
    TIM4->CCER &= ~TIM_CCER_CC4P;

    
/* 6 使能定时器计数器 */
    TIM4->CR1 |= TIM_CR1_CEN;
}
