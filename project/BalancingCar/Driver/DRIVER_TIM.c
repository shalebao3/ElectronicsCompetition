#include "DRIVER_TIM.h"

/**
 * @description: TIM4初始化，pwm模式
 * @return {*}
 */
void Dri_TIM4_Init(void)
{
/* CLOCK */
    /* 1.1 开启定时器4的时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    /* 1.2 开启GPIOB的时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

/* Pinmux */
/* 2. 设置GPIO的复用推挽输出 PB8 CNF=10 MODE=11*/
    GPIOB->CRH |= GPIO_CRH_MODE8;
    GPIOB->CRH |= GPIO_CRH_CNF8_1;
    GPIOB->CRH &= ~GPIO_CRH_CNF8_0;
    /* 2. 设置GPIO的复用推挽输出 PB9 CNF=10 MODE=11*/
    GPIOB->CRH |= GPIO_CRH_MODE9;
    GPIOB->CRH |= GPIO_CRH_CNF9_1;
    GPIOB->CRH &= ~GPIO_CRH_CNF9_0;

/* CoreConfig */
/* 3. 定时器配置 */
    /* pwm频率=10k； pwm频率= 72M(时钟频率) /(PSC+1)×(ARR+1) */
    /* 3.1 预分频器的配置 0 不分频 */
    TIM4->PSC = 0;
    /* 3.2 自动重装载寄存器的配置 */
    TIM4->ARR = 7199;
    /* 3.3 计数器的计数方向 0=向上 1=向下*/
    //  &= ~ &= ~ (位清除操作): 这一行代码通过“与非”逻辑将 DIR 位清零。 这意味着计数器将以向上计数的方式工作。
    TIM4->CR1 &= ~TIM_CR1_DIR;

/* Channel/Feature */
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

/* Link */
/* IRQ */
/* DMA */

/* Start */ 
/* 6 使能定时器计数器 */
    TIM4->CR1 |= TIM_CR1_CEN;
}


/**
 * @brief 定时器2,3编码器模式初始化
 * @return {*}
 */
void Dri_TIM_Encoder(void)
{
    // 1. **Clock**：RCC 使能 + 选择时钟源/分频
    // 开启 GPIOB\GPIOA 时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; 
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 开启定时器2和3的时钟源
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN ;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN ;

    // 开启  AFIO 时钟
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    // 2. **Pinmux/Route**：GPIO/AF/重映射/路由矩阵
    // 关闭 JTAG，保留SWD开启，释放 PB3\PB4\PB5 作为普通 IO 使用 ，SWJ_CFG = 010
    AFIO->MAPR &= ~AFIO_MAPR_SWJ_CFG_2;
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_1;
    AFIO->MAPR &= ~AFIO_MAPR_SWJ_CFG_0;

    // 设置 GPIOB PB3\PB4\PB5，GPIOA PA15 为浮空输入 mode=00,cnf=01
    GPIOB->CRL &= ~GPIO_CRL_MODE3;  // 00 输入模式
    GPIOB->CRL &= ~GPIO_CRL_CNF3_0; 
    GPIOB->CRL |= GPIO_CRL_CNF3_1;  // 01 浮空输入

    GPIOB->CRL &= ~GPIO_CRL_MODE4;
    GPIOB->CRL &= ~GPIO_CRL_CNF4_0;
    GPIOB->CRL |= GPIO_CRL_CNF4_1;

    GPIOB->CRL &= ~GPIO_CRL_MODE5;
    GPIOB->CRL &= ~GPIO_CRL_CNF5_0;
    GPIOB->CRL |= GPIO_CRL_CNF5_1;

    GPIOA->CRH &= ~GPIO_CRH_MODE15;
    GPIOA->CRH &= ~GPIO_CRH_MODE15_0;  
    GPIOA->CRH |= GPIO_CRH_CNF15_1;

    // PB3\PB4\PB5\PA15 作为定时器2和3的通道输入
    AFIO->MAPR |= AFIO_MAPR_TIM2_REMAP_1; // TIM2 完全重映射，AFIO_MAPR_TIM2 = 010
    AFIO->MAPR |= AFIO_MAPR_TIM3_REMAP_1;   // TIM3 部分重映射，AFIO_MAPR_TIM3 = 10


    // 3. **CoreConfig**：外设核心参数（波特率、分辨率、模式）

    TIM2->PSC = 0;               // 预分频器不分频
    TIM2->ARR = 65535 - 1;          // 自动重装载寄存器周期 1000
    TIM3->PSC = 0;
    TIM3->ARR = 65535 - 1;

    // 4. **Channel/Feature**：通道/功能块（多路、FIFO、滤波、触发源）
    // 编码器模式设置
    // 定时器2
    // 通道1
    TIM2->CCMR1 |= TIM_CCMR1_CC1S_0;  // 定时器2 通道1  01
    TIM2->CCMR1 &= ~TIM_CCMR1_CC1S_1;

    // 通道2
    TIM2->CCMR1 |= TIM_CCMR1_CC2S_0;   // 定时器2 通道2  01 
    TIM2->CCMR1 &= ~TIM_CCMR1_CC2S_1;

    TIM2->CCMR1 |= TIM_CCMR1_CC1S;
    TIM2->CCER &= ~TIM_CCER_CC2P;   // 定时器2 通道1 不反相

    // 开启CH1,CH2输入捕获 不反相 
    TIM2->CCER |= TIM_CCER_CC1P;
    TIM2->CCER |= TIM_CCER_CC1E;

    TIM2->CCER |= TIM_CCER_CC2P;
    TIM2->CCER |= TIM_CCER_CC2E;
    //双边沿计数模式 TIM_SMCR_SMS_0 = 011
    TIM2->SMCR |= TIM_SMCR_SMS_0;
    TIM2->SMCR |= TIM_SMCR_SMS_1;
    TIM2->SMCR &= ~TIM_SMCR_SMS_2;

    // 定时器3
    // 通道1
    TIM3->CCMR1 &= ~TIM_CCMR1_CC2S_0;   // 定时器3 通道2 选择编码器模式2
    TIM3->CCMR1 |= TIM_CCMR1_CC2S_1;

    // 通道2
    TIM3->CCMR1 &= ~TIM_CCMR1_CC1S_0;  // 定时器3 通道1 选择编码器模式2
    TIM3->CCMR1 |= TIM_CCMR1_CC1S_1;
    TIM3->CCER &= ~TIM_CCER_CC2P;   // 定时器3 通道2 不反相

    // 开启CH1,CH2输入捕获 不反相
    TIM3->CCER |= TIM_CCER_CC1P;
    TIM3->CCER |= TIM_CCER_CC2E;
    //双边沿计数模式 TIM_SMCR_SMS_0 = 011
    TIM3->SMCR |= TIM_SMCR_SMS_0;
    TIM3->SMCR |= TIM_SMCR_SMS_1;
    TIM3->SMCR &= ~TIM_SMCR_SMS_2;
    

    // 5. **Link**：与其它外设联动（触发/同步/路由/跨外设连接）
    // 6. **IRQ**：中断使能、清标志、NVIC、优先级
    // 7. **DMA**：DMA 请求、通道映射、burst/循环
    // 8. **Start**：启动顺序、清标志、使能输出/收发
    TIM2->CR1 |= TIM_CR1_CEN;   // 使能定时器2
    TIM3->CR1 |= TIM_CR1_CEN;   // 使能定时器3
}

