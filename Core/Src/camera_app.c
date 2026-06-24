#include "camera_app.h"
#include "dcmi.h"
#include "jpeg_stream.h"
#include "ov2640.h"
#include "ov2640_sccb.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

extern DCMI_HandleTypeDef hdcmi;
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_dcmi;

static volatile uint8_t g_frame_done = 0U;
static volatile uint8_t g_frame_error = 0U;

#define CAMERA_APP_DEBUG  0U
#define CAMERA_JPEG_DIAG  0U
#define CAMERA_WIDTH      320U
#define CAMERA_HEIGHT     240U

#define CAMERA_DMA_SETTLE_SPINS   8192U
#define CAMERA_DMA_STABLE_SPINS   64U

static void dbg_print(const char *s)
{
#if (CAMERA_APP_DEBUG != 0U)
    if (s != NULL)
    {
        (void)HAL_UART_Transmit(&huart1, (uint8_t *)s, (uint16_t)strlen(s), HAL_MAX_DELAY);
    }
#else
    (void)s;
#endif
}

static uint32_t camera_app_dma_get_ndtr(void)
{
    DMA_Stream_TypeDef *dma_stream = (DMA_Stream_TypeDef *)hdma_dcmi.Instance;

    if (dma_stream == NULL)
    {
        return 0U;
    }

    return dma_stream->NDTR;
}

#if (CAMERA_JPEG_DIAG != 0U)
static uint32_t camera_app_count_marker(uint8_t first, uint8_t second)
{
    uint32_t i;
    uint32_t count = 0U;
    uint8_t *buf = JPEG_Stream_GetBuf();

    for (i = 0U; i + 1U < JPEG_Stream_GetMaxSize(); i++)
    {
        if ((buf[i] == first) && (buf[i + 1U] == second))
        {
            count++;
        }
    }

    return count;
}

static void camera_app_dump_nojpeg_diag(const char *tag)
{
    char buf[256];
    int len;
    uint8_t *jpeg = JPEG_Stream_GetBuf();
    uint32_t soi_cnt = camera_app_count_marker(0xFFU, 0xD8U);
    uint32_t eoi_cnt = camera_app_count_marker(0xFFU, 0xD9U);
    uint32_t sr = DCMI->SR;
    uint32_t risr = DCMI->RISR;
    uint32_t cr = DCMI->CR;
    uint32_t pa6 = (GPIOA->IDR >> 6U) & 1U;
    uint32_t ph8 = (GPIOH->IDR >> 8U) & 1U;
    uint32_t pb7_9 = (GPIOB->IDR >> 7U) & 0x07U;
    uint32_t pd3 = (GPIOD->IDR >> 3U) & 0x01U;

    len = snprintf(buf, sizeof(buf),
                   "[JPEG:%s] ndtr=%lu sr=0x%08lX risr=0x%08lX cr=0x%08lX pclk=%lu href=%lu pb7_9=0x%lX pd3=%lu soi=%lu eoi=%lu buf0=%02X %02X %02X %02X %02X %02X %02X %02X\r\n",
                   tag,
                   (unsigned long)camera_app_dma_get_ndtr(),
                   (unsigned long)sr,
                   (unsigned long)risr,
                   (unsigned long)cr,
                   (unsigned long)pa6,
                   (unsigned long)ph8,
                   (unsigned long)pb7_9,
                   (unsigned long)pd3,
                   (unsigned long)soi_cnt,
                   (unsigned long)eoi_cnt,
                   jpeg[0], jpeg[1], jpeg[2], jpeg[3],
                   jpeg[4], jpeg[5], jpeg[6], jpeg[7]);
    HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
}

