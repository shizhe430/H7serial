#ifndef JPEG_DECODE_H
#define JPEG_DECODE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Decode JPEG to the AI input buffer.
 * Return 0 on success, non-zero on failure.
 */
uint8_t jpeg_to_ai_input(const uint8_t *jpg, uint32_t jpg_len, void *dst_input);
uint8_t jpeg_to_ai_input_view(const uint8_t *jpg, uint32_t jpg_len, void *dst_input,
                              int16_t offset_x, int16_t offset_y, uint8_t fill_gray);
uint8_t jpeg_to_rgb888(const uint8_t *jpg, uint32_t jpg_len,
                       uint8_t *dst_rgb, uint16_t dst_w, uint16_t dst_h);
uint8_t jpeg_decode_last_prepare_status(void);
uint8_t jpeg_decode_last_decomp_status(void);
uint16_t jpeg_decode_last_width(void);
uint16_t jpeg_decode_last_height(void);

#ifdef __cplusplus
}
#endif

#endif
