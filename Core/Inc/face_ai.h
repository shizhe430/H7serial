#ifndef __FACE_AI_H
#define __FACE_AI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"
#include <stdint.h>

#define FACE_AI_EMBEDDING_SIZE 128U

typedef struct
{
    uint8_t status;
    uint8_t detection_valid;
    uint8_t embedding_valid;
    uint8_t reserved;
    float score;
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
    uint32_t detection_ms;
    uint32_t identity_ms;
    uint32_t total_ms;
    float embedding_norm;
    uint8_t reference_ready;
    uint8_t match_valid;
    uint16_t matched_id;
    float similarity;
    float embedding[FACE_AI_EMBEDDING_SIZE];
} face_ai_result_t;

uint8_t FaceAI_RunJpeg(const uint8_t *jpg, uint32_t jpg_len, face_ai_result_t *result);
uint8_t FaceAI_RunJpegDiag(const uint8_t *jpg, uint32_t jpg_len);
uint8_t FaceAI_LoadWeights(void);
uint8_t FaceAI_LoadEnrollment(void);
uint8_t FaceAI_HasEnrollment(void);
uint8_t FaceAI_EnrollReference(uint16_t user_id, const face_ai_result_t *result);
uint8_t FaceAI_ClearEnrollment(void);
uint8_t FaceAI_Recognize(const face_ai_result_t *result, uint16_t *matched_id, float *similarity);

#ifdef __cplusplus
}
#endif

#endif /* __FACE_AI_H */
