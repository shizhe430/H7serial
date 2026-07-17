#include "camera_app.h"
#include "dcmi.h"
#include "jpeg_stream.h"
#include "jpeg_decode.h"
#include "ov2640.h"
#include "ov2640_sccb.h"
#include "pump.h"
#include "as608.h"
#include "oled_status.h"
#include "usart.h"
#include "voice_asr.h"
#include "waterlevel.h"
#include "waterlevel_data_params.h"
#include "test_image_input.h"
#include <stdarg.h>
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
#define CAMERA_RECOVER_RETRY_COUNT  2U
#define CAMERA_BAD_FRAME_THRESHOLD  2U
#define CAMERA_RECOVER_WARMUP_FRAMES 2U

#define CAMERA_DMA_SETTLE_SPINS   8192U
#define CAMERA_DMA_STABLE_SPINS   64U

#define CAMERA_AI_VISUAL_MAGIC0   0x41U
#define CAMERA_AI_VISUAL_MAGIC1   0x49U
#define CAMERA_AI_VISUAL_MAGIC2   0x56U
#define CAMERA_AI_VISUAL_MAGIC3   0x31U
#define CAMERA_AI_VISUAL_TAIL0    0x41U
#define CAMERA_AI_VISUAL_TAIL1    0x49U
#define CAMERA_AI_VISUAL_TAIL2    0x45U
#define CAMERA_AI_VISUAL_TAIL3    0x31U
#define CAMERA_AI_VISUAL_HEADER_SIZE 32U
#define CAMERA_AI_VISUAL_SEND_GRAY_ONLY 0U

#define PUMP_AI_CONF_MIN          0.60f
#define PUMP_DECISION_WINDOW_MS   5000U
#define PUMP_VOICE_PROMPT_GUARD_MS 2000U
#define PUMP_VOICE_START_DELAY_MS 1000U
#define PUMP_CAMERA_START_SETTLE_MS 300U
#define PUMP_STABLE_FRAME_COUNT   3U
#define PUMP_CUP_LOST_FRAME_COUNT 3U
#define PUMP_ABNORMAL_FRAME_COUNT 5U
#define PUMP_ABNORMAL_CONF_MIN    0.85f
#define PUMP_REG_WINDOW_SIZE      5U
#define PUMP_REG_MIN_FRAMES       3U
#define PUMP_REG_RISE_TOL         0.03f
#define PUMP_HALF_STOP_REG        0.50f
#define PUMP_FULL_STOP_REG        0.78f
#define PUMP_FULL_NEAR_STOP_REG   0.72f
#define PUMP_HALF_CLASS_STOP_REG  0.395f
#define PUMP_FULL_CLASS_STOP_REG  0.665f
#define PUMP_TARGET_CLASS_STOP_FRAMES 3U
#define FINGER_RECOG_POLL_MS      250U
#define FINGER_RECONNECT_MS       3000U
#define PUMP_OLED_STATUS_HOLD_MS  1200U
#define PUMP_OLED_COLD_TEMP_C     25U
#define PUMP_OLED_HOT_TEMP_C      85U
#define PUMP_FLOW_CAL_TIME_MS     17301U
#define PUMP_FLOW_CAL_VOLUME_ML   400U
#define PUMP_TIMED_DEMO_HALF_MS   ((PUMP_FLOW_CAL_TIME_MS + 1U) / 2U)
#define PUMP_TIMED_DEMO_STEP_MS   20U
#define PUMP_TIMED_DEMO_KEY_MS    80U
#define ESP32_CMD_BUF_SIZE        128U
#define ESP32_SPEAK_TEXT_SIZE     96U
#define ESP32_LINK_SELFTEST_ON_BOOT 0U
#define ESP32_LINK_SELFTEST_PERIOD_MS 1000U
#define ESP32_LINK_SELFTEST_MAX_COUNT 60U
#define WATERLEVEL_LOW_HALF_SPLIT      0.395f
#define WATERLEVEL_HALF_FULL_SPLIT     0.665f
#define WATERLEVEL_RAW_HALF_KEEP_REG   0.395f
#define WATERLEVEL_RAW_FULL_KEEP_REG   0.665f
#define WATERLEVEL_DEMO_FORCE_ALL_AS_FULL 0U
#define WATERLEVEL_DEMO_FULL_LOGIT_BIAS 0.000f
#define WATERLEVEL_DEMO_RING_CUP_ENABLE 0U
#define WATERLEVEL_DEMO_RING_INNER_R_MIN 20U
#define WATERLEVEL_DEMO_RING_INNER_R_MAX 40U
#define WATERLEVEL_DEMO_RING_RIM_R_MIN   46U
#define WATERLEVEL_DEMO_RING_RIM_R_MAX   68U
#define WATERLEVEL_DEMO_RING_OUTER_R_MIN 74U
#define WATERLEVEL_DEMO_RING_OUTER_R_MAX 96U
#define WATERLEVEL_DEMO_RING_DELTA_MIN   6U
#define WATERLEVEL_DEMO_RING_BRIGHT_MIN  110U

#define WATERLEVEL_IN_ZERO_POINT  (-128)
#define WATERLEVEL_OUT0_SCALE     (0.07420533150434494f)
#define WATERLEVEL_OUT0_ZERO_POINT (9)
#define WATERLEVEL_OUT1_SCALE     (0.0027080606669187546f)
#define WATERLEVEL_OUT1_ZERO_POINT (-128)

#if (AI_WATERLEVEL_IN_1_SIZE_BYTES == (CAMERA_AI_INPUT_SIZE * 4U))
#define CAMERA_AI_MODEL_FP32      1U
#else
#define CAMERA_AI_MODEL_FP32      0U
#endif

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
    uint8_t raw_class_id;
    float level_reg;
    float confidence;
    float logits[AI_WATERLEVEL_OUT_1_SIZE];
} camera_ai_result_t;

#if (APP_MODE == APP_MODE_PUMP_CTRL)
typedef struct
{
    uint32_t frame_count;
    uint32_t action_count;
    uint8_t last_class_id;
    uint8_t requested_state;
    uint8_t applied_state;
    float last_level_reg;
    float last_confidence;
    uint8_t stop_request;
    uint8_t abnormal_latched;
    uint8_t workflow_state;
    uint8_t dispense_target;
    uint8_t water_temp;
    uint8_t stable_candidate_class;
    uint8_t stable_candidate_count;
    uint8_t cup_lost_count;
    uint8_t abnormal_count;
    uint8_t target_reached_count;
    uint8_t auto_rearm_wait_no_cup;
    uint8_t key_cold_prev_down;
    uint8_t oled_status_state;
    uint8_t fingerprint_ready;
    uint8_t fingerprint_session_locked;
    uint8_t esp32_fp_reported;
    uint8_t esp32_done_reported;
    uint8_t voice_command_armed;
    uint32_t decision_deadline_ms;
    uint32_t voice_accept_after_ms;
    uint32_t oled_status_hold_until_ms;
    uint32_t fingerprint_last_poll_ms;
    uint32_t fingerprint_last_connect_ms;
    uint16_t session_user_id;
    uint16_t fingerprint_match_score;
    uint16_t last_volume_ml;
    uint8_t pump_run_active;
    uint32_t pump_run_started_ms;
    uint32_t pump_run_total_ms;
    float reg_hist[PUMP_REG_WINDOW_SIZE];
    uint8_t reg_hist_count;
} camera_pump_ctrl_state_t;

typedef enum
{
    CAMERA_WORK_STATE_STANDBY = 0U,
    CAMERA_WORK_STATE_DECISION_WINDOW,
    CAMERA_WORK_STATE_DISPENSING_AUTO_COLD,
    CAMERA_WORK_STATE_DISPENSING_VOICE_WAIT,
    CAMERA_WORK_STATE_DISPENSING_VOICE,
    CAMERA_WORK_STATE_DISPENSING_MANUAL_COLD,
    CAMERA_WORK_STATE_DONE,
    CAMERA_WORK_STATE_FAULT,
} camera_work_state_t;

typedef enum
{
    CAMERA_DISPENSE_TARGET_NONE = 0U,
    CAMERA_DISPENSE_TARGET_HALF,
    CAMERA_DISPENSE_TARGET_FULL,
    CAMERA_DISPENSE_TARGET_MANUAL_CONTINUOUS,
} camera_dispense_target_t;

typedef enum
{
    CAMERA_WATER_TEMP_COLD = 0U,
    CAMERA_WATER_TEMP_HOT,
} camera_water_temp_t;
#endif

#if ((APP_MODE == APP_MODE_AI_INFER) || (APP_MODE == APP_MODE_AI_TEST_IMAGE) || (APP_MODE == APP_MODE_PUMP_CTRL) || (APP_MODE == APP_MODE_AI_VISUAL))
static AI_ALIGNED(32) uint8_t g_frame_buf[CAMERA_RGB565_FRAME_BYTES];
static AI_ALIGNED(32) ai_u8 g_ai_activations[AI_WATERLEVEL_DATA_ACTIVATIONS_SIZE]
    __attribute__((section(".ai_ram_d1")));
static uint8_t g_ai_clahe_lut[CAMERA_AI_CLAHE_GRID_Y][CAMERA_AI_CLAHE_GRID_X][256];
static uint8_t g_ai_gray_buf[CAMERA_AI_INPUT_SIZE];
static camera_ai_context_t g_ai_ctx;
#endif
static uint8_t g_camera_ready = 0U;
#if (APP_MODE == APP_MODE_PUMP_CTRL)
static camera_pump_ctrl_state_t g_pump_ctrl;
static AS608_Handle g_finger_sensor;
static volatile uint8_t g_esp32_rx_byte = 0U;
static char g_esp32_rx_line[ESP32_CMD_BUF_SIZE];
static volatile uint8_t g_esp32_rx_index = 0U;
static volatile uint8_t g_esp32_start_pending = 0U;
static volatile uint8_t g_esp32_stop_pending = 0U;
static char g_esp32_speak_text[ESP32_SPEAK_TEXT_SIZE];
static volatile uint8_t g_esp32_speak_pending = 0U;
#if (PUMP_TIMED_DEMO_MODE != 0U)
static uint8_t g_timed_demo_cup_present = 0U;
static uint8_t g_timed_demo_key_prev_down = 0U;
static uint32_t g_timed_demo_key_last_ms = 0U;
#endif
#endif
static uint32_t g_camera_bad_frame_count = 0U;
static uint32_t g_camera_recover_warmup_frames = 0U;
static uint32_t g_camera_dcmi_profile_idx = 0U;

typedef struct
{
    uint32_t pck;
    uint32_t vs;
    uint32_t hs;
    uint32_t jpeg_mode;
    const char *tag;
} camera_dcmi_profile_t;

static const camera_dcmi_profile_t k_camera_dcmi_profiles[] = {
    {DCMI_PCKPOLARITY_RISING, DCMI_VSPOLARITY_LOW, DCMI_HSPOLARITY_LOW, DCMI_JPEG_ENABLE,  "RLL_JE"},
    {DCMI_PCKPOLARITY_RISING, DCMI_VSPOLARITY_LOW, DCMI_HSPOLARITY_LOW, DCMI_JPEG_DISABLE, "RLL_JD"},
};

static uint8_t camera_app_capture_jpeg_snapshot(uint32_t timeout_ms, uint32_t *jpeg_off, uint32_t *jpeg_len);
static uint8_t camera_app_validate_jpeg_frame(uint32_t jpeg_off, uint32_t jpeg_len);
static uint8_t camera_app_startup_validate(const char *reason);
static void camera_app_esp32_link_selftest_poll(void);
#if (APP_MODE == APP_MODE_PUMP_CTRL)
static void camera_app_pump_ctrl_apply_state(uint8_t state);
static void camera_app_pump_ctrl_set_work_state(uint8_t next_state, const char *reason);
static void camera_app_pump_ctrl_return_to_standby(const char *reason);
static void camera_app_pump_ctrl_start_manual_cold(const char *reason);
static uint8_t camera_app_pump_ctrl_session_active(void);
static void camera_app_esp32_init(void);
static void camera_app_esp32_rx_start(void);
static void camera_app_esp32_process_rx_line(const char *line);
#endif

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

static void camera_app_text_tx(const void *buf, uint16_t len)
{
#if (APP_MODE_STREAM_SILENT == 0U)
    if ((buf != NULL) && (len != 0U))
    {
        (void)HAL_UART_Transmit(&huart1, (uint8_t *)buf, len, HAL_MAX_DELAY);
    }
#else
    (void)buf;
    (void)len;
#endif
}

