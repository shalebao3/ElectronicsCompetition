#include "main.h"
#include "freertos.h"
#include "task.h"
#include "INTERFACE_TB6612.h"
#include "INTERFACE_Encoder.h"
#include "INTERFACE_MPU6050.h"
#include "Driver_USART.h"
#include "App_car.h"


void for_delay_ms(uint32_t ms)
{
	uint32_t Delay = ms * 72000 / 9; /* 72M时钟频率 */
	do
	{
	__NOP(); /* 空指令（NOP）来占用 CPU 时间 */
	} while (Delay--);
}


int main(void)	
{
	Int_MPU6050_Init();

	Driver_USART1_Init();

	Interface_TB6612_Init();

	Interface_Encoder();

	Interface_TB6612_SetPWM(3600, 3600);

	short gx, gy, gz;
	short ax, ay, az;
	
	while (1)
	{
		// printf("Encoder2 cnt: %d\n", Interface_Encoder_GetCount(2));
		// printf("Encoder3 cnt: %d\n", Interface_Encoder_GetCount(3));
		// for_delay_ms(1000);

		// =============测试MPU6050================
		Int_MPU6050_Get_Gryo(&gx, &gy, &gz);
		Int_MPU6050_Get_Accel(&ax, &ay, &az);
		// 这种格式最容易被 VOFA+ 识别为 6 个独立的通道
		// 只要数字和逗号，结尾必须换行
		// printf("%d,%d,%d,%d,%d,%d\n", gx, gy, gz, ax, ay, az);
		// for_delay_ms(20);

		// =============测试小车角度================
		App_Car_GetAngle();
		for_delay_ms(20);
	}	
	
}

extern void xPortSysTickHandler(void);
void SysTick_Handler(void)
{
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
	{
		xPortSysTickHandler();
	}	
}
