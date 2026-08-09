#include "lcd.h"
#include "font.h"
#include "delay.h"

/* 全局变量 */
u32 POINT_COLOR = 0xFF000000;
u32 BACK_COLOR = 0xFFFFFFFF;
_lcd_dev lcddev;

/* 延迟 */
static void opt_delay(u8 i) { while (i--); }
static u32 LCD_Pow(u8 m, u8 n) { u32 r = 1; while (n--) r *= m; return r; }

/* ---- 底层寄存器操作 ---- */
void LCD_WR_REG(u16 regval) { LCD->LCD_REG = regval; }
void LCD_WR_DATA(u16 data)  { LCD->LCD_RAM = data; }
u16 LCD_RD_DATA(void)       { return LCD->LCD_RAM; }

void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{
    LCD->LCD_REG = LCD_Reg;
    LCD->LCD_RAM = LCD_RegValue;
}

u16 LCD_ReadReg(u16 LCD_Reg)
{
    LCD_WR_REG(LCD_Reg);
    opt_delay(5);
    return LCD_RD_DATA();
}

void LCD_WriteRAM_Prepare(void) { LCD->LCD_REG = lcddev.wramcmd; }
void LCD_WriteRAM(u16 RGB_Code) { LCD->LCD_RAM = RGB_Code; }

/* ---- MPU配置 ---- */
void LCD_MPU_Config(void)
{
    MPU_Region_InitTypeDef MPU_Initure;
    HAL_MPU_Disable();
    MPU_Initure.Enable = MPU_REGION_ENABLE;
    MPU_Initure.Number = LCD_REGION_NUMBER;
    MPU_Initure.BaseAddress = LCD_ADDRESS_START;
    MPU_Initure.Size = LCD_REGION_SIZE;
    MPU_Initure.SubRegionDisable = 0X00;
    MPU_Initure.TypeExtField = MPU_TEX_LEVEL0;
    MPU_Initure.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_Initure.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
    MPU_Initure.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_Initure.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_Initure.IsBufferable = MPU_ACCESS_BUFFERABLE;
    HAL_MPU_ConfigRegion(&MPU_Initure);
    HAL_MPU_Enable(MPU_HFNMI_PRIVDEF);
}

/* ---- FMC GPIO初始化 ---- */
static void LCD_FMC_GPIO_Init(void)
{
    GPIO_InitTypeDef g;
    __HAL_RCC_FMC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    g.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7|
            GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    g.Mode = GPIO_MODE_AF_PP;
    g.Pull = GPIO_PULLUP;
    g.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    g.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOD, &g);

    g.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|
            GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &g);
}

/* ---- FMC初始化(直接寄存器访问) ---- */
void LCD_FMC_Init(void)
{
    LCD_FMC_GPIO_Init();

    /* BTCR[0]=BCR1: FMCEN=1,WFDIS=1,EXTMOD=1,WREN=1,MWID=16bit,MBKEN=1 */
    FMC_Bank1_R->BTCR[0] = FMC_BCR1_FMCEN | FMC_BCR1_WFDIS |
                           FMC_BCRx_EXTMOD | FMC_BCRx_WREN |
                           FMC_BCRx_MWID_0 | FMC_BCRx_MBKEN;

    /* BTCR[1]=BTR1读时序: ADDSET=17, DATAST=85, ModeA */
    FMC_Bank1_R->BTCR[1] = (85UL << FMC_BTRx_DATAST_Pos) |
                           (17UL << FMC_BTRx_ADDSET_Pos);

    /* BWTR[0]=BWTR1写时序: ADDSET=21, DATAST=21, ModeA */
    FMC_Bank1E_R->BWTR[0] = (21UL << FMC_BWTRx_DATAST_Pos) |
                            (21UL << FMC_BWTRx_ADDSET_Pos);
    HAL_Delay(50);
}

