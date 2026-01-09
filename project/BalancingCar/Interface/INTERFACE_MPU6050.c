#include "INTERFACE_MPU6050.h"


/**
 * @description: MPU6050读取一个字节
 * @param {uint8_t} reg_addr 寄存器地址
 * @param {uint8_t} *receive_byte 接收字节的指针
 * @return {uint8_t} 读取状态，1表示成功，0表示失败
 */
uint8_t Int_MPU6050_ReadByte(uint8_t reg_addr,uint8_t *receive_byte)
{
    // 根据I2C协议，读取一个字节
    // 1. 起始信号 S
    Driver_I2C2_Start();
    // 2. 发送设备地址+写 AD+W
    Driver_I2C_SendAddr(MPU_IIC_ADDR<<1 | 0); 
    // 3. 发送寄存器地址 RA
    Driver_I2C_SendByte(reg_addr);

    // 4. 起始信号 S
    Driver_I2C2_Start();
    // 5. 发送设备地址+读 AD+R
    Driver_I2C_SendAddr(MPU_IIC_ADDR<<1 | 1);


    // 6. 先准备NACK和停止信号  NACK P
    Driver_I2C2_NAck();
    Driver_I2C2_Stop();
    // 7. 读取一个字节 DATA
    *receive_byte = Driver_I2C_ReadByte();

    return OK;
}


/**
 * @description: 读取指定寄存器多个字节数据
 * @param {uint8_t} reg_addr 寄存器地址
 * @param {uint8_t} *receive_buff 接收字节的指针
 * @return {uint8_t} 读取状态，1表示成功，0表示失败
 */
uint8_t Int_MPU6050_ReadBytes(uint8_t reg_addr,uint8_t *receive_buff,uint8_t size)
{
    // 根据I2C协议，读取多个字节
    // 1. 起始信号 S
    Driver_I2C2_Start();
    // 2. 发送设备地址+写 AD+W
    Driver_I2C_SendAddr(MPU_IIC_ADDR<<1 | 0); 
    // 3. 发送寄存器地址 RA
    Driver_I2C_SendByte(reg_addr);
    // 4. 起始信号 S
    Driver_I2C2_Start();
    // 5. 发送设备地址+读 AD+R
    Driver_I2C_SendAddr(MPU_IIC_ADDR<<1 | 1);
    // 7. 读取多个字节  DATA
    for (int i = 0; i < size; i++)
    {
        if (i < size - 1)
        {
            // 不是最后一个字节，提前准备 ACK
            Driver_I2C2_Ack();
        }
        else
        {
            // 最后一个字节，提前准备 NACK 和 停止信号
            // 6. 先准备NACK和停止信号 NACK P
            Driver_I2C2_NAck();
            Driver_I2C2_Stop();
        }
        

        receive_buff[i] = Driver_I2C_ReadByte();
    }

    return OK;
}


/**
 * @description: 向MPU6050写入一个字节
 * @param {uint8_t} reg_addr 寄存器地址
 * @param {uint8_t} send_byte 发送的字节
 * @return {uint8_t} 写入状态，1表示成功，0表示失败
 */
uint8_t Int_MPU6050_WriteByte(uint8_t reg_addr,uint8_t send_byte)
{
    // 根据I2C协议，写入一个字节
    // 1. 起始信号 S
    Driver_I2C2_Start();
    // 2. 发送设备地址+写 AD+W
    Driver_I2C_SendAddr(MPU_IIC_ADDR<<1 | 0); 
    // 3. 发送寄存器地址 RA
    Driver_I2C_SendByte(reg_addr);
    // 4. 发送数据 DATA     
    Driver_I2C_SendByte(send_byte);
    // 5. 停止信号 P
    Driver_I2C2_Stop();
    return OK;
}

/**
 * @description: 写入指定寄存器多个字节数据
 * @param {uint8_t} reg_addr 寄存器地址
 * @param {uint8_t} *send_buff 发送字节的指针
 * @param {uint8_t} size 发送字节的数量
 * @return {uint8_t} 写入状态，1表示成功，0表示失败
 */
uint8_t Int_MPU6050_WriteBytes(uint8_t reg_addr,uint8_t *send_buff,uint8_t size)
{
    // 根据I2C协议，写入多个字节
    // 1. 起始信号 S
    Driver_I2C2_Start();
    // 2. 发送设备地址+写 AD+W
    Driver_I2C_SendAddr(MPU_IIC_ADDR<<1 | 0); 
    // 3. 发送寄存器地址 RA
    Driver_I2C_SendByte(reg_addr);      
    // 4. 发送多个字节 DATA
    for (int i = 0; i < size; i++)
    {
        Driver_I2C_SendByte(send_buff[i]);
    }
    // 5. 停止信号 P
    Driver_I2C2_Stop();
    return OK;
}

