/**
 ****************************************************************************************************
 * @file        atk_mc2640.c
 * @author      ALIENTEK
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MC2640模块驱动代码
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

#include "atk_mc2640.h"
#include "atk_mc2640_sccb.h"
#include "atk_mc2640_cfg.h"
#include "delay.h"

#if (ATK_MC2640_USING_DCMI != 0)
#include "atk_mc2640_dcmi.h"
#endif

#define ATK_MC2640_MID  0x7FA2
#define ATK_MC2640_PID  0x2642

typedef enum
{
    ATK_MC2640_REG_BANK_DSP = 0x00,
    ATK_MC2640_REG_BANK_SENSOR,
} atk_mc2640_reg_bank_t;

static struct
{
    struct {
        uint16_t width;
        uint16_t height;
    } output;

#if (ATK_MC2640_USING_DCMI == 0)
    struct {
        uint8_t *line_buf;
        DMA_HandleTypeDef dma_handle;
    } read;
#endif
} g_atk_mc2640_sta = {0};

/**
 * @brief  写寄存器
 * @param  reg: 寄存器地址
 * @param  dat: 要写入的数据
 * @retval 无
 */
static void atk_mc2640_write_reg(uint8_t reg, uint8_t dat)
{
    atk_mc2640_sccb_3_phase_write(ATK_MC2640_SCCB_ADDR, reg, dat);
}

/**
 * @brief  读寄存器
 * @param  reg: 寄存器地址
 * @retval 读取到的寄存器值
 */
static uint8_t atk_mc2640_read_reg(uint8_t reg)
{
    uint8_t dat = 0;

    atk_mc2640_sccb_2_phase_write(ATK_MC2640_SCCB_ADDR, reg);
    atk_mc2640_sccb_2_phase_read(ATK_MC2640_SCCB_ADDR, &dat);

    return dat;
}

/**
 * @brief  选择寄存器组(DSP或SENSOR)
 * @param  bank: 寄存器组类型
 *   @arg ATK_MC2640_REG_BANK_DSP: DSP寄存器组
 *   @arg ATK_MC2640_REG_BANK_SENSOR: SENSOR寄存器组
 * @retval ATK_MC2640_EOK: 成功; ATK_MC2640_EINVAL: 参数错误
 */
static uint8_t atk_mc2640_reg_bank_select(atk_mc2640_reg_bank_t bank)
{
    switch (bank)
    {
        case ATK_MC2640_REG_BANK_DSP:
        {
            atk_mc2640_write_reg(ATK_MC2640_REG_BANK_SEL, 0x00);
            break;
        }
        case ATK_MC2640_REG_BANK_SENSOR:
        {
            atk_mc2640_write_reg(ATK_MC2640_REG_BANK_SEL, 0x01);
            break;
        }
        default:
        {
            return ATK_MC2640_EINVAL;
        }
    }

    return ATK_MC2640_EOK;
}

/**
 * @brief  ATK-MC2640硬件初始化(复位和电源引脚)
 * @retval 无
 */
static void atk_mc2640_hw_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};

    ATK_MC2640_RST_GPIO_CLK_ENABLE();
    ATK_MC2640_PWDN_GPIO_CLK_ENABLE();

    gpio_init_struct.Pin    = ATK_MC2640_RST_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MC2640_RST_GPIO_PORT, &gpio_init_struct);

    gpio_init_struct.Pin    = ATK_MC2640_PWDN_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MC2640_PWDN_GPIO_PORT, &gpio_init_struct);

    ATK_MC2640_RST(1);
    ATK_MC2640_PWDN(1);
}

/**
 * @brief  ATK-MC2640退出掉电模式
 * @retval 无
 */
static void atk_mc2640_exit_power_down(void)
{
    ATK_MC2640_PWDN(0);
    delay_ms(10);
}

/**
 * @brief  ATK-MC2640硬件复位
 * @retval 无
 */
static void atk_mc2640_hw_reset(void)
{
    ATK_MC2640_RST(0);
    delay_ms(10);
    ATK_MC2640_RST(1);
    delay_ms(10);
}

/**
 * @brief  ATK-MC2640软件复位
 * @retval 无
 */
static void atk_mc2640_sw_reset(void)
{
    atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_SENSOR);
    atk_mc2640_write_reg(ATK_MC2640_REG_SENSOR_COM7, 0x80);
    delay_ms(50);
}

