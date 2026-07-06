#include "ov2640.h"
#include "ov2640_sccb.h"
#include "dcmi.h"
#include "main.h"
#include "gpio.h"

/*
 * OV2640 底层驱动 (最简版)
 */

#define OV2640_VIDEO_WIDTH               320U
#define OV2640_VIDEO_HEIGHT              240U
#define OV2640_VIDEO_JPEG_QUALITY        0x03U
#define OV2640_VIDEO_SENSOR_CLK_DIV      0x00U
#define OV2640_VIDEO_DVP_PCLK_DIV        0x02U
#define OV2640_EXPECTED_MID              0x7FA2U
#define OV2640_EXPECTED_PID              0x2642U
#define OV2640_PWRDN_ASSERT_MS           30U
#define OV2640_PWR_STABLE_MS             80U
#define OV2640_RESET_ASSERT_MS           30U
#define OV2640_RESET_RELEASE_MS          80U
#define OV2640_POST_RESET_MS             150U
#define OV2640_PROBE_ATTEMPTS            3U

static uint8_t s_initialized = 0U;
static uint8_t *s_frame_buf = NULL;
static uint32_t s_frame_buf_len = 0U;

static const uint8_t ov2640_init_common_cfg[][2] = {
    {0xFF, 0x00}, {0x2C, 0xFF}, {0x2E, 0xDF}, {0xFF, 0x01}, {0x3C, 0x32},
    {0x11, OV2640_VIDEO_SENSOR_CLK_DIV}, {0x09, 0x02}, {0x04, 0xA8}, {0x13, 0xE5},
    {0x14, 0x48}, {0x2C, 0x0C}, {0x33, 0x78}, {0x3A, 0x33}, {0x3B, 0xFB},
    {0x3E, 0x00}, {0x43, 0x11}, {0x16, 0x10}, {0x39, 0x92}, {0x35, 0xDA},
    {0x22, 0x1A}, {0x37, 0xC3}, {0x23, 0x00}, {0x34, 0xC0}, {0x36, 0x1A},
    {0x06, 0x88}, {0x07, 0xC0}, {0x0D, 0x87}, {0x0E, 0x41}, {0x4C, 0x00},
    {0x48, 0x00}, {0x5B, 0x00}, {0x42, 0x03}, {0x4A, 0x81}, {0x21, 0x99},
    {0x24, 0x40}, {0x25, 0x38}, {0x26, 0x82}, {0x5C, 0x00}, {0x63, 0x00},
    {0x46, 0x00}, {0x0C, 0x3C}, {0x61, 0x70}, {0x62, 0x80}, {0x7C, 0x05},
    {0x20, 0x80}, {0x28, 0x30}, {0x6C, 0x00}, {0x6D, 0x80}, {0x6E, 0x00},
    {0x70, 0x02}, {0x71, 0x94}, {0x73, 0xC1}, {0x3D, 0x34}, {0x5A, 0x57},
    {0x12, 0x00}, {0x17, 0x11}, {0x18, 0x75}, {0x19, 0x01}, {0x1A, 0x97},
    {0x32, 0x36}, {0x03, 0x0F}, {0x37, 0x40}, {0x4F, 0xCA}, {0x50, 0xA8},
    {0x5A, 0x23}, {0x6D, 0x00}, {0x6D, 0x38}, {0xFF, 0x00}, {0xE5, 0x7F},
    {0xF9, 0xC0}, {0x41, 0x24}, {0xE0, 0x14}, {0x76, 0xFF}, {0x33, 0xA0},
    {0x42, 0x20}, {0x43, 0x18}, {0x4C, 0x00}, {0x87, 0xD5}, {0x88, 0x3F},
    {0xD7, 0x03}, {0xD9, 0x10}, {0xD3, 0x82}, {0xC8, 0x08}, {0xC9, 0x80},
    {0x7C, 0x00}, {0x7D, 0x00}, {0x7C, 0x03}, {0x7D, 0x48}, {0x7D, 0x48},
    {0x7C, 0x08}, {0x7D, 0x20}, {0x7D, 0x10}, {0x7D, 0x0E}, {0x90, 0x00},
    {0x91, 0x0E}, {0x91, 0x1A}, {0x91, 0x31}, {0x91, 0x5A}, {0x91, 0x69},
    {0x91, 0x75}, {0x91, 0x7E}, {0x91, 0x88}, {0x91, 0x8F}, {0x91, 0x96},
    {0x91, 0xA3}, {0x91, 0xAF}, {0x91, 0xC4}, {0x91, 0xD7}, {0x91, 0xE8},
    {0x91, 0x20}, {0x92, 0x00}, {0x93, 0x06}, {0x93, 0xE3}, {0x93, 0x05},
    {0x93, 0x05}, {0x93, 0x00}, {0x93, 0x04}, {0x93, 0x00}, {0x93, 0x00},
    {0x93, 0x00}, {0x93, 0x00}, {0x93, 0x00}, {0x93, 0x00}, {0x93, 0x00},
    {0x96, 0x00}, {0x97, 0x08}, {0x97, 0x19}, {0x97, 0x02}, {0x97, 0x0C},
    {0x97, 0x24}, {0x97, 0x30}, {0x97, 0x28}, {0x97, 0x26}, {0x97, 0x02},
    {0x97, 0x98}, {0x97, 0x80}, {0x97, 0x00}, {0x97, 0x00}, {0xC3, 0xEF},
    {0xA4, 0x00}, {0xA8, 0x00}, {0xC5, 0x11}, {0xC6, 0x51}, {0xBF, 0x80},
    {0xC7, 0x10}, {0xB6, 0x66}, {0xB8, 0xA5}, {0xB7, 0x64}, {0xB9, 0x7C},
    {0xB3, 0xAF}, {0xB4, 0x97}, {0xB5, 0xFF}, {0xB0, 0xC5}, {0xB1, 0x94},
    {0xB2, 0x0F}, {0xC4, 0x5C}, {0xC0, 0xC8}, {0xC1, 0x96}, {0x8C, 0x00},
    {0x86, 0x3D}, {0x50, 0x00}, {0x51, 0x90}, {0x52, 0x2C}, {0x53, 0x00},
    {0x54, 0x00}, {0x55, 0x88}, {0x5A, 0x90}, {0x5B, 0x2C}, {0x5C, 0x05},
    {0xD3, 0x02}, {0xC3, 0xED}, {0x7F, 0x00}, {0xDA, 0x09}, {0xE5, 0x1F},
    {0xE1, 0x67}, {0xE0, 0x00}, {0xDD, 0x7F}, {0x05, 0x00},
};