static void camera_app_dump_jpeg_diag(uint32_t soi_off, uint32_t jpeg_len)
{
    char buf[192];
    int len;
    uint8_t *jpeg = JPEG_Stream_GetBuf();
    uint32_t soi_cnt = camera_app_count_marker(0xFFU, 0xD8U);
    uint32_t eoi_cnt = camera_app_count_marker(0xFFU, 0xD9U);

    len = snprintf(buf, sizeof(buf),
                   "[JPEG] off=%lu len=%lu ndtr=%lu sr=0x%08lX soi=%lu eoi=%lu head=%02X %02X %02X %02X %02X %02X tail=%02X %02X %02X %02X\r\n",
                   (unsigned long)soi_off,
                   (unsigned long)jpeg_len,
                   (unsigned long)camera_app_dma_get_ndtr(),
                   (unsigned long)DCMI->SR,
                   (unsigned long)soi_cnt,
                   (unsigned long)eoi_cnt,
                   jpeg[soi_off],
                   jpeg[soi_off + 1U],
                   jpeg[soi_off + 2U],
                   jpeg[soi_off + 3U],
                   jpeg[soi_off + 4U],
                   jpeg[soi_off + 5U],
                   jpeg[soi_off + jpeg_len - 4U],
                   jpeg[soi_off + jpeg_len - 3U],
                   jpeg[soi_off + jpeg_len - 2U],
                   jpeg[soi_off + jpeg_len - 1U]);
    HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
}
#endif

static void camera_app_wait_dma_settle(void)
{
    uint32_t last_ndtr;
    uint32_t stable_cnt = 0U;
    uint32_t spin;

    last_ndtr = camera_app_dma_get_ndtr();

    for (spin = 0U; spin < CAMERA_DMA_SETTLE_SPINS; spin++)
    {
        uint32_t ndtr = camera_app_dma_get_ndtr();
        uint32_t sr = DCMI->SR;

        if ((sr & DCMI_SR_FNE) == 0U)
        {
            if (ndtr == last_ndtr)
            {
                stable_cnt++;
                if (stable_cnt >= CAMERA_DMA_STABLE_SPINS)
                {
                    break;
                }
            }
            else
            {
                stable_cnt = 0U;
                last_ndtr = ndtr;
            }
        }
        else
        {
            stable_cnt = 0U;
            last_ndtr = ndtr;
        }
    }
}

#if (CAMERA_APP_DEBUG != 0U)
static void dbg_dump_dcmi_status(void)
{
    char buf[100];
    int len;
    uint32_t sr = DCMI->SR;
    uint32_t risr = DCMI->RISR;
    uint32_t cr = DCMI->CR;
    uint32_t ier = DCMI->IER;

    len = snprintf(buf, sizeof(buf),
                   "[DCMI] SR=0x%08lX RISR=0x%08lX CR=0x%08lX IER=0x%08lX\r\n"
                   "       VSYNC=%lu HSYNC=%lu FNE=%lu CAPTURE=%lu\r\n",
                   (unsigned long)sr, (unsigned long)risr, (unsigned long)cr, (unsigned long)ier,
                   (unsigned long)((sr >> 10) & 1UL),
                   (unsigned long)((sr >> 11) & 1UL),
                   (unsigned long)((sr >> 2) & 1UL),
                   (unsigned long)(cr & 1UL));
    HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
}

static void ov2640_colorbar_enable(void)
{
    uint8_t com7;

    OV2640_SCCB_WriteReg(OV2640_SCCB_ADDR, 0xFF, 0x01);
    OV2640_SCCB_ReadReg(OV2640_SCCB_ADDR, 0x12, &com7);
    com7 |= (uint8_t)(1U << 1);
    OV2640_SCCB_WriteReg(OV2640_SCCB_ADDR, 0x12, com7);
}

static void ov2640_colorbar_disable(void)
{
    uint8_t com7;

    OV2640_SCCB_WriteReg(OV2640_SCCB_ADDR, 0xFF, 0x01);
    OV2640_SCCB_ReadReg(OV2640_SCCB_ADDR, 0x12, &com7);
    com7 &= ~(uint8_t)(1U << 1);
    OV2640_SCCB_WriteReg(OV2640_SCCB_ADDR, 0x12, com7);
}