/**
 * @brief  获取ATK-MC2640模块ID
 * @retval 模块ID
 */
static uint16_t atk_mc2640_get_mid(void)
{
    uint16_t mid;

    atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_SENSOR);
    mid = atk_mc2640_read_reg(ATK_MC2640_REG_SENSOR_MIDH) << 8;
    mid |= atk_mc2640_read_reg(ATK_MC2640_REG_SENSOR_MIDL);

    return mid;
}

/**
 * @brief  获取ATK-MC2640产品ID
 * @retval 产品ID
 */
static uint16_t atk_mc2640_get_pid(void)
{
    uint16_t pid;

    atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_SENSOR);
    pid = atk_mc2640_read_reg(ATK_MC2640_REG_SENSOR_PIDH) << 8;
    pid |= atk_mc2640_read_reg(ATK_MC2640_REG_SENSOR_PIDL);

    return pid;
}

/**
 * @brief  ATK-MC2640初始化寄存器配置，并读取输出尺寸信息
 * @retval 无
 */
static void atk_mc2640_init_reg(void)
{
    uint32_t cfg_index;
    uint8_t zmow;
    uint8_t zmoh;
    uint8_t zmhh;

    for (cfg_index=0; cfg_index<(sizeof(atk_mc2640_init_uxga_cfg)/sizeof(atk_mc2640_init_uxga_cfg[0])); cfg_index++)
    {
        atk_mc2640_write_reg(atk_mc2640_init_uxga_cfg[cfg_index][0], atk_mc2640_init_uxga_cfg[cfg_index][1]);
    }

    atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
    zmow = atk_mc2640_read_reg(ATK_MC2640_REG_DSP_ZMOW);
    zmoh = atk_mc2640_read_reg(ATK_MC2640_REG_DSP_ZMOH);
    zmhh = atk_mc2640_read_reg(ATK_MC2640_REG_DSP_ZMHH);

    g_atk_mc2640_sta.output.width = ((uint16_t)zmow | ((zmhh & 0x03) << 8)) << 2;
    g_atk_mc2640_sta.output.height = ((uint16_t)zmoh | ((zmhh & 0x04) << 6)) << 2;
}

/**
 * @brief  ATK-MC2640初始化(包含硬件初始化、SCCB初始化、ID校验、寄存器配置等)
 * @retval ATK_MC2640_EOK: 成功; ATK_MC2640_ERROR: ID校验失败
 */
uint8_t atk_mc2640_init(void)
{
    uint16_t mid;
    uint16_t pid;

    atk_mc2640_hw_init();
    atk_mc2640_exit_power_down();
    atk_mc2640_hw_reset();
    atk_mc2640_sccb_init();
    atk_mc2640_sw_reset();

    mid = atk_mc2640_get_mid();
    if (mid != ATK_MC2640_MID)
    {
        return ATK_MC2640_ERROR;
    }

    pid = atk_mc2640_get_pid();
    if (pid != ATK_MC2640_PID)
    {
        return ATK_MC2640_ERROR;
    }

    atk_mc2640_init_reg();
    atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_SENSOR);

    atk_mc2640_write_reg(0x13, 0xE5);    // COM8: AGC+AEC+AWB开启(BandFilter关,与官方一致)

    atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
    atk_mc2640_write_reg(0x44, 0x03);    // JPEG画质(与官方jpeg_cfg一致)
#if (ATK_MC2640_USING_DCMI != 0)
    atk_mc2640_dcmi_init();
#endif

    return ATK_MC2640_EOK;
}

#if (ATK_MC2640_LED_CTL_BY_OV2640 == 0)
void atk_mc2640_led_on(void)
{
    ATK_MC2640_FLASH(1);
}

void atk_mc2640_led_off(void)
{
    ATK_MC2640_FLASH(0);
}
#else
void atk_mc2640_led_enable(void)
{
    atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_SENSOR);
    atk_mc2640_write_reg(ATK_MC2640_REG_SENSOR_COM22, 0x81);
    atk_mc2640_write_reg(ATK_MC2640_REG_SENSOR_COM22, 0x01);
}
#endif

/**
 * @brief  设置光照模式(自动/晴天/阴天/办公室/室内)
 * @param  mode: 光照模式
 * @retval ATK_MC2640_EOK: 成功; ATK_MC2640_EINVAL: 参数错误
 */
