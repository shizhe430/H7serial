#include "preprocess.h"

#define MODEL_INPUT_ZERO_POINT  (-128)

static uint8_t rgb565_to_gray(uint16_t pixel)
{
    uint8_t r = (uint8_t)(((pixel >> 11) & 0x1FU) << 3);
    uint8_t g = (uint8_t)(((pixel >> 5) & 0x3FU) << 2);
    uint8_t b = (uint8_t)((pixel & 0x1FU) << 3);

    return (uint8_t)(((uint16_t)r * 30U + (uint16_t)g * 59U + (uint16_t)b * 11U) / 100U);
}

void preprocess_rgb565_to_input(const uint16_t *src_rgb565, int8_t *dst_input)
{
    uint32_t y;

    if ((src_rgb565 == NULL) || (dst_input == NULL))
    {
        return;
    }

    for (y = 0U; y < PREPROCESS_DST_HEIGHT; y++)
    {
        uint32_t src_y = (y * PREPROCESS_SRC_HEIGHT) / PREPROCESS_DST_HEIGHT;
        uint32_t x;

        for (x = 0U; x < PREPROCESS_DST_WIDTH; x++)
        {
            uint32_t src_x = (x * PREPROCESS_SRC_WIDTH) / PREPROCESS_DST_WIDTH;
            uint16_t pixel = src_rgb565[(src_y * PREPROCESS_SRC_WIDTH) + src_x];
            uint8_t gray = rgb565_to_gray(pixel);

            dst_input[(y * PREPROCESS_DST_WIDTH) + x] = (int8_t)((int16_t)gray + MODEL_INPUT_ZERO_POINT);
        }
    }
}
