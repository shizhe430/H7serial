#include "jpeg_stream.h"

/*
 * JPEG 帧缓冲 —— 独立模块
 * - 64KB, 32字节对齐, .bss 段自动落 RAM_D1 (0x24000000)
 * - MPU 已将该区域设为 NON_CACHEABLE，无需手动 Cache 维护
 * - 应用层通过 JPEG_Stream_FindFrame() 搜索 SOI(0xFFD8) / EOI(0xFFD9)
 */

static volatile uint8_t s_buf[JPEG_BUF_SIZE] __attribute__((aligned(32)));

void JPEG_Stream_Init(void)
{
    /* .bss 已清零，无额外初始化 */
}

uint8_t *JPEG_Stream_GetBuf(void)
{
    return (uint8_t *)s_buf;
}

uint32_t JPEG_Stream_GetMaxSize(void)
{
    return JPEG_BUF_SIZE;
}

uint32_t JPEG_Stream_FindFrame(uint32_t *p_soi_offset)
{
    uint32_t i;
    uint32_t soi = 0U;
    uint32_t eoi = 0U;

    for (i = 0U; i < JPEG_BUF_SIZE - 1U; i++)
    {
        if (s_buf[i] == 0xFFU && s_buf[i + 1U] == 0xD8U)
        {
            soi = i;
            break;
        }
    }

    for (i = soi + 2U; i < JPEG_BUF_SIZE - 1U; i++)
    {
        if (s_buf[i] == 0xFFU && s_buf[i + 1U] == 0xD9U)
        {
            eoi = i + 2U;
            break;
        }
    }

    if (p_soi_offset != NULL)
    {
        *p_soi_offset = soi;
    }

    return (eoi > soi) ? (eoi - soi) : 0U;
}
