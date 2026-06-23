#include "camera_app.h"
#include "ov2640.h"
#include "ov2640_sccb.h"
#include "jpeg_stream.h"
#include "dcmi.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

/*
 * camera_app — OV2640 采集应用层
 *
 * 当前模式: APP_MODE_XCAM_VIEW
 *   循环: DCMI SNAPSHOT 采集一帧 → 查找 JPEG EOI → UART 原样发送
 *
 * 状态机 (预留扩展):
 *   IDLE → CAPTURE → PROCESS → SEND → IDLE
 */

extern DCMI_HandleTypeDef  hdcmi;
extern UART_HandleTypeDef  huart1;

static volatile uint8_t g_frame_done = 0U;

/* ── DCMI 回调 (覆盖 HAL 弱定义) ── */

void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi_ptr)
{
    (void)hdcmi_ptr;
    CameraApp_SignalFrameDone();
}

void HAL_DCMI_ErrorCallback(DCMI_HandleTypeDef *hdcmi_ptr)
{
    (void)hdcmi_ptr;
    CameraApp_SignalFrameDone();  /* 错误也释放等待，避免死等 */
}

/* ── ISR 回调 ── */

void CameraApp_SignalFrameDone(void)
{
    g_frame_done = 1U;
}

/* ── 内部辅助 ── */

static void dbg_print(const char *s)
{
    if (s != NULL)
    {
        (void)HAL_UART_Transmit(&huart1, (uint8_t *)s, (uint16_t)strlen(s), HAL_MAX_DELAY);
    }
}

static void dbg_dump_dcmi_status(void)
{
    char buf[100];
    int len;
    uint32_t sr = DCMI->SR;
    uint32_t risr = DCMI->RISR;
    uint32_t cr  = DCMI->CR;
    uint32_t ier = DCMI->IER;

    len = snprintf(buf, sizeof(buf),
                   "[DCMI] SR=0x%08lX RISR=0x%08lX CR=0x%08lX IER=0x%08lX\r\n"
                   "       VSYNC=%lu HSYNC=%lu FNE=%lu CAPTURE=%lu\r\n",
                   (unsigned long)sr, (unsigned long)risr, (unsigned long)cr, (unsigned long)ier,
                   (unsigned long)((sr >> 10) & 1UL),   /* VSYNC */
                   (unsigned long)((sr >> 11) & 1UL),   /* HSYNC */
                   (unsigned long)((sr >> 2)  & 1UL),   /* FNE   */
                   (unsigned long)((cr >> 0)  & 1UL));  /* CAPTURE */
    HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
}

/* OV2640 彩条测试：验证 DVP 数据通路 */
static void ov2640_colorbar_enable(void)
{
    uint8_t com7;

    /* 选 Sensor 寄存器组 */
    OV2640_SCCB_WriteReg(OV2640_SCCB_ADDR, 0xFF, 0x01);

    /* 读 COM7，置 bit2 */
    OV2640_SCCB_ReadReg(OV2640_SCCB_ADDR, 0x12, &com7);
    com7 |= (uint8_t)(1U << 2);
    OV2640_SCCB_WriteReg(OV2640_SCCB_ADDR, 0x12, com7);
    dbg_print("[DIAG] Color bar ENABLED\r\n");
}

static void ov2640_colorbar_disable(void)
{
    uint8_t com7;

    OV2640_SCCB_WriteReg(OV2640_SCCB_ADDR, 0xFF, 0x01);
    OV2640_SCCB_ReadReg(OV2640_SCCB_ADDR, 0x12, &com7);
    com7 &= ~(uint8_t)(1U << 2);
    OV2640_SCCB_WriteReg(OV2640_SCCB_ADDR, 0x12, com7);
}

/* ── 公开接口 ── */