static void camera_app_log(const char *s)
{
    if (s != NULL)
    {
        camera_app_text_tx(s, (uint16_t)strlen(s));
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

static void *camera_app_ai_input_data(void)
{
    if ((g_ai_ctx.input != NULL) && (g_ai_ctx.input[0].data != NULL))
    {
        return (void *)g_ai_ctx.input[0].data;
    }

    return g_ai_activations;
}

static uint8_t camera_app_ai_input_gray(const void *input, uint32_t idx)
{
#if (CAMERA_AI_MODEL_FP32 != 0U)
    const ai_float *input_f32 = (const ai_float *)input;
    float value;

    if (input_f32 == NULL)
    {
        return 0U;
    }

    value = input_f32[idx];
    if (value <= 0.0f)
    {
        return 0U;
    }
    if (value >= 1.0f)
    {
        return 255U;
    }

    return (uint8_t)((value * 255.0f) + 0.5f);
#else
    const ai_i8 *input_q7 = (const ai_i8 *)input;

    if (input_q7 == NULL)
    {
        return 0U;
    }

    return (uint8_t)((int16_t)input_q7[idx] - WATERLEVEL_IN_ZERO_POINT);
#endif
}

static int32_t camera_app_float_to_milli_signed(float value)
{
    if (value >= 0.0f)
    {
        return (int32_t)((value * 1000.0f) + 0.5f);
    }

    return (int32_t)((value * 1000.0f) - 0.5f);
}

static uint8_t camera_app_logit_to_visual_byte(float value)
{
    int32_t scaled = (int32_t)((value * 4.0f) + 128.0f);

    if (scaled < 0)
    {
        return 0U;
    }
    if (scaled > 255)
    {
        return 255U;
    }

    return (uint8_t)scaled;
}

static void camera_app_load_test_image(void *dst_input)
{
    uint32_t i;

    if (dst_input == NULL)
    {
        return;
    }

#if (CAMERA_AI_MODEL_FP32 != 0U)
    for (i = 0U; i < CAMERA_AI_INPUT_SIZE; i++)
    {
        ((ai_float *)dst_input)[i] =
            ((ai_float)((int16_t)g_test_image_input_q7[i] - WATERLEVEL_IN_ZERO_POINT)) / 255.0f;
    }
#else
    (void)i;
    memcpy(dst_input, g_test_image_input_q7, CAMERA_AI_INPUT_SIZE);
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

static uint8_t camera_app_reinit_dcmi(uint32_t pck_polarity,
                                      uint32_t vs_polarity,
                                      uint32_t hs_polarity,
                                      uint32_t jpeg_mode)
{
    (void)HAL_DCMI_Stop(&hdcmi);
    (void)HAL_DCMI_DeInit(&hdcmi);

    hdcmi.Instance = DCMI;
    hdcmi.Init.SynchroMode = DCMI_SYNCHRO_HARDWARE;
    hdcmi.Init.PCKPolarity = pck_polarity;
    hdcmi.Init.VSPolarity = vs_polarity;
    hdcmi.Init.HSPolarity = hs_polarity;
    hdcmi.Init.CaptureRate = DCMI_CR_ALL_FRAME;
    hdcmi.Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;
    hdcmi.Init.JPEGMode = jpeg_mode;
    hdcmi.Init.ByteSelectMode = DCMI_BSM_ALL;
    hdcmi.Init.ByteSelectStart = DCMI_OEBS_ODD;
    hdcmi.Init.LineSelectMode = DCMI_LSM_ALL;
    hdcmi.Init.LineSelectStart = DCMI_OELS_ODD;

    return (HAL_DCMI_Init(&hdcmi) == HAL_OK) ? 0U : 1U;
}

static uint8_t camera_app_jpeg_header_is_plausible(const uint8_t *jpeg, uint32_t jpeg_len)
{
    uint8_t marker;

    if ((jpeg == NULL) || (jpeg_len < 4U))
    {
        return 0U;
    }

    if ((jpeg[0] != 0xFFU) || (jpeg[1] != 0xD8U) || (jpeg[2] != 0xFFU))
    {
        return 0U;
    }

    marker = jpeg[3];
    if ((marker >= 0xE0U) && (marker <= 0xEFU))
    {
        return 1U;
    }

    switch (marker)
    {
        case 0xC0U:
        case 0xC2U:
        case 0xC4U:
        case 0xDBU:
        case 0xDDU:
        case 0xFEU:
            return 1U;

        default:
            return 0U;
    }
}

static uint8_t camera_app_reinit_dcmi_profile(uint32_t profile_idx)
{
    if (profile_idx >= (sizeof(k_camera_dcmi_profiles) / sizeof(k_camera_dcmi_profiles[0])))
    {
        return 1U;
    }

    return camera_app_reinit_dcmi(k_camera_dcmi_profiles[profile_idx].pck,
                                  k_camera_dcmi_profiles[profile_idx].vs,
                                  k_camera_dcmi_profiles[profile_idx].hs,
                                  k_camera_dcmi_profiles[profile_idx].jpeg_mode);
}

static uint8_t camera_app_recover_camera(const char *reason)
{
    uint16_t mid = 0U;
    uint16_t pid = 0U;
    uint32_t try_idx;
    uint32_t profile_step;
    uint32_t profile_count = (uint32_t)(sizeof(k_camera_dcmi_profiles) / sizeof(k_camera_dcmi_profiles[0]));
    char buf[96];
    int len;
    uint32_t jpeg_off = 0U;
    uint32_t jpeg_len = 0U;

    g_camera_ready = 0U;
    g_frame_done = 0U;
    g_frame_error = 0U;
    g_camera_bad_frame_count = 0U;

    len = snprintf(buf, sizeof(buf), "[APP] recover start reason=%s\r\n", (reason != NULL) ? reason : "unknown");
    camera_app_text_tx(buf, (uint16_t)len);

    for (profile_step = 0U; profile_step < profile_count; profile_step++)
    {
        uint32_t profile_idx = (g_camera_dcmi_profile_idx + profile_step) % profile_count;

        if (camera_app_reinit_dcmi_profile(profile_idx) != 0U)
        {
            continue;
        }

        HAL_NVIC_SetPriority(DCMI_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(DCMI_IRQn);

        JPEG_Stream_Init();
        OV2640_AttachFrameBuffer(JPEG_Stream_GetBuf(), JPEG_Stream_GetMaxSize());

        for (try_idx = 0U; try_idx < CAMERA_RECOVER_RETRY_COUNT; try_idx++)
        {
            if (OV2640_Probe(&mid, &pid) != OV2640_OK)
            {
                HAL_Delay(50U);
                continue;
            }

            if (OV2640_Init() != OV2640_OK)
            {
                HAL_Delay(50U);
                continue;
            }

            if (OV2640_SetOutputFormatJPEG() != OV2640_OK)
            {
                HAL_Delay(50U);
                continue;
            }

            if (OV2640_SetOutputSize(CAMERA_WIDTH, CAMERA_HEIGHT) != OV2640_OK)
            {
                HAL_Delay(50U);
                continue;
            }

            JPEG_Stream_Init();
            OV2640_AttachFrameBuffer(JPEG_Stream_GetBuf(), JPEG_Stream_GetMaxSize());
            HAL_Delay(150U);

            if (camera_app_capture_jpeg_snapshot(1500U, &jpeg_off, &jpeg_len) != 0U)
            {
                HAL_Delay(50U);
                continue;
            }

            if (camera_app_validate_jpeg_frame(jpeg_off, jpeg_len) != 0U)
            {
                HAL_Delay(50U);
                continue;
            }

            g_camera_dcmi_profile_idx = profile_idx;
            g_camera_ready = 1U;
            g_camera_recover_warmup_frames = CAMERA_RECOVER_WARMUP_FRAMES;
            len = snprintf(buf, sizeof(buf),
                           "[APP] recover ok mid=0x%04X pid=0x%04X try=%lu cfg=%s\r\n",
                           mid, pid, (unsigned long)(try_idx + 1U), k_camera_dcmi_profiles[profile_idx].tag);
            camera_app_text_tx(buf, (uint16_t)len);
            return 0U;
        }
    }

    len = snprintf(buf, sizeof(buf),
                   "[APP] recover fail mid=0x%04X pid=0x%04X\r\n", mid, pid);
    camera_app_text_tx(buf, (uint16_t)len);
    return 1U;
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
    memset(JPEG_Stream_GetBuf(), 0, JPEG_Stream_GetMaxSize());

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
    memset(JPEG_Stream_GetBuf(), 0, JPEG_Stream_GetMaxSize());

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

#if ((APP_MODE == APP_MODE_AI_INFER) || (APP_MODE == APP_MODE_AI_TEST_IMAGE) || (APP_MODE == APP_MODE_PUMP_CTRL) || (APP_MODE == APP_MODE_AI_VISUAL))
static uint8_t camera_app_validate_jpeg_frame(uint32_t jpeg_off, uint32_t jpeg_len)
{
    if (camera_app_jpeg_header_is_plausible(JPEG_Stream_GetBuf() + jpeg_off, jpeg_len) == 0U)
    {
        return 1U;
    }

    return (jpeg_to_ai_input(JPEG_Stream_GetBuf() + jpeg_off, jpeg_len, camera_app_ai_input_data()) == 0U) ? 0U : 2U;
}
#else
static uint8_t camera_app_validate_jpeg_frame(uint32_t jpeg_off, uint32_t jpeg_len)
{
    return camera_app_jpeg_header_is_plausible(JPEG_Stream_GetBuf() + jpeg_off, jpeg_len) ? 0U : 1U;
}
#endif

static uint8_t camera_app_startup_validate(const char *reason)
{
    uint32_t jpeg_off = 0U;
    uint32_t jpeg_len = 0U;
    uint32_t attempt;
    uint8_t *jpeg_buf = JPEG_Stream_GetBuf();

    for (attempt = 0U; attempt < 3U; attempt++)
    {
        JPEG_Stream_Init();
        OV2640_AttachFrameBuffer(JPEG_Stream_GetBuf(), JPEG_Stream_GetMaxSize());
        HAL_Delay(120U);

        if (camera_app_capture_jpeg_snapshot(1500U, &jpeg_off, &jpeg_len) != 0U)
        {
            HAL_Delay(120U);
            continue;
        }

        if (camera_app_jpeg_header_is_plausible(jpeg_buf + jpeg_off, jpeg_len) != 0U)
        {
            g_camera_bad_frame_count = 0U;
            return 0U;
        }

        HAL_Delay(120U);
    }

    return camera_app_recover_camera(reason);
}

static void camera_app_report_jpeg_diag(void)
{
    uint32_t soi_off = 0U;
    uint32_t jpeg_len = JPEG_Stream_FindFrame(&soi_off);
    uint8_t *buf = JPEG_Stream_GetBuf();
    char msg[256];
    int len;

    if ((jpeg_len == 0U) || ((soi_off + jpeg_len) > JPEG_Stream_GetMaxSize()))
    {
        len = snprintf(msg, sizeof(msg),
                       "[JDIAG] noframe len=%lu off=%lu ndtr=%lu buf0=%02X %02X %02X %02X %02X %02X %02X %02X\r\n",
                       (unsigned long)jpeg_len,
                       (unsigned long)soi_off,
                       (unsigned long)camera_app_dma_get_ndtr(),
                       buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
        HAL_UART_Transmit(&huart1, (uint8_t *)msg, (uint16_t)len, HAL_MAX_DELAY);
        return;
    }

    len = snprintf(msg, sizeof(msg),
                   "[JDIAG] off=%lu len=%lu ndtr=%lu head=%02X %02X %02X %02X %02X %02X %02X %02X tail=%02X %02X %02X %02X %02X %02X %02X %02X\r\n",
                   (unsigned long)soi_off,
                   (unsigned long)jpeg_len,
                   (unsigned long)camera_app_dma_get_ndtr(),
                   buf[soi_off + 0U], buf[soi_off + 1U], buf[soi_off + 2U], buf[soi_off + 3U],
                   buf[soi_off + 4U], buf[soi_off + 5U], buf[soi_off + 6U], buf[soi_off + 7U],
                   buf[soi_off + jpeg_len - 8U], buf[soi_off + jpeg_len - 7U],
                   buf[soi_off + jpeg_len - 6U], buf[soi_off + jpeg_len - 5U],
                   buf[soi_off + jpeg_len - 4U], buf[soi_off + jpeg_len - 3U],
                   buf[soi_off + jpeg_len - 2U], buf[soi_off + jpeg_len - 1U]);
    HAL_UART_Transmit(&huart1, (uint8_t *)msg, (uint16_t)len, HAL_MAX_DELAY);
}

typedef struct
{
    uint32_t pck;
    uint32_t vs;
    uint32_t hs;
    const char *tag;
} camera_dcmi_diag_cfg_t;

static const camera_dcmi_diag_cfg_t k_dcmi_diag_cfgs[] = {
    {DCMI_PCKPOLARITY_RISING,  DCMI_VSPOLARITY_LOW,  DCMI_HSPOLARITY_LOW,  "RLL"},
    {DCMI_PCKPOLARITY_RISING,  DCMI_VSPOLARITY_LOW,  DCMI_HSPOLARITY_HIGH, "RLH"},
    {DCMI_PCKPOLARITY_RISING,  DCMI_VSPOLARITY_HIGH, DCMI_HSPOLARITY_LOW,  "RHL"},
    {DCMI_PCKPOLARITY_RISING,  DCMI_VSPOLARITY_HIGH, DCMI_HSPOLARITY_HIGH, "RHH"},
    {DCMI_PCKPOLARITY_FALLING, DCMI_VSPOLARITY_LOW,  DCMI_HSPOLARITY_LOW,  "FLL"},
    {DCMI_PCKPOLARITY_FALLING, DCMI_VSPOLARITY_LOW,  DCMI_HSPOLARITY_HIGH, "FLH"},
    {DCMI_PCKPOLARITY_FALLING, DCMI_VSPOLARITY_HIGH, DCMI_HSPOLARITY_LOW,  "FHL"},
    {DCMI_PCKPOLARITY_FALLING, DCMI_VSPOLARITY_HIGH, DCMI_HSPOLARITY_HIGH, "FHH"},
};

static uint8_t camera_app_set_dcmi_diag_cfg(uint32_t idx)
{
    if (idx >= (sizeof(k_dcmi_diag_cfgs) / sizeof(k_dcmi_diag_cfgs[0])))
    {
        return 1U;
    }

    return (camera_app_reinit_dcmi(k_dcmi_diag_cfgs[idx].pck,
                                   k_dcmi_diag_cfgs[idx].vs,
                                   k_dcmi_diag_cfgs[idx].hs,
                                   DCMI_JPEG_ENABLE) == 0U) ? 0U : 2U;
}

#if ((APP_MODE == APP_MODE_AI_INFER) || (APP_MODE == APP_MODE_AI_TEST_IMAGE) || (APP_MODE == APP_MODE_PUMP_CTRL) || (APP_MODE == APP_MODE_AI_VISUAL))
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

    if ((g_ai_ctx.input[0].data == NULL) ||
        (g_ai_ctx.output[0].data == NULL) ||
        (g_ai_ctx.output[1].data == NULL))
    {
        return 3U;
    }

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

static void camera_app_preprocess_rgb565_to_ai(const uint16_t *src_rgb565, void *dst_input)
{
    uint32_t y;
    uint32_t gray_sum = 0U;
    uint32_t gray_count = 0U;
    uint32_t norm_num = CAMERA_AI_TARGET_MEAN_GRAY;
    uint32_t norm_den = 1U;
#if (CAMERA_AI_MODEL_FP32 != 0U)
    ai_float *dst = (ai_float *)dst_input;
#else
    ai_i8 *dst = (ai_i8 *)dst_input;
#endif

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

#if (CAMERA_AI_MODEL_FP32 != 0U)
            uint32_t scaled = gray;

            if (CAMERA_AI_ENABLE_BRIGHTNESS_NORM != 0U)
            {
                scaled = ((uint32_t)gray * norm_num + (norm_den / 2U)) / norm_den;
            }
            if (scaled > 255U)
            {
                scaled = 255U;
            }
            dst[idx] = ((ai_float)scaled) / 255.0f;
#else
            if (gray == 0U)
            {
                dst[idx] = (ai_i8)WATERLEVEL_IN_ZERO_POINT;
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
                dst[idx] = (ai_i8)((int16_t)scaled + WATERLEVEL_IN_ZERO_POINT);
            }
#endif
        }
    }
}

#if (CAMERA_AI_DUMP_INPUT_ONCE != 0U)
static void camera_app_dump_ai_input_once(const void *input)
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

static uint8_t camera_app_demo_center_cup_present(void)
{
#if (WATERLEVEL_DEMO_RING_CUP_ENABLE != 0U)
    uint32_t inner_sum = 0U;
    uint32_t inner_cnt = 0U;
    uint32_t rim_sum = 0U;
    uint32_t rim_cnt = 0U;
    uint32_t outer_sum = 0U;
    uint32_t outer_cnt = 0U;
    uint32_t y;

    for (y = 0U; y < 224U; y++)
    {
        uint32_t x;

        for (x = 0U; x < 224U; x++)
        {
            uint32_t idx = (y * 224U) + x;
            uint32_t dx = (x > CAMERA_AI_MASK_CENTER) ? (x - CAMERA_AI_MASK_CENTER) : (CAMERA_AI_MASK_CENTER - x);
            uint32_t dy = (y > CAMERA_AI_MASK_CENTER) ? (y - CAMERA_AI_MASK_CENTER) : (CAMERA_AI_MASK_CENTER - y);
            uint32_t d2 = (dx * dx) + (dy * dy);
            uint32_t gray = camera_app_ai_input_gray(camera_app_ai_input_data(), idx);

            if ((d2 >= (WATERLEVEL_DEMO_RING_INNER_R_MIN * WATERLEVEL_DEMO_RING_INNER_R_MIN)) &&
                (d2 <= (WATERLEVEL_DEMO_RING_INNER_R_MAX * WATERLEVEL_DEMO_RING_INNER_R_MAX)))
            {
                inner_sum += gray;
                inner_cnt++;
            }
            else if ((d2 >= (WATERLEVEL_DEMO_RING_RIM_R_MIN * WATERLEVEL_DEMO_RING_RIM_R_MIN)) &&
                     (d2 <= (WATERLEVEL_DEMO_RING_RIM_R_MAX * WATERLEVEL_DEMO_RING_RIM_R_MAX)))
            {
                rim_sum += gray;
                rim_cnt++;
            }
            else if ((d2 >= (WATERLEVEL_DEMO_RING_OUTER_R_MIN * WATERLEVEL_DEMO_RING_OUTER_R_MIN)) &&
                     (d2 <= (WATERLEVEL_DEMO_RING_OUTER_R_MAX * WATERLEVEL_DEMO_RING_OUTER_R_MAX)))
            {
                outer_sum += gray;
                outer_cnt++;
            }
        }
    }

    if ((inner_cnt == 0U) || (rim_cnt == 0U) || (outer_cnt == 0U))
    {
        return 0U;
    }

    {
        uint32_t inner_mean = inner_sum / inner_cnt;
        uint32_t rim_mean = rim_sum / rim_cnt;
        uint32_t outer_mean = outer_sum / outer_cnt;

        if ((rim_mean >= WATERLEVEL_DEMO_RING_BRIGHT_MIN) &&
            (rim_mean >= (inner_mean + WATERLEVEL_DEMO_RING_DELTA_MIN)) &&
            (rim_mean >= (outer_mean + WATERLEVEL_DEMO_RING_DELTA_MIN)))
        {
            return 1U;
        }
    }
#endif

    return 0U;
}

static float camera_app_ai_logit_with_bias(const camera_ai_result_t *result, uint32_t class_idx)
{
    float logit = result->logits[class_idx];

    if (class_idx == 3U)
    {
        logit += WATERLEVEL_DEMO_FULL_LOGIT_BIAS;
    }

    return logit;
}

static uint8_t camera_app_ai_run(camera_ai_result_t *result)
{
    ai_i32 batches;
    uint32_t i;
    const void *output_logits;
    const void *output_reg;
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

    output_logits = (const void *)g_ai_ctx.output[0].data;
    output_reg = (const void *)g_ai_ctx.output[1].data;
    if ((output_logits == NULL) || (output_reg == NULL))
    {
        return 3U;
    }

#if (CAMERA_AI_MODEL_FP32 != 0U)
    for (i = 0U; i < AI_WATERLEVEL_OUT_1_SIZE; i++)
    {
        result->logits[i] = ((const ai_float *)output_logits)[i];
    }
    result->level_reg = ((const ai_float *)output_reg)[0];
#else
    for (i = 0U; i < AI_WATERLEVEL_OUT_1_SIZE; i++)
    {
        result->logits[i] =
            ((float)((int32_t)((const ai_i8 *)output_logits)[i] - WATERLEVEL_OUT0_ZERO_POINT)) *
            WATERLEVEL_OUT0_SCALE;
    }
    result->level_reg =
        ((float)((int32_t)((const ai_i8 *)output_reg)[0] - WATERLEVEL_OUT1_ZERO_POINT)) *
        WATERLEVEL_OUT1_SCALE;
#endif
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
        float logit = camera_app_ai_logit_with_bias(result, i);
        if ((i == 0U) || (logit > best_logit))
        {
            best_logit = logit;
            best_idx = (uint8_t)i;
        }
    }

    for (i = 0U; i < AI_WATERLEVEL_OUT_1_SIZE; i++)
    {
        float logit = camera_app_ai_logit_with_bias(result, i);
        float e = expf(logit - best_logit);
        exp_sum += e;
        if (i == best_idx)
        {
            best_prob = e;
        }
    }

    result->raw_class_id = best_idx;
    result->class_id = best_idx;
    result->confidence = (exp_sum > 0.0f) ? (best_prob / exp_sum) : 0.0f;

    if ((result->raw_class_id == 1U) || (result->raw_class_id == 2U) || (result->raw_class_id == 3U))
    {
        /*
         * Use regression as the main waterline signal, but keep some respect for
         * the classifier argmax so half-cup frames are not overly collapsed into
         * "low" when reg hovers near the boundary.
         */
        if ((result->raw_class_id == 3U) && (result->level_reg >= WATERLEVEL_RAW_FULL_KEEP_REG))
        {
            result->class_id = 3U;
        }
        else if (result->level_reg >= WATERLEVEL_HALF_FULL_SPLIT)
        {
            result->class_id = 3U;
        }
        else if ((result->raw_class_id == 2U) && (result->level_reg >= WATERLEVEL_RAW_HALF_KEEP_REG))
        {
            result->class_id = 2U;
        }
        else if (result->level_reg >= WATERLEVEL_LOW_HALF_SPLIT)
        {
            result->class_id = 2U;
        }
        else
        {
            result->class_id = 1U;
        }
    }

#if (WATERLEVEL_DEMO_RING_CUP_ENABLE != 0U)
    if ((result->raw_class_id == 0U) && (camera_app_demo_center_cup_present() != 0U))
    {
        result->raw_class_id = 3U;
        result->class_id = 3U;
    }
#endif

#if (WATERLEVEL_DEMO_FORCE_ALL_AS_FULL != 0U)
    if ((result->class_id == 1U) || (result->class_id == 2U) || (result->class_id == 3U))
    {
        result->class_id = 3U;
    }
#endif

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
    int32_t logit0;
    int32_t logit1;
    int32_t logit2;
    int32_t logit3;
    int32_t logit4;

    if (result == NULL)
    {
        return;
    }

    conf_x10 = camera_app_float_to_permille(result->confidence);
    reg_x1000 = camera_app_float_to_permille(result->level_reg);
    fps_x10 = (pipeline_ms > 0U) ? (10000U / pipeline_ms) : 0U;
    logit0 = camera_app_float_to_milli_signed(result->logits[0]);
    logit1 = camera_app_float_to_milli_signed(result->logits[1]);
    logit2 = camera_app_float_to_milli_signed(result->logits[2]);
    logit3 = camera_app_float_to_milli_signed(result->logits[3]);
    logit4 = camera_app_float_to_milli_signed(result->logits[4]);

    len = snprintf(buf, sizeof(buf),
                   "[AI] cls=%lu raw_cls=%lu name=%s conf=%lu.%lu%% reg=0.%03lu pipe=%lums nn=%lums fps=%lu.%lu raw_x1000=[%ld,%ld,%ld,%ld,%ld]\r\n",
                   (unsigned long)result->class_id,
                   (unsigned long)result->raw_class_id,
                   camera_ai_class_name(result->class_id),
                   (unsigned long)(conf_x10 / 10U),
                   (unsigned long)(conf_x10 % 10U),
                   (unsigned long)reg_x1000,
                   (unsigned long)pipeline_ms,
                   (unsigned long)infer_ms,
                   (unsigned long)(fps_x10 / 10U),
                   (unsigned long)(fps_x10 % 10U),
                   (long)logit0,
                   (long)logit1,
                   (long)logit2,
                   (long)logit3,
                   (long)logit4);
    HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
}

static void camera_app_store_u16le(uint8_t *dst, uint16_t value)
{
    dst[0] = (uint8_t)(value & 0xFFU);
    dst[1] = (uint8_t)((value >> 8) & 0xFFU);
}

static void camera_app_store_u32le(uint8_t *dst, uint32_t value)
{
    dst[0] = (uint8_t)(value & 0xFFU);
    dst[1] = (uint8_t)((value >> 8) & 0xFFU);
    dst[2] = (uint8_t)((value >> 16) & 0xFFU);
    dst[3] = (uint8_t)((value >> 24) & 0xFFU);
}

static void camera_app_ai_visual_send_frame(const camera_ai_result_t *result,
                                            uint32_t pipeline_ms,
                                            uint32_t infer_ms,
                                            const uint8_t *jpeg,
                                            uint32_t jpeg_len)
{
    static uint32_t s_frame_id = 0U;
    uint8_t header[CAMERA_AI_VISUAL_HEADER_SIZE];
    uint8_t tail[4];
    uint16_t conf_permille;
    uint16_t reg_permille;
    uint16_t pipeline_ms16;
    uint16_t infer_ms16;

    if ((result == NULL) || (jpeg == NULL) || (jpeg_len == 0U) || (jpeg_len > 65535U))
    {
        return;
    }

    conf_permille = (uint16_t)camera_app_float_to_permille(result->confidence);
    reg_permille = (uint16_t)camera_app_float_to_permille(result->level_reg);
    pipeline_ms16 = (pipeline_ms > 0xFFFFU) ? 0xFFFFU : (uint16_t)pipeline_ms;
    infer_ms16 = (infer_ms > 0xFFFFU) ? 0xFFFFU : (uint16_t)infer_ms;

    memset(header, 0, sizeof(header));
    header[0] = CAMERA_AI_VISUAL_MAGIC0;
    header[1] = CAMERA_AI_VISUAL_MAGIC1;
    header[2] = CAMERA_AI_VISUAL_MAGIC2;
    header[3] = CAMERA_AI_VISUAL_MAGIC3;
    header[4] = 0x01U;
    header[5] = result->class_id;
    header[6] = result->raw_class_id;
    header[7] = 0U;
    camera_app_store_u16le(&header[8], conf_permille);
    camera_app_store_u16le(&header[10], reg_permille);
    camera_app_store_u16le(&header[12], pipeline_ms16);
    camera_app_store_u16le(&header[14], infer_ms16);
    camera_app_store_u32le(&header[16], jpeg_len);
    camera_app_store_u32le(&header[20], ++s_frame_id);
    header[24] = camera_app_logit_to_visual_byte(result->logits[0]);
    header[25] = camera_app_logit_to_visual_byte(result->logits[1]);
    header[26] = camera_app_logit_to_visual_byte(result->logits[2]);
    header[27] = camera_app_logit_to_visual_byte(result->logits[3]);
    header[28] = camera_app_logit_to_visual_byte(result->logits[4]);

    tail[0] = CAMERA_AI_VISUAL_TAIL0;
    tail[1] = CAMERA_AI_VISUAL_TAIL1;
    tail[2] = CAMERA_AI_VISUAL_TAIL2;
    tail[3] = CAMERA_AI_VISUAL_TAIL3;

    HAL_UART_Transmit(&huart1, header, (uint16_t)sizeof(header), HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1, (uint8_t *)jpeg, (uint16_t)jpeg_len, HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1, tail, (uint16_t)sizeof(tail), HAL_MAX_DELAY);
}

static void camera_app_ai_visual_send_gray_frame(const camera_ai_result_t *result,
                                                 uint32_t pipeline_ms,
                                                 uint32_t infer_ms,
                                                 const void *input)
{
    static uint32_t s_frame_id = 0U;
    uint8_t header[CAMERA_AI_VISUAL_HEADER_SIZE];
    uint8_t tail[4];
    uint16_t conf_permille;
    uint16_t reg_permille;
    uint16_t pipeline_ms16;
    uint16_t infer_ms16;
    uint32_t i;

    if ((result == NULL) || (input == NULL))
    {
        return;
    }

    conf_permille = (uint16_t)camera_app_float_to_permille(result->confidence);
    reg_permille = (uint16_t)camera_app_float_to_permille(result->level_reg);
    pipeline_ms16 = (pipeline_ms > 0xFFFFU) ? 0xFFFFU : (uint16_t)pipeline_ms;
    infer_ms16 = (infer_ms > 0xFFFFU) ? 0xFFFFU : (uint16_t)infer_ms;

    memset(header, 0, sizeof(header));
    header[0] = CAMERA_AI_VISUAL_MAGIC0;
    header[1] = CAMERA_AI_VISUAL_MAGIC1;
    header[2] = CAMERA_AI_VISUAL_MAGIC2;
    header[3] = CAMERA_AI_VISUAL_MAGIC3;
    header[4] = 0x02U;
    header[5] = result->class_id;
    header[6] = result->raw_class_id;
    header[7] = 0x02U;
    camera_app_store_u16le(&header[8], 224U);
    camera_app_store_u16le(&header[10], 224U);
    camera_app_store_u16le(&header[12], conf_permille);
    camera_app_store_u16le(&header[14], reg_permille);
    camera_app_store_u32le(&header[16], CAMERA_AI_INPUT_SIZE);
    camera_app_store_u32le(&header[20], ++s_frame_id);
    header[24] = camera_app_logit_to_visual_byte(result->logits[0]);
    header[25] = camera_app_logit_to_visual_byte(result->logits[1]);
    header[26] = camera_app_logit_to_visual_byte(result->logits[2]);
    header[27] = camera_app_logit_to_visual_byte(result->logits[3]);
    header[28] = camera_app_logit_to_visual_byte(result->logits[4]);
    header[29] = (uint8_t)pipeline_ms16;
    header[30] = (uint8_t)infer_ms16;
    header[31] = 0U;

    tail[0] = CAMERA_AI_VISUAL_TAIL0;
    tail[1] = CAMERA_AI_VISUAL_TAIL1;
    tail[2] = CAMERA_AI_VISUAL_TAIL2;
    tail[3] = CAMERA_AI_VISUAL_TAIL3;

    for (i = 0U; i < CAMERA_AI_INPUT_SIZE; i++)
    {
        g_ai_gray_buf[i] = camera_app_ai_input_gray(input, i);
    }

    HAL_UART_Transmit(&huart1, header, (uint16_t)sizeof(header), HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1, g_ai_gray_buf, (uint16_t)CAMERA_AI_INPUT_SIZE, HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1, tail, (uint16_t)sizeof(tail), HAL_MAX_DELAY);
}
#endif

int AI_GetState(void)
{
#if (APP_MODE == APP_MODE_PUMP_CTRL)
    return (int)g_pump_ctrl.applied_state;
#else
    return (int)PUMP_COMMAND_STOP;
#endif
}

#if (APP_MODE == APP_MODE_PUMP_CTRL)
static const char *camera_app_pump_cmd_name(uint8_t command)
{
    switch (command)
    {
        case PUMP_COMMAND_FAST:
            return "fast";

        case PUMP_COMMAND_SLOW:
            return "slow";

        case PUMP_COMMAND_STOP:
        default:
            return "stop";
    }
}

static const char *camera_app_work_state_name(uint8_t state)
{
    switch ((camera_work_state_t)state)
    {
        case CAMERA_WORK_STATE_STANDBY:
            return "standby";

        case CAMERA_WORK_STATE_DECISION_WINDOW:
            return "wait_cmd";

        case CAMERA_WORK_STATE_DISPENSING_AUTO_COLD:
            return "auto_cold";

        case CAMERA_WORK_STATE_DISPENSING_VOICE_WAIT:
            return "voice_wait";

        case CAMERA_WORK_STATE_DISPENSING_VOICE:
            return "voice";

        case CAMERA_WORK_STATE_DISPENSING_MANUAL_COLD:
            return "manual_cold";

        case CAMERA_WORK_STATE_DONE:
            return "done";

        case CAMERA_WORK_STATE_FAULT:
            return "fault";

        default:
            return "unknown";
    }
}

static const char *camera_app_target_name(uint8_t target)
{
    switch ((camera_dispense_target_t)target)
    {
        case CAMERA_DISPENSE_TARGET_HALF:
            return "half";

        case CAMERA_DISPENSE_TARGET_FULL:
            return "full";

        case CAMERA_DISPENSE_TARGET_MANUAL_CONTINUOUS:
            return "manual";

        case CAMERA_DISPENSE_TARGET_NONE:
        default:
            return "none";
    }
}

static const char *camera_app_temp_name(uint8_t temp)
{
    switch ((camera_water_temp_t)temp)
    {
        case CAMERA_WATER_TEMP_HOT:
            return "hot";

        case CAMERA_WATER_TEMP_COLD:
        default:
            return "cold";
    }
}

static uint8_t camera_app_cold_key_is_down(void)
{
    return (HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_7) == GPIO_PIN_RESET) ? 1U : 0U;
}

static void camera_app_oled_refresh_stub(void)
{
    uint8_t water_level;
    uint8_t water_out_state;
    uint16_t temp_val;
    uint16_t dev_id;
    uint32_t now_ms;

    if ((g_pump_ctrl.last_class_id >= 1U) && (g_pump_ctrl.last_class_id <= 3U))
    {
        water_level = (g_pump_ctrl.last_class_id >= 2U) ? 1U : 0U;
    }
    else
    {
        water_level = 0U;
    }

    now_ms = HAL_GetTick();
    if ((g_pump_ctrl.oled_status_hold_until_ms != 0U) &&
        ((int32_t)(now_ms - g_pump_ctrl.oled_status_hold_until_ms) < 0))
    {
        water_out_state = g_pump_ctrl.oled_status_state;
    }
    else
    {
        switch ((camera_work_state_t)g_pump_ctrl.workflow_state)
        {
            case CAMERA_WORK_STATE_DISPENSING_AUTO_COLD:
            case CAMERA_WORK_STATE_DISPENSING_VOICE:
            case CAMERA_WORK_STATE_DISPENSING_MANUAL_COLD:
                water_out_state = 1U;
                break;

            case CAMERA_WORK_STATE_FAULT:
                water_out_state = 2U;
                break;

            case CAMERA_WORK_STATE_DECISION_WINDOW:
            case CAMERA_WORK_STATE_DISPENSING_VOICE_WAIT:
            case CAMERA_WORK_STATE_DONE:
            case CAMERA_WORK_STATE_STANDBY:
            default:
                water_out_state = 0U;
                break;
        }
    }

    temp_val = (g_pump_ctrl.water_temp == CAMERA_WATER_TEMP_HOT) ? PUMP_OLED_HOT_TEMP_C : PUMP_OLED_COLD_TEMP_C;
    dev_id = g_pump_ctrl.session_user_id;
    if (((camera_work_state_t)g_pump_ctrl.workflow_state == CAMERA_WORK_STATE_STANDBY) &&
        ((g_pump_ctrl.oled_status_hold_until_ms == 0U) ||
         ((int32_t)(now_ms - g_pump_ctrl.oled_status_hold_until_ms) >= 0)))
    {
        dev_id = 0U;
    }

    OLED_Status_Show(water_level,
                     water_out_state,
                     temp_val,
                     g_pump_ctrl.water_temp,
                     dev_id);
}

static void camera_app_pump_ctrl_hold_oled_status(uint8_t water_out_state)
{
    g_pump_ctrl.oled_status_state = water_out_state;
    g_pump_ctrl.oled_status_hold_until_ms = HAL_GetTick() + PUMP_OLED_STATUS_HOLD_MS;
}

static uint8_t camera_app_voice_result_read(void)
{
    int result = Asr_Result();

    if (result < 0)
    {
        return 0U;
    }

    return (uint8_t)result;
}

static uint8_t camera_app_voice_result_is_supported(uint8_t result)
{
    switch (result)
    {
        case ASR_WORD_HALF_WATER:
        case ASR_WORD_FULL_WATER:
        case ASR_WORD_HALF_HOT_WATER:
        case ASR_WORD_FULL_HOT_WATER:
            return 1U;

        default:
            return 0U;
    }
}

static void camera_app_finger_log(const char *fmt, ...)
{
    char buf[160];
    va_list args;
    int len;

    if (fmt == NULL)
    {
        return;
    }

    va_start(args, fmt);
    len = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    if (len > 0)
    {
        HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
    }
}

static uint16_t camera_app_pump_ctrl_calc_volume_ml(uint32_t fill_time_ms)
{
    uint64_t scaled;

    if (fill_time_ms == 0U)
    {
        return 0U;
    }

    scaled = ((uint64_t)fill_time_ms * (uint64_t)PUMP_FLOW_CAL_VOLUME_ML) +
             ((uint64_t)PUMP_FLOW_CAL_TIME_MS / 2ULL);
    return (uint16_t)(scaled / (uint64_t)PUMP_FLOW_CAL_TIME_MS);
}

static uint32_t camera_app_pump_ctrl_get_fill_time_ms(void)
{
    uint32_t fill_time_ms = g_pump_ctrl.pump_run_total_ms;

    if ((g_pump_ctrl.pump_run_active != 0U) && (g_pump_ctrl.applied_state != PUMP_COMMAND_STOP))
    {
        fill_time_ms += (HAL_GetTick() - g_pump_ctrl.pump_run_started_ms);
    }

    return fill_time_ms;
}

static void camera_app_esp32_send_text(const char *text)
{
    size_t len;

    if (text == NULL)
    {
        return;
    }

    len = strlen(text);
    if (len == 0U)
    {
        return;
    }

    (void)HAL_UART_Transmit(&huart3, (uint8_t *)text, (uint16_t)len, 100U);
}

static void camera_app_esp32_log(const char *fmt, ...)
{
    char buf[192];
    va_list args;
    int len;

    va_start(args, fmt);
    len = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    if (len > 0)
    {
        HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
    }
}

static void camera_app_esp32_send_fp_event(uint16_t user_id)
{
    char buf[32];
    int len = snprintf(buf, sizeof(buf), "STM_FP:%u\r\n", (unsigned int)user_id);

    if (len > 0)
    {
        camera_app_esp32_send_text(buf);
    }
}

static void camera_app_esp32_send_done_event(uint16_t volume_ml)
{
    char buf[32];
    int len = snprintf(buf, sizeof(buf), "STM_DONE:%u\r\n", (unsigned int)volume_ml);

    if (len > 0)
    {
        camera_app_esp32_send_text(buf);
    }
}

static void camera_app_pump_ctrl_report_session_to_esp32(void)
{
    char buf[96];
    int len;
    uint16_t volume_ml;
    uint32_t fill_time_ms;

    fill_time_ms = camera_app_pump_ctrl_get_fill_time_ms();
    if ((fill_time_ms == 0U) || (g_pump_ctrl.esp32_done_reported != 0U))
    {
        return;
    }

    volume_ml = camera_app_pump_ctrl_calc_volume_ml(fill_time_ms);
    g_pump_ctrl.last_volume_ml = volume_ml;

    if (g_pump_ctrl.esp32_fp_reported == 0U)
    {
        camera_app_esp32_send_fp_event(g_pump_ctrl.session_user_id);
        g_pump_ctrl.esp32_fp_reported = 1U;
    }
    camera_app_esp32_send_done_event(volume_ml);
    g_pump_ctrl.esp32_done_reported = 1U;

    len = snprintf(buf, sizeof(buf),
                   "[ESP32] fp=%u volume_ml=%u fill_ms=%lu\r\n",
                   (unsigned int)g_pump_ctrl.session_user_id,
                   (unsigned int)volume_ml,
                   (unsigned long)fill_time_ms);
    if (len > 0)
    {
        HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
    }
}

static void camera_app_pump_ctrl_reset_fingerprint_session(void)
{
    g_pump_ctrl.fingerprint_session_locked = 0U;
    g_pump_ctrl.fingerprint_last_poll_ms = 0U;
    g_pump_ctrl.session_user_id = 0U;
    g_pump_ctrl.fingerprint_match_score = 0U;
    g_pump_ctrl.esp32_fp_reported = 0U;
    g_pump_ctrl.esp32_done_reported = 0U;
}

static uint8_t camera_app_finger_try_connect(void)
{
    AS608_SysPara sys_para = {0};
    uint16_t valid_count = 0U;
    uint8_t confirm;

    g_pump_ctrl.fingerprint_last_connect_ms = HAL_GetTick();
    confirm = AS608_AutoConnect(&g_finger_sensor);
    if (confirm != 0x00U)
    {
        g_pump_ctrl.fingerprint_ready = 0U;
        camera_app_finger_log("[FINGER] connect fail code=0x%02X wak=%lu\r\n",
                              (unsigned int)confirm,
                              (unsigned long)HAL_GPIO_ReadPin(AS608_WAK_GPIO_Port, AS608_WAK_Pin));
        return 1U;
    }

    g_pump_ctrl.fingerprint_ready = 1U;
    confirm = AS608_ReadSysPara(&g_finger_sensor, &sys_para);
    if (confirm == 0x00U)
    {
        (void)AS608_ValidTemplateNum(&g_finger_sensor, &valid_count);
        camera_app_finger_log("[FINGER] connect ok addr=0x%08lX baud=%lu cap=%u valid=%u wak=%lu\r\n",
                              (unsigned long)g_finger_sensor.address,
                              (unsigned long)g_finger_sensor.baudrate,
                              (unsigned int)sys_para.capacity,
                              (unsigned int)valid_count,
                              (unsigned long)HAL_GPIO_ReadPin(AS608_WAK_GPIO_Port, AS608_WAK_Pin));
    }
    else
    {
        camera_app_finger_log("[FINGER] connect ok para_fail=0x%02X baud=%lu wak=%lu\r\n",
                              (unsigned int)confirm,
                              (unsigned long)g_finger_sensor.baudrate,
                              (unsigned long)HAL_GPIO_ReadPin(AS608_WAK_GPIO_Port, AS608_WAK_Pin));
    }

    return 0U;
}

static void camera_app_finger_init(void)
{
    AS608_Init(&g_finger_sensor, &huart2);
    g_pump_ctrl.fingerprint_ready = 0U;
    g_pump_ctrl.fingerprint_last_connect_ms = 0U;
    camera_app_pump_ctrl_reset_fingerprint_session();
    (void)camera_app_finger_try_connect();
}

static void camera_app_finger_poll(uint32_t now_ms)
{
    AS608_SearchResult result = {0};
    uint8_t confirm;

    if (g_pump_ctrl.fingerprint_session_locked != 0U)
    {
        return;
    }

    if (g_pump_ctrl.fingerprint_ready == 0U)
    {
        if ((g_pump_ctrl.fingerprint_last_connect_ms == 0U) ||
            ((int32_t)(now_ms - g_pump_ctrl.fingerprint_last_connect_ms) >= (int32_t)FINGER_RECONNECT_MS))
        {
            (void)camera_app_finger_try_connect();
        }
        return;
    }

    if ((g_pump_ctrl.fingerprint_last_poll_ms != 0U) &&
        ((int32_t)(now_ms - g_pump_ctrl.fingerprint_last_poll_ms) < (int32_t)FINGER_RECOG_POLL_MS))
    {
        return;
    }
    g_pump_ctrl.fingerprint_last_poll_ms = now_ms;

    confirm = AS608_GetImage(&g_finger_sensor);
    if (confirm == 0x02U)
    {
        return;
    }
    if (confirm == 0xFFU)
    {
        g_pump_ctrl.fingerprint_ready = 0U;
        camera_app_finger_log("[FINGER] capture link_lost wak=%lu\r\n",
                              (unsigned long)HAL_GPIO_ReadPin(AS608_WAK_GPIO_Port, AS608_WAK_Pin));
        return;
    }
    if (confirm != 0x00U)
    {
        return;
    }

    confirm = AS608_GenChar(&g_finger_sensor, AS608_CHAR_BUFFER1);
    if (confirm == 0xFFU)
    {
        g_pump_ctrl.fingerprint_ready = 0U;
        camera_app_finger_log("[FINGER] feature link_lost wak=%lu\r\n",
                              (unsigned long)HAL_GPIO_ReadPin(AS608_WAK_GPIO_Port, AS608_WAK_Pin));
        return;
    }
    if (confirm != 0x00U)
    {
        return;
    }

    confirm = AS608_HighSpeedSearch(&g_finger_sensor,
                                    AS608_CHAR_BUFFER1,
                                    0U,
                                    g_finger_sensor.capacity,
                                    &result);
    if (confirm == 0xFFU)
    {
        g_pump_ctrl.fingerprint_ready = 0U;
        camera_app_finger_log("[FINGER] search link_lost wak=%lu\r\n",
                              (unsigned long)HAL_GPIO_ReadPin(AS608_WAK_GPIO_Port, AS608_WAK_Pin));
        return;
    }
    if (confirm != 0x00U)
    {
        return;
    }

    g_pump_ctrl.fingerprint_session_locked = 1U;
    g_pump_ctrl.session_user_id = result.page_id;
    g_pump_ctrl.fingerprint_match_score = result.match_score;
    camera_app_esp32_send_fp_event(result.page_id);
    g_pump_ctrl.esp32_fp_reported = 1U;
    camera_app_finger_log("[FINGER] match id=%u score=%u wak=%lu\r\n",
                          (unsigned int)result.page_id,
                          (unsigned int)result.match_score,
                          (unsigned long)HAL_GPIO_ReadPin(AS608_WAK_GPIO_Port, AS608_WAK_Pin));
    camera_app_oled_refresh_stub();
}

static void camera_app_esp32_rx_start(void)
{
}

static void camera_app_esp32_process_rx_line(const char *line)
{
    (void)line;
}

static uint8_t camera_app_esp32_play_text(const char *text)
{
    if ((text == NULL) || (text[0] == '\0'))
    {
        return 0U;
    }

    if (strcmp(text, "正在放水") == 0)
    {
        Asr_Speak(ASR_ANNOUNCER, ASR_SPEAK_DISPENSING);
        return 1U;
    }

    if (strcmp(text, "出水已完成") == 0)
    {
        Asr_Speak(ASR_ANNOUNCER, ASR_SPEAK_DONE);
        return 1U;
    }

    if (strcmp(text, "出现异常，已停水") == 0)
    {
        Asr_Speak(ASR_ANNOUNCER, ASR_SPEAK_ABORTED);
        return 1U;
    }

    if (strcmp(text, "检测到水杯") == 0)
    {
        Asr_Speak(ASR_ANNOUNCER, ASR_SPEAK_CUP_DETECTED);
        return 1U;
    }

    return 0U;
}

static void camera_app_pump_ctrl_start_remote_cold(const char *reason)
{
    if ((camera_work_state_t)g_pump_ctrl.workflow_state == CAMERA_WORK_STATE_STANDBY)
    {
        camera_app_pump_ctrl_reset_fingerprint_session();
    }

    g_pump_ctrl.auto_rearm_wait_no_cup = 0U;
    g_pump_ctrl.dispense_target = CAMERA_DISPENSE_TARGET_FULL;
    g_pump_ctrl.water_temp = CAMERA_WATER_TEMP_COLD;
    g_pump_ctrl.target_reached_count = 0U;
    memset(g_pump_ctrl.reg_hist, 0, sizeof(g_pump_ctrl.reg_hist));
    g_pump_ctrl.reg_hist_count = 0U;
    g_pump_ctrl.requested_state = PUMP_COMMAND_FAST;
    camera_app_pump_ctrl_set_work_state(CAMERA_WORK_STATE_DISPENSING_VOICE, reason);
    camera_app_pump_ctrl_apply_state(PUMP_COMMAND_FAST);
    Asr_Speak(ASR_ANNOUNCER, ASR_SPEAK_DISPENSING);
}

static void camera_app_esp32_process_pending(void)
{
    uint8_t start_pending = g_esp32_start_pending;
    uint8_t stop_pending = g_esp32_stop_pending;
    uint8_t speak_pending = g_esp32_speak_pending;
    char speak_text[ESP32_SPEAK_TEXT_SIZE];

    if (start_pending != 0U)
    {
        g_esp32_start_pending = 0U;
        if (g_camera_ready == 0U)
        {
            camera_app_esp32_log("[ESP32] start ignored vision_not_ready\r\n");
        }
        else
        {
            switch ((camera_work_state_t)g_pump_ctrl.workflow_state)
            {
                case CAMERA_WORK_STATE_STANDBY:
                case CAMERA_WORK_STATE_DECISION_WINDOW:
                case CAMERA_WORK_STATE_DISPENSING_VOICE_WAIT:
                case CAMERA_WORK_STATE_DONE:
                case CAMERA_WORK_STATE_FAULT:
                    if ((camera_work_state_t)g_pump_ctrl.workflow_state == CAMERA_WORK_STATE_DONE ||
                        (camera_work_state_t)g_pump_ctrl.workflow_state == CAMERA_WORK_STATE_FAULT)
                    {
                        camera_app_pump_ctrl_set_work_state(CAMERA_WORK_STATE_STANDBY, "app_cmd_recover");
                    }
                    camera_app_pump_ctrl_start_remote_cold("app_cmd_start");
                    break;

                default:
                    camera_app_esp32_log("[ESP32] start ignored wf=%s\r\n",
                                         camera_app_work_state_name(g_pump_ctrl.workflow_state));
                    break;
            }
        }
    }

    if (stop_pending != 0U)
    {
        g_esp32_stop_pending = 0U;
        if (camera_app_pump_ctrl_session_active() != 0U)
        {
            camera_app_pump_ctrl_return_to_standby("app_cmd_stop");
        }
        else
        {
            g_pump_ctrl.requested_state = PUMP_COMMAND_STOP;
            camera_app_pump_ctrl_apply_state(PUMP_COMMAND_STOP);
        }
    }

    if (speak_pending != 0U)
    {
        g_esp32_speak_pending = 0U;
        memcpy(speak_text, g_esp32_speak_text, sizeof(speak_text));
        speak_text[sizeof(speak_text) - 1U] = '\0';
        if (camera_app_esp32_play_text(speak_text) == 0U)
        {
            camera_app_esp32_log("[ESP32] speak unsupported=%s\r\n", speak_text);
        }
    }
}

static void camera_app_esp32_init(void)
{
    g_pump_ctrl.last_volume_ml = 0U;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if ((huart == NULL) || (huart->Instance != USART3))
    {
        return;
    }

    if (g_esp32_rx_byte == '\n')
    {
        g_esp32_rx_line[g_esp32_rx_index] = '\0';
        if (g_esp32_rx_index != 0U)
        {
            camera_app_esp32_process_rx_line(g_esp32_rx_line);
        }
        g_esp32_rx_index = 0U;
        memset(g_esp32_rx_line, 0, sizeof(g_esp32_rx_line));
    }
    else if ((g_esp32_rx_byte != '\r') && (g_esp32_rx_index + 1U < ESP32_CMD_BUF_SIZE))
    {
        g_esp32_rx_line[g_esp32_rx_index] = (char)g_esp32_rx_byte;
        g_esp32_rx_index++;
    }
    else
    {
        g_esp32_rx_index = 0U;
        memset(g_esp32_rx_line, 0, sizeof(g_esp32_rx_line));
    }

    camera_app_esp32_rx_start();
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if ((huart != NULL) && (huart->Instance == USART3))
    {
        g_esp32_rx_index = 0U;
        memset(g_esp32_rx_line, 0, sizeof(g_esp32_rx_line));
        camera_app_esp32_rx_start();
    }
}

static void camera_app_voice_apply_request(uint8_t voice_result,
                                           uint8_t *target_out,
                                           uint8_t *temp_out)
{
    uint8_t target = CAMERA_DISPENSE_TARGET_NONE;
    uint8_t temp = CAMERA_WATER_TEMP_COLD;

    switch (voice_result)
    {
        case ASR_WORD_HALF_WATER:
            target = CAMERA_DISPENSE_TARGET_HALF;
            temp = CAMERA_WATER_TEMP_COLD;
            break;

        case ASR_WORD_FULL_WATER:
            target = CAMERA_DISPENSE_TARGET_FULL;
            temp = CAMERA_WATER_TEMP_COLD;
            break;

        case ASR_WORD_HALF_HOT_WATER:
            target = CAMERA_DISPENSE_TARGET_HALF;
            temp = CAMERA_WATER_TEMP_HOT;
            break;

        case ASR_WORD_FULL_HOT_WATER:
            target = CAMERA_DISPENSE_TARGET_FULL;
            temp = CAMERA_WATER_TEMP_HOT;
            break;

        default:
            break;
    }

    if (target_out != NULL)
    {
        *target_out = target;
    }

    if (temp_out != NULL)
    {
        *temp_out = temp;
    }
}

static void camera_app_pump_ctrl_reset(void)
{
    memset(&g_pump_ctrl, 0, sizeof(g_pump_ctrl));
    g_pump_ctrl.workflow_state = CAMERA_WORK_STATE_STANDBY;
    g_pump_ctrl.water_temp = CAMERA_WATER_TEMP_COLD;
    g_pump_ctrl.key_cold_prev_down = camera_app_cold_key_is_down();
    camera_app_pump_ctrl_reset_fingerprint_session();
    Pump_Stop();
    camera_app_oled_refresh_stub();
}

static void camera_app_pump_ctrl_apply_state(uint8_t state)
{
    uint8_t next_state = state;
    char buf[128];
    int len;
    uint8_t prev_state;
    uint32_t now_ms;

    if (next_state > PUMP_COMMAND_SLOW)
    {
        next_state = PUMP_COMMAND_STOP;
    }

    if (g_pump_ctrl.applied_state == next_state)
    {
        return;
    }

    prev_state = g_pump_ctrl.applied_state;
    now_ms = HAL_GetTick();
    Pump_ApplyCommand(next_state);
    g_pump_ctrl.applied_state = next_state;
    if ((prev_state == PUMP_COMMAND_STOP) && (next_state != PUMP_COMMAND_STOP))
    {
        g_pump_ctrl.pump_run_started_ms = now_ms;
        g_pump_ctrl.pump_run_active = 1U;
    }
    else if ((prev_state != PUMP_COMMAND_STOP) && (next_state == PUMP_COMMAND_STOP) &&
             (g_pump_ctrl.pump_run_active != 0U))
    {
        g_pump_ctrl.pump_run_total_ms += (now_ms - g_pump_ctrl.pump_run_started_ms);
        g_pump_ctrl.pump_run_started_ms = 0U;
        g_pump_ctrl.pump_run_active = 0U;
    }
    g_pump_ctrl.action_count++;

    len = snprintf(buf, sizeof(buf),
                   "[PUMP] apply req=%s applied=%s duty=%u ph6=%lu actions=%lu\r\n",
                   camera_app_pump_cmd_name(g_pump_ctrl.requested_state),
                   camera_app_pump_cmd_name(g_pump_ctrl.applied_state),
                   (unsigned)Pump_GetLastDuty(),
                   (unsigned long)Pump_GetPinLevel(),
                   (unsigned long)g_pump_ctrl.action_count);
    HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
}

static void camera_app_pump_ctrl_set_work_state(uint8_t next_state, const char *reason)
{
    char buf[160];
    int len;

    if (g_pump_ctrl.workflow_state == next_state)
    {
        return;
    }

    len = snprintf(buf, sizeof(buf),
                   "[PUMP] wf %s -> %s reason=%s\r\n",
                   camera_app_work_state_name(g_pump_ctrl.workflow_state),
                   camera_app_work_state_name(next_state),
                   (reason != NULL) ? reason : "none");
    HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
    g_pump_ctrl.workflow_state = next_state;
    camera_app_oled_refresh_stub();
}

static void camera_app_pump_ctrl_clear_flow_context(void)
{
    g_pump_ctrl.dispense_target = CAMERA_DISPENSE_TARGET_NONE;
    g_pump_ctrl.water_temp = CAMERA_WATER_TEMP_COLD;
    g_pump_ctrl.stable_candidate_class = 0U;
    g_pump_ctrl.stable_candidate_count = 0U;
    g_pump_ctrl.cup_lost_count = 0U;
    g_pump_ctrl.abnormal_count = 0U;
    g_pump_ctrl.target_reached_count = 0U;
    g_pump_ctrl.decision_deadline_ms = 0U;
    g_pump_ctrl.voice_accept_after_ms = 0U;
    g_pump_ctrl.voice_command_armed = 0U;
    g_pump_ctrl.stop_request = 0U;
    memset(g_pump_ctrl.reg_hist, 0, sizeof(g_pump_ctrl.reg_hist));
    g_pump_ctrl.reg_hist_count = 0U;
    g_pump_ctrl.last_volume_ml = 0U;
    g_pump_ctrl.pump_run_active = 0U;
    g_pump_ctrl.pump_run_started_ms = 0U;
    g_pump_ctrl.pump_run_total_ms = 0U;
}

static void camera_app_pump_ctrl_cancel_pre_dispense(const char *reason)
{
    g_pump_ctrl.requested_state = PUMP_COMMAND_STOP;
    camera_app_pump_ctrl_apply_state(PUMP_COMMAND_STOP);
    g_pump_ctrl.auto_rearm_wait_no_cup = 0U;
    camera_app_pump_ctrl_clear_flow_context();
    camera_app_pump_ctrl_reset_fingerprint_session();
    camera_app_pump_ctrl_set_work_state(CAMERA_WORK_STATE_STANDBY, reason);
}

static void camera_app_pump_ctrl_return_to_standby(const char *reason)
{
    g_pump_ctrl.requested_state = PUMP_COMMAND_STOP;
    camera_app_pump_ctrl_apply_state(PUMP_COMMAND_STOP);
    camera_app_pump_ctrl_report_session_to_esp32();
    Asr_Speak(ASR_ANNOUNCER, ASR_SPEAK_DONE);
    camera_app_pump_ctrl_set_work_state(CAMERA_WORK_STATE_DONE, reason);
    camera_app_pump_ctrl_hold_oled_status(0U);
    g_pump_ctrl.auto_rearm_wait_no_cup = 1U;
    camera_app_pump_ctrl_clear_flow_context();
    camera_app_pump_ctrl_set_work_state(CAMERA_WORK_STATE_STANDBY, "ready");
}

static void camera_app_pump_ctrl_fault_to_standby(const char *reason)
{
    g_pump_ctrl.abnormal_latched = 1U;
    g_pump_ctrl.requested_state = PUMP_COMMAND_STOP;
    camera_app_pump_ctrl_apply_state(PUMP_COMMAND_STOP);
    camera_app_pump_ctrl_report_session_to_esp32();
    Asr_Speak(ASR_ANNOUNCER, ASR_SPEAK_ABORTED);
    camera_app_pump_ctrl_set_work_state(CAMERA_WORK_STATE_FAULT, reason);
    camera_app_pump_ctrl_hold_oled_status(2U);
    g_pump_ctrl.auto_rearm_wait_no_cup = 1U;
    camera_app_pump_ctrl_clear_flow_context();
    camera_app_pump_ctrl_set_work_state(CAMERA_WORK_STATE_STANDBY, "ready");
}

static void camera_app_pump_ctrl_enter_decision_window(uint32_t now_ms)
{
    uint8_t stale_voice_result;
    char buf[96];
    int len;

    stale_voice_result = camera_app_voice_result_read();
    camera_app_pump_ctrl_reset_fingerprint_session();
    g_pump_ctrl.dispense_target = CAMERA_DISPENSE_TARGET_NONE;
    g_pump_ctrl.water_temp = CAMERA_WATER_TEMP_COLD;
    g_pump_ctrl.voice_command_armed = 0U;
    camera_app_pump_ctrl_set_work_state(CAMERA_WORK_STATE_DECISION_WINDOW, "cup_stable");
    Asr_Speak(ASR_ANNOUNCER, ASR_SPEAK_CUP_DETECTED);
    now_ms = HAL_GetTick();
    g_pump_ctrl.voice_accept_after_ms = now_ms + PUMP_VOICE_PROMPT_GUARD_MS;
    g_pump_ctrl.decision_deadline_ms = g_pump_ctrl.voice_accept_after_ms + PUMP_DECISION_WINDOW_MS;

    len = snprintf(buf, sizeof(buf),
                   "[PUMP] voice_window guard_ms=%lu cmd_ms=%lu stale=%u\r\n",
                   (unsigned long)PUMP_VOICE_PROMPT_GUARD_MS,
                   (unsigned long)PUMP_DECISION_WINDOW_MS,
                   (unsigned int)stale_voice_result);
    if (len > 0)
    {
        HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
    }
}

static void camera_app_pump_ctrl_start_manual_cold(const char *reason)
{
    if ((camera_work_state_t)g_pump_ctrl.workflow_state == CAMERA_WORK_STATE_STANDBY)
    {
        camera_app_pump_ctrl_reset_fingerprint_session();
    }

    g_pump_ctrl.dispense_target = CAMERA_DISPENSE_TARGET_MANUAL_CONTINUOUS;
    g_pump_ctrl.water_temp = CAMERA_WATER_TEMP_COLD;
    g_pump_ctrl.target_reached_count = 0U;
    g_pump_ctrl.requested_state = PUMP_COMMAND_FAST;
    camera_app_pump_ctrl_set_work_state(CAMERA_WORK_STATE_DISPENSING_MANUAL_COLD, reason);
    camera_app_pump_ctrl_apply_state(PUMP_COMMAND_FAST);
}

static void camera_app_pump_ctrl_start_auto_cold(void)
{
    g_pump_ctrl.dispense_target = CAMERA_DISPENSE_TARGET_FULL;
    g_pump_ctrl.water_temp = CAMERA_WATER_TEMP_COLD;
    g_pump_ctrl.target_reached_count = 0U;
    g_pump_ctrl.cup_lost_count = 0U;
    g_pump_ctrl.abnormal_count = 0U;
    memset(g_pump_ctrl.reg_hist, 0, sizeof(g_pump_ctrl.reg_hist));
    g_pump_ctrl.reg_hist_count = 0U;
    g_pump_ctrl.requested_state = PUMP_COMMAND_FAST;
    camera_app_pump_ctrl_set_work_state(CAMERA_WORK_STATE_DISPENSING_AUTO_COLD, "timeout_auto");
    camera_app_pump_ctrl_apply_state(PUMP_COMMAND_FAST);
    Asr_Speak(ASR_ANNOUNCER, ASR_SPEAK_DISPENSING);
}

static void camera_app_pump_ctrl_start_voice_dispense(uint8_t target,
                                                      uint8_t temp,
                                                      const char *reason)
{
    if ((target != CAMERA_DISPENSE_TARGET_HALF) &&
        (target != CAMERA_DISPENSE_TARGET_FULL))
    {
        return;
    }

    g_pump_ctrl.dispense_target = target;
    g_pump_ctrl.water_temp = temp;
    g_pump_ctrl.target_reached_count = 0U;
    g_pump_ctrl.cup_lost_count = 0U;
    g_pump_ctrl.abnormal_count = 0U;
    memset(g_pump_ctrl.reg_hist, 0, sizeof(g_pump_ctrl.reg_hist));
    g_pump_ctrl.reg_hist_count = 0U;
    g_pump_ctrl.requested_state = PUMP_COMMAND_FAST;
    camera_app_pump_ctrl_set_work_state(CAMERA_WORK_STATE_DISPENSING_VOICE, reason);
    camera_app_pump_ctrl_apply_state(PUMP_COMMAND_FAST);
    Asr_Speak(ASR_ANNOUNCER, ASR_SPEAK_DISPENSING);
}

static void camera_app_pump_ctrl_start_voice_wait(uint8_t target,
                                                  uint8_t temp,
                                                  uint32_t now_ms,
                                                  const char *reason)
{
    if ((target != CAMERA_DISPENSE_TARGET_HALF) &&
        (target != CAMERA_DISPENSE_TARGET_FULL))
    {
        return;
    }

    g_pump_ctrl.dispense_target = target;
    g_pump_ctrl.water_temp = temp;
    g_pump_ctrl.target_reached_count = 0U;
    memset(g_pump_ctrl.reg_hist, 0, sizeof(g_pump_ctrl.reg_hist));
    g_pump_ctrl.reg_hist_count = 0U;
    g_pump_ctrl.decision_deadline_ms = now_ms + PUMP_VOICE_START_DELAY_MS;
    g_pump_ctrl.requested_state = PUMP_COMMAND_STOP;
    camera_app_pump_ctrl_set_work_state(CAMERA_WORK_STATE_DISPENSING_VOICE_WAIT, reason);
    camera_app_pump_ctrl_apply_state(PUMP_COMMAND_STOP);
}

static void camera_app_pump_ctrl_push_reg(float reg)
{
    if (g_pump_ctrl.reg_hist_count < PUMP_REG_WINDOW_SIZE)
    {
        g_pump_ctrl.reg_hist[g_pump_ctrl.reg_hist_count] = reg;
        g_pump_ctrl.reg_hist_count++;
        return;
    }

    memmove(&g_pump_ctrl.reg_hist[0],
            &g_pump_ctrl.reg_hist[1],
            (PUMP_REG_WINDOW_SIZE - 1U) * sizeof(g_pump_ctrl.reg_hist[0]));
    g_pump_ctrl.reg_hist[PUMP_REG_WINDOW_SIZE - 1U] = reg;
}

static uint8_t camera_app_pump_ctrl_reg_target_reached(float *avg_out)
{
    float sum = 0.0f;
    float stop_reg = 0.0f;
    float near_stop_reg = 0.0f;
    uint8_t rising = 0U;
    uint8_t over_count = 0U;
    uint8_t near_count = 0U;
    uint8_t i;
    uint8_t n = g_pump_ctrl.reg_hist_count;
    float avg;

    if (avg_out != NULL)
    {
        *avg_out = 0.0f;
    }

    if (n < PUMP_REG_MIN_FRAMES)
    {
        return 0U;
    }

    switch ((camera_dispense_target_t)g_pump_ctrl.dispense_target)
    {
        case CAMERA_DISPENSE_TARGET_HALF:
            stop_reg = PUMP_HALF_STOP_REG;
            near_stop_reg = PUMP_HALF_STOP_REG;
            break;

        case CAMERA_DISPENSE_TARGET_FULL:
            stop_reg = PUMP_FULL_STOP_REG;
            near_stop_reg = PUMP_FULL_NEAR_STOP_REG;
            break;

        case CAMERA_DISPENSE_TARGET_NONE:
        case CAMERA_DISPENSE_TARGET_MANUAL_CONTINUOUS:
        default:
            return 0U;
    }

    for (i = 0U; i < n; i++)
    {
        float sample = g_pump_ctrl.reg_hist[i];
        sum += sample;
        if (sample >= stop_reg)
        {
            over_count++;
        }
        if (sample >= near_stop_reg)
        {
            near_count++;
        }
        if ((i > 0U) && (sample + PUMP_REG_RISE_TOL >= g_pump_ctrl.reg_hist[i - 1U]))
        {
            rising++;
        }
    }

    avg = sum / (float)n;
    if (avg_out != NULL)
    {
        *avg_out = avg;
    }

    if (avg >= stop_reg)
    {
        return (rising >= (uint8_t)(n - 2U)) ? 1U : 0U;
    }

    if ((g_pump_ctrl.reg_hist[n - 1U] >= stop_reg) && (near_count >= 2U))
    {
        return 1U;
    }

    if ((stop_reg == PUMP_HALF_STOP_REG) && (over_count >= 3U))
    {
        return 1U;
    }

    return 0U;
}

static uint8_t camera_app_pump_ctrl_class_target_reached(const camera_ai_result_t *result)
{
    uint8_t target_match = 0U;

    if (result == NULL)
    {
        g_pump_ctrl.target_reached_count = 0U;
        return 0U;
    }

    if ((g_pump_ctrl.dispense_target == CAMERA_DISPENSE_TARGET_HALF) &&
        ((result->class_id == 2U) || (result->class_id == 3U)) &&
        (result->level_reg >= PUMP_HALF_CLASS_STOP_REG))
    {
        target_match = 1U;
    }
    else if ((g_pump_ctrl.dispense_target == CAMERA_DISPENSE_TARGET_FULL) &&
             (result->class_id == 3U) &&
             (result->level_reg >= PUMP_FULL_CLASS_STOP_REG))
    {
        target_match = 1U;
    }

    if (target_match != 0U)
    {
        if (g_pump_ctrl.target_reached_count < 0xFFU)
        {
            g_pump_ctrl.target_reached_count++;
        }
    }
    else
    {
        g_pump_ctrl.target_reached_count = 0U;
    }

    return (g_pump_ctrl.target_reached_count >= PUMP_TARGET_CLASS_STOP_FRAMES) ? 1U : 0U;
}

static uint8_t camera_app_pump_ctrl_is_cup_class(uint8_t class_id)
{
    return ((class_id >= 1U) && (class_id <= 3U)) ? 1U : 0U;
}

static void camera_app_pump_ctrl_update_stable_class(uint8_t class_id)
{
    if ((g_pump_ctrl.stable_candidate_class == class_id) ||
        ((camera_app_pump_ctrl_is_cup_class(g_pump_ctrl.stable_candidate_class) != 0U) &&
         (camera_app_pump_ctrl_is_cup_class(class_id) != 0U)))
    {
        g_pump_ctrl.stable_candidate_class = class_id;
        if (g_pump_ctrl.stable_candidate_count < 0xFFU)
        {
            g_pump_ctrl.stable_candidate_count++;
        }
    }
    else
    {
        g_pump_ctrl.stable_candidate_class = class_id;
        g_pump_ctrl.stable_candidate_count = 1U;
    }
}

static uint8_t camera_app_pump_ctrl_has_stable_cup(void)
{
    if (camera_app_pump_ctrl_is_cup_class(g_pump_ctrl.stable_candidate_class) == 0U)
    {
        return 0U;
    }

    return (g_pump_ctrl.stable_candidate_count >= PUMP_STABLE_FRAME_COUNT) ? 1U : 0U;
}

static uint8_t camera_app_pump_ctrl_has_stable_non_cup(void)
{
    if (camera_app_pump_ctrl_is_cup_class(g_pump_ctrl.stable_candidate_class) != 0U)
    {
        return 0U;
    }

    return (g_pump_ctrl.stable_candidate_count >= PUMP_STABLE_FRAME_COUNT) ? 1U : 0U;
}

static uint8_t camera_app_pump_ctrl_has_stable_class(uint8_t class_id)
{
    if (g_pump_ctrl.stable_candidate_class != class_id)
    {
        return 0U;
    }

    return (g_pump_ctrl.stable_candidate_count >= PUMP_STABLE_FRAME_COUNT) ? 1U : 0U;
}

static uint8_t camera_app_pump_ctrl_abnormal_guard_active(void)
{
    switch ((camera_work_state_t)g_pump_ctrl.workflow_state)
    {
        case CAMERA_WORK_STATE_DISPENSING_AUTO_COLD:
        case CAMERA_WORK_STATE_DISPENSING_VOICE:
            return 1U;

        default:
            return 0U;
    }
}

static void camera_app_pump_ctrl_update_guard_counts(const camera_ai_result_t *result)
{
    if (result == NULL)
    {
        return;
    }

    if (result->class_id == 0U)
    {
        if (g_pump_ctrl.cup_lost_count < 0xFFU)
        {
            g_pump_ctrl.cup_lost_count++;
        }
    }
    else
    {
        g_pump_ctrl.cup_lost_count = 0U;
    }

    if ((camera_app_pump_ctrl_abnormal_guard_active() != 0U) &&
        (result->class_id == 4U) &&
        (result->confidence >= PUMP_ABNORMAL_CONF_MIN))
    {
        if (g_pump_ctrl.abnormal_count < 0xFFU)
        {
            g_pump_ctrl.abnormal_count++;
        }
    }
    else
    {
        g_pump_ctrl.abnormal_count = 0U;
    }
}

static uint8_t camera_app_pump_ctrl_has_cup_lost_fault(void)
{
    return (g_pump_ctrl.cup_lost_count >= PUMP_CUP_LOST_FRAME_COUNT) ? 1U : 0U;
}

static uint8_t camera_app_pump_ctrl_has_abnormal_fault(void)
{
    return (g_pump_ctrl.abnormal_count >= PUMP_ABNORMAL_FRAME_COUNT) ? 1U : 0U;
}

static uint8_t camera_app_pump_ctrl_session_active(void)
{
    switch ((camera_work_state_t)g_pump_ctrl.workflow_state)
    {
        case CAMERA_WORK_STATE_DECISION_WINDOW:
        case CAMERA_WORK_STATE_DISPENSING_AUTO_COLD:
        case CAMERA_WORK_STATE_DISPENSING_VOICE_WAIT:
        case CAMERA_WORK_STATE_DISPENSING_VOICE:
        case CAMERA_WORK_STATE_DISPENSING_MANUAL_COLD:
            return 1U;

        case CAMERA_WORK_STATE_STANDBY:
        case CAMERA_WORK_STATE_DONE:
        case CAMERA_WORK_STATE_FAULT:
        default:
            return 0U;
    }
}

static uint8_t camera_app_pump_ctrl_cold_key_pressed(void)
{
    uint8_t key_down = camera_app_cold_key_is_down();
    uint8_t pressed = (key_down != 0U) && (g_pump_ctrl.key_cold_prev_down == 0U);

    g_pump_ctrl.key_cold_prev_down = key_down;
    return pressed;
}

static uint8_t camera_app_pump_ctrl_service_manual_fastpath(uint8_t vision_ready)
{
    uint8_t key_cold_pressed = camera_app_pump_ctrl_cold_key_pressed();

    switch ((camera_work_state_t)g_pump_ctrl.workflow_state)
    {
        case CAMERA_WORK_STATE_STANDBY:
            if (key_cold_pressed != 0U)
            {
                camera_app_pump_ctrl_start_manual_cold("manual_direct_start");
                return 1U;
            }
            break;

        case CAMERA_WORK_STATE_DECISION_WINDOW:
        case CAMERA_WORK_STATE_DISPENSING_VOICE_WAIT:
            if (key_cold_pressed != 0U)
            {
                camera_app_pump_ctrl_start_manual_cold("manual_key_override");
                return 1U;
            }
            break;

        case CAMERA_WORK_STATE_DISPENSING_MANUAL_COLD:
            g_pump_ctrl.requested_state = PUMP_COMMAND_FAST;
            if (key_cold_pressed != 0U)
            {
                camera_app_pump_ctrl_return_to_standby("cold_key_stop");
            }
            else
            {
                camera_app_pump_ctrl_apply_state(PUMP_COMMAND_FAST);
            }
            camera_app_oled_refresh_stub();
            return 1U;

        case CAMERA_WORK_STATE_DONE:
        case CAMERA_WORK_STATE_FAULT:
            g_pump_ctrl.requested_state = PUMP_COMMAND_STOP;
            camera_app_pump_ctrl_set_work_state(CAMERA_WORK_STATE_STANDBY, "recover_state");
            camera_app_oled_refresh_stub();
            return (vision_ready == 0U) ? 1U : 0U;

        default:
            break;
    }

    if (vision_ready == 0U)
    {
        g_pump_ctrl.requested_state = PUMP_COMMAND_STOP;
        camera_app_pump_ctrl_apply_state(PUMP_COMMAND_STOP);
        camera_app_oled_refresh_stub();
        return 1U;
    }

    return 0U;
}

static void camera_app_pump_ctrl_report(const camera_ai_result_t *result)
{
    char buf[224];
    int len;
    uint32_t conf_x10;
    uint32_t reg_x1000;
    uint32_t fill_time_ms;

    if (result == NULL)
    {
        return;
    }

    conf_x10 = camera_app_float_to_permille(result->confidence);
    reg_x1000 = camera_app_float_to_permille(result->level_reg);
    fill_time_ms = camera_app_pump_ctrl_get_fill_time_ms();

    len = snprintf(buf, sizeof(buf),
                   "[PUMP] cls=%lu raw_cls=%lu conf=%lu.%lu%% reg=0.%03lu wf=%s target=%s temp=%s fid=%u req=%s applied=%s duty=%u ph6=%lu fill_ms=%lu vol=%u frames=%lu actions=%lu\r\n",
                   (unsigned long)result->class_id,
                   (unsigned long)result->raw_class_id,
                   (unsigned long)(conf_x10 / 10U),
                   (unsigned long)(conf_x10 % 10U),
                   (unsigned long)reg_x1000,
                   camera_app_work_state_name(g_pump_ctrl.workflow_state),
                   camera_app_target_name(g_pump_ctrl.dispense_target),
                   camera_app_temp_name(g_pump_ctrl.water_temp),
                   (unsigned int)g_pump_ctrl.session_user_id,
                   camera_app_pump_cmd_name(g_pump_ctrl.requested_state),
                   camera_app_pump_cmd_name(g_pump_ctrl.applied_state),
                   (unsigned)Pump_GetLastDuty(),
                   (unsigned long)Pump_GetPinLevel(),
                   (unsigned long)fill_time_ms,
                   (unsigned int)camera_app_pump_ctrl_calc_volume_ml(fill_time_ms),
                   (unsigned long)g_pump_ctrl.frame_count,
                   (unsigned long)g_pump_ctrl.action_count);
    HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
}

static void camera_app_pump_ctrl_fail(const char *tag, uint32_t code, uint8_t fault_if_running)
{
    char buf[64];
    int len = snprintf(buf, sizeof(buf), "[PUMP] %s=%lu\r\n", tag, (unsigned long)code);

    HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
    if ((fault_if_running != 0U) && (g_pump_ctrl.pump_run_active != 0U))
    {
        camera_app_pump_ctrl_fault_to_standby(tag);
        return;
    }

    g_pump_ctrl.requested_state = PUMP_COMMAND_STOP;
    camera_app_pump_ctrl_apply_state(PUMP_COMMAND_STOP);
}

static void camera_app_pump_ctrl_decode_fail(uint32_t jpeg_off, uint32_t jpeg_len)
{
    char buf[192];
    uint8_t *jpeg = JPEG_Stream_GetBuf();
    int len;

    len = snprintf(buf, sizeof(buf),
                   "[PUMP] decode_err prep=%u decomp=%u wh=%ux%u jpeg=%lu head=%02X %02X %02X %02X\r\n",
                   (unsigned)jpeg_decode_last_prepare_status(),
                   (unsigned)jpeg_decode_last_decomp_status(),
                   (unsigned)jpeg_decode_last_width(),
                   (unsigned)jpeg_decode_last_height(),
                   (unsigned long)jpeg_len,
                   jpeg[jpeg_off + 0U],
                   jpeg[jpeg_off + 1U],
                   jpeg[jpeg_off + 2U],
                   jpeg[jpeg_off + 3U]);
    HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
    if ((g_camera_bad_frame_count >= CAMERA_BAD_FRAME_THRESHOLD) &&
        (g_pump_ctrl.pump_run_active != 0U))
    {
        camera_app_pump_ctrl_fault_to_standby("decode_err");
        return;
    }

    g_pump_ctrl.requested_state = PUMP_COMMAND_STOP;
    camera_app_pump_ctrl_apply_state(PUMP_COMMAND_STOP);
}

static void camera_app_pump_ctrl_consume_result(const camera_ai_result_t *result)
{
    uint8_t key_cold_pressed;
    uint8_t voice_result;
    char buf[96];
    int len;
    uint32_t now_ms;

    if (result == NULL)
    {
        return;
    }

    g_pump_ctrl.frame_count++;
    g_pump_ctrl.last_class_id = result->class_id;
    g_pump_ctrl.last_level_reg = result->level_reg;
    g_pump_ctrl.last_confidence = result->confidence;
    g_pump_ctrl.stop_request = ((result->class_id == 0U) || (result->class_id == 4U)) ? 1U : 0U;
    key_cold_pressed = camera_app_pump_ctrl_cold_key_pressed();
    voice_result = 0U;
    now_ms = HAL_GetTick();
    if (((camera_work_state_t)g_pump_ctrl.workflow_state == CAMERA_WORK_STATE_STANDBY) &&
        (g_pump_ctrl.oled_status_hold_until_ms != 0U) &&
        ((int32_t)(now_ms - g_pump_ctrl.oled_status_hold_until_ms) >= 0))
    {
        g_pump_ctrl.oled_status_hold_until_ms = 0U;
        camera_app_pump_ctrl_reset_fingerprint_session();
    }

    if (g_camera_recover_warmup_frames != 0U)
    {
        g_camera_recover_warmup_frames--;
        len = snprintf(buf, sizeof(buf),
                       "[PUMP] warmup_skip cls=%lu left=%lu\r\n",
                       (unsigned long)result->class_id,
                       (unsigned long)g_camera_recover_warmup_frames);
        HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
        return;
    }

    camera_app_pump_ctrl_update_stable_class(result->class_id);
    camera_app_pump_ctrl_update_guard_counts(result);

    switch ((camera_work_state_t)g_pump_ctrl.workflow_state)
    {
        case CAMERA_WORK_STATE_STANDBY:
            g_pump_ctrl.requested_state = PUMP_COMMAND_STOP;
            if (key_cold_pressed != 0U)
            {
                camera_app_pump_ctrl_start_manual_cold("manual_direct_start");
                break;
            }

            if (g_pump_ctrl.auto_rearm_wait_no_cup != 0U)
            {
                if (camera_app_pump_ctrl_has_stable_non_cup() != 0U)
                {
                    g_pump_ctrl.auto_rearm_wait_no_cup = 0U;
                }
                break;
            }

            if (camera_app_pump_ctrl_has_stable_cup() != 0U)
            {
                camera_app_pump_ctrl_enter_decision_window(now_ms);
            }
            break;

        case CAMERA_WORK_STATE_DECISION_WINDOW:
            g_pump_ctrl.requested_state = PUMP_COMMAND_STOP;
            if (key_cold_pressed != 0U)
            {
                camera_app_pump_ctrl_start_manual_cold("manual_key_override");
                break;
            }

            voice_result = camera_app_voice_result_read();
            now_ms = HAL_GetTick();
            if ((int32_t)(now_ms - g_pump_ctrl.voice_accept_after_ms) < 0)
            {
                g_pump_ctrl.voice_command_armed = 0U;
                camera_app_finger_poll(now_ms);
                break;
            }

            if (g_pump_ctrl.voice_command_armed == 0U)
            {
                if (voice_result == 0U)
                {
                    g_pump_ctrl.voice_command_armed = 1U;
                }
            }
            else if (camera_app_voice_result_is_supported(voice_result) != 0U)
            {
                uint8_t voice_target = CAMERA_DISPENSE_TARGET_NONE;
                uint8_t voice_temp = CAMERA_WATER_TEMP_COLD;

                camera_app_voice_apply_request(voice_result, &voice_target, &voice_temp);
                camera_app_pump_ctrl_start_voice_wait(voice_target,
                                                      voice_temp,
                                                      HAL_GetTick(),
                                                      "voice_cmd");
                break;
            }

            camera_app_finger_poll(now_ms);
            now_ms = HAL_GetTick();
            if ((int32_t)(now_ms - g_pump_ctrl.decision_deadline_ms) >= 0)
            {
                if (camera_app_pump_ctrl_has_stable_cup() != 0U)
                {
                    camera_app_pump_ctrl_start_auto_cold();
                }
                else if (camera_app_pump_ctrl_has_stable_non_cup() != 0U)
                {
                    camera_app_pump_ctrl_cancel_pre_dispense("cup_removed_before_dispense");
                }
            }
            break;

        case CAMERA_WORK_STATE_DISPENSING_VOICE_WAIT:
            g_pump_ctrl.requested_state = PUMP_COMMAND_STOP;
            if (key_cold_pressed != 0U)
            {
                camera_app_pump_ctrl_start_manual_cold("manual_key_override");
                break;
            }
            if ((int32_t)(now_ms - g_pump_ctrl.decision_deadline_ms) >= 0)
            {
                if (camera_app_pump_ctrl_has_stable_cup() != 0U)
                {
                    camera_app_pump_ctrl_start_voice_dispense(g_pump_ctrl.dispense_target,
                                                              g_pump_ctrl.water_temp,
                                                              "voice_delay_done");
                }
                else if (camera_app_pump_ctrl_has_stable_non_cup() != 0U)
                {
                    camera_app_pump_ctrl_cancel_pre_dispense("cup_removed_before_dispense");
                }
            }
            break;

        case CAMERA_WORK_STATE_DISPENSING_AUTO_COLD:
        case CAMERA_WORK_STATE_DISPENSING_VOICE:
        {
            float reg_avg = 0.0f;
            g_pump_ctrl.requested_state = PUMP_COMMAND_FAST;
            if (camera_app_pump_ctrl_has_cup_lost_fault() != 0U)
            {
                camera_app_pump_ctrl_fault_to_standby("cup_lost");
                break;
            }
            if (camera_app_pump_ctrl_has_abnormal_fault() != 0U)
            {
                g_pump_ctrl.abnormal_latched = 1U;
                camera_app_pump_ctrl_fault_to_standby("abnormal");
                break;
            }

            camera_app_pump_ctrl_push_reg(result->level_reg);

            if (camera_app_pump_ctrl_reg_target_reached(&reg_avg) != 0U)
            {
                camera_app_pump_ctrl_return_to_standby("target_reg");
                break;
            }

            if (camera_app_pump_ctrl_class_target_reached(result) != 0U)
            {
                camera_app_pump_ctrl_return_to_standby("target_cls");
                break;
            }

            camera_app_pump_ctrl_apply_state(PUMP_COMMAND_FAST);
            break;
        }

        case CAMERA_WORK_STATE_DISPENSING_MANUAL_COLD:
            g_pump_ctrl.requested_state = PUMP_COMMAND_FAST;
            if (key_cold_pressed != 0U)
            {
                camera_app_pump_ctrl_return_to_standby("cold_key_stop");
                break;
            }

            camera_app_pump_ctrl_apply_state(PUMP_COMMAND_FAST);
            break;

        case CAMERA_WORK_STATE_DONE:
        case CAMERA_WORK_STATE_FAULT:
        default:
            camera_app_pump_ctrl_set_work_state(CAMERA_WORK_STATE_STANDBY, "recover_state");
            g_pump_ctrl.requested_state = PUMP_COMMAND_STOP;
            break;
      }

      camera_app_oled_refresh_stub();
      camera_app_pump_ctrl_report(result);
  }

#if (PUMP_TIMED_DEMO_MODE != 0U)
static uint8_t camera_app_timed_demo_key_is_down(void)
{
    return (HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_6) == GPIO_PIN_RESET) ? 1U : 0U;
}

static uint8_t camera_app_timed_demo_key_pressed(uint32_t now_ms)
{
    uint8_t key_down = camera_app_timed_demo_key_is_down();
    uint8_t pressed = 0U;

    if ((key_down != 0U) && (g_timed_demo_key_prev_down == 0U) &&
        ((now_ms - g_timed_demo_key_last_ms) >= PUMP_TIMED_DEMO_KEY_MS))
    {
        pressed = 1U;
        g_timed_demo_key_last_ms = now_ms;
    }

    g_timed_demo_key_prev_down = key_down;
    return pressed;
}

static void camera_app_timed_demo_init(void)
{
    GPIO_InitTypeDef gpio_init = {0};

    __HAL_RCC_GPIOF_CLK_ENABLE();
    gpio_init.Pin = GPIO_PIN_6;
    gpio_init.Mode = GPIO_MODE_INPUT;
    gpio_init.Pull = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOF, &gpio_init);

    g_timed_demo_cup_present = 0U;
    g_timed_demo_key_prev_down = camera_app_timed_demo_key_is_down();
    g_timed_demo_key_last_ms = HAL_GetTick();
}

