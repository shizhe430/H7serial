#ifndef AI_APP_H
#define AI_APP_H

#include "main.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    AI_CLASS_NO_CUP = 0,
    AI_CLASS_EMPTY,
    AI_CLASS_QUARTER,
    AI_CLASS_HALF,
    AI_CLASS_FULL,
    AI_CLASS_COUNT
} ai_class_t;

typedef struct
{
    ai_class_t class_id;
    float confidence;
    int8_t raw_scores[AI_CLASS_COUNT];
} ai_result_t;

uint8_t ai_app_init(void);
uint8_t ai_app_run(const int8_t *input, ai_result_t *result);
const char *ai_app_class_name(ai_class_t class_id);

#ifdef __cplusplus
}
#endif

#endif
