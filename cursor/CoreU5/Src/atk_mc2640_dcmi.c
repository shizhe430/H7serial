/**
 ****************************************************************************************************
 * @file        atk_mc2640_dcmi.c
 * @author      ALIENTEK
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MC2640模块DCMI接口驱动代码
 *              STM32U575使用DCMI外设+GPDMA1实现摄像头数据采集。
 *              DCMI配置: 8位数据总线, 硬件同步, 上升沿捕获
 *              VSYNC低有效, HREF低有效(数据无效时为低电平)
 *              DMA: GPDMA1_Channel0, 链表模式, 32位字传输
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

#include "atk_mc2640_dcmi.h"
#include "dcmi.h"
#include <stdio.h>
#include <string.h>

#if (ATK_MC2640_USING_DCMI != 0)

extern DCMI_HandleTypeDef hdcmi;
extern UART_HandleTypeDef huart1;

static volatile uint8_t g_frame_sem = 0;
static volatile uint8_t g_error_flag = 0;

void atk_mc2640_dcmi_set_frame_done(uint8_t val)
{
    g_frame_sem = val;
}

uint8_t atk_mc2640_dcmi_get_frame_done(void)
{
    return g_frame_sem;
}

void atk_mc2640_dcmi_set_error(uint8_t val)
{
    g_error_flag = val;
}

uint8_t atk_mc2640_dcmi_get_error(void)
{
    return g_error_flag;
}

static void dcmi_debug_print(const char *text)
{
    if (text != NULL)
    {
        (void)HAL_UART_Transmit(&huart1, (uint8_t *)text, (uint16_t)strlen(text), HAL_MAX_DELAY);
    }
}

void atk_mc2640_dcmi_init(void)
{
}

uint8_t atk_mc2640_dcmi_start(uint32_t dts_addr, uint32_t meminc, uint32_t memdataalignment, uint32_t len)
{
    uint32_t start_tick;
    HAL_StatusTypeDef status;
    char dbg[96];

    (void)meminc;
    (void)memdataalignment;

    g_frame_sem = 0;
    g_error_flag = 0;

    __HAL_DCMI_ENABLE_IT(&hdcmi, DCMI_IT_FRAME);

    status = HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_SNAPSHOT, dts_addr, len);
    if (status != HAL_OK)
    {
        (void)snprintf(dbg, sizeof(dbg), "dcmi start dma fail, st=%lu\r\n", (uint32_t)status);
        dcmi_debug_print(dbg);
        return ATK_MC2640_ERROR;
    }

    start_tick = HAL_GetTick();
    while (g_frame_sem == 0)
    {
        if ((HAL_GetTick() - start_tick) > 2000U)
        {
            HAL_DCMI_Stop(&hdcmi);
            (void)snprintf(dbg, sizeof(dbg), "dcmi timeout, err=%lu sr=0x%08lX\r\n",
                           (uint32_t)g_error_flag,
                           (uint32_t)hdcmi.Instance->MISR);
            dcmi_debug_print(dbg);
            return ATK_MC2640_ERROR;
        }
    }

    HAL_DCMI_Stop(&hdcmi);

    if (g_error_flag)
    {
        (void)snprintf(dbg, sizeof(dbg), "dcmi error flag set, sr=0x%08lX\r\n", (uint32_t)hdcmi.Instance->MISR);
        dcmi_debug_print(dbg);
        return ATK_MC2640_ERROR;
    }

    return ATK_MC2640_EOK;
}

#endif
