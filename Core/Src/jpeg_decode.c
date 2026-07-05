#include "jpeg_decode.h"
#include "tjpgd.h"
#include <string.h>

#define JPEG_WORKBUF_SIZE   (4096U)
#define SRC_W               (320U)
#define SRC_H               (240U)
#define PAD_SIZE            (320U)
#define DST_W               (224U)
#define DST_H               (224U)
#define SAFE_CIRCLE_RADIUS  (100)
#define CLAHE_GRID_X        (8U)
#define CLAHE_GRID_Y        (8U)
#define CLAHE_TILE_W        (SRC_W / CLAHE_GRID_X)
#define CLAHE_TILE_H        (SRC_H / CLAHE_GRID_Y)
#define CLAHE_TILE_PIXELS   (CLAHE_TILE_W * CLAHE_TILE_H)
#define CLAHE_CLIP_LIMIT    (2U)
#define PAD_TOP             ((PAD_SIZE - SRC_H) / 2U)
#define PAD_LEFT            ((PAD_SIZE - SRC_W) / 2U)
#define CROP_START_X        ((PAD_SIZE - DST_W) / 2U)
#define CROP_START_Y        ((PAD_SIZE - DST_H) / 2U)

typedef struct
{
    const uint8_t *jpg;
    uint32_t jpg_len;
    uint32_t jpg_pos;
} jpg_stream_t;

static uint8_t s_workbuf[JPEG_WORKBUF_SIZE];
static uint8_t s_gray_320x240[SRC_W * SRC_H] __attribute__((section(".ai_ram_d1")));
static uint8_t s_clahe_lut[CLAHE_GRID_Y][CLAHE_GRID_X][256] __attribute__((section(".ai_ram_d1")));
static uint8_t s_last_prepare_status = JDR_OK;
static uint8_t s_last_decomp_status = JDR_OK;
static uint16_t s_last_width = 0U;
static uint16_t s_last_height = 0U;

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

static void clahe_build_lut_u8(const uint8_t *img)
{
    uint32_t tile_y;

    if (img == NULL)
    {
        return;
    }

    for (tile_y = 0U; tile_y < CLAHE_GRID_Y; tile_y++)
    {
        uint32_t tile_x;

        for (tile_x = 0U; tile_x < CLAHE_GRID_X; tile_x++)
        {
            uint16_t hist[256] = {0};
            uint32_t y_start = tile_y * CLAHE_TILE_H;
            uint32_t x_start = tile_x * CLAHE_TILE_W;
            uint32_t y;
            uint32_t excess = 0U;
            uint32_t clip_limit = (CLAHE_CLIP_LIMIT * CLAHE_TILE_PIXELS) / 256U;
            uint32_t cdf = 0U;

            if (clip_limit == 0U)
            {
                clip_limit = 1U;
            }

            for (y = 0U; y < CLAHE_TILE_H; y++)
            {
                uint32_t x;
                uint32_t row = (y_start + y) * SRC_W;

                for (x = 0U; x < CLAHE_TILE_W; x++)
                {
                    hist[img[row + x_start + x]]++;
                }
            }

            {
                uint32_t i;

                for (i = 0U; i < 256U; i++)
                {
                    if (hist[i] > clip_limit)
                    {
                        excess += (uint32_t)hist[i] - clip_limit;
                        hist[i] = (uint16_t)clip_limit;
                    }
                }

                if (excess > 0U)
                {
                    uint32_t redist = excess / 256U;
                    uint32_t residual = excess % 256U;

                    for (i = 0U; i < 256U; i++)
                    {
                        hist[i] = (uint16_t)(hist[i] + redist);
                    }

                    for (i = 0U; i < residual; i++)
                    {
                        hist[i]++;
                    }
                }

                for (i = 0U; i < 256U; i++)
                {
                    cdf += hist[i];
                    s_clahe_lut[tile_y][tile_x][i] =
                        (uint8_t)((cdf * 255U + (CLAHE_TILE_PIXELS / 2U)) / CLAHE_TILE_PIXELS);
                }
            }
        }
    }
}

