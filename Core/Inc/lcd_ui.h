#ifndef __LCD_UI_H
#define __LCD_UI_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    LCD_UI_ID_EVENT_NONE = 0U,
    LCD_UI_ID_EVENT_ENTER,
    LCD_UI_ID_EVENT_ENROLL,
    LCD_UI_ID_EVENT_DELETE,
    LCD_UI_ID_EVENT_EXIT,
} lcd_ui_id_event_t;

void LCD_UI_Init(void);
void LCD_UI_Poll(void);
void LCD_UI_ShowStatus(uint8_t water_level,
                       uint8_t water_out_state,
                       int16_t temp_tenths,
                       uint8_t hot_cold_mode,
                       uint16_t dev_id);
void LCD_UI_ShowCloud(const char *username_utf8,
                      uint32_t today_ml,
                      uint32_t use_count,
                      uint32_t remaining_ml,
                      uint32_t interval_min,
                      uint32_t suggested_ml,
                      uint8_t advice_received);
void LCD_UI_ShowFaceConfirmation(const uint8_t *rgb888,
                                 uint16_t frame_width,
                                 uint16_t frame_height,
                                 int16_t face_x1,
                                 int16_t face_y1,
                                 int16_t face_x2,
                                 int16_t face_y2,
                                 uint16_t user_id,
                                 uint32_t hold_ms);
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
                              uint8_t max_attempts);
void LCD_UI_CloseFaceScan(void);
uint8_t LCD_UI_IdManagerTakeEvent(lcd_ui_id_event_t *event, uint16_t *selected_id);
uint8_t LCD_UI_IdManagerIsActive(void);
void LCD_UI_IdManagerClose(void);
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
                               const char *status_text);

#ifdef __cplusplus
}
#endif

#endif /* __LCD_UI_H */
