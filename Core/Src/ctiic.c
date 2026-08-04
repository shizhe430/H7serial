#include "ctiic.h"
#include "delay.h"

static void CT_Delay(void)
{
    delay_us(3U);
}

void CT_IIC_Init(void)
{
    GPIO_InitTypeDef g;

    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();

    g.Pin = GPIO_PIN_6;
    g.Mode = GPIO_MODE_OUTPUT_OD;
    g.Pull = GPIO_PULLUP;
    g.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOH, &g);

    g.Pin = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOI, &g);

    CT_IIC_SDA(1);
    CT_IIC_SCL(1);
}

void CT_IIC_Start(void)
{
    CT_IIC_SDA(1);
    CT_IIC_SCL(1);
    CT_Delay();
    CT_IIC_SDA(0);
    CT_Delay();
    CT_IIC_SCL(0);
    CT_Delay();
}

void CT_IIC_Stop(void)
{
    CT_IIC_SDA(0);
    CT_Delay();
    CT_IIC_SCL(1);
    CT_Delay();
    CT_IIC_SDA(1);
    CT_Delay();
}

u8 CT_IIC_Wait_Ack(void)
{
    u8 t = 0U;

    CT_IIC_SDA(1);
    CT_Delay();
    CT_IIC_SCL(1);
    CT_Delay();
    while (CT_READ_SDA)
    {
        t++;
        if (t > 250U)
        {
            CT_IIC_Stop();
            return 1U;
        }
        CT_Delay();
    }
    CT_IIC_SCL(0);
    CT_Delay();
    return 0U;
}

void CT_IIC_Ack(void)
{
    CT_IIC_SDA(0);
    CT_Delay();
    CT_IIC_SCL(1);
    CT_Delay();
    CT_IIC_SCL(0);
    CT_Delay();
    CT_IIC_SDA(1);
    CT_Delay();
}

void CT_IIC_NAck(void)
{
    CT_IIC_SDA(1);
    CT_Delay();
    CT_IIC_SCL(1);
    CT_Delay();
    CT_IIC_SCL(0);
    CT_Delay();
}

void CT_IIC_Send_Byte(u8 txd)
{
    u8 t;

    for (t = 0U; t < 8U; t++)
    {
        CT_IIC_SDA((txd & 0x80U) >> 7U);
        txd <<= 1U;
        CT_Delay();
        CT_IIC_SCL(1);
        CT_Delay();
        CT_IIC_SCL(0);
        CT_Delay();
    }
    CT_IIC_SDA(1);
}

u8 CT_IIC_Read_Byte(unsigned char ack)
{
    u8 i;
    u8 receive = 0U;

    CT_IIC_SDA(1);
    for (i = 0U; i < 8U; i++)
    {
        receive <<= 1U;
        CT_IIC_SCL(1);
        CT_Delay();
        if (CT_READ_SDA)
        {
            receive++;
        }
        CT_IIC_SCL(0);
        CT_Delay();
    }
    if (!ack)
    {
        CT_IIC_NAck();
    }
    else
    {
        CT_IIC_Ack();
    }
    return receive;
}