uint8_t atk_mc2640_set_light_mode(atk_mc2640_light_mode_t mode)
{
    switch (mode)
    {
        case ATK_MC2640_LIGHT_MODE_AUTO:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0xC7, 0x00);
            break;
        }
        case ATK_MC2640_LIGHT_MODE_SUNNY:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0xC7, 0x40);
            atk_mc2640_write_reg(0xCC, 0x5E);
            atk_mc2640_write_reg(0xCD, 0x41);
            atk_mc2640_write_reg(0xCE, 0x54);
            break;
        }
        case ATK_MC2640_LIGHT_MODE_CLOUDY:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0xC7, 0x40);
            atk_mc2640_write_reg(0xCC, 0x65);
            atk_mc2640_write_reg(0xCD, 0x41);
            atk_mc2640_write_reg(0xCE, 0x4F);
            break;
        }
        case ATK_MC2640_LIGHT_MODE_OFFICE:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0xC7, 0x40);
            atk_mc2640_write_reg(0xCC, 0x52);
            atk_mc2640_write_reg(0xCD, 0x41);
            atk_mc2640_write_reg(0xCE, 0x66);
            break;
        }
        case ATK_MC2640_LIGHT_MODE_HOME:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0xC7, 0x40);
            atk_mc2640_write_reg(0xCC, 0x42);
            atk_mc2640_write_reg(0xCD, 0x3F);
            atk_mc2640_write_reg(0xCE, 0x71);
            break;
        }
        default:
        {
            return ATK_MC2640_EINVAL;
        }
    }

    return ATK_MC2640_EOK;
}

/**
 * @brief  设置色彩饱和度(0~4级)
 * @param  saturation: 饱和度等级
 * @retval ATK_MC2640_EOK: 成功; ATK_MC2640_EINVAL: 参数错误
 */
uint8_t atk_mc2640_set_color_saturation(atk_mc2640_color_saturation_t saturation)
{
    switch (saturation)
    {
        case ATK_MC2640_COLOR_SATURATION_0:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0x7C, 0x00);
            atk_mc2640_write_reg(0x7D, 0x02);
            atk_mc2640_write_reg(0x7C, 0x03);
            atk_mc2640_write_reg(0x7D, 0x68);
            atk_mc2640_write_reg(0x7D, 0x68);
            break;
        }
        case ATK_MC2640_COLOR_SATURATION_1:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0x7C, 0x00);
            atk_mc2640_write_reg(0x7D, 0x02);
            atk_mc2640_write_reg(0x7C, 0x03);
            atk_mc2640_write_reg(0x7D, 0x58);
            atk_mc2640_write_reg(0x7D, 0x58);
            break;
        }
        case ATK_MC2640_COLOR_SATURATION_2:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0x7C, 0x00);
            atk_mc2640_write_reg(0x7D, 0x02);
            atk_mc2640_write_reg(0x7C, 0x03);
            atk_mc2640_write_reg(0x7D, 0x48);
            atk_mc2640_write_reg(0x7D, 0x48);
            break;
        }
        case ATK_MC2640_COLOR_SATURATION_3:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0x7C, 0x00);
            atk_mc2640_write_reg(0x7D, 0x02);
            atk_mc2640_write_reg(0x7C, 0x03);
            atk_mc2640_write_reg(0x7D, 0x38);
            atk_mc2640_write_reg(0x7D, 0x38);
            break;
        }
        case ATK_MC2640_COLOR_SATURATION_4:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0x7C, 0x00);
            atk_mc2640_write_reg(0x7D, 0x02);
            atk_mc2640_write_reg(0x7C, 0x03);
            atk_mc2640_write_reg(0x7D, 0x28);
            atk_mc2640_write_reg(0x7D, 0x28);
            break;
        }
        default:
        {
            return ATK_MC2640_EINVAL;
        }
    }

    return ATK_MC2640_EOK;
}

/**
 * @brief  设置亮度(0~4级)
 * @param  brightness: 亮度等级
 * @retval ATK_MC2640_EOK: 成功; ATK_MC2640_EINVAL: 参数错误
 */