void CameraApp_Init(void)
{
    char buf[80];
    int  len;
    uint16_t mid = 0U, pid = 0U;

    /* 1) Probe ID (上电+复位+SCCB+SW复位+读ID) */
    if (OV2640_Probe(&mid, &pid) != OV2640_OK)
    {
        dbg_print("[APP] Probe FAILED!\r\n");
        return;
    }
    len = snprintf(buf, sizeof(buf),
                   "[APP] Probe OK  MID=0x%04X PID=0x%04X\r\n", mid, pid);
    HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);

    /* 2) 写JPEG配置表 (会重新上电+复位+SW复位+写表) */
    dbg_print("[APP] Writing config tables...\r\n");
    if (OV2640_Init() != OV2640_OK)
    {
        dbg_print("[APP] Init FAILED!\r\n");
        return;
    }
    dbg_print("[APP] Init OK\r\n");

    /* 3) JPEG 流缓冲 */
    JPEG_Stream_Init();

    len = snprintf(buf, sizeof(buf),
                   "[APP] BUF addr=0x%08lX size=%lu\r\n",
                   (unsigned long)(uint32_t)JPEG_Stream_GetBuf(),
                   (unsigned long)JPEG_Stream_GetMaxSize());
    HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);

    /* 4) DCMI IRQ */
    HAL_NVIC_SetPriority(DCMI_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DCMI_IRQn);

    /* 5) AWB等待 */
    dbg_print("[APP] Waiting AWB/AEC converge (1.5s)...\r\n");
    HAL_Delay(1500U);

    /* ─── 诊断: 17:40版本 — 彩条+SNAPSHOT+CONTINUOUS全套 ─── */
    {
        uint32_t diag_soi = 0U;
        uint32_t diag_len;
        uint32_t diag_tick;
        uint8_t  reg_com7, reg_com10, reg_0d, reg_0e;
        uint8_t  reg_33, reg_3c, reg_da, reg_11;

        /* 回读 OV2640 关键寄存器 */
        OV2640_SCCB_WriteReg(OV2640_SCCB_ADDR, 0xFF, 0x01);
        OV2640_SCCB_ReadReg(OV2640_SCCB_ADDR, 0x12, &reg_com7);
        OV2640_SCCB_ReadReg(OV2640_SCCB_ADDR, 0x15, &reg_com10);
        OV2640_SCCB_ReadReg(OV2640_SCCB_ADDR, 0x0D, &reg_0d);
        OV2640_SCCB_ReadReg(OV2640_SCCB_ADDR, 0x0E, &reg_0e);
        OV2640_SCCB_ReadReg(OV2640_SCCB_ADDR, 0x11, &reg_11);

        OV2640_SCCB_WriteReg(OV2640_SCCB_ADDR, 0xFF, 0x00);
        OV2640_SCCB_ReadReg(OV2640_SCCB_ADDR, 0x33, &reg_33);
        OV2640_SCCB_ReadReg(OV2640_SCCB_ADDR, 0x3C, &reg_3c);
        OV2640_SCCB_ReadReg(OV2640_SCCB_ADDR, 0xDA, &reg_da);

        len = snprintf(buf, sizeof(buf),
                       "[DIAG] Sns:COM7=0x%02X COM10=0x%02X CLKRC=0x%02X\r\n"
                       "       DSP:0x33=0x%02X 0x3C=0x%02X IMG_MODE=0x%02X\r\n",
                       reg_com7, reg_com10, reg_11,
                       reg_33, reg_3c, reg_da);
        HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);

        ov2640_colorbar_enable();
        HAL_Delay(100U);

        /* A: SNAPSHOT */
        dbg_print("[DIAG] A: SNAPSHOT capture...\r\n");
        g_frame_done = 0U;
        __HAL_DCMI_ENABLE_IT(&hdcmi, DCMI_IT_FRAME);
        if (HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_SNAPSHOT,
                               (uint32_t)JPEG_Stream_GetBuf(),
                               JPEG_Stream_GetMaxSize() / 4U) == HAL_OK)
        {
            diag_tick = HAL_GetTick();
            while (g_frame_done == 0U)
            {
                if ((HAL_GetTick() - diag_tick) > 2000U) break;
            }
            HAL_DCMI_Stop(&hdcmi);
        }
        dbg_dump_dcmi_status();
        diag_len = JPEG_Stream_FindFrame(&diag_soi);
        len = snprintf(buf, sizeof(buf),
                       "[DIAG] A result: SOI=%lu len=%lu\r\n",
                       (unsigned long)diag_soi, (unsigned long)diag_len);
        HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);

        /* B: CONTINUOUS PCLK test */
        dbg_print("[DIAG] B: CONTINUOUS mode (PCLK test)...\r\n");
        HAL_DCMI_Stop(&hdcmi);
        HAL_Delay(50U);
        if (HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS,
                               (uint32_t)JPEG_Stream_GetBuf(),
                               JPEG_Stream_GetMaxSize() / 4U) == HAL_OK)
        {
            uint32_t sr_poll;
            uint8_t  fne_ever = 0U;
            diag_tick = HAL_GetTick();
            while ((HAL_GetTick() - diag_tick) < 1000U)
            {
                sr_poll = DCMI->SR;
                if (sr_poll & DCMI_SR_FNE)
                {
                    fne_ever = 1U;
                    break;
                }
            }
            HAL_DCMI_Stop(&hdcmi);
            len = snprintf(buf, sizeof(buf),
                           "[DIAG] B: FNE ever=%lu (1=PCLK present, 0=NO clock)\r\n",
                           (unsigned long)fne_ever);
            HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
        }

        ov2640_colorbar_disable();
    }

    dbg_print("[APP] Ready. XCAM_VIEW streaming...\r\n");
}

void CameraApp_Run(void)
{
    uint32_t jpeg_len;
    uint32_t soi_off;
    uint32_t start_tick;

#if (APP_MODE == APP_MODE_XCAM_VIEW)

    /* CONTINUOUS模式: PCLK已验证通, 定时窗口采集 */
    HAL_DCMI_Stop(&hdcmi);
    HAL_Delay(5U);

    if (HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS,
                           (uint32_t)JPEG_Stream_GetBuf(),
                           JPEG_Stream_GetMaxSize() / 4U) != HAL_OK)
    {
        HAL_DCMI_Stop(&hdcmi);
        HAL_Delay(100U);
        return;
    }

    /* 300ms采集窗口 — 足够捕获多帧JPEG */
    start_tick = HAL_GetTick();
    while ((HAL_GetTick() - start_tick) < 300U) { __NOP(); }

    HAL_DCMI_Stop(&hdcmi);

    jpeg_len = JPEG_Stream_FindFrame(&soi_off);
    if (jpeg_len == 0U)
    {
        /* 前5帧打印调试 */
        static uint32_t nof_cnt = 0U;
        if (nof_cnt < 5U)
        {
            char dbg[40];
            int  l = snprintf(dbg, sizeof(dbg), "[F%lu] no JPEG\r\n",
                              (unsigned long)nof_cnt);
            HAL_UART_Transmit(&huart1, (uint8_t *)dbg, (uint16_t)l, HAL_MAX_DELAY);
            nof_cnt++;
        }
        return;
    }
    HAL_UART_Transmit(&huart1,
                      JPEG_Stream_GetBuf() + soi_off,
                      (uint16_t)jpeg_len,
                      HAL_MAX_DELAY);

#else
    (void)jpeg_len; (void)soi_off; (void)start_tick;
#endif
}
