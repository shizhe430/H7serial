/*
 * Board-side port of OpenMV's classic Haar/LBP face example.
 *
 * The cascade evaluation and LBP operator follow OpenMV's MIT-licensed imlib
 * implementation. Full SDRAM integral images replace OpenMV's moving-window
 * allocator so this diagnostic can run without MicroPython's memory runtime.
 */
#include "openmv_face.h"
#include "openmv_frontalface_data.h"
#include "jpeg_decode.h"
#include "stm32h7xx_hal.h"
#include <math.h>
#include <stddef.h>
#include <string.h>

#define OPENMV_FACE_IMAGE_W              320U
#define OPENMV_FACE_IMAGE_H              240U
#define OPENMV_FACE_RGB_BASE             0xC0500000UL
#define OPENMV_FACE_GRAY_BASE            0xC0540000UL
#define OPENMV_FACE_SUM_BASE             0xC0560000UL
#define OPENMV_FACE_SSQ_BASE             0xC05B0000UL
#define OPENMV_FACE_NORMALIZED_BASE      0xC0600000UL
#define OPENMV_FACE_RAW_GRAY_BASE        0xC0610000UL
#define OPENMV_FACE_NORMALIZED_SIZE      128U
#define OPENMV_FACE_HAAR_THRESHOLD       0.5f
#define OPENMV_FACE_HAAR_SCALE           1.10f
#define OPENMV_FACE_HAAR_STAGES          25
#define OPENMV_FACE_MAX_CANDIDATES       256U
#define OPENMV_FACE_MIN_BOX_SIZE         30U
#define OPENMV_FACE_MAX_BOX_SIZE         200U
#define OPENMV_FACE_TARGET_BOX_SIZE      90U
#define OPENMV_FACE_MIN_NEIGHBORS        5U
#define OPENMV_FACE_GROUP_EPS_PERCENT    20U
#define OPENMV_FACE_MATCH_DISTANCE_MAX   0.10f
#define OPENMV_FACE_LBP_HIST_SIZE        59U
#define OPENMV_FACE_LBP_REGIONS          8U
#define OPENMV_FACE_ENROLL_FRAMES        5U

typedef struct
{
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
} openmv_rect_t;

static uint8_t *const s_rgb = (uint8_t *)OPENMV_FACE_RGB_BASE;
static uint8_t *const s_gray = (uint8_t *)OPENMV_FACE_GRAY_BASE;
static uint8_t *const s_raw_gray = (uint8_t *)OPENMV_FACE_RAW_GRAY_BASE;
static uint32_t *const s_sum = (uint32_t *)OPENMV_FACE_SUM_BASE;
static uint32_t *const s_ssq = (uint32_t *)OPENMV_FACE_SSQ_BASE;
static uint8_t *const s_normalized = (uint8_t *)OPENMV_FACE_NORMALIZED_BASE;
static openmv_rect_t s_candidates[OPENMV_FACE_MAX_CANDIDATES];
static uint16_t s_candidate_parent[OPENMV_FACE_MAX_CANDIDATES];
static float s_last_desc[OPENMV_FACE_LBP_DESC_SIZE];
static float s_reference_desc[OPENMV_FACE_LBP_DESC_SIZE];
static float s_enrollment_sum[OPENMV_FACE_LBP_DESC_SIZE];
static uint8_t s_reference_valid;
static uint16_t s_reference_id;
static uint8_t s_enrollment_remaining;
static uint16_t s_enrollment_user_id;

static const uint8_t s_uniform_lbp[256] = {
     0,  1,  2,  3,  4, 58,  5,  6,  7, 58, 58, 58,  8, 58,  9, 10,
    11, 58, 58, 58, 58, 58, 58, 58, 12, 58, 58, 58, 13, 58, 14, 15,
    16, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58,
    17, 58, 58, 58, 58, 58, 58, 58, 18, 58, 58, 58, 19, 58, 20, 21,
    22, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58,
    58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58,
    23, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58,
    24, 58, 58, 58, 58, 58, 58, 25, 58, 58, 58, 26, 58, 27, 28,
    29, 30, 58, 31, 58, 58, 58, 32, 58, 58, 58, 58, 58, 58, 58, 33,
    58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 34,
    58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58,
    58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 35,
    36, 37, 58, 38, 58, 58, 58, 39, 58, 58, 58, 58, 58, 58, 58, 40,
    58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 41,
    42, 43, 58, 44, 58, 58, 58, 45, 58, 58, 58, 58, 58, 58, 58, 46,
    47, 48, 58, 49, 58, 58, 58, 50, 51, 52, 58, 53, 54, 55, 56, 57,
};