static void camera_app_timed_demo_make_result(camera_ai_result_t *result)
{
    uint32_t fill_ms;
    uint32_t target_ms;
    float progress;

    memset(result, 0, sizeof(*result));
    result->confidence = 0.92f;

    if (g_timed_demo_cup_present == 0U)
    {
        result->class_id = 0U;
        result->raw_class_id = 0U;
        result->confidence = 0.99f;
        result->level_reg = 0.01f;
        return;
    }

    if (g_pump_ctrl.pump_run_active == 0U)
    {
        result->class_id = 1U;
        result->raw_class_id = 1U;
        result->level_reg = 0.20f;
        return;
    }

    fill_ms = camera_app_pump_ctrl_get_fill_time_ms();
    target_ms = (g_pump_ctrl.dispense_target == CAMERA_DISPENSE_TARGET_HALF) ?
                PUMP_TIMED_DEMO_HALF_MS : PUMP_FLOW_CAL_TIME_MS;
    progress = (target_ms == 0U) ? 0.0f : ((float)fill_ms / (float)target_ms);
    if (progress > 1.0f)
    {
        progress = 1.0f;
    }

    if (g_pump_ctrl.dispense_target == CAMERA_DISPENSE_TARGET_HALF)
    {
        result->class_id = (progress < 0.45f) ? 1U : 2U;
        result->level_reg = 0.22f + (progress * 0.16f);
    }
    else
    {
        if (progress < 0.35f)
        {
            result->class_id = 1U;
        }
        else if (progress < 0.85f)
        {
            result->class_id = 2U;
        }
        else
        {
            result->class_id = 3U;
        }
        result->level_reg = 0.22f + (progress * 0.42f);
    }

    result->raw_class_id = result->class_id;
    result->confidence = 0.86f + (progress * 0.10f);
}

