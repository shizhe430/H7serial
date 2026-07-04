#include "ov2640_sccb.h"
#include "main.h"

/*
 * OV2640 SCCB 软件模拟（与 U5 参考代码 atk_mc2640_sccb.c 完全对齐）：
 * - PB4: SCL (OUTPUT_OD + PULLUP)
 * - PB3: SDA (OUTPUT_OD + PULLUP)
 * - SDA 始终保持在 OUTPUT_OD 模式，读数据时释放总线后直接读 IDR
 * - 不检查 ACK (SCCB 第9位为 Don't Care)
 * - DWT 微秒延时
 */

#define OV2640_SCCB_DELAY_US_MIN   5U

static uint8_t s_dwt_ready = 0U;

/* 直接寄存器操作，绕过 HAL 开销 */
#define SCCB_SCL_SET()   (GPIOB->BSRR = GPIO_PIN_4)
#define SCCB_SCL_CLR()   (GPIOB->BSRR = (uint32_t)GPIO_PIN_4 << 16U)
#define SCCB_SDA_SET()   (GPIOB->BSRR = GPIO_PIN_3)
#define SCCB_SDA_CLR()   (GPIOB->BSRR = (uint32_t)GPIO_PIN_3 << 16U)
#define SCCB_SDA_READ()  ((GPIOB->IDR & GPIO_PIN_3) != 0U)

static void sccb_scl_hi(void) { SCCB_SCL_SET(); }
static void sccb_scl_lo(void) { SCCB_SCL_CLR(); }
static void sccb_sda_hi(void) { SCCB_SDA_SET(); }
static void sccb_sda_lo(void) { SCCB_SDA_CLR(); }
static GPIO_PinState sccb_sda_read(void) { return SCCB_SDA_READ() ? GPIO_PIN_SET : GPIO_PIN_RESET; }

static void sccb_delay_us(uint32_t us)
{
    uint32_t start;
    uint32_t ticks;

    if (us < OV2640_SCCB_DELAY_US_MIN)
    {
        us = OV2640_SCCB_DELAY_US_MIN;
    }

    if (s_dwt_ready == 0U)
    {
        volatile uint32_t i;
        while (us--)
        {
            for (i = 0U; i < 25U; i++)
            {
                __NOP();
            }
        }
        return;
    }

    ticks = us * (SystemCoreClock / 1000000U);
    start = DWT->CYCCNT;
    while ((DWT->CYCCNT - start) < ticks)
    {
        __NOP();
    }
}

/* ── 总线原语（与 U5 完全一致） ── */

static void sccb_start(void)
{
    sccb_sda_hi();
    sccb_scl_hi();
    sccb_delay_us(5U);
    sccb_sda_lo();
    sccb_delay_us(5U);
    sccb_scl_lo();
}

static void sccb_stop(void)
{
    sccb_sda_lo();
    sccb_delay_us(5U);
    sccb_scl_hi();
    sccb_delay_us(5U);
    sccb_sda_hi();
    sccb_delay_us(5U);
}

/*
 * 写一字节（与 U5 atk_mc2640_sccb_write_byte 一致）：
 * - MSB first
 * - 先设 SDA 值，再发 SCL 脉冲
 * - 第9位 Don't Care：SDA=1, SCL 脉冲（不检查 ACK）
 */
static void sccb_write_byte(uint8_t data)
{
    int8_t i;

    for (i = 7; i >= 0; i--)
    {
        if ((data >> i) & 0x01U)
        {
            sccb_sda_hi();
        }
        else
        {
            sccb_sda_lo();
        }
        sccb_delay_us(5U);
        sccb_scl_hi();
        sccb_delay_us(5U);
        sccb_scl_lo();
    }

    /* Don't Care 位 */
    sccb_sda_hi();
    sccb_delay_us(5U);
    sccb_scl_hi();
    sccb_delay_us(5U);
    sccb_scl_lo();
}

/*
 * 读一字节（与 U5 atk_mc2640_sccb_read_byte 一致）：
 * - SDA 保持 OUTPUT_OD 模式不变
 * - 释放 SDA(=1) 后，直接读 IDR 获取从设备数据
 * - MSB first
 * - 最后发 NACK (SDA=1)
 */
static uint8_t sccb_read_byte(void)
{
    int8_t i;
    uint8_t data = 0U;
    uint8_t bit;

    sccb_sda_hi();              /* 释放 SDA，从设备驱动数据线 */

    for (i = 7; i >= 0; i--)
    {
        sccb_delay_us(5U);
        sccb_scl_hi();
        bit = (sccb_sda_read() != GPIO_PIN_RESET) ? 1U : 0U;
        data |= (bit << i);
        sccb_delay_us(5U);
        sccb_scl_lo();
    }

    /* NACK (Master 不拉低 SDA，表示读结束) */
    sccb_delay_us(5U);
    sccb_scl_hi();
    sccb_delay_us(5U);
    sccb_scl_lo();
    sccb_delay_us(5U);
    sccb_sda_lo();              /* 释放从设备 */
    sccb_delay_us(5U);

    return data;
}

/* ── 公开接口 ── */

void OV2640_SCCB_Init(void)
{
    GPIO_InitTypeDef gpio = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* SCL: 开漏 + 上拉 */
    gpio.Pin = OV2640_SCCB_SCL_GPIO_PIN;
    gpio.Mode = GPIO_MODE_OUTPUT_OD;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpio);

    /* SDA: 开漏 + 上拉（与 U5 相同，ATK_MC2640_SCCB_GPIO_PULLUP=0） */
    gpio.Pin = OV2640_SCCB_SDA_GPIO_PIN;
    gpio.Mode = GPIO_MODE_OUTPUT_OD;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpio);

    sccb_sda_hi();
    sccb_scl_hi();

    /* DWT 初始化 */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0U;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    s_dwt_ready = 1U;
}

HAL_StatusTypeDef OV2640_SCCB_WriteReg(uint8_t dev, uint8_t reg, uint8_t data)
{
    sccb_start();
    sccb_write_byte(dev & 0xFEU);
    sccb_write_byte(reg);
    sccb_write_byte(data);
    sccb_stop();
    return HAL_OK;
}

HAL_StatusTypeDef OV2640_SCCB_ReadReg(uint8_t dev, uint8_t reg, uint8_t *data)
{
    if (data == NULL)
    {
        return HAL_ERROR;
    }

    /* 第1相: 写寄存器地址 */
    sccb_start();
    sccb_write_byte(dev & 0xFEU);
    sccb_write_byte(reg);
    sccb_stop();

    /* 第2相: 读数据（独立事务） */
    sccb_start();
    sccb_write_byte((uint8_t)(dev | 0x01U));
    *data = sccb_read_byte();
    sccb_stop();

    return HAL_OK;
}
