#ifndef __COMMON_SVPWM_H
#define __COMMON_SVPWM_H

#include "stm32f10x.h"

// SVPWM 输入结构体：来自反帕克变换的 Alpha/Beta 电压
typedef struct {
    float alpha;
    float beta;
} svpwm_input_t;

// SVPWM 输出结构体：计算出的三相占空比 (0.0 ~ 1.0)
typedef struct {
    float a;
    float b;
    float c;
} svpwm_output_t;

// 函数声明
void SVPWM_Calculate(svpwm_input_t input, float v_bus, svpwm_output_t *output);

#endif
