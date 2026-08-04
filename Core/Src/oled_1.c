#include "oled_1.h"
#include "oledfont.h"
#include <stdio.h>
#include <string.h>

#define LCD_WIDTH_DEFAULT       800U
#define LCD_HEIGHT_DEFAULT      480U
#define LCD_BASE_ADDR           ((uint32_t)(0x60000000UL | 0x0007FFFEUL))
#define LCD_MPU_REGION          MPU_REGION_NUMBER2

#define LCD_COLOR_WHITE         0xFFFFU
#define LCD_COLOR_BLACK         0x0000U
#define LCD_COLOR_BLUE          0x001FU
#define LCD_COLOR_CYAN          0x07FFU
#define LCD_COLOR_GREEN         0x07E0U
#define LCD_COLOR_RED           0xF800U
#define LCD_COLOR_ORANGE        0xFCA0U
#define LCD_COLOR_GRAY          0x8410U
#define LCD_COLOR_LIGHT_GRAY    0xC618U
#define LCD_COLOR_DARK          0x2104U
#define LCD_COLOR_PANEL         0xF7BEU
#define LCD_COLOR_NAVY          0x0210U

#define LCD_SCAN_L2R_U2D        0U

typedef struct
{
    volatile uint16_t LCD_REG;
    volatile uint16_t LCD_RAM;
} lcd_bus_t;

typedef struct
{
    uint16_t width;
    uint16_t height;
    uint16_t id;
    uint8_t dir;
    uint16_t wramcmd;
    uint16_t setxcmd;
    uint16_t setycmd;
} lcd_dev_t;

static lcd_dev_t s_lcd = {
    LCD_WIDTH_DEFAULT,
    LCD_HEIGHT_DEFAULT,
    0U,
    1U,
    0x2C00U,
    0x2A00U,
    0x2B00U
};

static uint16_t s_fg_color = LCD_COLOR_BLACK;
static uint16_t s_bg_color = LCD_COLOR_WHITE;
static uint8_t s_lcd_ready = 0U;

static u8 last_water_level = 0xFFU;
static u8 last_water_out_state = 0xFFU;
static u8 last_hot_cold_mode = 0xFFU;
static u16 last_temp_val = 0xFFFFU;
static u16 last_dev_id = 0xFFFFU;
static u8 oled_init_draw = 0U;

#define LCD_BUS ((lcd_bus_t *)LCD_BASE_ADDR)

static void lcd_write_reg(uint16_t reg)
{
    LCD_BUS->LCD_REG = reg;
}

static void lcd_write_data(uint16_t data)
{
    LCD_BUS->LCD_RAM = data;
}

static uint16_t lcd_read_data(void)
{
    return LCD_BUS->LCD_RAM;
}

static void lcd_write_reg_value(uint16_t reg, uint16_t value)
{
    lcd_write_reg(reg);
    lcd_write_data(value);
}

static void lcd_mpu_config(void)
{
    MPU_Region_InitTypeDef region = {0};

    HAL_MPU_Disable();
    region.Enable = MPU_REGION_ENABLE;
    region.Number = LCD_MPU_REGION;
    region.BaseAddress = 0x60000000UL;
    region.Size = MPU_REGION_SIZE_256MB;
    region.SubRegionDisable = 0x00U;
    region.TypeExtField = MPU_TEX_LEVEL0;
    region.AccessPermission = MPU_REGION_FULL_ACCESS;
    region.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
    region.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    region.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    region.IsBufferable = MPU_ACCESS_BUFFERABLE;
    HAL_MPU_ConfigRegion(&region);
    HAL_MPU_Enable(MPU_HFNMI_PRIVDEF);
}

static void lcd_backlight_init(void)
{
    GPIO_InitTypeDef gpio = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();
    gpio.Pin = LCD_BL_Pin;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(LCD_BL_GPIO_Port, &gpio);
    HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);
}

static void lcd_fmc_gpio_init(void)
{
    GPIO_InitTypeDef gpio = {0};

    __HAL_RCC_FMC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio.Alternate = GPIO_AF12_FMC;

    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 |
               GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
               GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOD, &gpio);

    gpio.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
               GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |
               GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &gpio);
}

