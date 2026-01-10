#include "DRIVER_TIM.h"
#include "INTERFACE_Encoder.h"

/**
 * @description: 编码器接口初始化
 * @return {*}
 */
void Interface_Encoder(void)
{
    Dri_TIM_Encoder();
}


/**
 * @description: 获取定时器编码器的计数值，处理成有符号值
 * @return {int} 计数值
 */
int Interface_Encoder_GetCount(uint8_t tim)
{
    int encoder_count = 0;
    switch (tim)
    {
    case 2:
        encoder_count = (short)TIM2->CNT;
        TIM2->CNT = 0;  //读取后清零
        break;
    case 3:
        encoder_count = (short)TIM3->CNT;
        TIM3->CNT = 0;  //读取后清零
        break;
    default:
        break;
    }
    return encoder_count;
}