static uint32_t openmv_integral_lookup(const uint32_t *integral,
                                       uint16_t stride,
                                       int x,
                                       int y,
                                       int w,
                                       int h)
{
    return integral[(uint32_t)(y + h) * stride + (uint32_t)(x + w)]
         + integral[(uint32_t)y * stride + (uint32_t)x]
         - integral[(uint32_t)y * stride + (uint32_t)(x + w)]
         - integral[(uint32_t)(y + h) * stride + (uint32_t)x];
}

static void openmv_rgb_to_gray(void)
{
    uint32_t i;

    for (i = 0U; i < (OPENMV_FACE_IMAGE_W * OPENMV_FACE_IMAGE_H); i++)
    {
        uint32_t r = s_rgb[i * 3U + 0U];
        uint32_t g = s_rgb[i * 3U + 1U];
        uint32_t b = s_rgb[i * 3U + 2U];
        uint8_t gray = (uint8_t)((77U * r + 150U * g + 29U * b) >> 8U);

        s_raw_gray[i] = gray;
        s_gray[i] = gray;
    }
}

static void openmv_equalize_gray(void)
{
    uint32_t histogram[256] = {0};
    uint8_t mapping[256];
    uint32_t cumulative = 0U;
    uint32_t first = 0U;
    uint32_t pixels = OPENMV_FACE_IMAGE_W * OPENMV_FACE_IMAGE_H;
    uint32_t i;

    for (i = 0U; i < pixels; i++)
    {
        histogram[s_gray[i]]++;
    }
    for (i = 0U; i < 256U; i++)
    {
        cumulative += histogram[i];
        if ((first == 0U) && (cumulative != 0U))
        {
            first = cumulative;
        }
        if (pixels == first)
        {
            mapping[i] = (uint8_t)i;
        }
        else if (cumulative <= first)
        {
            mapping[i] = 0U;
        }
        else
        {
            mapping[i] = (uint8_t)(((cumulative - first) * 255U + ((pixels - first) / 2U))
                                   / (pixels - first));
        }
    }
    for (i = 0U; i < pixels; i++)
    {
        s_gray[i] = mapping[s_gray[i]];
    }
}

static void openmv_build_integrals(uint16_t scaled_w, uint16_t scaled_h)
{
    uint32_t x_ratio = ((OPENMV_FACE_IMAGE_W << 16U) / scaled_w) + 1U;
    uint32_t y_ratio = ((OPENMV_FACE_IMAGE_H << 16U) / scaled_h) + 1U;
    uint16_t y;

    for (y = 0U; y < scaled_h; y++)
    {
        uint32_t sy = ((uint32_t)y * y_ratio) >> 16U;
        uint32_t row_sum = 0U;
        uint32_t row_ssq = 0U;
        uint16_t x;

        if (sy >= OPENMV_FACE_IMAGE_H)
        {
            sy = OPENMV_FACE_IMAGE_H - 1U;
        }
        for (x = 0U; x < scaled_w; x++)
        {
            uint32_t sx = ((uint32_t)x * x_ratio) >> 16U;
            uint32_t pixel;
            uint32_t above = (y == 0U) ? 0U : s_sum[(uint32_t)(y - 1U) * scaled_w + x];
            uint32_t above_ssq = (y == 0U) ? 0U : s_ssq[(uint32_t)(y - 1U) * scaled_w + x];

            if (sx >= OPENMV_FACE_IMAGE_W)
            {
                sx = OPENMV_FACE_IMAGE_W - 1U;
            }
            pixel = s_gray[sy * OPENMV_FACE_IMAGE_W + sx];
            row_sum += pixel;
            row_ssq += pixel * pixel;
            s_sum[(uint32_t)y * scaled_w + x] = row_sum + above;
            s_ssq[(uint32_t)y * scaled_w + x] = row_ssq + above_ssq;
        }
    }
}

