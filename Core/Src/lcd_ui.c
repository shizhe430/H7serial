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
#define LCD_UI_FACE_CONFIRM_DEFAULT_MS 2500U
#define LCD_UI_FACE_FRAME_WIDTH  320U
#define LCD_UI_FACE_FRAME_HEIGHT 240U
#define LCD_UI_FACE_SCALE         2U
#define LCD_UI_FACE_DISPLAY_WIDTH 640U

#define LCD_UI_PAGE_HOME       0U
#define LCD_UI_PAGE_INFO       1U
#define LCD_UI_PAGE_FACE       2U
#define LCD_UI_PAGE_ID_MANAGER 3U
#define LCD_UI_PAGE_FACE_SCAN  4U

static uint8_t s_ui_ready;
static uint8_t s_touch_ready;
static uint8_t s_current_page = LCD_UI_PAGE_HOME;
static uint8_t s_touch_processed;
static uint32_t s_last_touch_scan_ms;
static uint32_t s_face_confirm_until_ms;
static u16 s_face_display_rows[LCD_UI_FACE_DISPLAY_WIDTH * LCD_UI_FACE_SCALE];
static volatile uint8_t s_id_event = LCD_UI_ID_EVENT_NONE;
static uint16_t s_id_selected = 1U;

static uint8_t s_last_water_level = 0xFFU;
static uint8_t s_last_water_out_state = 0xFFU;
static uint8_t s_last_hot_cold_mode = 0xFFU;
static int16_t s_last_temp_tenths = INT16_MIN;
static uint16_t s_last_dev_id = 0xFFFFU;
static uint32_t s_last_status_ms;
static char s_cloud_name[64];
static uint32_t s_cloud_today_ml;
static uint32_t s_cloud_use_count;
static uint32_t s_cloud_remaining_ml;
static uint32_t s_cloud_interval_min;
static uint32_t s_cloud_suggested_ml;
static uint8_t s_cloud_user_valid;
static uint8_t s_cloud_advice_valid;

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

