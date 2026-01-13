#include "COMMON_SVPWM.h"
#include <math.h>

/**
 * @description: 执行 SVPWM 算法，生成三相占空比
 * @param {input} 反帕克变换后的 alpha 和 beta 电压
 * @param {v_bus} 当前母线电压 (例如 12.0)
 * @param {output} 输出的三相占空比 (用于写入 CCR 寄存器)
 */
void SVPWM_Calculate(svpwm_input_t input, float v_bus, svpwm_output_t *output) {
    // 1. 定义中间变量，用于扇区判断
    float u1, u2, u3;
    u1 = input.beta;
    u2 = (sqrtf(3.0f) * input.alpha - input.beta) / 2.0f;
    u3 = (-sqrtf(3.0f) * input.alpha - input.beta) / 2.0f;

    // 2. 扇区判断 logic
    uint8_t a = (u1 > 0) ? 1 : 0;
    uint8_t b = (u2 > 0) ? 1 : 0;
    uint8_t c = (u3 > 0) ? 1 : 0;
    uint8_t sector = a + 2 * b + 4 * c;

    // 3. 计算矢量作用时间 T1, T2 (这里简化处理，直接映射到占空比分量)
    float x, y, z;
    float k = sqrtf(3.0f) / v_bus;
    x = k * input.beta;
    y = (sqrtf(3.0f) * k * input.alpha + k * input.beta) / 2.0f;
    z = (-sqrtf(3.0f) * k * input.alpha + k * input.beta) / 2.0f;

    float ta, tb, tc;
    switch (sector) {
        case 3: ta = (1 - x - y) / 2; tb = ta + x; tc = tb + y; break; // Sector 1
        case 1: tb = (1 + x - z) / 2; ta = tb - x; tc = ta + z; break; // Sector 2
        case 5: tb = (1 - y + z) / 2; tc = tb + y; ta = tc - z; break; // Sector 3
        case 4: tc = (1 + x + y) / 2; tb = tc - y; ta = tb - x; break; // Sector 4
        case 6: tc = (1 - x + z) / 2; ta = tc - z; tb = ta + x; break; // Sector 5
        case 2: ta = (1 + y - z) / 2; tc = ta + z; tb = tc - y; break; // Sector 6
        default: ta = 0.5f; tb = 0.5f; tc = 0.5f; break;
    }

    // 4. 将计算结果存入输出结构体
    output->a = ta;
    output->b = tb;
    output->c = tc;
}