static int openmv_run_window(uint16_t stride, int x, int y)
{
    uint32_t n = (uint32_t)openmv_frontalface_window_w * (uint32_t)openmv_frontalface_window_h;
    uint32_t image_sum = openmv_integral_lookup(s_sum, stride, x, y,
                                                openmv_frontalface_window_w,
                                                openmv_frontalface_window_h);
    uint32_t image_ssq = openmv_integral_lookup(s_ssq, stride, x, y,
                                                openmv_frontalface_window_w,
                                                openmv_frontalface_window_h);
    uint32_t mean = image_sum / n;
    uint32_t variance = image_ssq / n - (mean * mean);
    uint32_t stddev;
    uint32_t feature_index = 0U;
    uint32_t weight_index = 0U;
    uint32_t rectangle_index = 0U;
    int stage;

    if (variance < (50U * 50U))
    {
        return 0;
    }
    stddev = (uint32_t)sqrtf((float)((image_ssq * n) - (image_sum * image_sum)));

    for (stage = 0; (stage < openmv_frontalface_n_stages) &&
                    (stage < OPENMV_FACE_HAAR_STAGES); stage++)
    {
        int32_t stage_sum = 0;
        uint32_t feature_in_stage;

        for (feature_in_stage = 0U;
             feature_in_stage < openmv_frontalface_stages_array[stage];
             feature_in_stage++, feature_index++)
        {
            int32_t weighted_sum = 0;
            int32_t tree_threshold = (int32_t)openmv_frontalface_tree_thresh_array[feature_index]
                                   * (int32_t)stddev;
            int rectangle_count = openmv_frontalface_num_rectangles_array[feature_index];
            int rectangle;

            for (rectangle = 0; rectangle < rectangle_count; rectangle++)
            {
                uint32_t base = rectangle_index + ((uint32_t)rectangle * 4U);
                int rx = openmv_frontalface_rectangles_array[base + 0U];
                int ry = openmv_frontalface_rectangles_array[base + 1U];
                int rw = openmv_frontalface_rectangles_array[base + 2U];
                int rh = openmv_frontalface_rectangles_array[base + 3U];
                int32_t rectangle_sum = (int32_t)openmv_integral_lookup(s_sum, stride,
                                                                        x + rx, y + ry,
                                                                        rw, rh);
                weighted_sum += rectangle_sum
                              * ((int32_t)openmv_frontalface_weights_array[weight_index + rectangle] << 12);
            }

            stage_sum += (weighted_sum >= tree_threshold)
                           ? openmv_frontalface_alpha2_array[feature_index]
                           : openmv_frontalface_alpha1_array[feature_index];
            weight_index += (uint32_t)rectangle_count;
            rectangle_index += (uint32_t)rectangle_count * 4U;
        }

        if ((float)stage_sum < (OPENMV_FACE_HAAR_THRESHOLD
                                * (float)openmv_frontalface_stages_thresh_array[stage]))
        {
            return 0;
        }
    }
    return 1;
}

static int32_t openmv_abs_i32(int32_t value)
{
    return (value < 0) ? -value : value;
}

static uint8_t openmv_rect_similar(const openmv_rect_t *a, const openmv_rect_t *b)
{
    int32_t min_w = (a->w < b->w) ? a->w : b->w;
    int32_t min_h = (a->h < b->h) ? a->h : b->h;
    int32_t delta = ((min_w + min_h) * OPENMV_FACE_GROUP_EPS_PERCENT) / 200;

    return (uint8_t)((openmv_abs_i32((int32_t)a->x - b->x) <= delta) &&
                     (openmv_abs_i32((int32_t)a->y - b->y) <= delta) &&
                     (openmv_abs_i32(((int32_t)a->x + a->w) -
                                     ((int32_t)b->x + b->w)) <= delta) &&
                     (openmv_abs_i32(((int32_t)a->y + a->h) -
                                     ((int32_t)b->y + b->h)) <= delta));
}

static uint16_t openmv_candidate_root(uint16_t index)
{
    uint16_t root = index;

    while (s_candidate_parent[root] != root)
    {
        root = s_candidate_parent[root];
    }
    while (s_candidate_parent[index] != index)
    {
        uint16_t parent = s_candidate_parent[index];
        s_candidate_parent[index] = root;
        index = parent;
    }
    return root;
}

