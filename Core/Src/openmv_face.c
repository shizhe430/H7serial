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
#include "quadspi.h"
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
#define OPENMV_FACE_MATCH_DISTANCE_MAX   0.12f
#define OPENMV_FACE_MATCH_DISTANCE_MARGIN 0.02f
#define OPENMV_FACE_LBP_HIST_SIZE        59U
#define OPENMV_FACE_LBP_REGIONS          8U
#define OPENMV_FACE_ENROLL_FRAMES        5U
#define OPENMV_FACE_ENROLL_INTERVAL_MS   1200U
#define OPENMV_FACE_DB_RAM_BASE          0xC1900000UL
#define OPENMV_FACE_DB_FLASH_OFFSET      0x01100000UL
#define OPENMV_FACE_DB_FLASH_SIZE        0x00080000UL
#define OPENMV_FACE_DB_MAGIC             0x3142504CUL
#define OPENMV_FACE_DB_VERSION           2U
#define OPENMV_FACE_DB_LEGACY_VERSION    1U
#define OPENMV_FACE_ENROLL_RAM_BASE      0xC1980000UL

typedef struct
{
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
} openmv_rect_t;

typedef struct __attribute__((packed))
{
    uint16_t user_id;
    uint8_t template_count;
    uint8_t reserved;
    float descriptor[OPENMV_FACE_TEMPLATES_PER_USER][OPENMV_FACE_LBP_DESC_SIZE];
} openmv_face_db_entry_t;

typedef struct __attribute__((packed))
{
    uint32_t magic;
    uint16_t version;
    uint16_t capacity;
    uint16_t count;
    uint16_t reserved;
    openmv_face_db_entry_t entries[OPENMV_FACE_MAX_USERS];
    uint32_t crc;
} openmv_face_db_t;

typedef struct __attribute__((packed))
{
    uint16_t user_id;
    uint16_t reserved;
    float descriptor[OPENMV_FACE_LBP_DESC_SIZE];
} openmv_face_legacy_entry_t;

typedef struct __attribute__((packed))
{
    uint32_t magic;
    uint16_t version;
    uint16_t capacity;
    uint16_t count;
    uint16_t reserved;
    openmv_face_legacy_entry_t entries[OPENMV_FACE_MAX_USERS];
    uint32_t crc;
} openmv_face_legacy_db_t;

_Static_assert(sizeof(openmv_face_db_t) <= OPENMV_FACE_DB_FLASH_SIZE,
               "OpenMV face database exceeds reserved QSPI range");

static uint8_t *const s_rgb = (uint8_t *)OPENMV_FACE_RGB_BASE;
static uint8_t *const s_gray = (uint8_t *)OPENMV_FACE_GRAY_BASE;
static uint8_t *const s_raw_gray = (uint8_t *)OPENMV_FACE_RAW_GRAY_BASE;
static uint32_t *const s_sum = (uint32_t *)OPENMV_FACE_SUM_BASE;
static uint32_t *const s_ssq = (uint32_t *)OPENMV_FACE_SSQ_BASE;
static uint8_t *const s_normalized = (uint8_t *)OPENMV_FACE_NORMALIZED_BASE;
static openmv_rect_t s_candidates[OPENMV_FACE_MAX_CANDIDATES];
static uint16_t s_candidate_parent[OPENMV_FACE_MAX_CANDIDATES];
static float s_last_desc[OPENMV_FACE_LBP_DESC_SIZE];
static openmv_face_db_t *const s_face_db = (openmv_face_db_t *)OPENMV_FACE_DB_RAM_BASE;
static float *const s_enrollment_templates = (float *)OPENMV_FACE_ENROLL_RAM_BASE;
static uint8_t s_db_initialized;
static uint8_t s_enrollment_remaining;
static int8_t s_enrollment_slot = -1;
static uint16_t s_enrollment_user_id;
static uint32_t s_enrollment_next_capture_ms;

static uint32_t openmv_face_crc32(const uint8_t *data, uint32_t size)
{
    uint32_t crc = 0xFFFFFFFFUL;
    uint32_t i;

    for (i = 0U; i < size; i++)
    {
        uint32_t bit;

        crc ^= data[i];
        for (bit = 0U; bit < 8U; bit++)
        {
            crc = ((crc & 1U) != 0U) ? ((crc >> 1U) ^ 0xEDB88320UL) : (crc >> 1U);
        }
    }
    return crc ^ 0xFFFFFFFFUL;
}

