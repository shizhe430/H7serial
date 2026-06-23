/**
 ****************************************************************************************************
 * @file        atk_mc2640_sccb.h
 * @author      ALIENTEK
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MC2640模块SCCB接口配置头文件
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:ALIENTEK Mini Pro H750开发板
 * 视频教程:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司地址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#ifndef __ATK_MC2640_SCCB_H
#define __ATK_MC2640_SCCB_H

#include "main.h"

#define ATK_MC2640_SCCB_GPIO_PULLUP             0

#define ATK_MC2640_SCCB_SCL_GPIO_PORT           GPIOB
#define ATK_MC2640_SCCB_SCL_GPIO_PIN            GPIO_PIN_13
#define ATK_MC2640_SCCB_SCL_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)
#define ATK_MC2640_SCCB_SDA_GPIO_PORT           GPIOB
#define ATK_MC2640_SCCB_SDA_GPIO_PIN            GPIO_PIN_14
#define ATK_MC2640_SCCB_SDA_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

#define ATK_MC2640_SCCB_SCL(x)                  do{ x ?                                                                                             \
                                                    HAL_GPIO_WritePin(ATK_MC2640_SCCB_SCL_GPIO_PORT, ATK_MC2640_SCCB_SCL_GPIO_PIN, GPIO_PIN_SET) :  \
                                                    HAL_GPIO_WritePin(ATK_MC2640_SCCB_SCL_GPIO_PORT, ATK_MC2640_SCCB_SCL_GPIO_PIN, GPIO_PIN_RESET); \
                                                }while(0)
#define ATK_MC2640_SCCB_SDA(x)                  do{ x ?                                                                                             \
                                                    HAL_GPIO_WritePin(ATK_MC2640_SCCB_SDA_GPIO_PORT, ATK_MC2640_SCCB_SDA_GPIO_PIN, GPIO_PIN_SET) :  \
                                                    HAL_GPIO_WritePin(ATK_MC2640_SCCB_SDA_GPIO_PORT, ATK_MC2640_SCCB_SDA_GPIO_PIN, GPIO_PIN_RESET); \
                                                }while(0)
#define ATK_MC2640_SCCB_READ_SDA()              HAL_GPIO_ReadPin(ATK_MC2640_SCCB_SDA_GPIO_PORT, ATK_MC2640_SCCB_SDA_GPIO_PIN)

void atk_mc2640_sccb_init(void);
void atk_mc2640_sccb_3_phase_write(uint8_t id_addr, uint8_t sub_addr, uint8_t dat);
void atk_mc2640_sccb_2_phase_write(uint8_t id_addr, uint8_t sub_addr);
void atk_mc2640_sccb_2_phase_read(uint8_t id_addr, uint8_t *dat);

#endif