/* ---- LCD初始化 ---- */
void LCD_Init(void)
{
    GPIO_InitTypeDef g;

    /* 背光PB5 */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    g.Pin = LCD_BL_Pin;
    g.Mode = GPIO_MODE_OUTPUT_PP;
    g.Pull = GPIO_PULLUP;
    g.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(LCD_BL_GPIO_Port, &g);

    LCD_MPU_Config();
    LCD_FMC_Init();
    LCD_WR_REG(0X0100);
    HAL_Delay(120);

    /* ---- 读取LCD ID ---- */
    LCD_WR_REG(0XD3);
    lcddev.id = LCD_RD_DATA();
    lcddev.id = LCD_RD_DATA();
    lcddev.id = LCD_RD_DATA();
    lcddev.id <<= 8;
    lcddev.id |= LCD_RD_DATA();

    if (lcddev.id != 0X9341) {
        LCD_WR_REG(0X04);
        lcddev.id = LCD_RD_DATA();
        lcddev.id = LCD_RD_DATA();
        lcddev.id <<= 8;
        lcddev.id |= LCD_RD_DATA();
        if (lcddev.id == 0X8552) lcddev.id = 0x7789;

        if (lcddev.id != 0x7789) {
            LCD_WR_REG(0XD4);
            lcddev.id = LCD_RD_DATA();
            lcddev.id = LCD_RD_DATA();
            lcddev.id = LCD_RD_DATA();
            lcddev.id <<= 8;
            lcddev.id |= LCD_RD_DATA();

            if (lcddev.id != 0X5310) {
                LCD_WR_REG(0XD3);
                lcddev.id = LCD_RD_DATA();
                lcddev.id = LCD_RD_DATA();
                lcddev.id = LCD_RD_DATA();
                lcddev.id <<= 8;
                lcddev.id |= LCD_RD_DATA();

                if (lcddev.id != 0x7796) {
                    LCD_WriteReg(0xF000, 0x0055);
                    LCD_WriteReg(0xF001, 0x00AA);
                    LCD_WriteReg(0xF002, 0x0052);
                    LCD_WriteReg(0xF003, 0x0008);
                    LCD_WriteReg(0xF004, 0x0001);

                    LCD_WR_REG(0xC500);
                    lcddev.id = LCD_RD_DATA();
                    lcddev.id <<= 8;
                    LCD_WR_REG(0xC501);
                    lcddev.id |= LCD_RD_DATA();
                    HAL_Delay(5);

                    if (lcddev.id != 0X5510) {
                        LCD_WR_REG(0XD3);
                        lcddev.id = LCD_RD_DATA();
                        lcddev.id = LCD_RD_DATA();
                        lcddev.id = LCD_RD_DATA();
                        lcddev.id <<= 8;
                        lcddev.id |= LCD_RD_DATA();
                        if (lcddev.id != 0x9806) {
                            LCD_WR_REG(0XA1);
                            lcddev.id = LCD_RD_DATA();
                            lcddev.id = LCD_RD_DATA();
                            lcddev.id <<= 8;
                            lcddev.id |= LCD_RD_DATA();
                            if (lcddev.id == 0X5761) lcddev.id = 0X1963;
                        }
                    }
                }
            }
        }
    }

    /* ---- NT35510初始化(0x5510) ---- */
    if (lcddev.id == 0X5510) {
        LCD_WriteReg(0xF000, 0x55); LCD_WriteReg(0xF001, 0xAA);
        LCD_WriteReg(0xF002, 0x52); LCD_WriteReg(0xF003, 0x08);
        LCD_WriteReg(0xF004, 0x01);

        /* 电源电压 */
        LCD_WriteReg(0xB000,0x0D); LCD_WriteReg(0xB001,0x0D); LCD_WriteReg(0xB002,0x0D);
        LCD_WriteReg(0xB600,0x34); LCD_WriteReg(0xB601,0x34); LCD_WriteReg(0xB602,0x34);
        LCD_WriteReg(0xB100,0x0D); LCD_WriteReg(0xB101,0x0D); LCD_WriteReg(0xB102,0x0D);
        LCD_WriteReg(0xB700,0x34); LCD_WriteReg(0xB701,0x34); LCD_WriteReg(0xB702,0x34);
        LCD_WriteReg(0xB200,0x00); LCD_WriteReg(0xB201,0x00); LCD_WriteReg(0xB202,0x00);
        LCD_WriteReg(0xB800,0x24); LCD_WriteReg(0xB801,0x24); LCD_WriteReg(0xB802,0x24);
        LCD_WriteReg(0xBF00,0x01);
        LCD_WriteReg(0xB300,0x0F); LCD_WriteReg(0xB301,0x0F); LCD_WriteReg(0xB302,0x0F);
        LCD_WriteReg(0xB900,0x34); LCD_WriteReg(0xB901,0x34); LCD_WriteReg(0xB902,0x34);
        LCD_WriteReg(0xB500,0x08); LCD_WriteReg(0xB501,0x08); LCD_WriteReg(0xB502,0x08);
        LCD_WriteReg(0xC200,0x03);
        LCD_WriteReg(0xBA00,0x24); LCD_WriteReg(0xBA01,0x24); LCD_WriteReg(0xBA02,0x24);
        LCD_WriteReg(0xBC00,0x00); LCD_WriteReg(0xBC01,0x78); LCD_WriteReg(0xBC02,0x00);
        LCD_WriteReg(0xBD00,0x00); LCD_WriteReg(0xBD01,0x78); LCD_WriteReg(0xBD02,0x00);
        LCD_WriteReg(0xBE00,0x00); LCD_WriteReg(0xBE01,0x64);

        /* Gamma D100-D133 (52 registers) */
        LCD_WriteReg(0xD100,0x00); LCD_WriteReg(0xD101,0x33); LCD_WriteReg(0xD102,0x00);
        LCD_WriteReg(0xD103,0x34); LCD_WriteReg(0xD104,0x00); LCD_WriteReg(0xD105,0x3A);
        LCD_WriteReg(0xD106,0x00); LCD_WriteReg(0xD107,0x4A); LCD_WriteReg(0xD108,0x00);
        LCD_WriteReg(0xD109,0x5C); LCD_WriteReg(0xD10A,0x00); LCD_WriteReg(0xD10B,0x81);
        LCD_WriteReg(0xD10C,0x00); LCD_WriteReg(0xD10D,0xA6); LCD_WriteReg(0xD10E,0x00);
        LCD_WriteReg(0xD10F,0xE5); LCD_WriteReg(0xD110,0x01); LCD_WriteReg(0xD111,0x13);
        LCD_WriteReg(0xD112,0x01); LCD_WriteReg(0xD113,0x54); LCD_WriteReg(0xD114,0x01);
        LCD_WriteReg(0xD115,0x82); LCD_WriteReg(0xD116,0x01); LCD_WriteReg(0xD117,0xCA);
        LCD_WriteReg(0xD118,0x02); LCD_WriteReg(0xD119,0x00); LCD_WriteReg(0xD11A,0x02);
        LCD_WriteReg(0xD11B,0x01); LCD_WriteReg(0xD11C,0x02); LCD_WriteReg(0xD11D,0x34);
        LCD_WriteReg(0xD11E,0x02); LCD_WriteReg(0xD11F,0x67); LCD_WriteReg(0xD120,0x02);
        LCD_WriteReg(0xD121,0x84); LCD_WriteReg(0xD122,0x02); LCD_WriteReg(0xD123,0xA4);
        LCD_WriteReg(0xD124,0x02); LCD_WriteReg(0xD125,0xB7); LCD_WriteReg(0xD126,0x02);
        LCD_WriteReg(0xD127,0xCF); LCD_WriteReg(0xD128,0x02); LCD_WriteReg(0xD129,0xDE);
        LCD_WriteReg(0xD12A,0x02); LCD_WriteReg(0xD12B,0xF2); LCD_WriteReg(0xD12C,0x02);
        LCD_WriteReg(0xD12D,0xFE); LCD_WriteReg(0xD12E,0x03); LCD_WriteReg(0xD12F,0x10);
        LCD_WriteReg(0xD130,0x03); LCD_WriteReg(0xD131,0x33); LCD_WriteReg(0xD132,0x03);
        LCD_WriteReg(0xD133,0x6D);

        /* Gamma D200-D233 */
        LCD_WriteReg(0xD200,0x00); LCD_WriteReg(0xD201,0x33); LCD_WriteReg(0xD202,0x00);
        LCD_WriteReg(0xD203,0x34); LCD_WriteReg(0xD204,0x00); LCD_WriteReg(0xD205,0x3A);
        LCD_WriteReg(0xD206,0x00); LCD_WriteReg(0xD207,0x4A); LCD_WriteReg(0xD208,0x00);
        LCD_WriteReg(0xD209,0x5C); LCD_WriteReg(0xD20A,0x00); LCD_WriteReg(0xD20B,0x81);
        LCD_WriteReg(0xD20C,0x00); LCD_WriteReg(0xD20D,0xA6); LCD_WriteReg(0xD20E,0x00);
        LCD_WriteReg(0xD20F,0xE5); LCD_WriteReg(0xD210,0x01); LCD_WriteReg(0xD211,0x13);
        LCD_WriteReg(0xD212,0x01); LCD_WriteReg(0xD213,0x54); LCD_WriteReg(0xD214,0x01);
        LCD_WriteReg(0xD215,0x82); LCD_WriteReg(0xD216,0x01); LCD_WriteReg(0xD217,0xCA);
        LCD_WriteReg(0xD218,0x02); LCD_WriteReg(0xD219,0x00); LCD_WriteReg(0xD21A,0x02);
        LCD_WriteReg(0xD21B,0x01); LCD_WriteReg(0xD21C,0x02); LCD_WriteReg(0xD21D,0x34);
        LCD_WriteReg(0xD21E,0x02); LCD_WriteReg(0xD21F,0x67); LCD_WriteReg(0xD220,0x02);
        LCD_WriteReg(0xD221,0x84); LCD_WriteReg(0xD222,0x02); LCD_WriteReg(0xD223,0xA4);
        LCD_WriteReg(0xD224,0x02); LCD_WriteReg(0xD225,0xB7); LCD_WriteReg(0xD226,0x02);
        LCD_WriteReg(0xD227,0xCF); LCD_WriteReg(0xD228,0x02); LCD_WriteReg(0xD229,0xDE);
        LCD_WriteReg(0xD22A,0x02); LCD_WriteReg(0xD22B,0xF2); LCD_WriteReg(0xD22C,0x02);
        LCD_WriteReg(0xD22D,0xFE); LCD_WriteReg(0xD22E,0x03); LCD_WriteReg(0xD22F,0x10);
        LCD_WriteReg(0xD230,0x03); LCD_WriteReg(0xD231,0x33); LCD_WriteReg(0xD232,0x03);
        LCD_WriteReg(0xD233,0x6D);

        /* Gamma D300-D333 */
        LCD_WriteReg(0xD300,0x00); LCD_WriteReg(0xD301,0x33); LCD_WriteReg(0xD302,0x00);
        LCD_WriteReg(0xD303,0x34); LCD_WriteReg(0xD304,0x00); LCD_WriteReg(0xD305,0x3A);
        LCD_WriteReg(0xD306,0x00); LCD_WriteReg(0xD307,0x4A); LCD_WriteReg(0xD308,0x00);
        LCD_WriteReg(0xD309,0x5C); LCD_WriteReg(0xD30A,0x00); LCD_WriteReg(0xD30B,0x81);
        LCD_WriteReg(0xD30C,0x00); LCD_WriteReg(0xD30D,0xA6); LCD_WriteReg(0xD30E,0x00);
        LCD_WriteReg(0xD30F,0xE5); LCD_WriteReg(0xD310,0x01); LCD_WriteReg(0xD311,0x13);
        LCD_WriteReg(0xD312,0x01); LCD_WriteReg(0xD313,0x54); LCD_WriteReg(0xD314,0x01);
        LCD_WriteReg(0xD315,0x82); LCD_WriteReg(0xD316,0x01); LCD_WriteReg(0xD317,0xCA);
        LCD_WriteReg(0xD318,0x02); LCD_WriteReg(0xD319,0x00); LCD_WriteReg(0xD31A,0x02);
        LCD_WriteReg(0xD31B,0x01); LCD_WriteReg(0xD31C,0x02); LCD_WriteReg(0xD31D,0x34);
        LCD_WriteReg(0xD31E,0x02); LCD_WriteReg(0xD31F,0x67); LCD_WriteReg(0xD320,0x02);
        LCD_WriteReg(0xD321,0x84); LCD_WriteReg(0xD322,0x02); LCD_WriteReg(0xD323,0xA4);
        LCD_WriteReg(0xD324,0x02); LCD_WriteReg(0xD325,0xB7); LCD_WriteReg(0xD326,0x02);
        LCD_WriteReg(0xD327,0xCF); LCD_WriteReg(0xD328,0x02); LCD_WriteReg(0xD329,0xDE);
        LCD_WriteReg(0xD32A,0x02); LCD_WriteReg(0xD32B,0xF2); LCD_WriteReg(0xD32C,0x02);
        LCD_WriteReg(0xD32D,0xFE); LCD_WriteReg(0xD32E,0x03); LCD_WriteReg(0xD32F,0x10);
        LCD_WriteReg(0xD330,0x03); LCD_WriteReg(0xD331,0x33); LCD_WriteReg(0xD332,0x03);
        LCD_WriteReg(0xD333,0x6D);

        /* Gamma D400-D433 */
        LCD_WriteReg(0xD400,0x00); LCD_WriteReg(0xD401,0x33); LCD_WriteReg(0xD402,0x00);
        LCD_WriteReg(0xD403,0x34); LCD_WriteReg(0xD404,0x00); LCD_WriteReg(0xD405,0x3A);
        LCD_WriteReg(0xD406,0x00); LCD_WriteReg(0xD407,0x4A); LCD_WriteReg(0xD408,0x00);
        LCD_WriteReg(0xD409,0x5C); LCD_WriteReg(0xD40A,0x00); LCD_WriteReg(0xD40B,0x81);
        LCD_WriteReg(0xD40C,0x00); LCD_WriteReg(0xD40D,0xA6); LCD_WriteReg(0xD40E,0x00);
        LCD_WriteReg(0xD40F,0xE5); LCD_WriteReg(0xD410,0x01); LCD_WriteReg(0xD411,0x13);
        LCD_WriteReg(0xD412,0x01); LCD_WriteReg(0xD413,0x54); LCD_WriteReg(0xD414,0x01);
        LCD_WriteReg(0xD415,0x82); LCD_WriteReg(0xD416,0x01); LCD_WriteReg(0xD417,0xCA);
        LCD_WriteReg(0xD418,0x02); LCD_WriteReg(0xD419,0x00); LCD_WriteReg(0xD41A,0x02);
        LCD_WriteReg(0xD41B,0x01); LCD_WriteReg(0xD41C,0x02); LCD_WriteReg(0xD41D,0x34);
        LCD_WriteReg(0xD41E,0x02); LCD_WriteReg(0xD41F,0x67); LCD_WriteReg(0xD420,0x02);
        LCD_WriteReg(0xD421,0x84); LCD_WriteReg(0xD422,0x02); LCD_WriteReg(0xD423,0xA4);
        LCD_WriteReg(0xD424,0x02); LCD_WriteReg(0xD425,0xB7); LCD_WriteReg(0xD426,0x02);
        LCD_WriteReg(0xD427,0xCF); LCD_WriteReg(0xD428,0x02); LCD_WriteReg(0xD429,0xDE);
        LCD_WriteReg(0xD42A,0x02); LCD_WriteReg(0xD42B,0xF2); LCD_WriteReg(0xD42C,0x02);
        LCD_WriteReg(0xD42D,0xFE); LCD_WriteReg(0xD42E,0x03); LCD_WriteReg(0xD42F,0x10);
        LCD_WriteReg(0xD430,0x03); LCD_WriteReg(0xD431,0x33); LCD_WriteReg(0xD432,0x03);
        LCD_WriteReg(0xD433,0x6D);

        /* --- LV2 Page 0 配置（参考实验31） --- */
        LCD_WriteReg(0xF000,0x0055); LCD_WriteReg(0xF001,0x00AA);
        LCD_WriteReg(0xF002,0x0052); LCD_WriteReg(0xF003,0x0008);
        LCD_WriteReg(0xF004,0x0000);  /* Page 0 */

        LCD_WriteReg(0xB100,0x00CC); LCD_WriteReg(0xB101,0x0000);  /* 显示控制 */
        LCD_WriteReg(0xB600,0x0005);  /* Source保持时间 */
        LCD_WriteReg(0xB700,0x0070); LCD_WriteReg(0xB701,0x0070);  /* Gate EQ */
        LCD_WriteReg(0xB800,0x0001); LCD_WriteReg(0xB801,0x0003);
        LCD_WriteReg(0xB802,0x0003); LCD_WriteReg(0xB803,0x0003);  /* Source EQ */
        LCD_WriteReg(0xBC00,0x0002); LCD_WriteReg(0xBC01,0x0000);
        LCD_WriteReg(0xBC02,0x0000);  /* 反转模式 */
        LCD_WriteReg(0xC900,0x00D0); LCD_WriteReg(0xC901,0x0002);
        LCD_WriteReg(0xC902,0x0050); LCD_WriteReg(0xC903,0x0050);
        LCD_WriteReg(0xC904,0x0050);  /* 时序控制 */
        LCD_WR_REG(0x3500); LCD_WR_DATA(0x00);  /* 内部时钟 */

        /* 像素格式: 16位色, 退出睡眠, 开显示 */
        LCD_WR_REG(0X3A00); LCD_WR_DATA(0x55);
        LCD_WR_REG(0X1100); HAL_Delay(100);
        LCD_WR_REG(0X2900);
    }

    LCD_Display_Dir(1);  /* 横屏 */
    LCD_LED(1);
    LCD_Clear(WHITE);
}

