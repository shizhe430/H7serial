#include "jpeg_decode.h"
#include "preprocess.h"
#include "tjpgd.h"
#include <string.h>

#define JPEG_WORKBUF_SIZE   (4096U)
#define SRC_W               (PREPROCESS_SRC_WIDTH)
#define SRC_H               (PREPROCESS_SRC_HEIGHT)
#define DST_W               (PREPROCESS_DST_WIDTH)
#define DST_H               (PREPROCESS_DST_HEIGHT)
#define SAFE_CIRCLE_RADIUS  (100)

typedef struct
{
    const uint8_t *jpg;
    uint32_t jpg_len;
    uint32_t jpg_pos;
} jpg_stream_t;

static uint8_t s_workbuf[JPEG_WORKBUF_SIZE];
static uint8_t s_gray_320x240[SRC_W * SRC_H];

static uint8_t clamp_u8(int32_t v)
{
    if (v < 0)
    {
        return 0U;
    }
    if (v > 255)
    {
        return 255U;
    }
    return (uint8_t)v;
}

static void histogram_equalize_u8(uint8_t *img, uint32_t w, uint32_t h)
{
    uint32_t hist[256] = {0U};
    uint32_t cdf[256];
    uint32_t i;
    uint32_t total = w * h;
    uint32_t cdf_min = 0U;
    uint8_t lut[256];

    if ((img == NULL) || (total == 0U))
    {
        return;
    }

    for (i = 0U; i < total; i++)
    {
        hist[img[i]]++;
    }

    cdf[0] = hist[0];
    for (i = 1U; i < 256U; i++)
    {
        cdf[i] = cdf[i - 1U] + hist[i];
    }

    for (i = 0U; i < 256U; i++)
    {
        if (cdf[i] != 0U)
        {
            cdf_min = cdf[i];
            break;
        }
    }

    if (cdf_min >= total)
    {
        return;
    }

    for (i = 0U; i < 256U; i++)
    {
        int32_t num = (int32_t)(cdf[i] - cdf_min);
        int32_t den = (int32_t)(total - cdf_min);
        int32_t v = (num <= 0 || den <= 0) ? 0 : (num * 255) / den;
        lut[i] = clamp_u8(v);
    }

    for (i = 0U; i < total; i++)
    {
        img[i] = lut[img[i]];
    }
}

static uint8_t apply_circle_mask_u8(uint8_t pix, int32_t x, int32_t y)
{
    int32_t cx = (int32_t)(DST_W / 2U);
    int32_t cy = (int32_t)(DST_H / 2U);
    int32_t dx = x - cx;
    int32_t dy = y - cy;
    int32_t r2 = SAFE_CIRCLE_RADIUS * SAFE_CIRCLE_RADIUS;
    int32_t d2 = dx * dx + dy * dy;

    if (d2 <= r2)
    {
        return pix;
    }

    return 0U;
}

static unsigned int tjpgd_input(JDEC *jd, uint8_t *buff, unsigned int nbyte)
{
    jpg_stream_t *s = (jpg_stream_t *)jd->device;
    uint32_t remain;

    if (s == NULL)
    {
        return 0U;
    }

    if (s->jpg_pos >= s->jpg_len)
    {
        return 0U;
    }

    remain = s->jpg_len - s->jpg_pos;
    if (nbyte > remain)
    {
        nbyte = (unsigned int)remain;
    }

    if (buff != NULL)
    {
        memcpy(buff, &s->jpg[s->jpg_pos], nbyte);
    }
    s->jpg_pos += nbyte;

    return nbyte;
}

static int32_t rgb_to_gray(uint8_t r, uint8_t g, uint8_t b)
{
    return ((int32_t)r * 30 + (int32_t)g * 59 + (int32_t)b * 11) / 100;
}

static int tjpgd_output(JDEC *jd, void *bitmap, JRECT *rect)
{
    uint8_t *src = (uint8_t *)bitmap;
    uint32_t x;
    uint32_t y;
    uint32_t w;
    uint32_t h;

    (void)jd;

    if ((bitmap == NULL) || (rect == NULL))
    {
        return 0U;
    }

    if ((rect->right >= SRC_W) || (rect->bottom >= SRC_H))
    {
        return 0U;
    }

    w = (uint32_t)(rect->right - rect->left + 1);
    h = (uint32_t)(rect->bottom - rect->top + 1);

    for (y = 0U; y < h; y++)
    {
        for (x = 0U; x < w; x++)
        {
            uint32_t sx = x * 3U;
            uint8_t r = src[sx + 0U];
            uint8_t g = src[sx + 1U];
            uint8_t b = src[sx + 2U];
            uint8_t gray = clamp_u8(rgb_to_gray(r, g, b));
            uint32_t dst_x = (uint32_t)rect->left + x;
            uint32_t dst_y = (uint32_t)rect->top + y;
            s_gray_320x240[dst_y * SRC_W + dst_x] = gray;
        }
        src += (w * 3U);
    }

    return 1U;
}

uint8_t jpeg_to_ai_input(const uint8_t *jpg, uint32_t jpg_len, int8_t *dst_input)
{
    JDEC jd;
    JRESULT jr;
    jpg_stream_t stream;
    uint32_t y;

    if ((jpg == NULL) || (jpg_len == 0U) || (dst_input == NULL))
    {
        return 1U;
    }

    memset(s_gray_320x240, 0, sizeof(s_gray_320x240));

    stream.jpg = jpg;
    stream.jpg_len = jpg_len;
    stream.jpg_pos = 0U;

    jr = jd_prepare(&jd, tjpgd_input, s_workbuf, sizeof(s_workbuf), &stream);
    if (jr != JDR_OK)
    {
        return 2U;
    }

    jr = jd_decomp(&jd, tjpgd_output, 0);
    if (jr != JDR_OK)
    {
        return 3U;
    }

    /* 与训练对齐：先对 320x240 灰度图做全局直方图均衡化 */
    histogram_equalize_u8(s_gray_320x240, SRC_W, SRC_H);

    /* 与训练对齐：中心裁剪 224x224（不是整图缩放）+ 安全圆遮罩 */
    {
        uint32_t x_start = (SRC_W - DST_W) / 2U;
        uint32_t y_start = (SRC_H - DST_H) / 2U;

        for (y = 0U; y < DST_H; y++)
        {
            uint32_t src_y = y_start + y;
            uint32_t x;
            for (x = 0U; x < DST_W; x++)
            {
                uint32_t src_x = x_start + x;
                uint8_t g = s_gray_320x240[src_y * SRC_W + src_x];
                g = apply_circle_mask_u8(g, (int32_t)x, (int32_t)y);
                dst_input[y * DST_W + x] = (int8_t)((int16_t)g - 128);
            }
        }
    }

    return 0U;
}
