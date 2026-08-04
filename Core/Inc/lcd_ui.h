#ifndef __LCD_UI_H
#define __LCD_UI_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void LCD_UI_Init(void);
void LCD_UI_Poll(void);
void LCD_UI_ShowStatus(uint8_t water_level,
                       uint8_t water_out_state,
                       uint16_t temp_val,
                       uint8_t hot_cold_mode,
                       uint16_t dev_id);

#ifdef __cplusplus
}
#endif

#endif /* __LCD_UI_H */
