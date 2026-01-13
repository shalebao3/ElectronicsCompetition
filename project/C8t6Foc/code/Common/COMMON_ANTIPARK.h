#ifndef __COMMON_ANTIPARK_H
#define __COMMON_ANTIPARK_H

#include "main.h"

/* 1. 先定义结构体类型（必须放在最前面） */
typedef struct {
    float d;
    float q;
} dq_t;

typedef struct {
    float alpha;
    float beta;
} alphabeta_t;

/* 2. 再声明函数原型（此时编译器已经认识 dq_t 了） */
void Inverse_Park_Transformation(dq_t input, float angle, alphabeta_t *output);

#endif /* 注意最后留一个空行 */
