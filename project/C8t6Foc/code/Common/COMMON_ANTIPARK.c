#include "COMMON_ANTIPARK.h"
#include <math.h>
#include <stdio.h>



/**
 * @brief 反帕克变换 (Inverse Park Transformation)
 * @param input  输入的 dq 旋转坐标系矢量
 * @param angle  当前转子电角度 (弧度)
 * @param output 输出的 alpha-beta 静止坐标系矢量
 */
void Inverse_Park_Transformation(dq_t input, float angle, alphabeta_t *output) {
    // 预先计算正余弦值以提高效率
    float cos_theta = cosf(angle);
    float sin_theta = sinf(angle);

    // 反帕克变换公式
    output->alpha = input.d * cos_theta - input.q * sin_theta;
    output->beta  = input.d * sin_theta + input.q * cos_theta;
}

