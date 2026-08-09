#ifndef OPENMV_FACE_H
#define OPENMV_FACE_H

#include "face_ai.h"
#include <stdint.h>

#define OPENMV_FACE_LBP_DESC_SIZE 3776U
#define OPENMV_FACE_MAX_USERS     5U
#define OPENMV_FACE_TEMPLATES_PER_USER 5U
#define OPENMV_FACE_FRAME_WIDTH   320U
#define OPENMV_FACE_FRAME_HEIGHT  240U

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
uint8_t OpenMVFace_InitDatabase(void);
uint8_t OpenMVFace_BeginEnrollment(uint16_t user_id);
void OpenMVFace_CancelEnrollment(void);
uint8_t OpenMVFace_EnrollmentActive(void);
uint8_t OpenMVFace_GetEnrollmentRemaining(void);
uint8_t OpenMVFace_DeleteUser(uint16_t user_id);
uint8_t OpenMVFace_ClearDatabase(void);
uint8_t OpenMVFace_GetUserCount(void);
uint16_t OpenMVFace_GetUserId(uint8_t slot);
const uint8_t *OpenMVFace_GetLastRgbFrame(void);
void OpenMVFace_ClearReference(void);
uint8_t OpenMVFace_HasReference(void);

#endif /* OPENMV_FACE_H */