static void lcd_fmc_init(void)
{
    lcd_fmc_gpio_init();

    FMC_Bank1_R->BTCR[0] = FMC_BCR1_FMCEN | FMC_BCR1_WFDIS |
                           FMC_BCRx_EXTMOD | FMC_BCRx_WREN |
                           FMC_BCRx_MWID_0 | FMC_BCRx_MBKEN;
    FMC_Bank1_R->BTCR[1] = (85UL << FMC_BTRx_DATAST_Pos) |
                           (17UL << FMC_BTRx_ADDSET_Pos);
    FMC_Bank1E_R->BWTR[0] = (21UL << FMC_BWTRx_DATAST_Pos) |
                            (21UL << FMC_BWTRx_ADDSET_Pos);
    HAL_Delay(50U);
}

static uint16_t lcd_read_id(void)
{
    uint16_t id;

    lcd_write_reg(0xD3U);
    (void)lcd_read_data();
    (void)lcd_read_data();
    id = lcd_read_data();
    id <<= 8;
    id |= lcd_read_data();
    if (id == 0x9341U)
    {
        return id;
    }

    lcd_write_reg(0x04U);
    (void)lcd_read_data();
    id = lcd_read_data();
    id <<= 8;
    id |= lcd_read_data();
    if (id == 0x8552U)
    {
        return 0x7789U;
    }
    if (id == 0x7789U)
    {
        return id;
    }

    lcd_write_reg(0xD4U);
    (void)lcd_read_data();
    (void)lcd_read_data();
    id = lcd_read_data();
    id <<= 8;
    id |= lcd_read_data();
    if (id == 0x5310U)
    {
        return id;
    }

    lcd_write_reg_value(0xF000U, 0x0055U);
    lcd_write_reg_value(0xF001U, 0x00AAU);
    lcd_write_reg_value(0xF002U, 0x0052U);
    lcd_write_reg_value(0xF003U, 0x0008U);
    lcd_write_reg_value(0xF004U, 0x0001U);
    lcd_write_reg(0xC500U);
    id = lcd_read_data();
    id <<= 8;
    lcd_write_reg(0xC501U);
    id |= lcd_read_data();
    HAL_Delay(5U);
    return id;
}

