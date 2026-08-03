#include "face_ai.h"
#include "jpeg_decode.h"
#include "usart.h"
#include "yunet.h"
#include "yunet_data_params.h"
#include "sface.h"
#include "sface_data_params.h"
#include "quadspi.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern UART_HandleTypeDef huart1;

#define FACE_AI_IMG_W              320U
#define FACE_AI_IMG_H              240U
#define FACE_AI_DET_SIZE           320U
#define FACE_AI_ID_SIZE            112U
#define FACE_AI_PAD_TOP            ((FACE_AI_DET_SIZE - FACE_AI_IMG_H) / 2U)
#define FACE_AI_ACT_BASE           0xC0000000UL
#define FACE_AI_RGB_BASE           0xC0400000UL
#define FACE_AI_RGB_BYTES          (FACE_AI_IMG_W * FACE_AI_IMG_H * 3U)
#define FACE_AI_DET_WEIGHT_QSPI    0x90000000UL
#define FACE_AI_ID_WEIGHT_QSPI     0x90400000UL
#define FACE_AI_DET_WEIGHT_RAM     0xC0800000UL
#define FACE_AI_ID_WEIGHT_RAM      0xC0B00000UL
#define FACE_AI_DET_MIN_SCORE      0.55f
#define FACE_AI_MATCH_THRESHOLD    0.363f
#define FACE_AI_DB_BASE            0xC1800000UL
#define FACE_AI_DB_FLASH_OFFSET    0x01000000UL
#define FACE_AI_DB_FLASH_SIZE      0x00010000UL
#define FACE_AI_DB_MAGIC           0x31454346UL
#define FACE_AI_DB_VERSION         1U
#define FACE_AI_DB_RECORD_BYTES    (4U + 2U + 2U + 4U + 4U + (FACE_AI_EMBEDDING_SIZE * 4U) + 4U)

#define FACE_AI_SDRAM_BASE         0xC0000000UL
#define FACE_AI_SDRAM_BYTES        (32UL * 1024UL * 1024UL)
#define FACE_AI_ACT_BYTES          ((AI_YUNET_DATA_ACTIVATIONS_SIZE > AI_SFACE_DATA_ACTIVATIONS_SIZE) ? \
                                    AI_YUNET_DATA_ACTIVATIONS_SIZE : AI_SFACE_DATA_ACTIVATIONS_SIZE)

typedef struct __attribute__((packed))
{
    uint32_t magic;
    uint16_t version;
    uint16_t user_id;
    uint32_t embedding_bytes;
    float norm;
    float embedding[FACE_AI_EMBEDDING_SIZE];
    uint32_t crc;
} face_ai_enrollment_record_t;

#if (AI_SFACE_OUT_1_SIZE != FACE_AI_EMBEDDING_SIZE)
#error "SFace output size does not match the face enrollment database"
#endif

#if ((FACE_AI_ACT_BASE < FACE_AI_SDRAM_BASE) || \
     ((FACE_AI_ACT_BASE + FACE_AI_ACT_BYTES) > FACE_AI_RGB_BASE) || \
     ((FACE_AI_RGB_BASE + FACE_AI_RGB_BYTES) > FACE_AI_DET_WEIGHT_RAM) || \
     ((FACE_AI_DET_WEIGHT_RAM + AI_YUNET_DATA_WEIGHTS_SIZE) > FACE_AI_ID_WEIGHT_RAM) || \
     ((FACE_AI_ID_WEIGHT_RAM + AI_SFACE_DATA_WEIGHTS_SIZE) > (FACE_AI_SDRAM_BASE + FACE_AI_SDRAM_BYTES)))
#error "Face AI SDRAM regions overlap or exceed the configured 32MB SDRAM"
#endif

#if ((FACE_AI_DB_BASE < FACE_AI_SDRAM_BASE) || \
     ((FACE_AI_DB_BASE + FACE_AI_DB_RECORD_BYTES) > (FACE_AI_SDRAM_BASE + FACE_AI_SDRAM_BYTES)) || \
     ((FACE_AI_ID_WEIGHT_RAM + AI_SFACE_DATA_WEIGHTS_SIZE) > FACE_AI_DB_BASE))
#error "Face enrollment database overlaps the face AI SDRAM regions"
#endif

static face_ai_enrollment_record_t *const s_face_db =
    (face_ai_enrollment_record_t *)FACE_AI_DB_BASE;