static void lcd_ui_log_gram_test(void)
{
    static const u16 patterns[] = {0x0000U, 0xFFFFU, 0x0080U, 0xFF7FU};
    u16 actual[sizeof(patterns) / sizeof(patterns[0])];
    char msg[128];
    uint32_t i;
    int len;

    for (i = 0U; i < (sizeof(patterns) / sizeof(patterns[0])); i++)
    {
        LCD_Fast_DrawPoint((u16)i, 0U, patterns[i]);
    }
    for (i = 0U; i < (sizeof(patterns) / sizeof(patterns[0])); i++)
    {
        actual[i] = (u16)LCD_ReadPoint((u16)i, 0U);
    }

    len = snprintf(msg, sizeof(msg),
                   "[LCD] gram exp=%04X,%04X,%04X,%04X got=%04X,%04X,%04X,%04X\r\n",
                   (unsigned int)patterns[0], (unsigned int)patterns[1],
                   (unsigned int)patterns[2], (unsigned int)patterns[3],
                   (unsigned int)actual[0], (unsigned int)actual[1],
                   (unsigned int)actual[2], (unsigned int)actual[3]);
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

static u16 lcd_ui_show_u32_32(u16 x, u16 y, uint32_t value, u16 color)
{
    char buf[11];
    uint8_t i;
    int len = snprintf(buf, sizeof(buf), "%lu", (unsigned long)value);

    if (len <= 0)
    {
        return x;
    }
    for (i = 0U; (i < (uint8_t)len) && (i < 10U); i++)
    {
        if ((buf[i] >= '0') && (buf[i] <= '9'))
        {
            LCD_ShowChinese((u16)(x + (32U * i)), y, (u8)(20U + (buf[i] - '0')), color);
        }
    }
    return (u16)(x + (32U * (uint8_t)len));
}

static uint8_t lcd_ui_ascii_text_valid(const char *text)
{
    if ((text == NULL) || (*text == '\0'))
    {
        return 0U;
    }
    while (*text != '\0')
    {
        if (((uint8_t)*text < 0x20U) || ((uint8_t)*text > 0x7EU))
        {
            return 0U;
        }
        text++;
    }
    return 1U;
}

static void lcd_ui_show_ascii_transparent(u16 x, u16 y, const char *text, u16 color)
{
    u32 previous_color = POINT_COLOR;

    POINT_COLOR = color;
    while ((text != NULL) && (*text != '\0') && (x <= 784U))
    {
        uint8_t value = (uint8_t)*text;
        if ((value >= 0x20U) && (value <= 0x7EU))
        {
            LCD_ShowChar(x, y, value, 16U, 1U);
            x = (u16)(x + 8U);
        }
        text++;
    }
    POINT_COLOR = previous_color;
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

    lcd_ui_fill_round_rect(340, 184, 459, 303, 15, LCD_UI_ORANGE);
    lcd_ui_show_mono_image(384, 202, 32, 32, gImage_hot_water_logo, WHITE);
    LCD_ShowChinese(368, 254, 0, WHITE);
    LCD_ShowChinese(400, 254, 1, WHITE);

    lcd_ui_fill_round_rect(480, 184, 599, 303, 15, BLUE);
    lcd_ui_show_mono_image(523, 202, 32, 32, gImage_cold_water_logo, WHITE);
    LCD_ShowChinese(508, 254, 2, WHITE);
    LCD_ShowChinese(540, 254, 1, WHITE);

    lcd_ui_fill_round_rect(640, 184, 759, 303, 15, GREEN);
    LCD_Color_Fill(684, 202, 719, 237, lcd_ui_img(gImage_id_logo));
    lcd_ui_show_ascii_transparent(692, 254, "ID", WHITE);
    lcd_ui_show_ascii_transparent(672, 278, "MANAGE", WHITE);

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

static void lcd_ui_draw_info_cloud(void)
{
    char fallback[24];
    char count_text[20];
    u16 next_x;

    lcd_ui_restore_background(320, 150, 680, 182);
    lcd_ui_restore_background(320, 195, 780, 227);
    lcd_ui_restore_background(320, 240, 780, 272);
    lcd_ui_restore_background(320, 285, 780, 317);
    lcd_ui_restore_background(320, 330, 780, 362);
    lcd_ui_restore_background(320, 375, 780, 407);

    if (s_last_dev_id != 0xFFFFU)
    {
        (void)lcd_ui_show_u32_32(330, 150, s_last_dev_id, GREEN);
    }
    if (s_cloud_user_valid == 0U)
    {
        return;
    }

    if (lcd_ui_ascii_text_valid(s_cloud_name) != 0U)
    {
        lcd_ui_show_ascii_transparent(330, 203, s_cloud_name, BLACK);
    }
    else
    {
        (void)snprintf(fallback, sizeof(fallback), "ID %u", (unsigned int)s_last_dev_id);
        lcd_ui_show_ascii_transparent(330, 203, fallback, BLACK);
    }

    next_x = lcd_ui_show_u32_32(330, 240, s_cloud_today_ml, GREEN);
    lcd_ui_show_ascii_transparent(next_x, 248, "ml", BLACK);
    (void)snprintf(count_text, sizeof(count_text), "  x%lu", (unsigned long)s_cloud_use_count);
    lcd_ui_show_ascii_transparent(610, 248, count_text, BLACK);

    if (s_cloud_advice_valid != 0U)
    {
        next_x = lcd_ui_show_u32_32(330, 285, s_cloud_suggested_ml, GREEN);
        lcd_ui_show_ascii_transparent(next_x, 293, "ml", BLACK);
        next_x = lcd_ui_show_u32_32(330, 330, s_cloud_interval_min, GREEN);
        lcd_ui_show_ascii_transparent(next_x, 338, "min", BLACK);
    }

    next_x = lcd_ui_show_u32_32(330, 375, s_cloud_remaining_ml, GREEN);
    lcd_ui_show_ascii_transparent(next_x, 383, "ml", BLACK);
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
        s_last_temp_tenths = INT16_MIN;
        s_last_dev_id = 0xFFFFU;
    }
    else
    {
        lcd_ui_draw_info();
        lcd_ui_draw_info_cloud();
    }
}

static void lcd_ui_close_face_confirmation(void)
{
    if (s_current_page == LCD_UI_PAGE_FACE)
    {
        s_face_confirm_until_ms = 0U;
        lcd_ui_switch_page(LCD_UI_PAGE_HOME);
        lcd_ui_log("[LCD] face confirm closed\r\n");
    }
}

static u16 lcd_ui_rgb888_to_rgb565(const uint8_t *pixel)
{
    return (u16)((((u16)pixel[0] & 0xF8U) << 8U) |
                 (((u16)pixel[1] & 0xFCU) << 3U) |
                 (((u16)pixel[2]) >> 3U));
}

static uint8_t lcd_ui_u16_digit_count(uint16_t value)
{
    uint8_t digits = 1U;

    while (value >= 10U)
    {
        value = (uint16_t)(value / 10U);
        digits++;
    }
    return digits;
}

static void lcd_ui_draw_face_box(int16_t x1, int16_t y1, int16_t x2, int16_t y2, u16 color)
{
    int32_t left = x1;
    int32_t top = y1;
    int32_t right = x2;
    int32_t bottom = y2;
    uint8_t thickness;
    u32 previous_color;

    if ((right <= left) || (bottom <= top))
    {
        return;
    }
    if (left < 0) { left = 0; }
    if (top < 0) { top = 0; }
    if (right > (int32_t)LCD_UI_FACE_FRAME_WIDTH) { right = LCD_UI_FACE_FRAME_WIDTH; }
    if (bottom > (int32_t)LCD_UI_FACE_FRAME_HEIGHT) { bottom = LCD_UI_FACE_FRAME_HEIGHT; }

    left *= LCD_UI_FACE_SCALE;
    top *= LCD_UI_FACE_SCALE;
    right = (right * LCD_UI_FACE_SCALE) - 1;
    bottom = (bottom * LCD_UI_FACE_SCALE) - 1;
    previous_color = POINT_COLOR;
    POINT_COLOR = color;
    for (thickness = 0U; thickness < 3U; thickness++)
    {
        if (((left + thickness) < (right - thickness)) &&
            ((top + thickness) < (bottom - thickness)))
        {
            LCD_DrawRectangle((u16)(left + thickness),
                              (u16)(top + thickness),
                              (u16)(right - thickness),
                              (u16)(bottom - thickness));
        }
    }
    POINT_COLOR = previous_color;
}

static void lcd_ui_draw_face_frame(const uint8_t *rgb888)
{
    uint16_t source_y;

    for (source_y = 0U; source_y < LCD_UI_FACE_FRAME_HEIGHT; source_y++)
    {
        uint16_t source_x;
        u16 *first_row = s_face_display_rows;
        u16 *second_row = &s_face_display_rows[LCD_UI_FACE_DISPLAY_WIDTH];

        for (source_x = 0U; source_x < LCD_UI_FACE_FRAME_WIDTH; source_x++)
        {
            const uint8_t *pixel = &rgb888[(((uint32_t)source_y * LCD_UI_FACE_FRAME_WIDTH) + source_x) * 3U];
            u16 color = lcd_ui_rgb888_to_rgb565(pixel);
            first_row[source_x * 2U] = color;
            first_row[(source_x * 2U) + 1U] = color;
        }
        memcpy(second_row, first_row, LCD_UI_FACE_DISPLAY_WIDTH * sizeof(first_row[0]));
        LCD_Color_Fill(0U,
                       (u16)(source_y * 2U),
                       LCD_UI_FACE_DISPLAY_WIDTH - 1U,
                       (u16)((source_y * 2U) + 1U),
                       s_face_display_rows);
    }
}

static void lcd_ui_draw_face_confirmation(const uint8_t *rgb888,
                                          int16_t face_x1,
                                          int16_t face_y1,
                                          int16_t face_x2,
                                          int16_t face_y2,
                                          uint16_t user_id)
{
    uint8_t digits;
    u16 id_x;

    lcd_ui_draw_face_frame(rgb888);

    LCD_Fill(LCD_UI_FACE_DISPLAY_WIDTH, 0U, 799U, 479U, BLACK);
    LCD_Fill(LCD_UI_FACE_DISPLAY_WIDTH, 0U, LCD_UI_FACE_DISPLAY_WIDTH + 3U, 479U, GREEN);
    lcd_ui_show_ascii_transparent(698U, 72U, "FACE", WHITE);
    lcd_ui_show_ascii_transparent(690U, 112U, "MATCH", GREEN);
    lcd_ui_show_ascii_transparent(712U, 184U, "ID", WHITE);
    digits = lcd_ui_u16_digit_count(user_id);
    id_x = (u16)(LCD_UI_FACE_DISPLAY_WIDTH + ((160U - ((u16)digits * 32U)) / 2U));
    (void)lcd_ui_show_u32_32(id_x, 224U, user_id, GREEN);
    lcd_ui_draw_face_box(face_x1, face_y1, face_x2, face_y2, GREEN);
}

static void lcd_ui_draw_face_scan(const uint8_t *rgb888,
                                  int16_t face_x1,
                                  int16_t face_y1,
                                  int16_t face_x2,
                                  int16_t face_y2,
                                  uint8_t detection_valid,
                                  uint8_t match_valid,
                                  uint16_t matched_id,
                                  uint8_t attempt,
                                  uint8_t max_attempts)
{
    char status[20];
    char progress[12];
    u16 box_color = (match_valid != 0U) ? GREEN : YELLOW;

    lcd_ui_draw_face_frame(rgb888);
    LCD_Fill(LCD_UI_FACE_DISPLAY_WIDTH, 0U, 799U, 479U, BLACK);
    LCD_Fill(LCD_UI_FACE_DISPLAY_WIDTH, 0U, LCD_UI_FACE_DISPLAY_WIDTH + 3U,
             479U, box_color);
    lcd_ui_show_ascii_transparent(674U, 40U, "FACE SCAN", WHITE);
    (void)snprintf(progress, sizeof(progress), "%u / %u",
                   (unsigned int)attempt, (unsigned int)max_attempts);
    lcd_ui_show_ascii_transparent(694U, 88U, progress, WHITE);

    if ((match_valid != 0U) && (matched_id != 0U))
    {
        (void)snprintf(status, sizeof(status), "MATCH ID %u",
                       (unsigned int)matched_id);
    }
    else if (detection_valid != 0U)
    {
        (void)snprintf(status, sizeof(status), "NO MATCH");
    }
    else
    {
        (void)snprintf(status, sizeof(status), "NO FACE");
    }
    lcd_ui_show_ascii_transparent(672U, 142U, status,
                                  (match_valid != 0U) ? GREEN : WHITE);
    lcd_ui_show_ascii_transparent(666U, 206U, "LOOK CAMERA", WHITE);
    if (detection_valid != 0U)
    {
        lcd_ui_draw_face_box(face_x1, face_y1, face_x2, face_y2, box_color);
    }
}

static void lcd_ui_draw_id_button(u16 y1, u16 y2, u16 color, const char *label)
{
    size_t length = (label != NULL) ? strlen(label) : 0U;
    u16 x = (u16)(720U - ((length * 8U) / 2U));

    lcd_ui_fill_round_rect(650U, y1, 790U, y2, 8U, color);
    lcd_ui_show_ascii_transparent(x, (u16)(y1 + ((y2 - y1 - 16U) / 2U)), label, WHITE);
}

static u16 lcd_ui_id_notice_color(const char *text)
{
    if (text == NULL)
    {
        return WHITE;
    }
    if ((strstr(text, "SAVED") != NULL) || (strstr(text, "DELETE OK") != NULL) ||
        (strncmp(text, "MATCH ID", 8U) == 0))
    {
        return GREEN;
    }
    if ((strstr(text, "ERROR") != NULL) || (strstr(text, "DB FULL") != NULL) ||
        (strstr(text, "NOT FOUND") != NULL))
    {
        return RED;
    }
    if ((strstr(text, "ENROLL") != NULL) || (strstr(text, "REQUEST") != NULL) ||
        (strstr(text, "/5") != NULL))
    {
        return YELLOW;
    }
    return WHITE;
}

static void lcd_ui_draw_id_notice(const char *text)
{
    LCD_Fill(648U, 338U, 798U, 408U, BLACK);
    if ((text != NULL) && (*text != '\0'))
    {
        lcd_ui_show_ascii_transparent(650U, 350U, text, lcd_ui_id_notice_color(text));
    }
}

static void lcd_ui_draw_id_selection(void)
{
    LCD_Fill(650U, 82U, 790U, 118U, BLACK);
    lcd_ui_show_ascii_transparent(650U, 88U, "SELECT ID", WHITE);
    (void)lcd_ui_show_u32_32(758U, 82U, s_id_selected, GREEN);
}

static void lcd_ui_draw_id_manager(void)
{
    LCD_Clear(BLACK);
    LCD_Fill(640U, 0U, 643U, 479U, GREEN);
    lcd_ui_show_ascii_transparent(666U, 12U, "ID MANAGER", WHITE);
    lcd_ui_show_ascii_transparent(650U, 48U, "USERS", WHITE);
    lcd_ui_draw_id_selection();
    lcd_ui_fill_round_rect(650U, 128U, 714U, 178U, 8U, 0x7BEFU);
    lcd_ui_fill_round_rect(726U, 128U, 790U, 178U, 8U, 0x7BEFU);
    lcd_ui_show_ascii_transparent(678U, 145U, "-", WHITE);
    lcd_ui_show_ascii_transparent(754U, 145U, "+", WHITE);
    lcd_ui_draw_id_button(194U, 252U, GREEN, "ENROLL");
    lcd_ui_draw_id_button(268U, 326U, RED, "DELETE");
    lcd_ui_draw_id_button(420U, 470U, LCD_UI_CYAN_THEME, "BACK");
    lcd_ui_show_ascii_transparent(650U, 346U, "WAITING", WHITE);
}

static void lcd_ui_open_id_manager(void)
{
    s_current_page = LCD_UI_PAGE_ID_MANAGER;
    s_id_selected = 1U;
    s_id_event = LCD_UI_ID_EVENT_ENTER;
    lcd_ui_draw_id_manager();
    lcd_ui_log("[LCD] id manager enter\r\n");
}

static u16 lcd_ui_show_temp_32(u16 x, u16 y, int16_t temp_tenths, u16 color)
{
    uint16_t magnitude;
    uint16_t next_x;

    if (temp_tenths == INT16_MIN)
    {
        LCD_ShowChar(x, (u16)(y + 8U), '-', 16U, 1U);
        LCD_ShowChar((u16)(x + 16U), (u16)(y + 8U), '-', 16U, 1U);
        return (u16)(x + 32U);
    }

    magnitude = (uint16_t)((temp_tenths < 0) ? -temp_tenths : temp_tenths);
    next_x = x;
    if (temp_tenths < 0)
    {
        LCD_ShowChar(next_x, (u16)(y + 8U), '-', 16U, 1U);
        next_x = (u16)(next_x + 16U);
    }
    next_x = lcd_ui_show_u32_32(next_x, y, magnitude / 10U, color);
    LCD_ShowChar(next_x, (u16)(y + 8U), '.', 16U, 1U);
    next_x = (u16)(next_x + 16U);
    LCD_ShowChinese(next_x, y, (u8)(20U + (magnitude % 10U)), color);
    next_x = (u16)(next_x + 32U);
    LCD_ShowChinese(next_x, y, 17U, color);
    return (u16)(next_x + 32U);
}

static void lcd_ui_draw_home_status(uint8_t water_level,
                                    uint8_t water_out_state,
                                    int16_t temp_tenths,
                                    uint8_t hot_cold_mode,
                                    uint16_t dev_id)
{
    uint8_t hot_active = ((water_out_state == 1U) && (hot_cold_mode == 1U)) ? 1U : 0U;
    uint8_t cold_active = ((water_out_state == 1U) && (hot_cold_mode != 1U)) ? 1U : 0U;
    uint8_t fault_active = (water_out_state == 2U) ? 1U : 0U;

    if (temp_tenths != s_last_temp_tenths)
    {
        lcd_ui_restore_background(210, 150, 370, 183);
        (void)lcd_ui_show_temp_32(210, 150, temp_tenths, GREEN);
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
            LCD_ShowChinese(210, 210, 57, RED);
            LCD_ShowChinese(242, 210, 1, RED);
        }
    }

    if (dev_id != s_last_dev_id)
    {
        lcd_ui_restore_background(210, 270, 330, 303);
        lcd_ui_show_u32_32(210, 270, dev_id, GREEN);
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
    lcd_ui_log_gram_test();
    lcd_ui_draw_home();
    s_ui_ready = 1U;
}

void LCD_UI_Poll(void)
{
    uint32_t now_ms;
    uint8_t scan_result;
    uint8_t t;

    if (s_ui_ready == 0U)
    {
        return;
    }

    now_ms = HAL_GetTick();
    if ((s_current_page == LCD_UI_PAGE_FACE) &&
        ((int32_t)(now_ms - s_face_confirm_until_ms) >= 0))
    {
        lcd_ui_close_face_confirmation();
    }
    if (s_touch_ready == 0U)
    {
        return;
    }
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

            if (s_current_page == LCD_UI_PAGE_FACE)
            {
                (void)tx;
                (void)ty;
                lcd_ui_close_face_confirmation();
                s_touch_processed = 1U;
                break;
            }
            else if (s_current_page == LCD_UI_PAGE_HOME)
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
                if ((tx >= 630U) && (tx <= 770U) && (ty >= 170U) && (ty <= 309U))
                {
                    lcd_ui_open_id_manager();
                    s_touch_processed = 1U;
                    break;
                }
            }
            else if (s_current_page == LCD_UI_PAGE_ID_MANAGER)
            {
                if ((tx >= 640U) && (tx <= 720U) && (ty >= 118U) && (ty <= 188U))
                {
                    s_id_selected = (s_id_selected <= 1U) ? 5U : (uint16_t)(s_id_selected - 1U);
                    lcd_ui_draw_id_selection();
                    s_touch_processed = 1U;
                    break;
                }
                if ((tx >= 720U) && (tx <= 799U) && (ty >= 118U) && (ty <= 188U))
                {
                    s_id_selected = (s_id_selected >= 5U) ? 1U : (uint16_t)(s_id_selected + 1U);
                    lcd_ui_draw_id_selection();
                    s_touch_processed = 1U;
                    break;
                }
                if ((tx >= 640U) && (tx <= 799U) && (ty >= 188U) && (ty <= 260U))
                {
                    if (s_id_event == LCD_UI_ID_EVENT_NONE)
                    {
                        s_id_event = LCD_UI_ID_EVENT_ENROLL;
                        lcd_ui_draw_id_notice("ENROLL REQUEST");
                    }
                    s_touch_processed = 1U;
                    break;
                }
                if ((tx >= 640U) && (tx <= 799U) && (ty >= 260U) && (ty <= 334U))
                {
                    if (s_id_event == LCD_UI_ID_EVENT_NONE)
                    {
                        s_id_event = LCD_UI_ID_EVENT_DELETE;
                        lcd_ui_draw_id_notice("DELETE REQUEST");
                    }
                    s_touch_processed = 1U;
                    break;
                }
                if ((tx >= 640U) && (tx <= 799U) && (ty >= 410U) && (ty <= 479U))
                {
                    if (s_id_event == LCD_UI_ID_EVENT_NONE)
                    {
                        s_id_event = LCD_UI_ID_EVENT_EXIT;
                    }
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
                       int16_t temp_tenths,
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
               (temp_tenths != s_last_temp_tenths) ||
               (hot_cold_mode != s_last_hot_cold_mode) ||
               (dev_id != s_last_dev_id)) ? 1U : 0U;
    now_ms = HAL_GetTick();
    if ((changed == 0U) && ((now_ms - s_last_status_ms) < LCD_UI_REFRESH_MS))
    {
        return;
    }

    if (s_current_page == LCD_UI_PAGE_HOME)
    {
        lcd_ui_draw_home_status(water_level, water_out_state, temp_tenths,
                                hot_cold_mode, dev_id);
    }

    s_last_water_level = water_level;
    s_last_water_out_state = water_out_state;
    s_last_temp_tenths = temp_tenths;
    s_last_hot_cold_mode = hot_cold_mode;
    s_last_dev_id = dev_id;
    s_last_status_ms = now_ms;
}

void LCD_UI_ShowCloud(const char *username_utf8,
                      uint32_t today_ml,
                      uint32_t use_count,
                      uint32_t remaining_ml,
                      uint32_t interval_min,
                      uint32_t suggested_ml,
                      uint8_t advice_received)
{
    size_t name_length = (username_utf8 != NULL) ? strlen(username_utf8) : 0U;

    if (name_length >= sizeof(s_cloud_name))
    {
        name_length = sizeof(s_cloud_name) - 1U;
    }
    if (name_length != 0U)
    {
        memcpy(s_cloud_name, username_utf8, name_length);
    }
    s_cloud_name[name_length] = '\0';
    s_cloud_today_ml = today_ml;
    s_cloud_use_count = use_count;
    s_cloud_remaining_ml = remaining_ml;
    s_cloud_interval_min = interval_min;
    s_cloud_suggested_ml = suggested_ml;
    s_cloud_user_valid = (name_length != 0U) ? 1U : 0U;
    s_cloud_advice_valid = (advice_received != 0U) ? 1U : 0U;

    if ((s_ui_ready != 0U) && (s_current_page == LCD_UI_PAGE_INFO))
    {
        lcd_ui_draw_info_cloud();
    }
}

void LCD_UI_ShowFaceConfirmation(const uint8_t *rgb888,
                                 uint16_t frame_width,
                                 uint16_t frame_height,
                                 int16_t face_x1,
                                 int16_t face_y1,
                                 int16_t face_x2,
                                 int16_t face_y2,
                                 uint16_t user_id,
                                 uint32_t hold_ms)
{
    char msg[80];
    int len;

    if ((s_ui_ready == 0U) || (rgb888 == NULL) || (user_id == 0U) ||
        (frame_width != LCD_UI_FACE_FRAME_WIDTH) ||
        (frame_height != LCD_UI_FACE_FRAME_HEIGHT))
    {
        return;
    }
    if (hold_ms == 0U)
    {
        hold_ms = LCD_UI_FACE_CONFIRM_DEFAULT_MS;
    }

    lcd_ui_draw_face_confirmation(rgb888, face_x1, face_y1, face_x2, face_y2, user_id);
    s_current_page = LCD_UI_PAGE_FACE;
    s_face_confirm_until_ms = HAL_GetTick() + hold_ms;
    s_touch_processed = 0U;

    len = snprintf(msg, sizeof(msg),
                   "[LCD] face confirm id=%u hold_ms=%lu\r\n",
                   (unsigned int)user_id,
                   (unsigned long)hold_ms);
    if (len > 0)
    {
        lcd_ui_log(msg);
    }
}

void LCD_UI_ShowFaceScanFrame(const uint8_t *rgb888,
                              uint16_t frame_width,
                              uint16_t frame_height,
                              int16_t face_x1,
                              int16_t face_y1,
                              int16_t face_x2,
                              int16_t face_y2,
                              uint8_t detection_valid,
                              uint8_t match_valid,
                              uint16_t matched_id,
                              uint8_t attempt,
                              uint8_t max_attempts)
{
    if ((s_ui_ready == 0U) || (rgb888 == NULL) ||
        (frame_width != LCD_UI_FACE_FRAME_WIDTH) ||
        (frame_height != LCD_UI_FACE_FRAME_HEIGHT))
    {
        return;
    }

    s_current_page = LCD_UI_PAGE_FACE_SCAN;
    s_face_confirm_until_ms = 0U;
    lcd_ui_draw_face_scan(rgb888,
                          face_x1, face_y1, face_x2, face_y2,
                          detection_valid, match_valid, matched_id,
                          attempt, max_attempts);
}

void LCD_UI_CloseFaceScan(void)
{
    if (s_current_page == LCD_UI_PAGE_FACE_SCAN)
    {
        lcd_ui_switch_page(LCD_UI_PAGE_HOME);
        lcd_ui_log("[LCD] face scan closed\r\n");
    }
}

uint8_t LCD_UI_IdManagerTakeEvent(lcd_ui_id_event_t *event, uint16_t *selected_id)
{
    uint8_t pending = s_id_event;

    if (event != NULL)
    {
        *event = (lcd_ui_id_event_t)pending;
    }
    if (selected_id != NULL)
    {
        *selected_id = s_id_selected;
    }
    s_id_event = LCD_UI_ID_EVENT_NONE;
    return (pending != LCD_UI_ID_EVENT_NONE) ? 1U : 0U;
}

uint8_t LCD_UI_IdManagerIsActive(void)
{
    return (s_current_page == LCD_UI_PAGE_ID_MANAGER) ? 1U : 0U;
}

void LCD_UI_IdManagerClose(void)
{
    if (s_current_page == LCD_UI_PAGE_ID_MANAGER)
    {
        s_id_event = LCD_UI_ID_EVENT_NONE;
        lcd_ui_switch_page(LCD_UI_PAGE_HOME);
        lcd_ui_log("[LCD] id manager closed\r\n");
    }
}

void LCD_UI_ShowIdManagerFrame(const uint8_t *rgb888,
                               uint16_t frame_width,
                               uint16_t frame_height,
                               int16_t face_x1,
                               int16_t face_y1,
                               int16_t face_x2,
                               int16_t face_y2,
                               uint8_t detection_valid,
                               uint8_t match_valid,
                               uint16_t matched_id,
                               uint8_t occupied_mask,
                               const char *status_text)
{
    char users[20];
    char result[20];
    uint8_t id;
    size_t pos = 0U;
    u16 box_color = (match_valid != 0U) ? GREEN : YELLOW;

    if ((s_ui_ready == 0U) || (s_current_page != LCD_UI_PAGE_ID_MANAGER) ||
        (rgb888 == NULL) || (frame_width != LCD_UI_FACE_FRAME_WIDTH) ||
        (frame_height != LCD_UI_FACE_FRAME_HEIGHT))
    {
        return;
    }

    lcd_ui_draw_face_frame(rgb888);
    if (detection_valid != 0U)
    {
        lcd_ui_draw_face_box(face_x1, face_y1, face_x2, face_y2, box_color);
    }

    LCD_Fill(696U, 44U, 798U, 70U, BLACK);
    for (id = 1U; id <= 5U; id++)
    {
        if ((occupied_mask & (uint8_t)(1U << (id - 1U))) != 0U)
        {
            int written = snprintf(&users[pos], sizeof(users) - pos, "%s%u",
                                   (pos == 0U) ? "" : ",", (unsigned int)id);
            if (written > 0)
            {
                pos += (size_t)written;
            }
        }
    }
    if (pos == 0U)
    {
        (void)snprintf(users, sizeof(users), "NONE");
    }
    lcd_ui_show_ascii_transparent(700U, 48U, users, GREEN);

    if ((status_text != NULL) && (*status_text != '\0'))
    {
        (void)snprintf(result, sizeof(result), "%s", status_text);
    }
    else if ((match_valid != 0U) && (matched_id != 0U))
    {
        (void)snprintf(result, sizeof(result), "MATCH ID %u", (unsigned int)matched_id);
    }
    else if (detection_valid != 0U)
    {
        (void)snprintf(result, sizeof(result), "NO MATCH");
    }
    else
    {
        (void)snprintf(result, sizeof(result), "NO FACE");
    }
    lcd_ui_draw_id_notice(result);
}
