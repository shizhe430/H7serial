#ifndef __JPEG_STREAM_H
#define __JPEG_STREAM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"
#include <stdint.h>

#define JPEG_BUF_SIZE  (64U * 1024U)

void       JPEG_Stream_Init(void);
uint8_t   *JPEG_Stream_GetBuf(void);
uint32_t   JPEG_Stream_GetMaxSize(void);
uint32_t   JPEG_Stream_FindFrame(uint32_t *p_soi_offset);

#ifdef __cplusplus
}
#endif

#endif /* __JPEG_STREAM_H */