static uint8_t openmv_face_db_is_valid(const openmv_face_db_t *db)
{
    uint16_t valid_count = 0U;
    uint32_t i;

    if ((db == NULL) ||
        (db->magic != OPENMV_FACE_DB_MAGIC) ||
        (db->version != OPENMV_FACE_DB_VERSION) ||
        (db->capacity != OPENMV_FACE_MAX_USERS) ||
        (db->count > OPENMV_FACE_MAX_USERS) ||
        (openmv_face_crc32((const uint8_t *)db,
                           (uint32_t)offsetof(openmv_face_db_t, crc)) != db->crc))
    {
        return 0U;
    }
    for (i = 0U; i < OPENMV_FACE_MAX_USERS; i++)
    {
        uint32_t j;

        if (db->entries[i].user_id == 0U)
        {
            if (db->entries[i].template_count != 0U)
            {
                return 0U;
            }
            continue;
        }
        if ((db->entries[i].template_count == 0U) ||
            (db->entries[i].template_count > OPENMV_FACE_TEMPLATES_PER_USER))
        {
            return 0U;
        }
        valid_count++;
        for (j = i + 1U; j < OPENMV_FACE_MAX_USERS; j++)
        {
            if (db->entries[i].user_id == db->entries[j].user_id)
            {
                return 0U;
            }
        }
    }
    return (valid_count == db->count) ? 1U : 0U;
}

static uint8_t openmv_face_legacy_db_is_valid(const openmv_face_legacy_db_t *db)
{
    uint16_t valid_count = 0U;
    uint32_t i;

    if ((db == NULL) ||
        (db->magic != OPENMV_FACE_DB_MAGIC) ||
        (db->version != OPENMV_FACE_DB_LEGACY_VERSION) ||
        (db->capacity != OPENMV_FACE_MAX_USERS) ||
        (db->count > OPENMV_FACE_MAX_USERS) ||
        (openmv_face_crc32((const uint8_t *)db,
                           (uint32_t)offsetof(openmv_face_legacy_db_t, crc)) != db->crc))
    {
        return 0U;
    }
    for (i = 0U; i < OPENMV_FACE_MAX_USERS; i++)
    {
        uint32_t j;

        if (db->entries[i].user_id == 0U)
        {
            continue;
        }
        valid_count++;
        for (j = i + 1U; j < OPENMV_FACE_MAX_USERS; j++)
        {
            if (db->entries[i].user_id == db->entries[j].user_id)
            {
                return 0U;
            }
        }
    }
    return (valid_count == db->count) ? 1U : 0U;
}

static void openmv_face_db_prepare(void)
{
    uint32_t i;
    uint16_t count = 0U;

    s_face_db->magic = OPENMV_FACE_DB_MAGIC;
    s_face_db->version = OPENMV_FACE_DB_VERSION;
    s_face_db->capacity = OPENMV_FACE_MAX_USERS;
    s_face_db->reserved = 0U;
    for (i = 0U; i < OPENMV_FACE_MAX_USERS; i++)
    {
        s_face_db->entries[i].reserved = 0U;
        if (s_face_db->entries[i].user_id != 0U)
        {
            count++;
        }
        else
        {
            s_face_db->entries[i].template_count = 0U;
        }
    }
    s_face_db->count = count;
    s_face_db->crc = openmv_face_crc32((const uint8_t *)s_face_db,
                                       (uint32_t)offsetof(openmv_face_db_t, crc));
}

static void openmv_face_db_reset_ram(void)
{
    memset((void *)s_face_db, 0, sizeof(*s_face_db));
    openmv_face_db_prepare();
    SCB_CleanDCache_by_Addr((uint32_t *)s_face_db, (int32_t)sizeof(*s_face_db));
}

static int32_t openmv_face_db_find_user(uint16_t user_id)
{
    uint32_t i;

    for (i = 0U; i < OPENMV_FACE_MAX_USERS; i++)
    {
        if (s_face_db->entries[i].user_id == user_id)
        {
            return (int32_t)i;
        }
    }
    return -1;
}

static int32_t openmv_face_db_find_free(void)
{
    uint32_t i;

    for (i = 0U; i < OPENMV_FACE_MAX_USERS; i++)
    {
        if (s_face_db->entries[i].user_id == 0U)
        {
            return (int32_t)i;
        }
    }
    return -1;
}