static const uint8_t ov2640_yuv422_cfg[][2] = {
    {0xFF, 0x00},
    {0xDA, 0x10},
    {0xD7, 0x03},
    {0xDF, 0x00},
    {0x33, 0x80},
    {0x3C, 0x40},
    {0xE1, 0x77},
    {0x00, 0x00},
};

static const uint8_t ov2640_jpeg_cfg[][2] = {
    {0xFF, 0x01},
    {0xE0, 0x14},
    {0xE1, 0x77},
    {0xE5, 0x1F},
    {0xD7, 0x03},
    {0xDA, 0x10},
    {0xE0, 0x00},
};

static const uint8_t ov2640_rgb565_cfg[][2] = {
    {0xFF, 0x00},
    {0xDA, 0x09},
    {0xD7, 0x03},
    {0xDF, 0x02},
    {0x33, 0xA0},
    {0x3C, 0x00},
    {0xE1, 0x67},
    {0xFF, 0x01},
    {0xE0, 0x00},
    {0xE1, 0x00},
    {0xE5, 0x00},
    {0xD7, 0x00},
    {0xDA, 0x00},
    {0xE0, 0x00},
};

static const uint8_t ov2640_video_fast_cfg[][2] = {
    {0xFF, 0x00},
    {0xD3, OV2640_VIDEO_DVP_PCLK_DIV},
    {0x44, OV2640_VIDEO_JPEG_QUALITY},
    {0xDA, 0x10},
    {0xE0, 0x00},
};