/* ---- 显示控制 ---- */
void LCD_DisplayOn(void)
{
    if (lcddev.id == 0X5510) LCD_WR_REG(0X2900);
    else LCD_WR_REG(0X29);
}
void LCD_DisplayOff(void)
{
    if (lcddev.id == 0X5510) LCD_WR_REG(0X2800);
    else LCD_WR_REG(0X28);
}

/* ---- 显示方向 ---- */
void LCD_Display_Dir(u8 dir)
{
    lcddev.dir = dir;
    if (dir == 0) {
        lcddev.width = 240; lcddev.height = 320;
        if (lcddev.id == 0x5510) { lcddev.wramcmd=0X2C00; lcddev.setxcmd=0X2A00; lcddev.setycmd=0X2B00; lcddev.width=480; lcddev.height=800; }
        else if (lcddev.id == 0X1963) { lcddev.wramcmd=0X2C; lcddev.setxcmd=0X2B; lcddev.setycmd=0X2A; lcddev.width=480; lcddev.height=800; }
        else { lcddev.wramcmd=0X2C; lcddev.setxcmd=0X2A; lcddev.setycmd=0X2B; }
        if (lcddev.id == 0X5310 || lcddev.id == 0x7796) { lcddev.width=320; lcddev.height=480; }
        if (lcddev.id == 0X9806) { lcddev.width=480; lcddev.height=800; }
    } else {
        lcddev.width = 320; lcddev.height = 240;
        if (lcddev.id == 0x5510) { lcddev.wramcmd=0X2C00; lcddev.setxcmd=0X2A00; lcddev.setycmd=0X2B00; lcddev.width=800; lcddev.height=480; }
        else if (lcddev.id == 0X1963 || lcddev.id == 0x9806) { lcddev.wramcmd=0X2C; lcddev.setxcmd=0X2A; lcddev.setycmd=0X2B; lcddev.width=800; lcddev.height=480; }
        else { lcddev.wramcmd=0X2C; lcddev.setxcmd=0X2A; lcddev.setycmd=0X2B; }
        if (lcddev.id == 0X5310 || lcddev.id == 0x7796) { lcddev.width=480; lcddev.height=320; }
    }
    LCD_Scan_Dir(DFT_SCAN_DIR);
}

