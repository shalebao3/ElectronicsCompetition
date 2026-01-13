#include "DRIVER_TIM1.h"
#include "stm32f10x.h"



// ====== 可改参数 ======
#define TIM1_CLK_HZ     72000000u   // TIM1 时钟（常见 72MHz）
#define PWM_FREQ_HZ     20000u      // PWM 频率
#define DEADTIME_NS     500u        // 死区（ns）
#define DUTY_PERMILLE   500u        // 初始占空比（千分比 0~1000）

static inline uint16_t clamp_u16(uint16_t v, uint16_t lo, uint16_t hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static inline uint8_t deadtime_dtg_from_ns(uint32_t deadtime_ns)
{
    // 简化：只用 DTG=0..127 的线性段（够用就好）
    // ticks ~= deadtime / (1/TIM1_CLK_HZ) = deadtime_ns * TIM1_CLK_HZ / 1e9
    uint32_t ticks = (deadtime_ns * (uint64_t)TIM1_CLK_HZ + 999999999ull) / 1000000000ull;
    if (ticks > 127u) ticks = 127u;
    return (uint8_t)ticks;
}

void Dri_TIM1_SetDutyPermille(uint16_t d1, uint16_t d2, uint16_t d3)
{
    d1 = clamp_u16(d1, 0, 1000);
    d2 = clamp_u16(d2, 0, 1000);
    d3 = clamp_u16(d3, 0, 1000);

    uint16_t arr = (uint16_t)TIM1->ARR;

    TIM1->CCR1 = (uint16_t)(((uint32_t)(arr + 1u) * d1) / 1000u);
    TIM1->CCR2 = (uint16_t)(((uint32_t)(arr + 1u) * d2) / 1000u);
    TIM1->CCR3 = (uint16_t)(((uint32_t)(arr + 1u) * d3) / 1000u);
}

/**
 * @description: TIM1初始化，pwm模式（三相互补）
 * @return {*}
 */
void Dri_TIM1_Init(void)
{
    // 1. **Clock**：RCC 使能 + 选择时钟源/分频
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   // PA8/9/10
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;   // PB13/14/15
    // RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // ❌不 remap 不需要

    // 2. **Pinmux/Route**：GPIO/AF/重映射/路由矩阵
    // 默认映射：CH1/2/3=PA8/9/10；CH1N/2N/3N=PB13/14/15
    // 配置为 AF Push-Pull 50MHz（CNF=10, MODE=11）
    GPIOA->CRH &= ~(
        GPIO_CRH_MODE8 | GPIO_CRH_CNF8 |
        GPIO_CRH_MODE9 | GPIO_CRH_CNF9 |
        GPIO_CRH_MODE10| GPIO_CRH_CNF10
    );
    GPIOA->CRH |= (
        (GPIO_CRH_MODE8_0  | GPIO_CRH_MODE8_1  | GPIO_CRH_CNF8_1)  |
        (GPIO_CRH_MODE9_0  | GPIO_CRH_MODE9_1  | GPIO_CRH_CNF9_1)  |
        (GPIO_CRH_MODE10_0 | GPIO_CRH_MODE10_1 | GPIO_CRH_CNF10_1)
    );

    GPIOB->CRH &= ~(
        GPIO_CRH_MODE13 | GPIO_CRH_CNF13 |
        GPIO_CRH_MODE14 | GPIO_CRH_CNF14 |
        GPIO_CRH_MODE15 | GPIO_CRH_CNF15
    );
    GPIOB->CRH |= (
        (GPIO_CRH_MODE13_0 | GPIO_CRH_MODE13_1 | GPIO_CRH_CNF13_1) |
        (GPIO_CRH_MODE14_0 | GPIO_CRH_MODE14_1 | GPIO_CRH_CNF14_1) |
        (GPIO_CRH_MODE15_0 | GPIO_CRH_MODE15_1 | GPIO_CRH_CNF15_1)
    );

    // 3. **CoreConfig**：外设核心参数（波特率、分辨率、模式）
    // 先停表
    TIM1->CR1 &= ~TIM_CR1_CEN;

    // 中心对齐（更适合电机/互补 PWM）：CMS=01，ARR 预装载
    TIM1->CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS | TIM_CR1_CKD);
    TIM1->CR1 |=  TIM_CR1_CMS_0;     // Center-aligned mode 1
    TIM1->CR1 |=  TIM_CR1_ARPE;      // ARR preload

    // PWM 频率：中心对齐 f = TIMclk / (2*(PSC+1)*(ARR+1))
    uint16_t psc = 0;
    uint32_t arr = (TIM1_CLK_HZ / (2u * (uint32_t)PWM_FREQ_HZ * (psc + 1u))) - 1u;

    TIM1->PSC = psc;
    TIM1->ARR = (uint16_t)arr;

    // 4. **Channel/Feature**：通道/功能块（多路、FIFO、滤波、触发源）
    // PWM1 + CCR 预装载：OCxM=110，OCxPE=1
    TIM1->CCMR1 = 0;
    TIM1->CCMR2 = 0;

    // 4. **Channel/Feature**：通道/功能块（多路、FIFO、滤波、触发源）
    // PWM1: OCxM = 110 -> OCxM_2 | OCxM_1；再开预装载 OCxPE

    // ---- CH1 ----
    TIM1->CCMR1 &= ~(TIM_CCMR1_OC1M);  // 清 OC1M[2:0]
    TIM1->CCMR1 |=  (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE);

    // ---- CH2 ----
    TIM1->CCMR1 &= ~(TIM_CCMR1_OC2M);  // 清 OC2M[2:0]
    TIM1->CCMR1 |=  (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE);

    // ---- CH3 ----
    TIM1->CCMR2 &= ~(TIM_CCMR2_OC3M);  // 清 OC3M[2:0]
    TIM1->CCMR2 |=  (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE);


    // 初始占空比（千分比）
    Dri_TIM1_SetDutyPermille(DUTY_PERMILLE, DUTY_PERMILLE, DUTY_PERMILLE);

    // 使能输出：主通道 + 互补通道
    // CCxE=1, CCxNE=1（极性默认高有效，需要反相再配 CCxP/CCxNP）
    TIM1->CCER = 0;
    TIM1->CCER |= TIM_CCER_CC1E  | TIM_CCER_CC1NE;
    TIM1->CCER |= TIM_CCER_CC2E  | TIM_CCER_CC2NE;
    TIM1->CCER |= TIM_CCER_CC3E  | TIM_CCER_CC3NE;

    // 5. **Link**：与其它外设联动（触发/同步/路由/跨外设连接）
    // 本例不做触发同步（需要 TRGO/从模式再加）

    // 6. **IRQ**：中断使能、清标志、NVIC、优先级
    // 本例不启用中断

    // 7. **DMA**：DMA 请求、通道映射、burst/循环
    // 本例不启用 DMA

    // 8. **Start**：启动顺序、清标志、使能输出/收发
    // 死区 + MOE（高级定时器必须开 MOE，否则没波形）
    uint8_t dtg = deadtime_dtg_from_ns(DEADTIME_NS);

    TIM1->BDTR = 0;
    TIM1->BDTR |= (uint32_t)dtg;       // DTG[7:0]
    TIM1->BDTR |= TIM_BDTR_MOE;        // 主输出使能 🔥

    // 强制更新：把预装载的 PSC/ARR/CCR “拍板生效”
    TIM1->EGR = TIM_EGR_UG;

    // 启动
    TIM1->CR1 |= TIM_CR1_CEN;
}
