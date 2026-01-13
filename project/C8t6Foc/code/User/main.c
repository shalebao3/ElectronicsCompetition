#include "main.h"
#include "Driver_USART.h"
#include "freertos.h"
#include "task.h"

// 1. 定义任务函数
void MyTask_Hello(void *pvParameters)
{
    while (1)
    {
        printf("Hello\r\n");
        // 现在这里是真正的 1000ms 精准延时
        vTaskDelay(pdMS_TO_TICKS(1000)); 
    }
}

int main(void)  
{
    // 硬件初始化
    //SystemClock_Config_72M(); // 确保你之前写的 72M 初始化还在
    Driver_USART1_Init();

    // 2. 创建任务
    // 任务函数, 任务名, 堆栈大小, 参数, 优先级, 任务句柄
    xTaskCreate(MyTask_Hello, "HelloTask", 128, NULL, 1, NULL);

    // 3. 启动调度器 (这是最关键的一步，程序运行到这里后会跳转到任务里，不再回来)
    vTaskStartScheduler();

    // 如果启动成功，程序永远不会执行到这里
    while (1); 
}

// 你的 SysTick_Handler 保持不变，它是对的
extern void xPortSysTickHandler(void);
void SysTick_Handler(void)
{
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        xPortSysTickHandler();
    }   
}
