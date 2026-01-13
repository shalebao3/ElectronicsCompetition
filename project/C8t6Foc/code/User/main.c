#include "main.h"
#include "Driver_USART.h"
#include "COMMON_ANTIPARK.h"
#include "COMMON_SVPWM.h"    // 1. 必须包含新算法的头文件
#include "freertos.h"
#include "task.h"
#include <stdio.h>
#include <math.h>

// 2. 任务函数声明
void MyTask_FOC_Control(void *pvParameters);

int main(void)  
{
    // 核心配置：设置中断优先级分组为 4，这是 FreeRTOS 运行的基础
    SCB->AIRCR = 0x05FA0300; 

    // 硬件初始化
    Driver_USART1_Init(); // 初始化 115200 串口

    // 3. 创建 FOC 控制任务
    // 堆栈大小建议 512，因为同时运行两个算法并 printf 比较耗内存
    xTaskCreate(MyTask_FOC_Control, "FOC_Ctrl", 512, NULL, 1, NULL);

    printf("FOC System Starting...\r\n");

    // 4. 启动调度器
    vTaskStartScheduler();

    while (1); 
}

// 5. 任务的具体实现：串联整个 FOC 链路
void MyTask_FOC_Control(void *pvParameters)
{
    float angle = 0.05f;
    dq_t v_dq = {0.4f, 0.2f}; // 模拟设定的 D/Q 轴电压
    alphabeta_t v_ab;
    svpwm_output_t v_pwm;     // 存储 SVPWM 输出的三相占空比

    while (1)
    {
        // A. 更新电角度 (模拟电机旋转)
        angle += 0.05f; 
        if (angle > 6.283185f) angle = 0.0f;

        // B. 运行反帕克变换：得到 Alpha/Beta 电压
        Inverse_Park_Transformation(v_dq, angle, &v_ab);

        // C. 运行 SVPWM 算法：将 Alpha/Beta 转换为三相 PWM 占空比
        // 这里的 12.0f 代表你的母线电压 (Vbus)
        svpwm_input_t sv_in = {v_ab.alpha, v_ab.beta};
        SVPWM_Calculate(sv_in, 12.0f, &v_pwm);

        // D. 格式化输出到 VOFA+ 观察马鞍波
        // 顺序为：Phase_A, Phase_B, Phase_C
        printf("%.3f,%.3f,%.3f\n", v_pwm.a, v_pwm.b, v_pwm.c);

        // E. 控制频率：1ms 一次 (1kHz)，对于基础调试非常稳
        vTaskDelay(pdMS_TO_TICKS(5)); 
    }
}

// 保持你之前的 SysTick_Handler 逻辑
extern void xPortSysTickHandler(void);
void SysTick_Handler(void)
{
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        xPortSysTickHandler();
    }   
}