/* ---- 扫描方向 ---- */
void LCD_Scan_Dir(u8 dir)
{
    u16 regval = 0, dirreg = 0, temp;

    if ((lcddev.dir == 1 && lcddev.id != 0X1963) || (lcddev.dir == 0 && lcddev.id == 0X1963)) {
        switch(dir) { case 0: dir=6; break; case 1: dir=7; break; case 2: dir=4; break;
            case 3: dir=5; break; case 4: dir=1; break; case 5: dir=0; break;
            case 6: dir=3; break; case 7: dir=2; break; }
    }
    switch(dir) {
        case L2R_U2D: regval|=(0<<7)|(0<<6)|(0<<5); break;
        case L2R_D2U: regval|=(1<<7)|(0<<6)|(0<<5); break;
        case R2L_U2D: regval|=(0<<7)|(1<<6)|(0<<5); break;
        case R2L_D2U: regval|=(1<<7)|(1<<6)|(0<<5); break;
        case U2D_L2R: regval|=(0<<7)|(0<<6)|(1<<5); break;
        case U2D_R2L: regval|=(0<<7)|(1<<6)|(1<<5); break;
        case D2U_L2R: regval|=(1<<7)|(0<<6)|(1<<5); break;
        case D2U_R2L: regval|=(1<<7)|(1<<6)|(1<<5); break;
    }
    dirreg = (lcddev.id == 0X5510) ? 0X3600 : 0X36;
    if (lcddev.id == 0X9341 || lcddev.id == 0X7789 || lcddev.id == 0x7796) regval |= 0X08;
    LCD_WriteReg(dirreg, regval);

    if (lcddev.id != 0X1963) {
        if (regval & 0X20) { if (lcddev.width<lcddev.height) { temp=lcddev.width; lcddev.width=lcddev.height; lcddev.height=temp; } }
        else { if (lcddev.width>lcddev.height) { temp=lcddev.width; lcddev.width=lcddev.height; lcddev.height=temp; } }
    }

    if (lcddev.id == 0X5510) {
        LCD_WR_REG(lcddev.setxcmd);   LCD_WR_DATA(0); LCD_WR_REG(lcddev.setxcmd+1); LCD_WR_DATA(0);
        LCD_WR_REG(lcddev.setxcmd+2); LCD_WR_DATA((lcddev.width-1)>>8);  LCD_WR_REG(lcddev.setxcmd+3); LCD_WR_DATA((lcddev.width-1)&0XFF);
        LCD_WR_REG(lcddev.setycmd);   LCD_WR_DATA(0); LCD_WR_REG(lcddev.setycmd+1); LCD_WR_DATA(0);
        LCD_WR_REG(lcddev.setycmd+2); LCD_WR_DATA((lcddev.height-1)>>8); LCD_WR_REG(lcddev.setycmd+3); LCD_WR_DATA((lcddev.height-1)&0XFF);
    } else {
        LCD_WR_REG(lcddev.setxcmd); LCD_WR_DATA(0); LCD_WR_DATA(0); LCD_WR_DATA((lcddev.width-1)>>8); LCD_WR_DATA((lcddev.width-1)&0XFF);
        LCD_WR_REG(lcddev.setycmd); LCD_WR_DATA(0); LCD_WR_DATA(0); LCD_WR_DATA((lcddev.height-1)>>8); LCD_WR_DATA((lcddev.height-1)&0XFF);
    }
}

