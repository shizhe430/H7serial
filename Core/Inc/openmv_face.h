#ifndef OPENMV_FACE_H
#define OPENMV_FACE_H

#include "face_ai.h"
#include <stdint.h>

#define OPENMV_FACE_LBP_DESC_SIZE 3776U

typedef struct
{
    uint32_t decode_ms;
    uint32_t grayscale_ms;
    uint32_t haar_ms;
    uint32_t lbp_ms;
    float distance;
    uint16_t candidates;
} openmv_face_stats_t;

uint8_t OpenMVFace_RunJpeg(const uint8_t *jpg,
                           uint32_t jpg_len,
                           face_ai_result_t *result,
                           openmv_face_stats_t *stats);
uint8_t OpenMVFace_BeginEnrollment(uint16_t user_id);
void OpenMVFace_ClearReference(void);
uint8_t OpenMVFace_HasReference(void);

#endif /* OPENMV_FACE_H */
