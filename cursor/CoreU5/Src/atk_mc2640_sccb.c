/**
 ****************************************************************************************************
 * @file        atk_mc2640_sccb.c
 * @author      ALIENTEK
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MC2640模块SCCB接口驱动代码
 *              SCCB(OmniVision串行摄像头控制总线)与I2C类似，
 *              但只支持3相写和2相读，不支持连续读。
 *              使用GPIO开漏输出模拟SCCB时序，SCL:PB13, SDA:PB14
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

#include "atk_mc2640_sccb.h"
#include "delay.h"

#define ATK_MC2640_SCCB_WRITE   0x00
#define ATK_MC2640_SCCB_READ    0x01

#if (ATK_MC2640_SCCB_GPIO_PULLUP != 0)
static void atk_mc2640_sccb_set_sda_output(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};

    gpio_init_struct.Pin    = ATK_MC2640_SCCB_SDA_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MC2640_SCCB_SDA_GPIO_PORT, &gpio_init_struct);
}

static void atk_mc2640_sccb_set_sda_input(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};

    gpio_init_struct.Pin    = ATK_MC2640_SCCB_SDA_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_INPUT;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MC2640_SCCB_SDA_GPIO_PORT, &gpio_init_struct);
}
#endif

static inline void atk_mc2640_sccb_delay(void)
{
    delay_us(5);
}

static void atk_mc2640_sccb_start(void)
{
    ATK_MC2640_SCCB_SDA(1);
    ATK_MC2640_SCCB_SCL(1);
    atk_mc2640_sccb_delay();
    ATK_MC2640_SCCB_SDA(0);
    atk_mc2640_sccb_delay();
    ATK_MC2640_SCCB_SCL(0);
}

static void atk_mc2640_sccb_stop(void)
{
    ATK_MC2640_SCCB_SDA(0);
    atk_mc2640_sccb_delay();
    ATK_MC2640_SCCB_SCL(1);
    atk_mc2640_sccb_delay();
    ATK_MC2640_SCCB_SDA(1);
    atk_mc2640_sccb_delay();
}

static void atk_mc2640_sccb_write_byte(uint8_t dat)
{
    int8_t dat_index;
    uint8_t dat_bit;

    for (dat_index=7; dat_index>=0; dat_index--)
    {
        dat_bit = (dat >> dat_index) & 0x01;
        ATK_MC2640_SCCB_SDA(dat_bit);
        atk_mc2640_sccb_delay();
        ATK_MC2640_SCCB_SCL(1);
        atk_mc2640_sccb_delay();
        ATK_MC2640_SCCB_SCL(0);
    }

    ATK_MC2640_SCCB_SDA(1);
    atk_mc2640_sccb_delay();
    ATK_MC2640_SCCB_SCL(1);
    atk_mc2640_sccb_delay();
    ATK_MC2640_SCCB_SCL(0);
}

static void atk_mc2640_sccb_read_byte(uint8_t *dat)
{
    int8_t dat_index;
    uint8_t dat_bit;

    *dat = 0;

#if (ATK_MC2640_SCCB_GPIO_PULLUP != 0)
    atk_mc2640_sccb_set_sda_input();
#endif

    for (dat_index=7; dat_index>=0; dat_index--)
    {
        atk_mc2640_sccb_delay();
        ATK_MC2640_SCCB_SCL(1);
        dat_bit = ATK_MC2640_SCCB_READ_SDA();
        *dat |= (dat_bit << dat_index);
        atk_mc2640_sccb_delay();
        ATK_MC2640_SCCB_SCL(0);
    }

    atk_mc2640_sccb_delay();
    ATK_MC2640_SCCB_SCL(1);
    atk_mc2640_sccb_delay();
    ATK_MC2640_SCCB_SCL(0);
    atk_mc2640_sccb_delay();
    ATK_MC2640_SCCB_SDA(0);
    atk_mc2640_sccb_delay();

#if (ATK_MC2640_SCCB_GPIO_PULLUP != 0)
    atk_mc2640_sccb_set_sda_output();
#endif
}

void atk_mc2640_sccb_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};

    ATK_MC2640_SCCB_SCL_GPIO_CLK_ENABLE();
    ATK_MC2640_SCCB_SDA_GPIO_CLK_ENABLE();

    gpio_init_struct.Pin    = ATK_MC2640_SCCB_SCL_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_OD;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MC2640_SCCB_SCL_GPIO_PORT, &gpio_init_struct);

    gpio_init_struct.Pin    = ATK_MC2640_SCCB_SDA_GPIO_PIN;
#if (ATK_MC2640_SCCB_GPIO_PULLUP != 0)
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
#else
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_OD;
#endif
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MC2640_SCCB_SDA_GPIO_PORT, &gpio_init_struct);

    atk_mc2640_sccb_stop();
}

void atk_mc2640_sccb_3_phase_write(uint8_t id_addr, uint8_t sub_addr, uint8_t dat)
{
    atk_mc2640_sccb_start();
    atk_mc2640_sccb_write_byte((id_addr << 1) | ATK_MC2640_SCCB_WRITE);
    atk_mc2640_sccb_write_byte(sub_addr);
    atk_mc2640_sccb_write_byte(dat);
    atk_mc2640_sccb_stop();
}

void atk_mc2640_sccb_2_phase_write(uint8_t id_addr, uint8_t sub_addr)
{
    atk_mc2640_sccb_start();
    atk_mc2640_sccb_write_byte((id_addr << 1) | ATK_MC2640_SCCB_WRITE);
    atk_mc2640_sccb_write_byte(sub_addr);
    atk_mc2640_sccb_stop();
}

void atk_mc2640_sccb_2_phase_read(uint8_t id_addr, uint8_t *dat)
{
    atk_mc2640_sccb_start();
    atk_mc2640_sccb_write_byte((id_addr << 1) | ATK_MC2640_SCCB_READ);
    atk_mc2640_sccb_read_byte(dat);
    atk_mc2640_sccb_stop();
}
