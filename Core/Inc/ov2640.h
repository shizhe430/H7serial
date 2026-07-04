#ifndef __OV2640_H
#define __OV2640_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"
#include <stdint.h>

#define OV2640_OK            0U
#define OV2640_ERROR         1U

#define OV2640_SCCB_ADDR     0x60U
#define OV2640_REG_ID_MIDH   0x1CU
#define OV2640_REG_ID_MIDL   0x1DU
#define OV2640_REG_ID_PIDH   0x0AU
#define OV2640_REG_ID_PIDL   0x0BU

typedef enum
{
    OV2640_LIGHT_MODE_AUTO = 0x00,
    OV2640_LIGHT_MODE_SUNNY,
    OV2640_LIGHT_MODE_CLOUDY,
    OV2640_LIGHT_MODE_OFFICE,
    OV2640_LIGHT_MODE_HOME,
} ov2640_light_mode_t;

typedef enum
{
    OV2640_COLOR_SATURATION_0 = 0x00,
    OV2640_COLOR_SATURATION_1,
    OV2640_COLOR_SATURATION_2,
    OV2640_COLOR_SATURATION_3,
    OV2640_COLOR_SATURATION_4,
} ov2640_color_saturation_t;

typedef enum
{
    OV2640_BRIGHTNESS_0 = 0x00,
    OV2640_BRIGHTNESS_1,
    OV2640_BRIGHTNESS_2,
    OV2640_BRIGHTNESS_3,
    OV2640_BRIGHTNESS_4,
} ov2640_brightness_t;

typedef enum
{
    OV2640_CONTRAST_0 = 0x00,
    OV2640_CONTRAST_1,
    OV2640_CONTRAST_2,
    OV2640_CONTRAST_3,
    OV2640_CONTRAST_4,
} ov2640_contrast_t;

typedef enum
{
    OV2640_SPECIAL_EFFECT_ANTIQUE = 0x00,
    OV2640_SPECIAL_EFFECT_BLUISH,
    OV2640_SPECIAL_EFFECT_GREENISH,
    OV2640_SPECIAL_EFFECT_REDISH,
    OV2640_SPECIAL_EFFECT_BW,
    OV2640_SPECIAL_EFFECT_NEGATIVE,
    OV2640_SPECIAL_EFFECT_BW_NEGATIVE,
    OV2640_SPECIAL_EFFECT_NORMAL,
} ov2640_special_effect_t;
uint8_t OV2640_Init(void);
uint8_t OV2640_IsReady(void);
uint8_t OV2640_ProbeID(uint16_t *mid, uint16_t *pid);
uint8_t OV2640_Probe(uint16_t *mid, uint16_t *pid);  /* 仅上电+复位+SCCB+读ID */
uint8_t OV2640_SetLightMode(ov2640_light_mode_t mode);
uint8_t OV2640_SetColorSaturation(ov2640_color_saturation_t saturation);
uint8_t OV2640_SetBrightness(ov2640_brightness_t brightness);
uint8_t OV2640_SetContrast(ov2640_contrast_t contrast);
uint8_t OV2640_SetSpecialEffect(ov2640_special_effect_t effect);
uint8_t OV2640_SetOutputFormatJPEG(void);
uint8_t OV2640_SetOutputFormatRGB565(void);
uint8_t OV2640_SetOutputSize(uint16_t width, uint16_t height);
uint8_t OV2640_StartSnapshot(void);
uint8_t OV2640_StopCapture(void);
void OV2640_AttachFrameBuffer(uint8_t *buf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* __OV2640_H */
