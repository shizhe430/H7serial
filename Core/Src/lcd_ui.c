#include "lcd_ui.h"

#include "camera_app.h"
#include "lcd.h"
#include "touch.h"
#include "delay.h"
#include "usart.h"
#include "character_library.h"
#include "9th_logo2.h"
#include "wendu_logo.h"
#include "shuiwei_logo.h"
#include "id_logo.h"
#include "hot_water_logo.h"
#include "cold_water_logo.h"
#include "geren_logo.h"
#include "yinshuiji_logo.h"
#include "yinshui_logo.h"
#include "water_logo.h"
#include "jianyi_logo.h"
#include "yue_logo.h"
#include "background_img.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define LCD_UI_ORANGE          0xFCA0U
#define LCD_UI_CYAN_THEME      0x07FFU
#define LCD_UI_REFRESH_MS      150U
#define LCD_UI_TOUCH_SCAN_MS   20U

#define LCD_UI_PAGE_HOME       0U
#define LCD_UI_PAGE_INFO       1U

static uint8_t s_ui_ready;
static uint8_t s_touch_ready;
static uint8_t s_current_page = LCD_UI_PAGE_HOME;
static uint8_t s_touch_processed;
static uint32_t s_last_touch_scan_ms;

static uint8_t s_last_water_level = 0xFFU;
static uint8_t s_last_water_out_state = 0xFFU;
static uint8_t s_last_hot_cold_mode = 0xFFU;
static uint16_t s_last_temp_val = 0xFFFFU;
static uint16_t s_last_dev_id = 0xFFFFU;
static uint32_t s_last_status_ms;

static void lcd_ui_log(const char *text)
{
    if (text != NULL)
    {
        (void)HAL_UART_Transmit(&huart1, (uint8_t *)text, (uint16_t)strlen(text), 100U);
    }
}

static void lcd_ui_log_init_result(void)
{
    char msg[96];
    int len = snprintf(msg, sizeof(msg), "[LCD] id=0x%04X size=%ux%u touch=%u\r\n",
                       (unsigned int)lcddev.id,
                       (unsigned int)lcddev.width,
                       (unsigned int)lcddev.height,
                       (unsigned int)s_touch_ready);
    if (len > 0)
    {
        lcd_ui_log(msg);
    }
}

static u16 *lcd_ui_img(const unsigned char *img)
{
    return (u16 *)(uintptr_t)img;
}

static void lcd_ui_restore_background(u16 x1, u16 y1, u16 x2, u16 y2)
{
    const u16 *bg = (const u16 *)gImage_background;
    u16 y;

    for (y = y1; y <= y2; y++)
    {
        LCD_Color_Fill(x1, y, x2, y, (u16 *)&bg[(y * 800U) + x1]);
    }
}

static u16 lcd_ui_show_u16_32(u16 x, u16 y, uint16_t value, u16 color)
{
    char buf[6];
    uint8_t i;
    int len = snprintf(buf, sizeof(buf), "%u", (unsigned int)value);

    if (len <= 0)
    {
        return x;
    }
    for (i = 0U; (i < (uint8_t)len) && (i < 5U); i++)
    {
        if ((buf[i] >= '0') && (buf[i] <= '9'))
        {
            LCD_ShowChinese((u16)(x + (32U * i)), y, (u8)(20U + (buf[i] - '0')), color);
        }
    }
    return (u16)(x + (32U * (uint8_t)len));
}

static void lcd_ui_show_mono_image(u16 x, u16 y, u16 w, u16 h,
                                   const u8 *img, u16 color)
{
    u16 row;
    u16 col;
    const u8 *data = img + 6U;
    u16 bytes_per_row = (u16)((w + 7U) / 8U);

    for (row = 0U; row < h; row++)
    {
        for (col = 0U; col < w; col++)
        {
            u8 byte_val = data[(row * bytes_per_row) + (col >> 3U)];
            if ((byte_val & (0x80U >> (col & 7U))) != 0U)
            {
                LCD_Fast_DrawPoint((u16)(x + col), (u16)(y + row), color);
            }
        }
    }
}

