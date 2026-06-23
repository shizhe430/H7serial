/**
 ****************************************************************************************************
 * @file        atk_mc2640.h
 * @author      ALIENTEK
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MC2640模块配置头文件
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:ALIENTEK Mini Pro H750开发板
 * 视频教程:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司地址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#ifndef __ATK_MC2640_H
#define __ATK_MC2640_H

#include "main.h"

#define ATK_MC2640_USING_DCMI                   1

#if (ATK_MC2640_USING_DCMI == 0)
#define ATK_MC2640_DATA_PIN_IN_SAME_GPIO_PORT   0
#endif

#define ATK_MC2640_LED_CTL_BY_OV2640            1

#if (ATK_MC2640_USING_DCMI == 0)
#if (ATK_MC2640_DATA_PIN_IN_SAME_GPIO_PORT != 0)
#define ATK_MC2640_DATE_GPIO_PORT  GPIOC
#define ATK_MC2640_DATA_READ_MASK  0x00FF
#endif
#endif

#if (ATK_MC2640_USING_DCMI == 0)
#define ATK_MC2640_DMA_INTERFACE                GPDMA1_Channel0
#define ATK_MC2640_DMA_CLK_ENABLE()             do{ __HAL_RCC_GPDMA1_CLK_ENABLE(); }while(0)
#endif

#define ATK_MC2640_RST_GPIO_PORT                GPIOB
#define ATK_MC2640_RST_GPIO_PIN                 GPIO_PIN_0
#define ATK_MC2640_RST_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)
#define ATK_MC2640_PWDN_GPIO_PORT               GPIOC
#define ATK_MC2640_PWDN_GPIO_PIN                GPIO_PIN_4
#define ATK_MC2640_PWDN_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
#if (ATK_MC2640_LED_CTL_BY_OV2640 == 0)
#define ATK_MC2640_FLASH_GPIO_PORT              GPIOA
#define ATK_MC2640_FLASH_GPIO_PIN               GPIO_PIN_8
#define ATK_MC2640_FLASH_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)
#endif

#define ATK_MC2640_RST(x)                       do{ x ?                                                                                         \
                                                    HAL_GPIO_WritePin(ATK_MC2640_RST_GPIO_PORT, ATK_MC2640_RST_GPIO_PIN, GPIO_PIN_SET) :        \
                                                    HAL_GPIO_WritePin(ATK_MC2640_RST_GPIO_PORT, ATK_MC2640_RST_GPIO_PIN, GPIO_PIN_RESET);       \
                                                }while(0)
#define ATK_MC2640_PWDN(x)                      do{ x ?                                                                                         \
                                                    HAL_GPIO_WritePin(ATK_MC2640_PWDN_GPIO_PORT, ATK_MC2640_PWDN_GPIO_PIN, GPIO_PIN_SET) :      \
                                                    HAL_GPIO_WritePin(ATK_MC2640_PWDN_GPIO_PORT, ATK_MC2640_PWDN_GPIO_PIN, GPIO_PIN_RESET);     \
                                                }while(0)
#if (ATK_MC2640_LED_CTL_BY_OV2640 == 0)
#define ATK_MC2640_FLASH(x)                     do{ x ?                                                                                         \
                                                    HAL_GPIO_WritePin(ATK_MC2640_FLASH_GPIO_PORT, ATK_MC2640_FLASH_GPIO_PIN, GPIO_PIN_SET) :    \
                                                    HAL_GPIO_WritePin(ATK_MC2640_FLASH_GPIO_PORT, ATK_MC2640_FLASH_GPIO_PIN, GPIO_PIN_RESET);   \
                                                }while(0)
#endif

#define ATK_MC2640_SCCB_ADDR                    0x30

typedef enum
{
    ATK_MC2640_LIGHT_MODE_AUTO = 0x00,
    ATK_MC2640_LIGHT_MODE_SUNNY,
    ATK_MC2640_LIGHT_MODE_CLOUDY,
    ATK_MC2640_LIGHT_MODE_OFFICE,
    ATK_MC2640_LIGHT_MODE_HOME,
} atk_mc2640_light_mode_t;

typedef enum
{
    ATK_MC2640_COLOR_SATURATION_0 = 0x00,
    ATK_MC2640_COLOR_SATURATION_1,
    ATK_MC2640_COLOR_SATURATION_2,
    ATK_MC2640_COLOR_SATURATION_3,
    ATK_MC2640_COLOR_SATURATION_4,
} atk_mc2640_color_saturation_t;

typedef enum
{
    ATK_MC2640_BRIGHTNESS_0 = 0x00,
    ATK_MC2640_BRIGHTNESS_1,
    ATK_MC2640_BRIGHTNESS_2,
    ATK_MC2640_BRIGHTNESS_3,
    ATK_MC2640_BRIGHTNESS_4,
} atk_mc2640_brightness_t;

typedef enum
{
    ATK_MC2640_CONTRAST_0 = 0x00,
    ATK_MC2640_CONTRAST_1,
    ATK_MC2640_CONTRAST_2,
    ATK_MC2640_CONTRAST_3,
    ATK_MC2640_CONTRAST_4,
} atk_mc2640_contrast_t;

typedef enum
{
    ATK_MC2640_SPECIAL_EFFECT_ANTIQUE = 0x00,
    ATK_MC2640_SPECIAL_EFFECT_BLUISH,
    ATK_MC2640_SPECIAL_EFFECT_GREENISH,
    ATK_MC2640_SPECIAL_EFFECT_REDISH,
    ATK_MC2640_SPECIAL_EFFECT_BW,
    ATK_MC2640_SPECIAL_EFFECT_NEGATIVE,
    ATK_MC2640_SPECIAL_EFFECT_BW_NEGATIVE,
    ATK_MC2640_SPECIAL_EFFECT_NORMAL,
} atk_mc2640_special_effect_t;

typedef enum
{
    ATK_MC2640_OUTPUT_FORMAT_RGB565 = 0x00,
    ATK_MC2640_OUTPUT_FORMAT_JPEG,
} atk_mc2640_output_format_t;

typedef enum
{
    ATK_MC2640_GET_TYPE_DTS_8B_NOINC = 0x00,
    ATK_MC2640_GET_TYPE_DTS_8B_INC,
    ATK_MC2640_GET_TYPE_DTS_16B_NOINC,
    ATK_MC2640_GET_TYPE_DTS_16B_INC,
    ATK_MC2640_GET_TYPE_DTS_32B_NOINC,
    ATK_MC2640_GET_TYPE_DTS_32B_INC,
} atk_mc2640_get_type_t;

#define ATK_MC2640_EOK      0
#define ATK_MC2640_ERROR    1
#define ATK_MC2640_EINVAL   2
#define ATK_MC2640_ENOMEM   3
#define ATK_MC2640_EEMPTY   4

uint8_t atk_mc2640_init(void);
#if (ATK_MC2640_LED_CTL_BY_OV2640 == 0)
void atk_mc2640_led_on(void);
void atk_mc2640_led_off(void);
#else
void atk_mc2640_led_enable(void);
#endif
uint8_t atk_mc2640_set_light_mode(atk_mc2640_light_mode_t mode);
uint8_t atk_mc2640_set_color_saturation(atk_mc2640_color_saturation_t saturation);
uint8_t atk_mc2640_set_brightness(atk_mc2640_brightness_t brightness);
uint8_t atk_mc2640_set_contrast(atk_mc2640_contrast_t contrast);
uint8_t atk_mc2640_set_special_effect(atk_mc2640_special_effect_t effect);
uint8_t atk_mc2640_set_output_format(atk_mc2640_output_format_t format);
uint8_t atk_mc2640_set_output_size(uint16_t width, uint16_t height);
void atk_mc2640_set_sensor_window(uint16_t start_x, uint16_t start_y, uint16_t width, uint16_t height);
uint8_t atk_mc2640_set_image_window(uint16_t off_x, uint16_t off_y, uint16_t width, uint16_t height);
void atk_mc2640_set_image_size(uint16_t width, uint16_t height);
uint8_t atk_mc2640_set_output_speed(uint8_t clk_dev, uint8_t pclk_dev);
void atk_mc2640_colorbar_enable(void);
void atk_mc2640_colorbar_disable(void);
uint8_t atk_mc2640_get_frame(uint32_t dts_addr, atk_mc2640_get_type_t type, void (*before_transfer)(void));

#endif
