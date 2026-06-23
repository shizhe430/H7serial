#include "ai_app.h"
#include "network.h"
#include "network_data.h"
#include <string.h>

STAI_NETWORK_CONTEXT_DECLARE(g_network_ctx, STAI_NETWORK_CONTEXT_SIZE);
static STAI_ALIGNED(4) int8_t g_network_input[STAI_NETWORK_IN_1_SIZE_BYTES];
static STAI_ALIGNED(4) int8_t g_network_output[STAI_NETWORK_OUT_1_SIZE_BYTES];
static STAI_ALIGNED(4) uint8_t g_network_activations[STAI_NETWORK_ACTIVATIONS_SIZE_BYTES];

static const char *const g_class_names[AI_CLASS_COUNT] = {
    "no_cup",
    "empty",
    "quarter",
    "half",
    "full"
};

static float ai_score_to_confidence(int8_t score)
{
    int32_t shifted = (int32_t)score - STAI_NETWORK_OUT_1_ZERO_POINT;
    float real_score = (float)shifted * STAI_NETWORK_OUT_1_SCALE;

    if (real_score <= 0.0f)
    {
        return 0.0f;
    }

    if (real_score >= 1.0f)
    {
        return 1.0f;
    }

    return real_score;
}

uint8_t ai_app_init(void)
{
    stai_ptr activations[STAI_NETWORK_ACTIVATIONS_NUM] = { g_network_activations };
    stai_ptr weights[STAI_NETWORK_WEIGHTS_NUM] = { (stai_ptr)g_network_weights_array };
    stai_ptr inputs[STAI_NETWORK_IN_NUM] = { (stai_ptr)g_network_input };
    stai_ptr outputs[STAI_NETWORK_OUT_NUM] = { (stai_ptr)g_network_output };

    if (stai_network_init(g_network_ctx) != STAI_SUCCESS)
    {
        return 1U;
    }

    if (stai_network_set_activations(g_network_ctx, activations, STAI_NETWORK_ACTIVATIONS_NUM) != STAI_SUCCESS)
    {
        return 1U;
    }

    if (stai_network_set_weights(g_network_ctx, weights, STAI_NETWORK_WEIGHTS_NUM) != STAI_SUCCESS)
    {
        return 1U;
    }

    if (stai_network_set_inputs(g_network_ctx, inputs, STAI_NETWORK_IN_NUM) != STAI_SUCCESS)
    {
        return 1U;
    }

    if (stai_network_set_outputs(g_network_ctx, outputs, STAI_NETWORK_OUT_NUM) != STAI_SUCCESS)
    {
        return 1U;
    }

    return 0U;
}

uint8_t ai_app_run(const int8_t *input, ai_result_t *result)
{
    uint32_t i;
    uint32_t best_idx = 0U;
    int8_t best_score;

    if ((input == NULL) || (result == NULL))
    {
        return 1U;
    }

    memcpy(g_network_input, input, sizeof(g_network_input));

    if (stai_network_run(g_network_ctx, STAI_MODE_SYNC) != STAI_SUCCESS)
    {
        return 1U;
    }

    memcpy(result->raw_scores, g_network_output, sizeof(result->raw_scores));

    best_score = result->raw_scores[0];
    for (i = 1U; i < AI_CLASS_COUNT; i++)
    {
        if (result->raw_scores[i] > best_score)
        {
            best_score = result->raw_scores[i];
            best_idx = i;
        }
    }

    result->class_id = (ai_class_t)best_idx;
    result->confidence = ai_score_to_confidence(best_score);

    return 0U;
}

const char *ai_app_class_name(ai_class_t class_id)
{
    if ((uint32_t)class_id >= AI_CLASS_COUNT)
    {
        return "unknown";
    }

    return g_class_names[class_id];
}
