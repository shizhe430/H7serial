/**
 ****************************************************************************************************
 * @file        delay.h
 * @author      ALIENTEK
 * @version     V1.0
 * @date        2022-06-21
 * @brief       延时驱动头文件
 ****************************************************************************************************
 */

#ifndef __DELAY_H
#define __DELAY_H

#include "main.h"

void delay_init(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

#endif