static void camera_app_timed_demo_run(void)
{
    camera_ai_result_t result;
    camera_work_state_t state;
    uint32_t now_ms = HAL_GetTick();
    uint32_t fill_ms;
    uint32_t target_ms;

    state = (camera_work_state_t)g_pump_ctrl.workflow_state;
    if (camera_app_timed_demo_key_pressed(now_ms) != 0U)
    {
        if ((state == CAMERA_WORK_STATE_DISPENSING_AUTO_COLD) ||
            (state == CAMERA_WORK_STATE_DISPENSING_VOICE))
        {
            camera_app_log("[DEMO] abnormal trigger=PF6\r\n");
            camera_app_pump_ctrl_fault_to_standby("timed_demo_abnormal");
        }
        else
        {
            g_timed_demo_cup_present ^= 1U;
            camera_app_log((g_timed_demo_cup_present != 0U) ?
                           "[DEMO] cup=present trigger=PF6\r\n" :
                           "[DEMO] cup=removed trigger=PF6\r\n");
        }
    }

    state = (camera_work_state_t)g_pump_ctrl.workflow_state;
    if ((state == CAMERA_WORK_STATE_DISPENSING_AUTO_COLD) ||
        (state == CAMERA_WORK_STATE_DISPENSING_VOICE) ||
        (state == CAMERA_WORK_STATE_DISPENSING_MANUAL_COLD))
    {
        fill_ms = camera_app_pump_ctrl_get_fill_time_ms();
        target_ms = (g_pump_ctrl.dispense_target == CAMERA_DISPENSE_TARGET_HALF) ?
                    PUMP_TIMED_DEMO_HALF_MS : PUMP_FLOW_CAL_TIME_MS;
        if (fill_ms >= target_ms)
        {
            camera_app_timed_demo_make_result(&result);
            result.class_id = (g_pump_ctrl.dispense_target == CAMERA_DISPENSE_TARGET_HALF) ? 2U : 3U;
            result.raw_class_id = result.class_id;
            result.level_reg = (result.class_id == 2U) ? 0.50f : 0.80f;
            result.confidence = 0.96f;
            g_pump_ctrl.last_class_id = result.class_id;
            g_pump_ctrl.last_level_reg = result.level_reg;
            g_pump_ctrl.last_confidence = result.confidence;
            camera_app_pump_ctrl_return_to_standby("timed_demo_target");
            camera_app_oled_refresh_stub();
            camera_app_pump_ctrl_report(&result);
            return;
        }
    }

    camera_app_timed_demo_make_result(&result);
    camera_app_pump_ctrl_consume_result(&result);
}
#endif
#endif