static uint8_t openmv_select_best_merged(uint16_t count,
                                         openmv_rect_t *face,
                                         uint16_t *neighbor_count)
{
    uint16_t best_count = 0U;
    uint16_t best_size_error = 0xFFFFU;
    openmv_rect_t best = {0};
    uint16_t i;

    if ((count == 0U) || (face == NULL) || (neighbor_count == NULL))
    {
        return 1U;
    }
    for (i = 0U; i < count; i++)
    {
        uint16_t j;

        s_candidate_parent[i] = i;
        for (j = 0U; j < i; j++)
        {
            if (openmv_rect_similar(&s_candidates[i], &s_candidates[j]) != 0U)
            {
                uint16_t root_i = openmv_candidate_root(i);
                uint16_t root_j = openmv_candidate_root(j);

                if (root_i != root_j)
                {
                    s_candidate_parent[root_i] = root_j;
                }
            }
        }
    }
    for (i = 0U; i < count; i++)
    {
        int32_t x = 0;
        int32_t y = 0;
        int32_t w = 0;
        int32_t h = 0;
        uint16_t merged_count = 0U;
        uint16_t j;

        if (openmv_candidate_root(i) != i)
        {
            continue;
        }
        for (j = 0U; j < count; j++)
        {
            if (openmv_candidate_root(j) == i)
            {
                x += s_candidates[j].x;
                y += s_candidates[j].y;
                w += s_candidates[j].w;
                h += s_candidates[j].h;
                merged_count++;
            }
        }
        if (merged_count == 0U)
        {
            continue;
        }
        x /= merged_count;
        y /= merged_count;
        w /= merged_count;
        h /= merged_count;
        {
            uint16_t average_size = (uint16_t)((w + h) / 2);
            uint16_t size_error = (average_size > OPENMV_FACE_TARGET_BOX_SIZE)
                                    ? (average_size - OPENMV_FACE_TARGET_BOX_SIZE)
                                    : (OPENMV_FACE_TARGET_BOX_SIZE - average_size);

            if ((merged_count > best_count) ||
                ((merged_count == best_count) && (size_error < best_size_error)))
            {
                best_count = merged_count;
                best_size_error = size_error;
                best.x = (int16_t)x;
                best.y = (int16_t)y;
                best.w = (int16_t)w;
                best.h = (int16_t)h;
            }
        }
    }
    if (best_count == 0U)
    {
        return 1U;
    }
    *face = best;
    *neighbor_count = best_count;
    return 0U;
}

static uint16_t openmv_detect_face(openmv_rect_t *face)
{
    float factor;
    int step = (int)((OPENMV_FACE_IMAGE_W * 50U) / 1000U);
    uint16_t count = 0U;
    uint16_t neighbor_count = 0U;

    if (step > openmv_frontalface_window_h)
    {
        step = openmv_frontalface_window_h;
    }
    for (factor = 1.0f; ; factor *= OPENMV_FACE_HAAR_SCALE)
    {
        uint16_t scaled_w = (uint16_t)((float)OPENMV_FACE_IMAGE_W / factor);
        uint16_t scaled_h = (uint16_t)((float)OPENMV_FACE_IMAGE_H / factor);
        int x_limit;
        int y_limit;
        int y;

        if ((scaled_w < (uint16_t)openmv_frontalface_window_w) ||
            (scaled_h < (uint16_t)openmv_frontalface_window_h))
        {
            break;
        }
        openmv_build_integrals(scaled_w, scaled_h);
        step = (int)((float)step / factor);
        if (step == 0)
        {
            step = 1;
        }
        x_limit = (int)scaled_w - openmv_frontalface_window_w;
        y_limit = (int)scaled_h - openmv_frontalface_window_h;
        for (y = 0; y < y_limit; y += step)
        {
            int x;
            for (x = 0; x < x_limit; x += step)
            {
                if (openmv_run_window(scaled_w, x, y) != 0)
                {
                    uint16_t box_size = (uint16_t)(((float)openmv_frontalface_window_w * factor) + 0.5f);

                    if ((box_size >= OPENMV_FACE_MIN_BOX_SIZE) &&
                        (box_size <= OPENMV_FACE_MAX_BOX_SIZE) &&
                        (count < OPENMV_FACE_MAX_CANDIDATES))
                    {
                        s_candidates[count].x = (int16_t)(((float)x * factor) + 0.5f);
                        s_candidates[count].y = (int16_t)(((float)y * factor) + 0.5f);
                        s_candidates[count].w = (int16_t)(((float)openmv_frontalface_window_w * factor) + 0.5f);
                        s_candidates[count].h = (int16_t)(((float)openmv_frontalface_window_h * factor) + 0.5f);
                        count++;
                    }
                }
            }
        }
    }
    if (count != 0U)
    {
        (void)openmv_select_best_merged(count, face, &neighbor_count);
        if (neighbor_count < OPENMV_FACE_MIN_NEIGHBORS)
        {
            neighbor_count = 0U;
        }
    }
    return neighbor_count;
}