static const uint8_t ov2640_light_mode_auto_cfg[][2] = {
    {0xFF, 0x00}, {0xC7, 0x00},
};
static const uint8_t ov2640_light_mode_sunny_cfg[][2] = {
    {0xFF, 0x00}, {0xC7, 0x40}, {0xCC, 0x5E}, {0xCD, 0x41}, {0xCE, 0x54},
};
static const uint8_t ov2640_light_mode_cloudy_cfg[][2] = {
    {0xFF, 0x00}, {0xC7, 0x40}, {0xCC, 0x65}, {0xCD, 0x41}, {0xCE, 0x4F},
};
static const uint8_t ov2640_light_mode_office_cfg[][2] = {
    {0xFF, 0x00}, {0xC7, 0x40}, {0xCC, 0x52}, {0xCD, 0x41}, {0xCE, 0x66},
};
static const uint8_t ov2640_light_mode_home_cfg[][2] = {
    {0xFF, 0x00}, {0xC7, 0x40}, {0xCC, 0x42}, {0xCD, 0x3F}, {0xCE, 0x71},
};

static const uint8_t ov2640_saturation_0_cfg[][2] = {
    {0xFF, 0x00}, {0x7C, 0x00}, {0x7D, 0x02}, {0x7C, 0x03}, {0x7D, 0x68}, {0x7D, 0x68},
};
static const uint8_t ov2640_saturation_1_cfg[][2] = {
    {0xFF, 0x00}, {0x7C, 0x00}, {0x7D, 0x02}, {0x7C, 0x03}, {0x7D, 0x58}, {0x7D, 0x58},
};
static const uint8_t ov2640_saturation_2_cfg[][2] = {
    {0xFF, 0x00}, {0x7C, 0x00}, {0x7D, 0x02}, {0x7C, 0x03}, {0x7D, 0x48}, {0x7D, 0x48},
};
static const uint8_t ov2640_saturation_3_cfg[][2] = {
    {0xFF, 0x00}, {0x7C, 0x00}, {0x7D, 0x02}, {0x7C, 0x03}, {0x7D, 0x38}, {0x7D, 0x38},
};
static const uint8_t ov2640_saturation_4_cfg[][2] = {
    {0xFF, 0x00}, {0x7C, 0x00}, {0x7D, 0x02}, {0x7C, 0x03}, {0x7D, 0x28}, {0x7D, 0x28},
};

static const uint8_t ov2640_brightness_0_cfg[][2] = {
    {0xFF, 0x00}, {0x7C, 0x00}, {0x7D, 0x04}, {0x7C, 0x09}, {0x7D, 0x40}, {0x7D, 0x00},
};
static const uint8_t ov2640_brightness_1_cfg[][2] = {
    {0xFF, 0x00}, {0x7C, 0x00}, {0x7D, 0x04}, {0x7C, 0x09}, {0x7D, 0x30}, {0x7D, 0x00},
};
static const uint8_t ov2640_brightness_2_cfg[][2] = {
    {0xFF, 0x00}, {0x7C, 0x00}, {0x7D, 0x04}, {0x7C, 0x09}, {0x7D, 0x20}, {0x7D, 0x00},
};
static const uint8_t ov2640_brightness_3_cfg[][2] = {
    {0xFF, 0x00}, {0x7C, 0x00}, {0x7D, 0x04}, {0x7C, 0x09}, {0x7D, 0x10}, {0x7D, 0x00},
};
static const uint8_t ov2640_brightness_4_cfg[][2] = {
    {0xFF, 0x00}, {0x7C, 0x00}, {0x7D, 0x04}, {0x7C, 0x09}, {0x7D, 0x00}, {0x7D, 0x00},
};