#if (ESP32_LINK_SELFTEST_ON_BOOT != 0U)
static void camera_app_esp32_link_selftest_poll(void)
{
    static uint32_t s_last_ms = 0U;
    static uint32_t s_count = 0U;
    static const uint8_t hello_msg[] = "STM_HELLO:115200\r\n";
    static const uint8_t fp_msg[] = "STM_FP:1\r\n";
    static const uint8_t done_msg[] = "STM_DONE:320\r\n";
    uint32_t now_ms = HAL_GetTick();

    if (s_count >= ESP32_LINK_SELFTEST_MAX_COUNT)
    {
        return;
    }

    if ((now_ms - s_last_ms) < ESP32_LINK_SELFTEST_PERIOD_MS)
    {
        return;
    }
    s_last_ms = now_ms;
    s_count++;

    (void)HAL_UART_Transmit(&huart3, (uint8_t *)hello_msg,
                            (uint16_t)(sizeof(hello_msg) - 1U), 100U);
    HAL_Delay(5U);
    (void)HAL_UART_Transmit(&huart3, (uint8_t *)fp_msg,
                            (uint16_t)(sizeof(fp_msg) - 1U), 100U);
    HAL_Delay(5U);
    (void)HAL_UART_Transmit(&huart3, (uint8_t *)done_msg,
                            (uint16_t)(sizeof(done_msg) - 1U), 100U);
#if (APP_MODE_STREAM_SILENT == 0U)
    camera_app_log("[ESP32] selftest sent hello fp=1 done=320\r\n");
#endif
}
#else
static void camera_app_esp32_link_selftest_poll(void)
{
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
#elif (APP_MODE == APP_MODE_AI_VISUAL)
    camera_app_log("[APP] CameraApp_Init enter\r\n");
    camera_app_log("[APP] mode=AI_VISUAL\r\n");
#elif (APP_MODE == APP_MODE_AI_TEST_IMAGE)
    camera_app_log("[APP] CameraApp_Init enter\r\n");
    camera_app_log("[APP] mode=AI_TEST_IMAGE\r\n");
#elif (APP_MODE == APP_MODE_JPEG_DIAG)
    camera_app_log("[APP] CameraApp_Init enter\r\n");
    camera_app_log("[APP] mode=JPEG_DIAG\r\n");
#elif (APP_MODE == APP_MODE_COLORBAR_VIEW)
    camera_app_log("[APP] CameraApp_Init enter\r\n");
    camera_app_log("[APP] mode=COLORBAR_VIEW\r\n");
#elif (APP_MODE == APP_MODE_PUMP_CTRL)
    camera_app_log("[APP] CameraApp_Init enter\r\n");
    camera_app_log("[APP] mode=PUMP_CTRL\r\n");
#else
    camera_app_log("[APP] CameraApp_Init enter\r\n");
    camera_app_log("[APP] mode=XCAM_VIEW\r\n");
#endif

#if (CAMERA_JPEG_DIAG != 0U)
    {
        static const char msg[] = "[JPEG] diag mode on\r\n";
        camera_app_text_tx(msg, (uint16_t)(sizeof(msg) - 1U));
    }
#endif

#if (APP_MODE == APP_MODE_PUMP_CTRL)
    Pump_Init();
    camera_app_log("[APP] pump init ok\r\n");
    camera_app_pump_ctrl_reset();
    camera_app_finger_init();
    camera_app_esp32_init();
#if (PUMP_TIMED_DEMO_MODE != 0U)
    camera_app_timed_demo_init();
    g_camera_ready = 1U;
    camera_app_log("[APP] mode=TIMED_DEMO camera_ai=bypassed\r\n");
    camera_app_log("[APP] PF6=cup/abnormal PF7=manual half_ms=8651 full_ms=17301\r\n");
    camera_app_log("[APP] CameraApp_Init done\r\n");
    return;
#endif
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
            camera_app_text_tx(buf, (uint16_t)len);
#if (APP_MODE == APP_MODE_PUMP_CTRL)
            camera_app_log("[APP] vision disabled, manual mode only\r\n");
#endif
#if (CAMERA_JPEG_DIAG != 0U)
            {
                static const char msg[] = "[JPEG] probe fail\r\n";
                camera_app_text_tx(msg, (uint16_t)(sizeof(msg) - 1U));
            }
#endif
            dbg_print("[APP] Probe FAILED!\r\n");
            return;
        }

        {
            char buf[64];
            int len = snprintf(buf, sizeof(buf), "[APP] probe ok mid=0x%04X pid=0x%04X\r\n", mid, pid);
            camera_app_text_tx(buf, (uint16_t)len);
        }
    }

