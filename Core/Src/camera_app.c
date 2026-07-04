#include "camera_app.h"
#include "dcmi.h"
#include "jpeg_stream.h"
#include "jpeg_decode.h"
#include "ov2640.h"
#include "ov2640_sccb.h"
#include "usart.h"
#include "waterlevel.h"
#include "waterlevel_data_params.h"
#include "test_image_input.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

extern DCMI_HandleTypeDef hdcmi;
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_dcmi;

static volatile uint8_t g_frame_done = 0U;
static volatile uint8_t g_frame_error = 0U;

#define CAMERA_APP_DEBUG  0U
#define CAMERA_JPEG_DIAG  0U
#define CAMERA_WIDTH      320U
#define CAMERA_HEIGHT     240U
#define CAMERA_RGB565_FRAME_BYTES   (CAMERA_WIDTH * CAMERA_HEIGHT * 2U)
#define CAMERA_AI_INPUT_SIZE        (224U * 224U)
#define CAMERA_AI_MASK_RADIUS       100
#define CAMERA_AI_MASK_CENTER       112
#define CAMERA_AI_DUMP_INPUT_ONCE   0U
#define CAMERA_AI_DUMP_ONLY         0U
#define CAMERA_AI_RGB565_SWAP_BYTES 1U
#define CAMERA_AI_CLAHE_GRID_X      8U
#define CAMERA_AI_CLAHE_GRID_Y      8U
#define CAMERA_AI_CLAHE_TILE_W      (CAMERA_WIDTH / CAMERA_AI_CLAHE_GRID_X)
#define CAMERA_AI_CLAHE_TILE_H      (CAMERA_HEIGHT / CAMERA_AI_CLAHE_GRID_Y)
#define CAMERA_AI_CLAHE_TILE_PIXELS (CAMERA_AI_CLAHE_TILE_W * CAMERA_AI_CLAHE_TILE_H)
#define CAMERA_AI_CLAHE_CLIP_LIMIT  2U
#define CAMERA_AI_PAD_SIZE          320U
#define CAMERA_AI_PAD_TOP           ((CAMERA_AI_PAD_SIZE - CAMERA_HEIGHT) / 2U)
#define CAMERA_AI_PAD_LEFT          ((CAMERA_AI_PAD_SIZE - CAMERA_WIDTH) / 2U)
#define CAMERA_AI_CROP_START_X      ((CAMERA_AI_PAD_SIZE - 224U) / 2U)
#define CAMERA_AI_CROP_START_Y      ((CAMERA_AI_PAD_SIZE - 224U) / 2U)
#define CAMERA_AI_ENABLE_BRIGHTNESS_NORM 0U
#define CAMERA_AI_TARGET_MEAN_GRAY  91U
#define CAMERA_AI_VERBOSE_LOG       0U
#define CAMERA_PROBE_RETRY_COUNT    3U

#define CAMERA_DMA_SETTLE_SPINS   8192U
#define CAMERA_DMA_STABLE_SPINS   64U

#define WATERLEVEL_IN_ZERO_POINT  (-128)
#define WATERLEVEL_OUT0_SCALE     (0.098395728f)
#define WATERLEVEL_OUT0_ZERO_POINT (-9)
#define WATERLEVEL_OUT1_SCALE     (0.003897347f)
#define WATERLEVEL_OUT1_ZERO_POINT (-128)

typedef struct
{
    uint8_t initialized;
    ai_handle network;
    ai_buffer *input;
    ai_buffer *output;
} camera_ai_context_t;

typedef struct
{
    uint8_t class_id;
    float level_reg;
    float confidence;
    int8_t logits[AI_WATERLEVEL_OUT_1_SIZE];
} camera_ai_result_t;

typedef struct
{
    uint32_t frame_count;
    uint8_t last_class_id;
    float last_level_reg;
    float last_confidence;
    uint8_t stop_request;
    uint8_t abnormal_latched;
} camera_pump_ctrl_state_t;

#if ((APP_MODE == APP_MODE_AI_INFER) || (APP_MODE == APP_MODE_AI_TEST_IMAGE) || (APP_MODE == APP_MODE_PUMP_CTRL))
static AI_ALIGNED(32) uint8_t g_frame_buf[CAMERA_RGB565_FRAME_BYTES];
static AI_ALIGNED(32) ai_u8 g_ai_activations[AI_WATERLEVEL_DATA_ACTIVATIONS_SIZE]
    __attribute__((section(".ai_ram_d1")));