uint8_t atk_mc2640_set_brightness(atk_mc2640_brightness_t brightness)
{
    switch (brightness)
    {
        case ATK_MC2640_BRIGHTNESS_0:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0x7C, 0x00);
            atk_mc2640_write_reg(0x7D, 0x04);
            atk_mc2640_write_reg(0x7C, 0x09);
            atk_mc2640_write_reg(0x7D, 0x40);
            atk_mc2640_write_reg(0x7D, 0x00);
            break;
        }
        case ATK_MC2640_BRIGHTNESS_1:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0x7C, 0x00);
            atk_mc2640_write_reg(0x7D, 0x04);
            atk_mc2640_write_reg(0x7C, 0x09);
            atk_mc2640_write_reg(0x7D, 0x30);
            atk_mc2640_write_reg(0x7D, 0x00);
            break;
        }
        case ATK_MC2640_BRIGHTNESS_2:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0x7C, 0x00);
            atk_mc2640_write_reg(0x7D, 0x04);
            atk_mc2640_write_reg(0x7C, 0x09);
            atk_mc2640_write_reg(0x7D, 0x20);
            atk_mc2640_write_reg(0x7D, 0x00);
            break;
        }
        case ATK_MC2640_BRIGHTNESS_3:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0x7C, 0x00);
            atk_mc2640_write_reg(0x7D, 0x04);
            atk_mc2640_write_reg(0x7C, 0x09);
            atk_mc2640_write_reg(0x7D, 0x10);
            atk_mc2640_write_reg(0x7D, 0x00);
            break;
        }
        case ATK_MC2640_BRIGHTNESS_4:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0x7C, 0x00);
            atk_mc2640_write_reg(0x7D, 0x04);
            atk_mc2640_write_reg(0x7C, 0x09);
            atk_mc2640_write_reg(0x7D, 0x00);
            atk_mc2640_write_reg(0x7D, 0x00);
            break;
        }
        default:
        {
            return ATK_MC2640_EINVAL;
        }
    }

    return ATK_MC2640_EOK;
}

/**
 * @brief  设置对比度(0~4级)
 * @param  contrast: 对比度等级
 * @retval ATK_MC2640_EOK: 成功; ATK_MC2640_EINVAL: 参数错误
 */
uint8_t atk_mc2640_set_contrast(atk_mc2640_contrast_t contrast)
{
    switch (contrast)
    {
        case ATK_MC2640_CONTRAST_0:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0x7C, 0x00);
            atk_mc2640_write_reg(0x7D, 0x04);
            atk_mc2640_write_reg(0x7C, 0x07);
            atk_mc2640_write_reg(0x7D, 0x20);
            atk_mc2640_write_reg(0x7D, 0x28);
            atk_mc2640_write_reg(0x7D, 0x0C);
            atk_mc2640_write_reg(0x7D, 0x06);
            break;
        }
        case ATK_MC2640_CONTRAST_1:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0x7C, 0x00);
            atk_mc2640_write_reg(0x7D, 0x04);
            atk_mc2640_write_reg(0x7C, 0x07);
            atk_mc2640_write_reg(0x7D, 0x20);
            atk_mc2640_write_reg(0x7D, 0x24);
            atk_mc2640_write_reg(0x7D, 0x16);
            atk_mc2640_write_reg(0x7D, 0x06);
            break;
        }
        case ATK_MC2640_CONTRAST_2:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0x7C, 0x00);
            atk_mc2640_write_reg(0x7D, 0x04);
            atk_mc2640_write_reg(0x7C, 0x07);
            atk_mc2640_write_reg(0x7D, 0x20);
            atk_mc2640_write_reg(0x7D, 0x20);
            atk_mc2640_write_reg(0x7D, 0x20);
            atk_mc2640_write_reg(0x7D, 0x06);
            break;
        }
        case ATK_MC2640_CONTRAST_3:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0x7C, 0x00);
            atk_mc2640_write_reg(0x7D, 0x04);
            atk_mc2640_write_reg(0x7C, 0x07);
            atk_mc2640_write_reg(0x7D, 0x20);
            atk_mc2640_write_reg(0x7D, 0x1C);
            atk_mc2640_write_reg(0x7D, 0x2A);
            atk_mc2640_write_reg(0x7D, 0x06);
            break;
        }
        case ATK_MC2640_CONTRAST_4:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0x7C, 0x00);
            atk_mc2640_write_reg(0x7D, 0x04);
            atk_mc2640_write_reg(0x7C, 0x07);
            atk_mc2640_write_reg(0x7D, 0x20);
            atk_mc2640_write_reg(0x7D, 0x18);
            atk_mc2640_write_reg(0x7D, 0x34);
            atk_mc2640_write_reg(0x7D, 0x06);
            break;
        }
        default:
        {
            return ATK_MC2640_EINVAL;
        }
    }

    return ATK_MC2640_EOK;
}