static const uint8_t ov2640_contrast_0_cfg[][2] = {
    {0xFF, 0x00}, {0x7C, 0x00}, {0x7D, 0x04}, {0x7C, 0x07}, {0x7D, 0x20}, {0x7D, 0x28}, {0x7D, 0x0C}, {0x7D, 0x06},
};
static const uint8_t ov2640_contrast_1_cfg[][2] = {
    {0xFF, 0x00}, {0x7C, 0x00}, {0x7D, 0x04}, {0x7C, 0x07}, {0x7D, 0x20}, {0x7D, 0x24}, {0x7D, 0x16}, {0x7D, 0x06},
};
static const uint8_t ov2640_contrast_2_cfg[][2] = {
    {0xFF, 0x00}, {0x7C, 0x00}, {0x7D, 0x04}, {0x7C, 0x07}, {0x7D, 0x20}, {0x7D, 0x20}, {0x7D, 0x20}, {0x7D, 0x06},
};
static const uint8_t ov2640_contrast_3_cfg[][2] = {
    {0xFF, 0x00}, {0x7C, 0x00}, {0x7D, 0x04}, {0x7C, 0x07}, {0x7D, 0x20}, {0x7D, 0x1C}, {0x7D, 0x2A}, {0x7D, 0x06},
};
static const uint8_t ov2640_contrast_4_cfg[][2] = {
    {0xFF, 0x00}, {0x7C, 0x00}, {0x7D, 0x04}, {0x7C, 0x07}, {0x7D, 0x20}, {0x7D, 0x18}, {0x7D, 0x34}, {0x7D, 0x06},
};

static const uint8_t ov2640_effect_normal_cfg[][2] = {
    {0xFF, 0x00}, {0x7C, 0x00}, {0x7D, 0x00}, {0x7C, 0x05}, {0x7D, 0x80}, {0x7D, 0x80},
};

/* ── 内部函数 ── */

static void ov2640_assert_power_down(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
}

static void ov2640_release_power_down(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
}

static void ov2640_hw_reset(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
    HAL_Delay(OV2640_RESET_ASSERT_MS);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
    HAL_Delay(OV2640_RESET_RELEASE_MS);
}

static void ov2640_power_cycle_and_reset(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
    ov2640_assert_power_down();
    HAL_Delay(OV2640_PWRDN_ASSERT_MS);

    ov2640_release_power_down();
    HAL_Delay(OV2640_PWR_STABLE_MS);

    ov2640_hw_reset();
    HAL_Delay(OV2640_POST_RESET_MS);

    /* A second clean reset pulse helps after ST-LINK reflash/reset sequences. */
    ov2640_hw_reset();
    HAL_Delay(OV2640_POST_RESET_MS);
}

static uint8_t ov2640_write_reg(uint8_t reg, uint8_t data)
{
    return (OV2640_SCCB_WriteReg(OV2640_SCCB_ADDR, reg, data) == HAL_OK) ? OV2640_OK : OV2640_ERROR;
}

static uint8_t ov2640_read_reg(uint8_t reg, uint8_t *data)
{
    return (OV2640_SCCB_ReadReg(OV2640_SCCB_ADDR, reg, data) == HAL_OK) ? OV2640_OK : OV2640_ERROR;
}

static void ov2640_sw_reset(void)
{
    ov2640_write_reg(0xFF, 0x01U);
    ov2640_write_reg(0x12, 0x80U);
    HAL_Delay(50U);
}

static uint8_t ov2640_write_table(const uint8_t table[][2], uint32_t count)
{
    uint32_t i;
    for (i = 0U; i < count; i++)
    {
        if (ov2640_write_reg(table[i][0], table[i][1]) != OV2640_OK)
            return OV2640_ERROR;
    }
    return OV2640_OK;
}

static uint8_t ov2640_apply_table(const uint8_t table[][2], uint32_t count)
{
    return ov2640_write_table(table, count);
}

/* ── 公开接口 ── */

uint8_t OV2640_IsReady(void) { return s_initialized; }

