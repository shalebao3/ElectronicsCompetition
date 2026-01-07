#include "Driver_I2C.h"

/**
 * @description: I2C2 init
 * @return {*}
 */
void Driver_I2C2_Init(void)
{
    /* 1. enable clocks */
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    /* 2. configure GPIOB PB10(SCL), PB11(SDA) as AF open-drain */
    GPIOB->CRH |= (GPIO_CRH_MODE10 | GPIO_CRH_MODE11 | GPIO_CRH_CNF10 | GPIO_CRH_CNF11);

    /* 3. configure I2C2 */
    I2C2->CR1 &= ~I2C_CR1_SMBUS;
    I2C2->CR2 |= 36 << 0; /* PCLK1 = 36 MHz */
    I2C2->CCR &= ~I2C_CCR_FS;
    I2C2->CCR |= 180 << 0;
    I2C2->TRISE |= 37;

    /* 4. enable I2C2 */
    I2C2->CR1 |= I2C_CR1_PE;
}

/**
 * @description: start condition
 * @return {*}
 */
uint8_t Driver_I2C2_Start(void)
{
    I2C2->CR1 |= I2C_CR1_START;

    uint16_t timeout = 0xffff;
    while (((I2C2->SR1 & I2C_SR1_SB) == 0) && timeout)
    {
        timeout--;
    }

    return timeout ? OK : FAIL;
}

/**
 * @description: stop condition
 * @return {*}
 */
void Driver_I2C2_Stop(void)
{
    I2C2->CR1 |= I2C_CR1_STOP;
}

/**
 * @description: ack
 */
void Driver_I2C2_Ack(void)
{
    I2C2->CR1 |= I2C_CR1_ACK;
}

/**
 * @description: nack
 */
void Driver_I2C2_NAck(void)
{
    I2C2->CR1 &= ~I2C_CR1_ACK;
}

/**
 * @description: send device address
 * @param {uint8_t} addr
 */
uint8_t Driver_I2C_SendAddr(uint8_t addr)
{
    I2C2->DR = addr;

    uint16_t timeout = 0xffff;
    while (((I2C2->SR1 & I2C_SR1_ADDR) == 0) && timeout)
    {
        timeout--;
    }

    if (timeout)
    {
        I2C2->SR2;
    }

    return timeout ? OK : FAIL;
}

/**
 * @description: send one byte
 * @param {uint8_t} byte
 */
uint8_t Driver_I2C_SendByte(uint8_t byte)
{
    uint16_t timeout = 0xffff;
    while (((I2C2->SR1 & I2C_SR1_TXE) == 0) && timeout)
    {
        timeout--;
    }

    I2C2->DR = byte;

    timeout = 0xffff;
    while (((I2C2->SR1 & I2C_SR1_BTF) == 0) && timeout)
    {
        timeout--;
    }

    return timeout ? OK : FAIL;
}

/**
 * @description: read one byte
 * @return {*}
 */
uint8_t Driver_I2C_ReadByte(void)
{
    uint16_t timeout = 0xffff;
    while (((I2C2->SR1 & I2C_SR1_RXNE) == 0) && timeout)
    {
        timeout--;
    }

    return timeout ? I2C2->DR : 0;
}