static void lcd_init_nt35510(void)
{
    lcd_write_reg_value(0xF000U, 0x55U);
    lcd_write_reg_value(0xF001U, 0xAAU);
    lcd_write_reg_value(0xF002U, 0x52U);
    lcd_write_reg_value(0xF003U, 0x08U);
    lcd_write_reg_value(0xF004U, 0x01U);

    lcd_write_reg_value(0xB000U, 0x0DU); lcd_write_reg_value(0xB001U, 0x0DU); lcd_write_reg_value(0xB002U, 0x0DU);
    lcd_write_reg_value(0xB600U, 0x34U); lcd_write_reg_value(0xB601U, 0x34U); lcd_write_reg_value(0xB602U, 0x34U);
    lcd_write_reg_value(0xB100U, 0x0DU); lcd_write_reg_value(0xB101U, 0x0DU); lcd_write_reg_value(0xB102U, 0x0DU);
    lcd_write_reg_value(0xB700U, 0x34U); lcd_write_reg_value(0xB701U, 0x34U); lcd_write_reg_value(0xB702U, 0x34U);
    lcd_write_reg_value(0xB200U, 0x00U); lcd_write_reg_value(0xB201U, 0x00U); lcd_write_reg_value(0xB202U, 0x00U);
    lcd_write_reg_value(0xB800U, 0x24U); lcd_write_reg_value(0xB801U, 0x24U); lcd_write_reg_value(0xB802U, 0x24U);
    lcd_write_reg_value(0xBF00U, 0x01U);
    lcd_write_reg_value(0xB300U, 0x0FU); lcd_write_reg_value(0xB301U, 0x0FU); lcd_write_reg_value(0xB302U, 0x0FU);
    lcd_write_reg_value(0xB900U, 0x34U); lcd_write_reg_value(0xB901U, 0x34U); lcd_write_reg_value(0xB902U, 0x34U);
    lcd_write_reg_value(0xB500U, 0x08U); lcd_write_reg_value(0xB501U, 0x08U); lcd_write_reg_value(0xB502U, 0x08U);
    lcd_write_reg_value(0xC200U, 0x03U);
    lcd_write_reg_value(0xBA00U, 0x24U); lcd_write_reg_value(0xBA01U, 0x24U); lcd_write_reg_value(0xBA02U, 0x24U);
    lcd_write_reg_value(0xBC00U, 0x00U); lcd_write_reg_value(0xBC01U, 0x78U); lcd_write_reg_value(0xBC02U, 0x00U);
    lcd_write_reg_value(0xBD00U, 0x00U); lcd_write_reg_value(0xBD01U, 0x78U); lcd_write_reg_value(0xBD02U, 0x00U);
    lcd_write_reg_value(0xBE00U, 0x00U); lcd_write_reg_value(0xBE01U, 0x64U);

    lcd_write_reg_value(0xF000U, 0x0055U);
    lcd_write_reg_value(0xF001U, 0x00AAU);
    lcd_write_reg_value(0xF002U, 0x0052U);
    lcd_write_reg_value(0xF003U, 0x0008U);
    lcd_write_reg_value(0xF004U, 0x0000U);
    lcd_write_reg_value(0xB100U, 0x00CCU);
    lcd_write_reg_value(0xB101U, 0x0000U);
    lcd_write_reg_value(0xB600U, 0x0005U);
    lcd_write_reg_value(0xB700U, 0x0070U);
    lcd_write_reg_value(0xB701U, 0x0070U);
    lcd_write_reg_value(0xB800U, 0x0001U);
    lcd_write_reg_value(0xB801U, 0x0003U);
    lcd_write_reg_value(0xB802U, 0x0003U);
    lcd_write_reg_value(0xB803U, 0x0003U);
    lcd_write_reg_value(0xBC00U, 0x0002U);
    lcd_write_reg_value(0xBC01U, 0x0000U);
    lcd_write_reg_value(0xBC02U, 0x0000U);
    lcd_write_reg_value(0xC900U, 0x00D0U);
    lcd_write_reg_value(0xC901U, 0x0002U);
    lcd_write_reg_value(0xC902U, 0x0050U);
    lcd_write_reg_value(0xC903U, 0x0050U);
    lcd_write_reg_value(0xC904U, 0x0050U);
    lcd_write_reg(0x3500U);
    lcd_write_data(0x00U);
    lcd_write_reg(0x3A00U);
    lcd_write_data(0x55U);
    lcd_write_reg(0x1100U);
    HAL_Delay(100U);
    lcd_write_reg(0x2900U);

    s_lcd.wramcmd = 0x2C00U;
    s_lcd.setxcmd = 0x2A00U;
    s_lcd.setycmd = 0x2B00U;
    s_lcd.width = LCD_WIDTH_DEFAULT;
    s_lcd.height = LCD_HEIGHT_DEFAULT;
    s_lcd.dir = 1U;
    lcd_write_reg_value(0x3600U, 0x28U);
}

static void lcd_set_window(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey)
{
    if (sx >= s_lcd.width)
    {
        sx = (uint16_t)(s_lcd.width - 1U);
    }
    if (sy >= s_lcd.height)
    {
        sy = (uint16_t)(s_lcd.height - 1U);
    }
    if (ex >= s_lcd.width)
    {
        ex = (uint16_t)(s_lcd.width - 1U);
    }
    if (ey >= s_lcd.height)
    {
        ey = (uint16_t)(s_lcd.height - 1U);
    }

    if (s_lcd.id == 0x5510U)
    {
        lcd_write_reg(s_lcd.setxcmd);     lcd_write_data((uint16_t)(sx >> 8));
        lcd_write_reg(s_lcd.setxcmd + 1U); lcd_write_data((uint16_t)(sx & 0xFFU));
        lcd_write_reg(s_lcd.setxcmd + 2U); lcd_write_data((uint16_t)(ex >> 8));
        lcd_write_reg(s_lcd.setxcmd + 3U); lcd_write_data((uint16_t)(ex & 0xFFU));
        lcd_write_reg(s_lcd.setycmd);     lcd_write_data((uint16_t)(sy >> 8));
        lcd_write_reg(s_lcd.setycmd + 1U); lcd_write_data((uint16_t)(sy & 0xFFU));
        lcd_write_reg(s_lcd.setycmd + 2U); lcd_write_data((uint16_t)(ey >> 8));
        lcd_write_reg(s_lcd.setycmd + 3U); lcd_write_data((uint16_t)(ey & 0xFFU));
    }
    else
    {
        lcd_write_reg(s_lcd.setxcmd);
        lcd_write_data((uint16_t)(sx >> 8));
        lcd_write_data((uint16_t)(sx & 0xFFU));
        lcd_write_data((uint16_t)(ex >> 8));
        lcd_write_data((uint16_t)(ex & 0xFFU));
        lcd_write_reg(s_lcd.setycmd);
        lcd_write_data((uint16_t)(sy >> 8));
        lcd_write_data((uint16_t)(sy & 0xFFU));
        lcd_write_data((uint16_t)(ey >> 8));
        lcd_write_data((uint16_t)(ey & 0xFFU));
    }
}

