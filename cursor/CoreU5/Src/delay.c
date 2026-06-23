/**
 ****************************************************************************************************
 * @file        delay.c
 * @author      ALIENTEK
 * @version     V1.0
 * @date        2022-06-21
 * @brief       延时驱动实现
 *              使用DWT(Data Watchpoint and Trace)周期计数器实现精确微秒延时
 *              DWT是Cortex-M33内核自带的调试单元，32位循环计数器
 *              每个CPU时钟周期递增1，可提供纳秒级精度
 ****************************************************************************************************
 */

#include "delay.h"

static uint32_t g_fac_us = 0;

void delay_init(void)
{
    g_fac_us = SystemCoreClock / 1000000;
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void delay_us(uint32_t us)
{
    uint32_t start;
    uint32_t total;

    total = us * g_fac_us;
    start = DWT->CYCCNT;

    while ((DWT->CYCCNT - start) < total)
    {
    }
}

void delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}
