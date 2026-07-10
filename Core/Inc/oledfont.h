#ifndef __OLEDFONT_H
#define __OLEDFONT_H

#include "main.h"

typedef unsigned char  u8;
typedef unsigned int   u16;
typedef unsigned long  u32;

// 汉字结构体定义
// 每个汉字的 UTF-8 编码 + 16×16点阵数据
typedef struct {
    uint8_t Utf8[3];    // UTF-8编码（中文固定占3字节）
    uint8_t Msk[32];    // 16×16点阵数据（列行式阳码，和你原来的一致）
} typFNT_UTF8_16;

typedef struct
{
	u8 Index[2];
	u8 Msk[72];
}typFNT_GB24;

typedef struct
{
	u8 Index[2];
	u8 Msk[128];
}typFNT_GB32;



// 只声明，不定义（解决重复定义报错）
extern const u8           ascii_1608[][16];
extern const u8           ascii_2412[][48];
extern const u8           ascii_3216[][64];

extern const typFNT_UTF8_16 utf8_tfont16[];
extern const int utf8_tfont16_len;
extern const typFNT_GB24  tfont24[];
extern const int tfont24_len;
extern const typFNT_GB32  tfont32[];
extern const int tfont32_len;

#endif
