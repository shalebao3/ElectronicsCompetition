#include "main.h"
#include "Driver_USART.h"


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

	while (1)
	{
		printf("123456");
		for_delay_ms(1000);
	}	
	
}
