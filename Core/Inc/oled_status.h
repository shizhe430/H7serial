#ifndef __OLED_STATUS_H
#define __OLED_STATUS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void OLED_Status_Init(void);
void OLED_Status_Poll(void);
void OLED_Status_Show(uint8_t water_level,
                      uint8_t water_out_state,
                      int16_t temp_tenths,
                      uint8_t hot_cold_mode,
                      uint16_t dev_id);
void OLED_Status_ShowCloud(const char *username_utf8,
                           uint32_t today_ml,
                           uint32_t use_count,
                           uint32_t remaining_ml,
                           uint32_t interval_min,
                           uint32_t suggested_ml,
                           uint8_t advice_received);

#ifdef __cplusplus
}
#endif

#endif /* __OLED_STATUS_H */