/* ---- 光标/窗口 ---- */
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
    if (lcddev.id == 0X1963 && lcddev.dir == 0) Xpos = lcddev.width - 1 - Xpos;
    if (lcddev.id == 0X5510) {
        LCD_WR_REG(lcddev.setxcmd);   LCD_WR_DATA(Xpos>>8); LCD_WR_REG(lcddev.setxcmd+1); LCD_WR_DATA(Xpos&0XFF);
        LCD_WR_REG(lcddev.setycmd);   LCD_WR_DATA(Ypos>>8); LCD_WR_REG(lcddev.setycmd+1); LCD_WR_DATA(Ypos&0XFF);
    } else {
        LCD_WR_REG(lcddev.setxcmd);   LCD_WR_DATA(Xpos>>8); LCD_WR_DATA(Xpos&0XFF);
        LCD_WR_REG(lcddev.setycmd);   LCD_WR_DATA(Ypos>>8); LCD_WR_DATA(Ypos&0XFF);
    }
}

void LCD_Set_Window(u16 sx, u16 sy, u16 width, u16 height)
{
    u16 twidth = sx + width - 1, theight = sy + height - 1;
    if (lcddev.id == 0X1963 && lcddev.dir != 1) {
        sx = lcddev.width - width - sx; height = sy + height - 1;
        LCD_WR_REG(lcddev.setxcmd); LCD_WR_DATA(sx>>8); LCD_WR_DATA(sx&0XFF); LCD_WR_DATA((sx+width-1)>>8); LCD_WR_DATA((sx+width-1)&0XFF);
        LCD_WR_REG(lcddev.setycmd); LCD_WR_DATA(sy>>8); LCD_WR_DATA(sy&0XFF); LCD_WR_DATA(height>>8); LCD_WR_DATA(height&0XFF);
    } else if (lcddev.id == 0X5510) {
        LCD_WR_REG(lcddev.setxcmd);   LCD_WR_DATA(sx>>8); LCD_WR_REG(lcddev.setxcmd+1); LCD_WR_DATA(sx&0XFF);
        LCD_WR_REG(lcddev.setxcmd+2); LCD_WR_DATA(twidth>>8); LCD_WR_REG(lcddev.setxcmd+3); LCD_WR_DATA(twidth&0XFF);
        LCD_WR_REG(lcddev.setycmd);   LCD_WR_DATA(sy>>8); LCD_WR_REG(lcddev.setycmd+1); LCD_WR_DATA(sy&0XFF);
        LCD_WR_REG(lcddev.setycmd+2); LCD_WR_DATA(theight>>8); LCD_WR_REG(lcddev.setycmd+3); LCD_WR_DATA(theight&0XFF);
    } else {
        LCD_WR_REG(lcddev.setxcmd); LCD_WR_DATA(sx>>8); LCD_WR_DATA(sx&0XFF); LCD_WR_DATA(twidth>>8); LCD_WR_DATA(twidth&0XFF);
        LCD_WR_REG(lcddev.setycmd); LCD_WR_DATA(sy>>8); LCD_WR_DATA(sy&0XFF); LCD_WR_DATA(theight>>8); LCD_WR_DATA(theight&0XFF);
    }
}