static uint8_t openmv_face_db_save(void)
{
    const openmv_face_db_t *flash_db =
        (const openmv_face_db_t *)(0x90000000UL + OPENMV_FACE_DB_FLASH_OFFSET);
    uint8_t status = 0U;
    uint8_t map_status;

    openmv_face_db_prepare();
    SCB_CleanDCache_by_Addr((uint32_t *)s_face_db, (int32_t)sizeof(*s_face_db));
    if (MX_QUADSPI_DisableMemoryMapped() != 0U)
    {
        return 1U;
    }
    if (MX_QUADSPI_EraseRange(OPENMV_FACE_DB_FLASH_OFFSET,
                              OPENMV_FACE_DB_FLASH_SIZE) != 0U)
    {
        status = 2U;
    }
    if ((status == 0U) &&
        (MX_QUADSPI_ProgramRange(OPENMV_FACE_DB_FLASH_OFFSET,
                                 (const uint8_t *)s_face_db,
                                 (uint32_t)sizeof(*s_face_db)) != 0U))
    {
        status = 3U;
    }
    map_status = MX_QUADSPI_EnableMemoryMapped();
    if ((status == 0U) && (map_status != 0U))
    {
        status = 4U;
    }
    SCB_CleanInvalidateDCache();
    if ((status == 0U) &&
        ((openmv_face_db_is_valid(flash_db) == 0U) ||
         (memcmp(flash_db, (const void *)s_face_db, sizeof(*s_face_db)) != 0)))
    {
        status = 5U;
    }
    return status;
}

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

uint8_t OpenMVFace_InitDatabase(void)
{
    const openmv_face_db_t *flash_db =
        (const openmv_face_db_t *)(0x90000000UL + OPENMV_FACE_DB_FLASH_OFFSET);
    const openmv_face_legacy_db_t *legacy_db =
        (const openmv_face_legacy_db_t *)(0x90000000UL + OPENMV_FACE_DB_FLASH_OFFSET);

    SCB_CleanInvalidateDCache();
    if (openmv_face_db_is_valid(flash_db) != 0U)
    {
        memcpy((void *)s_face_db, flash_db, sizeof(*s_face_db));
        SCB_CleanDCache_by_Addr((uint32_t *)s_face_db, (int32_t)sizeof(*s_face_db));
        s_db_initialized = 1U;
        return 0U;
    }
    if (openmv_face_legacy_db_is_valid(legacy_db) != 0U)
    {
        uint32_t i;

        openmv_face_db_reset_ram();
        for (i = 0U; i < OPENMV_FACE_MAX_USERS; i++)
        {
            if (legacy_db->entries[i].user_id != 0U)
            {
                s_face_db->entries[i].user_id = legacy_db->entries[i].user_id;
                s_face_db->entries[i].template_count = 1U;
                memcpy(s_face_db->entries[i].descriptor[0],
                       legacy_db->entries[i].descriptor,
                       sizeof(legacy_db->entries[i].descriptor));
            }
        }
        s_db_initialized = 1U;
        return (openmv_face_db_save() == 0U) ? 0U : 2U;
    }
    else
    {
        openmv_face_db_reset_ram();
        s_db_initialized = 1U;
        return 1U;
    }
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
    if (s_db_initialized == 0U)
    {
        (void)OpenMVFace_InitDatabase();
    }
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
        result->reference_ready = (s_face_db->count != 0U) ? 1U : 0U;
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
        if ((int32_t)(HAL_GetTick() - s_enrollment_next_capture_ms) >= 0)
        {
            uint32_t template_index = OPENMV_FACE_ENROLL_FRAMES - s_enrollment_remaining;

            memcpy(&s_enrollment_templates[template_index * OPENMV_FACE_LBP_DESC_SIZE],
                   s_last_desc,
                   sizeof(s_last_desc));
            s_enrollment_remaining--;
            s_enrollment_next_capture_ms = HAL_GetTick() + OPENMV_FACE_ENROLL_INTERVAL_MS;
        }
        if (s_enrollment_remaining == 0U)
        {
            int32_t slot = s_enrollment_slot;

            if ((slot < 0) || (slot >= (int32_t)OPENMV_FACE_MAX_USERS))
            {
                result->status = 30U;
                result->reference_ready = (s_face_db->count != 0U) ? 1U : 0U;
                result->total_ms = HAL_GetTick() - started_ms;
                return result->status;
            }
            s_face_db->entries[slot].user_id = s_enrollment_user_id;
            s_face_db->entries[slot].template_count = OPENMV_FACE_TEMPLATES_PER_USER;
            memcpy(s_face_db->entries[slot].descriptor,
                   s_enrollment_templates,
                   sizeof(s_face_db->entries[slot].descriptor));
            if (openmv_face_db_save() == 0U)
            {
                result->reference_ready = 1U;
                result->match_valid = 1U;
                result->matched_id = s_enrollment_user_id;
                result->similarity = 1.0f;
            }
            else
            {
                result->status = 31U;
            }
        }
    }
    else if (s_face_db->count != 0U)
    {
        uint32_t i;
        float best_distance = 1.0e30f;
        float second_distance = 1.0e30f;
        uint16_t best_user_id = 0U;

        for (i = 0U; i < OPENMV_FACE_MAX_USERS; i++)
        {
            if (s_face_db->entries[i].user_id != 0U)
            {
                uint32_t template_index;
                float user_distance = 1.0e30f;

                for (template_index = 0U;
                     template_index < s_face_db->entries[i].template_count;
                     template_index++)
                {
                    float candidate_distance =
                        openmv_lbp_distance(s_face_db->entries[i].descriptor[template_index],
                                            s_last_desc);

                    if (candidate_distance < user_distance)
                    {
                        user_distance = candidate_distance;
                    }
                }
                if (user_distance < best_distance)
                {
                    second_distance = best_distance;
                    best_distance = user_distance;
                    best_user_id = s_face_db->entries[i].user_id;
                }
                else if (user_distance < second_distance)
                {
                    second_distance = user_distance;
                }
            }
        }
        distance = best_distance;
        stats->distance = distance;
        result->embedding_norm = distance;
        result->similarity = (distance < OPENMV_FACE_MATCH_DISTANCE_MAX)
                               ? (1.0f - ((float)distance / (float)OPENMV_FACE_MATCH_DISTANCE_MAX))
                               : 0.0f;
        if ((distance <= OPENMV_FACE_MATCH_DISTANCE_MAX) &&
            ((second_distance == 1.0e30f) ||
             ((second_distance - distance) >= OPENMV_FACE_MATCH_DISTANCE_MARGIN)))
        {
            result->match_valid = 1U;
            result->matched_id = best_user_id;
        }
    }
    result->reference_ready = (s_face_db->count != 0U) ? 1U : 0U;
    result->total_ms = HAL_GetTick() - started_ms;
    return result->status;
}

