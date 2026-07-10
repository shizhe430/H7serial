#ifndef __OLED_STATUS_H
#define __OLED_STATUS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void OLED_Status_Init(void);
void OLED_Status_Show(uint8_t water_level,
                      uint8_t water_out_state,
                      uint16_t temp_val,
                      uint8_t hot_cold_mode,
                      uint16_t dev_id);

#ifdef __cplusplus
}
#endif

#endif /* __OLED_STATUS_H */