/* ---- 绘图 ---- */
void LCD_DrawPoint(u16 x, u16 y)
{
    LCD_SetCursor(x, y);
    LCD_WriteRAM_Prepare();
    LCD->LCD_RAM = (u16)POINT_COLOR;
}

void LCD_Fast_DrawPoint(u16 x, u16 y, u32 color)
{
    if (lcddev.id == 0X5510) {
        LCD_WR_REG(lcddev.setxcmd);   LCD_WR_DATA(x>>8); LCD_WR_REG(lcddev.setxcmd+1); LCD_WR_DATA(x&0XFF);
        LCD_WR_REG(lcddev.setycmd);   LCD_WR_DATA(y>>8); LCD_WR_REG(lcddev.setycmd+1); LCD_WR_DATA(y&0XFF);
    } else if (lcddev.id == 0X1963) {
        if (lcddev.dir == 0) x = lcddev.width - 1 - x;
        LCD_WR_REG(lcddev.setxcmd); LCD_WR_DATA(x>>8); LCD_WR_DATA(x&0XFF); LCD_WR_DATA(x>>8); LCD_WR_DATA(x&0XFF);
        LCD_WR_REG(lcddev.setycmd); LCD_WR_DATA(y>>8); LCD_WR_DATA(y&0XFF); LCD_WR_DATA(y>>8); LCD_WR_DATA(y&0XFF);
    } else {
        LCD_WR_REG(lcddev.setxcmd); LCD_WR_DATA(x>>8); LCD_WR_DATA(x&0XFF);
        LCD_WR_REG(lcddev.setycmd); LCD_WR_DATA(y>>8); LCD_WR_DATA(y&0XFF);
    }
    LCD->LCD_REG = lcddev.wramcmd;
    LCD->LCD_RAM = (u16)color;
}