typedef struct
{
    float score;
    float x1;
    float y1;
    float x2;
    float y2;
    float landmark[5][2];
    uint8_t valid;
} face_ai_detection_t;

typedef struct
{
    float scale;
    int32_t zero_point;
} face_ai_quant_t;

static const face_ai_quant_t s_cls_quant[3] = {
    {0.003292752f, -128},
    {0.003212299f, -128},
    {0.002707064f, -128}
};

static const face_ai_quant_t s_obj_quant[3] = {
    {0.003796757f, -128},
    {0.003871595f, -128},
    {0.000669989f, -128}
};

static const face_ai_quant_t s_bbox_quant[3] = {
    {0.011400674f, -61},
    {0.011022569f, -46},
    {0.012532321f, -16}
};

static const face_ai_quant_t s_landmark_quant[3] = {
    {0.019418273f, -26},
    {0.015351823f, -33},
    {0.016416648f, -45}
};

static const float s_sface_template[5][2] = {
    {38.2946f, 51.6963f},
    {73.5318f, 51.5014f},
    {56.0252f, 71.7366f},
    {41.5493f, 92.3655f},
    {70.7299f, 92.2041f}
};

static void face_ai_log(const char *text)
{
    if (text != NULL)
    {
        (void)HAL_UART_Transmit(&huart1, (uint8_t *)text, (uint16_t)strlen(text), HAL_MAX_DELAY);
    }
}

static uint32_t face_ai_crc32(const uint8_t *data, uint32_t size)
{
    uint32_t crc = 0xFFFFFFFFUL;
    uint32_t i;

    for (i = 0U; i < size; i++)
    {
        uint32_t bit;

        crc ^= data[i];
        for (bit = 0U; bit < 8U; bit++)
        {
            crc = ((crc & 1U) != 0U) ? ((crc >> 1U) ^ 0xEDB88320UL) : (crc >> 1U);
        }
    }
    return crc ^ 0xFFFFFFFFUL;
}

static uint8_t face_ai_db_is_valid(const face_ai_enrollment_record_t *record)
{
    uint32_t crc;

    if ((record == NULL) ||
        (record->magic != FACE_AI_DB_MAGIC) ||
        (record->version != FACE_AI_DB_VERSION) ||
        (record->user_id == 0U) ||
        (record->embedding_bytes != sizeof(record->embedding)) ||
        (record->norm <= 1.0e-6f))
    {
        return 0U;
    }

    crc = face_ai_crc32((const uint8_t *)record, (uint32_t)offsetof(face_ai_enrollment_record_t, crc));
    return (crc == record->crc) ? 1U : 0U;
}

static void face_ai_db_clear_ram(void)
{
    memset((void *)s_face_db, 0, sizeof(*s_face_db));
    SCB_CleanDCache_by_Addr((uint32_t *)s_face_db, (int32_t)sizeof(*s_face_db));
}

static int32_t face_ai_float_milli(float value)
{
    return (value >= 0.0f) ? (int32_t)((value * 1000.0f) + 0.5f)
                           : (int32_t)((value * 1000.0f) - 0.5f);
}

static float face_ai_clampf(float value, float lo, float hi)
{
    if (value < lo)
    {
        return lo;
    }
    if (value > hi)
    {
        return hi;
    }
    return value;
}

static float face_ai_dequant_s8(int8_t value, const face_ai_quant_t *quant)
{
    return ((float)((int32_t)value - quant->zero_point)) * quant->scale;
}

static void face_ai_fill_yunet_input(int8_t *input, const uint8_t *rgb)
{
    uint32_t plane = FACE_AI_DET_SIZE * FACE_AI_DET_SIZE;
    uint32_t y;

    memset(input, 0x80, AI_YUNET_IN_1_SIZE_BYTES);
    for (y = 0U; y < FACE_AI_IMG_H; y++)
    {
        uint32_t x;
        uint32_t dst_y = y + FACE_AI_PAD_TOP;

        for (x = 0U; x < FACE_AI_IMG_W; x++)
        {
            const uint8_t *pixel = &rgb[((y * FACE_AI_IMG_W) + x) * 3U];
            uint32_t dst = (dst_y * FACE_AI_DET_SIZE) + x;

            input[dst] = (int8_t)((int32_t)pixel[0] - 128);
            input[plane + dst] = (int8_t)((int32_t)pixel[1] - 128);
            input[(2U * plane) + dst] = (int8_t)((int32_t)pixel[2] - 128);
        }
    }
}

