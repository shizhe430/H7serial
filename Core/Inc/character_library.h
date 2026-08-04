#ifndef __CHARACTER_LIBRARY_H
#define __CHARACTER_LIBRARY_H

#include "sys.h"

/* PCtoLCD2002 32x32 中文字库, 列序(column-major), 4字节/列, MSB(bit7=上方) */
/* 索引: 0=热 1=水 2=冷 3=度 4=建 5=议 6=今 7=日 8=出 9=停 10=止 11=位 12=返 13=回 14=正 15=常 16=温 17=℃ 18=+ 19=- 20=0..29=9 30=. 31=I 32=D 33=个 34=人 35=信 36=息 37=余 38=额 39=饮 40=已 41=完 42=成 43=贺 44=爷 45=李 46=奶 47=曹 48=叔 49=： 50=正 51=在 52=￥ 53=姓 54=名 */
extern const u8 font_chinese_32[][128];

/* 在(x,y)处显示32x32中文字符, color为像素颜色 */
void LCD_ShowChinese(u16 x, u16 y, u8 index, u16 color);

#endif