static void openmv_normalize_face(const openmv_rect_t *face)
{
    int margin_x = (face->w * 15) / 100;
    int margin_y = (face->h * 15) / 100;
    int x0 = face->x - margin_x;
    int y0 = face->y - margin_y;
    int x1 = face->x + face->w + margin_x;
    int y1 = face->y + face->h + margin_y;
    uint32_t y;

    if (x0 < 0) { x0 = 0; }
    if (y0 < 0) { y0 = 0; }
    if (x1 > (int)OPENMV_FACE_IMAGE_W) { x1 = OPENMV_FACE_IMAGE_W; }
    if (y1 > (int)OPENMV_FACE_IMAGE_H) { y1 = OPENMV_FACE_IMAGE_H; }
    for (y = 0U; y < OPENMV_FACE_NORMALIZED_SIZE; y++)
    {
        float source_y = (float)y0
                       + ((((float)y + 0.5f) * (float)(y1 - y0)) / OPENMV_FACE_NORMALIZED_SIZE)
                       - 0.5f;
        int y_low = (int)source_y;
        int y_high;
        float y_fraction;
        uint32_t x;

        if (y_low < y0) { y_low = y0; }
        y_high = y_low + 1;
        if (y_high >= y1) { y_high = y1 - 1; }
        y_fraction = source_y - (float)y_low;
        if (y_fraction < 0.0f) { y_fraction = 0.0f; }
        for (x = 0U; x < OPENMV_FACE_NORMALIZED_SIZE; x++)
        {
            float source_x = (float)x0
                           + ((((float)x + 0.5f) * (float)(x1 - x0)) / OPENMV_FACE_NORMALIZED_SIZE)
                           - 0.5f;
            int x_low = (int)source_x;
            int x_high;
            float x_fraction;
            float top;
            float bottom;
            float value;

            if (x_low < x0) { x_low = x0; }
            x_high = x_low + 1;
            if (x_high >= x1) { x_high = x1 - 1; }
            x_fraction = source_x - (float)x_low;
            if (x_fraction < 0.0f) { x_fraction = 0.0f; }
            top = (float)s_raw_gray[(uint32_t)y_low * OPENMV_FACE_IMAGE_W + (uint32_t)x_low]
                + x_fraction * ((float)s_raw_gray[(uint32_t)y_low * OPENMV_FACE_IMAGE_W + (uint32_t)x_high]
                                - (float)s_raw_gray[(uint32_t)y_low * OPENMV_FACE_IMAGE_W + (uint32_t)x_low]);
            bottom = (float)s_raw_gray[(uint32_t)y_high * OPENMV_FACE_IMAGE_W + (uint32_t)x_low]
                   + x_fraction * ((float)s_raw_gray[(uint32_t)y_high * OPENMV_FACE_IMAGE_W + (uint32_t)x_high]
                                   - (float)s_raw_gray[(uint32_t)y_high * OPENMV_FACE_IMAGE_W + (uint32_t)x_low]);
            value = top + y_fraction * (bottom - top);
            s_normalized[y * OPENMV_FACE_NORMALIZED_SIZE + x] = (uint8_t)(value + 0.5f);
        }
    }

    {
        uint32_t histogram[256] = {0};
        uint8_t mapping[256];
        uint32_t cumulative = 0U;
        uint32_t first = 0U;
        uint32_t pixels = OPENMV_FACE_NORMALIZED_SIZE * OPENMV_FACE_NORMALIZED_SIZE;
        uint32_t i;

        for (i = 0U; i < pixels; i++) { histogram[s_normalized[i]]++; }
        for (i = 0U; i < 256U; i++)
        {
            cumulative += histogram[i];
            if ((first == 0U) && (cumulative != 0U)) { first = cumulative; }
            if (pixels == first) { mapping[i] = (uint8_t)i; }
            else if (cumulative <= first) { mapping[i] = 0U; }
            else
            {
                mapping[i] = (uint8_t)(((cumulative - first) * 255U + ((pixels - first) / 2U))
                                       / (pixels - first));
            }
        }
        for (i = 0U; i < pixels; i++) { s_normalized[i] = mapping[s_normalized[i]]; }
    }
}