static AI_ALIGNED(32) ai_i8 g_ai_input_data[AI_WATERLEVEL_IN_1_SIZE]
    __attribute__((section(".ai_dtcm")));
static AI_ALIGNED(32) ai_i8 g_ai_output_logits[AI_WATERLEVEL_OUT_1_SIZE]
    __attribute__((section(".ai_dtcm")));
static AI_ALIGNED(32) ai_i8 g_ai_output_reg[AI_WATERLEVEL_OUT_2_SIZE]
    __attribute__((section(".ai_dtcm")));
static uint8_t g_ai_clahe_lut[CAMERA_AI_CLAHE_GRID_Y][CAMERA_AI_CLAHE_GRID_X][256];
static uint8_t g_ai_gray_buf[CAMERA_AI_INPUT_SIZE];
static camera_ai_context_t g_ai_ctx;
#endif
static uint8_t g_camera_ready = 0U;
static camera_pump_ctrl_state_t g_pump_ctrl;

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

static void camera_app_log(const char *s)
{
    if (s != NULL)
    {
        (void)HAL_UART_Transmit(&huart1, (uint8_t *)s, (uint16_t)strlen(s), HAL_MAX_DELAY);
    }
}

static uint32_t camera_app_float_to_permille(float value)
{
    if (value <= 0.0f)
    {
        return 0U;
    }

    if (value >= 1.0f)
    {
        return 1000U;
    }

    return (uint32_t)((value * 1000.0f) + 0.5f);
}