/**
 * @brief  设置特效模式(复古/偏蓝/偏绿/偏红/黑白/负片/黑白负片/正常)
 * @param  effect: 特效类型
 * @retval ATK_MC2640_EOK: 成功; ATK_MC2640_EINVAL: 参数错误
 */
uint8_t atk_mc2640_set_special_effect(atk_mc2640_special_effect_t effect)
{
    switch (effect)
    {
        case ATK_MC2640_SPECIAL_EFFECT_ANTIQUE:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0x7C, 0x00);
            atk_mc2640_write_reg(0x7D, 0x18);
            atk_mc2640_write_reg(0x7C, 0x05);
            atk_mc2640_write_reg(0x7D, 0x40);
            atk_mc2640_write_reg(0x7D, 0xA6);
            break;
        }
        case ATK_MC2640_SPECIAL_EFFECT_BLUISH:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0x7C, 0x00);
            atk_mc2640_write_reg(0x7D, 0x18);
            atk_mc2640_write_reg(0x7C, 0x05);
            atk_mc2640_write_reg(0x7D, 0xA0);
            atk_mc2640_write_reg(0x7D, 0x40);
            break;
        }
        case ATK_MC2640_SPECIAL_EFFECT_GREENISH:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0x7C, 0x00);
            atk_mc2640_write_reg(0x7D, 0x18);
            atk_mc2640_write_reg(0x7C, 0x05);
            atk_mc2640_write_reg(0x7D, 0x40);
            atk_mc2640_write_reg(0x7D, 0x40);
            break;
        }
        case ATK_MC2640_SPECIAL_EFFECT_REDISH:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0x7C, 0x00);
            atk_mc2640_write_reg(0x7D, 0x18);
            atk_mc2640_write_reg(0x7C, 0x05);
            atk_mc2640_write_reg(0x7D, 0x40);
            atk_mc2640_write_reg(0x7D, 0xC0);
            break;
        }
        case ATK_MC2640_SPECIAL_EFFECT_BW:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0x7C, 0x00);
            atk_mc2640_write_reg(0x7D, 0x18);
            atk_mc2640_write_reg(0x7C, 0x05);
            atk_mc2640_write_reg(0x7D, 0x80);
            atk_mc2640_write_reg(0x7D, 0x80);
            break;
        }
        case ATK_MC2640_SPECIAL_EFFECT_NEGATIVE:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0x7C, 0x00);
            atk_mc2640_write_reg(0x7D, 0x40);
            atk_mc2640_write_reg(0x7C, 0x05);
            atk_mc2640_write_reg(0x7D, 0x80);
            atk_mc2640_write_reg(0x7D, 0x80);
            break;
        }
        case ATK_MC2640_SPECIAL_EFFECT_BW_NEGATIVE:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0x7C, 0x00);
            atk_mc2640_write_reg(0x7D, 0x58);
            atk_mc2640_write_reg(0x7C, 0x05);
            atk_mc2640_write_reg(0x7D, 0x80);
            atk_mc2640_write_reg(0x7D, 0x80);
            break;
        }
        case ATK_MC2640_SPECIAL_EFFECT_NORMAL:
        {
            atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
            atk_mc2640_write_reg(0x7C, 0x00);
            atk_mc2640_write_reg(0x7D, 0x00);
            atk_mc2640_write_reg(0x7C, 0x05);
            atk_mc2640_write_reg(0x7D, 0x80);
            atk_mc2640_write_reg(0x7D, 0x80);
            break;
        }
        default:
        {
            return ATK_MC2640_EINVAL;
        }
    }

    return ATK_MC2640_EOK;
}

/**
 * @brief  设置输出格式(RGB565或JPEG)
 * @param  format: 输出格式
 * @retval ATK_MC2640_EOK: 成功; ATK_MC2640_EINVAL: 参数错误
 */
