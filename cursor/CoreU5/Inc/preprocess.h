#ifndef PREPROCESS_H
#define PREPROCESS_H

#include "main.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PREPROCESS_SRC_WIDTH    320U
#define PREPROCESS_SRC_HEIGHT   240U
#define PREPROCESS_DST_WIDTH    224U
#define PREPROCESS_DST_HEIGHT   224U
#define PREPROCESS_DST_SIZE     (PREPROCESS_DST_WIDTH * PREPROCESS_DST_HEIGHT)

void preprocess_rgb565_to_input(const uint16_t *src_rgb565, int8_t *dst_input);

#ifdef __cplusplus
}
#endif

#endif