static uint8_t camera_app_ai_input_gray(const ai_i8 *input, uint32_t idx)
{
    return (uint8_t)((int16_t)input[idx] - WATERLEVEL_IN_ZERO_POINT);
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

static uint8_t camera_app_capture_snapshot(uint32_t timeout_ms)
{
    uint32_t start_tick;

    g_frame_done = 0U;
    g_frame_error = 0U;

    if (OV2640_StartSnapshot() != OV2640_OK)
    {
        HAL_DCMI_Stop(&hdcmi);
        HAL_Delay(20U);
        return 1U;
    }

    start_tick = HAL_GetTick();
    while (g_frame_done == 0U)
    {
        if ((HAL_GetTick() - start_tick) > timeout_ms)
        {
            HAL_DCMI_Stop(&hdcmi);
            return 2U;
        }
    }

    camera_app_wait_dma_settle();
    HAL_DCMI_Stop(&hdcmi);

    return (g_frame_error != 0U) ? 3U : 0U;
}

static uint8_t camera_app_capture_jpeg_snapshot(uint32_t timeout_ms, uint32_t *jpeg_off, uint32_t *jpeg_len)
{
    uint32_t start_tick;

    if ((jpeg_off == NULL) || (jpeg_len == NULL))
    {
        return 1U;
    }

    *jpeg_off = 0U;
    *jpeg_len = 0U;
    g_frame_done = 0U;
    g_frame_error = 0U;

    if (OV2640_StartSnapshot() != OV2640_OK)
    {
        HAL_DCMI_Stop(&hdcmi);
        HAL_Delay(20U);
        return 1U;
    }

    start_tick = HAL_GetTick();
    while (g_frame_done == 0U)
    {
        if ((HAL_GetTick() - start_tick) > timeout_ms)
        {
            HAL_DCMI_Stop(&hdcmi);
            return 2U;
        }
    }

    camera_app_wait_dma_settle();
    HAL_DCMI_Stop(&hdcmi);

    if (g_frame_error != 0U)
    {
        return 3U;
    }

    *jpeg_len = JPEG_Stream_FindFrame(jpeg_off);
    if (*jpeg_len == 0U)
    {
        return 4U;
    }

    if ((*jpeg_off + *jpeg_len) > JPEG_Stream_GetMaxSize())
    {
        return 5U;
    }

    return 0U;
}

#if ((APP_MODE == APP_MODE_AI_INFER) || (APP_MODE == APP_MODE_AI_TEST_IMAGE) || (APP_MODE == APP_MODE_PUMP_CTRL))
static const char *camera_ai_class_name(uint8_t class_id)
{
    static const char *const k_names[5] = {
        "no_cup",
        "low",
        "half",
        "full",
        "abnormal"
    };

    if (class_id >= 5U)
    {
        return "unknown";
    }

    return k_names[class_id];
}

static uint8_t camera_app_ai_init(void)
{
    ai_handle activations[] = { g_ai_activations };
    ai_error err;

    if (g_ai_ctx.initialized != 0U)
    {
        return 0U;
    }

    err = ai_waterlevel_create_and_init(&g_ai_ctx.network, activations, NULL);
    if (err.type != AI_ERROR_NONE)
    {
        return 1U;
    }

    g_ai_ctx.input = ai_waterlevel_inputs_get(g_ai_ctx.network, NULL);
    g_ai_ctx.output = ai_waterlevel_outputs_get(g_ai_ctx.network, NULL);
    if ((g_ai_ctx.input == NULL) || (g_ai_ctx.output == NULL))
    {
        return 2U;
    }

    g_ai_ctx.input[0].data = AI_HANDLE_PTR(g_ai_input_data);
    g_ai_ctx.output[0].data = AI_HANDLE_PTR(g_ai_output_logits);
    g_ai_ctx.output[1].data = AI_HANDLE_PTR(g_ai_output_reg);
    g_ai_ctx.initialized = 1U;

    return 0U;
}

static uint8_t camera_app_rgb565_gray(uint16_t pixel)
{
#if (CAMERA_AI_RGB565_SWAP_BYTES != 0U)
    pixel = (uint16_t)((pixel << 8) | (pixel >> 8));
#endif
    uint32_t r = ((uint32_t)((pixel >> 11) & 0x1FU) * 255U) / 31U;
    uint32_t g = ((uint32_t)((pixel >> 5) & 0x3FU) * 255U) / 63U;
    uint32_t b = ((uint32_t)(pixel & 0x1FU) * 255U) / 31U;

    return (uint8_t)((r * 30U + g * 59U + b * 11U) / 100U);
}

static void camera_app_clahe_build_lut(const uint16_t *src_rgb565)
{
    uint32_t tile_y;

    for (tile_y = 0U; tile_y < CAMERA_AI_CLAHE_GRID_Y; tile_y++)
    {
        uint32_t tile_x;

        for (tile_x = 0U; tile_x < CAMERA_AI_CLAHE_GRID_X; tile_x++)
        {
            uint16_t hist[256] = {0};
            uint32_t y_start = tile_y * CAMERA_AI_CLAHE_TILE_H;
            uint32_t x_start = tile_x * CAMERA_AI_CLAHE_TILE_W;
            uint32_t y;
            uint32_t excess = 0U;
            uint32_t clip_limit = (CAMERA_AI_CLAHE_CLIP_LIMIT * CAMERA_AI_CLAHE_TILE_PIXELS) / 256U;
            uint32_t cdf = 0U;

            if (clip_limit == 0U)
            {
                clip_limit = 1U;
            }

            for (y = 0U; y < CAMERA_AI_CLAHE_TILE_H; y++)
            {
                uint32_t x;
                uint32_t src_row = (y_start + y) * CAMERA_WIDTH;

                for (x = 0U; x < CAMERA_AI_CLAHE_TILE_W; x++)
                {
                    uint8_t gray = camera_app_rgb565_gray(src_rgb565[src_row + x_start + x]);
                    hist[gray]++;
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
                    g_ai_clahe_lut[tile_y][tile_x][i] =
                        (uint8_t)((cdf * 255U + (CAMERA_AI_CLAHE_TILE_PIXELS / 2U)) / CAMERA_AI_CLAHE_TILE_PIXELS);
                }
            }
        }
    }
}

static uint8_t camera_app_clahe_apply_gray(const uint16_t *src_rgb565, uint32_t src_x, uint32_t src_y)
{
    uint8_t gray = camera_app_rgb565_gray(src_rgb565[(src_y * CAMERA_WIDTH) + src_x]);
    uint32_t tx0 = src_x / CAMERA_AI_CLAHE_TILE_W;
    uint32_t ty0 = src_y / CAMERA_AI_CLAHE_TILE_H;
    uint32_t tx1 = (tx0 + 1U < CAMERA_AI_CLAHE_GRID_X) ? (tx0 + 1U) : tx0;
    uint32_t ty1 = (ty0 + 1U < CAMERA_AI_CLAHE_GRID_Y) ? (ty0 + 1U) : ty0;
    uint32_t fx = src_x % CAMERA_AI_CLAHE_TILE_W;
    uint32_t fy = src_y % CAMERA_AI_CLAHE_TILE_H;
    uint32_t wx0 = CAMERA_AI_CLAHE_TILE_W - fx;
    uint32_t wx1 = fx;
    uint32_t wy0 = CAMERA_AI_CLAHE_TILE_H - fy;
    uint32_t wy1 = fy;
    uint32_t v00 = g_ai_clahe_lut[ty0][tx0][gray];
    uint32_t v01 = g_ai_clahe_lut[ty0][tx1][gray];
    uint32_t v10 = g_ai_clahe_lut[ty1][tx0][gray];
    uint32_t v11 = g_ai_clahe_lut[ty1][tx1][gray];
    uint32_t top = (v00 * wx0) + (v01 * wx1);
    uint32_t bottom = (v10 * wx0) + (v11 * wx1);
    uint32_t value = (top * wy0) + (bottom * wy1);

    return (uint8_t)((value + ((CAMERA_AI_CLAHE_TILE_W * CAMERA_AI_CLAHE_TILE_H) / 2U)) /
                     (CAMERA_AI_CLAHE_TILE_W * CAMERA_AI_CLAHE_TILE_H));
}

static void camera_app_preprocess_rgb565_to_ai(const uint16_t *src_rgb565, ai_i8 *dst_input)
{
    uint32_t y;
    uint32_t gray_sum = 0U;
    uint32_t gray_count = 0U;
    uint32_t norm_num = CAMERA_AI_TARGET_MEAN_GRAY;
    uint32_t norm_den = 1U;

    camera_app_clahe_build_lut(src_rgb565);

    for (y = 0U; y < 224U; y++)
    {
        uint32_t x;
        uint32_t square_y = y + CAMERA_AI_CROP_START_Y;

        for (x = 0U; x < 224U; x++)
        {
            uint32_t dx = (x > CAMERA_AI_MASK_CENTER) ? (x - CAMERA_AI_MASK_CENTER) : (CAMERA_AI_MASK_CENTER - x);
            uint32_t dy = (y > CAMERA_AI_MASK_CENTER) ? (y - CAMERA_AI_MASK_CENTER) : (CAMERA_AI_MASK_CENTER - y);
            uint32_t idx = (y * 224U) + x;
            uint32_t square_x = x + CAMERA_AI_CROP_START_X;

            if (((dx * dx) + (dy * dy)) > (CAMERA_AI_MASK_RADIUS * CAMERA_AI_MASK_RADIUS))
            {
                g_ai_gray_buf[idx] = 0U;
            }
            else if ((square_y < CAMERA_AI_PAD_TOP) ||
                     (square_y >= (CAMERA_AI_PAD_TOP + CAMERA_HEIGHT)) ||
                     (square_x < CAMERA_AI_PAD_LEFT) ||
                     (square_x >= (CAMERA_AI_PAD_LEFT + CAMERA_WIDTH)))
            {
                g_ai_gray_buf[idx] = 0U;
            }
            else
            {
                uint32_t src_x = square_x - CAMERA_AI_PAD_LEFT;
                uint32_t src_y = square_y - CAMERA_AI_PAD_TOP;
                uint8_t gray = camera_app_clahe_apply_gray(src_rgb565, src_x, src_y);
                g_ai_gray_buf[idx] = gray;
                if (gray != 0U)
                {
                    gray_sum += gray;
                    gray_count++;
                }
            }
        }
    }

    if ((CAMERA_AI_ENABLE_BRIGHTNESS_NORM != 0U) && (gray_count != 0U))
    {
        norm_den = gray_sum / gray_count;
        if (norm_den == 0U)
        {
            norm_den = 1U;
        }
    }

    for (y = 0U; y < 224U; y++)
    {
        uint32_t x;

        for (x = 0U; x < 224U; x++)
        {
            uint32_t idx = (y * 224U) + x;
            uint8_t gray = g_ai_gray_buf[idx];

            if (gray == 0U)
            {
                dst_input[idx] = (ai_i8)WATERLEVEL_IN_ZERO_POINT;
            }
            else
            {
                uint32_t scaled = gray;

                if (CAMERA_AI_ENABLE_BRIGHTNESS_NORM != 0U)
                {
                    scaled = ((uint32_t)gray * norm_num + (norm_den / 2U)) / norm_den;
                }
                if (scaled > 255U)
                {
                    scaled = 255U;
                }
                dst_input[idx] = (ai_i8)((int16_t)scaled + WATERLEVEL_IN_ZERO_POINT);
            }
        }
    }
}

#if (CAMERA_AI_DUMP_INPUT_ONCE != 0U)
static void camera_app_dump_ai_input_once(const ai_i8 *input)
{
    static uint8_t s_dump_done = 0U;
    uint32_t min_v = 255U;
    uint32_t max_v = 0U;
    uint32_t sum_v = 0U;
    char row_buf[480];
    char stat_buf[96];
    uint32_t y;

    if ((input == NULL) || (s_dump_done != 0U))
    {
        return;
    }

    s_dump_done = 1U;
    camera_app_log("[AI:DUMP] begin w=224 h=224 fmt=gray8_hex\r\n");

    for (y = 0U; y < 224U; y++)
    {
        uint32_t x;
        int pos;

        pos = snprintf(row_buf, sizeof(row_buf), "[AI:DUMP:ROW %03lu] ", (unsigned long)y);
        for (x = 0U; x < 224U; x++)
        {
            uint32_t idx = (y * 224U) + x;
            uint8_t gray = camera_app_ai_input_gray(input, idx);

            if (gray < min_v)
            {
                min_v = gray;
            }
            if (gray > max_v)
            {
                max_v = gray;
            }
            sum_v += gray;

            pos += snprintf(&row_buf[pos], sizeof(row_buf) - (uint32_t)pos, "%02X", gray);
        }
        row_buf[pos++] = '\r';
        row_buf[pos++] = '\n';
        row_buf[pos] = '\0';
        HAL_UART_Transmit(&huart1, (uint8_t *)row_buf, (uint16_t)pos, HAL_MAX_DELAY);
    }

    (void)snprintf(stat_buf, sizeof(stat_buf),
                   "[AI:DUMP] stats min=%lu max=%lu mean=%lu\r\n",
                   (unsigned long)min_v,
                   (unsigned long)max_v,
                   (unsigned long)(sum_v / CAMERA_AI_INPUT_SIZE));
    camera_app_log(stat_buf);
    camera_app_log("[AI:DUMP] end\r\n");
}
#endif

static uint8_t camera_app_ai_run(camera_ai_result_t *result)
{
    ai_i32 batches;
    uint32_t i;
    float best_logit = -1000.0f;
    float exp_sum = 0.0f;
    float best_prob = 0.0f;
    uint8_t best_idx = 0U;

    if (result == NULL)
    {
        return 1U;
    }

    batches = ai_waterlevel_run(g_ai_ctx.network, g_ai_ctx.input, g_ai_ctx.output);
    if (batches != 1)
    {
        return 2U;
    }

    memcpy(result->logits, g_ai_output_logits, sizeof(result->logits));
    result->level_reg = (((float)((int32_t)g_ai_output_reg[0] - WATERLEVEL_OUT1_ZERO_POINT)) * WATERLEVEL_OUT1_SCALE);
    if (result->level_reg < 0.0f)
    {
        result->level_reg = 0.0f;
    }
    else if (result->level_reg > 1.0f)
    {
        result->level_reg = 1.0f;
    }

    for (i = 0U; i < AI_WATERLEVEL_OUT_1_SIZE; i++)
    {
        float logit = ((float)((int32_t)g_ai_output_logits[i] - WATERLEVEL_OUT0_ZERO_POINT)) * WATERLEVEL_OUT0_SCALE;
        if ((i == 0U) || (logit > best_logit))
        {
            best_logit = logit;
            best_idx = (uint8_t)i;
        }
    }

    for (i = 0U; i < AI_WATERLEVEL_OUT_1_SIZE; i++)
    {
        float logit = ((float)((int32_t)g_ai_output_logits[i] - WATERLEVEL_OUT0_ZERO_POINT)) * WATERLEVEL_OUT0_SCALE;
        float e = expf(logit - best_logit);
        exp_sum += e;
        if (i == best_idx)
        {
            best_prob = e;
        }
    }

    result->class_id = best_idx;
    result->confidence = (exp_sum > 0.0f) ? (best_prob / exp_sum) : 0.0f;
    return 0U;
}

static void camera_app_ai_report(const camera_ai_result_t *result,
                                 uint32_t pipeline_ms,
                                 uint32_t infer_ms)
{
    char buf[192];
    int len;
    uint32_t conf_x10;
    uint32_t reg_x1000;
    uint32_t fps_x10;

    if (result == NULL)
    {
        return;
    }

    conf_x10 = camera_app_float_to_permille(result->confidence);
    reg_x1000 = camera_app_float_to_permille(result->level_reg);
    fps_x10 = (pipeline_ms > 0U) ? (10000U / pipeline_ms) : 0U;

    len = snprintf(buf, sizeof(buf),
                   "[AI] cls=%lu name=%s conf=%lu.%lu%% reg=0.%03lu pipe=%lums nn=%lums fps=%lu.%lu raw=[%d,%d,%d,%d,%d]\r\n",
                   (unsigned long)result->class_id,
                   camera_ai_class_name(result->class_id),
                   (unsigned long)(conf_x10 / 10U),
                   (unsigned long)(conf_x10 % 10U),
                   (unsigned long)reg_x1000,
                   (unsigned long)pipeline_ms,
                   (unsigned long)infer_ms,
                   (unsigned long)(fps_x10 / 10U),
                   (unsigned long)(fps_x10 % 10U),
                   (int)result->logits[0],
                   (int)result->logits[1],
                   (int)result->logits[2],
                   (int)result->logits[3],
                   (int)result->logits[4]);
    HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
}
#endif

#if (APP_MODE == APP_MODE_PUMP_CTRL)
static void camera_app_pump_ctrl_reset(void)
{
    memset(&g_pump_ctrl, 0, sizeof(g_pump_ctrl));
}

static void camera_app_pump_ctrl_consume_result(const camera_ai_result_t *result)
{
    if (result == NULL)
    {
        return;
    }

    g_pump_ctrl.frame_count++;
    g_pump_ctrl.last_class_id = result->class_id;
    g_pump_ctrl.last_level_reg = result->level_reg;
    g_pump_ctrl.last_confidence = result->confidence;
    g_pump_ctrl.stop_request = ((result->class_id == 3U) || (result->class_id == 4U)) ? 1U : 0U;

    if (result->class_id == 4U)
    {
        g_pump_ctrl.abnormal_latched = 1U;
    }

    /* Future pump-control integration point:
       1. add debounce / multi-frame state machine
       2. call user-provided pump start/stop interface
       3. add timeout / fault interlock handling */
}
#endif

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

#if (APP_MODE == APP_MODE_AI_INFER)
    camera_app_log("[APP] CameraApp_Init enter\r\n");
    camera_app_log("[APP] mode=AI_INFER\r\n");
#elif (APP_MODE == APP_MODE_AI_TEST_IMAGE)
    camera_app_log("[APP] CameraApp_Init enter\r\n");
    camera_app_log("[APP] mode=AI_TEST_IMAGE\r\n");
#elif (APP_MODE == APP_MODE_PUMP_CTRL)
#else
    camera_app_log("[APP] CameraApp_Init enter\r\n");
    camera_app_log("[APP] mode=XCAM_VIEW\r\n");
#endif

#if (CAMERA_JPEG_DIAG != 0U)
    {
        static const char msg[] = "[JPEG] diag mode on\r\n";
        HAL_UART_Transmit(&huart1, (uint8_t *)msg, (uint16_t)(sizeof(msg) - 1U), HAL_MAX_DELAY);
    }
#endif

    if (APP_MODE != APP_MODE_AI_TEST_IMAGE)
    {
        uint32_t probe_try;
        uint8_t probe_ok = 0U;

        for (probe_try = 0U; probe_try < CAMERA_PROBE_RETRY_COUNT; probe_try++)
        {
            if (OV2640_Probe(&mid, &pid) == OV2640_OK)
            {
                probe_ok = 1U;
                break;
            }

            HAL_Delay(50U);
        }

        if (probe_ok == 0U)
        {
            char buf[80];
            int len = snprintf(buf, sizeof(buf),
                               "[APP] probe fail mid=0x%04X pid=0x%04X\r\n",
                               mid, pid);
            HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
#if (CAMERA_JPEG_DIAG != 0U)
            {
                static const char msg[] = "[JPEG] probe fail\r\n";
                HAL_UART_Transmit(&huart1, (uint8_t *)msg, (uint16_t)(sizeof(msg) - 1U), HAL_MAX_DELAY);
            }
#endif
            dbg_print("[APP] Probe FAILED!\r\n");
            return;
        }

        {
            char buf[64];
            int len = snprintf(buf, sizeof(buf), "[APP] probe ok mid=0x%04X pid=0x%04X\r\n", mid, pid);
            HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
        }
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

    if (APP_MODE != APP_MODE_AI_TEST_IMAGE)
    {
        if (OV2640_Init() != OV2640_OK)
        {
            camera_app_log("[APP] ov init fail\r\n");
#if (CAMERA_JPEG_DIAG != 0U)
            {
                static const char msg[] = "[JPEG] init fail\r\n";
                HAL_UART_Transmit(&huart1, (uint8_t *)msg, (uint16_t)(sizeof(msg) - 1U), HAL_MAX_DELAY);
            }
#endif
            dbg_print("[APP] Init FAILED!\r\n");
            return;
        }
        camera_app_log("[APP] ov init ok\r\n");
    }

    if (APP_MODE != APP_MODE_AI_TEST_IMAGE)
    {
        if (OV2640_SetOutputFormatJPEG() != OV2640_OK)
        {
            camera_app_log("[APP] jpeg fmt fail\r\n");
#if (CAMERA_JPEG_DIAG != 0U)
            {
                static const char msg[] = "[JPEG] fmt fail\r\n";
                HAL_UART_Transmit(&huart1, (uint8_t *)msg, (uint16_t)(sizeof(msg) - 1U), HAL_MAX_DELAY);
            }
#endif
            dbg_print("[APP] JPEG format FAILED!\r\n");
            return;
        }
        camera_app_log("[APP] jpeg fmt ok\r\n");
    }

    if (APP_MODE != APP_MODE_AI_TEST_IMAGE)
    {
        if (OV2640_SetOutputSize(CAMERA_WIDTH, CAMERA_HEIGHT) != OV2640_OK)
        {
            camera_app_log("[APP] size fail\r\n");
#if (CAMERA_JPEG_DIAG != 0U)
            {
                static const char msg[] = "[JPEG] size fail\r\n";
                HAL_UART_Transmit(&huart1, (uint8_t *)msg, (uint16_t)(sizeof(msg) - 1U), HAL_MAX_DELAY);
            }
#endif
            dbg_print("[APP] Output size FAILED!\r\n");
            return;
        }
        camera_app_log("[APP] size ok\r\n");
    }

    if (APP_MODE != APP_MODE_AI_TEST_IMAGE)
    {
        JPEG_Stream_Init();
        OV2640_AttachFrameBuffer(JPEG_Stream_GetBuf(), JPEG_Stream_GetMaxSize());

        HAL_NVIC_SetPriority(DCMI_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(DCMI_IRQn);

        HAL_Delay(1500U);
    }

#if ((APP_MODE == APP_MODE_AI_INFER) || (APP_MODE == APP_MODE_AI_TEST_IMAGE) || (APP_MODE == APP_MODE_PUMP_CTRL))
#if (APP_MODE == APP_MODE_AI_INFER)
    if (OV2640_SetOutputFormatJPEG() != OV2640_OK)
    {
        camera_app_log("[APP] jpeg fmt fail\r\n");
        return;
    }
    camera_app_log("[APP] jpeg fmt ok\r\n");
#endif
    if (camera_app_ai_init() != 0U)
    {
#if (APP_MODE != APP_MODE_PUMP_CTRL)
        camera_app_log("[APP] ai init fail\r\n");
#endif
        return;
    }
#if (APP_MODE == APP_MODE_AI_INFER)
    camera_app_log("[APP] ai init ok\r\n");
#elif (APP_MODE == APP_MODE_PUMP_CTRL)
    camera_app_pump_ctrl_reset();
#endif
#endif

    g_camera_ready = 1U;
#if (APP_MODE != APP_MODE_PUMP_CTRL)
    camera_app_log("[APP] CameraApp_Init done\r\n");
#endif

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
    if (g_camera_ready == 0U)
    {
        HAL_Delay(100U);
        return;
    }

#if (APP_MODE == APP_MODE_XCAM_VIEW)
    uint32_t jpeg_len;
    uint32_t soi_off;

    if (camera_app_capture_snapshot(1000U) != 0U)
    {
#if (CAMERA_JPEG_DIAG != 0U)
        camera_app_dump_nojpeg_diag("start");
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
#elif (APP_MODE == APP_MODE_AI_INFER)
    {
        camera_ai_result_t result;
        uint32_t pipeline_start = HAL_GetTick();
        uint32_t infer_start;
        uint32_t infer_ms;
        uint32_t jpeg_off = 0U;
        uint32_t jpeg_len = 0U;
        uint8_t status;
        static uint8_t first_enter = 1U;

        if (first_enter != 0U)
        {
            camera_app_log("[AI] run enter\r\n");
            first_enter = 0U;
        }

#if (CAMERA_AI_VERBOSE_LOG != 0U)
        camera_app_log("[AI] capture start\r\n");
#endif
        status = camera_app_capture_jpeg_snapshot(3000U, &jpeg_off, &jpeg_len);

        if (status != 0U)
        {
            char buf[64];
            int len = snprintf(buf, sizeof(buf), "[AI] capture_err=%lu\r\n", (unsigned long)status);
            HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
            HAL_Delay(100U);
            return;
        }

#if (CAMERA_AI_VERBOSE_LOG != 0U)
        camera_app_log("[AI] capture ok\r\n");
#endif
        if (jpeg_to_ai_input(JPEG_Stream_GetBuf() + jpeg_off, jpeg_len, (int8_t *)g_ai_input_data) != 0U)
        {
            camera_app_log("[AI] decode fail\r\n");
            HAL_Delay(100U);
            return;
        }
#if (CAMERA_AI_VERBOSE_LOG != 0U)
        camera_app_log("[AI] prep ok\r\n");
#endif
#if (CAMERA_AI_DUMP_INPUT_ONCE != 0U)
        camera_app_dump_ai_input_once(g_ai_input_data);
#if (CAMERA_AI_DUMP_ONLY != 0U)
        camera_app_log("[AI:DUMP] hold\r\n");
        g_camera_ready = 0U;
        HAL_Delay(100U);
        return;
#endif
#endif

#if (CAMERA_AI_VERBOSE_LOG != 0U)
        camera_app_log("[AI] infer start\r\n");
#endif
        infer_start = HAL_GetTick();
        if (camera_app_ai_run(&result) != 0U)
        {
            static const char msg[] = "[AI] infer fail\r\n";
            HAL_UART_Transmit(&huart1, (uint8_t *)msg, (uint16_t)(sizeof(msg) - 1U), HAL_MAX_DELAY);
            HAL_Delay(100U);
            return;
        }
        infer_ms = HAL_GetTick() - infer_start;

#if (CAMERA_AI_VERBOSE_LOG != 0U)
        camera_app_log("[AI] infer ok\r\n");
#endif
        camera_app_ai_report(&result, HAL_GetTick() - pipeline_start, infer_ms);
    }
#elif (APP_MODE == APP_MODE_AI_TEST_IMAGE)
    {
        camera_ai_result_t result;
        uint32_t infer_start;
        uint32_t infer_ms;
        static uint8_t ran_once = 0U;

        if (ran_once != 0U)
        {
            HAL_Delay(100U);
            return;
        }
        ran_once = 1U;

        camera_app_log("[AI:TEST] feed fixed input\r\n");
        memcpy(g_ai_input_data, g_test_image_input_q7, sizeof(g_ai_input_data));
        camera_app_log("[AI:TEST] infer start\r\n");
        infer_start = HAL_GetTick();
        if (camera_app_ai_run(&result) != 0U)
        {
            camera_app_log("[AI:TEST] infer fail\r\n");
            g_camera_ready = 0U;
            return;
        }
        infer_ms = HAL_GetTick() - infer_start;
        camera_app_log("[AI:TEST] infer ok\r\n");
        camera_app_ai_report(&result, infer_ms, infer_ms);
        g_camera_ready = 0U;
    }
#elif (APP_MODE == APP_MODE_PUMP_CTRL)
    {
        camera_ai_result_t result;
        uint32_t jpeg_off = 0U;
        uint32_t jpeg_len = 0U;
        uint8_t status;

        status = camera_app_capture_jpeg_snapshot(3000U, &jpeg_off, &jpeg_len);
        if (status != 0U)
        {
            return;
        }

        if (jpeg_to_ai_input(JPEG_Stream_GetBuf() + jpeg_off, jpeg_len, (int8_t *)g_ai_input_data) != 0U)
        {
            return;
        }

        if (camera_app_ai_run(&result) != 0U)
        {
            return;
        }

        camera_app_pump_ctrl_consume_result(&result);
    }
#endif
}
