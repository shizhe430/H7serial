/**
 ****************************************************************************************************
 * @file        atk_mc2640_dcmi.h
 * @author      ALIENTEK
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MC2640模块DCMI接口配置头文件
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

#ifndef __ATK_MC2640_DCMI_H
#define __ATK_MC2640_DCMI_H

#include "atk_mc2640.h"
#include "dcmi.h"

#if (ATK_MC2640_USING_DCMI != 0)

void atk_mc2640_dcmi_init(void);
uint8_t atk_mc2640_dcmi_start(uint32_t dts_addr, uint32_t meminc, uint32_t memdataalignment, uint32_t len);
void atk_mc2640_dcmi_set_frame_done(uint8_t val);
uint8_t atk_mc2640_dcmi_get_frame_done(void);
void atk_mc2640_dcmi_set_error(uint8_t val);
uint8_t atk_mc2640_dcmi_get_error(void);

#endif

#endif
