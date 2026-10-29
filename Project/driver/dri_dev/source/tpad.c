#include "tpad.h"
#include "../../../User/delay.h"

/* 空载基准值：没有触摸时，TPAD 充电到捕获电平所需的计数值。 */
volatile uint16_t g_tpad_default_val = 0;

/* 下面三个函数只在本文件内部使用，不暴露给外部模块。 */
static void tpad_reset(void);
static uint16_t tpad_get_val(void);
static uint16_t tpad_get_maxval(uint8_t n);

uint8_t tpad_init(void)
{
    uint16_t buf[10];
    uint16_t temp;
    uint8_t i;
    uint8_t j;

    /* 开启 GPIOA 和 TIM2 时钟。TPAD 接在 PA5，也就是 TIM2_CH1。 */
    RCC->AHB1ENR |= (1 << 0);
    RCC->APB1ENR |= (1 << 0);

    /* PA5 复用为 AF1，对应 TIM2_CH1 输入捕获。 */
    GPIOA->MODER &= ~(3 << (5 * 2));
    GPIOA->MODER |=  (2 << (5 * 2));
    GPIOA->AFR[0] &= ~(0xF << (5 * 4));
    GPIOA->AFR[0] |=  (1 << (5 * 4));

    /* TIM2 输入捕获配置：PSC=7 对齐标准例程 tpad_init(8) 的设置。 */
    TIM2->PSC = 7;
    TIM2->ARR = TPAD_ARR_MAX_VAL;
    TIM2->EGR = TIM_EGR_UG;
    TIM2->CR1 = 0;
    TIM2->DIER = 0;
    TIM2->CCMR1 &= ~(3 << 0);
    TIM2->CCMR1 |=  (1 << 0);
    TIM2->CCER = TIM_CCER_CC1E;

    /* 连续采样 10 次，用来计算空载基准值。 */
    for (i = 0; i < 10; i++)
    {
        buf[i] = tpad_get_val();
        delay_ms(10);
    }

    for (i = 0; i < 9; i++)
    {
        for (j = i + 1; j < 10; j++)
        {
            if (buf[i] > buf[j])
            {
                uint16_t t = buf[i];
                buf[i] = buf[j];
                buf[j] = t;
            }
        }
    }

    /* 去掉偏大/偏小的样本，取中间 6 个值平均，减少抖动影响。 */
    temp = 0;
    for (i = 2; i < 8; i++)
    {
        temp += buf[i];
    }

    g_tpad_default_val = temp / 6;

    if (g_tpad_default_val > ((uint16_t)TPAD_ARR_MAX_VAL / 2))
    {
        return 1;
    }

    return 0;
}

static void tpad_reset(void)
{
    /* 先把 PA5 切成普通输出低电平，给触摸电容放电。 */
    GPIOA->MODER &= ~(3 << (5 * 2));
    GPIOA->MODER |=  (1 << (5 * 2));
    GPIOA->OTYPER &= ~(1 << 5);
    GPIOA->PUPDR &= ~(3 << (5 * 2));
    GPIOA->ODR &= ~(1 << 5);
    delay_ms(5);

    /* 放电完成后切回 TIM2_CH1 复用功能，等待电容重新充电产生捕获边沿。 */
    GPIOA->MODER &= ~(3 << (5 * 2));
    GPIOA->MODER |=  (2 << (5 * 2));
    GPIOA->AFR[0] &= ~(0xF << (5 * 4));
    GPIOA->AFR[0] |=  (1 << (5 * 4));

    /* 每次测量前清标志和计数器，再启动 TIM2。 */
    TIM2->CR1 &= ~TIM_CR1_CEN;
    TIM2->SR = 0;
    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_CEN;
}

static uint16_t tpad_get_val(void)
{
    uint8_t timeout = 0;

    /* 完成一次“放电 -> 重新充电 -> 输入捕获”的测量。 */
    tpad_reset();

    while ((TIM2->SR & TIM_SR_CC1IF) == 0)
    {
        if (TIM2->CNT > TPAD_TIMEOUT_VAL)
        {
            timeout = 1;
            break;
        }
    }

    if (timeout)
    {
        return (uint16_t)TIM2->CNT;
    }

    return (uint16_t)TIM2->CCR1;
}

static uint16_t tpad_get_maxval(uint8_t n)
{
    uint16_t maxv = 0;

    while (n--)
    {
        uint16_t temp = tpad_get_val();
        if (temp > maxv)
        {
            maxv = temp;
        }
    }

    return maxv;
}

uint8_t tpad_scan(uint8_t mode)
{
    static uint8_t keyen = 0;
    uint8_t res = 0;
    uint8_t sample = (mode == 0) ? 3 : 6;
    uint16_t rval = tpad_get_maxval(sample);

    /* 连续触发模式下，每次扫描都允许重新判定触摸。 */
    if (mode)
    {
        keyen = 0;
    }

    /* 触摸会让电容变大，充电变慢，所以捕获计数值会高于空载基准。 */
    if (rval > (g_tpad_default_val + TPAD_GATE_VAL))
    {
        if (keyen == 0)
        {
            res = 1;
        }
        keyen = 3;
    }

    if (keyen)
    {
        keyen--;
    }

    return res;
}

uint8_t tpad_is_touched(void)
{
    return tpad_scan(0);
}

uint16_t tpad_get_debug_val(void)
{
    return tpad_get_val();
}
