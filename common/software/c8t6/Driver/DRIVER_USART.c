#include "Driver_USART.h"

/**
?* @description: 初始化串口1
?*/
void Driver_USART1_Init(void)
{
    /* 1. 开启时钟 */
    /* 1.1 串口1外设的时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    /* 1.2 GPIO时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    /* 2. 配置GPIO引脚的工作模式 ?PA9=Tx(复用推挽 CNF=10 MODE=11) ?PA10=Rx(浮空输入 CNF=01 MODE=00)*/
    GPIOA->CRH |= GPIO_CRH_CNF9_1;
    GPIOA->CRH &= ~GPIO_CRH_CNF9_0;
    GPIOA->CRH |= GPIO_CRH_MODE9;
    GPIOA->CRH &= ~GPIO_CRH_CNF10_1;
    GPIOA->CRH |= GPIO_CRH_CNF10_0;
    GPIOA->CRH &= ~GPIO_CRH_MODE10;
    /* 3. 串口的参数配置 */
    /* 3.1 配置波特率 115200 */
    USART1->BRR = 0x271;
    /* 3.2 配置一个字的长度 8位 */
    USART1->CR1 &= ~USART_CR1_M;
    /* 3.3 配置不需要校验位 */
    USART1->CR1 &= ~USART_CR1_PCE;
    /* 3.4 配置停止位的长度 */
    USART1->CR2 &= ~USART_CR2_STOP;
    /* 3.5 使能接收和发送 */
    USART1->CR1 |= USART_CR1_TE;
    USART1->CR1 |= USART_CR1_RE;
    /* 4. 使能串口 */
    USART1->CR1 |= USART_CR1_UE;
}

/**
 * @description: 发送一个字节
 * @param {uint8_t} byte 要发送的字节
 */
void Driver_USART1_SendChar(uint8_t byte)
{
    /* 1. 等待发送寄存器为空 */
    while ((USART1->SR & USART_SR_TXE) == 0)
    {
    }

    /* 2. 数据写出到数据寄存器 */
    USART1->DR = byte;
}

/**
 * @description: 发送一个字符串
 * @param {uint8_t} *str 要发送的字符串
 * @param {uint16_t} len 字符串中字节的长度
 * @return {*}
 */
void Driver_USART1_SendString(uint8_t *str, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
    {
        Driver_USART1_SendChar(str[i]);
    }
}

/**
 * @description: 接收一个字节的数据
 * @return {*} 接收到的字节
 */
uint8_t Driver_USART1_ReceiveChar(void)
{
 /* 等待数据寄存器非空 */
 while ((USART1->SR & USART_SR_RXNE) == 0);
 return USART1->DR;
}

/**
 * @description: 接收变长数据.接收到的数据存入到buff中
 * @param {uint8_t} buff 存放接收到的数据
 * @param {uint8_t} *len 存放收到的数据的字节的长度
 */
void Driver_USART1_ReceiveString(uint8_t buff[], uint8_t *len)
{
uint8_t i = 0;
while (1)
    {
    // 等待接收非空
    while ((USART1->SR & USART_SR_RXNE) == 0)
    {
    //在等待期间, 判断是否收到空闲帧
    if (USART1->SR & USART_SR_IDLE)
    {
        *len = i;
        return;
    }
    }
    buff[i] = USART1->DR;
    i++;
    }
}

int fputc(int c, FILE *file)
{
    Driver_USART1_SendChar(c);
    return c;
}
