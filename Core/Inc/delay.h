/**
  ******************************************************************************
  * @file    delay.h
  * @brief   延时函数头文件 (DWT硬件实现)
  ******************************************************************************
  */

#ifndef __DELAY_H
#define __DELAY_H

#include "main.h"

void delay_init(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

#endif