static void face_ai_decode_yunet_head(ai_buffer *outputs,
                                      uint32_t head,
                                      uint32_t anchors,
                                      uint32_t feature_w,
                                      uint32_t stride,
                                      face_ai_detection_t *best)
{
    const int8_t *cls = (const int8_t *)outputs[head].data;
    const int8_t *obj = (const int8_t *)outputs[head + 3U].data;
    const int8_t *bbox = (const int8_t *)outputs[head + 6U].data;
    const int8_t *landmark = (const int8_t *)outputs[head + 9U].data;
    uint32_t i;

    for (i = 0U; i < anchors; i++)
    {
        float cls_score = face_ai_dequant_s8(cls[i], &s_cls_quant[head]);
        float obj_score = face_ai_dequant_s8(obj[i], &s_obj_quant[head]);
        float score = cls_score * obj_score;

        if (score > best->score)
        {
            float anchor_x = ((float)(i % feature_w) + 0.5f) * (float)stride;
            float anchor_y = ((float)(i / feature_w) + 0.5f) * (float)stride;
            float center_x = face_ai_dequant_s8(bbox[(i * 4U) + 0U], &s_bbox_quant[head]);
            float center_y = face_ai_dequant_s8(bbox[(i * 4U) + 1U], &s_bbox_quant[head]);
            float width_log = face_ai_dequant_s8(bbox[(i * 4U) + 2U], &s_bbox_quant[head]);
            float height_log = face_ai_dequant_s8(bbox[(i * 4U) + 3U], &s_bbox_quant[head]);
            float width;
            float height;
            uint32_t point;

            center_x = (center_x * (float)stride) + anchor_x;
            center_y = (center_y * (float)stride) + anchor_y - (float)FACE_AI_PAD_TOP;
            width = expf(face_ai_clampf(width_log, -8.0f, 8.0f)) * (float)stride;
            height = expf(face_ai_clampf(height_log, -8.0f, 8.0f)) * (float)stride;

            best->score = score;
            best->x1 = face_ai_clampf(center_x - (width * 0.5f), 0.0f, (float)(FACE_AI_IMG_W - 1U));
            best->y1 = face_ai_clampf(center_y - (height * 0.5f), 0.0f, (float)(FACE_AI_IMG_H - 1U));
            best->x2 = face_ai_clampf(center_x + (width * 0.5f), 0.0f, (float)(FACE_AI_IMG_W - 1U));
            best->y2 = face_ai_clampf(center_y + (height * 0.5f), 0.0f, (float)(FACE_AI_IMG_H - 1U));

            for (point = 0U; point < 5U; point++)
            {
                float point_x = face_ai_dequant_s8(landmark[(i * 10U) + (point * 2U)],
                                                   &s_landmark_quant[head]);
                float point_y = face_ai_dequant_s8(landmark[(i * 10U) + (point * 2U) + 1U],
                                                   &s_landmark_quant[head]);
                best->landmark[point][0] = (point_x * (float)stride) + anchor_x;
                best->landmark[point][1] = (point_y * (float)stride) + anchor_y - (float)FACE_AI_PAD_TOP;
            }
        }
    }
}

static face_ai_detection_t face_ai_best_detection(ai_buffer *outputs)
{
    face_ai_detection_t best;

    memset(&best, 0, sizeof(best));
    best.score = -1.0f;
    face_ai_decode_yunet_head(outputs, 0U, 1600U, 40U, 8U, &best);
    face_ai_decode_yunet_head(outputs, 1U, 400U, 20U, 16U, &best);
    face_ai_decode_yunet_head(outputs, 2U, 100U, 10U, 32U, &best);
    best.valid = ((best.score >= FACE_AI_DET_MIN_SCORE) &&
                  (best.x2 > best.x1) && (best.y2 > best.y1)) ? 1U : 0U;
    return best;
}