static void lcd_ui_fill_round_rect(u16 x1, u16 y1, u16 x2, u16 y2,
                                   u8 r, u16 color)
{
    u16 y;

    for (y = y1; y <= y2; y++)
    {
        u8 off = 0U;
        if (y < (u16)(y1 + r))
        {
            off = (u8)(y1 + r - y);
        }
        else if (y > (u16)(y2 - r))
        {
            off = (u8)(y - (y2 - r));
        }

        if (off > 0U)
        {
            u8 inset;
            for (inset = 0U; inset <= r; inset++)
            {
                if ((((r - inset) * (r - inset)) + (off * off)) <= (r * r))
                {
                    break;
                }
            }
            off = inset;
        }
        LCD_Fill((u16)(x1 + off), y, (u16)(x2 - off), y, color);
    }
}

static void lcd_ui_draw_status_bar(uint8_t hot, uint8_t cold, uint8_t fault)
{
    if (fault != 0U)
    {
        LCD_Fill(0, 400, 799, 479, RED);
        LCD_ShowChinese(336, 420, 9, WHITE);
        LCD_ShowChinese(368, 420, 10, WHITE);
        return;
    }

    if (hot != 0U)
    {
        LCD_Fill(0, 400, 799, 479, YELLOW);
        LCD_ShowChinese(336, 420, 50, BLACK);
        LCD_ShowChinese(368, 420, 51, BLACK);
        LCD_ShowChinese(400, 420, 8, BLACK);
        LCD_ShowChinese(432, 420, 1, BLACK);
    }
    else if (cold != 0U)
    {
        LCD_Fill(0, 400, 799, 479, GREEN);
        LCD_ShowChinese(336, 420, 50, WHITE);
        LCD_ShowChinese(368, 420, 51, WHITE);
        LCD_ShowChinese(400, 420, 8, WHITE);
        LCD_ShowChinese(432, 420, 1, WHITE);
    }
    else
    {
        LCD_Color_Fill(0, 400, 799, 479, lcd_ui_img(&gImage_background[640000]));
    }
}

static void lcd_ui_draw_home(void)
{
    LCD_Clear(WHITE);
    LCD_Color_Fill(0, 0, 799, 479, lcd_ui_img(gImage_background));
    LCD_Color_Fill(0, 0, 63, 63, lcd_ui_img(gImage_yinshuiji_logo));
    LCD_Color_Fill(102, 0, 697, 133, lcd_ui_img(gImage_9th_logo2));

    LCD_Color_Fill(42, 148, 77, 183, lcd_ui_img(gImage_wendu_logo));
    LCD_ShowChinese(90, 150, 16, BLACK);
    LCD_ShowChinese(122, 150, 3, BLACK);
    LCD_ShowChinese(154, 150, 49, BLACK);

    LCD_Color_Fill(42, 208, 77, 243, lcd_ui_img(gImage_shuiwei_logo));
    LCD_ShowChinese(90, 210, 1, BLACK);
    LCD_ShowChinese(122, 210, 11, BLACK);
    LCD_ShowChinese(154, 210, 49, BLACK);

    LCD_Color_Fill(42, 268, 77, 303, lcd_ui_img(gImage_id_logo));
    LCD_ShowChinese(90, 270, 31, BLACK);
    LCD_ShowChinese(122, 270, 32, BLACK);
    LCD_ShowChinese(154, 270, 49, BLACK);

    LCD_Color_Fill(42, 322, 77, 357, lcd_ui_img(gImage_geren_logo));
    lcd_ui_fill_round_rect(90, 310, 250, 360, 10, 0x7BEFU);
    LCD_ShowChinese(98, 320, 33, WHITE);
    LCD_ShowChinese(130, 320, 34, WHITE);
    LCD_ShowChinese(162, 320, 35, WHITE);
    LCD_ShowChinese(194, 320, 36, WHITE);

    lcd_ui_fill_round_rect(340, 180, 459, 299, 15, LCD_UI_ORANGE);
    lcd_ui_show_mono_image(384, 198, 32, 32, gImage_hot_water_logo, WHITE);
    LCD_ShowChinese(368, 250, 0, WHITE);
    LCD_ShowChinese(400, 250, 1, WHITE);

    lcd_ui_fill_round_rect(480, 180, 599, 299, 15, BLUE);
    lcd_ui_show_mono_image(523, 198, 32, 32, gImage_cold_water_logo, WHITE);
    LCD_ShowChinese(508, 250, 2, WHITE);
    LCD_ShowChinese(540, 250, 1, WHITE);

    lcd_ui_draw_status_bar(0U, 0U, 0U);
}