static void camera_app_run_diagnostics(void)
{
    char buf[96];
    int len;
    uint32_t diag_soi = 0U;
    uint32_t diag_len;
    uint32_t diag_tick;
    uint8_t reg_com7;
    uint8_t reg_com10;
    uint8_t reg_33;
    uint8_t reg_3c;
    uint8_t reg_da;
    uint8_t reg_11;

    OV2640_SCCB_WriteReg(OV2640_SCCB_ADDR, 0xFF, 0x01);
    OV2640_SCCB_ReadReg(OV2640_SCCB_ADDR, 0x12, &reg_com7);
    OV2640_SCCB_ReadReg(OV2640_SCCB_ADDR, 0x15, &reg_com10);
    OV2640_SCCB_ReadReg(OV2640_SCCB_ADDR, 0x11, &reg_11);

    OV2640_SCCB_WriteReg(OV2640_SCCB_ADDR, 0xFF, 0x00);
    OV2640_SCCB_ReadReg(OV2640_SCCB_ADDR, 0x33, &reg_33);
    OV2640_SCCB_ReadReg(OV2640_SCCB_ADDR, 0x3C, &reg_3c);
    OV2640_SCCB_ReadReg(OV2640_SCCB_ADDR, 0xDA, &reg_da);

    len = snprintf(buf, sizeof(buf),
                   "[DIAG] COM7=0x%02X COM10=0x%02X CLKRC=0x%02X 33=0x%02X 3C=0x%02X DA=0x%02X\r\n",
                   reg_com7, reg_com10, reg_11, reg_33, reg_3c, reg_da);
    HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);

    ov2640_colorbar_enable();
    HAL_Delay(100U);

    g_frame_done = 0U;
    __HAL_DCMI_ENABLE_IT(&hdcmi, DCMI_IT_FRAME);
    if (HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_SNAPSHOT,
                           (uint32_t)JPEG_Stream_GetBuf(),
                           JPEG_Stream_GetMaxSize() / 4U) == HAL_OK)
    {
        diag_tick = HAL_GetTick();
        while (g_frame_done == 0U)
        {
            if ((HAL_GetTick() - diag_tick) > 2000U)
            {
                break;
            }
        }
        HAL_DCMI_Stop(&hdcmi);
    }

    dbg_dump_dcmi_status();
    diag_len = JPEG_Stream_FindFrame(&diag_soi);
    len = snprintf(buf, sizeof(buf),
                   "[DIAG] SOI=%lu len=%lu\r\n",
                   (unsigned long)diag_soi, (unsigned long)diag_len);
    HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);

    ov2640_colorbar_disable();
}
#endif

void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi_ptr)
{
    (void)hdcmi_ptr;
    CameraApp_SignalFrameDone();
}

void HAL_DCMI_ErrorCallback(DCMI_HandleTypeDef *hdcmi_ptr)
{
    (void)hdcmi_ptr;
    g_frame_error = 1U;
    CameraApp_SignalFrameDone();
}

void CameraApp_SignalFrameDone(void)
{
    g_frame_done = 1U;
}

void CameraApp_Init(void)
{
    uint16_t mid = 0U;
    uint16_t pid = 0U;

#if (CAMERA_JPEG_DIAG != 0U)
    {
        static const char msg[] = "[JPEG] diag mode on\r\n";
        HAL_UART_Transmit(&huart1, (uint8_t *)msg, (uint16_t)(sizeof(msg) - 1U), HAL_MAX_DELAY);
    }
#endif

    if (OV2640_Probe(&mid, &pid) != OV2640_OK)
    {
#if (CAMERA_JPEG_DIAG != 0U)
        {
            static const char msg[] = "[JPEG] probe fail\r\n";
            HAL_UART_Transmit(&huart1, (uint8_t *)msg, (uint16_t)(sizeof(msg) - 1U), HAL_MAX_DELAY);
        }
#endif
        dbg_print("[APP] Probe FAILED!\r\n");
        return;
    }

#if (CAMERA_APP_DEBUG != 0U)
    {
        char buf[80];
        int len;
    len = snprintf(buf, sizeof(buf),
                   "[APP] Probe OK MID=0x%04X PID=0x%04X\r\n", mid, pid);
    HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
    }
#endif

    if (OV2640_Init() != OV2640_OK)
    {
#if (CAMERA_JPEG_DIAG != 0U)
        {
            static const char msg[] = "[JPEG] init fail\r\n";
            HAL_UART_Transmit(&huart1, (uint8_t *)msg, (uint16_t)(sizeof(msg) - 1U), HAL_MAX_DELAY);
        }
#endif
        dbg_print("[APP] Init FAILED!\r\n");
        return;
    }

    if (OV2640_SetOutputFormatJPEG() != OV2640_OK)
    {
#if (CAMERA_JPEG_DIAG != 0U)
        {
            static const char msg[] = "[JPEG] fmt fail\r\n";
            HAL_UART_Transmit(&huart1, (uint8_t *)msg, (uint16_t)(sizeof(msg) - 1U), HAL_MAX_DELAY);
        }
#endif
        dbg_print("[APP] JPEG format FAILED!\r\n");
        return;
    }

    if (OV2640_SetOutputSize(CAMERA_WIDTH, CAMERA_HEIGHT) != OV2640_OK)
    {
#if (CAMERA_JPEG_DIAG != 0U)
        {
            static const char msg[] = "[JPEG] size fail\r\n";
            HAL_UART_Transmit(&huart1, (uint8_t *)msg, (uint16_t)(sizeof(msg) - 1U), HAL_MAX_DELAY);
        }
#endif
        dbg_print("[APP] Output size FAILED!\r\n");
        return;
    }

    JPEG_Stream_Init();
    OV2640_AttachFrameBuffer(JPEG_Stream_GetBuf(), JPEG_Stream_GetMaxSize());

    HAL_NVIC_SetPriority(DCMI_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DCMI_IRQn);

    HAL_Delay(1500U);

#if (CAMERA_APP_DEBUG != 0U)
    {
    char buf[80];
    int len;
    len = snprintf(buf, sizeof(buf),
                   "[APP] BUF addr=0x%08lX size=%lu\r\n",
                   (unsigned long)(uint32_t)JPEG_Stream_GetBuf(),
                   (unsigned long)JPEG_Stream_GetMaxSize());
    HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
    camera_app_run_diagnostics();
    }
#endif
}

