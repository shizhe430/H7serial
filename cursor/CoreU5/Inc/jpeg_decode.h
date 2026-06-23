#ifndef JPEG_DECODE_H
#define JPEG_DECODE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 解码 JPEG 到 224x224 灰度 int8（zero point = -128）
 * 返回 0 表示成功，非 0 表示失败
 */
uint8_t jpeg_to_ai_input(const uint8_t *jpg, uint32_t jpg_len, int8_t *dst_input);

#ifdef __cplusplus
}
#endif

#endif