static uint8_t openmv_lbp_descriptor(const openmv_rect_t *face, float *desc)
{
    int y;

    if ((face->w <= 0) || (face->h <= 0))
    {
        return 1U;
    }
    openmv_normalize_face(face);

    memset(desc, 0, sizeof(float) * OPENMV_FACE_LBP_DESC_SIZE);
    for (y = 0; y < (int)OPENMV_FACE_NORMALIZED_SIZE - 2; y++)
    {
        int region_y = (y < 96) ? (y / 16) : (6 + ((y - 96) / 15));
        int x;

        for (x = 0; x < (int)OPENMV_FACE_NORMALIZED_SIZE - 2; x++)
        {
            uint8_t code = 0U;
            uint8_t center = s_normalized[(uint32_t)(y + 1) * OPENMV_FACE_NORMALIZED_SIZE + (uint32_t)(x + 1)];
            int region_x = (x < 96) ? (x / 16) : (6 + ((x - 96) / 15));
            uint32_t histogram;

            code |= (uint8_t)((s_normalized[(uint32_t)(y + 0) * OPENMV_FACE_NORMALIZED_SIZE + (uint32_t)(x + 0)] >= center) << 0);
            code |= (uint8_t)((s_normalized[(uint32_t)(y + 0) * OPENMV_FACE_NORMALIZED_SIZE + (uint32_t)(x + 1)] >= center) << 1);
            code |= (uint8_t)((s_normalized[(uint32_t)(y + 0) * OPENMV_FACE_NORMALIZED_SIZE + (uint32_t)(x + 2)] >= center) << 2);
            code |= (uint8_t)((s_normalized[(uint32_t)(y + 1) * OPENMV_FACE_NORMALIZED_SIZE + (uint32_t)(x + 2)] >= center) << 3);
            code |= (uint8_t)((s_normalized[(uint32_t)(y + 2) * OPENMV_FACE_NORMALIZED_SIZE + (uint32_t)(x + 2)] >= center) << 4);
            code |= (uint8_t)((s_normalized[(uint32_t)(y + 2) * OPENMV_FACE_NORMALIZED_SIZE + (uint32_t)(x + 1)] >= center) << 5);
            code |= (uint8_t)((s_normalized[(uint32_t)(y + 2) * OPENMV_FACE_NORMALIZED_SIZE + (uint32_t)(x + 0)] >= center) << 6);
            code |= (uint8_t)((s_normalized[(uint32_t)(y + 1) * OPENMV_FACE_NORMALIZED_SIZE + (uint32_t)(x + 0)] >= center) << 7);
            histogram = ((uint32_t)region_y * OPENMV_FACE_LBP_REGIONS + (uint32_t)region_x)
                      * OPENMV_FACE_LBP_HIST_SIZE + s_uniform_lbp[code];
            desc[histogram] += 1.0f;
        }
    }

    {
        uint32_t region;
        for (region = 0U; region < (OPENMV_FACE_LBP_REGIONS * OPENMV_FACE_LBP_REGIONS); region++)
        {
            float total = 0.0f;
            uint32_t bin;
            for (bin = 0U; bin < OPENMV_FACE_LBP_HIST_SIZE; bin++)
            {
                total += desc[region * OPENMV_FACE_LBP_HIST_SIZE + bin];
            }
            if (total > 0.0f)
            {
                for (bin = 0U; bin < OPENMV_FACE_LBP_HIST_SIZE; bin++)
                {
                    desc[region * OPENMV_FACE_LBP_HIST_SIZE + bin] /= total;
                }
            }
        }
    }
    return 0U;
}

static float openmv_lbp_distance(const float *a, const float *b)
{
    float sum = 0.0f;
    uint32_t i;

    for (i = 0U; i < OPENMV_FACE_LBP_DESC_SIZE; i++)
    {
        float difference = a[i] - b[i];
        float denominator = a[i] + b[i] + 1.0e-8f;
        sum += (difference * difference) / denominator;
    }
    return 0.5f * sum / 64.0f;
}