/*
 * @description: 设置MPU6050的数字低通滤波器
 * @param {uint16_t} rate 采样率，单位Hz
 */
void Int_MPU6050_Set_DLPF_CFG(uint16_t rate)
{
    rate = rate / 2; // DLPF cutoff is typically half the sample rate.
    uint8_t dlpf_cfg = 0;

    if (rate >= 188)      dlpf_cfg = 1;
    else if (rate > 98)   dlpf_cfg = 2;
    else if (rate > 42)   dlpf_cfg = 3;
    else if (rate > 20)   dlpf_cfg = 4;
    else if (rate > 10)   dlpf_cfg = 5;
    else                  dlpf_cfg = 6;

    Int_MPU6050_WriteByte(MPU_CFG_REG, dlpf_cfg);
}

/*
 * @description: 设置陀螺仪采样率
 * @param {uint16_t} rate 采样率，单位Hz
 */
void Int_MPU6050_SetGyroByte(uint16_t rate)
{
    if (rate < 4) rate = 4;
    else if (rate > 1000) rate = 1000;

    uint8_t simplrt_div = 1000 / rate - 1;

    Int_MPU6050_WriteByte(MPU_SAMPLE_RATE_REG, simplrt_div);
    Int_MPU6050_Set_DLPF_CFG(rate);
}

// 初始化函数
void Int_MPU6050_Init(void)
{
    uint8_t dev_id = 0;

    // 初始化IIC
    Driver_I2C2_Init();
    // 复位-》延迟一会-》唤醒
    Int_MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 0x80);
    for (volatile int i = 0; i < 72000; i++);
    Int_MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 0x00);

    // 陀螺仪量程±2000°/s，加速度计量程±2g
    Int_MPU6050_WriteByte(MPU_GYRO_CFG_REG, 3 << 3);
    Int_MPU6050_WriteByte(MPU_ACCEL_CFG_REG, 0 << 3);

    // 其他功能配置（可选）
    Int_MPU6050_WriteByte(MPU_INT_EN_REG, 0x0e); // 数据准备中断使能
    Int_MPU6050_WriteByte(MPU_USER_CTRL_REG, 0x00); // I2C主模式禁用
    Int_MPU6050_WriteByte(MPU_FIFO_EN_REG, 0x00);//关闭FIFO

    // 配置时钟源之前，确认正常工作，读一下id
    Int_MPU6050_ReadByte(MPU_DEVICE_ID_REG, &dev_id);

    if (dev_id == 0x68 || dev_id == MPU_IIC_ADDR)
    {
        // 设置时钟源为陀螺仪X轴
        Int_MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 0x01);
        // 设置陀螺仪采样率、数字低通滤波器
        Int_MPU6050_SetGyroByte(100);
        // 让两个传感器退出待机模式，进入正常工作模式
        Int_MPU6050_WriteByte(MPU_PWR_MGMT2_REG, 0x00);
    }
}

/**
 * @description: 获取陀螺仪数据,拼接两字节数据，16位有符号整数
 * @param {short} *gx 陀螺仪X轴数据
 * @param {short} *gy 陀螺仪Y轴数据
 * @param {short} *gz 陀螺仪Z轴数据
 */
void Int_MPU6050_Get_Gryo(short *gx,short *gy,short *gz)
{
    uint8_t buff[6];

    /*
        buff[0] ：角速度x轴高字节
        buff[1] ：角速度x轴低字节
        buff[2] ：角速度y轴高字节
        buff[3] ：角速度y轴低字节
        buff[4] ：角速度z轴高字节
        buff[5] ：角速度z轴低字节
    */
    Int_MPU6050_ReadBytes(MPU_GYRO_XOUTH_REG,buff,6);
    *gx = (short)(buff[0]<<8 | buff[1]);
    *gy = (short)(buff[2]<<8 | buff[3]);
    *gz = (short)(buff[4]<<8 | buff[5]);
}


/**
 * @description: 获取加速度数据,拼接两字节数据，16位有符号整数
 * @param {short} *ax 加速度X轴数据
 * @param {short} *ay 加速度Y轴数据
 * @param {short} *az 加速度Z轴数据
 */
void Int_MPU6050_Get_Accel(short *ax,short *ay,short *az)
{
    uint8_t buff[6];
    /*
        buff[0] ：加速度x轴高字节
        buff[1] ：加速度x轴低字节
        buff[2] ：加速度y轴高字节
        buff[3] ：加速度y轴低字节
        buff[4] ：加速度z轴高字节
        buff[5] ：加速度z轴低字节
    */
    Int_MPU6050_ReadBytes(MPU_ACCEL_XOUTH_REG,buff,6);
    *ax = (short)(buff[0]<<8 | buff[1]);
    *ay = (short)(buff[2]<<8 | buff[3]);
    *az = (short)(buff[4]<<8 | buff[5]);
}

