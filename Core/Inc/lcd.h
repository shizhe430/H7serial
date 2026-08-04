#ifndef __LCD_H
#define __LCD_H
#include "sys.h"

/* LCD设备结构 */
typedef struct {
    u16 width;
    u16 height;
    u16 id;
    u8  dir;
    u16 wramcmd;
    u16 setxcmd;
    u16 setycmd;
} _lcd_dev;

extern _lcd_dev lcddev;
extern u32 POINT_COLOR;
extern u32 BACK_COLOR;

/* FMC/LCD接口 - 16位8080并口 */
typedef struct {
    vu16 LCD_REG;
    vu16 LCD_RAM;
} LCD_TypeDef;

#define LCD_BASE  ((u32)(0x60000000 | 0x0007FFFE))
#define LCD       ((LCD_TypeDef *)LCD_BASE)

/* MPU */
#define LCD_REGION_NUMBER   MPU_REGION_NUMBER2
#define LCD_ADDRESS_START   0x60000000
#define LCD_REGION_SIZE     MPU_REGION_SIZE_256MB

/* 背光 PB5 */
#define LCD_LED(n) (n ? HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET) : HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET))

/* 扫描方向 */
#define L2R_U2D  0
#define L2R_D2U  1
#define R2L_U2D  2
#define R2L_D2U  3
#define U2D_L2R  4
#define U2D_R2L  5
#define D2U_L2R  6
#define D2U_R2L  7
#define DFT_SCAN_DIR  L2R_U2D

/* 颜色 */
#define WHITE         0xFFFF
#define BLACK         0x0000
#define BLUE          0x001F
#define BRED          0xF81F
#define GRED          0xFFE0
#define GBLUE         0x07FF
#define RED           0xF800
#define MAGENTA       0xF81F
#define GREEN         0x07E0
#define CYAN          0x7FFF
#define YELLOW        0xFFE0
#define BROWN         0xBC40
#define BRRED         0xFC07
#define GRAY          0x8430
#define DARKBLUE      0x01CF
#define LIGHTBLUE     0x7D7C
#define GRAYBLUE      0x5458
#define LIGHTGREEN    0x841F
#define LGRAY         0xC618
#define LGRAYBLUE     0xA651
#define LBBLUE        0x2B12

/* 函数声明 */
void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(u32 Color);
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_DrawPoint(u16 x, u16 y);
void LCD_Fast_DrawPoint(u16 x, u16 y, u32 color);
u32  LCD_ReadPoint(u16 x, u16 y);
void LCD_Draw_Circle(u16 x0, u16 y0, u8 r);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u32 color);
void LCD_Color_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 *color);
void LCD_ShowChar(u16 x, u16 y, u8 num, u8 size, u8 mode);
void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size);
void LCD_ShowxNum(u16 x, u16 y, u32 num, u8 len, u8 size, u8 mode);
void LCD_ShowString(u16 x, u16 y, u16 width, u16 height, u8 size, u8 *p);
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
u16  LCD_ReadReg(u16 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);
void LCD_Scan_Dir(u8 dir);
void LCD_Display_Dir(u8 dir);
void LCD_Set_Window(u16 sx, u16 sy, u16 width, u16 height);
void LCD_MPU_Config(void);
void LCD_FMC_Init(void);

#endif