u32 LCD_ReadPoint(u16 x, u16 y)
{
    u16 r=0,g=0,b=0;
    if (x>=lcddev.width||y>=lcddev.height) return 0;
    LCD_SetCursor(x, y);
    LCD_WR_REG(lcddev.id==0X5510?0X2E00:0X2E);
    r = LCD_RD_DATA(); opt_delay(2);
    r = LCD_RD_DATA();
    if (lcddev.id==0X7796) return r;
    opt_delay(2); b = LCD_RD_DATA();
    g = r & 0XFF; g <<= 8;
    return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));
}

void LCD_Clear(u32 Color)
{
    u32 total = (u32)lcddev.width * lcddev.height;
    if (lcddev.id == 0X5510) {
        LCD_WR_REG(lcddev.setxcmd);   LCD_WR_DATA(0); LCD_WR_REG(lcddev.setxcmd+1); LCD_WR_DATA(0);
        LCD_WR_REG(lcddev.setxcmd+2); LCD_WR_DATA((lcddev.width-1)>>8); LCD_WR_REG(lcddev.setxcmd+3); LCD_WR_DATA((lcddev.width-1)&0XFF);
        LCD_WR_REG(lcddev.setycmd);   LCD_WR_DATA(0); LCD_WR_REG(lcddev.setycmd+1); LCD_WR_DATA(0);
        LCD_WR_REG(lcddev.setycmd+2); LCD_WR_DATA((lcddev.height-1)>>8); LCD_WR_REG(lcddev.setycmd+3); LCD_WR_DATA((lcddev.height-1)&0XFF);
    } else {
        LCD_WR_REG(lcddev.setxcmd); LCD_WR_DATA(0); LCD_WR_DATA(0); LCD_WR_DATA((lcddev.width-1)>>8); LCD_WR_DATA((lcddev.width-1)&0XFF);
        LCD_WR_REG(lcddev.setycmd); LCD_WR_DATA(0); LCD_WR_DATA(0); LCD_WR_DATA((lcddev.height-1)>>8); LCD_WR_DATA((lcddev.height-1)&0XFF);
    }
    LCD_WriteRAM_Prepare();
    LCD->LCD_RAM = (u16)Color;
    total--;
    while (total--) LCD->LCD_RAM = (u16)Color;
}

void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u32 color)
{
    u16 i,j;
    if (lcddev.id == 0X5510) {
        LCD_WR_REG(lcddev.setxcmd);   LCD_WR_DATA(sx>>8); LCD_WR_REG(lcddev.setxcmd+1); LCD_WR_DATA(sx&0XFF);
        LCD_WR_REG(lcddev.setxcmd+2); LCD_WR_DATA(ex>>8); LCD_WR_REG(lcddev.setxcmd+3); LCD_WR_DATA(ex&0XFF);
        LCD_WR_REG(lcddev.setycmd);   LCD_WR_DATA(sy>>8); LCD_WR_REG(lcddev.setycmd+1); LCD_WR_DATA(sy&0XFF);
        LCD_WR_REG(lcddev.setycmd+2); LCD_WR_DATA(ey>>8); LCD_WR_REG(lcddev.setycmd+3); LCD_WR_DATA(ey&0XFF);
    } else {
        LCD_WR_REG(lcddev.setxcmd); LCD_WR_DATA(sx>>8); LCD_WR_DATA(sx&0XFF); LCD_WR_DATA(ex>>8); LCD_WR_DATA(ex&0XFF);
        LCD_WR_REG(lcddev.setycmd); LCD_WR_DATA(sy>>8); LCD_WR_DATA(sy&0XFF); LCD_WR_DATA(ey>>8); LCD_WR_DATA(ey&0XFF);
    }
    LCD_WriteRAM_Prepare();
    for (i=0; i<=ey-sy; i++)
        for (j=0; j<=ex-sx; j++)
            LCD->LCD_RAM = (u16)color;
}