uint8_t atk_mc2640_set_output_format(atk_mc2640_output_format_t format)
{
    uint32_t cfg_index;

    switch (format)
    {
        case ATK_MC2640_OUTPUT_FORMAT_RGB565:
        {
            for (cfg_index=0; cfg_index<(sizeof(atk_mc2640_set_rgb565_cfg)/sizeof(atk_mc2640_set_rgb565_cfg[0])); cfg_index++)
            {
                atk_mc2640_write_reg(atk_mc2640_set_rgb565_cfg[cfg_index][0], atk_mc2640_set_rgb565_cfg[cfg_index][1]);
            }
            break;
        }
        case ATK_MC2640_OUTPUT_FORMAT_JPEG:
        {
            for (cfg_index=0; cfg_index<(sizeof(atk_mc2640_set_yuv422_cfg)/sizeof(atk_mc2640_set_yuv422_cfg[0])); cfg_index++)
            {
                atk_mc2640_write_reg(atk_mc2640_set_yuv422_cfg[cfg_index][0], atk_mc2640_set_yuv422_cfg[cfg_index][1]);
            }
            for (cfg_index=0; cfg_index<(sizeof(atk_mc2640_set_jpeg_cfg)/sizeof(atk_mc2640_set_jpeg_cfg[0])); cfg_index++)
            {
                atk_mc2640_write_reg(atk_mc2640_set_jpeg_cfg[cfg_index][0], atk_mc2640_set_jpeg_cfg[cfg_index][1]);
            }
            break;
        }
        default:
        {
            return ATK_MC2640_EINVAL;
        }
    }

    return ATK_MC2640_EOK;
}

/**
 * @brief  设置输出图像尺寸(宽高必须4的倍数)
 * @param  width: 输出宽度
 * @param  height: 输出高度
 * @retval ATK_MC2640_EOK: 成功; ATK_MC2640_EINVAL: 参数错误
 */
uint8_t atk_mc2640_set_output_size(uint16_t width, uint16_t height)
{
    uint16_t output_width;
    uint16_t output_height;

    if (((width & (4 - 1)) != 0) || ((height & (4 - 1)) != 0))
    {
        return ATK_MC2640_EINVAL;
    }

    g_atk_mc2640_sta.output.width = width;
    g_atk_mc2640_sta.output.height = height;

    output_width = width >> 2;
    output_height = height >> 2;

    atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
    atk_mc2640_write_reg(ATK_MC2640_REG_DSP_RESET, 0x04);
    atk_mc2640_write_reg(ATK_MC2640_REG_DSP_ZMOW, (uint8_t)(output_width & 0x00FF));
    atk_mc2640_write_reg(ATK_MC2640_REG_DSP_ZMOH, (uint8_t)(output_height & 0x00FF));
    atk_mc2640_write_reg(ATK_MC2640_REG_DSP_ZMHH, ((uint8_t)(output_width >> 8) & 0x03) | ((uint8_t)(output_height >> 6) & 0x04));
    atk_mc2640_write_reg(ATK_MC2640_REG_DSP_RESET, 0x00);

    return ATK_MC2640_EOK;
}

/**
 * @brief  设置传感器窗口(设置感光区域起始坐标和大小)
 * @param  start_x: 起始X坐标
 * @param  start_y: 起始Y坐标
 * @param  width: 窗口宽度
 * @param  height: 窗口高度
 * @retval 无
 */
void atk_mc2640_set_sensor_window(uint16_t start_x, uint16_t start_y, uint16_t width, uint16_t height)
{
    uint16_t end_x;
    uint16_t end_y;
    uint8_t raw_com1;
    uint8_t com1;
    uint8_t raw_reg32;
    uint8_t reg32;

    end_x = start_x + (width >> 1);
    end_y = start_y + (height >> 1);

    atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_SENSOR);

    raw_com1 = atk_mc2640_read_reg(ATK_MC2640_REG_SENSOR_COM1);
    com1 = (raw_com1 & 0xF0) | (((end_y & 0x03) << 2) | (start_y & 0x03));
    atk_mc2640_write_reg(ATK_MC2640_REG_SENSOR_COM1, com1);
    atk_mc2640_write_reg(ATK_MC2640_REG_SENSOR_VSTRT, start_y >> 2);
    atk_mc2640_write_reg(ATK_MC2640_REG_SENSOR_VEND, end_y >> 2);

    raw_reg32 = atk_mc2640_read_reg(ATK_MC2640_REG_SENSOR_REG32);
    reg32 = (raw_reg32 & 0xC0) | (((end_x & 0x07) << 3) | (start_x & 0x07));
    atk_mc2640_write_reg(ATK_MC2640_REG_SENSOR_REG32, reg32);
    atk_mc2640_write_reg(ATK_MC2640_REG_SENSOR_HREFST, start_x >> 3);
    atk_mc2640_write_reg(ATK_MC2640_REG_SENSOR_HREFEND, end_x >> 3);
}