static uint8_t face_ai_run_yunet(const uint8_t *rgb,
                                 face_ai_detection_t *detection,
                                 uint32_t *infer_ms)
{
    ai_handle network = AI_HANDLE_NULL;
    ai_handle activations[AI_YUNET_DATA_ACTIVATIONS_COUNT] = {AI_HANDLE_PTR(FACE_AI_ACT_BASE)};
    ai_error err;
    ai_buffer *input;
    ai_buffer *output;
    ai_i32 batch;
    uint32_t start_ms;

    face_ai_log("[FACE_AI] yunet create begin\r\n");
    err = ai_yunet_create_and_init(&network, activations, NULL);
    if (err.type != AI_ERROR_NONE)
    {
        char msg[96];
        int len = snprintf(msg, sizeof(msg), "[FACE_AI] yunet init err type=%d code=%d\r\n",
                           err.type, err.code);
        if (len > 0)
        {
            face_ai_log(msg);
        }
        return 1U;
    }

    input = ai_yunet_inputs_get(network, NULL);
    output = ai_yunet_outputs_get(network, NULL);
    if ((input == NULL) || (output == NULL) || (input[0].data == NULL))
    {
        (void)ai_yunet_destroy(network);
        return 2U;
    }

    face_ai_fill_yunet_input((int8_t *)input[0].data, rgb);
    SCB_CleanInvalidateDCache();
    face_ai_log("[FACE_AI] yunet run begin\r\n");
    start_ms = HAL_GetTick();
    batch = ai_yunet_run(network, input, output);
    if (infer_ms != NULL)
    {
        *infer_ms = HAL_GetTick() - start_ms;
    }
    face_ai_log("[FACE_AI] yunet run end\r\n");
    SCB_CleanInvalidateDCache();

    if (batch != 1)
    {
        err = ai_yunet_get_error(network);
        (void)ai_yunet_destroy(network);
        {
            char msg[96];
            int len = snprintf(msg, sizeof(msg), "[FACE_AI] yunet run err batch=%ld type=%d code=%d\r\n",
                               (long)batch, err.type, err.code);
            if (len > 0)
            {
                face_ai_log(msg);
            }
        }
        return 3U;
    }

    *detection = face_ai_best_detection(output);
    (void)ai_yunet_destroy(network);
    return 0U;
}

static float face_ai_bilinear_rgb(const uint8_t *rgb, float x, float y, uint32_t channel)
{
    uint32_t x0;
    uint32_t y0;
    uint32_t x1;
    uint32_t y1;
    float fx;
    float fy;
    float v00;
    float v01;
    float v10;
    float v11;

    x = face_ai_clampf(x, 0.0f, (float)(FACE_AI_IMG_W - 1U));
    y = face_ai_clampf(y, 0.0f, (float)(FACE_AI_IMG_H - 1U));
    x0 = (uint32_t)x;
    y0 = (uint32_t)y;
    x1 = (x0 + 1U < FACE_AI_IMG_W) ? (x0 + 1U) : x0;
    y1 = (y0 + 1U < FACE_AI_IMG_H) ? (y0 + 1U) : y0;
    fx = x - (float)x0;
    fy = y - (float)y0;

    v00 = (float)rgb[((y0 * FACE_AI_IMG_W + x0) * 3U) + channel];
    v01 = (float)rgb[((y0 * FACE_AI_IMG_W + x1) * 3U) + channel];
    v10 = (float)rgb[((y1 * FACE_AI_IMG_W + x0) * 3U) + channel];
    v11 = (float)rgb[((y1 * FACE_AI_IMG_W + x1) * 3U) + channel];
    return ((1.0f - fy) * (((1.0f - fx) * v00) + (fx * v01))) +
           (fy * (((1.0f - fx) * v10) + (fx * v11)));
}

static uint8_t face_ai_fill_sface_input(float *input,
                                        const uint8_t *rgb,
                                        const face_ai_detection_t *detection)
{
    float template_center_x = 0.0f;
    float template_center_y = 0.0f;
    float source_center_x = 0.0f;
    float source_center_y = 0.0f;
    float numerator_a = 0.0f;
    float numerator_b = 0.0f;
    float denominator = 0.0f;
    float a;
    float b;
    float tx;
    float ty;
    uint32_t point;
    uint32_t y;

    if ((input == NULL) || (rgb == NULL) || (detection == NULL) || (detection->valid == 0U))
    {
        return 1U;
    }

    for (point = 0U; point < 5U; point++)
    {
        template_center_x += s_sface_template[point][0];
        template_center_y += s_sface_template[point][1];
        source_center_x += detection->landmark[point][0];
        source_center_y += detection->landmark[point][1];
    }
    template_center_x *= 0.2f;
    template_center_y *= 0.2f;
    source_center_x *= 0.2f;
    source_center_y *= 0.2f;

    for (point = 0U; point < 5U; point++)
    {
        float dx = s_sface_template[point][0] - template_center_x;
        float dy = s_sface_template[point][1] - template_center_y;
        float sx = detection->landmark[point][0] - source_center_x;
        float sy = detection->landmark[point][1] - source_center_y;

        numerator_a += (dx * sx) + (dy * sy);
        numerator_b += (dx * sy) - (dy * sx);
        denominator += (dx * dx) + (dy * dy);
    }
    if (denominator < 1.0e-6f)
    {
        return 2U;
    }

    a = numerator_a / denominator;
    b = numerator_b / denominator;
    tx = source_center_x - (a * template_center_x) + (b * template_center_y);
    ty = source_center_y - (b * template_center_x) - (a * template_center_y);

    for (y = 0U; y < FACE_AI_ID_SIZE; y++)
    {
        uint32_t x;

        for (x = 0U; x < FACE_AI_ID_SIZE; x++)
        {
            float source_x = (a * (float)x) - (b * (float)y) + tx;
            float source_y = (b * (float)x) + (a * (float)y) + ty;
            uint32_t dst = (y * FACE_AI_ID_SIZE) + x;
            uint32_t plane = FACE_AI_ID_SIZE * FACE_AI_ID_SIZE;

            input[dst] = face_ai_bilinear_rgb(rgb, source_x, source_y, 0U);
            input[plane + dst] = face_ai_bilinear_rgb(rgb, source_x, source_y, 1U);
            input[(2U * plane) + dst] = face_ai_bilinear_rgb(rgb, source_x, source_y, 2U);
        }
    }

    return 0U;
}

