#include "main.h"
#include "Driver_USART.h"
#include "Driver_TIM1.h"
#include "COMMON_ANTIPARK.h"
#include "freertos.h"
#include "task.h"


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
	Driver_USART1_Init();
    Dri_TIM1_Init();

    // printf("c1,c2,c3\r\n");

    while (1)
    {
		// 测试 1：只发纯英文，不要发数字和变量
		printf("Hello\r\n"); 
		for_delay_ms(1000);
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