/**
 * @brief  设置图像窗口(DSP输出图像的偏移和尺寸，宽高必须4的倍数)
 * @param  off_x: X方向偏移
 * @param  off_y: Y方向偏移
 * @param  width: 窗口宽度
 * @param  height: 窗口高度
 * @retval ATK_MC2640_EOK: 成功; ATK_MC2640_EINVAL: 参数错误
 */
uint8_t atk_mc2640_set_image_window(uint16_t off_x, uint16_t off_y, uint16_t width, uint16_t height)
{
    uint16_t hsize;
    uint16_t vsize;
    uint8_t vhyx;

    if (((width & (4 - 1)) != 0) || ((height & (4 - 1)) != 0))
    {
        return ATK_MC2640_EINVAL;
    }

    hsize = width >> 2;
    vsize = height >> 2;

    vhyx = (uint8_t)(((vsize >> 1) & 0x80) | ((off_y >> 4) & 0x70) | ((hsize >> 5) & 0x08) | ((off_x >> 8) & 0x07));

    atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
    atk_mc2640_write_reg(ATK_MC2640_REG_DSP_RESET, 0x04);
    atk_mc2640_write_reg(ATK_MC2640_REG_DSP_HSIZE, (uint8_t)(hsize & 0x00FF));
    atk_mc2640_write_reg(ATK_MC2640_REG_DSP_VSIZE, (uint8_t)(vsize & 0x00FF));
    atk_mc2640_write_reg(ATK_MC2640_REG_DSP_XOFFL, (uint8_t)(off_x & 0x00FF));
    atk_mc2640_write_reg(ATK_MC2640_REG_DSP_YOFFL, (uint8_t)(off_y & 0x00FF));
    atk_mc2640_write_reg(ATK_MC2640_REG_DSP_VHYX, vhyx);
    atk_mc2640_write_reg(ATK_MC2640_REG_DSP_TEST, (uint8_t)((hsize >> 2) & 0x80));
    atk_mc2640_write_reg(ATK_MC2640_REG_DSP_RESET, 0x00);

    return ATK_MC2640_EOK;
}

/**
 * @brief  设置图像尺寸(DSP图像处理尺寸)
 * @param  width: 图像宽度
 * @param  height: 图像高度
 * @retval 无
 */
void atk_mc2640_set_image_size(uint16_t width, uint16_t height)
{
    uint8_t sizel;

    sizel = (uint8_t)(((width & 0x07) << 3) | (height & 0x07) | ((width >> 4) & 0x80));

    atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
    atk_mc2640_write_reg(ATK_MC2640_REG_DSP_RESET, 0x04);
    atk_mc2640_write_reg(ATK_MC2640_REG_DSP_HSIZE8, (uint8_t)((width >> 3) & 0x00FF));
    atk_mc2640_write_reg(ATK_MC2640_REG_DSP_VSIZE8, (uint8_t)((height >> 3) & 0x00FF));
    atk_mc2640_write_reg(ATK_MC2640_REG_DSP_SIZEL, sizel);
    atk_mc2640_write_reg(ATK_MC2640_REG_DSP_RESET, 0x00);
}

/**
 * @brief  设置输出速度(时钟分频)
 * @param  clk_dev: 输入时钟分频值(0~63)
 * @param  pclk_dev: PCLK分频值(1~127)
 * @retval ATK_MC2640_EOK: 成功; ATK_MC2640_EINVAL: 参数错误
 */
uint8_t atk_mc2640_set_output_speed(uint8_t clk_dev, uint8_t pclk_dev)
{
    if (clk_dev > 63)
    {
        return ATK_MC2640_EINVAL;
    }

    if ((pclk_dev == 0) || (pclk_dev > 127))
    {
        return ATK_MC2640_EINVAL;
    }

    atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_DSP);
    atk_mc2640_write_reg(ATK_MC2640_REG_DSP_R_DVP_SP, pclk_dev);
    atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_SENSOR);
    atk_mc2640_write_reg(ATK_MC2640_REG_SENSOR_CLKRC, clk_dev);

    return ATK_MC2640_EOK;
}

