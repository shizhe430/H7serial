#ifndef __OLED_H
#define __OLED_H

#include "main.h"

#define USE_HORIZONTAL 0  // 0竖屏 1横屏

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

void OLED_Init(void);
void OLED_WR_REG(u8 reg);
void OLED_WR_Byte(u8 dat);
void OLED_Fill(u16 xstr,u8 ystr,u16 xend,u8 yend,u8 color);
void OLED_ShowChinese(u8 x,u8 y,u8 *s,u8 sizey,u8 mode);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 sizey,u8 mode);
void OLED_ShowString(u8 x,u8 y,u8 *dp,u8 sizey,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 sizey,u8 mode);
void OLED_DrawBMP(u8 x,u8 y,u16 length,u8 width,const u8 BMP[],u8 mode);
void OLED_DrawSingleBMP(u8 x,u8 y,u16 length,u8 width,const u8 BMP[],u8 mode);
void OLED_Clear(void);
void Disp_DrinkerUI(u8 water_level, u8 water_out_state, u16 temp_val, u8 hot_cold_mode, u16 dev_id);
void Disp_DrawStatic(void);
void Disp_DrawDynamic(u8 water_level, u8 water_out_state, u16 temp_val, u8 hot_cold_mode, u16 dev_id);
#endif
