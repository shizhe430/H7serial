/**
  ******************************************************************************
  * @file    delay.c
  * @brief   延时函数实现 (DWT硬件实现微妙级延时)
  ******************************************************************************
  */

#include "delay.h"

static uint32_t SystemCoreClock_MHz = 0;

void delay_init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    SystemCoreClock_MHz = SystemCoreClock / 1000000;
}

void delay_us(uint32_t us)
{
    if (SystemCoreClock_MHz == 0) delay_init();
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * SystemCoreClock_MHz;
    while ((DWT->CYCCNT - start) < ticks);
}

void delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}
