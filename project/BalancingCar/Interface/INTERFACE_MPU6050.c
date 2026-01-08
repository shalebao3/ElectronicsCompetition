#include "INTERFACE_MPU6050.h"


/**
 * @description: MPU6050读取一个字节
 * @param {uint8_t} reg_addr 寄存器地址
 * @param {uint8_t} *receive_byte 接收字节的指针
 * @return {uint8_t} 读取状态，1表示成功，0表示失败
 */
uint8_t Int_MPU6050_ReadByte(uint8_t reg_addr,uint_8_t *receive_byte)
{
    // 根据I2C协议，读取一个字节
    // 1. 起始信号
    Driver_I2C2_Start();
    // 2. 发送设备地址+写
    Driver_I2C_SendAddr(MPU_IIC_ADDR<<1 | 0); 
    // 3. 发送寄存器地址
    Driver_I2C_SendByte(reg_addr);
    // 起始信号
    // 先准备NACK和停止信号
    Driver_I2C2_NAck();
    Driver_I2C2_Stop();
    // 读取一个字节
    *receive_byte = Driver_I2C_ReadByte();

    return OK;
}