static void lcd_fill_rect(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color)
{
    uint32_t count;

    if ((sx > ex) || (sy > ey))
    {
        return;
    }

    lcd_set_window(sx, sy, ex, ey);
    lcd_write_reg(s_lcd.wramcmd);
    count = ((uint32_t)(ex - sx + 1U)) * ((uint32_t)(ey - sy + 1U));
    while (count-- != 0U)
    {
        lcd_write_data(color);
    }
}

static void lcd_draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    if ((x >= s_lcd.width) || (y >= s_lcd.height))
    {
        return;
    }

    lcd_set_window(x, y, x, y);
    lcd_write_reg(s_lcd.wramcmd);
    lcd_write_data(color);
}

static void lcd_draw_char_scaled(uint16_t x, uint16_t y, char ch,
                                 uint8_t scale, uint16_t fg, uint16_t bg,
                                 uint8_t transparent)
{
    uint8_t row;
    uint8_t col;
    uint8_t sx;
    uint8_t sy;
    const uint8_t *glyph;

    if ((ch < ' ') || (ch > '~'))
    {
        ch = ' ';
    }

    if (scale == 0U)
    {
        scale = 1U;
    }

    glyph = ascii_1608[(uint8_t)ch - (uint8_t)' '];
    for (row = 0U; row < 16U; row++)
    {
        uint8_t bits = glyph[row];
        for (col = 0U; col < 8U; col++)
        {
            uint16_t color;
            uint8_t on = ((bits & (0x80U >> col)) != 0U) ? 1U : 0U;

            if ((on == 0U) && (transparent != 0U))
            {
                continue;
            }
            color = (on != 0U) ? fg : bg;
            for (sy = 0U; sy < scale; sy++)
            {
                for (sx = 0U; sx < scale; sx++)
                {
                    lcd_draw_pixel((uint16_t)(x + (col * scale) + sx),
                                   (uint16_t)(y + (row * scale) + sy),
                                   color);
                }
            }
        }
    }
}

static void lcd_draw_text_scaled(uint16_t x, uint16_t y, const char *text,
                                 uint8_t scale, uint16_t fg, uint16_t bg,
                                 uint8_t transparent)
{
    uint16_t cursor = x;

    while ((text != NULL) && (*text != '\0'))
    {
        lcd_draw_char_scaled(cursor, y, *text, scale, fg, bg, transparent);
        cursor = (uint16_t)(cursor + (8U * scale) + scale);
        text++;
    }
}

static void lcd_draw_num_fixed(uint16_t x, uint16_t y, uint32_t num,
                               uint8_t width, uint8_t scale,
                               uint16_t fg, uint16_t bg)
{
    char buf[12];

    (void)snprintf(buf, sizeof(buf), "%0*lu", (int)width, (unsigned long)num);
    lcd_draw_text_scaled(x, y, buf, scale, fg, bg, 0U);
}

static void lcd_draw_card(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          const char *title)
{
    lcd_fill_rect(x, y, (uint16_t)(x + w - 1U), (uint16_t)(y + h - 1U), LCD_COLOR_PANEL);
    lcd_fill_rect(x, y, (uint16_t)(x + w - 1U), (uint16_t)(y + 4U), LCD_COLOR_BLUE);
    lcd_fill_rect(x, (uint16_t)(y + h - 5U), (uint16_t)(x + w - 1U),
                  (uint16_t)(y + h - 1U), LCD_COLOR_BLUE);
    lcd_fill_rect(x, y, (uint16_t)(x + 4U), (uint16_t)(y + h - 1U), LCD_COLOR_BLUE);
    lcd_fill_rect((uint16_t)(x + w - 5U), y, (uint16_t)(x + w - 1U),
                  (uint16_t)(y + h - 1U), LCD_COLOR_BLUE);
    lcd_draw_text_scaled((uint16_t)(x + 18U), (uint16_t)(y + 16U), title,
                         2U, LCD_COLOR_DARK, LCD_COLOR_PANEL, 1U);
}