void CameraApp_Run(void)
{
    uint32_t jpeg_len;
    uint32_t soi_off;
    uint32_t start_tick;

#if (APP_MODE == APP_MODE_XCAM_VIEW)
    g_frame_done = 0U;
    g_frame_error = 0U;
    if (OV2640_StartSnapshot() != OV2640_OK)
    {
#if (CAMERA_JPEG_DIAG != 0U)
        camera_app_dump_nojpeg_diag("start");
#endif
        HAL_DCMI_Stop(&hdcmi);
        HAL_Delay(20U);
        return;
    }

    start_tick = HAL_GetTick();
    while (g_frame_done == 0U)
    {
        if ((HAL_GetTick() - start_tick) > 1000U)
        {
#if (CAMERA_JPEG_DIAG != 0U)
            camera_app_dump_nojpeg_diag("timeout");
#endif
            HAL_DCMI_Stop(&hdcmi);
            return;
        }
    }

    camera_app_wait_dma_settle();
    HAL_DCMI_Stop(&hdcmi);

    if (g_frame_error != 0U)
    {
#if (CAMERA_JPEG_DIAG != 0U)
        camera_app_dump_nojpeg_diag("dcmierr");
#endif
        return;
    }

    jpeg_len = JPEG_Stream_FindFrame(&soi_off);
    if (jpeg_len == 0U)
    {
#if (CAMERA_JPEG_DIAG != 0U)
        camera_app_dump_nojpeg_diag("noframe");
#endif
        return;
    }

    if ((soi_off + jpeg_len) > JPEG_Stream_GetMaxSize())
    {
#if (CAMERA_JPEG_DIAG != 0U)
        camera_app_dump_nojpeg_diag("range");
#endif
        return;
    }

    if ((JPEG_Stream_GetBuf()[soi_off] != 0xFFU) ||
        (JPEG_Stream_GetBuf()[soi_off + 1U] != 0xD8U) ||
        (JPEG_Stream_GetBuf()[soi_off + jpeg_len - 2U] != 0xFFU) ||
        (JPEG_Stream_GetBuf()[soi_off + jpeg_len - 1U] != 0xD9U))
    {
#if (CAMERA_JPEG_DIAG != 0U)
        camera_app_dump_nojpeg_diag("marker");
#endif
        return;
    }

#if (CAMERA_JPEG_DIAG != 0U)
    camera_app_dump_jpeg_diag(soi_off, jpeg_len);
#else
    HAL_UART_Transmit(&huart1,
                      JPEG_Stream_GetBuf() + soi_off,
                      (uint16_t)jpeg_len,
                      HAL_MAX_DELAY);
#endif
#else
    (void)jpeg_len;
    (void)soi_off;
    (void)start_tick;
#endif
}