uint8_t OpenMVFace_RunJpeg(const uint8_t *jpg,
                           uint32_t jpg_len,
                           face_ai_result_t *result,
                           openmv_face_stats_t *stats)
{
    openmv_rect_t face = {0};
    uint32_t started_ms;
    uint32_t phase_ms;
    float distance = 0.0f;
    uint16_t candidates;
    uint8_t status;

    if ((jpg == NULL) || (jpg_len == 0U) || (result == NULL) || (stats == NULL))
    {
        return 1U;
    }
    memset(result, 0, sizeof(*result));
    memset(stats, 0, sizeof(*stats));
    started_ms = HAL_GetTick();

    phase_ms = HAL_GetTick();
    status = jpeg_to_rgb888(jpg, jpg_len, s_rgb, OPENMV_FACE_IMAGE_W, OPENMV_FACE_IMAGE_H);
    stats->decode_ms = HAL_GetTick() - phase_ms;
    if (status != 0U)
    {
        result->status = (uint8_t)(10U + status);
        result->total_ms = HAL_GetTick() - started_ms;
        return result->status;
    }

    phase_ms = HAL_GetTick();
    openmv_rgb_to_gray();
    openmv_equalize_gray();
    stats->grayscale_ms = HAL_GetTick() - phase_ms;

    phase_ms = HAL_GetTick();
    candidates = openmv_detect_face(&face);
    stats->haar_ms = HAL_GetTick() - phase_ms;
    stats->candidates = candidates;
    result->detection_ms = stats->decode_ms + stats->grayscale_ms + stats->haar_ms;
    if (candidates == 0U)
    {
        result->status = 0U;
        result->reference_ready = s_reference_valid;
        result->total_ms = HAL_GetTick() - started_ms;
        return 0U;
    }

    result->detection_valid = 1U;
    result->score = 1.0f;
    result->x1 = face.x;
    result->y1 = face.y;
    result->x2 = (int16_t)(face.x + face.w);
    result->y2 = (int16_t)(face.y + face.h);

    phase_ms = HAL_GetTick();
    if (openmv_lbp_descriptor(&face, s_last_desc) != 0U)
    {
        result->status = 20U;
        result->total_ms = HAL_GetTick() - started_ms;
        return result->status;
    }
    stats->lbp_ms = HAL_GetTick() - phase_ms;
    result->identity_ms = stats->lbp_ms;
    result->embedding_valid = 1U;
    if (s_enrollment_remaining != 0U)
    {
        uint32_t i;
        for (i = 0U; i < OPENMV_FACE_LBP_DESC_SIZE; i++)
        {
            s_enrollment_sum[i] += s_last_desc[i];
        }
        s_enrollment_remaining--;
        if (s_enrollment_remaining == 0U)
        {
            for (i = 0U; i < OPENMV_FACE_LBP_DESC_SIZE; i++)
            {
                s_reference_desc[i] = s_enrollment_sum[i] / (float)OPENMV_FACE_ENROLL_FRAMES;
            }
            s_reference_id = s_enrollment_user_id;
            s_reference_valid = 1U;
            result->reference_ready = 1U;
            result->match_valid = 1U;
            result->matched_id = s_reference_id;
            result->similarity = 1.0f;
        }
    }
    else if (s_reference_valid != 0U)
    {
        distance = openmv_lbp_distance(s_reference_desc, s_last_desc);
        stats->distance = distance;
        result->embedding_norm = distance;
        result->similarity = (distance < OPENMV_FACE_MATCH_DISTANCE_MAX)
                               ? (1.0f - ((float)distance / (float)OPENMV_FACE_MATCH_DISTANCE_MAX))
                               : 0.0f;
        if (distance <= OPENMV_FACE_MATCH_DISTANCE_MAX)
        {
            result->match_valid = 1U;
            result->matched_id = s_reference_id;
        }
    }
    result->total_ms = HAL_GetTick() - started_ms;
    return 0U;
}

uint8_t OpenMVFace_BeginEnrollment(uint16_t user_id)
{
    if (user_id == 0U)
    {
        return 1U;
    }
    memset(s_enrollment_sum, 0, sizeof(s_enrollment_sum));
    s_enrollment_user_id = user_id;
    s_enrollment_remaining = OPENMV_FACE_ENROLL_FRAMES;
    s_reference_valid = 0U;
    return 0U;
}

void OpenMVFace_ClearReference(void)
{
    memset(s_reference_desc, 0, sizeof(s_reference_desc));
    s_reference_id = 0U;
    s_reference_valid = 0U;
    s_enrollment_remaining = 0U;
    s_enrollment_user_id = 0U;
}

uint8_t OpenMVFace_HasReference(void)
{
    return s_reference_valid;
}