static void lcd_ui_draw_info(void)
{
    LCD_Clear(WHITE);
    LCD_Color_Fill(0, 0, 799, 479, lcd_ui_img(gImage_background));
    LCD_Color_Fill(0, 0, 63, 63, lcd_ui_img(gImage_yinshuiji_logo));
    LCD_Color_Fill(102, 0, 697, 133, lcd_ui_img(gImage_9th_logo2));

    LCD_Color_Fill(102, 148, 137, 183, lcd_ui_img(gImage_id_logo));
    LCD_ShowChinese(150, 150, 31, BLACK);
    LCD_ShowChinese(182, 150, 32, BLACK);
    LCD_ShowChinese(214, 150, 49, BLACK);

    LCD_Color_Fill(102, 193, 137, 228, lcd_ui_img(gImage_geren_logo));
    LCD_ShowChinese(150, 195, 53, BLACK);
    LCD_ShowChinese(182, 195, 54, BLACK);
    LCD_ShowChinese(214, 195, 49, BLACK);

    LCD_Color_Fill(102, 238, 137, 273, lcd_ui_img(gImage_yinshui_logo));
    LCD_ShowChinese(150, 240, 6, BLACK);
    LCD_ShowChinese(182, 240, 7, BLACK);
    LCD_ShowChinese(214, 240, 39, BLACK);
    LCD_ShowChinese(246, 240, 1, BLACK);
    LCD_ShowChinese(278, 240, 49, BLACK);

    LCD_Color_Fill(102, 283, 137, 318, lcd_ui_img(gImage_water_logo));
    LCD_ShowChinese(150, 285, 4, BLACK);
    LCD_ShowChinese(182, 285, 5, BLACK);
    LCD_ShowChinese(214, 285, 39, BLACK);
    LCD_ShowChinese(246, 285, 1, BLACK);
    LCD_ShowChinese(278, 285, 49, BLACK);

    LCD_Color_Fill(102, 328, 137, 363, lcd_ui_img(gImage_jianyi_logo));
    LCD_ShowChinese(150, 330, 4, BLACK);
    LCD_ShowChinese(182, 330, 5, BLACK);
    LCD_ShowChinese(214, 330, 49, BLACK);

    LCD_Color_Fill(102, 373, 137, 408, lcd_ui_img(gImage_yue_logo));
    LCD_ShowChinese(150, 375, 37, BLACK);
    LCD_ShowChinese(182, 375, 38, BLACK);
    LCD_ShowChinese(214, 375, 49, BLACK);

    lcd_ui_fill_round_rect(690, 430, 779, 469, 8, LCD_UI_CYAN_THEME);
    LCD_ShowChinese(700, 435, 12, BLACK);
    LCD_ShowChinese(732, 435, 13, BLACK);
}

static void lcd_ui_switch_page(uint8_t page)
{
    s_current_page = page;
    if (page == LCD_UI_PAGE_HOME)
    {
        lcd_ui_draw_home();
        s_last_water_level = 0xFFU;
        s_last_water_out_state = 0xFFU;
        s_last_hot_cold_mode = 0xFFU;
        s_last_temp_val = 0xFFFFU;
        s_last_dev_id = 0xFFFFU;
    }
    else
    {
        lcd_ui_draw_info();
    }
}

static void lcd_ui_draw_home_status(uint8_t water_level,
                                    uint8_t water_out_state,
                                    uint16_t temp_val,
                                    uint8_t hot_cold_mode,
                                    uint16_t dev_id)
{
    uint8_t hot_active = ((water_out_state == 1U) && (hot_cold_mode == 1U)) ? 1U : 0U;
    uint8_t cold_active = ((water_out_state == 1U) && (hot_cold_mode != 1U)) ? 1U : 0U;
    uint8_t fault_active = (water_out_state == 2U) ? 1U : 0U;

    if (temp_val != s_last_temp_val)
    {
        u16 next_x;

        lcd_ui_restore_background(210, 150, 370, 183);
        next_x = lcd_ui_show_u16_32(210, 150, temp_val, BLACK);
        LCD_ShowChinese(next_x, 150, 17, BLACK);
    }

    if (water_level != s_last_water_level)
    {
        lcd_ui_restore_background(210, 210, 310, 243);
        if (water_level != 0U)
        {
            LCD_ShowChinese(210, 210, 14, GREEN);
            LCD_ShowChinese(242, 210, 15, GREEN);
        }
        else
        {
            LCD_ShowChinese(210, 210, 9, RED);
            LCD_ShowChinese(242, 210, 10, RED);
        }
    }

    if (dev_id != s_last_dev_id)
    {
        lcd_ui_restore_background(210, 270, 330, 303);
        lcd_ui_show_u16_32(210, 270, dev_id, BLACK);
    }

    if ((water_out_state != s_last_water_out_state) ||
        (hot_cold_mode != s_last_hot_cold_mode))
    {
        lcd_ui_draw_status_bar(hot_active, cold_active, fault_active);
    }
}