static uint8_t face_ai_run_sface(const uint8_t *rgb,
                                 const face_ai_detection_t *detection,
                                 float *sum_sq,
                                 float first4[4],
                                 float embedding[FACE_AI_EMBEDDING_SIZE],
                                 uint32_t *infer_ms)
{
    ai_handle network = AI_HANDLE_NULL;
    ai_handle activations[AI_SFACE_DATA_ACTIVATIONS_COUNT] = {AI_HANDLE_PTR(FACE_AI_ACT_BASE)};
    ai_error err;
    ai_buffer *input;
    ai_buffer *output;
    ai_i32 batch;
    uint32_t start_ms;
    uint32_t i;
    uint8_t align_status;

    face_ai_log("[FACE_AI] sface create begin\r\n");
    err = ai_sface_create_and_init(&network, activations, NULL);
    if (err.type != AI_ERROR_NONE)
    {
        char msg[96];
        int len = snprintf(msg, sizeof(msg), "[FACE_AI] sface init err type=%d code=%d\r\n",
                           err.type, err.code);
        if (len > 0)
        {
            face_ai_log(msg);
        }
        return 1U;
    }

    input = ai_sface_inputs_get(network, NULL);
    output = ai_sface_outputs_get(network, NULL);
    if ((input == NULL) || (output == NULL) || (input[0].data == NULL))
    {
        (void)ai_sface_destroy(network);
        return 2U;
    }

    align_status = face_ai_fill_sface_input((float *)input[0].data, rgb, detection);
    if (align_status != 0U)
    {
        (void)ai_sface_destroy(network);
        return (uint8_t)(3U + align_status);
    }

    SCB_CleanInvalidateDCache();
    face_ai_log("[FACE_AI] sface run begin\r\n");
    start_ms = HAL_GetTick();
    batch = ai_sface_run(network, input, output);
    if (infer_ms != NULL)
    {
        *infer_ms = HAL_GetTick() - start_ms;
    }
    face_ai_log("[FACE_AI] sface run end\r\n");
    SCB_CleanInvalidateDCache();

    if (batch != 1)
    {
        err = ai_sface_get_error(network);
        (void)ai_sface_destroy(network);
        {
            char msg[96];
            int len = snprintf(msg, sizeof(msg), "[FACE_AI] sface run err batch=%ld type=%d code=%d\r\n",
                               (long)batch, err.type, err.code);
            if (len > 0)
            {
                face_ai_log(msg);
            }
        }
        return 3U;
    }

    *sum_sq = 0.0f;
    for (i = 0U; i < AI_SFACE_OUT_1_SIZE; i++)
    {
        float value = ((float *)output[0].data)[i];

        if (i < 4U)
        {
            first4[i] = value;
        }
        embedding[i] = value;
        *sum_sq += value * value;
    }

    (void)ai_sface_destroy(network);
    return 0U;
}