void LCD_Color_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 *color)
{
    u16 w=ex-sx+1, h=ey-sy+1, i, j;
    if (lcddev.id == 0X5510) {
        LCD_WR_REG(lcddev.setxcmd);   LCD_WR_DATA(sx>>8); LCD_WR_REG(lcddev.setxcmd+1); LCD_WR_DATA(sx&0XFF);
        LCD_WR_REG(lcddev.setxcmd+2); LCD_WR_DATA(ex>>8); LCD_WR_REG(lcddev.setxcmd+3); LCD_WR_DATA(ex&0XFF);
        LCD_WR_REG(lcddev.setycmd);   LCD_WR_DATA(sy>>8); LCD_WR_REG(lcddev.setycmd+1); LCD_WR_DATA(sy&0XFF);
        LCD_WR_REG(lcddev.setycmd+2); LCD_WR_DATA(ey>>8); LCD_WR_REG(lcddev.setycmd+3); LCD_WR_DATA(ey&0XFF);
    } else {
        LCD_WR_REG(lcddev.setxcmd); LCD_WR_DATA(sx>>8); LCD_WR_DATA(sx&0XFF); LCD_WR_DATA(ex>>8); LCD_WR_DATA(ex&0XFF);
        LCD_WR_REG(lcddev.setycmd); LCD_WR_DATA(sy>>8); LCD_WR_DATA(sy&0XFF); LCD_WR_DATA(ey>>8); LCD_WR_DATA(ey&0XFF);
    }
    LCD_WriteRAM_Prepare();
    for (i=0; i<h; i++)
        for (j=0; j<w; j++)
            LCD->LCD_RAM = color[i*w+j];
}

void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
    u16 t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy;
    delta_x = x2-x1; delta_y = y2-y1;
    if (delta_x>0) incx=1; else if (delta_x==0) incx=0; else { incx=-1; delta_x=-delta_x; }
    if (delta_y>0) incy=1; else if (delta_y==0) incy=0; else { incy=-1; delta_y=-delta_y; }
    if (delta_x>delta_y) distance=delta_x; else distance=delta_y;
    for (t=0; t<=distance+1; t++) {
        LCD_DrawPoint(x1, y1);
        xerr+=delta_x; yerr+=delta_y;
        if (xerr>distance) { xerr-=distance; x1+=incx; }
        if (yerr>distance) { yerr-=distance; y1+=incy; }
    }
}

void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
    LCD_DrawLine(x1,y1,x2,y1); LCD_DrawLine(x1,y1,x1,y2);
    LCD_DrawLine(x1,y2,x2,y2); LCD_DrawLine(x2,y1,x2,y2);
}

void LCD_Draw_Circle(u16 x0, u16 y0, u8 r)
{
    int a=0, b=r, di=3-2*r;
    while (a<=b) {
        LCD_DrawPoint(x0-b,y0-a); LCD_DrawPoint(x0+b,y0-a);
        LCD_DrawPoint(x0-a,y0+b); LCD_DrawPoint(x0+a,y0+b);
        LCD_DrawPoint(x0-b,y0+a); LCD_DrawPoint(x0+b,y0+a);
        LCD_DrawPoint(x0-a,y0-b); LCD_DrawPoint(x0+a,y0-b);
        a++;
        if (di<0) di+=4*a+6;
        else { di+=10+4*(a-b); b--; }
    }
}

/* ---- 字符显示 ---- */
void LCD_ShowChar(u16 x, u16 y, u8 num, u8 size, u8 mode)
{
    u8 temp, t1, t;
    u8 csize = (size==12)?12:16;
    const u8 *p = ASCII_8x16 + (num-32)*16;

    for (t=0; t<csize; t++) {
        temp = p[t];
        for (t1=0; t1<8; t1++) {
            if (temp & 0x80) LCD_Fast_DrawPoint(x+t1, y+t, POINT_COLOR);
            else if (mode==0) LCD_Fast_DrawPoint(x+t1, y+t, BACK_COLOR);
            temp <<= 1;
        }
    }
}

void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size)
{
    u8 t, temp, enshow=0;
    for (t=0; t<len; t++) {
        temp = (num/LCD_Pow(10,len-t-1))%10;
        if (enshow==0 && t<(len-1)) { if (temp==0) { LCD_ShowChar(x+(size/2)*t,y,' ',size,0); continue; } else enshow=1; }
        LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0);
    }
}

void LCD_ShowxNum(u16 x, u16 y, u32 num, u8 len, u8 size, u8 mode)
{
    u8 t, temp, enshow=0;
    for (t=0; t<len; t++) {
        temp = (num/LCD_Pow(10,len-t-1))%10;
        if (enshow==0 && t<(len-1)) { if (temp==0) { if (mode&0X04) LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);
            else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01); continue; } else enshow=1; }
        LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01);
    }
}

void LCD_ShowString(u16 x, u16 y, u16 width, u16 height, u8 size, u8 *p)
{
    u8 x0 = x;
    u8 y0 = y;
    while (*p) {
        if (y > (y0 + height - 1)) return;
        if (x > (x0 + width - 1)) { x = x0; y += size; }
        LCD_ShowChar(x, y, *p, size, 0);
        x += size / 2;
        p++;
    }
}