void LCD_UI_Init(void)
{
    if (s_ui_ready != 0U)
    {
        return;
    }

    delay_init();
    LCD_Init();
    s_touch_ready = (TP_Init() == 0U) ? 1U : 0U;
    lcd_ui_log_init_result();
    lcd_ui_draw_home();
    s_ui_ready = 1U;
}

void LCD_UI_Poll(void)
{
    uint32_t now_ms;
    uint8_t scan_result;
    uint8_t t;

    if ((s_ui_ready == 0U) || (s_touch_ready == 0U))
    {
        return;
    }

    now_ms = HAL_GetTick();
    if ((now_ms - s_last_touch_scan_ms) < LCD_UI_TOUCH_SCAN_MS)
    {
        return;
    }
    s_last_touch_scan_ms = now_ms;

    scan_result = tp_dev.scan(0);
    if (scan_result == 0U)
    {
        s_touch_processed = 0U;
        return;
    }

    if (s_touch_processed != 0U)
    {
        return;
    }

    for (t = 0U; t < 5U; t++)
    {
        if ((tp_dev.sta & (1U << t)) != 0U)
        {
            u16 tx = tp_dev.x[t];
            u16 ty = tp_dev.y[t];

            if (s_current_page == LCD_UI_PAGE_HOME)
            {
                if ((tx >= 330U) && (tx <= 469U) && (ty >= 170U) && (ty <= 309U))
                {
                    lcd_ui_log("[LCD] touch hot request\r\n");
                    CameraApp_LcdHotPressed();
                    s_touch_processed = 1U;
                    break;
                }
                if ((tx >= 470U) && (tx <= 609U) && (ty >= 170U) && (ty <= 309U))
                {
                    lcd_ui_log("[LCD] touch cold request\r\n");
                    CameraApp_LcdColdPressed();
                    s_touch_processed = 1U;
                    break;
                }
                if ((tx >= 90U) && (tx <= 250U) && (ty >= 310U) && (ty <= 360U))
                {
                    lcd_ui_log("[LCD] touch info page\r\n");
                    lcd_ui_switch_page(LCD_UI_PAGE_INFO);
                    s_touch_processed = 1U;
                    break;
                }
            }
            else if ((tx >= 700U) && (tx <= 779U) && (ty >= 430U) && (ty <= 469U))
            {
                lcd_ui_log("[LCD] touch home page\r\n");
                lcd_ui_switch_page(LCD_UI_PAGE_HOME);
                s_touch_processed = 1U;
                break;
            }
        }
    }
}

void LCD_UI_ShowStatus(uint8_t water_level,
                       uint8_t water_out_state,
                       uint16_t temp_val,
                       uint8_t hot_cold_mode,
                       uint16_t dev_id)
{
    uint8_t changed;
    uint32_t now_ms;

    if (s_ui_ready == 0U)
    {
        return;
    }

    changed = ((water_level != s_last_water_level) ||
               (water_out_state != s_last_water_out_state) ||
               (temp_val != s_last_temp_val) ||
               (hot_cold_mode != s_last_hot_cold_mode) ||
               (dev_id != s_last_dev_id)) ? 1U : 0U;
    now_ms = HAL_GetTick();
    if ((changed == 0U) && ((now_ms - s_last_status_ms) < LCD_UI_REFRESH_MS))
    {
        return;
    }

    if (s_current_page == LCD_UI_PAGE_HOME)
    {
        lcd_ui_draw_home_status(water_level, water_out_state, temp_val,
                                hot_cold_mode, dev_id);
    }

    s_last_water_level = water_level;
    s_last_water_out_state = water_out_state;
    s_last_temp_val = temp_val;
    s_last_hot_cold_mode = hot_cold_mode;
    s_last_dev_id = dev_id;
    s_last_status_ms = now_ms;
}
