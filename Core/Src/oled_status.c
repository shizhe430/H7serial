#include "oled_status.h"
#include "lcd_ui.h"
#include "main.h"

#define OLED_STATUS_REFRESH_MS 150U

static uint8_t s_oled_ready = 0U;
static uint32_t s_last_refresh_ms = 0U;
static uint8_t s_last_water_level = 0xFFU;
static uint8_t s_last_water_out_state = 0xFFU;
static uint8_t s_last_hot_cold_mode = 0xFFU;
static uint16_t s_last_temp_val = 0xFFFFU;
static uint16_t s_last_dev_id = 0xFFFFU;

void OLED_Status_Init(void)
{
    if (s_oled_ready != 0U)
    {
        return;
    }

    LCD_UI_Init();
    s_oled_ready = 1U;
}

void OLED_Status_Poll(void)
{
    if (s_oled_ready != 0U)
    {
        LCD_UI_Poll();
    }
}

void OLED_Status_Show(uint8_t water_level,
                      uint8_t water_out_state,
                      uint16_t temp_val,
                      uint8_t hot_cold_mode,
                      uint16_t dev_id)
{
    uint8_t changed;
    uint32_t now_ms;

    if (s_oled_ready == 0U)
    {
        return;
    }

    changed = ((water_level != s_last_water_level) ||
               (water_out_state != s_last_water_out_state) ||
               (temp_val != s_last_temp_val) ||
               (hot_cold_mode != s_last_hot_cold_mode) ||
               (dev_id != s_last_dev_id)) ? 1U : 0U;

    now_ms = HAL_GetTick();
    if ((changed == 0U) && ((now_ms - s_last_refresh_ms) < OLED_STATUS_REFRESH_MS))
    {
        return;
    }

    LCD_UI_ShowStatus(water_level, water_out_state, temp_val, hot_cold_mode, dev_id);

    s_last_water_level = water_level;
    s_last_water_out_state = water_out_state;
    s_last_temp_val = temp_val;
    s_last_hot_cold_mode = hot_cold_mode;
    s_last_dev_id = dev_id;
    s_last_refresh_ms = now_ms;
}
