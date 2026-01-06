#include "main.h"
#include "freertos.h"
#include "task.h"
#include "INTERFACE_TB6612.h"

int main(void)
{
	
	Interface_TB6612_Init();
	Interface_TB6612_SetPWM(3600, 3600);
	
	while (1)
	{
		
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