uint8_t OV2640_ProbeID(uint16_t *mid, uint16_t *pid)
{
    uint8_t h = 0U, l = 0U;
    if ((mid == NULL) || (pid == NULL)) return OV2640_ERROR;
    if (ov2640_write_reg(0xFF, 0x01U) != OV2640_OK) return OV2640_ERROR;
    if (ov2640_read_reg(OV2640_REG_ID_MIDH, &h) != OV2640_OK) return OV2640_ERROR;
    if (ov2640_read_reg(OV2640_REG_ID_MIDL, &l) != OV2640_OK) return OV2640_ERROR;
    *mid = ((uint16_t)h << 8) | l;
    if (ov2640_read_reg(OV2640_REG_ID_PIDH, &h) != OV2640_OK) return OV2640_ERROR;
    if (ov2640_read_reg(OV2640_REG_ID_PIDL, &l) != OV2640_OK) return OV2640_ERROR;
    *pid = ((uint16_t)h << 8) | l;
    return OV2640_OK;
}


uint8_t OV2640_SetOutputFormatJPEG(void)
{
    if (ov2640_write_table(ov2640_yuv422_cfg, sizeof(ov2640_yuv422_cfg) / sizeof(ov2640_yuv422_cfg[0])) != OV2640_OK)
    {
        return OV2640_ERROR;
    }

    return (ov2640_write_table(ov2640_jpeg_cfg, sizeof(ov2640_jpeg_cfg) / sizeof(ov2640_jpeg_cfg[0])) == OV2640_OK) ? OV2640_OK : OV2640_ERROR;
}

uint8_t OV2640_SetOutputFormatRGB565(void)
{
    return (ov2640_write_table(ov2640_rgb565_cfg, sizeof(ov2640_rgb565_cfg) / sizeof(ov2640_rgb565_cfg[0])) == OV2640_OK) ? OV2640_OK : OV2640_ERROR;
}

uint8_t OV2640_SetLightMode(ov2640_light_mode_t mode)
{
    switch (mode)
    {
    case OV2640_LIGHT_MODE_AUTO:   return ov2640_apply_table(ov2640_light_mode_auto_cfg, sizeof(ov2640_light_mode_auto_cfg) / sizeof(ov2640_light_mode_auto_cfg[0]));
    case OV2640_LIGHT_MODE_SUNNY:  return ov2640_apply_table(ov2640_light_mode_sunny_cfg, sizeof(ov2640_light_mode_sunny_cfg) / sizeof(ov2640_light_mode_sunny_cfg[0]));
    case OV2640_LIGHT_MODE_CLOUDY: return ov2640_apply_table(ov2640_light_mode_cloudy_cfg, sizeof(ov2640_light_mode_cloudy_cfg) / sizeof(ov2640_light_mode_cloudy_cfg[0]));
    case OV2640_LIGHT_MODE_OFFICE: return ov2640_apply_table(ov2640_light_mode_office_cfg, sizeof(ov2640_light_mode_office_cfg) / sizeof(ov2640_light_mode_office_cfg[0]));
    case OV2640_LIGHT_MODE_HOME:   return ov2640_apply_table(ov2640_light_mode_home_cfg, sizeof(ov2640_light_mode_home_cfg) / sizeof(ov2640_light_mode_home_cfg[0]));
    default: return OV2640_ERROR;
    }
}

uint8_t OV2640_SetColorSaturation(ov2640_color_saturation_t saturation)
{
    switch (saturation)
    {
    case OV2640_COLOR_SATURATION_0: return ov2640_apply_table(ov2640_saturation_0_cfg, sizeof(ov2640_saturation_0_cfg) / sizeof(ov2640_saturation_0_cfg[0]));
    case OV2640_COLOR_SATURATION_1: return ov2640_apply_table(ov2640_saturation_1_cfg, sizeof(ov2640_saturation_1_cfg) / sizeof(ov2640_saturation_1_cfg[0]));
    case OV2640_COLOR_SATURATION_2: return ov2640_apply_table(ov2640_saturation_2_cfg, sizeof(ov2640_saturation_2_cfg) / sizeof(ov2640_saturation_2_cfg[0]));
    case OV2640_COLOR_SATURATION_3: return ov2640_apply_table(ov2640_saturation_3_cfg, sizeof(ov2640_saturation_3_cfg) / sizeof(ov2640_saturation_3_cfg[0]));
    case OV2640_COLOR_SATURATION_4: return ov2640_apply_table(ov2640_saturation_4_cfg, sizeof(ov2640_saturation_4_cfg) / sizeof(ov2640_saturation_4_cfg[0]));
    default: return OV2640_ERROR;
    }
}