static uint8_t clahe_apply_u8(const uint8_t *img, uint32_t src_x, uint32_t src_y)
{
    uint8_t gray = img[(src_y * SRC_W) + src_x];
    uint32_t tx0 = src_x / CLAHE_TILE_W;
    uint32_t ty0 = src_y / CLAHE_TILE_H;
    uint32_t tx1 = (tx0 + 1U < CLAHE_GRID_X) ? (tx0 + 1U) : tx0;
    uint32_t ty1 = (ty0 + 1U < CLAHE_GRID_Y) ? (ty0 + 1U) : ty0;
    uint32_t fx = src_x % CLAHE_TILE_W;
    uint32_t fy = src_y % CLAHE_TILE_H;
    uint32_t wx0 = CLAHE_TILE_W - fx;
    uint32_t wx1 = fx;
    uint32_t wy0 = CLAHE_TILE_H - fy;
    uint32_t wy1 = fy;
    uint32_t v00 = s_clahe_lut[ty0][tx0][gray];
    uint32_t v01 = s_clahe_lut[ty0][tx1][gray];
    uint32_t v10 = s_clahe_lut[ty1][tx0][gray];
    uint32_t v11 = s_clahe_lut[ty1][tx1][gray];
    uint32_t top = (v00 * wx0) + (v01 * wx1);
    uint32_t bottom = (v10 * wx0) + (v11 * wx1);
    uint32_t value = (top * wy0) + (bottom * wy1);

    return (uint8_t)((value + ((CLAHE_TILE_W * CLAHE_TILE_H) / 2U)) /
                     (CLAHE_TILE_W * CLAHE_TILE_H));
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
    s_last_prepare_status = JDR_OK;
    s_last_decomp_status = JDR_OK;
    s_last_width = 0U;
    s_last_height = 0U;

    stream.jpg = jpg;
    stream.jpg_len = jpg_len;
    stream.jpg_pos = 0U;

    jr = jd_prepare(&jd, tjpgd_input, s_workbuf, sizeof(s_workbuf), &stream);
    s_last_prepare_status = (uint8_t)jr;
    if (jr != JDR_OK)
    {
        return 2U;
    }

    s_last_width = (uint16_t)jd.width;
    s_last_height = (uint16_t)jd.height;
    jr = jd_decomp(&jd, tjpgd_output, 0);
    s_last_decomp_status = (uint8_t)jr;
    if (jr != JDR_OK)
    {
        return 3U;
    }

    /* Match training preprocess_v3.py: CLAHE on 320x240, then pad->crop->circle mask. */
    clahe_build_lut_u8(s_gray_320x240);

    for (y = 0U; y < DST_H; y++)
    {
        uint32_t x;
        uint32_t square_y = y + CROP_START_Y;

        for (x = 0U; x < DST_W; x++)
        {
            uint32_t square_x = x + CROP_START_X;
            uint8_t g = 0U;

            if ((square_y >= PAD_TOP) &&
                (square_y < (PAD_TOP + SRC_H)) &&
                (square_x >= PAD_LEFT) &&
                (square_x < (PAD_LEFT + SRC_W)))
            {
                uint32_t src_x = square_x - PAD_LEFT;
                uint32_t src_y = square_y - PAD_TOP;
                g = clahe_apply_u8(s_gray_320x240, src_x, src_y);
            }

            g = apply_circle_mask_u8(g, (int32_t)x, (int32_t)y);
            dst_input[y * DST_W + x] = (int8_t)((int16_t)g - 128);
        }
    }

    return 0U;
}

uint8_t jpeg_decode_last_prepare_status(void)
{
    return s_last_prepare_status;
}

uint8_t jpeg_decode_last_decomp_status(void)
{
    return s_last_decomp_status;
}

uint16_t jpeg_decode_last_width(void)
{
    return s_last_width;
}

uint16_t jpeg_decode_last_height(void)
{
    return s_last_height;
}