void OLED_WR_REG(u8 reg)
{
    lcd_write_reg((uint16_t)reg);
}

void OLED_WR_Byte(u8 dat)
{
    lcd_write_data((uint16_t)dat);
}

void OLED_Init(void)
{
    if (s_lcd_ready != 0U)
    {
        return;
    }

    lcd_mpu_config();
    lcd_backlight_init();
    lcd_fmc_init();

    s_lcd.id = lcd_read_id();
    if (s_lcd.id == 0x5510U)
    {
        lcd_init_nt35510();
    }
    else
    {
        s_lcd.id = 0x5510U;
        lcd_init_nt35510();
    }

    s_lcd_ready = 1U;
    printf("[LCD] id=0x%04X size=%ux%u\r\n",
           (unsigned int)s_lcd.id,
           (unsigned int)s_lcd.width,
           (unsigned int)s_lcd.height);
    OLED_Clear();
}

void OLED_Clear(void)
{
    if (s_lcd_ready == 0U)
    {
        return;
    }
    lcd_fill_rect(0U, 0U, (uint16_t)(s_lcd.width - 1U),
                  (uint16_t)(s_lcd.height - 1U), LCD_COLOR_WHITE);
}

void OLED_Fill(u16 xstr, u8 ystr, u16 xend, u8 yend, u8 color)
{
    uint16_t rgb = (color == 0U) ? LCD_COLOR_BLACK : LCD_COLOR_WHITE;

    if (s_lcd_ready == 0U)
    {
        return;
    }

    lcd_fill_rect(xstr, ystr, xend, yend, rgb);
}

void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 sizey, u8 mode)
{
    uint8_t scale = (sizey >= 32U) ? 2U : 1U;
    uint16_t fg = (mode != 0U) ? s_bg_color : s_fg_color;
    uint16_t bg = (mode != 0U) ? s_fg_color : s_bg_color;

    lcd_draw_char_scaled(x, y, (char)chr, scale, fg, bg, 0U);
}

void OLED_ShowString(u8 x, u8 y, u8 *dp, u8 sizey, u8 mode)
{
    uint8_t scale = (sizey >= 32U) ? 2U : 1U;
    uint16_t fg = (mode != 0U) ? s_bg_color : s_fg_color;
    uint16_t bg = (mode != 0U) ? s_fg_color : s_bg_color;

    lcd_draw_text_scaled(x, y, (const char *)dp, scale, fg, bg, 0U);
}

static uint32_t oled_pow(u8 m, u8 n)
{
    uint32_t result = 1U;

    while (n-- != 0U)
    {
        result *= m;
    }
    return result;
}

void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 sizey, u8 mode)
{
    uint8_t scale = (sizey >= 32U) ? 2U : 1U;
    uint8_t i;
    uint16_t fg = (mode != 0U) ? s_bg_color : s_fg_color;
    uint16_t bg = (mode != 0U) ? s_fg_color : s_bg_color;

    for (i = 0U; i < len; i++)
    {
        uint8_t digit = (uint8_t)((num / oled_pow(10U, (u8)(len - i - 1U))) % 10U);
        lcd_draw_char_scaled((uint16_t)(x + (i * (8U * scale + scale))), y,
                             (char)('0' + digit), scale, fg, bg, 0U);
    }
}

void OLED_ShowChinese(u8 x, u8 y, u8 *s, u8 sizey, u8 mode)
{
    (void)x;
    (void)y;
    (void)s;
    (void)sizey;
    (void)mode;
}

void OLED_DrawBMP(u8 x, u8 y, u16 length, u8 width, const u8 BMP[], u8 mode)
{
    (void)x;
    (void)y;
    (void)length;
    (void)width;
    (void)BMP;
    (void)mode;
}

void OLED_DrawSingleBMP(u8 x, u8 y, u16 length, u8 width, const u8 BMP[], u8 mode)
{
    OLED_DrawBMP(x, y, length, width, BMP, mode);
}

