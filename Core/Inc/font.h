/**
  ******************************************************************************
  * @file    font.h
  * @brief   ASCII字体表头文件
  ******************************************************************************
  */

#ifndef __FONT_H
#define __FONT_H

#include <stdint.h>

extern const uint8_t ASCII_8x16[];   /* 8x16字体: 95字符 x 16字节 */
extern const uint8_t ASCII_16x24[];  /* 16x24字体: 部分字符 x 48字节 */

#define FONT_8X16_WIDTH     8
#define FONT_8X16_HEIGHT    16
#define FONT_16X24_WIDTH    16
#define FONT_16X24_HEIGHT   24

#endif
