#include "App_car.h"


short gx, gy, gz;
short ax, ay, az;

float car_angle; //通过加速度计算出来的倾角
float car_gyro;  //Y轴角速度，采样值转换成角度


extern float angle;      //卡尔曼滤波后的角度


/**
 * @brief  获取小车倾角
 * 
 */
void App_Car_GetAngle(void)
{
    // 通过加速度计算倾角

    // 1 读取6050数据
    Int_MPU6050_Get_Gryo(&gx, &gy, &gz);
    Int_MPU6050_Get_Accel(&ax, &ay, &az);

    // 1 atan2(y, x) 返回从 x 轴到点 (x, y) 的角度，结果以弧度表示，范围为 -π 到 π。
    car_angle = atan2(ax, az) * 180 / PI;  // 倾角

    // 角速度，量程是2000度每秒，对应65535/4000=16.4 LSB/°/s
    car_gyro = (float)gy / 16.4f;

    // 计算的倾角、角速度，通过卡尔曼滤波器计算最终角度
    Com_Filter_Kalman(car_angle, car_gyro);


    printf("car_angle: %.2f\r\n", angle);
    printf("car_gyro: %.2f\r\n", car_gyro);
}