#if (CAMERA_APP_DEBUG != 0U)
    {
        char buf[80];
        int len;
    len = snprintf(buf, sizeof(buf),
                   "[APP] Probe OK MID=0x%04X PID=0x%04X\r\n", mid, pid);
    camera_app_text_tx(buf, (uint16_t)len);
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
                camera_app_text_tx(msg, (uint16_t)(sizeof(msg) - 1U));
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
                camera_app_text_tx(msg, (uint16_t)(sizeof(msg) - 1U));
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
                camera_app_text_tx(msg, (uint16_t)(sizeof(msg) - 1U));
            }
#endif
            dbg_print("[APP] Output size FAILED!\r\n");
            return;
        }
        camera_app_log("[APP] size ok\r\n");
    }

    if (APP_MODE != APP_MODE_AI_TEST_IMAGE)
    {
        if (camera_app_reinit_dcmi_profile(g_camera_dcmi_profile_idx) != 0U)
        {
            camera_app_log("[APP] dcmi cfg fail\r\n");
            return;
        }

        JPEG_Stream_Init();
        OV2640_AttachFrameBuffer(JPEG_Stream_GetBuf(), JPEG_Stream_GetMaxSize());

        HAL_NVIC_SetPriority(DCMI_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(DCMI_IRQn);

        HAL_Delay(1500U);
    }

    if (APP_MODE != APP_MODE_AI_TEST_IMAGE)
    {
        if (camera_app_startup_validate("startup_boot") != 0U)
        {
            camera_app_log("[APP] startup check warn\r\n");
            g_camera_bad_frame_count = CAMERA_BAD_FRAME_THRESHOLD;
        }
    }

#if (APP_MODE == APP_MODE_COLORBAR_VIEW)
    ov2640_colorbar_enable();
    camera_app_log("[APP] colorbar on\r\n");
#endif

#if ((APP_MODE == APP_MODE_AI_INFER) || (APP_MODE == APP_MODE_AI_TEST_IMAGE) || (APP_MODE == APP_MODE_PUMP_CTRL) || (APP_MODE == APP_MODE_AI_VISUAL))
#if ((APP_MODE == APP_MODE_AI_INFER) || (APP_MODE == APP_MODE_PUMP_CTRL) || (APP_MODE == APP_MODE_AI_VISUAL))
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
#else
        camera_app_log("[APP] ai init fail\r\n");
#endif
        return;
    }