uint8_t FaceAI_RunJpeg(const uint8_t *jpg, uint32_t jpg_len, face_ai_result_t *result)
{
    uint8_t *rgb = (uint8_t *)FACE_AI_RGB_BASE;
    face_ai_detection_t detection;
    face_ai_result_t local_result;
    float sum_sq = 0.0f;
    float first4[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    uint32_t t0 = HAL_GetTick();
    uint32_t det_ms = 0U;
    uint32_t id_ms = 0U;
    uint8_t status;
    uint8_t recognize_status = 1U;
    char msg[256];
    int len;

    if (result == NULL)
    {
        result = &local_result;
    }
    memset(result, 0, sizeof(*result));

    face_ai_log("[FACE_AI] decode begin\r\n");
    status = jpeg_to_rgb888(jpg, jpg_len, rgb, FACE_AI_IMG_W, FACE_AI_IMG_H);
    if (status != 0U)
    {
        len = snprintf(msg, sizeof(msg),
                       "[FACE_AI] decode fail status=%u prep=%u decomp=%u wh=%ux%u jpeg=%lu\r\n",
                       (unsigned)status,
                       (unsigned)jpeg_decode_last_prepare_status(),
                       (unsigned)jpeg_decode_last_decomp_status(),
                       (unsigned)jpeg_decode_last_width(),
                       (unsigned)jpeg_decode_last_height(),
                       (unsigned long)jpg_len);
        if (len > 0)
        {
            face_ai_log(msg);
        }
        result->status = 1U;
        return 1U;
    }

    SCB_CleanInvalidateDCache_by_Addr((uint32_t *)rgb, (int32_t)FACE_AI_RGB_BYTES);
    face_ai_log("[FACE_AI] decode ok\r\n");

    status = face_ai_run_yunet(rgb, &detection, &det_ms);
    if (status != 0U)
    {
        result->status = (uint8_t)(10U + status);
        return (uint8_t)(10U + status);
    }

    result->score = detection.score;
    result->detection_valid = detection.valid;
    result->x1 = (int16_t)detection.x1;
    result->y1 = (int16_t)detection.y1;
    result->x2 = (int16_t)detection.x2;
    result->y2 = (int16_t)detection.y2;
    result->detection_ms = det_ms;
    result->total_ms = HAL_GetTick() - t0;

    len = snprintf(msg, sizeof(msg),
                   "[FACE_AI] yunet score=%ld.%03ld valid=%u box=%ld,%ld,%ld,%ld det_ms=%lu\r\n",
                   (long)(face_ai_float_milli(detection.score) / 1000),
                   labs(face_ai_float_milli(detection.score) % 1000),
                   (unsigned)detection.valid,
                   (long)detection.x1, (long)detection.y1,
                   (long)detection.x2, (long)detection.y2,
                   (unsigned long)det_ms);
    if (len > 0)
    {
        face_ai_log(msg);
    }

    if (detection.valid == 0U)
    {
        face_ai_log("[FACE_AI] no face; sface skipped\r\n");
        return 0U;
    }

    status = face_ai_run_sface(rgb,
                               &detection,
                               &sum_sq,
                               first4,
                               result->embedding,
                               &id_ms);
    if (status != 0U)
    {
        result->status = (uint8_t)(20U + status);
        return (uint8_t)(20U + status);
    }

    result->embedding_valid = 1U;
    result->identity_ms = id_ms;
    result->embedding_norm = sqrtf(sum_sq);
    result->total_ms = HAL_GetTick() - t0;

    {
        uint16_t matched_id = 0U;
        float similarity = 0.0f;
        recognize_status = FaceAI_Recognize(result, &matched_id, &similarity);

        result->reference_ready = (recognize_status != 1U) ? 1U : 0U;
        result->match_valid = (recognize_status == 0U) ? 1U : 0U;
        result->matched_id = matched_id;
        result->similarity = similarity;
    }

    len = snprintf(msg, sizeof(msg),
                   "[FACE_AI] sface id_ms=%lu emb_norm=%ld.%03ld e0=%ld e1=%ld e2=%ld e3=%ld total_ms=%lu\r\n",
                   (unsigned long)id_ms,
                   (long)(face_ai_float_milli(sqrtf(sum_sq)) / 1000),
                   labs(face_ai_float_milli(sqrtf(sum_sq)) % 1000),
                   (long)face_ai_float_milli(first4[0]),
                   (long)face_ai_float_milli(first4[1]),
                   (long)face_ai_float_milli(first4[2]),
                   (long)face_ai_float_milli(first4[3]),
                   (unsigned long)(HAL_GetTick() - t0));
    if (len > 0)
    {
        face_ai_log(msg);
    }

    {
        int32_t similarity_milli = face_ai_float_milli(result->similarity);
        len = snprintf(msg, sizeof(msg),
                       "[FACE_AI] identity status=%u ref=%u match=%u id=%u similarity=%ld.%03ld threshold=0.363\r\n",
                       (unsigned)recognize_status,
                       (unsigned)result->reference_ready,
                       (unsigned)result->match_valid,
                       (unsigned)result->matched_id,
                       (long)(similarity_milli / 1000),
                       labs(similarity_milli % 1000));
        if (len > 0)
        {
            face_ai_log(msg);
        }
    }

    return 0U;
}

uint8_t FaceAI_RunJpegDiag(const uint8_t *jpg, uint32_t jpg_len)
{
    return FaceAI_RunJpeg(jpg, jpg_len, NULL);
}

uint8_t FaceAI_LoadWeights(void)
{
    const uint8_t *det_src = (const uint8_t *)FACE_AI_DET_WEIGHT_QSPI;
    const uint8_t *id_src = (const uint8_t *)FACE_AI_ID_WEIGHT_QSPI;
    uint8_t *det_dst = (uint8_t *)FACE_AI_DET_WEIGHT_RAM;
    uint8_t *id_dst = (uint8_t *)FACE_AI_ID_WEIGHT_RAM;
    uint32_t det_bytes = AI_YUNET_DATA_WEIGHTS_SIZE;
    uint32_t id_bytes = AI_SFACE_DATA_WEIGHTS_SIZE;
    uint32_t det_cache_bytes = (det_bytes + 31U) & ~31U;
    uint32_t id_cache_bytes = (id_bytes + 31U) & ~31U;
    char msg[128];
    int len;

    len = snprintf(msg, sizeof(msg), "[FACE_AI] weights load begin yunet=%lu sface=%lu\r\n",
                   (unsigned long)det_bytes, (unsigned long)id_bytes);
    if (len > 0)
    {
        face_ai_log(msg);
    }

    memcpy(det_dst, det_src, det_bytes);
    memcpy(id_dst, id_src, id_bytes);
    SCB_CleanDCache_by_Addr((uint32_t *)det_dst, (int32_t)det_cache_bytes);
    SCB_CleanDCache_by_Addr((uint32_t *)id_dst, (int32_t)id_cache_bytes);
    SCB_InvalidateDCache_by_Addr((void *)det_dst, (int32_t)det_cache_bytes);
    SCB_InvalidateDCache_by_Addr((void *)id_dst, (int32_t)id_cache_bytes);

    if ((memcmp(det_dst, det_src, 32U) != 0) ||
        (memcmp(det_dst + det_bytes - 32U, det_src + det_bytes - 32U, 32U) != 0) ||
        (memcmp(id_dst, id_src, 32U) != 0) ||
        (memcmp(id_dst + id_bytes - 32U, id_src + id_bytes - 32U, 32U) != 0))
    {
        face_ai_log("[FACE_AI] weights load verify=FAIL\r\n");
        return 1U;
    }

    face_ai_log("[FACE_AI] weights load verify=OK\r\n");
    return 0U;
}

uint8_t FaceAI_LoadEnrollment(void)
{
    const face_ai_enrollment_record_t *flash_record =
        (const face_ai_enrollment_record_t *)(0x90000000UL + FACE_AI_DB_FLASH_OFFSET);

    SCB_CleanInvalidateDCache();
    if (face_ai_db_is_valid(flash_record) == 0U)
    {
        face_ai_db_clear_ram();
        face_ai_log("[FACE_AI] enrollment load empty\r\n");
        return 1U;
    }

    memcpy((void *)s_face_db, flash_record, sizeof(*s_face_db));
    SCB_CleanDCache_by_Addr((uint32_t *)s_face_db, (int32_t)sizeof(*s_face_db));
    {
        char msg[80];
        int len = snprintf(msg, sizeof(msg),
                           "[FACE_AI] enrollment load id=%u ready=1\r\n",
                           (unsigned)s_face_db->user_id);
        if (len > 0)
        {
            face_ai_log(msg);
        }
    }
    return 0U;
}

uint8_t FaceAI_HasEnrollment(void)
{
    SCB_InvalidateDCache_by_Addr((void *)s_face_db, (int32_t)sizeof(*s_face_db));
    return face_ai_db_is_valid(s_face_db);
}

uint8_t FaceAI_Recognize(const face_ai_result_t *result,
                         uint16_t *matched_id,
                         float *similarity)
{
    float dot = 0.0f;
    float current_norm_sq = 0.0f;
    float value;
    float score;
    uint32_t i;

    if (matched_id != NULL)
    {
        *matched_id = 0U;
    }
    if (similarity != NULL)
    {
        *similarity = 0.0f;
    }
    if ((result == NULL) || (result->embedding_valid == 0U))
    {
        return 2U;
    }
    SCB_InvalidateDCache_by_Addr((void *)s_face_db, (int32_t)sizeof(*s_face_db));
    if (face_ai_db_is_valid(s_face_db) == 0U)
    {
        return 1U;
    }

    for (i = 0U; i < FACE_AI_EMBEDDING_SIZE; i++)
    {
        value = result->embedding[i];
        dot += value * s_face_db->embedding[i];
        current_norm_sq += value * value;
    }
    if ((current_norm_sq <= 1.0e-6f) || (s_face_db->norm <= 1.0e-6f))
    {
        return 3U;
    }

    score = dot / (sqrtf(current_norm_sq) * s_face_db->norm);
    if (similarity != NULL)
    {
        *similarity = score;
    }
    if (score < FACE_AI_MATCH_THRESHOLD)
    {
        return 4U;
    }
    if (matched_id != NULL)
    {
        *matched_id = s_face_db->user_id;
    }
    return 0U;
}

uint8_t FaceAI_EnrollReference(uint16_t user_id, const face_ai_result_t *result)
{
    face_ai_enrollment_record_t record;
    uint8_t status = 0U;
    uint8_t map_status;
    uint32_t i;

    if ((user_id == 0U) || (result == NULL) || (result->embedding_valid == 0U))
    {
        return 1U;
    }

    memset(&record, 0, sizeof(record));
    record.magic = FACE_AI_DB_MAGIC;
    record.version = FACE_AI_DB_VERSION;
    record.user_id = user_id;
    record.embedding_bytes = sizeof(record.embedding);
    record.norm = 0.0f;
    for (i = 0U; i < FACE_AI_EMBEDDING_SIZE; i++)
    {
        record.embedding[i] = result->embedding[i];
        record.norm += result->embedding[i] * result->embedding[i];
    }
    record.norm = sqrtf(record.norm);
    if (record.norm <= 1.0e-6f)
    {
        return 2U;
    }
    record.crc = face_ai_crc32((const uint8_t *)&record,
                               (uint32_t)offsetof(face_ai_enrollment_record_t, crc));

    if (MX_QUADSPI_DisableMemoryMapped() != 0U)
    {
        return 3U;
    }
    if (MX_QUADSPI_EraseRange(FACE_AI_DB_FLASH_OFFSET, FACE_AI_DB_FLASH_SIZE) != 0U)
    {
        status = 4U;
    }
    if ((status == 0U) &&
        (MX_QUADSPI_ProgramRange(FACE_AI_DB_FLASH_OFFSET,
                                 (const uint8_t *)&record,
                                 (uint32_t)sizeof(record)) != 0U))
    {
        status = 5U;
    }
    map_status = MX_QUADSPI_EnableMemoryMapped();
    if ((status == 0U) && (map_status != 0U))
    {
        status = 6U;
    }
    SCB_CleanInvalidateDCache();
    if (status != 0U)
    {
        face_ai_log("[FACE_AI] enrollment save fail\r\n");
        return status;
    }

    memcpy((void *)s_face_db, &record, sizeof(record));
    SCB_CleanDCache_by_Addr((uint32_t *)s_face_db, (int32_t)sizeof(*s_face_db));
    {
        char msg[80];
        int len = snprintf(msg, sizeof(msg),
                           "[FACE_AI] enrollment saved id=%u similarity=1.000\r\n",
                           (unsigned)user_id);
        if (len > 0)
        {
            face_ai_log(msg);
        }
    }
    return 0U;
}

uint8_t FaceAI_ClearEnrollment(void)
{
    uint8_t status;

    if (MX_QUADSPI_DisableMemoryMapped() != 0U)
    {
        return 1U;
    }
    status = MX_QUADSPI_EraseRange(FACE_AI_DB_FLASH_OFFSET, FACE_AI_DB_FLASH_SIZE);
    if (MX_QUADSPI_EnableMemoryMapped() != 0U)
    {
        status = (status == 0U) ? 2U : status;
    }
    SCB_CleanInvalidateDCache();
    face_ai_db_clear_ram();
    if (status == 0U)
    {
        face_ai_log("[FACE_AI] enrollment cleared\r\n");
    }
    return status;
}
