#include "main.h"
#include "freertos.h"
#include "task.h"

int main(void)
{
		
	
	
}

extern void xPortSysTickHandler(void);
void SysTick_Handler(void)
{
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
	{
		xPortSysTickHandler();
	}	
}