#if ((APP_MODE == APP_MODE_AI_INFER) || (APP_MODE == APP_MODE_AI_VISUAL))
    camera_app_log("[APP] ai init ok\r\n");
#elif (APP_MODE == APP_MODE_PUMP_CTRL)
    camera_app_pump_ctrl_reset();
    camera_app_log("[APP] ai init ok\r\n");
#endif
#endif

    g_camera_ready = 1U;
#if (APP_MODE != APP_MODE_PUMP_CTRL)
    camera_app_log("[APP] CameraApp_Init done\r\n");
#else
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
    camera_app_esp32_link_selftest_poll();

#if (APP_MODE == APP_MODE_PUMP_CTRL)
#if (PUMP_TIMED_DEMO_MODE != 0U)
    camera_app_timed_demo_run();
    HAL_Delay(PUMP_TIMED_DEMO_STEP_MS);
    return;
#endif
    if (camera_app_pump_ctrl_service_manual_fastpath(g_camera_ready) != 0U)
    {
        HAL_Delay(20U);
        return;
    }
#endif

    if (g_camera_ready == 0U)
    {
        HAL_Delay(100U);
        return;
    }

#if ((APP_MODE == APP_MODE_XCAM_VIEW) || (APP_MODE == APP_MODE_COLORBAR_VIEW))
    uint32_t jpeg_len;
    uint32_t soi_off;

    if (camera_app_capture_snapshot(1000U) != 0U)
    {
        g_camera_bad_frame_count++;
        if (g_camera_bad_frame_count >= CAMERA_BAD_FRAME_THRESHOLD)
        {
            (void)camera_app_recover_camera("xcam_capture");
        }
#if (CAMERA_JPEG_DIAG != 0U)
        camera_app_dump_nojpeg_diag("start");
#endif
        return;
    }

    jpeg_len = JPEG_Stream_FindFrame(&soi_off);
    if (jpeg_len == 0U)
    {
        g_camera_bad_frame_count++;
        if (g_camera_bad_frame_count >= CAMERA_BAD_FRAME_THRESHOLD)
        {
            (void)camera_app_recover_camera("xcam_noframe");
        }
#if (CAMERA_JPEG_DIAG != 0U)
        camera_app_dump_nojpeg_diag("noframe");
#endif
        return;
    }

    if ((soi_off + jpeg_len) > JPEG_Stream_GetMaxSize())
    {
        g_camera_bad_frame_count++;
        if (g_camera_bad_frame_count >= CAMERA_BAD_FRAME_THRESHOLD)
        {
            (void)camera_app_recover_camera("xcam_range");
        }
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
        g_camera_bad_frame_count++;
        if (g_camera_bad_frame_count >= CAMERA_BAD_FRAME_THRESHOLD)
        {
            (void)camera_app_recover_camera("xcam_marker");
        }
#if (CAMERA_JPEG_DIAG != 0U)
        camera_app_dump_nojpeg_diag("marker");
#endif
        return;
    }

    g_camera_bad_frame_count = 0U;

