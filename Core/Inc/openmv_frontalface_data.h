#ifndef OPENMV_FRONTALFACE_DATA_H
#define OPENMV_FRONTALFACE_DATA_H

#include <stdint.h>

#define OPENMV_FRONTALFACE_FEATURE_COUNT   2913U
#define OPENMV_FRONTALFACE_RECTANGLE_COUNT 6383U

extern const int openmv_frontalface_window_w;
extern const int openmv_frontalface_window_h;
extern const int openmv_frontalface_n_stages;
extern const uint8_t openmv_frontalface_stages_array[];
extern const int16_t openmv_frontalface_stages_thresh_array[];
extern const int16_t openmv_frontalface_tree_thresh_array[];
extern const int16_t openmv_frontalface_alpha1_array[];
extern const int16_t openmv_frontalface_alpha2_array[];
extern const int8_t openmv_frontalface_num_rectangles_array[];
extern const int8_t openmv_frontalface_weights_array[];
extern const int8_t openmv_frontalface_rectangles_array[];

#endif /* OPENMV_FRONTALFACE_DATA_H */
