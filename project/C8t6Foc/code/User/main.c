#include "main.h"
#include "Driver_USART.h"
#include "COMMON_ANTIPARK.h" // 确保包含你的算法头文件
#include "freertos.h"
#include "task.h"
#include <math.h> // 必须包含数学库

// 1. 任务函数：运行反帕克变换并输出波形
void MyTask_FOC_Antipark(void *pvParameters)
{
    float angle = 0.0f;          // 电角度 (弧度制 0 ~ 2*PI)
    dq_t v_dq = {0.6f, 0.2f};    // 模拟给定的 D轴和 Q轴电压（直流分量）
    alphabeta_t v_ab;            // 存放变换后的 Alpha/Beta 输出

    while (1)
    {
        // A. 模拟电机旋转：角度步进（步进越小，正弦波越细腻）
        angle += 0.05f; 
        if (angle > 6.283185f) angle = 0.0f; // 超过 2*PI 后清零

        // B. 执行反帕克变换：将 DQ 旋转坐标系 转回 Alpha/Beta 静止坐标系
        // 公式：V_alpha = Vd*cos - Vq*sin; V_beta = Vd*sin + Vq*cos;
        Inverse_Park_Transformation(v_dq, angle, &v_ab);

        // C. 格式化输出到 VOFA+ (FireWater 协议)
        // 格式为：数据1,数据2\n
        printf("%.3f,%.3f\n", v_ab.alpha, v_ab.beta);

        // D. 这里的延时非常关键！20ms 对应 50Hz 的波形刷新率
        vTaskDelay(pdMS_TO_TICKS(20)); 
    }
}

int main(void)  
{
    // --- 硬件初始化 ---
    // 系统已经由 SystemInit 自动升频至 72MHz
    Driver_USART1_Init();

    // --- 创建 FOC 波形显示任务 ---
    // 注意：堆栈给 256 或 512，因为 printf 打印浮点数比较占内存
    xTaskCreate(MyTask_FOC_Antipark, "FOC_Wave", 256, NULL, 1, NULL);

    // --- 启动 FreeRTOS 调度器 ---
    vTaskStartScheduler();

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