uint8_t OV2640_SetBrightness(ov2640_brightness_t brightness)
{
    switch (brightness)
    {
    case OV2640_BRIGHTNESS_0: return ov2640_apply_table(ov2640_brightness_0_cfg, sizeof(ov2640_brightness_0_cfg) / sizeof(ov2640_brightness_0_cfg[0]));
    case OV2640_BRIGHTNESS_1: return ov2640_apply_table(ov2640_brightness_1_cfg, sizeof(ov2640_brightness_1_cfg) / sizeof(ov2640_brightness_1_cfg[0]));
    case OV2640_BRIGHTNESS_2: return ov2640_apply_table(ov2640_brightness_2_cfg, sizeof(ov2640_brightness_2_cfg) / sizeof(ov2640_brightness_2_cfg[0]));
    case OV2640_BRIGHTNESS_3: return ov2640_apply_table(ov2640_brightness_3_cfg, sizeof(ov2640_brightness_3_cfg) / sizeof(ov2640_brightness_3_cfg[0]));
    case OV2640_BRIGHTNESS_4: return ov2640_apply_table(ov2640_brightness_4_cfg, sizeof(ov2640_brightness_4_cfg) / sizeof(ov2640_brightness_4_cfg[0]));
    default: return OV2640_ERROR;
    }
}

uint8_t OV2640_SetContrast(ov2640_contrast_t contrast)
{
    switch (contrast)
    {
    case OV2640_CONTRAST_0: return ov2640_apply_table(ov2640_contrast_0_cfg, sizeof(ov2640_contrast_0_cfg) / sizeof(ov2640_contrast_0_cfg[0]));
    case OV2640_CONTRAST_1: return ov2640_apply_table(ov2640_contrast_1_cfg, sizeof(ov2640_contrast_1_cfg) / sizeof(ov2640_contrast_1_cfg[0]));
    case OV2640_CONTRAST_2: return ov2640_apply_table(ov2640_contrast_2_cfg, sizeof(ov2640_contrast_2_cfg) / sizeof(ov2640_contrast_2_cfg[0]));
    case OV2640_CONTRAST_3: return ov2640_apply_table(ov2640_contrast_3_cfg, sizeof(ov2640_contrast_3_cfg) / sizeof(ov2640_contrast_3_cfg[0]));
    case OV2640_CONTRAST_4: return ov2640_apply_table(ov2640_contrast_4_cfg, sizeof(ov2640_contrast_4_cfg) / sizeof(ov2640_contrast_4_cfg[0]));
    default: return OV2640_ERROR;
    }
}

uint8_t OV2640_SetSpecialEffect(ov2640_special_effect_t effect)
{
    switch (effect)
    {
    case OV2640_SPECIAL_EFFECT_NORMAL:
        return ov2640_apply_table(ov2640_effect_normal_cfg, sizeof(ov2640_effect_normal_cfg) / sizeof(ov2640_effect_normal_cfg[0]));
    default:
        return OV2640_ERROR;
    }
}

uint8_t OV2640_SetOutputSize(uint16_t width, uint16_t height)
{
    uint16_t ow, oh;
    if ((width != OV2640_VIDEO_WIDTH) || (height != OV2640_VIDEO_HEIGHT)) return OV2640_ERROR;

    ow = width >> 2;
    oh = height >> 2;

    if (ov2640_write_reg(0xFF, 0x00U) != OV2640_OK) return OV2640_ERROR;
    if (ov2640_write_reg(0xE0, 0x04U) != OV2640_OK) return OV2640_ERROR;
    if (ov2640_write_reg(0x5A, (uint8_t)(ow & 0xFFU)) != OV2640_OK) return OV2640_ERROR;
    if (ov2640_write_reg(0x5B, (uint8_t)(oh & 0xFFU)) != OV2640_OK) return OV2640_ERROR;
    if (ov2640_write_reg(0x5C, (uint8_t)(((ow >> 8) & 0x03U) | ((oh >> 6) & 0x04U))) != OV2640_OK) return OV2640_ERROR;
    if (ov2640_write_reg(0xE0, 0x00U) != OV2640_OK) return OV2640_ERROR;
    return OV2640_OK;
}