uint8_t OpenMVFace_BeginEnrollment(uint16_t user_id)
{
    int32_t slot;

    if (user_id == 0U)
    {
        return 1U;
    }
    if (s_db_initialized == 0U)
    {
        (void)OpenMVFace_InitDatabase();
    }
    slot = openmv_face_db_find_user(user_id);
    if (slot < 0)
    {
        slot = openmv_face_db_find_free();
    }
    if (slot < 0)
    {
        return 2U;
    }
    memset(s_enrollment_templates, 0,
           OPENMV_FACE_TEMPLATES_PER_USER * OPENMV_FACE_LBP_DESC_SIZE * sizeof(float));
    s_enrollment_slot = (int8_t)slot;
    s_enrollment_user_id = user_id;
    s_enrollment_remaining = OPENMV_FACE_ENROLL_FRAMES;
    s_enrollment_next_capture_ms = HAL_GetTick() + OPENMV_FACE_ENROLL_INTERVAL_MS;
    return 0U;
}

void OpenMVFace_CancelEnrollment(void)
{
    s_enrollment_remaining = 0U;
    s_enrollment_slot = -1;
    s_enrollment_user_id = 0U;
    s_enrollment_next_capture_ms = 0U;
}

uint8_t OpenMVFace_EnrollmentActive(void)
{
    return (s_enrollment_remaining != 0U) ? 1U : 0U;
}

uint8_t OpenMVFace_GetEnrollmentRemaining(void)
{
    return s_enrollment_remaining;
}

const uint8_t *OpenMVFace_GetLastRgbFrame(void)
{
    return s_rgb;
}

uint8_t OpenMVFace_DeleteUser(uint16_t user_id)
{
    int32_t slot;
    uint8_t status;

    if (s_db_initialized == 0U)
    {
        (void)OpenMVFace_InitDatabase();
    }
    slot = openmv_face_db_find_user(user_id);
    if (slot < 0)
    {
        return 1U;
    }
    memset(&s_face_db->entries[slot], 0, sizeof(s_face_db->entries[slot]));
    status = openmv_face_db_save();
    return (status == 0U) ? 0U : (uint8_t)(10U + status);
}

uint8_t OpenMVFace_ClearDatabase(void)
{
    uint8_t status;

    openmv_face_db_reset_ram();
    s_db_initialized = 1U;
    s_enrollment_remaining = 0U;
    s_enrollment_slot = -1;
    s_enrollment_user_id = 0U;
    s_enrollment_next_capture_ms = 0U;
    status = openmv_face_db_save();
    return (status == 0U) ? 0U : (uint8_t)(10U + status);
}

uint8_t OpenMVFace_GetUserCount(void)
{
    return (s_db_initialized != 0U) ? (uint8_t)s_face_db->count : 0U;
}

uint16_t OpenMVFace_GetUserId(uint8_t slot)
{
    if ((s_db_initialized == 0U) || (slot >= OPENMV_FACE_MAX_USERS))
    {
        return 0U;
    }
    return s_face_db->entries[slot].user_id;
}

void OpenMVFace_ClearReference(void)
{
    (void)OpenMVFace_ClearDatabase();
}

uint8_t OpenMVFace_HasReference(void)
{
    return (OpenMVFace_GetUserCount() != 0U) ? 1U : 0U;
}
