#include "main.h"
#include "freertos.h"
#include "task.h"
#include "INTERFACE_TB6612.h"
#include "INTERFACE_Encoder.h"


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
	
	Interface_TB6612_Init();

	Interface_Encoder();

	Interface_TB6612_SetPWM(3600, 3600);

	
	while (1)
	{
		printf("Encoder2 cnt: %d\n", Interface_Encoder_GetCount(2));
		printf("Encoder3 cnt: %d\n", Interface_Encoder_GetCount(3));
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