uint8_t OV2640_StartSnapshot(void)
{
    if ((s_initialized == 0U) || (s_frame_buf == NULL)) return OV2640_ERROR;
    __HAL_DCMI_ENABLE_IT(&hdcmi, DCMI_IT_FRAME);
    return (HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_SNAPSHOT, (uint32_t)s_frame_buf, s_frame_buf_len / 4U) == HAL_OK) ? OV2640_OK : OV2640_ERROR;
}

uint8_t OV2640_StopCapture(void)
{
    return (HAL_DCMI_Stop(&hdcmi) == HAL_OK) ? OV2640_OK : OV2640_ERROR;
}

void OV2640_AttachFrameBuffer(uint8_t *buf, uint32_t len) { s_frame_buf = buf; s_frame_buf_len = len; }

uint8_t OV2640_Probe(uint16_t *mid, uint16_t *pid)
{
    uint32_t attempt;

    if ((mid == NULL) || (pid == NULL)) return OV2640_ERROR;
    /* 唯一一次断电→上电 (U5 hw_init + exit_power_down) */
    for (attempt = 0U; attempt < OV2640_PROBE_ATTEMPTS; attempt++)
    {
        *mid = 0U;
        *pid = 0U;

        ov2640_power_cycle_and_reset();
        OV2640_SCCB_Init();
        ov2640_sw_reset();

        if (OV2640_ProbeID(mid, pid) != OV2640_OK)
        {
            HAL_Delay(OV2640_POST_RESET_MS);
            continue;
        }

        if ((*mid == OV2640_EXPECTED_MID) && (*pid == OV2640_EXPECTED_PID))
        {
            return OV2640_OK;
        }

        HAL_Delay(OV2640_POST_RESET_MS);
    }

    return OV2640_ERROR;
}

uint8_t OV2640_Init(void)
{
    uint16_t mid = 0U, pid = 0U;
    s_initialized = 0U;
    /* 摄像头已上电, 只做SW复位+写表 */
    ov2640_sw_reset();
    HAL_Delay(OV2640_POST_RESET_MS);
    if ((OV2640_ProbeID(&mid, &pid) != OV2640_OK) ||
        (mid != OV2640_EXPECTED_MID) ||
        (pid != OV2640_EXPECTED_PID))
    {
        ov2640_power_cycle_and_reset();
        OV2640_SCCB_Init();
        ov2640_sw_reset();
        HAL_Delay(OV2640_POST_RESET_MS);

        if (OV2640_ProbeID(&mid, &pid) != OV2640_OK) return OV2640_ERROR;
        if ((mid != OV2640_EXPECTED_MID) || (pid != OV2640_EXPECTED_PID)) return OV2640_ERROR;
    }
    if (ov2640_write_table(ov2640_init_common_cfg, sizeof(ov2640_init_common_cfg) / sizeof(ov2640_init_common_cfg[0])) != OV2640_OK) return OV2640_ERROR;
    if (ov2640_write_table(ov2640_video_fast_cfg, sizeof(ov2640_video_fast_cfg) / sizeof(ov2640_video_fast_cfg[0])) != OV2640_OK) return OV2640_ERROR;
    if (OV2640_SetOutputFormatJPEG() != OV2640_OK) return OV2640_ERROR;
    HAL_Delay(OV2640_POST_RESET_MS);
    if (OV2640_SetOutputSize(OV2640_VIDEO_WIDTH, OV2640_VIDEO_HEIGHT) != OV2640_OK) return OV2640_ERROR;
    s_initialized = 1U;
    return OV2640_OK;
}