/**
 * @brief  使能彩条测试模式
 * @retval 无
 */
void atk_mc2640_colorbar_enable(void)
{
    uint8_t com7;

    atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_SENSOR);
    com7 = atk_mc2640_read_reg(ATK_MC2640_REG_SENSOR_COM7);
    com7 |= (uint8_t)(1 << 1);
    atk_mc2640_write_reg(ATK_MC2640_REG_SENSOR_COM7, com7);
}

/**
 * @brief  关闭彩条测试模式
 * @retval 无
 */
void atk_mc2640_colorbar_disable(void)
{
    uint8_t com7;

    atk_mc2640_reg_bank_select(ATK_MC2640_REG_BANK_SENSOR);
    com7 = atk_mc2640_read_reg(ATK_MC2640_REG_SENSOR_COM7);
    com7 &= ~(uint8_t)(1 << 1);
    atk_mc2640_write_reg(ATK_MC2640_REG_SENSOR_COM7, com7);
}

#if (ATK_MC2640_USING_DCMI != 0)
/**
 * @brief  获取一帧图像数据(通过DCMI+DMA传输)
 * @param  dts_addr: 目标内存地址
 * @param  type: 获取类型(8/16/32位，地址是否递增)
 * @param  before_transfer: 传输前的回调函数(可为NULL)
 * @retval ATK_MC2640_EOK: 成功; ATK_MC2640_EINVAL: 参数错误
 */
uint8_t atk_mc2640_get_frame(uint32_t dts_addr, atk_mc2640_get_type_t type, void (*before_transfer)(void))
{
    uint32_t meminc;
    uint32_t memdataalignment;
    uint32_t len;

    switch (type)
    {
        case ATK_MC2640_GET_TYPE_DTS_8B_NOINC:
        {
            meminc = DMA_DINC_FIXED;
            memdataalignment = DMA_DEST_DATAWIDTH_BYTE;
            len = (g_atk_mc2640_sta.output.width * g_atk_mc2640_sta.output.height) / sizeof(uint8_t);
            break;
        }
        case ATK_MC2640_GET_TYPE_DTS_8B_INC:
        {
            meminc = DMA_DINC_INCREMENTED;
            memdataalignment = DMA_DEST_DATAWIDTH_BYTE;
            len = (g_atk_mc2640_sta.output.width * g_atk_mc2640_sta.output.height) / sizeof(uint8_t);
            break;
        }
        case ATK_MC2640_GET_TYPE_DTS_16B_NOINC:
        {
            meminc = DMA_DINC_FIXED;
            memdataalignment = DMA_DEST_DATAWIDTH_HALFWORD;
            len = (g_atk_mc2640_sta.output.width * g_atk_mc2640_sta.output.height) / sizeof(uint16_t);
            break;
        }
        case ATK_MC2640_GET_TYPE_DTS_16B_INC:
        {
            meminc = DMA_DINC_INCREMENTED;
            memdataalignment = DMA_DEST_DATAWIDTH_HALFWORD;
            len = (g_atk_mc2640_sta.output.width * g_atk_mc2640_sta.output.height) / sizeof(uint16_t);
            break;
        }
        case ATK_MC2640_GET_TYPE_DTS_32B_NOINC:
        {
            meminc = DMA_DINC_FIXED;
            memdataalignment = DMA_DEST_DATAWIDTH_WORD;
            len = (g_atk_mc2640_sta.output.width * g_atk_mc2640_sta.output.height) / sizeof(uint32_t);
            break;
        }
        case ATK_MC2640_GET_TYPE_DTS_32B_INC:
        {
            meminc = DMA_DINC_INCREMENTED;
            memdataalignment = DMA_DEST_DATAWIDTH_WORD;
            len = (g_atk_mc2640_sta.output.width * g_atk_mc2640_sta.output.height) / sizeof(uint32_t);
            break;
        }
        default:
        {
            return ATK_MC2640_EINVAL;
        }
    }

    if (before_transfer != NULL)
    {
        before_transfer();
    }

    return atk_mc2640_dcmi_start(dts_addr, meminc, memdataalignment, len);
}
#else
uint8_t atk_mc2640_get_frame(uint32_t dts_addr, atk_mc2640_get_type_t type, void (*before_transfer)(void))
{
    (void)dts_addr;
    (void)type;
    (void)before_transfer;
    return ATK_MC2640_EINVAL;
}
#endif