void Disp_DrawStatic(void)
{
    if (s_lcd_ready == 0U)
    {
        return;
    }

    lcd_fill_rect(0U, 0U, 799U, 479U, LCD_COLOR_WHITE);
    lcd_fill_rect(0U, 0U, 799U, 74U, LCD_COLOR_NAVY);
    lcd_draw_text_scaled(28U, 18U, "SMART WATER DISPENSER", 3U,
                         LCD_COLOR_WHITE, LCD_COLOR_NAVY, 1U);

    lcd_draw_card(28U, 102U, 350U, 120U, "USER ID");
    lcd_draw_card(422U, 102U, 350U, 120U, "TEMPERATURE");
    lcd_draw_card(28U, 260U, 350U, 150U, "WATER LEVEL");
    lcd_draw_card(422U, 260U, 350U, 150U, "PUMP STATE");

    lcd_fill_rect(28U, 436U, 772U, 450U, LCD_COLOR_LIGHT_GRAY);
    lcd_draw_text_scaled(30U, 455U, "Camera0: water level    Camera1: face ID", 1U,
                         LCD_COLOR_GRAY, LCD_COLOR_WHITE, 1U);
}

void Disp_DrawDynamic(u8 water_level, u8 water_out_state, u16 temp_val,
                      u8 hot_cold_mode, u16 dev_id)
{
    const char *mode_text = (hot_cold_mode == 1U) ? "HOT" : "COLD";
    const char *level_text = (water_level == 1U) ? "OK" : "LOW";
    const char *state_text = "IDLE";
    uint16_t level_color = (water_level == 1U) ? LCD_COLOR_GREEN : LCD_COLOR_ORANGE;
    uint16_t state_color = LCD_COLOR_GRAY;

    if (water_out_state == 1U)
    {
        state_text = "RUNNING";
        state_color = LCD_COLOR_GREEN;
    }
    else if (water_out_state == 2U)
    {
        state_text = "ABNORMAL";
        state_color = LCD_COLOR_RED;
    }

    if ((dev_id != last_dev_id) || (oled_init_draw == 0U))
    {
        lcd_fill_rect(70U, 150U, 336U, 198U, LCD_COLOR_PANEL);
        lcd_draw_num_fixed(72U, 148U, dev_id, 3U, 3U, LCD_COLOR_BLACK, LCD_COLOR_PANEL);
        last_dev_id = dev_id;
    }

    if ((temp_val != last_temp_val) ||
        (hot_cold_mode != last_hot_cold_mode) ||
        (oled_init_draw == 0U))
    {
        lcd_fill_rect(466U, 145U, 728U, 202U, LCD_COLOR_PANEL);
        lcd_draw_num_fixed(466U, 148U, temp_val, 2U, 3U, LCD_COLOR_BLACK, LCD_COLOR_PANEL);
        lcd_draw_text_scaled(580U, 148U, "C", 3U, LCD_COLOR_BLACK, LCD_COLOR_PANEL, 1U);
        lcd_draw_text_scaled(654U, 162U, mode_text, 2U,
                             (hot_cold_mode == 1U) ? LCD_COLOR_ORANGE : LCD_COLOR_BLUE,
                             LCD_COLOR_PANEL, 1U);
        last_temp_val = temp_val;
        last_hot_cold_mode = hot_cold_mode;
    }

    if ((water_level != last_water_level) || (oled_init_draw == 0U))
    {
        lcd_fill_rect(70U, 324U, 336U, 372U, LCD_COLOR_PANEL);
        lcd_draw_text_scaled(72U, 320U, level_text, 3U, level_color, LCD_COLOR_PANEL, 1U);
        last_water_level = water_level;
    }

    if ((water_out_state != last_water_out_state) || (oled_init_draw == 0U))
    {
        lcd_fill_rect(466U, 324U, 730U, 372U, LCD_COLOR_PANEL);
        lcd_draw_text_scaled(466U, 320U, state_text, 3U, state_color, LCD_COLOR_PANEL, 1U);
        last_water_out_state = water_out_state;
    }
}

void Disp_DrinkerUI(u8 water_level, u8 water_out_state, u16 temp_val,
                    u8 hot_cold_mode, u16 dev_id)
{
    if (oled_init_draw == 0U)
    {
        Disp_DrawStatic();
        Disp_DrawDynamic(water_level, water_out_state, temp_val, hot_cold_mode, dev_id);
        oled_init_draw = 1U;
        return;
    }

    Disp_DrawDynamic(water_level, water_out_state, temp_val, hot_cold_mode, dev_id);
}