#if (CAMERA_JPEG_DIAG != 0U)
    camera_app_dump_jpeg_diag(soi_off, jpeg_len);
#else
    HAL_UART_Transmit(&huart1,
                      JPEG_Stream_GetBuf() + soi_off,
                      (uint16_t)jpeg_len,
                      HAL_MAX_DELAY);
#endif
#elif (APP_MODE == APP_MODE_JPEG_DIAG)
    {
        static uint32_t cfg_idx = 0U;
        uint32_t jpeg_off = 0U;
        uint32_t jpeg_len = 0U;
        uint8_t status;
        char msg[128];
        int len;

        status = camera_app_set_dcmi_diag_cfg(cfg_idx);
        if (status != 0U)
        {
            len = snprintf(msg, sizeof(msg),
                           "[JDIAG] cfg=%s reinit_err=%lu\r\n",
                           k_dcmi_diag_cfgs[cfg_idx].tag,
                           (unsigned long)status);
            HAL_UART_Transmit(&huart1, (uint8_t *)msg, (uint16_t)len, HAL_MAX_DELAY);
            cfg_idx = (cfg_idx + 1U) % (sizeof(k_dcmi_diag_cfgs) / sizeof(k_dcmi_diag_cfgs[0]));
            HAL_Delay(100U);
            return;
        }

        status = camera_app_capture_jpeg_snapshot(1000U, &jpeg_off, &jpeg_len);
        if (status != 0U)
        {
            len = snprintf(msg, sizeof(msg),
                           "[JDIAG] cfg=%s capture_err=%lu ndtr=%lu\r\n",
                           k_dcmi_diag_cfgs[cfg_idx].tag,
                           (unsigned long)status,
                           (unsigned long)camera_app_dma_get_ndtr());
            HAL_UART_Transmit(&huart1, (uint8_t *)msg, (uint16_t)len, HAL_MAX_DELAY);
            cfg_idx = (cfg_idx + 1U) % (sizeof(k_dcmi_diag_cfgs) / sizeof(k_dcmi_diag_cfgs[0]));
            HAL_Delay(100U);
            return;
        }

        len = snprintf(msg, sizeof(msg), "[JDIAG] cfg=%s\r\n", k_dcmi_diag_cfgs[cfg_idx].tag);
        HAL_UART_Transmit(&huart1, (uint8_t *)msg, (uint16_t)len, HAL_MAX_DELAY);
        camera_app_report_jpeg_diag();
        cfg_idx = (cfg_idx + 1U) % (sizeof(k_dcmi_diag_cfgs) / sizeof(k_dcmi_diag_cfgs[0]));
        (void)jpeg_off;
        (void)jpeg_len;
    }
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
            g_camera_bad_frame_count++;
            if (g_camera_bad_frame_count >= CAMERA_BAD_FRAME_THRESHOLD)
            {
                (void)camera_app_recover_camera("ai_capture");
            }
            HAL_Delay(100U);
            return;
        }

        if (camera_app_jpeg_header_is_plausible(JPEG_Stream_GetBuf() + jpeg_off, jpeg_len) == 0U)
        {
            char buf[160];
            int len = snprintf(buf, sizeof(buf),
                               "[AI] jpeg_bad len=%lu head=%02X %02X %02X %02X\r\n",
                               (unsigned long)jpeg_len,
                               JPEG_Stream_GetBuf()[jpeg_off + 0U],
                               JPEG_Stream_GetBuf()[jpeg_off + 1U],
                               JPEG_Stream_GetBuf()[jpeg_off + 2U],
                               JPEG_Stream_GetBuf()[jpeg_off + 3U]);
            HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
            g_camera_bad_frame_count++;
            if (g_camera_bad_frame_count >= CAMERA_BAD_FRAME_THRESHOLD)
            {
                (void)camera_app_recover_camera("ai_jpeg");
            }
            HAL_Delay(100U);
            return;
        }

#if (CAMERA_AI_VERBOSE_LOG != 0U)
        camera_app_log("[AI] capture ok\r\n");
#endif
        if (jpeg_to_ai_input(JPEG_Stream_GetBuf() + jpeg_off, jpeg_len, camera_app_ai_input_data()) != 0U)
        {
            char buf[160];
            int len = snprintf(buf, sizeof(buf),
                               "[AI] decode fail prep=%u decomp=%u wh=%ux%u jpeg=%lu head=%02X %02X %02X %02X\r\n",
                               (unsigned)jpeg_decode_last_prepare_status(),
                               (unsigned)jpeg_decode_last_decomp_status(),
                               (unsigned)jpeg_decode_last_width(),
                               (unsigned)jpeg_decode_last_height(),
                               (unsigned long)jpeg_len,
                               JPEG_Stream_GetBuf()[jpeg_off + 0U],
                               JPEG_Stream_GetBuf()[jpeg_off + 1U],
                               JPEG_Stream_GetBuf()[jpeg_off + 2U],
                               JPEG_Stream_GetBuf()[jpeg_off + 3U]);
            HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
            g_camera_bad_frame_count++;
            if (g_camera_bad_frame_count >= CAMERA_BAD_FRAME_THRESHOLD)
            {
                (void)camera_app_recover_camera("ai_decode");
            }
            HAL_Delay(100U);
            return;
        }
        g_camera_bad_frame_count = 0U;
#if (CAMERA_AI_VERBOSE_LOG != 0U)
        camera_app_log("[AI] prep ok\r\n");
#endif
#if (CAMERA_AI_DUMP_INPUT_ONCE != 0U)
        camera_app_dump_ai_input_once(camera_app_ai_input_data());
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
#elif (APP_MODE == APP_MODE_AI_VISUAL)
    {
        camera_ai_result_t result;
        uint32_t pipeline_start = HAL_GetTick();
        uint32_t infer_start;
        uint32_t infer_ms;
        uint32_t jpeg_off = 0U;
        uint32_t jpeg_len = 0U;
        uint8_t status;

        status = camera_app_capture_jpeg_snapshot(3000U, &jpeg_off, &jpeg_len);
        if (status != 0U)
        {
            g_camera_bad_frame_count++;
            if (g_camera_bad_frame_count >= CAMERA_BAD_FRAME_THRESHOLD)
            {
                (void)camera_app_recover_camera("visual_capture");
            }
            HAL_Delay(50U);
            return;
        }

        if (camera_app_jpeg_header_is_plausible(JPEG_Stream_GetBuf() + jpeg_off, jpeg_len) == 0U)
        {
            g_camera_bad_frame_count++;
            if (g_camera_bad_frame_count >= CAMERA_BAD_FRAME_THRESHOLD)
            {
                (void)camera_app_recover_camera("visual_jpeg");
            }
            HAL_Delay(50U);
            return;
        }

        if (jpeg_to_ai_input(JPEG_Stream_GetBuf() + jpeg_off, jpeg_len, camera_app_ai_input_data()) != 0U)
        {
            g_camera_bad_frame_count++;
            if (g_camera_bad_frame_count >= CAMERA_BAD_FRAME_THRESHOLD)
            {
                (void)camera_app_recover_camera("visual_decode");
            }
            HAL_Delay(50U);
            return;
        }

        g_camera_bad_frame_count = 0U;

        infer_start = HAL_GetTick();
        if (camera_app_ai_run(&result) != 0U)
        {
            HAL_Delay(50U);
            return;
        }
        infer_ms = HAL_GetTick() - infer_start;

        #if (CAMERA_AI_VISUAL_SEND_GRAY_ONLY != 0U)
        camera_app_ai_visual_send_gray_frame(&result,
                                             HAL_GetTick() - pipeline_start,
                                             infer_ms,
                                             camera_app_ai_input_data());
        #else
        camera_app_ai_visual_send_frame(&result,
                                        HAL_GetTick() - pipeline_start,
                                        infer_ms,
                                        JPEG_Stream_GetBuf() + jpeg_off,
                                        jpeg_len);
        #endif
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
        camera_app_load_test_image(camera_app_ai_input_data());
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

        if ((g_pump_ctrl.pump_run_active != 0U) &&
            ((HAL_GetTick() - g_pump_ctrl.pump_run_started_ms) < PUMP_CAMERA_START_SETTLE_MS))
        {
            HAL_Delay(20U);
            return;
        }

        status = camera_app_capture_jpeg_snapshot(3000U, &jpeg_off, &jpeg_len);
        if (status != 0U)
        {
            g_camera_bad_frame_count++;
            camera_app_pump_ctrl_fail("capture_err", status,
                                      (g_camera_bad_frame_count >= CAMERA_BAD_FRAME_THRESHOLD) ? 1U : 0U);
            if (g_camera_bad_frame_count >= CAMERA_BAD_FRAME_THRESHOLD)
            {
                (void)camera_app_recover_camera("pump_capture");
            }
            HAL_Delay(100U);
            return;
        }

        if (camera_app_jpeg_header_is_plausible(JPEG_Stream_GetBuf() + jpeg_off, jpeg_len) == 0U)
        {
            g_camera_bad_frame_count++;
            camera_app_pump_ctrl_decode_fail(jpeg_off, jpeg_len);
            if (g_camera_bad_frame_count >= CAMERA_BAD_FRAME_THRESHOLD)
            {
                (void)camera_app_recover_camera("pump_jpeg");
            }
            HAL_Delay(100U);
            return;
        }

        if (jpeg_to_ai_input(JPEG_Stream_GetBuf() + jpeg_off, jpeg_len, camera_app_ai_input_data()) != 0U)
        {
            g_camera_bad_frame_count++;
            camera_app_pump_ctrl_decode_fail(jpeg_off, jpeg_len);
            if (g_camera_bad_frame_count >= CAMERA_BAD_FRAME_THRESHOLD)
            {
                (void)camera_app_recover_camera("pump_decode");
            }
            HAL_Delay(100U);
            return;
        }
        g_camera_bad_frame_count = 0U;

        if (camera_app_ai_run(&result) != 0U)
        {
            camera_app_pump_ctrl_fail("infer_err", 1U, 1U);
            HAL_Delay(100U);
            return;
        }

        camera_app_pump_ctrl_consume_result(&result);
    }
#endif
}
