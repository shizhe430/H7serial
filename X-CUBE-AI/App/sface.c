/**
  ******************************************************************************
  * @file    sface.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-08-02T18:20:41+0800
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */


#include "sface.h"
#include "sface_data.h"

#include "ai_platform.h"
#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"



#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_sface
 
#undef AI_SFACE_MODEL_SIGNATURE
#define AI_SFACE_MODEL_SIGNATURE     "0x83938fe16d629ff1dbfdccff1004bf0f"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "2026-08-02T18:20:41+0800"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_SFACE_N_BATCHES
#define AI_SFACE_N_BATCHES         (1)

static ai_ptr g_sface_activations_map[1] = AI_C_ARRAY_INIT;
static ai_ptr g_sface_weights_map[1] = AI_C_ARRAY_INIT;



/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  data_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 37632, AI_STATIC)

/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  data_Transpose_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 37632, AI_STATIC)

/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  _minusscalar0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 37632, AI_STATIC)

/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  _minusscalar0_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 37632, AI_STATIC)

/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  _mulscalar0_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 37633, AI_STATIC)

/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  conv_1_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 401408, AI_STATIC)

/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  conv_1_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  conv_1_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  conv_1_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  conv_1_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 401408, AI_STATIC)

/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  conv_2_dw_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 401408, AI_STATIC)

/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  conv_2_dw_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  conv_2_dw_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  conv_2_dw_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  conv_2_dw_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 401408, AI_STATIC)

/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  conv_2_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 802816, AI_STATIC)

/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  conv_2_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 802816, AI_STATIC)

/* Array#17 */
AI_ARRAY_OBJ_DECLARE(
  conv_2_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 802816, AI_STATIC)

/* Array#18 */
AI_ARRAY_OBJ_DECLARE(
  conv_2_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 802816, AI_STATIC)

/* Array#19 */
AI_ARRAY_OBJ_DECLARE(
  conv_2_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 802816, AI_STATIC)

/* Array#20 */
AI_ARRAY_OBJ_DECLARE(
  conv_3_dw_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 200704, AI_STATIC)

/* Array#21 */
AI_ARRAY_OBJ_DECLARE(
  conv_3_dw_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#22 */
AI_ARRAY_OBJ_DECLARE(
  conv_3_dw_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#23 */
AI_ARRAY_OBJ_DECLARE(
  conv_3_dw_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#24 */
AI_ARRAY_OBJ_DECLARE(
  conv_3_dw_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 200704, AI_STATIC)

/* Array#25 */
AI_ARRAY_OBJ_DECLARE(
  conv_3_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 401408, AI_STATIC)

/* Array#26 */
AI_ARRAY_OBJ_DECLARE(
  conv_3_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#27 */
AI_ARRAY_OBJ_DECLARE(
  conv_3_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#28 */
AI_ARRAY_OBJ_DECLARE(
  conv_3_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#29 */
AI_ARRAY_OBJ_DECLARE(
  conv_3_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 401408, AI_STATIC)

/* Array#30 */
AI_ARRAY_OBJ_DECLARE(
  conv_4_dw_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 401408, AI_STATIC)

/* Array#31 */
AI_ARRAY_OBJ_DECLARE(
  conv_4_dw_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#32 */
AI_ARRAY_OBJ_DECLARE(
  conv_4_dw_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#33 */
AI_ARRAY_OBJ_DECLARE(
  conv_4_dw_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#34 */
AI_ARRAY_OBJ_DECLARE(
  conv_4_dw_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 401408, AI_STATIC)

/* Array#35 */
AI_ARRAY_OBJ_DECLARE(
  conv_4_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 401408, AI_STATIC)

/* Array#36 */
AI_ARRAY_OBJ_DECLARE(
  conv_4_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#37 */
AI_ARRAY_OBJ_DECLARE(
  conv_4_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#38 */
AI_ARRAY_OBJ_DECLARE(
  conv_4_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#39 */
AI_ARRAY_OBJ_DECLARE(
  conv_4_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 401408, AI_STATIC)

/* Array#40 */
AI_ARRAY_OBJ_DECLARE(
  conv_5_dw_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#41 */
AI_ARRAY_OBJ_DECLARE(
  conv_5_dw_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#42 */
AI_ARRAY_OBJ_DECLARE(
  conv_5_dw_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#43 */
AI_ARRAY_OBJ_DECLARE(
  conv_5_dw_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#44 */
AI_ARRAY_OBJ_DECLARE(
  conv_5_dw_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#45 */
AI_ARRAY_OBJ_DECLARE(
  conv_5_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 200704, AI_STATIC)

/* Array#46 */
AI_ARRAY_OBJ_DECLARE(
  conv_5_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#47 */
AI_ARRAY_OBJ_DECLARE(
  conv_5_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#48 */
AI_ARRAY_OBJ_DECLARE(
  conv_5_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#49 */
AI_ARRAY_OBJ_DECLARE(
  conv_5_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 200704, AI_STATIC)

/* Array#50 */
AI_ARRAY_OBJ_DECLARE(
  conv_6_dw_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 200704, AI_STATIC)

/* Array#51 */
AI_ARRAY_OBJ_DECLARE(
  conv_6_dw_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#52 */
AI_ARRAY_OBJ_DECLARE(
  conv_6_dw_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#53 */
AI_ARRAY_OBJ_DECLARE(
  conv_6_dw_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#54 */
AI_ARRAY_OBJ_DECLARE(
  conv_6_dw_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 200704, AI_STATIC)

/* Array#55 */
AI_ARRAY_OBJ_DECLARE(
  conv_6_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 200704, AI_STATIC)

/* Array#56 */
AI_ARRAY_OBJ_DECLARE(
  conv_6_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#57 */
AI_ARRAY_OBJ_DECLARE(
  conv_6_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#58 */
AI_ARRAY_OBJ_DECLARE(
  conv_6_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#59 */
AI_ARRAY_OBJ_DECLARE(
  conv_6_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 200704, AI_STATIC)

/* Array#60 */
AI_ARRAY_OBJ_DECLARE(
  conv_7_dw_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 50176, AI_STATIC)

/* Array#61 */
AI_ARRAY_OBJ_DECLARE(
  conv_7_dw_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#62 */
AI_ARRAY_OBJ_DECLARE(
  conv_7_dw_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#63 */
AI_ARRAY_OBJ_DECLARE(
  conv_7_dw_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#64 */
AI_ARRAY_OBJ_DECLARE(
  conv_7_dw_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 50176, AI_STATIC)

/* Array#65 */
AI_ARRAY_OBJ_DECLARE(
  conv_7_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#66 */
AI_ARRAY_OBJ_DECLARE(
  conv_7_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#67 */
AI_ARRAY_OBJ_DECLARE(
  conv_7_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#68 */
AI_ARRAY_OBJ_DECLARE(
  conv_7_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#69 */
AI_ARRAY_OBJ_DECLARE(
  conv_7_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#70 */
AI_ARRAY_OBJ_DECLARE(
  conv_8_dw_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#71 */
AI_ARRAY_OBJ_DECLARE(
  conv_8_dw_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#72 */
AI_ARRAY_OBJ_DECLARE(
  conv_8_dw_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#73 */
AI_ARRAY_OBJ_DECLARE(
  conv_8_dw_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#74 */
AI_ARRAY_OBJ_DECLARE(
  conv_8_dw_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#75 */
AI_ARRAY_OBJ_DECLARE(
  conv_8_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#76 */
AI_ARRAY_OBJ_DECLARE(
  conv_8_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#77 */
AI_ARRAY_OBJ_DECLARE(
  conv_8_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#78 */
AI_ARRAY_OBJ_DECLARE(
  conv_8_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#79 */
AI_ARRAY_OBJ_DECLARE(
  conv_8_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#80 */
AI_ARRAY_OBJ_DECLARE(
  conv_9_dw_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#81 */
AI_ARRAY_OBJ_DECLARE(
  conv_9_dw_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#82 */
AI_ARRAY_OBJ_DECLARE(
  conv_9_dw_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#83 */
AI_ARRAY_OBJ_DECLARE(
  conv_9_dw_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#84 */
AI_ARRAY_OBJ_DECLARE(
  conv_9_dw_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#85 */
AI_ARRAY_OBJ_DECLARE(
  conv_9_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#86 */
AI_ARRAY_OBJ_DECLARE(
  conv_9_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#87 */
AI_ARRAY_OBJ_DECLARE(
  conv_9_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#88 */
AI_ARRAY_OBJ_DECLARE(
  conv_9_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#89 */
AI_ARRAY_OBJ_DECLARE(
  conv_9_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#90 */
AI_ARRAY_OBJ_DECLARE(
  conv_10_dw_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#91 */
AI_ARRAY_OBJ_DECLARE(
  conv_10_dw_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#92 */
AI_ARRAY_OBJ_DECLARE(
  conv_10_dw_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#93 */
AI_ARRAY_OBJ_DECLARE(
  conv_10_dw_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#94 */
AI_ARRAY_OBJ_DECLARE(
  conv_10_dw_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#95 */
AI_ARRAY_OBJ_DECLARE(
  conv_10_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#96 */
AI_ARRAY_OBJ_DECLARE(
  conv_10_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#97 */
AI_ARRAY_OBJ_DECLARE(
  conv_10_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#98 */
AI_ARRAY_OBJ_DECLARE(
  conv_10_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#99 */
AI_ARRAY_OBJ_DECLARE(
  conv_10_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#100 */
AI_ARRAY_OBJ_DECLARE(
  conv_11_dw_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#101 */
AI_ARRAY_OBJ_DECLARE(
  conv_11_dw_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#102 */
AI_ARRAY_OBJ_DECLARE(
  conv_11_dw_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#103 */
AI_ARRAY_OBJ_DECLARE(
  conv_11_dw_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#104 */
AI_ARRAY_OBJ_DECLARE(
  conv_11_dw_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#105 */
AI_ARRAY_OBJ_DECLARE(
  conv_11_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#106 */
AI_ARRAY_OBJ_DECLARE(
  conv_11_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#107 */
AI_ARRAY_OBJ_DECLARE(
  conv_11_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#108 */
AI_ARRAY_OBJ_DECLARE(
  conv_11_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#109 */
AI_ARRAY_OBJ_DECLARE(
  conv_11_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#110 */
AI_ARRAY_OBJ_DECLARE(
  conv_12_dw_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#111 */
AI_ARRAY_OBJ_DECLARE(
  conv_12_dw_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#112 */
AI_ARRAY_OBJ_DECLARE(
  conv_12_dw_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#113 */
AI_ARRAY_OBJ_DECLARE(
  conv_12_dw_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#114 */
AI_ARRAY_OBJ_DECLARE(
  conv_12_dw_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#115 */
AI_ARRAY_OBJ_DECLARE(
  conv_12_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#116 */
AI_ARRAY_OBJ_DECLARE(
  conv_12_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#117 */
AI_ARRAY_OBJ_DECLARE(
  conv_12_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#118 */
AI_ARRAY_OBJ_DECLARE(
  conv_12_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#119 */
AI_ARRAY_OBJ_DECLARE(
  conv_12_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#120 */
AI_ARRAY_OBJ_DECLARE(
  conv_13_dw_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 25088, AI_STATIC)

/* Array#121 */
AI_ARRAY_OBJ_DECLARE(
  conv_13_dw_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#122 */
AI_ARRAY_OBJ_DECLARE(
  conv_13_dw_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#123 */
AI_ARRAY_OBJ_DECLARE(
  conv_13_dw_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#124 */
AI_ARRAY_OBJ_DECLARE(
  conv_13_dw_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 25088, AI_STATIC)

/* Array#125 */
AI_ARRAY_OBJ_DECLARE(
  conv_13_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 50176, AI_STATIC)

/* Array#126 */
AI_ARRAY_OBJ_DECLARE(
  conv_13_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#127 */
AI_ARRAY_OBJ_DECLARE(
  conv_13_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#128 */
AI_ARRAY_OBJ_DECLARE(
  conv_13_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#129 */
AI_ARRAY_OBJ_DECLARE(
  conv_13_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 50176, AI_STATIC)

/* Array#130 */
AI_ARRAY_OBJ_DECLARE(
  conv_14_dw_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 50176, AI_STATIC)

/* Array#131 */
AI_ARRAY_OBJ_DECLARE(
  conv_14_dw_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#132 */
AI_ARRAY_OBJ_DECLARE(
  conv_14_dw_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#133 */
AI_ARRAY_OBJ_DECLARE(
  conv_14_dw_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#134 */
AI_ARRAY_OBJ_DECLARE(
  conv_14_dw_relu_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 50176, AI_STATIC)

/* Array#135 */
AI_ARRAY_OBJ_DECLARE(
  conv_14_conv2d_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 50176, AI_STATIC)

/* Array#136 */
AI_ARRAY_OBJ_DECLARE(
  conv_14_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#137 */
AI_ARRAY_OBJ_DECLARE(
  conv_14_batchnorm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#138 */
AI_ARRAY_OBJ_DECLARE(
  conv_14_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#139 */
AI_ARRAY_OBJ_DECLARE(
  bn1_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#140 */
AI_ARRAY_OBJ_DECLARE(
  bn1_0_conversion_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 50176, AI_STATIC)

/* Array#141 */
AI_ARRAY_OBJ_DECLARE(
  flatten_254_to_chlast_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 50176, AI_STATIC)

/* Array#142 */
AI_ARRAY_OBJ_DECLARE(
  pre_fc1_MatMul_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 128, AI_STATIC)

/* Array#143 */
AI_ARRAY_OBJ_DECLARE(
  pre_fc1_quantized_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 128, AI_STATIC)

/* Array#144 */
AI_ARRAY_OBJ_DECLARE(
  pre_fc1_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#145 */
AI_ARRAY_OBJ_DECLARE(
  fc1_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 128, AI_STATIC)

/* Array#146 */
AI_ARRAY_OBJ_DECLARE(
  scalar_op1_4D_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#147 */
AI_ARRAY_OBJ_DECLARE(
  scalar_op2_quantized_4D_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1, AI_STATIC)

/* Array#148 */
AI_ARRAY_OBJ_DECLARE(
  conv_1_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 864, AI_STATIC)

/* Array#149 */
AI_ARRAY_OBJ_DECLARE(
  conv_1_conv2d_quantized_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 32, AI_STATIC)

/* Array#150 */
AI_ARRAY_OBJ_DECLARE(
  conv_1_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 32, AI_STATIC)

/* Array#151 */
AI_ARRAY_OBJ_DECLARE(
  conv_1_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 32, AI_STATIC)

/* Array#152 */
AI_ARRAY_OBJ_DECLARE(
  conv_1_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 33, AI_STATIC)

/* Array#153 */
AI_ARRAY_OBJ_DECLARE(
  conv_2_dw_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 288, AI_STATIC)

/* Array#154 */
AI_ARRAY_OBJ_DECLARE(
  conv_2_dw_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 32, AI_STATIC)

/* Array#155 */
AI_ARRAY_OBJ_DECLARE(
  conv_2_dw_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 32, AI_STATIC)

/* Array#156 */
AI_ARRAY_OBJ_DECLARE(
  conv_2_dw_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 33, AI_STATIC)

/* Array#157 */
AI_ARRAY_OBJ_DECLARE(
  conv_2_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 2048, AI_STATIC)

/* Array#158 */
AI_ARRAY_OBJ_DECLARE(
  conv_2_conv2d_quantized_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 64, AI_STATIC)

/* Array#159 */
AI_ARRAY_OBJ_DECLARE(
  conv_2_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#160 */
AI_ARRAY_OBJ_DECLARE(
  conv_2_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#161 */
AI_ARRAY_OBJ_DECLARE(
  conv_2_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 65, AI_STATIC)

/* Array#162 */
AI_ARRAY_OBJ_DECLARE(
  conv_3_dw_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 576, AI_STATIC)

/* Array#163 */
AI_ARRAY_OBJ_DECLARE(
  conv_3_dw_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#164 */
AI_ARRAY_OBJ_DECLARE(
  conv_3_dw_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#165 */
AI_ARRAY_OBJ_DECLARE(
  conv_3_dw_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 65, AI_STATIC)

/* Array#166 */
AI_ARRAY_OBJ_DECLARE(
  conv_3_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 8192, AI_STATIC)

/* Array#167 */
AI_ARRAY_OBJ_DECLARE(
  conv_3_conv2d_quantized_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 128, AI_STATIC)

/* Array#168 */
AI_ARRAY_OBJ_DECLARE(
  conv_3_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#169 */
AI_ARRAY_OBJ_DECLARE(
  conv_3_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#170 */
AI_ARRAY_OBJ_DECLARE(
  conv_3_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 129, AI_STATIC)

/* Array#171 */
AI_ARRAY_OBJ_DECLARE(
  conv_4_dw_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1152, AI_STATIC)

/* Array#172 */
AI_ARRAY_OBJ_DECLARE(
  conv_4_dw_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#173 */
AI_ARRAY_OBJ_DECLARE(
  conv_4_dw_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#174 */
AI_ARRAY_OBJ_DECLARE(
  conv_4_dw_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 129, AI_STATIC)

/* Array#175 */
AI_ARRAY_OBJ_DECLARE(
  conv_4_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 16384, AI_STATIC)

/* Array#176 */
AI_ARRAY_OBJ_DECLARE(
  conv_4_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#177 */
AI_ARRAY_OBJ_DECLARE(
  conv_4_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#178 */
AI_ARRAY_OBJ_DECLARE(
  conv_4_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 129, AI_STATIC)

/* Array#179 */
AI_ARRAY_OBJ_DECLARE(
  conv_5_dw_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1152, AI_STATIC)

/* Array#180 */
AI_ARRAY_OBJ_DECLARE(
  conv_5_dw_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#181 */
AI_ARRAY_OBJ_DECLARE(
  conv_5_dw_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#182 */
AI_ARRAY_OBJ_DECLARE(
  conv_5_dw_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 129, AI_STATIC)

/* Array#183 */
AI_ARRAY_OBJ_DECLARE(
  conv_5_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 32768, AI_STATIC)

/* Array#184 */
AI_ARRAY_OBJ_DECLARE(
  conv_5_conv2d_quantized_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 256, AI_STATIC)

/* Array#185 */
AI_ARRAY_OBJ_DECLARE(
  conv_5_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#186 */
AI_ARRAY_OBJ_DECLARE(
  conv_5_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#187 */
AI_ARRAY_OBJ_DECLARE(
  conv_5_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 257, AI_STATIC)

/* Array#188 */
AI_ARRAY_OBJ_DECLARE(
  conv_6_dw_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 2304, AI_STATIC)

/* Array#189 */
AI_ARRAY_OBJ_DECLARE(
  conv_6_dw_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#190 */
AI_ARRAY_OBJ_DECLARE(
  conv_6_dw_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#191 */
AI_ARRAY_OBJ_DECLARE(
  conv_6_dw_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 257, AI_STATIC)

/* Array#192 */
AI_ARRAY_OBJ_DECLARE(
  conv_6_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 65536, AI_STATIC)

/* Array#193 */
AI_ARRAY_OBJ_DECLARE(
  conv_6_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#194 */
AI_ARRAY_OBJ_DECLARE(
  conv_6_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#195 */
AI_ARRAY_OBJ_DECLARE(
  conv_6_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 257, AI_STATIC)

/* Array#196 */
AI_ARRAY_OBJ_DECLARE(
  conv_7_dw_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 2304, AI_STATIC)

/* Array#197 */
AI_ARRAY_OBJ_DECLARE(
  conv_7_dw_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#198 */
AI_ARRAY_OBJ_DECLARE(
  conv_7_dw_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#199 */
AI_ARRAY_OBJ_DECLARE(
  conv_7_dw_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 257, AI_STATIC)

/* Array#200 */
AI_ARRAY_OBJ_DECLARE(
  conv_7_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 131072, AI_STATIC)

/* Array#201 */
AI_ARRAY_OBJ_DECLARE(
  conv_7_conv2d_quantized_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 512, AI_STATIC)

/* Array#202 */
AI_ARRAY_OBJ_DECLARE(
  conv_7_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#203 */
AI_ARRAY_OBJ_DECLARE(
  conv_7_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#204 */
AI_ARRAY_OBJ_DECLARE(
  conv_7_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 513, AI_STATIC)

/* Array#205 */
AI_ARRAY_OBJ_DECLARE(
  conv_8_dw_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 4608, AI_STATIC)

/* Array#206 */
AI_ARRAY_OBJ_DECLARE(
  conv_8_dw_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#207 */
AI_ARRAY_OBJ_DECLARE(
  conv_8_dw_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#208 */
AI_ARRAY_OBJ_DECLARE(
  conv_8_dw_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 513, AI_STATIC)

/* Array#209 */
AI_ARRAY_OBJ_DECLARE(
  conv_8_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 262144, AI_STATIC)

/* Array#210 */
AI_ARRAY_OBJ_DECLARE(
  conv_8_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#211 */
AI_ARRAY_OBJ_DECLARE(
  conv_8_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#212 */
AI_ARRAY_OBJ_DECLARE(
  conv_8_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 513, AI_STATIC)

/* Array#213 */
AI_ARRAY_OBJ_DECLARE(
  conv_9_dw_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 4608, AI_STATIC)

/* Array#214 */
AI_ARRAY_OBJ_DECLARE(
  conv_9_dw_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#215 */
AI_ARRAY_OBJ_DECLARE(
  conv_9_dw_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#216 */
AI_ARRAY_OBJ_DECLARE(
  conv_9_dw_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 513, AI_STATIC)

/* Array#217 */
AI_ARRAY_OBJ_DECLARE(
  conv_9_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 262144, AI_STATIC)

/* Array#218 */
AI_ARRAY_OBJ_DECLARE(
  conv_9_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#219 */
AI_ARRAY_OBJ_DECLARE(
  conv_9_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#220 */
AI_ARRAY_OBJ_DECLARE(
  conv_9_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 513, AI_STATIC)

/* Array#221 */
AI_ARRAY_OBJ_DECLARE(
  conv_10_dw_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 4608, AI_STATIC)

/* Array#222 */
AI_ARRAY_OBJ_DECLARE(
  conv_10_dw_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#223 */
AI_ARRAY_OBJ_DECLARE(
  conv_10_dw_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#224 */
AI_ARRAY_OBJ_DECLARE(
  conv_10_dw_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 513, AI_STATIC)

/* Array#225 */
AI_ARRAY_OBJ_DECLARE(
  conv_10_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 262144, AI_STATIC)

/* Array#226 */
AI_ARRAY_OBJ_DECLARE(
  conv_10_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#227 */
AI_ARRAY_OBJ_DECLARE(
  conv_10_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#228 */
AI_ARRAY_OBJ_DECLARE(
  conv_10_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 513, AI_STATIC)

/* Array#229 */
AI_ARRAY_OBJ_DECLARE(
  conv_11_dw_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 4608, AI_STATIC)

/* Array#230 */
AI_ARRAY_OBJ_DECLARE(
  conv_11_dw_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#231 */
AI_ARRAY_OBJ_DECLARE(
  conv_11_dw_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#232 */
AI_ARRAY_OBJ_DECLARE(
  conv_11_dw_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 513, AI_STATIC)

/* Array#233 */
AI_ARRAY_OBJ_DECLARE(
  conv_11_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 262144, AI_STATIC)

/* Array#234 */
AI_ARRAY_OBJ_DECLARE(
  conv_11_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#235 */
AI_ARRAY_OBJ_DECLARE(
  conv_11_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#236 */
AI_ARRAY_OBJ_DECLARE(
  conv_11_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 513, AI_STATIC)

/* Array#237 */
AI_ARRAY_OBJ_DECLARE(
  conv_12_dw_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 4608, AI_STATIC)

/* Array#238 */
AI_ARRAY_OBJ_DECLARE(
  conv_12_dw_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#239 */
AI_ARRAY_OBJ_DECLARE(
  conv_12_dw_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#240 */
AI_ARRAY_OBJ_DECLARE(
  conv_12_dw_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 513, AI_STATIC)

/* Array#241 */
AI_ARRAY_OBJ_DECLARE(
  conv_12_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 262144, AI_STATIC)

/* Array#242 */
AI_ARRAY_OBJ_DECLARE(
  conv_12_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#243 */
AI_ARRAY_OBJ_DECLARE(
  conv_12_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#244 */
AI_ARRAY_OBJ_DECLARE(
  conv_12_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 513, AI_STATIC)

/* Array#245 */
AI_ARRAY_OBJ_DECLARE(
  conv_13_dw_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 4608, AI_STATIC)

/* Array#246 */
AI_ARRAY_OBJ_DECLARE(
  conv_13_dw_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#247 */
AI_ARRAY_OBJ_DECLARE(
  conv_13_dw_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#248 */
AI_ARRAY_OBJ_DECLARE(
  conv_13_dw_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 513, AI_STATIC)

/* Array#249 */
AI_ARRAY_OBJ_DECLARE(
  conv_13_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 524288, AI_STATIC)

/* Array#250 */
AI_ARRAY_OBJ_DECLARE(
  conv_13_conv2d_quantized_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 1024, AI_STATIC)

/* Array#251 */
AI_ARRAY_OBJ_DECLARE(
  conv_13_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1024, AI_STATIC)

/* Array#252 */
AI_ARRAY_OBJ_DECLARE(
  conv_13_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1024, AI_STATIC)

/* Array#253 */
AI_ARRAY_OBJ_DECLARE(
  conv_13_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1025, AI_STATIC)

/* Array#254 */
AI_ARRAY_OBJ_DECLARE(
  conv_14_dw_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 9216, AI_STATIC)

/* Array#255 */
AI_ARRAY_OBJ_DECLARE(
  conv_14_dw_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1024, AI_STATIC)

/* Array#256 */
AI_ARRAY_OBJ_DECLARE(
  conv_14_dw_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1024, AI_STATIC)

/* Array#257 */
AI_ARRAY_OBJ_DECLARE(
  conv_14_dw_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1025, AI_STATIC)

/* Array#258 */
AI_ARRAY_OBJ_DECLARE(
  conv_14_conv2d_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1048576, AI_STATIC)

/* Array#259 */
AI_ARRAY_OBJ_DECLARE(
  conv_14_batchnorm_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1024, AI_STATIC)

/* Array#260 */
AI_ARRAY_OBJ_DECLARE(
  conv_14_batchnorm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1024, AI_STATIC)

/* Array#261 */
AI_ARRAY_OBJ_DECLARE(
  conv_14_relu_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1025, AI_STATIC)

/* Array#262 */
AI_ARRAY_OBJ_DECLARE(
  bn1_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1024, AI_STATIC)

/* Array#263 */
AI_ARRAY_OBJ_DECLARE(
  bn1_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1024, AI_STATIC)

/* Array#264 */
AI_ARRAY_OBJ_DECLARE(
  pre_fc1_MatMul_quantized_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 6422528, AI_STATIC)

/* Array#265 */
AI_ARRAY_OBJ_DECLARE(
  pre_fc1_bias_quantized_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 128, AI_STATIC)

/* Array#266 */
AI_ARRAY_OBJ_DECLARE(
  fc1_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#267 */
AI_ARRAY_OBJ_DECLARE(
  fc1_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#268 */
AI_ARRAY_OBJ_DECLARE(
  conv_1_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1964, AI_STATIC)

/* Array#269 */
AI_ARRAY_OBJ_DECLARE(
  conv_2_dw_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 865, AI_STATIC)

/* Array#270 */
AI_ARRAY_OBJ_DECLARE(
  conv_2_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 128, AI_STATIC)

/* Array#271 */
AI_ARRAY_OBJ_DECLARE(
  conv_3_dw_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1729, AI_STATIC)

/* Array#272 */
AI_ARRAY_OBJ_DECLARE(
  conv_3_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 256, AI_STATIC)

/* Array#273 */
AI_ARRAY_OBJ_DECLARE(
  conv_4_dw_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 3457, AI_STATIC)

/* Array#274 */
AI_ARRAY_OBJ_DECLARE(
  conv_4_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 512, AI_STATIC)

/* Array#275 */
AI_ARRAY_OBJ_DECLARE(
  conv_5_dw_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 3457, AI_STATIC)

/* Array#276 */
AI_ARRAY_OBJ_DECLARE(
  conv_5_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 512, AI_STATIC)

/* Array#277 */
AI_ARRAY_OBJ_DECLARE(
  conv_6_dw_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 6913, AI_STATIC)

/* Array#278 */
AI_ARRAY_OBJ_DECLARE(
  conv_6_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1024, AI_STATIC)

/* Array#279 */
AI_ARRAY_OBJ_DECLARE(
  conv_7_dw_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 6913, AI_STATIC)

/* Array#280 */
AI_ARRAY_OBJ_DECLARE(
  conv_7_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1024, AI_STATIC)

/* Array#281 */
AI_ARRAY_OBJ_DECLARE(
  conv_8_dw_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 13825, AI_STATIC)

/* Array#282 */
AI_ARRAY_OBJ_DECLARE(
  conv_8_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 2048, AI_STATIC)

/* Array#283 */
AI_ARRAY_OBJ_DECLARE(
  conv_9_dw_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 13825, AI_STATIC)

/* Array#284 */
AI_ARRAY_OBJ_DECLARE(
  conv_9_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 2048, AI_STATIC)

/* Array#285 */
AI_ARRAY_OBJ_DECLARE(
  conv_10_dw_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 13825, AI_STATIC)

/* Array#286 */
AI_ARRAY_OBJ_DECLARE(
  conv_10_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 2048, AI_STATIC)

/* Array#287 */
AI_ARRAY_OBJ_DECLARE(
  conv_11_dw_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 13825, AI_STATIC)

/* Array#288 */
AI_ARRAY_OBJ_DECLARE(
  conv_11_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 2048, AI_STATIC)

/* Array#289 */
AI_ARRAY_OBJ_DECLARE(
  conv_12_dw_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 13825, AI_STATIC)

/* Array#290 */
AI_ARRAY_OBJ_DECLARE(
  conv_12_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 2048, AI_STATIC)

/* Array#291 */
AI_ARRAY_OBJ_DECLARE(
  conv_13_dw_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 13825, AI_STATIC)

/* Array#292 */
AI_ARRAY_OBJ_DECLARE(
  conv_13_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 2048, AI_STATIC)

/* Array#293 */
AI_ARRAY_OBJ_DECLARE(
  conv_14_dw_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 27649, AI_STATIC)

/* Array#294 */
AI_ARRAY_OBJ_DECLARE(
  conv_14_conv2d_quantized_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 4096, AI_STATIC)

/* Array#295 */
AI_ARRAY_OBJ_DECLARE(
  pre_fc1_MatMul_quantized_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 50176, AI_STATIC)

/**  Array metadata declarations section  *************************************/
/* Int quant #0 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(_minusscalar0_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(1.003937005996704f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #1 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(_mulscalar0_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00784325785934925f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #2 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(bn1_0_conversion_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0039725154638290405f),
    AI_PACK_INTQ_ZP(-9)))

/* Int quant #3 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_10_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0186925046145916f),
    AI_PACK_INTQ_ZP(-23)))

/* Int quant #4 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_10_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0033490790519863367f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #5 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_10_dw_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00824945792555809f),
    AI_PACK_INTQ_ZP(4)))

/* Int quant #6 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_10_dw_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0033710480201989412f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #7 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_10_dw_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.04229329526424408f),
    AI_PACK_INTQ_ZP(-52)))

/* Int quant #8 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_10_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.034810762852430344f),
    AI_PACK_INTQ_ZP(-71)))

/* Int quant #9 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_11_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.017962582409381866f),
    AI_PACK_INTQ_ZP(-23)))

/* Int quant #10 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_11_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.002803599927574396f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #11 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_11_dw_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.007093817926943302f),
    AI_PACK_INTQ_ZP(7)))

/* Int quant #12 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_11_dw_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.002647112123668194f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #13 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_11_dw_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0411190390586853f),
    AI_PACK_INTQ_ZP(-62)))

/* Int quant #14 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_11_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.02218402922153473f),
    AI_PACK_INTQ_ZP(-68)))

/* Int quant #15 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_12_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.016739172860980034f),
    AI_PACK_INTQ_ZP(17)))

/* Int quant #16 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_12_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0028038257732987404f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #17 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_12_dw_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.006591557990759611f),
    AI_PACK_INTQ_ZP(-9)))

/* Int quant #18 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_12_dw_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0021290385629981756f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #19 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_12_dw_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.029234007000923157f),
    AI_PACK_INTQ_ZP(-62)))

/* Int quant #20 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_12_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.022465964779257774f),
    AI_PACK_INTQ_ZP(-76)))

/* Int quant #21 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_13_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.013545985333621502f),
    AI_PACK_INTQ_ZP(12)))

/* Int quant #22 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_13_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.001637941924855113f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #23 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_13_dw_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.006243205163627863f),
    AI_PACK_INTQ_ZP(-36)))

/* Int quant #24 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_13_dw_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.000921063416171819f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #25 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_13_dw_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.03331552818417549f),
    AI_PACK_INTQ_ZP(-70)))

/* Int quant #26 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_13_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.02047414518892765f),
    AI_PACK_INTQ_ZP(-78)))

/* Int quant #27 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_14_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.010705579072237015f),
    AI_PACK_INTQ_ZP(19)))

/* Int quant #28 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_14_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0017876120982691646f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #29 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_14_dw_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.004396785981953144f),
    AI_PACK_INTQ_ZP(8)))

/* Int quant #30 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_14_dw_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0010845608776435256f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #31 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_14_dw_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.022525593638420105f),
    AI_PACK_INTQ_ZP(-100)))

/* Int quant #32 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_1_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.02183494158089161f),
    AI_PACK_INTQ_ZP(-3)))

/* Int quant #33 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_1_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.004639357794076204f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #34 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_1_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.07479344308376312f),
    AI_PACK_INTQ_ZP(-23)))

/* Int quant #35 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_2_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.044635627418756485f),
    AI_PACK_INTQ_ZP(14)))

/* Int quant #36 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_2_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.004649615380913019f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #37 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_2_dw_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.03990639001131058f),
    AI_PACK_INTQ_ZP(19)))

/* Int quant #38 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_2_dw_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.004254276864230633f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #39 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_2_dw_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.07871090620756149f),
    AI_PACK_INTQ_ZP(-44)))

/* Int quant #40 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_2_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.07884729653596878f),
    AI_PACK_INTQ_ZP(-65)))

/* Int quant #41 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_3_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.036884766072034836f),
    AI_PACK_INTQ_ZP(18)))

/* Int quant #42 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_3_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0032617535907775164f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #43 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_3_dw_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.035105712711811066f),
    AI_PACK_INTQ_ZP(-20)))

/* Int quant #44 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_3_dw_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00173721625469625f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #45 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_3_dw_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.10327931493520737f),
    AI_PACK_INTQ_ZP(-74)))

/* Int quant #46 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_3_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.05282672867178917f),
    AI_PACK_INTQ_ZP(-51)))

/* Int quant #47 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_4_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.04028554633259773f),
    AI_PACK_INTQ_ZP(36)))

/* Int quant #48 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_4_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.004595943260937929f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #49 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_4_dw_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.023005714640021324f),
    AI_PACK_INTQ_ZP(-11)))

/* Int quant #50 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_4_dw_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.004262926056981087f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #51 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_4_dw_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.06664860248565674f),
    AI_PACK_INTQ_ZP(-72)))

/* Int quant #52 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_4_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.05155051127076149f),
    AI_PACK_INTQ_ZP(-68)))

/* Int quant #53 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_5_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.024361886084079742f),
    AI_PACK_INTQ_ZP(17)))

/* Int quant #54 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_5_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0033875422086566687f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #55 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_5_dw_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.01813429966568947f),
    AI_PACK_INTQ_ZP(4)))

/* Int quant #56 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_5_dw_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0014852811582386494f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #57 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_5_dw_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0736120194196701f),
    AI_PACK_INTQ_ZP(-34)))

/* Int quant #58 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_5_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0357653982937336f),
    AI_PACK_INTQ_ZP(-46)))

/* Int quant #59 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_6_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.030957238748669624f),
    AI_PACK_INTQ_ZP(2)))

/* Int quant #60 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_6_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0034122976940125227f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #61 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_6_dw_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.014092927798628807f),
    AI_PACK_INTQ_ZP(15)))

/* Int quant #62 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_6_dw_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.004190919920802116f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #63 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_6_dw_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.053411345928907394f),
    AI_PACK_INTQ_ZP(-70)))

/* Int quant #64 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_6_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.03433064743876457f),
    AI_PACK_INTQ_ZP(-59)))

/* Int quant #65 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_7_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.024206124246120453f),
    AI_PACK_INTQ_ZP(28)))

/* Int quant #66 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_7_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0032277172431349754f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #67 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_7_dw_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.013474398292601109f),
    AI_PACK_INTQ_ZP(-15)))

/* Int quant #68 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_7_dw_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0016555381007492542f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #69 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_7_dw_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.04551009461283684f),
    AI_PACK_INTQ_ZP(-36)))

/* Int quant #70 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_7_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.031247543171048164f),
    AI_PACK_INTQ_ZP(-54)))

/* Int quant #71 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_8_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.03145582228899002f),
    AI_PACK_INTQ_ZP(20)))

/* Int quant #72 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_8_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0030797896906733513f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #73 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_8_dw_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.008076961152255535f),
    AI_PACK_INTQ_ZP(1)))

/* Int quant #74 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_8_dw_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0033894504886120558f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #75 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_8_dw_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.042725153267383575f),
    AI_PACK_INTQ_ZP(-51)))

/* Int quant #76 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_8_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.028475211933255196f),
    AI_PACK_INTQ_ZP(-49)))

/* Int quant #77 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_9_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.024298805743455887f),
    AI_PACK_INTQ_ZP(14)))

/* Int quant #78 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_9_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0036300483625382185f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #79 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_9_dw_conv2d_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.008191248401999474f),
    AI_PACK_INTQ_ZP(-25)))

/* Int quant #80 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_9_dw_conv2d_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0030717456247657537f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #81 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_9_dw_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.04104342684149742f),
    AI_PACK_INTQ_ZP(-49)))

/* Int quant #82 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv_9_relu_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.027967847883701324f),
    AI_PACK_INTQ_ZP(-44)))

/* Int quant #83 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(flatten_254_to_chlast_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0039725154638290405f),
    AI_PACK_INTQ_ZP(-9)))

/* Int quant #84 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(pre_fc1_MatMul_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.010133507661521435f),
    AI_PACK_INTQ_ZP(10)))

/* Int quant #85 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(pre_fc1_MatMul_quantized_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0005366023979149759f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #86 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(pre_fc1_bias_quantized_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(1.486837736264235e-10f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #87 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(pre_fc1_quantized_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.010133507661521435f),
    AI_PACK_INTQ_ZP(10)))

/* Int quant #88 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(scalar_op2_quantized_4D_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(6.151574780233204e-05f),
    AI_PACK_INTQ_ZP(0)))

/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  _minusscalar0_output, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 112, 112), AI_STRIDE_INIT(4, 4, 4, 12, 1344),
  1, &_minusscalar0_output_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  _minusscalar0_quantized_output, AI_STATIC,
  1, 0x1,
  AI_SHAPE_INIT(4, 1, 3, 112, 112), AI_STRIDE_INIT(4, 1, 1, 3, 336),
  1, &_minusscalar0_quantized_output_array, &_minusscalar0_quantized_output_array_intq)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  _mulscalar0_quantized_output, AI_STATIC,
  2, 0x1,
  AI_SHAPE_INIT(4, 1, 3, 112, 112), AI_STRIDE_INIT(4, 1, 1, 3, 336),
  1, &_mulscalar0_quantized_output_array, &_mulscalar0_quantized_output_array_intq)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  bn1_0_conversion_output, AI_STATIC,
  3, 0x1,
  AI_SHAPE_INIT(4, 1, 1024, 7, 7), AI_STRIDE_INIT(4, 1, 1, 1024, 7168),
  1, &bn1_0_conversion_output_array, &bn1_0_conversion_output_array_intq)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  bn1_bias, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4096, 4096),
  1, &bn1_bias_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  bn1_output, AI_STATIC,
  5, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 7, 7), AI_STRIDE_INIT(4, 4, 4, 4096, 28672),
  1, &bn1_output_array, NULL)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  bn1_scale, AI_STATIC,
  6, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4096, 4096),
  1, &bn1_scale_array, NULL)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  conv_10_batchnorm_bias, AI_STATIC,
  7, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_10_batchnorm_bias_array, NULL)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  conv_10_batchnorm_output, AI_STATIC,
  8, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_10_batchnorm_output_array, NULL)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  conv_10_batchnorm_scale, AI_STATIC,
  9, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_10_batchnorm_scale_array, NULL)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  conv_10_conv2d_output, AI_STATIC,
  10, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_10_conv2d_output_array, NULL)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  conv_10_conv2d_quantized_output, AI_STATIC,
  11, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 1, 1, 512, 7168),
  1, &conv_10_conv2d_quantized_output_array, &conv_10_conv2d_quantized_output_array_intq)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  conv_10_conv2d_quantized_scratch0, AI_STATIC,
  12, 0x0,
  AI_SHAPE_INIT(4, 1, 2048, 1, 1), AI_STRIDE_INIT(4, 1, 1, 2048, 2048),
  1, &conv_10_conv2d_quantized_scratch0_array, NULL)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  conv_10_conv2d_quantized_weights, AI_STATIC,
  13, 0x1,
  AI_SHAPE_INIT(4, 512, 1, 1, 512), AI_STRIDE_INIT(4, 1, 512, 262144, 262144),
  1, &conv_10_conv2d_quantized_weights_array, &conv_10_conv2d_quantized_weights_array_intq)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  conv_10_dw_batchnorm_bias, AI_STATIC,
  14, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_10_dw_batchnorm_bias_array, NULL)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  conv_10_dw_batchnorm_output, AI_STATIC,
  15, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_10_dw_batchnorm_output_array, NULL)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  conv_10_dw_batchnorm_scale, AI_STATIC,
  16, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_10_dw_batchnorm_scale_array, NULL)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  conv_10_dw_conv2d_output, AI_STATIC,
  17, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_10_dw_conv2d_output_array, NULL)

/* Tensor #18 */
AI_TENSOR_OBJ_DECLARE(
  conv_10_dw_conv2d_quantized_output, AI_STATIC,
  18, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 1, 1, 512, 7168),
  1, &conv_10_dw_conv2d_quantized_output_array, &conv_10_dw_conv2d_quantized_output_array_intq)

/* Tensor #19 */
AI_TENSOR_OBJ_DECLARE(
  conv_10_dw_conv2d_quantized_scratch0, AI_STATIC,
  19, 0x0,
  AI_SHAPE_INIT(4, 1, 13825, 1, 1), AI_STRIDE_INIT(4, 1, 1, 13825, 13825),
  1, &conv_10_dw_conv2d_quantized_scratch0_array, NULL)

/* Tensor #20 */
AI_TENSOR_OBJ_DECLARE(
  conv_10_dw_conv2d_quantized_weights, AI_STATIC,
  20, 0x1,
  AI_SHAPE_INIT(4, 512, 3, 3, 1), AI_STRIDE_INIT(4, 1, 512, 512, 1536),
  1, &conv_10_dw_conv2d_quantized_weights_array, &conv_10_dw_conv2d_quantized_weights_array_intq)

/* Tensor #21 */
AI_TENSOR_OBJ_DECLARE(
  conv_10_dw_relu_alpha, AI_STATIC,
  21, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_10_dw_relu_alpha_array, NULL)

/* Tensor #22 */
AI_TENSOR_OBJ_DECLARE(
  conv_10_dw_relu_output, AI_STATIC,
  22, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_10_dw_relu_output_array, NULL)

/* Tensor #23 */
AI_TENSOR_OBJ_DECLARE(
  conv_10_dw_relu_quantized_output, AI_STATIC,
  23, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 1, 1, 512, 7168),
  1, &conv_10_dw_relu_quantized_output_array, &conv_10_dw_relu_quantized_output_array_intq)

/* Tensor #24 */
AI_TENSOR_OBJ_DECLARE(
  conv_10_relu_alpha, AI_STATIC,
  24, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_10_relu_alpha_array, NULL)

/* Tensor #25 */
AI_TENSOR_OBJ_DECLARE(
  conv_10_relu_output, AI_STATIC,
  25, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_10_relu_output_array, NULL)

/* Tensor #26 */
AI_TENSOR_OBJ_DECLARE(
  conv_10_relu_quantized_output, AI_STATIC,
  26, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 1, 1, 512, 7168),
  1, &conv_10_relu_quantized_output_array, &conv_10_relu_quantized_output_array_intq)

/* Tensor #27 */
AI_TENSOR_OBJ_DECLARE(
  conv_11_batchnorm_bias, AI_STATIC,
  27, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_11_batchnorm_bias_array, NULL)

/* Tensor #28 */
AI_TENSOR_OBJ_DECLARE(
  conv_11_batchnorm_output, AI_STATIC,
  28, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_11_batchnorm_output_array, NULL)

/* Tensor #29 */
AI_TENSOR_OBJ_DECLARE(
  conv_11_batchnorm_scale, AI_STATIC,
  29, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_11_batchnorm_scale_array, NULL)

/* Tensor #30 */
AI_TENSOR_OBJ_DECLARE(
  conv_11_conv2d_output, AI_STATIC,
  30, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_11_conv2d_output_array, NULL)

/* Tensor #31 */
AI_TENSOR_OBJ_DECLARE(
  conv_11_conv2d_quantized_output, AI_STATIC,
  31, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 1, 1, 512, 7168),
  1, &conv_11_conv2d_quantized_output_array, &conv_11_conv2d_quantized_output_array_intq)

/* Tensor #32 */
AI_TENSOR_OBJ_DECLARE(
  conv_11_conv2d_quantized_scratch0, AI_STATIC,
  32, 0x0,
  AI_SHAPE_INIT(4, 1, 2048, 1, 1), AI_STRIDE_INIT(4, 1, 1, 2048, 2048),
  1, &conv_11_conv2d_quantized_scratch0_array, NULL)

/* Tensor #33 */
AI_TENSOR_OBJ_DECLARE(
  conv_11_conv2d_quantized_weights, AI_STATIC,
  33, 0x1,
  AI_SHAPE_INIT(4, 512, 1, 1, 512), AI_STRIDE_INIT(4, 1, 512, 262144, 262144),
  1, &conv_11_conv2d_quantized_weights_array, &conv_11_conv2d_quantized_weights_array_intq)

/* Tensor #34 */
AI_TENSOR_OBJ_DECLARE(
  conv_11_dw_batchnorm_bias, AI_STATIC,
  34, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_11_dw_batchnorm_bias_array, NULL)

/* Tensor #35 */
AI_TENSOR_OBJ_DECLARE(
  conv_11_dw_batchnorm_output, AI_STATIC,
  35, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_11_dw_batchnorm_output_array, NULL)

/* Tensor #36 */
AI_TENSOR_OBJ_DECLARE(
  conv_11_dw_batchnorm_scale, AI_STATIC,
  36, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_11_dw_batchnorm_scale_array, NULL)

/* Tensor #37 */
AI_TENSOR_OBJ_DECLARE(
  conv_11_dw_conv2d_output, AI_STATIC,
  37, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_11_dw_conv2d_output_array, NULL)

/* Tensor #38 */
AI_TENSOR_OBJ_DECLARE(
  conv_11_dw_conv2d_quantized_output, AI_STATIC,
  38, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 1, 1, 512, 7168),
  1, &conv_11_dw_conv2d_quantized_output_array, &conv_11_dw_conv2d_quantized_output_array_intq)

/* Tensor #39 */
AI_TENSOR_OBJ_DECLARE(
  conv_11_dw_conv2d_quantized_scratch0, AI_STATIC,
  39, 0x0,
  AI_SHAPE_INIT(4, 1, 13825, 1, 1), AI_STRIDE_INIT(4, 1, 1, 13825, 13825),
  1, &conv_11_dw_conv2d_quantized_scratch0_array, NULL)

/* Tensor #40 */
AI_TENSOR_OBJ_DECLARE(
  conv_11_dw_conv2d_quantized_weights, AI_STATIC,
  40, 0x1,
  AI_SHAPE_INIT(4, 512, 3, 3, 1), AI_STRIDE_INIT(4, 1, 512, 512, 1536),
  1, &conv_11_dw_conv2d_quantized_weights_array, &conv_11_dw_conv2d_quantized_weights_array_intq)

/* Tensor #41 */
AI_TENSOR_OBJ_DECLARE(
  conv_11_dw_relu_alpha, AI_STATIC,
  41, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_11_dw_relu_alpha_array, NULL)

/* Tensor #42 */
AI_TENSOR_OBJ_DECLARE(
  conv_11_dw_relu_output, AI_STATIC,
  42, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_11_dw_relu_output_array, NULL)

/* Tensor #43 */
AI_TENSOR_OBJ_DECLARE(
  conv_11_dw_relu_quantized_output, AI_STATIC,
  43, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 1, 1, 512, 7168),
  1, &conv_11_dw_relu_quantized_output_array, &conv_11_dw_relu_quantized_output_array_intq)

/* Tensor #44 */
AI_TENSOR_OBJ_DECLARE(
  conv_11_relu_alpha, AI_STATIC,
  44, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_11_relu_alpha_array, NULL)

/* Tensor #45 */
AI_TENSOR_OBJ_DECLARE(
  conv_11_relu_output, AI_STATIC,
  45, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_11_relu_output_array, NULL)

/* Tensor #46 */
AI_TENSOR_OBJ_DECLARE(
  conv_11_relu_quantized_output, AI_STATIC,
  46, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 1, 1, 512, 7168),
  1, &conv_11_relu_quantized_output_array, &conv_11_relu_quantized_output_array_intq)

/* Tensor #47 */
AI_TENSOR_OBJ_DECLARE(
  conv_12_batchnorm_bias, AI_STATIC,
  47, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_12_batchnorm_bias_array, NULL)

/* Tensor #48 */
AI_TENSOR_OBJ_DECLARE(
  conv_12_batchnorm_output, AI_STATIC,
  48, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_12_batchnorm_output_array, NULL)

/* Tensor #49 */
AI_TENSOR_OBJ_DECLARE(
  conv_12_batchnorm_scale, AI_STATIC,
  49, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_12_batchnorm_scale_array, NULL)

/* Tensor #50 */
AI_TENSOR_OBJ_DECLARE(
  conv_12_conv2d_output, AI_STATIC,
  50, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_12_conv2d_output_array, NULL)

/* Tensor #51 */
AI_TENSOR_OBJ_DECLARE(
  conv_12_conv2d_quantized_output, AI_STATIC,
  51, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 1, 1, 512, 7168),
  1, &conv_12_conv2d_quantized_output_array, &conv_12_conv2d_quantized_output_array_intq)

/* Tensor #52 */
AI_TENSOR_OBJ_DECLARE(
  conv_12_conv2d_quantized_scratch0, AI_STATIC,
  52, 0x0,
  AI_SHAPE_INIT(4, 1, 2048, 1, 1), AI_STRIDE_INIT(4, 1, 1, 2048, 2048),
  1, &conv_12_conv2d_quantized_scratch0_array, NULL)

/* Tensor #53 */
AI_TENSOR_OBJ_DECLARE(
  conv_12_conv2d_quantized_weights, AI_STATIC,
  53, 0x1,
  AI_SHAPE_INIT(4, 512, 1, 1, 512), AI_STRIDE_INIT(4, 1, 512, 262144, 262144),
  1, &conv_12_conv2d_quantized_weights_array, &conv_12_conv2d_quantized_weights_array_intq)

/* Tensor #54 */
AI_TENSOR_OBJ_DECLARE(
  conv_12_dw_batchnorm_bias, AI_STATIC,
  54, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_12_dw_batchnorm_bias_array, NULL)

/* Tensor #55 */
AI_TENSOR_OBJ_DECLARE(
  conv_12_dw_batchnorm_output, AI_STATIC,
  55, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_12_dw_batchnorm_output_array, NULL)

/* Tensor #56 */
AI_TENSOR_OBJ_DECLARE(
  conv_12_dw_batchnorm_scale, AI_STATIC,
  56, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_12_dw_batchnorm_scale_array, NULL)

/* Tensor #57 */
AI_TENSOR_OBJ_DECLARE(
  conv_12_dw_conv2d_output, AI_STATIC,
  57, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_12_dw_conv2d_output_array, NULL)

/* Tensor #58 */
AI_TENSOR_OBJ_DECLARE(
  conv_12_dw_conv2d_quantized_output, AI_STATIC,
  58, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 1, 1, 512, 7168),
  1, &conv_12_dw_conv2d_quantized_output_array, &conv_12_dw_conv2d_quantized_output_array_intq)

/* Tensor #59 */
AI_TENSOR_OBJ_DECLARE(
  conv_12_dw_conv2d_quantized_scratch0, AI_STATIC,
  59, 0x0,
  AI_SHAPE_INIT(4, 1, 13825, 1, 1), AI_STRIDE_INIT(4, 1, 1, 13825, 13825),
  1, &conv_12_dw_conv2d_quantized_scratch0_array, NULL)

/* Tensor #60 */
AI_TENSOR_OBJ_DECLARE(
  conv_12_dw_conv2d_quantized_weights, AI_STATIC,
  60, 0x1,
  AI_SHAPE_INIT(4, 512, 3, 3, 1), AI_STRIDE_INIT(4, 1, 512, 512, 1536),
  1, &conv_12_dw_conv2d_quantized_weights_array, &conv_12_dw_conv2d_quantized_weights_array_intq)

/* Tensor #61 */
AI_TENSOR_OBJ_DECLARE(
  conv_12_dw_relu_alpha, AI_STATIC,
  61, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_12_dw_relu_alpha_array, NULL)

/* Tensor #62 */
AI_TENSOR_OBJ_DECLARE(
  conv_12_dw_relu_output, AI_STATIC,
  62, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_12_dw_relu_output_array, NULL)

/* Tensor #63 */
AI_TENSOR_OBJ_DECLARE(
  conv_12_dw_relu_quantized_output, AI_STATIC,
  63, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 1, 1, 512, 7168),
  1, &conv_12_dw_relu_quantized_output_array, &conv_12_dw_relu_quantized_output_array_intq)

/* Tensor #64 */
AI_TENSOR_OBJ_DECLARE(
  conv_12_relu_alpha, AI_STATIC,
  64, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_12_relu_alpha_array, NULL)

/* Tensor #65 */
AI_TENSOR_OBJ_DECLARE(
  conv_12_relu_output, AI_STATIC,
  65, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_12_relu_output_array, NULL)

/* Tensor #66 */
AI_TENSOR_OBJ_DECLARE(
  conv_12_relu_quantized_output, AI_STATIC,
  66, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 1, 1, 512, 7168),
  1, &conv_12_relu_quantized_output_array, &conv_12_relu_quantized_output_array_intq)

/* Tensor #67 */
AI_TENSOR_OBJ_DECLARE(
  conv_13_batchnorm_bias, AI_STATIC,
  67, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4096, 4096),
  1, &conv_13_batchnorm_bias_array, NULL)

/* Tensor #68 */
AI_TENSOR_OBJ_DECLARE(
  conv_13_batchnorm_output, AI_STATIC,
  68, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 7, 7), AI_STRIDE_INIT(4, 4, 4, 4096, 28672),
  1, &conv_13_batchnorm_output_array, NULL)

/* Tensor #69 */
AI_TENSOR_OBJ_DECLARE(
  conv_13_batchnorm_scale, AI_STATIC,
  69, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4096, 4096),
  1, &conv_13_batchnorm_scale_array, NULL)

/* Tensor #70 */
AI_TENSOR_OBJ_DECLARE(
  conv_13_conv2d_output, AI_STATIC,
  70, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 7, 7), AI_STRIDE_INIT(4, 4, 4, 4096, 28672),
  1, &conv_13_conv2d_output_array, NULL)

/* Tensor #71 */
AI_TENSOR_OBJ_DECLARE(
  conv_13_conv2d_quantized_bias, AI_STATIC,
  71, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4096, 4096),
  1, &conv_13_conv2d_quantized_bias_array, NULL)

/* Tensor #72 */
AI_TENSOR_OBJ_DECLARE(
  conv_13_conv2d_quantized_output, AI_STATIC,
  72, 0x1,
  AI_SHAPE_INIT(4, 1, 1024, 7, 7), AI_STRIDE_INIT(4, 1, 1, 1024, 7168),
  1, &conv_13_conv2d_quantized_output_array, &conv_13_conv2d_quantized_output_array_intq)

/* Tensor #73 */
AI_TENSOR_OBJ_DECLARE(
  conv_13_conv2d_quantized_scratch0, AI_STATIC,
  73, 0x0,
  AI_SHAPE_INIT(4, 1, 2048, 1, 1), AI_STRIDE_INIT(4, 1, 1, 2048, 2048),
  1, &conv_13_conv2d_quantized_scratch0_array, NULL)

/* Tensor #74 */
AI_TENSOR_OBJ_DECLARE(
  conv_13_conv2d_quantized_weights, AI_STATIC,
  74, 0x1,
  AI_SHAPE_INIT(4, 512, 1, 1, 1024), AI_STRIDE_INIT(4, 1, 512, 524288, 524288),
  1, &conv_13_conv2d_quantized_weights_array, &conv_13_conv2d_quantized_weights_array_intq)

/* Tensor #75 */
AI_TENSOR_OBJ_DECLARE(
  conv_13_dw_batchnorm_bias, AI_STATIC,
  75, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_13_dw_batchnorm_bias_array, NULL)

/* Tensor #76 */
AI_TENSOR_OBJ_DECLARE(
  conv_13_dw_batchnorm_output, AI_STATIC,
  76, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &conv_13_dw_batchnorm_output_array, NULL)

/* Tensor #77 */
AI_TENSOR_OBJ_DECLARE(
  conv_13_dw_batchnorm_scale, AI_STATIC,
  77, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_13_dw_batchnorm_scale_array, NULL)

/* Tensor #78 */
AI_TENSOR_OBJ_DECLARE(
  conv_13_dw_conv2d_output, AI_STATIC,
  78, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &conv_13_dw_conv2d_output_array, NULL)

/* Tensor #79 */
AI_TENSOR_OBJ_DECLARE(
  conv_13_dw_conv2d_quantized_output, AI_STATIC,
  79, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 1, 1, 512, 3584),
  1, &conv_13_dw_conv2d_quantized_output_array, &conv_13_dw_conv2d_quantized_output_array_intq)

/* Tensor #80 */
AI_TENSOR_OBJ_DECLARE(
  conv_13_dw_conv2d_quantized_scratch0, AI_STATIC,
  80, 0x0,
  AI_SHAPE_INIT(4, 1, 13825, 1, 1), AI_STRIDE_INIT(4, 1, 1, 13825, 13825),
  1, &conv_13_dw_conv2d_quantized_scratch0_array, NULL)

/* Tensor #81 */
AI_TENSOR_OBJ_DECLARE(
  conv_13_dw_conv2d_quantized_weights, AI_STATIC,
  81, 0x1,
  AI_SHAPE_INIT(4, 512, 3, 3, 1), AI_STRIDE_INIT(4, 1, 512, 512, 1536),
  1, &conv_13_dw_conv2d_quantized_weights_array, &conv_13_dw_conv2d_quantized_weights_array_intq)

/* Tensor #82 */
AI_TENSOR_OBJ_DECLARE(
  conv_13_dw_relu_alpha, AI_STATIC,
  82, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_13_dw_relu_alpha_array, NULL)

/* Tensor #83 */
AI_TENSOR_OBJ_DECLARE(
  conv_13_dw_relu_output, AI_STATIC,
  83, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &conv_13_dw_relu_output_array, NULL)

/* Tensor #84 */
AI_TENSOR_OBJ_DECLARE(
  conv_13_dw_relu_quantized_output, AI_STATIC,
  84, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 1, 1, 512, 3584),
  1, &conv_13_dw_relu_quantized_output_array, &conv_13_dw_relu_quantized_output_array_intq)

/* Tensor #85 */
AI_TENSOR_OBJ_DECLARE(
  conv_13_relu_alpha, AI_STATIC,
  85, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4096, 4096),
  1, &conv_13_relu_alpha_array, NULL)

/* Tensor #86 */
AI_TENSOR_OBJ_DECLARE(
  conv_13_relu_output, AI_STATIC,
  86, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 7, 7), AI_STRIDE_INIT(4, 4, 4, 4096, 28672),
  1, &conv_13_relu_output_array, NULL)

/* Tensor #87 */
AI_TENSOR_OBJ_DECLARE(
  conv_13_relu_quantized_output, AI_STATIC,
  87, 0x1,
  AI_SHAPE_INIT(4, 1, 1024, 7, 7), AI_STRIDE_INIT(4, 1, 1, 1024, 7168),
  1, &conv_13_relu_quantized_output_array, &conv_13_relu_quantized_output_array_intq)

/* Tensor #88 */
AI_TENSOR_OBJ_DECLARE(
  conv_14_batchnorm_bias, AI_STATIC,
  88, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4096, 4096),
  1, &conv_14_batchnorm_bias_array, NULL)

/* Tensor #89 */
AI_TENSOR_OBJ_DECLARE(
  conv_14_batchnorm_output, AI_STATIC,
  89, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 7, 7), AI_STRIDE_INIT(4, 4, 4, 4096, 28672),
  1, &conv_14_batchnorm_output_array, NULL)

/* Tensor #90 */
AI_TENSOR_OBJ_DECLARE(
  conv_14_batchnorm_scale, AI_STATIC,
  90, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4096, 4096),
  1, &conv_14_batchnorm_scale_array, NULL)

/* Tensor #91 */
AI_TENSOR_OBJ_DECLARE(
  conv_14_conv2d_output, AI_STATIC,
  91, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 7, 7), AI_STRIDE_INIT(4, 4, 4, 4096, 28672),
  1, &conv_14_conv2d_output_array, NULL)

/* Tensor #92 */
AI_TENSOR_OBJ_DECLARE(
  conv_14_conv2d_quantized_output, AI_STATIC,
  92, 0x1,
  AI_SHAPE_INIT(4, 1, 1024, 7, 7), AI_STRIDE_INIT(4, 1, 1, 1024, 7168),
  1, &conv_14_conv2d_quantized_output_array, &conv_14_conv2d_quantized_output_array_intq)

/* Tensor #93 */
AI_TENSOR_OBJ_DECLARE(
  conv_14_conv2d_quantized_scratch0, AI_STATIC,
  93, 0x0,
  AI_SHAPE_INIT(4, 1, 4096, 1, 1), AI_STRIDE_INIT(4, 1, 1, 4096, 4096),
  1, &conv_14_conv2d_quantized_scratch0_array, NULL)

/* Tensor #94 */
AI_TENSOR_OBJ_DECLARE(
  conv_14_conv2d_quantized_weights, AI_STATIC,
  94, 0x1,
  AI_SHAPE_INIT(4, 1024, 1, 1, 1024), AI_STRIDE_INIT(4, 1, 1024, 1048576, 1048576),
  1, &conv_14_conv2d_quantized_weights_array, &conv_14_conv2d_quantized_weights_array_intq)

/* Tensor #95 */
AI_TENSOR_OBJ_DECLARE(
  conv_14_dw_batchnorm_bias, AI_STATIC,
  95, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4096, 4096),
  1, &conv_14_dw_batchnorm_bias_array, NULL)

/* Tensor #96 */
AI_TENSOR_OBJ_DECLARE(
  conv_14_dw_batchnorm_output, AI_STATIC,
  96, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 7, 7), AI_STRIDE_INIT(4, 4, 4, 4096, 28672),
  1, &conv_14_dw_batchnorm_output_array, NULL)

/* Tensor #97 */
AI_TENSOR_OBJ_DECLARE(
  conv_14_dw_batchnorm_scale, AI_STATIC,
  97, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4096, 4096),
  1, &conv_14_dw_batchnorm_scale_array, NULL)

/* Tensor #98 */
AI_TENSOR_OBJ_DECLARE(
  conv_14_dw_conv2d_output, AI_STATIC,
  98, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 7, 7), AI_STRIDE_INIT(4, 4, 4, 4096, 28672),
  1, &conv_14_dw_conv2d_output_array, NULL)

/* Tensor #99 */
AI_TENSOR_OBJ_DECLARE(
  conv_14_dw_conv2d_quantized_output, AI_STATIC,
  99, 0x1,
  AI_SHAPE_INIT(4, 1, 1024, 7, 7), AI_STRIDE_INIT(4, 1, 1, 1024, 7168),
  1, &conv_14_dw_conv2d_quantized_output_array, &conv_14_dw_conv2d_quantized_output_array_intq)

/* Tensor #100 */
AI_TENSOR_OBJ_DECLARE(
  conv_14_dw_conv2d_quantized_scratch0, AI_STATIC,
  100, 0x0,
  AI_SHAPE_INIT(4, 1, 27649, 1, 1), AI_STRIDE_INIT(4, 1, 1, 27649, 27649),
  1, &conv_14_dw_conv2d_quantized_scratch0_array, NULL)

/* Tensor #101 */
AI_TENSOR_OBJ_DECLARE(
  conv_14_dw_conv2d_quantized_weights, AI_STATIC,
  101, 0x1,
  AI_SHAPE_INIT(4, 1024, 3, 3, 1), AI_STRIDE_INIT(4, 1, 1024, 1024, 3072),
  1, &conv_14_dw_conv2d_quantized_weights_array, &conv_14_dw_conv2d_quantized_weights_array_intq)

/* Tensor #102 */
AI_TENSOR_OBJ_DECLARE(
  conv_14_dw_relu_alpha, AI_STATIC,
  102, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4096, 4096),
  1, &conv_14_dw_relu_alpha_array, NULL)

/* Tensor #103 */
AI_TENSOR_OBJ_DECLARE(
  conv_14_dw_relu_output, AI_STATIC,
  103, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 7, 7), AI_STRIDE_INIT(4, 4, 4, 4096, 28672),
  1, &conv_14_dw_relu_output_array, NULL)

/* Tensor #104 */
AI_TENSOR_OBJ_DECLARE(
  conv_14_dw_relu_quantized_output, AI_STATIC,
  104, 0x1,
  AI_SHAPE_INIT(4, 1, 1024, 7, 7), AI_STRIDE_INIT(4, 1, 1, 1024, 7168),
  1, &conv_14_dw_relu_quantized_output_array, &conv_14_dw_relu_quantized_output_array_intq)

/* Tensor #105 */
AI_TENSOR_OBJ_DECLARE(
  conv_14_relu_alpha, AI_STATIC,
  105, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4096, 4096),
  1, &conv_14_relu_alpha_array, NULL)

/* Tensor #106 */
AI_TENSOR_OBJ_DECLARE(
  conv_14_relu_output, AI_STATIC,
  106, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 7, 7), AI_STRIDE_INIT(4, 4, 4, 4096, 28672),
  1, &conv_14_relu_output_array, NULL)

/* Tensor #107 */
AI_TENSOR_OBJ_DECLARE(
  conv_1_batchnorm_bias, AI_STATIC,
  107, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &conv_1_batchnorm_bias_array, NULL)

/* Tensor #108 */
AI_TENSOR_OBJ_DECLARE(
  conv_1_batchnorm_output, AI_STATIC,
  108, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 112, 112), AI_STRIDE_INIT(4, 4, 4, 128, 14336),
  1, &conv_1_batchnorm_output_array, NULL)

/* Tensor #109 */
AI_TENSOR_OBJ_DECLARE(
  conv_1_batchnorm_scale, AI_STATIC,
  109, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &conv_1_batchnorm_scale_array, NULL)

/* Tensor #110 */
AI_TENSOR_OBJ_DECLARE(
  conv_1_conv2d_output, AI_STATIC,
  110, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 112, 112), AI_STRIDE_INIT(4, 4, 4, 128, 14336),
  1, &conv_1_conv2d_output_array, NULL)

/* Tensor #111 */
AI_TENSOR_OBJ_DECLARE(
  conv_1_conv2d_quantized_bias, AI_STATIC,
  111, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &conv_1_conv2d_quantized_bias_array, NULL)

/* Tensor #112 */
AI_TENSOR_OBJ_DECLARE(
  conv_1_conv2d_quantized_output, AI_STATIC,
  112, 0x1,
  AI_SHAPE_INIT(4, 1, 32, 112, 112), AI_STRIDE_INIT(4, 1, 1, 32, 3584),
  1, &conv_1_conv2d_quantized_output_array, &conv_1_conv2d_quantized_output_array_intq)

/* Tensor #113 */
AI_TENSOR_OBJ_DECLARE(
  conv_1_conv2d_quantized_scratch0, AI_STATIC,
  113, 0x0,
  AI_SHAPE_INIT(4, 1, 1964, 1, 1), AI_STRIDE_INIT(4, 1, 1, 1964, 1964),
  1, &conv_1_conv2d_quantized_scratch0_array, NULL)

/* Tensor #114 */
AI_TENSOR_OBJ_DECLARE(
  conv_1_conv2d_quantized_weights, AI_STATIC,
  114, 0x1,
  AI_SHAPE_INIT(4, 3, 3, 3, 32), AI_STRIDE_INIT(4, 1, 3, 96, 288),
  1, &conv_1_conv2d_quantized_weights_array, &conv_1_conv2d_quantized_weights_array_intq)

/* Tensor #115 */
AI_TENSOR_OBJ_DECLARE(
  conv_1_relu_alpha, AI_STATIC,
  115, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &conv_1_relu_alpha_array, NULL)

/* Tensor #116 */
AI_TENSOR_OBJ_DECLARE(
  conv_1_relu_output, AI_STATIC,
  116, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 112, 112), AI_STRIDE_INIT(4, 4, 4, 128, 14336),
  1, &conv_1_relu_output_array, NULL)

/* Tensor #117 */
AI_TENSOR_OBJ_DECLARE(
  conv_1_relu_quantized_output, AI_STATIC,
  117, 0x1,
  AI_SHAPE_INIT(4, 1, 32, 112, 112), AI_STRIDE_INIT(4, 1, 1, 32, 3584),
  1, &conv_1_relu_quantized_output_array, &conv_1_relu_quantized_output_array_intq)

/* Tensor #118 */
AI_TENSOR_OBJ_DECLARE(
  conv_2_batchnorm_bias, AI_STATIC,
  118, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &conv_2_batchnorm_bias_array, NULL)

/* Tensor #119 */
AI_TENSOR_OBJ_DECLARE(
  conv_2_batchnorm_output, AI_STATIC,
  119, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 112, 112), AI_STRIDE_INIT(4, 4, 4, 256, 28672),
  1, &conv_2_batchnorm_output_array, NULL)

/* Tensor #120 */
AI_TENSOR_OBJ_DECLARE(
  conv_2_batchnorm_scale, AI_STATIC,
  120, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &conv_2_batchnorm_scale_array, NULL)

/* Tensor #121 */
AI_TENSOR_OBJ_DECLARE(
  conv_2_conv2d_output, AI_STATIC,
  121, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 112, 112), AI_STRIDE_INIT(4, 4, 4, 256, 28672),
  1, &conv_2_conv2d_output_array, NULL)

/* Tensor #122 */
AI_TENSOR_OBJ_DECLARE(
  conv_2_conv2d_quantized_bias, AI_STATIC,
  122, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &conv_2_conv2d_quantized_bias_array, NULL)

/* Tensor #123 */
AI_TENSOR_OBJ_DECLARE(
  conv_2_conv2d_quantized_output, AI_STATIC,
  123, 0x1,
  AI_SHAPE_INIT(4, 1, 64, 112, 112), AI_STRIDE_INIT(4, 1, 1, 64, 7168),
  1, &conv_2_conv2d_quantized_output_array, &conv_2_conv2d_quantized_output_array_intq)

/* Tensor #124 */
AI_TENSOR_OBJ_DECLARE(
  conv_2_conv2d_quantized_scratch0, AI_STATIC,
  124, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 1, 1, 128, 128),
  1, &conv_2_conv2d_quantized_scratch0_array, NULL)

/* Tensor #125 */
AI_TENSOR_OBJ_DECLARE(
  conv_2_conv2d_quantized_weights, AI_STATIC,
  125, 0x1,
  AI_SHAPE_INIT(4, 32, 1, 1, 64), AI_STRIDE_INIT(4, 1, 32, 2048, 2048),
  1, &conv_2_conv2d_quantized_weights_array, &conv_2_conv2d_quantized_weights_array_intq)

/* Tensor #126 */
AI_TENSOR_OBJ_DECLARE(
  conv_2_dw_batchnorm_bias, AI_STATIC,
  126, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &conv_2_dw_batchnorm_bias_array, NULL)

/* Tensor #127 */
AI_TENSOR_OBJ_DECLARE(
  conv_2_dw_batchnorm_output, AI_STATIC,
  127, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 112, 112), AI_STRIDE_INIT(4, 4, 4, 128, 14336),
  1, &conv_2_dw_batchnorm_output_array, NULL)

/* Tensor #128 */
AI_TENSOR_OBJ_DECLARE(
  conv_2_dw_batchnorm_scale, AI_STATIC,
  128, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &conv_2_dw_batchnorm_scale_array, NULL)

/* Tensor #129 */
AI_TENSOR_OBJ_DECLARE(
  conv_2_dw_conv2d_output, AI_STATIC,
  129, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 112, 112), AI_STRIDE_INIT(4, 4, 4, 128, 14336),
  1, &conv_2_dw_conv2d_output_array, NULL)

/* Tensor #130 */
AI_TENSOR_OBJ_DECLARE(
  conv_2_dw_conv2d_quantized_output, AI_STATIC,
  130, 0x1,
  AI_SHAPE_INIT(4, 1, 32, 112, 112), AI_STRIDE_INIT(4, 1, 1, 32, 3584),
  1, &conv_2_dw_conv2d_quantized_output_array, &conv_2_dw_conv2d_quantized_output_array_intq)

/* Tensor #131 */
AI_TENSOR_OBJ_DECLARE(
  conv_2_dw_conv2d_quantized_scratch0, AI_STATIC,
  131, 0x0,
  AI_SHAPE_INIT(4, 1, 865, 1, 1), AI_STRIDE_INIT(4, 1, 1, 865, 865),
  1, &conv_2_dw_conv2d_quantized_scratch0_array, NULL)

/* Tensor #132 */
AI_TENSOR_OBJ_DECLARE(
  conv_2_dw_conv2d_quantized_weights, AI_STATIC,
  132, 0x1,
  AI_SHAPE_INIT(4, 32, 3, 3, 1), AI_STRIDE_INIT(4, 1, 32, 32, 96),
  1, &conv_2_dw_conv2d_quantized_weights_array, &conv_2_dw_conv2d_quantized_weights_array_intq)

/* Tensor #133 */
AI_TENSOR_OBJ_DECLARE(
  conv_2_dw_relu_alpha, AI_STATIC,
  133, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &conv_2_dw_relu_alpha_array, NULL)

/* Tensor #134 */
AI_TENSOR_OBJ_DECLARE(
  conv_2_dw_relu_output, AI_STATIC,
  134, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 112, 112), AI_STRIDE_INIT(4, 4, 4, 128, 14336),
  1, &conv_2_dw_relu_output_array, NULL)

/* Tensor #135 */
AI_TENSOR_OBJ_DECLARE(
  conv_2_dw_relu_quantized_output, AI_STATIC,
  135, 0x1,
  AI_SHAPE_INIT(4, 1, 32, 112, 112), AI_STRIDE_INIT(4, 1, 1, 32, 3584),
  1, &conv_2_dw_relu_quantized_output_array, &conv_2_dw_relu_quantized_output_array_intq)

/* Tensor #136 */
AI_TENSOR_OBJ_DECLARE(
  conv_2_relu_alpha, AI_STATIC,
  136, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &conv_2_relu_alpha_array, NULL)

/* Tensor #137 */
AI_TENSOR_OBJ_DECLARE(
  conv_2_relu_output, AI_STATIC,
  137, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 112, 112), AI_STRIDE_INIT(4, 4, 4, 256, 28672),
  1, &conv_2_relu_output_array, NULL)

/* Tensor #138 */
AI_TENSOR_OBJ_DECLARE(
  conv_2_relu_quantized_output, AI_STATIC,
  138, 0x1,
  AI_SHAPE_INIT(4, 1, 64, 112, 112), AI_STRIDE_INIT(4, 1, 1, 64, 7168),
  1, &conv_2_relu_quantized_output_array, &conv_2_relu_quantized_output_array_intq)

/* Tensor #139 */
AI_TENSOR_OBJ_DECLARE(
  conv_3_batchnorm_bias, AI_STATIC,
  139, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &conv_3_batchnorm_bias_array, NULL)

/* Tensor #140 */
AI_TENSOR_OBJ_DECLARE(
  conv_3_batchnorm_output, AI_STATIC,
  140, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 4, 4, 512, 28672),
  1, &conv_3_batchnorm_output_array, NULL)

/* Tensor #141 */
AI_TENSOR_OBJ_DECLARE(
  conv_3_batchnorm_scale, AI_STATIC,
  141, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &conv_3_batchnorm_scale_array, NULL)

/* Tensor #142 */
AI_TENSOR_OBJ_DECLARE(
  conv_3_conv2d_output, AI_STATIC,
  142, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 4, 4, 512, 28672),
  1, &conv_3_conv2d_output_array, NULL)

/* Tensor #143 */
AI_TENSOR_OBJ_DECLARE(
  conv_3_conv2d_quantized_bias, AI_STATIC,
  143, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &conv_3_conv2d_quantized_bias_array, NULL)

/* Tensor #144 */
AI_TENSOR_OBJ_DECLARE(
  conv_3_conv2d_quantized_output, AI_STATIC,
  144, 0x1,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 1, 1, 128, 7168),
  1, &conv_3_conv2d_quantized_output_array, &conv_3_conv2d_quantized_output_array_intq)

/* Tensor #145 */
AI_TENSOR_OBJ_DECLARE(
  conv_3_conv2d_quantized_scratch0, AI_STATIC,
  145, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 1, 1, 256, 256),
  1, &conv_3_conv2d_quantized_scratch0_array, NULL)

/* Tensor #146 */
AI_TENSOR_OBJ_DECLARE(
  conv_3_conv2d_quantized_weights, AI_STATIC,
  146, 0x1,
  AI_SHAPE_INIT(4, 64, 1, 1, 128), AI_STRIDE_INIT(4, 1, 64, 8192, 8192),
  1, &conv_3_conv2d_quantized_weights_array, &conv_3_conv2d_quantized_weights_array_intq)

/* Tensor #147 */
AI_TENSOR_OBJ_DECLARE(
  conv_3_dw_batchnorm_bias, AI_STATIC,
  147, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &conv_3_dw_batchnorm_bias_array, NULL)

/* Tensor #148 */
AI_TENSOR_OBJ_DECLARE(
  conv_3_dw_batchnorm_output, AI_STATIC,
  148, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &conv_3_dw_batchnorm_output_array, NULL)

/* Tensor #149 */
AI_TENSOR_OBJ_DECLARE(
  conv_3_dw_batchnorm_scale, AI_STATIC,
  149, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &conv_3_dw_batchnorm_scale_array, NULL)

/* Tensor #150 */
AI_TENSOR_OBJ_DECLARE(
  conv_3_dw_conv2d_output, AI_STATIC,
  150, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &conv_3_dw_conv2d_output_array, NULL)

/* Tensor #151 */
AI_TENSOR_OBJ_DECLARE(
  conv_3_dw_conv2d_quantized_output, AI_STATIC,
  151, 0x1,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 1, 1, 64, 3584),
  1, &conv_3_dw_conv2d_quantized_output_array, &conv_3_dw_conv2d_quantized_output_array_intq)

/* Tensor #152 */
AI_TENSOR_OBJ_DECLARE(
  conv_3_dw_conv2d_quantized_scratch0, AI_STATIC,
  152, 0x0,
  AI_SHAPE_INIT(4, 1, 1729, 1, 1), AI_STRIDE_INIT(4, 1, 1, 1729, 1729),
  1, &conv_3_dw_conv2d_quantized_scratch0_array, NULL)

/* Tensor #153 */
AI_TENSOR_OBJ_DECLARE(
  conv_3_dw_conv2d_quantized_weights, AI_STATIC,
  153, 0x1,
  AI_SHAPE_INIT(4, 64, 3, 3, 1), AI_STRIDE_INIT(4, 1, 64, 64, 192),
  1, &conv_3_dw_conv2d_quantized_weights_array, &conv_3_dw_conv2d_quantized_weights_array_intq)

/* Tensor #154 */
AI_TENSOR_OBJ_DECLARE(
  conv_3_dw_relu_alpha, AI_STATIC,
  154, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &conv_3_dw_relu_alpha_array, NULL)

/* Tensor #155 */
AI_TENSOR_OBJ_DECLARE(
  conv_3_dw_relu_output, AI_STATIC,
  155, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &conv_3_dw_relu_output_array, NULL)

/* Tensor #156 */
AI_TENSOR_OBJ_DECLARE(
  conv_3_dw_relu_quantized_output, AI_STATIC,
  156, 0x1,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 1, 1, 64, 3584),
  1, &conv_3_dw_relu_quantized_output_array, &conv_3_dw_relu_quantized_output_array_intq)

/* Tensor #157 */
AI_TENSOR_OBJ_DECLARE(
  conv_3_relu_alpha, AI_STATIC,
  157, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &conv_3_relu_alpha_array, NULL)

/* Tensor #158 */
AI_TENSOR_OBJ_DECLARE(
  conv_3_relu_output, AI_STATIC,
  158, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 4, 4, 512, 28672),
  1, &conv_3_relu_output_array, NULL)

/* Tensor #159 */
AI_TENSOR_OBJ_DECLARE(
  conv_3_relu_quantized_output, AI_STATIC,
  159, 0x1,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 1, 1, 128, 7168),
  1, &conv_3_relu_quantized_output_array, &conv_3_relu_quantized_output_array_intq)

/* Tensor #160 */
AI_TENSOR_OBJ_DECLARE(
  conv_4_batchnorm_bias, AI_STATIC,
  160, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &conv_4_batchnorm_bias_array, NULL)

/* Tensor #161 */
AI_TENSOR_OBJ_DECLARE(
  conv_4_batchnorm_output, AI_STATIC,
  161, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 4, 4, 512, 28672),
  1, &conv_4_batchnorm_output_array, NULL)

/* Tensor #162 */
AI_TENSOR_OBJ_DECLARE(
  conv_4_batchnorm_scale, AI_STATIC,
  162, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &conv_4_batchnorm_scale_array, NULL)

/* Tensor #163 */
AI_TENSOR_OBJ_DECLARE(
  conv_4_conv2d_output, AI_STATIC,
  163, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 4, 4, 512, 28672),
  1, &conv_4_conv2d_output_array, NULL)

/* Tensor #164 */
AI_TENSOR_OBJ_DECLARE(
  conv_4_conv2d_quantized_output, AI_STATIC,
  164, 0x1,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 1, 1, 128, 7168),
  1, &conv_4_conv2d_quantized_output_array, &conv_4_conv2d_quantized_output_array_intq)

/* Tensor #165 */
AI_TENSOR_OBJ_DECLARE(
  conv_4_conv2d_quantized_scratch0, AI_STATIC,
  165, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 1, 1, 512, 512),
  1, &conv_4_conv2d_quantized_scratch0_array, NULL)

/* Tensor #166 */
AI_TENSOR_OBJ_DECLARE(
  conv_4_conv2d_quantized_weights, AI_STATIC,
  166, 0x1,
  AI_SHAPE_INIT(4, 128, 1, 1, 128), AI_STRIDE_INIT(4, 1, 128, 16384, 16384),
  1, &conv_4_conv2d_quantized_weights_array, &conv_4_conv2d_quantized_weights_array_intq)

/* Tensor #167 */
AI_TENSOR_OBJ_DECLARE(
  conv_4_dw_batchnorm_bias, AI_STATIC,
  167, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &conv_4_dw_batchnorm_bias_array, NULL)

/* Tensor #168 */
AI_TENSOR_OBJ_DECLARE(
  conv_4_dw_batchnorm_output, AI_STATIC,
  168, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 4, 4, 512, 28672),
  1, &conv_4_dw_batchnorm_output_array, NULL)

/* Tensor #169 */
AI_TENSOR_OBJ_DECLARE(
  conv_4_dw_batchnorm_scale, AI_STATIC,
  169, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &conv_4_dw_batchnorm_scale_array, NULL)

/* Tensor #170 */
AI_TENSOR_OBJ_DECLARE(
  conv_4_dw_conv2d_output, AI_STATIC,
  170, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 4, 4, 512, 28672),
  1, &conv_4_dw_conv2d_output_array, NULL)

/* Tensor #171 */
AI_TENSOR_OBJ_DECLARE(
  conv_4_dw_conv2d_quantized_output, AI_STATIC,
  171, 0x1,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 1, 1, 128, 7168),
  1, &conv_4_dw_conv2d_quantized_output_array, &conv_4_dw_conv2d_quantized_output_array_intq)

/* Tensor #172 */
AI_TENSOR_OBJ_DECLARE(
  conv_4_dw_conv2d_quantized_scratch0, AI_STATIC,
  172, 0x0,
  AI_SHAPE_INIT(4, 1, 3457, 1, 1), AI_STRIDE_INIT(4, 1, 1, 3457, 3457),
  1, &conv_4_dw_conv2d_quantized_scratch0_array, NULL)

/* Tensor #173 */
AI_TENSOR_OBJ_DECLARE(
  conv_4_dw_conv2d_quantized_weights, AI_STATIC,
  173, 0x1,
  AI_SHAPE_INIT(4, 128, 3, 3, 1), AI_STRIDE_INIT(4, 1, 128, 128, 384),
  1, &conv_4_dw_conv2d_quantized_weights_array, &conv_4_dw_conv2d_quantized_weights_array_intq)

/* Tensor #174 */
AI_TENSOR_OBJ_DECLARE(
  conv_4_dw_relu_alpha, AI_STATIC,
  174, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &conv_4_dw_relu_alpha_array, NULL)

/* Tensor #175 */
AI_TENSOR_OBJ_DECLARE(
  conv_4_dw_relu_output, AI_STATIC,
  175, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 4, 4, 512, 28672),
  1, &conv_4_dw_relu_output_array, NULL)

/* Tensor #176 */
AI_TENSOR_OBJ_DECLARE(
  conv_4_dw_relu_quantized_output, AI_STATIC,
  176, 0x1,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 1, 1, 128, 7168),
  1, &conv_4_dw_relu_quantized_output_array, &conv_4_dw_relu_quantized_output_array_intq)

/* Tensor #177 */
AI_TENSOR_OBJ_DECLARE(
  conv_4_relu_alpha, AI_STATIC,
  177, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &conv_4_relu_alpha_array, NULL)

/* Tensor #178 */
AI_TENSOR_OBJ_DECLARE(
  conv_4_relu_output, AI_STATIC,
  178, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 4, 4, 512, 28672),
  1, &conv_4_relu_output_array, NULL)

/* Tensor #179 */
AI_TENSOR_OBJ_DECLARE(
  conv_4_relu_quantized_output, AI_STATIC,
  179, 0x1,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 1, 1, 128, 7168),
  1, &conv_4_relu_quantized_output_array, &conv_4_relu_quantized_output_array_intq)

/* Tensor #180 */
AI_TENSOR_OBJ_DECLARE(
  conv_5_batchnorm_bias, AI_STATIC,
  180, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &conv_5_batchnorm_bias_array, NULL)

/* Tensor #181 */
AI_TENSOR_OBJ_DECLARE(
  conv_5_batchnorm_output, AI_STATIC,
  181, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 28, 28), AI_STRIDE_INIT(4, 4, 4, 1024, 28672),
  1, &conv_5_batchnorm_output_array, NULL)

/* Tensor #182 */
AI_TENSOR_OBJ_DECLARE(
  conv_5_batchnorm_scale, AI_STATIC,
  182, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &conv_5_batchnorm_scale_array, NULL)

/* Tensor #183 */
AI_TENSOR_OBJ_DECLARE(
  conv_5_conv2d_output, AI_STATIC,
  183, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 28, 28), AI_STRIDE_INIT(4, 4, 4, 1024, 28672),
  1, &conv_5_conv2d_output_array, NULL)

/* Tensor #184 */
AI_TENSOR_OBJ_DECLARE(
  conv_5_conv2d_quantized_bias, AI_STATIC,
  184, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &conv_5_conv2d_quantized_bias_array, NULL)

/* Tensor #185 */
AI_TENSOR_OBJ_DECLARE(
  conv_5_conv2d_quantized_output, AI_STATIC,
  185, 0x1,
  AI_SHAPE_INIT(4, 1, 256, 28, 28), AI_STRIDE_INIT(4, 1, 1, 256, 7168),
  1, &conv_5_conv2d_quantized_output_array, &conv_5_conv2d_quantized_output_array_intq)

/* Tensor #186 */
AI_TENSOR_OBJ_DECLARE(
  conv_5_conv2d_quantized_scratch0, AI_STATIC,
  186, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 1, 1, 512, 512),
  1, &conv_5_conv2d_quantized_scratch0_array, NULL)

/* Tensor #187 */
AI_TENSOR_OBJ_DECLARE(
  conv_5_conv2d_quantized_weights, AI_STATIC,
  187, 0x1,
  AI_SHAPE_INIT(4, 128, 1, 1, 256), AI_STRIDE_INIT(4, 1, 128, 32768, 32768),
  1, &conv_5_conv2d_quantized_weights_array, &conv_5_conv2d_quantized_weights_array_intq)

/* Tensor #188 */
AI_TENSOR_OBJ_DECLARE(
  conv_5_dw_batchnorm_bias, AI_STATIC,
  188, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &conv_5_dw_batchnorm_bias_array, NULL)

/* Tensor #189 */
AI_TENSOR_OBJ_DECLARE(
  conv_5_dw_batchnorm_output, AI_STATIC,
  189, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &conv_5_dw_batchnorm_output_array, NULL)

/* Tensor #190 */
AI_TENSOR_OBJ_DECLARE(
  conv_5_dw_batchnorm_scale, AI_STATIC,
  190, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &conv_5_dw_batchnorm_scale_array, NULL)

/* Tensor #191 */
AI_TENSOR_OBJ_DECLARE(
  conv_5_dw_conv2d_output, AI_STATIC,
  191, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &conv_5_dw_conv2d_output_array, NULL)

/* Tensor #192 */
AI_TENSOR_OBJ_DECLARE(
  conv_5_dw_conv2d_quantized_output, AI_STATIC,
  192, 0x1,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 1, 1, 128, 3584),
  1, &conv_5_dw_conv2d_quantized_output_array, &conv_5_dw_conv2d_quantized_output_array_intq)

/* Tensor #193 */
AI_TENSOR_OBJ_DECLARE(
  conv_5_dw_conv2d_quantized_scratch0, AI_STATIC,
  193, 0x0,
  AI_SHAPE_INIT(4, 1, 3457, 1, 1), AI_STRIDE_INIT(4, 1, 1, 3457, 3457),
  1, &conv_5_dw_conv2d_quantized_scratch0_array, NULL)

/* Tensor #194 */
AI_TENSOR_OBJ_DECLARE(
  conv_5_dw_conv2d_quantized_weights, AI_STATIC,
  194, 0x1,
  AI_SHAPE_INIT(4, 128, 3, 3, 1), AI_STRIDE_INIT(4, 1, 128, 128, 384),
  1, &conv_5_dw_conv2d_quantized_weights_array, &conv_5_dw_conv2d_quantized_weights_array_intq)

/* Tensor #195 */
AI_TENSOR_OBJ_DECLARE(
  conv_5_dw_relu_alpha, AI_STATIC,
  195, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &conv_5_dw_relu_alpha_array, NULL)

/* Tensor #196 */
AI_TENSOR_OBJ_DECLARE(
  conv_5_dw_relu_output, AI_STATIC,
  196, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &conv_5_dw_relu_output_array, NULL)

/* Tensor #197 */
AI_TENSOR_OBJ_DECLARE(
  conv_5_dw_relu_quantized_output, AI_STATIC,
  197, 0x1,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 1, 1, 128, 3584),
  1, &conv_5_dw_relu_quantized_output_array, &conv_5_dw_relu_quantized_output_array_intq)

/* Tensor #198 */
AI_TENSOR_OBJ_DECLARE(
  conv_5_relu_alpha, AI_STATIC,
  198, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &conv_5_relu_alpha_array, NULL)

/* Tensor #199 */
AI_TENSOR_OBJ_DECLARE(
  conv_5_relu_output, AI_STATIC,
  199, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 28, 28), AI_STRIDE_INIT(4, 4, 4, 1024, 28672),
  1, &conv_5_relu_output_array, NULL)

/* Tensor #200 */
AI_TENSOR_OBJ_DECLARE(
  conv_5_relu_quantized_output, AI_STATIC,
  200, 0x1,
  AI_SHAPE_INIT(4, 1, 256, 28, 28), AI_STRIDE_INIT(4, 1, 1, 256, 7168),
  1, &conv_5_relu_quantized_output_array, &conv_5_relu_quantized_output_array_intq)

/* Tensor #201 */
AI_TENSOR_OBJ_DECLARE(
  conv_6_batchnorm_bias, AI_STATIC,
  201, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &conv_6_batchnorm_bias_array, NULL)

/* Tensor #202 */
AI_TENSOR_OBJ_DECLARE(
  conv_6_batchnorm_output, AI_STATIC,
  202, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 28, 28), AI_STRIDE_INIT(4, 4, 4, 1024, 28672),
  1, &conv_6_batchnorm_output_array, NULL)

/* Tensor #203 */
AI_TENSOR_OBJ_DECLARE(
  conv_6_batchnorm_scale, AI_STATIC,
  203, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &conv_6_batchnorm_scale_array, NULL)

/* Tensor #204 */
AI_TENSOR_OBJ_DECLARE(
  conv_6_conv2d_output, AI_STATIC,
  204, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 28, 28), AI_STRIDE_INIT(4, 4, 4, 1024, 28672),
  1, &conv_6_conv2d_output_array, NULL)

/* Tensor #205 */
AI_TENSOR_OBJ_DECLARE(
  conv_6_conv2d_quantized_output, AI_STATIC,
  205, 0x1,
  AI_SHAPE_INIT(4, 1, 256, 28, 28), AI_STRIDE_INIT(4, 1, 1, 256, 7168),
  1, &conv_6_conv2d_quantized_output_array, &conv_6_conv2d_quantized_output_array_intq)

/* Tensor #206 */
AI_TENSOR_OBJ_DECLARE(
  conv_6_conv2d_quantized_scratch0, AI_STATIC,
  206, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 1, 1), AI_STRIDE_INIT(4, 1, 1, 1024, 1024),
  1, &conv_6_conv2d_quantized_scratch0_array, NULL)

/* Tensor #207 */
AI_TENSOR_OBJ_DECLARE(
  conv_6_conv2d_quantized_weights, AI_STATIC,
  207, 0x1,
  AI_SHAPE_INIT(4, 256, 1, 1, 256), AI_STRIDE_INIT(4, 1, 256, 65536, 65536),
  1, &conv_6_conv2d_quantized_weights_array, &conv_6_conv2d_quantized_weights_array_intq)

/* Tensor #208 */
AI_TENSOR_OBJ_DECLARE(
  conv_6_dw_batchnorm_bias, AI_STATIC,
  208, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &conv_6_dw_batchnorm_bias_array, NULL)

/* Tensor #209 */
AI_TENSOR_OBJ_DECLARE(
  conv_6_dw_batchnorm_output, AI_STATIC,
  209, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 28, 28), AI_STRIDE_INIT(4, 4, 4, 1024, 28672),
  1, &conv_6_dw_batchnorm_output_array, NULL)

/* Tensor #210 */
AI_TENSOR_OBJ_DECLARE(
  conv_6_dw_batchnorm_scale, AI_STATIC,
  210, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &conv_6_dw_batchnorm_scale_array, NULL)

/* Tensor #211 */
AI_TENSOR_OBJ_DECLARE(
  conv_6_dw_conv2d_output, AI_STATIC,
  211, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 28, 28), AI_STRIDE_INIT(4, 4, 4, 1024, 28672),
  1, &conv_6_dw_conv2d_output_array, NULL)

/* Tensor #212 */
AI_TENSOR_OBJ_DECLARE(
  conv_6_dw_conv2d_quantized_output, AI_STATIC,
  212, 0x1,
  AI_SHAPE_INIT(4, 1, 256, 28, 28), AI_STRIDE_INIT(4, 1, 1, 256, 7168),
  1, &conv_6_dw_conv2d_quantized_output_array, &conv_6_dw_conv2d_quantized_output_array_intq)

/* Tensor #213 */
AI_TENSOR_OBJ_DECLARE(
  conv_6_dw_conv2d_quantized_scratch0, AI_STATIC,
  213, 0x0,
  AI_SHAPE_INIT(4, 1, 6913, 1, 1), AI_STRIDE_INIT(4, 1, 1, 6913, 6913),
  1, &conv_6_dw_conv2d_quantized_scratch0_array, NULL)

/* Tensor #214 */
AI_TENSOR_OBJ_DECLARE(
  conv_6_dw_conv2d_quantized_weights, AI_STATIC,
  214, 0x1,
  AI_SHAPE_INIT(4, 256, 3, 3, 1), AI_STRIDE_INIT(4, 1, 256, 256, 768),
  1, &conv_6_dw_conv2d_quantized_weights_array, &conv_6_dw_conv2d_quantized_weights_array_intq)

/* Tensor #215 */
AI_TENSOR_OBJ_DECLARE(
  conv_6_dw_relu_alpha, AI_STATIC,
  215, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &conv_6_dw_relu_alpha_array, NULL)

/* Tensor #216 */
AI_TENSOR_OBJ_DECLARE(
  conv_6_dw_relu_output, AI_STATIC,
  216, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 28, 28), AI_STRIDE_INIT(4, 4, 4, 1024, 28672),
  1, &conv_6_dw_relu_output_array, NULL)

/* Tensor #217 */
AI_TENSOR_OBJ_DECLARE(
  conv_6_dw_relu_quantized_output, AI_STATIC,
  217, 0x1,
  AI_SHAPE_INIT(4, 1, 256, 28, 28), AI_STRIDE_INIT(4, 1, 1, 256, 7168),
  1, &conv_6_dw_relu_quantized_output_array, &conv_6_dw_relu_quantized_output_array_intq)

/* Tensor #218 */
AI_TENSOR_OBJ_DECLARE(
  conv_6_relu_alpha, AI_STATIC,
  218, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &conv_6_relu_alpha_array, NULL)

/* Tensor #219 */
AI_TENSOR_OBJ_DECLARE(
  conv_6_relu_output, AI_STATIC,
  219, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 28, 28), AI_STRIDE_INIT(4, 4, 4, 1024, 28672),
  1, &conv_6_relu_output_array, NULL)

/* Tensor #220 */
AI_TENSOR_OBJ_DECLARE(
  conv_6_relu_quantized_output, AI_STATIC,
  220, 0x1,
  AI_SHAPE_INIT(4, 1, 256, 28, 28), AI_STRIDE_INIT(4, 1, 1, 256, 7168),
  1, &conv_6_relu_quantized_output_array, &conv_6_relu_quantized_output_array_intq)

/* Tensor #221 */
AI_TENSOR_OBJ_DECLARE(
  conv_7_batchnorm_bias, AI_STATIC,
  221, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_7_batchnorm_bias_array, NULL)

/* Tensor #222 */
AI_TENSOR_OBJ_DECLARE(
  conv_7_batchnorm_output, AI_STATIC,
  222, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_7_batchnorm_output_array, NULL)

/* Tensor #223 */
AI_TENSOR_OBJ_DECLARE(
  conv_7_batchnorm_scale, AI_STATIC,
  223, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_7_batchnorm_scale_array, NULL)

/* Tensor #224 */
AI_TENSOR_OBJ_DECLARE(
  conv_7_conv2d_output, AI_STATIC,
  224, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_7_conv2d_output_array, NULL)

/* Tensor #225 */
AI_TENSOR_OBJ_DECLARE(
  conv_7_conv2d_quantized_bias, AI_STATIC,
  225, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_7_conv2d_quantized_bias_array, NULL)

/* Tensor #226 */
AI_TENSOR_OBJ_DECLARE(
  conv_7_conv2d_quantized_output, AI_STATIC,
  226, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 1, 1, 512, 7168),
  1, &conv_7_conv2d_quantized_output_array, &conv_7_conv2d_quantized_output_array_intq)

/* Tensor #227 */
AI_TENSOR_OBJ_DECLARE(
  conv_7_conv2d_quantized_scratch0, AI_STATIC,
  227, 0x0,
  AI_SHAPE_INIT(4, 1, 1024, 1, 1), AI_STRIDE_INIT(4, 1, 1, 1024, 1024),
  1, &conv_7_conv2d_quantized_scratch0_array, NULL)

/* Tensor #228 */
AI_TENSOR_OBJ_DECLARE(
  conv_7_conv2d_quantized_weights, AI_STATIC,
  228, 0x1,
  AI_SHAPE_INIT(4, 256, 1, 1, 512), AI_STRIDE_INIT(4, 1, 256, 131072, 131072),
  1, &conv_7_conv2d_quantized_weights_array, &conv_7_conv2d_quantized_weights_array_intq)

/* Tensor #229 */
AI_TENSOR_OBJ_DECLARE(
  conv_7_dw_batchnorm_bias, AI_STATIC,
  229, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &conv_7_dw_batchnorm_bias_array, NULL)

/* Tensor #230 */
AI_TENSOR_OBJ_DECLARE(
  conv_7_dw_batchnorm_output, AI_STATIC,
  230, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &conv_7_dw_batchnorm_output_array, NULL)

/* Tensor #231 */
AI_TENSOR_OBJ_DECLARE(
  conv_7_dw_batchnorm_scale, AI_STATIC,
  231, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &conv_7_dw_batchnorm_scale_array, NULL)

/* Tensor #232 */
AI_TENSOR_OBJ_DECLARE(
  conv_7_dw_conv2d_output, AI_STATIC,
  232, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &conv_7_dw_conv2d_output_array, NULL)

/* Tensor #233 */
AI_TENSOR_OBJ_DECLARE(
  conv_7_dw_conv2d_quantized_output, AI_STATIC,
  233, 0x1,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 1, 1, 256, 3584),
  1, &conv_7_dw_conv2d_quantized_output_array, &conv_7_dw_conv2d_quantized_output_array_intq)

/* Tensor #234 */
AI_TENSOR_OBJ_DECLARE(
  conv_7_dw_conv2d_quantized_scratch0, AI_STATIC,
  234, 0x0,
  AI_SHAPE_INIT(4, 1, 6913, 1, 1), AI_STRIDE_INIT(4, 1, 1, 6913, 6913),
  1, &conv_7_dw_conv2d_quantized_scratch0_array, NULL)

/* Tensor #235 */
AI_TENSOR_OBJ_DECLARE(
  conv_7_dw_conv2d_quantized_weights, AI_STATIC,
  235, 0x1,
  AI_SHAPE_INIT(4, 256, 3, 3, 1), AI_STRIDE_INIT(4, 1, 256, 256, 768),
  1, &conv_7_dw_conv2d_quantized_weights_array, &conv_7_dw_conv2d_quantized_weights_array_intq)

/* Tensor #236 */
AI_TENSOR_OBJ_DECLARE(
  conv_7_dw_relu_alpha, AI_STATIC,
  236, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &conv_7_dw_relu_alpha_array, NULL)

/* Tensor #237 */
AI_TENSOR_OBJ_DECLARE(
  conv_7_dw_relu_output, AI_STATIC,
  237, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &conv_7_dw_relu_output_array, NULL)

/* Tensor #238 */
AI_TENSOR_OBJ_DECLARE(
  conv_7_dw_relu_quantized_output, AI_STATIC,
  238, 0x1,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 1, 1, 256, 3584),
  1, &conv_7_dw_relu_quantized_output_array, &conv_7_dw_relu_quantized_output_array_intq)

/* Tensor #239 */
AI_TENSOR_OBJ_DECLARE(
  conv_7_relu_alpha, AI_STATIC,
  239, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_7_relu_alpha_array, NULL)

/* Tensor #240 */
AI_TENSOR_OBJ_DECLARE(
  conv_7_relu_output, AI_STATIC,
  240, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_7_relu_output_array, NULL)

/* Tensor #241 */
AI_TENSOR_OBJ_DECLARE(
  conv_7_relu_quantized_output, AI_STATIC,
  241, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 1, 1, 512, 7168),
  1, &conv_7_relu_quantized_output_array, &conv_7_relu_quantized_output_array_intq)

/* Tensor #242 */
AI_TENSOR_OBJ_DECLARE(
  conv_8_batchnorm_bias, AI_STATIC,
  242, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_8_batchnorm_bias_array, NULL)

/* Tensor #243 */
AI_TENSOR_OBJ_DECLARE(
  conv_8_batchnorm_output, AI_STATIC,
  243, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_8_batchnorm_output_array, NULL)

/* Tensor #244 */
AI_TENSOR_OBJ_DECLARE(
  conv_8_batchnorm_scale, AI_STATIC,
  244, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_8_batchnorm_scale_array, NULL)

/* Tensor #245 */
AI_TENSOR_OBJ_DECLARE(
  conv_8_conv2d_output, AI_STATIC,
  245, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_8_conv2d_output_array, NULL)

/* Tensor #246 */
AI_TENSOR_OBJ_DECLARE(
  conv_8_conv2d_quantized_output, AI_STATIC,
  246, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 1, 1, 512, 7168),
  1, &conv_8_conv2d_quantized_output_array, &conv_8_conv2d_quantized_output_array_intq)

/* Tensor #247 */
AI_TENSOR_OBJ_DECLARE(
  conv_8_conv2d_quantized_scratch0, AI_STATIC,
  247, 0x0,
  AI_SHAPE_INIT(4, 1, 2048, 1, 1), AI_STRIDE_INIT(4, 1, 1, 2048, 2048),
  1, &conv_8_conv2d_quantized_scratch0_array, NULL)

/* Tensor #248 */
AI_TENSOR_OBJ_DECLARE(
  conv_8_conv2d_quantized_weights, AI_STATIC,
  248, 0x1,
  AI_SHAPE_INIT(4, 512, 1, 1, 512), AI_STRIDE_INIT(4, 1, 512, 262144, 262144),
  1, &conv_8_conv2d_quantized_weights_array, &conv_8_conv2d_quantized_weights_array_intq)

/* Tensor #249 */
AI_TENSOR_OBJ_DECLARE(
  conv_8_dw_batchnorm_bias, AI_STATIC,
  249, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_8_dw_batchnorm_bias_array, NULL)

/* Tensor #250 */
AI_TENSOR_OBJ_DECLARE(
  conv_8_dw_batchnorm_output, AI_STATIC,
  250, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_8_dw_batchnorm_output_array, NULL)

/* Tensor #251 */
AI_TENSOR_OBJ_DECLARE(
  conv_8_dw_batchnorm_scale, AI_STATIC,
  251, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_8_dw_batchnorm_scale_array, NULL)

/* Tensor #252 */
AI_TENSOR_OBJ_DECLARE(
  conv_8_dw_conv2d_output, AI_STATIC,
  252, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_8_dw_conv2d_output_array, NULL)

/* Tensor #253 */
AI_TENSOR_OBJ_DECLARE(
  conv_8_dw_conv2d_quantized_output, AI_STATIC,
  253, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 1, 1, 512, 7168),
  1, &conv_8_dw_conv2d_quantized_output_array, &conv_8_dw_conv2d_quantized_output_array_intq)

/* Tensor #254 */
AI_TENSOR_OBJ_DECLARE(
  conv_8_dw_conv2d_quantized_scratch0, AI_STATIC,
  254, 0x0,
  AI_SHAPE_INIT(4, 1, 13825, 1, 1), AI_STRIDE_INIT(4, 1, 1, 13825, 13825),
  1, &conv_8_dw_conv2d_quantized_scratch0_array, NULL)

/* Tensor #255 */
AI_TENSOR_OBJ_DECLARE(
  conv_8_dw_conv2d_quantized_weights, AI_STATIC,
  255, 0x1,
  AI_SHAPE_INIT(4, 512, 3, 3, 1), AI_STRIDE_INIT(4, 1, 512, 512, 1536),
  1, &conv_8_dw_conv2d_quantized_weights_array, &conv_8_dw_conv2d_quantized_weights_array_intq)

/* Tensor #256 */
AI_TENSOR_OBJ_DECLARE(
  conv_8_dw_relu_alpha, AI_STATIC,
  256, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_8_dw_relu_alpha_array, NULL)

/* Tensor #257 */
AI_TENSOR_OBJ_DECLARE(
  conv_8_dw_relu_output, AI_STATIC,
  257, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_8_dw_relu_output_array, NULL)

/* Tensor #258 */
AI_TENSOR_OBJ_DECLARE(
  conv_8_dw_relu_quantized_output, AI_STATIC,
  258, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 1, 1, 512, 7168),
  1, &conv_8_dw_relu_quantized_output_array, &conv_8_dw_relu_quantized_output_array_intq)

/* Tensor #259 */
AI_TENSOR_OBJ_DECLARE(
  conv_8_relu_alpha, AI_STATIC,
  259, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_8_relu_alpha_array, NULL)

/* Tensor #260 */
AI_TENSOR_OBJ_DECLARE(
  conv_8_relu_output, AI_STATIC,
  260, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_8_relu_output_array, NULL)

/* Tensor #261 */
AI_TENSOR_OBJ_DECLARE(
  conv_8_relu_quantized_output, AI_STATIC,
  261, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 1, 1, 512, 7168),
  1, &conv_8_relu_quantized_output_array, &conv_8_relu_quantized_output_array_intq)

/* Tensor #262 */
AI_TENSOR_OBJ_DECLARE(
  conv_9_batchnorm_bias, AI_STATIC,
  262, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_9_batchnorm_bias_array, NULL)

/* Tensor #263 */
AI_TENSOR_OBJ_DECLARE(
  conv_9_batchnorm_output, AI_STATIC,
  263, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_9_batchnorm_output_array, NULL)

/* Tensor #264 */
AI_TENSOR_OBJ_DECLARE(
  conv_9_batchnorm_scale, AI_STATIC,
  264, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_9_batchnorm_scale_array, NULL)

/* Tensor #265 */
AI_TENSOR_OBJ_DECLARE(
  conv_9_conv2d_output, AI_STATIC,
  265, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_9_conv2d_output_array, NULL)

/* Tensor #266 */
AI_TENSOR_OBJ_DECLARE(
  conv_9_conv2d_quantized_output, AI_STATIC,
  266, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 1, 1, 512, 7168),
  1, &conv_9_conv2d_quantized_output_array, &conv_9_conv2d_quantized_output_array_intq)

/* Tensor #267 */
AI_TENSOR_OBJ_DECLARE(
  conv_9_conv2d_quantized_scratch0, AI_STATIC,
  267, 0x0,
  AI_SHAPE_INIT(4, 1, 2048, 1, 1), AI_STRIDE_INIT(4, 1, 1, 2048, 2048),
  1, &conv_9_conv2d_quantized_scratch0_array, NULL)

/* Tensor #268 */
AI_TENSOR_OBJ_DECLARE(
  conv_9_conv2d_quantized_weights, AI_STATIC,
  268, 0x1,
  AI_SHAPE_INIT(4, 512, 1, 1, 512), AI_STRIDE_INIT(4, 1, 512, 262144, 262144),
  1, &conv_9_conv2d_quantized_weights_array, &conv_9_conv2d_quantized_weights_array_intq)

/* Tensor #269 */
AI_TENSOR_OBJ_DECLARE(
  conv_9_dw_batchnorm_bias, AI_STATIC,
  269, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_9_dw_batchnorm_bias_array, NULL)

/* Tensor #270 */
AI_TENSOR_OBJ_DECLARE(
  conv_9_dw_batchnorm_output, AI_STATIC,
  270, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_9_dw_batchnorm_output_array, NULL)

/* Tensor #271 */
AI_TENSOR_OBJ_DECLARE(
  conv_9_dw_batchnorm_scale, AI_STATIC,
  271, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_9_dw_batchnorm_scale_array, NULL)

/* Tensor #272 */
AI_TENSOR_OBJ_DECLARE(
  conv_9_dw_conv2d_output, AI_STATIC,
  272, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_9_dw_conv2d_output_array, NULL)

/* Tensor #273 */
AI_TENSOR_OBJ_DECLARE(
  conv_9_dw_conv2d_quantized_output, AI_STATIC,
  273, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 1, 1, 512, 7168),
  1, &conv_9_dw_conv2d_quantized_output_array, &conv_9_dw_conv2d_quantized_output_array_intq)

/* Tensor #274 */
AI_TENSOR_OBJ_DECLARE(
  conv_9_dw_conv2d_quantized_scratch0, AI_STATIC,
  274, 0x0,
  AI_SHAPE_INIT(4, 1, 13825, 1, 1), AI_STRIDE_INIT(4, 1, 1, 13825, 13825),
  1, &conv_9_dw_conv2d_quantized_scratch0_array, NULL)

/* Tensor #275 */
AI_TENSOR_OBJ_DECLARE(
  conv_9_dw_conv2d_quantized_weights, AI_STATIC,
  275, 0x1,
  AI_SHAPE_INIT(4, 512, 3, 3, 1), AI_STRIDE_INIT(4, 1, 512, 512, 1536),
  1, &conv_9_dw_conv2d_quantized_weights_array, &conv_9_dw_conv2d_quantized_weights_array_intq)

/* Tensor #276 */
AI_TENSOR_OBJ_DECLARE(
  conv_9_dw_relu_alpha, AI_STATIC,
  276, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_9_dw_relu_alpha_array, NULL)

/* Tensor #277 */
AI_TENSOR_OBJ_DECLARE(
  conv_9_dw_relu_output, AI_STATIC,
  277, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_9_dw_relu_output_array, NULL)

/* Tensor #278 */
AI_TENSOR_OBJ_DECLARE(
  conv_9_dw_relu_quantized_output, AI_STATIC,
  278, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 1, 1, 512, 7168),
  1, &conv_9_dw_relu_quantized_output_array, &conv_9_dw_relu_quantized_output_array_intq)

/* Tensor #279 */
AI_TENSOR_OBJ_DECLARE(
  conv_9_relu_alpha, AI_STATIC,
  279, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &conv_9_relu_alpha_array, NULL)

/* Tensor #280 */
AI_TENSOR_OBJ_DECLARE(
  conv_9_relu_output, AI_STATIC,
  280, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &conv_9_relu_output_array, NULL)

/* Tensor #281 */
AI_TENSOR_OBJ_DECLARE(
  conv_9_relu_quantized_output, AI_STATIC,
  281, 0x1,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 1, 1, 512, 7168),
  1, &conv_9_relu_quantized_output_array, &conv_9_relu_quantized_output_array_intq)

/* Tensor #282 */
AI_TENSOR_OBJ_DECLARE(
  data_Transpose_output, AI_STATIC,
  282, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 112, 112), AI_STRIDE_INIT(4, 4, 4, 12, 1344),
  1, &data_Transpose_output_array, NULL)

/* Tensor #283 */
AI_TENSOR_OBJ_DECLARE(
  data_output, AI_STATIC,
  283, 0x0,
  AI_SHAPE_INIT(4, 1, 112, 112, 3), AI_STRIDE_INIT(4, 4, 4, 448, 50176),
  1, &data_output_array, NULL)

/* Tensor #284 */
AI_TENSOR_OBJ_DECLARE(
  fc1_bias, AI_STATIC,
  284, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &fc1_bias_array, NULL)

/* Tensor #285 */
AI_TENSOR_OBJ_DECLARE(
  fc1_output, AI_STATIC,
  285, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &fc1_output_array, NULL)

/* Tensor #286 */
AI_TENSOR_OBJ_DECLARE(
  fc1_scale, AI_STATIC,
  286, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &fc1_scale_array, NULL)

/* Tensor #287 */
AI_TENSOR_OBJ_DECLARE(
  flatten_254_to_chlast_output, AI_STATIC,
  287, 0x1,
  AI_SHAPE_INIT(4, 1, 7, 7, 1024), AI_STRIDE_INIT(4, 1, 1, 7, 49),
  1, &flatten_254_to_chlast_output_array, &flatten_254_to_chlast_output_array_intq)

/* Tensor #288 */
AI_TENSOR_OBJ_DECLARE(
  flatten_254_to_chlast_output0, AI_STATIC,
  288, 0x1,
  AI_SHAPE_INIT(4, 1, 50176, 1, 1), AI_STRIDE_INIT(4, 1, 1, 50176, 50176),
  1, &flatten_254_to_chlast_output_array, &flatten_254_to_chlast_output_array_intq)

/* Tensor #289 */
AI_TENSOR_OBJ_DECLARE(
  pre_fc1_MatMul_quantized_output, AI_STATIC,
  289, 0x1,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 1, 1, 128, 128),
  1, &pre_fc1_MatMul_quantized_output_array, &pre_fc1_MatMul_quantized_output_array_intq)

/* Tensor #290 */
AI_TENSOR_OBJ_DECLARE(
  pre_fc1_MatMul_quantized_scratch0, AI_STATIC,
  290, 0x0,
  AI_SHAPE_INIT(4, 1, 50176, 1, 1), AI_STRIDE_INIT(4, 2, 2, 100352, 100352),
  1, &pre_fc1_MatMul_quantized_scratch0_array, NULL)

/* Tensor #291 */
AI_TENSOR_OBJ_DECLARE(
  pre_fc1_MatMul_quantized_weights, AI_STATIC,
  291, 0x1,
  AI_SHAPE_INIT(4, 50176, 128, 1, 1), AI_STRIDE_INIT(4, 1, 50176, 6422528, 6422528),
  1, &pre_fc1_MatMul_quantized_weights_array, &pre_fc1_MatMul_quantized_weights_array_intq)

/* Tensor #292 */
AI_TENSOR_OBJ_DECLARE(
  pre_fc1_bias_quantized, AI_STATIC,
  292, 0x1,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 1, 1, 128, 128),
  1, &pre_fc1_bias_quantized_array, &pre_fc1_bias_quantized_array_intq)

/* Tensor #293 */
AI_TENSOR_OBJ_DECLARE(
  pre_fc1_output, AI_STATIC,
  293, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &pre_fc1_output_array, NULL)

/* Tensor #294 */
AI_TENSOR_OBJ_DECLARE(
  pre_fc1_quantized_output, AI_STATIC,
  294, 0x1,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 1, 1, 128, 128),
  1, &pre_fc1_quantized_output_array, &pre_fc1_quantized_output_array_intq)

/* Tensor #295 */
AI_TENSOR_OBJ_DECLARE(
  scalar_op1_4D, AI_STATIC,
  295, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &scalar_op1_4D_array, NULL)

/* Tensor #296 */
AI_TENSOR_OBJ_DECLARE(
  scalar_op2_quantized_4D, AI_STATIC,
  296, 0x1,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 1, 1, 1, 1),
  1, &scalar_op2_quantized_4D_array, &scalar_op2_quantized_4D_array_intq)



/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  fc1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pre_fc1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &fc1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &fc1_scale, &fc1_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  fc1_layer, 144,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &fc1_chain,
  NULL, &fc1_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  pre_fc1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pre_fc1_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pre_fc1_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  pre_fc1_layer, 143,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &pre_fc1_chain,
  NULL, &fc1_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  pre_fc1_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &pre_fc1_MatMul_quantized_output, &pre_fc1_bias_quantized),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pre_fc1_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  pre_fc1_quantized_layer, 142,
  ELTWISE_INTEGER_TYPE, 0x0, NULL,
  eltwise_integer, forward_eltwise_integer_INT8,
  &pre_fc1_quantized_chain,
  NULL, &pre_fc1_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_INT8, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  pre_fc1_MatMul_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &flatten_254_to_chlast_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pre_fc1_MatMul_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &pre_fc1_MatMul_quantized_weights, &conv_3_conv2d_quantized_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pre_fc1_MatMul_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  pre_fc1_MatMul_quantized_layer, 141,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA,
  &pre_fc1_MatMul_quantized_chain,
  NULL, &pre_fc1_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  flatten_254_to_chlast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &bn1_0_conversion_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &flatten_254_to_chlast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  flatten_254_to_chlast_layer, 139,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &flatten_254_to_chlast_chain,
  NULL, &pre_fc1_MatMul_quantized_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_HEIGHT, AI_SHAPE_CHANNEL, AI_SHAPE_DEPTH, AI_SHAPE_EXTENSION), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  bn1_0_conversion_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &bn1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &bn1_0_conversion_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  bn1_0_conversion_layer, 138,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &bn1_0_conversion_chain,
  NULL, &flatten_254_to_chlast_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  bn1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_14_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &bn1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &bn1_scale, &bn1_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  bn1_layer, 138,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &bn1_chain,
  NULL, &bn1_0_conversion_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_14_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_14_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_14_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_14_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_14_relu_layer, 137,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_14_relu_chain,
  NULL, &bn1_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_14_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_14_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_14_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_14_batchnorm_scale, &conv_14_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_14_batchnorm_layer, 136,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_14_batchnorm_chain,
  NULL, &conv_14_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_14_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_14_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_14_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_14_conv2d_layer, 135,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_14_conv2d_chain,
  NULL, &conv_14_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_14_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_14_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_14_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_14_conv2d_quantized_weights, &conv_13_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_14_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_14_conv2d_quantized_layer, 134,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_14_conv2d_quantized_chain,
  NULL, &conv_14_conv2d_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_14_dw_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_14_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_14_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_14_dw_relu_quantized_layer, 133,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_14_dw_relu_quantized_chain,
  NULL, &conv_14_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_14_dw_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_14_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_14_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_14_dw_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_14_dw_relu_layer, 132,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_14_dw_relu_chain,
  NULL, &conv_14_dw_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_14_dw_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_14_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_14_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_14_dw_batchnorm_scale, &conv_14_dw_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_14_dw_batchnorm_layer, 131,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_14_dw_batchnorm_chain,
  NULL, &conv_14_dw_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_14_dw_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_14_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_14_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_14_dw_conv2d_layer, 130,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_14_dw_conv2d_chain,
  NULL, &conv_14_dw_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_14_dw_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_14_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_14_dw_conv2d_quantized_weights, &conv_13_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_14_dw_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_14_dw_conv2d_quantized_layer, 129,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_14_dw_conv2d_quantized_chain,
  NULL, &conv_14_dw_conv2d_layer, AI_STATIC, 
  .groups = 1024, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_13_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_13_relu_quantized_layer, 128,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_13_relu_quantized_chain,
  NULL, &conv_14_dw_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_13_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_13_relu_layer, 127,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_13_relu_chain,
  NULL, &conv_13_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_13_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_13_batchnorm_scale, &conv_13_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_13_batchnorm_layer, 126,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_13_batchnorm_chain,
  NULL, &conv_13_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_13_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_13_conv2d_layer, 125,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_13_conv2d_chain,
  NULL, &conv_13_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_13_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_13_conv2d_quantized_weights, &conv_13_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_13_conv2d_quantized_layer, 124,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_13_conv2d_quantized_chain,
  NULL, &conv_13_conv2d_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_13_dw_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_13_dw_relu_quantized_layer, 123,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_13_dw_relu_quantized_chain,
  NULL, &conv_13_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_13_dw_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_dw_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_13_dw_relu_layer, 122,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_13_dw_relu_chain,
  NULL, &conv_13_dw_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_13_dw_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_13_dw_batchnorm_scale, &conv_13_dw_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_13_dw_batchnorm_layer, 121,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_13_dw_batchnorm_chain,
  NULL, &conv_13_dw_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_13_dw_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_13_dw_conv2d_layer, 120,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_13_dw_conv2d_chain,
  NULL, &conv_13_dw_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_13_dw_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_13_dw_conv2d_quantized_weights, &conv_7_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_13_dw_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_13_dw_conv2d_quantized_layer, 119,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_13_dw_conv2d_quantized_chain,
  NULL, &conv_13_dw_conv2d_layer, AI_STATIC, 
  .groups = 512, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_12_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_12_relu_quantized_layer, 118,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_12_relu_quantized_chain,
  NULL, &conv_13_dw_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_12_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_12_relu_layer, 117,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_12_relu_chain,
  NULL, &conv_12_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_12_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_12_batchnorm_scale, &conv_12_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_12_batchnorm_layer, 116,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_12_batchnorm_chain,
  NULL, &conv_12_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_12_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_12_conv2d_layer, 115,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_12_conv2d_chain,
  NULL, &conv_12_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_12_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_12_conv2d_quantized_weights, &conv_7_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_12_conv2d_quantized_layer, 114,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_12_conv2d_quantized_chain,
  NULL, &conv_12_conv2d_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_12_dw_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_12_dw_relu_quantized_layer, 113,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_12_dw_relu_quantized_chain,
  NULL, &conv_12_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_12_dw_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_dw_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_12_dw_relu_layer, 112,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_12_dw_relu_chain,
  NULL, &conv_12_dw_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_12_dw_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_12_dw_batchnorm_scale, &conv_12_dw_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_12_dw_batchnorm_layer, 111,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_12_dw_batchnorm_chain,
  NULL, &conv_12_dw_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_12_dw_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_12_dw_conv2d_layer, 110,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_12_dw_conv2d_chain,
  NULL, &conv_12_dw_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_12_dw_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_12_dw_conv2d_quantized_weights, &conv_7_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_12_dw_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_12_dw_conv2d_quantized_layer, 109,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_12_dw_conv2d_quantized_chain,
  NULL, &conv_12_dw_conv2d_layer, AI_STATIC, 
  .groups = 512, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_11_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_11_relu_quantized_layer, 108,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_11_relu_quantized_chain,
  NULL, &conv_12_dw_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_11_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_11_relu_layer, 107,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_11_relu_chain,
  NULL, &conv_11_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_11_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_11_batchnorm_scale, &conv_11_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_11_batchnorm_layer, 106,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_11_batchnorm_chain,
  NULL, &conv_11_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_11_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_11_conv2d_layer, 105,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_11_conv2d_chain,
  NULL, &conv_11_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_11_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_11_conv2d_quantized_weights, &conv_7_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_11_conv2d_quantized_layer, 104,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_11_conv2d_quantized_chain,
  NULL, &conv_11_conv2d_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_11_dw_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_11_dw_relu_quantized_layer, 103,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_11_dw_relu_quantized_chain,
  NULL, &conv_11_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_11_dw_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_dw_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_11_dw_relu_layer, 102,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_11_dw_relu_chain,
  NULL, &conv_11_dw_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_11_dw_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_11_dw_batchnorm_scale, &conv_11_dw_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_11_dw_batchnorm_layer, 101,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_11_dw_batchnorm_chain,
  NULL, &conv_11_dw_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_11_dw_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_11_dw_conv2d_layer, 100,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_11_dw_conv2d_chain,
  NULL, &conv_11_dw_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_11_dw_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_11_dw_conv2d_quantized_weights, &conv_7_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_11_dw_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_11_dw_conv2d_quantized_layer, 99,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_11_dw_conv2d_quantized_chain,
  NULL, &conv_11_dw_conv2d_layer, AI_STATIC, 
  .groups = 512, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_10_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_10_relu_quantized_layer, 98,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_10_relu_quantized_chain,
  NULL, &conv_11_dw_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_10_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_10_relu_layer, 97,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_10_relu_chain,
  NULL, &conv_10_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_10_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_10_batchnorm_scale, &conv_10_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_10_batchnorm_layer, 96,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_10_batchnorm_chain,
  NULL, &conv_10_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_10_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_10_conv2d_layer, 95,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_10_conv2d_chain,
  NULL, &conv_10_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_10_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_10_conv2d_quantized_weights, &conv_7_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_10_conv2d_quantized_layer, 94,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_10_conv2d_quantized_chain,
  NULL, &conv_10_conv2d_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_10_dw_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_10_dw_relu_quantized_layer, 93,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_10_dw_relu_quantized_chain,
  NULL, &conv_10_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_10_dw_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_dw_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_10_dw_relu_layer, 92,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_10_dw_relu_chain,
  NULL, &conv_10_dw_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_10_dw_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_10_dw_batchnorm_scale, &conv_10_dw_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_10_dw_batchnorm_layer, 91,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_10_dw_batchnorm_chain,
  NULL, &conv_10_dw_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_10_dw_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_10_dw_conv2d_layer, 90,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_10_dw_conv2d_chain,
  NULL, &conv_10_dw_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_10_dw_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_10_dw_conv2d_quantized_weights, &conv_7_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_10_dw_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_10_dw_conv2d_quantized_layer, 89,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_10_dw_conv2d_quantized_chain,
  NULL, &conv_10_dw_conv2d_layer, AI_STATIC, 
  .groups = 512, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_9_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_9_relu_quantized_layer, 88,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_9_relu_quantized_chain,
  NULL, &conv_10_dw_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_9_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_9_relu_layer, 87,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_9_relu_chain,
  NULL, &conv_9_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_9_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_9_batchnorm_scale, &conv_9_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_9_batchnorm_layer, 86,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_9_batchnorm_chain,
  NULL, &conv_9_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_9_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_9_conv2d_layer, 85,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_9_conv2d_chain,
  NULL, &conv_9_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_9_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_9_conv2d_quantized_weights, &conv_7_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_9_conv2d_quantized_layer, 84,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_9_conv2d_quantized_chain,
  NULL, &conv_9_conv2d_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_9_dw_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_9_dw_relu_quantized_layer, 83,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_9_dw_relu_quantized_chain,
  NULL, &conv_9_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_9_dw_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_dw_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_9_dw_relu_layer, 82,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_9_dw_relu_chain,
  NULL, &conv_9_dw_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_9_dw_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_9_dw_batchnorm_scale, &conv_9_dw_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_9_dw_batchnorm_layer, 81,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_9_dw_batchnorm_chain,
  NULL, &conv_9_dw_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_9_dw_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_9_dw_conv2d_layer, 80,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_9_dw_conv2d_chain,
  NULL, &conv_9_dw_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_9_dw_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_9_dw_conv2d_quantized_weights, &conv_7_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_9_dw_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_9_dw_conv2d_quantized_layer, 79,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_9_dw_conv2d_quantized_chain,
  NULL, &conv_9_dw_conv2d_layer, AI_STATIC, 
  .groups = 512, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_8_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_8_relu_quantized_layer, 78,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_8_relu_quantized_chain,
  NULL, &conv_9_dw_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_8_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_8_relu_layer, 77,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_8_relu_chain,
  NULL, &conv_8_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_8_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_8_batchnorm_scale, &conv_8_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_8_batchnorm_layer, 76,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_8_batchnorm_chain,
  NULL, &conv_8_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_8_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_8_conv2d_layer, 75,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_8_conv2d_chain,
  NULL, &conv_8_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_8_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_8_conv2d_quantized_weights, &conv_7_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_8_conv2d_quantized_layer, 74,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_8_conv2d_quantized_chain,
  NULL, &conv_8_conv2d_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_8_dw_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_8_dw_relu_quantized_layer, 73,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_8_dw_relu_quantized_chain,
  NULL, &conv_8_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_8_dw_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_dw_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_8_dw_relu_layer, 72,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_8_dw_relu_chain,
  NULL, &conv_8_dw_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_8_dw_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_8_dw_batchnorm_scale, &conv_8_dw_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_8_dw_batchnorm_layer, 71,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_8_dw_batchnorm_chain,
  NULL, &conv_8_dw_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_8_dw_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_8_dw_conv2d_layer, 70,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_8_dw_conv2d_chain,
  NULL, &conv_8_dw_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_8_dw_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_8_dw_conv2d_quantized_weights, &conv_7_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_8_dw_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_8_dw_conv2d_quantized_layer, 69,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_8_dw_conv2d_quantized_chain,
  NULL, &conv_8_dw_conv2d_layer, AI_STATIC, 
  .groups = 512, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_7_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_7_relu_quantized_layer, 68,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_7_relu_quantized_chain,
  NULL, &conv_8_dw_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_7_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_7_relu_layer, 67,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_7_relu_chain,
  NULL, &conv_7_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_7_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_7_batchnorm_scale, &conv_7_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_7_batchnorm_layer, 66,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_7_batchnorm_chain,
  NULL, &conv_7_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_7_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_7_conv2d_layer, 65,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_7_conv2d_chain,
  NULL, &conv_7_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_7_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_7_conv2d_quantized_weights, &conv_7_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_7_conv2d_quantized_layer, 64,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_7_conv2d_quantized_chain,
  NULL, &conv_7_conv2d_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_7_dw_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_7_dw_relu_quantized_layer, 63,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_7_dw_relu_quantized_chain,
  NULL, &conv_7_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_7_dw_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_dw_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_7_dw_relu_layer, 62,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_7_dw_relu_chain,
  NULL, &conv_7_dw_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_7_dw_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_7_dw_batchnorm_scale, &conv_7_dw_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_7_dw_batchnorm_layer, 61,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_7_dw_batchnorm_chain,
  NULL, &conv_7_dw_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_7_dw_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_7_dw_conv2d_layer, 60,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_7_dw_conv2d_chain,
  NULL, &conv_7_dw_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_7_dw_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_7_dw_conv2d_quantized_weights, &conv_5_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_7_dw_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_7_dw_conv2d_quantized_layer, 59,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_7_dw_conv2d_quantized_chain,
  NULL, &conv_7_dw_conv2d_layer, AI_STATIC, 
  .groups = 256, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_6_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_6_relu_quantized_layer, 58,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_6_relu_quantized_chain,
  NULL, &conv_7_dw_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_6_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_6_relu_layer, 57,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_6_relu_chain,
  NULL, &conv_6_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_6_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_6_batchnorm_scale, &conv_6_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_6_batchnorm_layer, 56,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_6_batchnorm_chain,
  NULL, &conv_6_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_6_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_6_conv2d_layer, 55,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_6_conv2d_chain,
  NULL, &conv_6_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_6_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_6_conv2d_quantized_weights, &conv_5_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_6_conv2d_quantized_layer, 54,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_6_conv2d_quantized_chain,
  NULL, &conv_6_conv2d_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_6_dw_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_6_dw_relu_quantized_layer, 53,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_6_dw_relu_quantized_chain,
  NULL, &conv_6_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_6_dw_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_dw_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_6_dw_relu_layer, 52,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_6_dw_relu_chain,
  NULL, &conv_6_dw_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_6_dw_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_6_dw_batchnorm_scale, &conv_6_dw_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_6_dw_batchnorm_layer, 51,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_6_dw_batchnorm_chain,
  NULL, &conv_6_dw_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_6_dw_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_6_dw_conv2d_layer, 50,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_6_dw_conv2d_chain,
  NULL, &conv_6_dw_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_6_dw_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_6_dw_conv2d_quantized_weights, &conv_5_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_6_dw_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_6_dw_conv2d_quantized_layer, 49,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_6_dw_conv2d_quantized_chain,
  NULL, &conv_6_dw_conv2d_layer, AI_STATIC, 
  .groups = 256, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_5_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_5_relu_quantized_layer, 48,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_5_relu_quantized_chain,
  NULL, &conv_6_dw_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_5_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_5_relu_layer, 47,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_5_relu_chain,
  NULL, &conv_5_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_5_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_5_batchnorm_scale, &conv_5_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_5_batchnorm_layer, 46,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_5_batchnorm_chain,
  NULL, &conv_5_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_5_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_5_conv2d_layer, 45,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_5_conv2d_chain,
  NULL, &conv_5_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_5_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_5_conv2d_quantized_weights, &conv_5_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_5_conv2d_quantized_layer, 44,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_5_conv2d_quantized_chain,
  NULL, &conv_5_conv2d_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_5_dw_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_5_dw_relu_quantized_layer, 43,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_5_dw_relu_quantized_chain,
  NULL, &conv_5_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_5_dw_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_dw_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_5_dw_relu_layer, 42,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_5_dw_relu_chain,
  NULL, &conv_5_dw_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_5_dw_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_5_dw_batchnorm_scale, &conv_5_dw_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_5_dw_batchnorm_layer, 41,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_5_dw_batchnorm_chain,
  NULL, &conv_5_dw_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_5_dw_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_5_dw_conv2d_layer, 40,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_5_dw_conv2d_chain,
  NULL, &conv_5_dw_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_5_dw_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_5_dw_conv2d_quantized_weights, &conv_3_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_5_dw_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_5_dw_conv2d_quantized_layer, 39,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_5_dw_conv2d_quantized_chain,
  NULL, &conv_5_dw_conv2d_layer, AI_STATIC, 
  .groups = 128, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_4_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_4_relu_quantized_layer, 38,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_4_relu_quantized_chain,
  NULL, &conv_5_dw_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_4_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_4_relu_layer, 37,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_4_relu_chain,
  NULL, &conv_4_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_4_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_4_batchnorm_scale, &conv_4_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_4_batchnorm_layer, 36,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_4_batchnorm_chain,
  NULL, &conv_4_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_4_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_4_conv2d_layer, 35,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_4_conv2d_chain,
  NULL, &conv_4_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_4_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_4_conv2d_quantized_weights, &conv_3_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_4_conv2d_quantized_layer, 34,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_4_conv2d_quantized_chain,
  NULL, &conv_4_conv2d_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_4_dw_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_4_dw_relu_quantized_layer, 33,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_4_dw_relu_quantized_chain,
  NULL, &conv_4_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_4_dw_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_dw_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_4_dw_relu_layer, 32,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_4_dw_relu_chain,
  NULL, &conv_4_dw_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_4_dw_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_4_dw_batchnorm_scale, &conv_4_dw_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_4_dw_batchnorm_layer, 31,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_4_dw_batchnorm_chain,
  NULL, &conv_4_dw_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_4_dw_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_4_dw_conv2d_layer, 30,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_4_dw_conv2d_chain,
  NULL, &conv_4_dw_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_4_dw_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_4_dw_conv2d_quantized_weights, &conv_3_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_4_dw_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_4_dw_conv2d_quantized_layer, 29,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_4_dw_conv2d_quantized_chain,
  NULL, &conv_4_dw_conv2d_layer, AI_STATIC, 
  .groups = 128, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_3_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_3_relu_quantized_layer, 28,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_3_relu_quantized_chain,
  NULL, &conv_4_dw_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_3_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_3_relu_layer, 27,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_3_relu_chain,
  NULL, &conv_3_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_3_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_3_batchnorm_scale, &conv_3_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_3_batchnorm_layer, 26,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_3_batchnorm_chain,
  NULL, &conv_3_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_3_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_3_conv2d_layer, 25,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_3_conv2d_chain,
  NULL, &conv_3_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_3_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_3_conv2d_quantized_weights, &conv_3_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_3_conv2d_quantized_layer, 24,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_3_conv2d_quantized_chain,
  NULL, &conv_3_conv2d_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_3_dw_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_3_dw_relu_quantized_layer, 23,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_3_dw_relu_quantized_chain,
  NULL, &conv_3_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_3_dw_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_dw_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_3_dw_relu_layer, 22,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_3_dw_relu_chain,
  NULL, &conv_3_dw_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_3_dw_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_3_dw_batchnorm_scale, &conv_3_dw_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_3_dw_batchnorm_layer, 21,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_3_dw_batchnorm_chain,
  NULL, &conv_3_dw_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_3_dw_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_3_dw_conv2d_layer, 20,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_3_dw_conv2d_chain,
  NULL, &conv_3_dw_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_3_dw_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_3_dw_conv2d_quantized_weights, &conv_2_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_3_dw_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_3_dw_conv2d_quantized_layer, 19,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_3_dw_conv2d_quantized_chain,
  NULL, &conv_3_dw_conv2d_layer, AI_STATIC, 
  .groups = 64, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_2_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_2_relu_quantized_layer, 18,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_2_relu_quantized_chain,
  NULL, &conv_3_dw_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_2_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_2_relu_layer, 17,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_2_relu_chain,
  NULL, &conv_2_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_2_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_2_batchnorm_scale, &conv_2_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_2_batchnorm_layer, 16,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_2_batchnorm_chain,
  NULL, &conv_2_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_2_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_2_conv2d_layer, 15,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_2_conv2d_chain,
  NULL, &conv_2_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_2_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_2_conv2d_quantized_weights, &conv_2_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_2_conv2d_quantized_layer, 14,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_2_conv2d_quantized_chain,
  NULL, &conv_2_conv2d_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_2_dw_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_dw_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_2_dw_relu_quantized_layer, 13,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_2_dw_relu_quantized_chain,
  NULL, &conv_2_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_2_dw_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_dw_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_dw_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_2_dw_relu_layer, 12,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_2_dw_relu_chain,
  NULL, &conv_2_dw_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_2_dw_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_dw_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_2_dw_batchnorm_scale, &conv_2_dw_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_2_dw_batchnorm_layer, 11,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_2_dw_batchnorm_chain,
  NULL, &conv_2_dw_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_2_dw_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_dw_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_2_dw_conv2d_layer, 10,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_2_dw_conv2d_chain,
  NULL, &conv_2_dw_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_2_dw_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_1_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_dw_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_2_dw_conv2d_quantized_weights, &conv_1_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_2_dw_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_2_dw_conv2d_quantized_layer, 9,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_2_dw_conv2d_quantized_chain,
  NULL, &conv_2_dw_conv2d_layer, AI_STATIC, 
  .groups = 32, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_1_relu_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_1_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_1_relu_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_1_relu_quantized_layer, 8,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_1_relu_quantized_chain,
  NULL, &conv_2_dw_conv2d_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_1_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_1_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_1_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_1_relu_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_1_relu_layer, 7,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &conv_1_relu_chain,
  NULL, &conv_1_relu_quantized_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_1_batchnorm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_1_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_1_batchnorm_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv_1_batchnorm_scale, &conv_1_batchnorm_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_1_batchnorm_layer, 6,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &conv_1_batchnorm_chain,
  NULL, &conv_1_relu_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_1_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_1_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_1_conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv_1_conv2d_layer, 5,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conv_1_conv2d_chain,
  NULL, &conv_1_batchnorm_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv_1_conv2d_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_mulscalar0_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_1_conv2d_quantized_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv_1_conv2d_quantized_weights, &conv_1_conv2d_quantized_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv_1_conv2d_quantized_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv_1_conv2d_quantized_layer, 4,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_integer_SSSA,
  &conv_1_conv2d_quantized_chain,
  NULL, &conv_1_conv2d_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _mulscalar0_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_minusscalar0_quantized_output, &scalar_op2_quantized_4D),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_mulscalar0_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _mulscalar0_quantized_layer, 3,
  ELTWISE_INTEGER_TYPE, 0x0, NULL,
  eltwise_integer, forward_eltwise_integer_INT8,
  &_mulscalar0_quantized_chain,
  NULL, &conv_1_conv2d_quantized_layer, AI_STATIC, 
  .operation = ai_mul_f32, 
  .buffer_operation = ai_mul_buffer_INT8, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _minusscalar0_quantized_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_minusscalar0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_minusscalar0_quantized_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _minusscalar0_quantized_layer, 2,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &_minusscalar0_quantized_chain,
  NULL, &_mulscalar0_quantized_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _minusscalar0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &data_Transpose_output, &scalar_op1_4D),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_minusscalar0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _minusscalar0_layer, 1,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &_minusscalar0_chain,
  NULL, &_minusscalar0_quantized_layer, AI_STATIC, 
  .operation = ai_sub_f32, 
  .buffer_operation = ai_sub_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  data_Transpose_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &data_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &data_Transpose_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  data_Transpose_layer, 2,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &data_Transpose_chain,
  NULL, &_minusscalar0_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_HEIGHT, AI_SHAPE_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_DEPTH, AI_SHAPE_EXTENSION), 
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 9756468, 1, 1),
    9756468, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 3211264, 1, 1),
    3211264, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_SFACE_IN_NUM, &data_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_SFACE_OUT_NUM, &fc1_output),
  &data_Transpose_layer, 0xaf7f5448, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 9756468, 1, 1),
      9756468, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 3211264, 1, 1),
      3211264, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_SFACE_IN_NUM, &data_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_SFACE_OUT_NUM, &fc1_output),
  &data_Transpose_layer, 0xaf7f5448, NULL)

#endif	/*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/



/******************************************************************************/
AI_DECLARE_STATIC
ai_bool sface_configure_activations(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_activations_map(g_sface_activations_map, 1, params)) {
    /* Updating activations (byte) offsets */
    
    data_output_array.data = AI_PTR(g_sface_activations_map[0] + 2119936);
    data_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 2119936);
    data_Transpose_output_array.data = AI_PTR(g_sface_activations_map[0] + 1969408);
    data_Transpose_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 1969408);
    _minusscalar0_output_array.data = AI_PTR(g_sface_activations_map[0] + 1969408);
    _minusscalar0_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 1969408);
    _minusscalar0_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 1969408);
    _minusscalar0_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 1969408);
    _mulscalar0_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 1969404);
    _mulscalar0_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 1969404);
    conv_1_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 1967440);
    conv_1_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 1967440);
    conv_1_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 2007040);
    conv_1_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 2007040);
    conv_1_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 802816);
    conv_1_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 802816);
    conv_1_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 802816);
    conv_1_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 802816);
    conv_1_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 802816);
    conv_1_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 802816);
    conv_1_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 802816);
    conv_1_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 802816);
    conv_2_dw_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 2407580);
    conv_2_dw_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 2407580);
    conv_2_dw_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 2006172);
    conv_2_dw_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 2006172);
    conv_2_dw_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 801948);
    conv_2_dw_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 801948);
    conv_2_dw_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 801948);
    conv_2_dw_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 801948);
    conv_2_dw_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 801948);
    conv_2_dw_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 801948);
    conv_2_dw_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 400540);
    conv_2_dw_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 400540);
    conv_2_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 2408320);
    conv_2_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 2408320);
    conv_2_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 2408448);
    conv_2_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 2408448);
    conv_2_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_2_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_2_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_2_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_2_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_2_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_2_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_2_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_3_dw_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 802816);
    conv_3_dw_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 802816);
    conv_3_dw_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 804548);
    conv_3_dw_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 804548);
    conv_3_dw_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_3_dw_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_3_dw_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 802816);
    conv_3_dw_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 802816);
    conv_3_dw_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_3_dw_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_3_dw_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 802816);
    conv_3_dw_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 802816);
    conv_3_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_3_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_3_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 256);
    conv_3_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 256);
    conv_3_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 401664);
    conv_3_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 401664);
    conv_3_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 401664);
    conv_3_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 401664);
    conv_3_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 401664);
    conv_3_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 401664);
    conv_3_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_3_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_4_dw_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_4_dw_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_4_dw_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 404868);
    conv_4_dw_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 404868);
    conv_4_dw_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 806276);
    conv_4_dw_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 806276);
    conv_4_dw_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 806276);
    conv_4_dw_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 806276);
    conv_4_dw_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 806276);
    conv_4_dw_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 806276);
    conv_4_dw_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_4_dw_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_4_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_4_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_4_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 401920);
    conv_4_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 401920);
    conv_4_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 803328);
    conv_4_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 803328);
    conv_4_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 803328);
    conv_4_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 803328);
    conv_4_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 803328);
    conv_4_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 803328);
    conv_4_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_4_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_5_dw_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_5_dw_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_5_dw_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 404868);
    conv_5_dw_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 404868);
    conv_5_dw_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_5_dw_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_5_dw_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_5_dw_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_5_dw_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_5_dw_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_5_dw_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_5_dw_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_5_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_5_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_5_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 512);
    conv_5_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 512);
    conv_5_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 201216);
    conv_5_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 201216);
    conv_5_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 1004032);
    conv_5_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 1004032);
    conv_5_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_5_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_5_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 802816);
    conv_5_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 802816);
    conv_6_dw_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_6_dw_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_6_dw_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 6916);
    conv_6_dw_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 6916);
    conv_6_dw_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 207620);
    conv_6_dw_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 207620);
    conv_6_dw_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 1010436);
    conv_6_dw_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 1010436);
    conv_6_dw_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_6_dw_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_6_dw_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 802816);
    conv_6_dw_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 802816);
    conv_6_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_6_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_6_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 1024);
    conv_6_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 1024);
    conv_6_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 201728);
    conv_6_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 201728);
    conv_6_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 1004544);
    conv_6_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 1004544);
    conv_6_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_6_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_6_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 802816);
    conv_6_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 802816);
    conv_7_dw_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_7_dw_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_7_dw_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 6916);
    conv_7_dw_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 6916);
    conv_7_dw_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 57092);
    conv_7_dw_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 57092);
    conv_7_dw_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 257796);
    conv_7_dw_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 257796);
    conv_7_dw_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_7_dw_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_7_dw_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 200704);
    conv_7_dw_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 200704);
    conv_7_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_7_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_7_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 1024);
    conv_7_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 1024);
    conv_7_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 101376);
    conv_7_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 101376);
    conv_7_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 502784);
    conv_7_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 502784);
    conv_7_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_7_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_7_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_7_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_8_dw_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_8_dw_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_8_dw_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 13828);
    conv_8_dw_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 13828);
    conv_8_dw_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 114180);
    conv_8_dw_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 114180);
    conv_8_dw_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 515588);
    conv_8_dw_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 515588);
    conv_8_dw_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_8_dw_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_8_dw_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_8_dw_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_8_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_8_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_8_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 2048);
    conv_8_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 2048);
    conv_8_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 102400);
    conv_8_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 102400);
    conv_8_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 503808);
    conv_8_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 503808);
    conv_8_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_8_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_8_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_8_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_9_dw_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_9_dw_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_9_dw_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 13828);
    conv_9_dw_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 13828);
    conv_9_dw_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 114180);
    conv_9_dw_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 114180);
    conv_9_dw_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 515588);
    conv_9_dw_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 515588);
    conv_9_dw_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_9_dw_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_9_dw_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_9_dw_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_9_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_9_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_9_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 2048);
    conv_9_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 2048);
    conv_9_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 102400);
    conv_9_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 102400);
    conv_9_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 503808);
    conv_9_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 503808);
    conv_9_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_9_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_9_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_9_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_10_dw_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_10_dw_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_10_dw_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 13828);
    conv_10_dw_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 13828);
    conv_10_dw_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 114180);
    conv_10_dw_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 114180);
    conv_10_dw_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 515588);
    conv_10_dw_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 515588);
    conv_10_dw_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_10_dw_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_10_dw_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_10_dw_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_10_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_10_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_10_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 2048);
    conv_10_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 2048);
    conv_10_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 102400);
    conv_10_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 102400);
    conv_10_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 503808);
    conv_10_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 503808);
    conv_10_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_10_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_10_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_10_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_11_dw_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_11_dw_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_11_dw_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 13828);
    conv_11_dw_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 13828);
    conv_11_dw_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 114180);
    conv_11_dw_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 114180);
    conv_11_dw_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 515588);
    conv_11_dw_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 515588);
    conv_11_dw_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_11_dw_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_11_dw_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_11_dw_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_11_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_11_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_11_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 2048);
    conv_11_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 2048);
    conv_11_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 102400);
    conv_11_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 102400);
    conv_11_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 503808);
    conv_11_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 503808);
    conv_11_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_11_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_11_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_11_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_12_dw_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_12_dw_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_12_dw_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 13828);
    conv_12_dw_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 13828);
    conv_12_dw_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 114180);
    conv_12_dw_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 114180);
    conv_12_dw_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 515588);
    conv_12_dw_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 515588);
    conv_12_dw_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_12_dw_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_12_dw_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_12_dw_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_12_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_12_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_12_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 2048);
    conv_12_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 2048);
    conv_12_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 102400);
    conv_12_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 102400);
    conv_12_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 503808);
    conv_12_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 503808);
    conv_12_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_12_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_12_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_12_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 401408);
    conv_13_dw_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_13_dw_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_13_dw_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 13828);
    conv_13_dw_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 13828);
    conv_13_dw_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 38916);
    conv_13_dw_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 38916);
    conv_13_dw_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 139268);
    conv_13_dw_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 139268);
    conv_13_dw_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_13_dw_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_13_dw_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 100352);
    conv_13_dw_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 100352);
    conv_13_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_13_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_13_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 2048);
    conv_13_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 2048);
    conv_13_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 52224);
    conv_13_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 52224);
    conv_13_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 252928);
    conv_13_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 252928);
    conv_13_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_13_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_13_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 200704);
    conv_13_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 200704);
    conv_14_dw_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_14_dw_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_14_dw_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 27652);
    conv_14_dw_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 27652);
    conv_14_dw_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 77828);
    conv_14_dw_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 77828);
    conv_14_dw_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 278532);
    conv_14_dw_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 278532);
    conv_14_dw_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_14_dw_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_14_dw_relu_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 200704);
    conv_14_dw_relu_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 200704);
    conv_14_conv2d_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_14_conv2d_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    conv_14_conv2d_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 4096);
    conv_14_conv2d_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 4096);
    conv_14_conv2d_output_array.data = AI_PTR(g_sface_activations_map[0] + 54272);
    conv_14_conv2d_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 54272);
    conv_14_batchnorm_output_array.data = AI_PTR(g_sface_activations_map[0] + 254976);
    conv_14_batchnorm_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 254976);
    conv_14_relu_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    conv_14_relu_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    bn1_output_array.data = AI_PTR(g_sface_activations_map[0] + 200704);
    bn1_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 200704);
    bn1_0_conversion_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    bn1_0_conversion_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    flatten_254_to_chlast_output_array.data = AI_PTR(g_sface_activations_map[0] + 50176);
    flatten_254_to_chlast_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 50176);
    pre_fc1_MatMul_quantized_scratch0_array.data = AI_PTR(g_sface_activations_map[0] + 100352);
    pre_fc1_MatMul_quantized_scratch0_array.data_start = AI_PTR(g_sface_activations_map[0] + 100352);
    pre_fc1_MatMul_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 0);
    pre_fc1_MatMul_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 0);
    pre_fc1_quantized_output_array.data = AI_PTR(g_sface_activations_map[0] + 128);
    pre_fc1_quantized_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 128);
    pre_fc1_output_array.data = AI_PTR(g_sface_activations_map[0] + 256);
    pre_fc1_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 256);
    fc1_output_array.data = AI_PTR(g_sface_activations_map[0] + 768);
    fc1_output_array.data_start = AI_PTR(g_sface_activations_map[0] + 768);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_ACTIVATIONS);
  return false;
}




/******************************************************************************/
AI_DECLARE_STATIC
ai_bool sface_configure_weights(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_weights_map(g_sface_weights_map, 1, params)) {
    /* Updating weights (byte) offsets */
    
    scalar_op1_4D_array.format |= AI_FMT_FLAG_CONST;
    scalar_op1_4D_array.data = AI_PTR(g_sface_weights_map[0] + 0);
    scalar_op1_4D_array.data_start = AI_PTR(g_sface_weights_map[0] + 0);
    scalar_op2_quantized_4D_array.format |= AI_FMT_FLAG_CONST;
    scalar_op2_quantized_4D_array.data = AI_PTR(g_sface_weights_map[0] + 4);
    scalar_op2_quantized_4D_array.data_start = AI_PTR(g_sface_weights_map[0] + 4);
    conv_1_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_1_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 8);
    conv_1_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 8);
    conv_1_conv2d_quantized_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_1_conv2d_quantized_bias_array.data = AI_PTR(g_sface_weights_map[0] + 872);
    conv_1_conv2d_quantized_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 872);
    conv_1_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_1_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 1000);
    conv_1_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 1000);
    conv_1_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_1_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 1128);
    conv_1_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 1128);
    conv_1_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_1_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 1256);
    conv_1_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 1256);
    conv_2_dw_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_2_dw_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 1388);
    conv_2_dw_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 1388);
    conv_2_dw_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_2_dw_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 1676);
    conv_2_dw_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 1676);
    conv_2_dw_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_2_dw_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 1804);
    conv_2_dw_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 1804);
    conv_2_dw_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_2_dw_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 1932);
    conv_2_dw_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 1932);
    conv_2_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_2_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 2064);
    conv_2_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 2064);
    conv_2_conv2d_quantized_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_2_conv2d_quantized_bias_array.data = AI_PTR(g_sface_weights_map[0] + 4112);
    conv_2_conv2d_quantized_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 4112);
    conv_2_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_2_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 4368);
    conv_2_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 4368);
    conv_2_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_2_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 4624);
    conv_2_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 4624);
    conv_2_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_2_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 4880);
    conv_2_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 4880);
    conv_3_dw_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_3_dw_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 5140);
    conv_3_dw_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 5140);
    conv_3_dw_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_3_dw_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 5716);
    conv_3_dw_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 5716);
    conv_3_dw_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_3_dw_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 5972);
    conv_3_dw_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 5972);
    conv_3_dw_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_3_dw_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 6228);
    conv_3_dw_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 6228);
    conv_3_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_3_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 6488);
    conv_3_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 6488);
    conv_3_conv2d_quantized_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_3_conv2d_quantized_bias_array.data = AI_PTR(g_sface_weights_map[0] + 14680);
    conv_3_conv2d_quantized_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 14680);
    conv_3_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_3_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 15192);
    conv_3_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 15192);
    conv_3_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_3_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 15704);
    conv_3_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 15704);
    conv_3_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_3_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 16216);
    conv_3_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 16216);
    conv_4_dw_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_4_dw_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 16732);
    conv_4_dw_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 16732);
    conv_4_dw_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_4_dw_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 17884);
    conv_4_dw_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 17884);
    conv_4_dw_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_4_dw_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 18396);
    conv_4_dw_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 18396);
    conv_4_dw_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_4_dw_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 18908);
    conv_4_dw_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 18908);
    conv_4_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_4_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 19424);
    conv_4_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 19424);
    conv_4_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_4_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 35808);
    conv_4_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 35808);
    conv_4_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_4_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 36320);
    conv_4_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 36320);
    conv_4_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_4_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 36832);
    conv_4_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 36832);
    conv_5_dw_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_5_dw_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 37348);
    conv_5_dw_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 37348);
    conv_5_dw_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_5_dw_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 38500);
    conv_5_dw_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 38500);
    conv_5_dw_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_5_dw_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 39012);
    conv_5_dw_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 39012);
    conv_5_dw_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_5_dw_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 39524);
    conv_5_dw_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 39524);
    conv_5_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_5_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 40040);
    conv_5_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 40040);
    conv_5_conv2d_quantized_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_5_conv2d_quantized_bias_array.data = AI_PTR(g_sface_weights_map[0] + 72808);
    conv_5_conv2d_quantized_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 72808);
    conv_5_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_5_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 73832);
    conv_5_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 73832);
    conv_5_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_5_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 74856);
    conv_5_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 74856);
    conv_5_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_5_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 75880);
    conv_5_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 75880);
    conv_6_dw_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_6_dw_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 76908);
    conv_6_dw_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 76908);
    conv_6_dw_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_6_dw_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 79212);
    conv_6_dw_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 79212);
    conv_6_dw_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_6_dw_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 80236);
    conv_6_dw_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 80236);
    conv_6_dw_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_6_dw_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 81260);
    conv_6_dw_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 81260);
    conv_6_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_6_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 82288);
    conv_6_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 82288);
    conv_6_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_6_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 147824);
    conv_6_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 147824);
    conv_6_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_6_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 148848);
    conv_6_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 148848);
    conv_6_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_6_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 149872);
    conv_6_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 149872);
    conv_7_dw_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_7_dw_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 150900);
    conv_7_dw_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 150900);
    conv_7_dw_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_7_dw_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 153204);
    conv_7_dw_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 153204);
    conv_7_dw_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_7_dw_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 154228);
    conv_7_dw_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 154228);
    conv_7_dw_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_7_dw_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 155252);
    conv_7_dw_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 155252);
    conv_7_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_7_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 156280);
    conv_7_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 156280);
    conv_7_conv2d_quantized_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_7_conv2d_quantized_bias_array.data = AI_PTR(g_sface_weights_map[0] + 287352);
    conv_7_conv2d_quantized_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 287352);
    conv_7_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_7_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 289400);
    conv_7_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 289400);
    conv_7_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_7_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 291448);
    conv_7_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 291448);
    conv_7_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_7_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 293496);
    conv_7_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 293496);
    conv_8_dw_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_8_dw_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 295548);
    conv_8_dw_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 295548);
    conv_8_dw_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_8_dw_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 300156);
    conv_8_dw_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 300156);
    conv_8_dw_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_8_dw_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 302204);
    conv_8_dw_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 302204);
    conv_8_dw_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_8_dw_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 304252);
    conv_8_dw_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 304252);
    conv_8_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_8_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 306304);
    conv_8_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 306304);
    conv_8_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_8_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 568448);
    conv_8_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 568448);
    conv_8_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_8_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 570496);
    conv_8_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 570496);
    conv_8_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_8_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 572544);
    conv_8_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 572544);
    conv_9_dw_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_9_dw_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 574596);
    conv_9_dw_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 574596);
    conv_9_dw_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_9_dw_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 579204);
    conv_9_dw_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 579204);
    conv_9_dw_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_9_dw_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 581252);
    conv_9_dw_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 581252);
    conv_9_dw_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_9_dw_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 583300);
    conv_9_dw_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 583300);
    conv_9_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_9_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 585352);
    conv_9_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 585352);
    conv_9_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_9_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 847496);
    conv_9_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 847496);
    conv_9_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_9_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 849544);
    conv_9_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 849544);
    conv_9_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_9_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 851592);
    conv_9_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 851592);
    conv_10_dw_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_10_dw_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 853644);
    conv_10_dw_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 853644);
    conv_10_dw_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_10_dw_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 858252);
    conv_10_dw_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 858252);
    conv_10_dw_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_10_dw_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 860300);
    conv_10_dw_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 860300);
    conv_10_dw_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_10_dw_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 862348);
    conv_10_dw_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 862348);
    conv_10_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_10_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 864400);
    conv_10_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 864400);
    conv_10_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_10_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 1126544);
    conv_10_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 1126544);
    conv_10_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_10_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 1128592);
    conv_10_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 1128592);
    conv_10_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_10_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 1130640);
    conv_10_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 1130640);
    conv_11_dw_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_11_dw_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 1132692);
    conv_11_dw_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 1132692);
    conv_11_dw_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_11_dw_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 1137300);
    conv_11_dw_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 1137300);
    conv_11_dw_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_11_dw_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 1139348);
    conv_11_dw_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 1139348);
    conv_11_dw_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_11_dw_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 1141396);
    conv_11_dw_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 1141396);
    conv_11_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_11_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 1143448);
    conv_11_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 1143448);
    conv_11_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_11_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 1405592);
    conv_11_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 1405592);
    conv_11_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_11_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 1407640);
    conv_11_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 1407640);
    conv_11_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_11_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 1409688);
    conv_11_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 1409688);
    conv_12_dw_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_12_dw_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 1411740);
    conv_12_dw_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 1411740);
    conv_12_dw_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_12_dw_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 1416348);
    conv_12_dw_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 1416348);
    conv_12_dw_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_12_dw_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 1418396);
    conv_12_dw_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 1418396);
    conv_12_dw_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_12_dw_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 1420444);
    conv_12_dw_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 1420444);
    conv_12_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_12_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 1422496);
    conv_12_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 1422496);
    conv_12_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_12_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 1684640);
    conv_12_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 1684640);
    conv_12_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_12_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 1686688);
    conv_12_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 1686688);
    conv_12_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_12_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 1688736);
    conv_12_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 1688736);
    conv_13_dw_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_13_dw_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 1690788);
    conv_13_dw_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 1690788);
    conv_13_dw_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_13_dw_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 1695396);
    conv_13_dw_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 1695396);
    conv_13_dw_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_13_dw_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 1697444);
    conv_13_dw_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 1697444);
    conv_13_dw_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_13_dw_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 1699492);
    conv_13_dw_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 1699492);
    conv_13_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_13_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 1701544);
    conv_13_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 1701544);
    conv_13_conv2d_quantized_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_13_conv2d_quantized_bias_array.data = AI_PTR(g_sface_weights_map[0] + 2225832);
    conv_13_conv2d_quantized_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 2225832);
    conv_13_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_13_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 2229928);
    conv_13_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 2229928);
    conv_13_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_13_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 2234024);
    conv_13_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 2234024);
    conv_13_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_13_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 2238120);
    conv_13_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 2238120);
    conv_14_dw_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_14_dw_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 2242220);
    conv_14_dw_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 2242220);
    conv_14_dw_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_14_dw_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 2251436);
    conv_14_dw_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 2251436);
    conv_14_dw_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_14_dw_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 2255532);
    conv_14_dw_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 2255532);
    conv_14_dw_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_14_dw_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 2259628);
    conv_14_dw_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 2259628);
    conv_14_conv2d_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    conv_14_conv2d_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 2263728);
    conv_14_conv2d_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 2263728);
    conv_14_batchnorm_scale_array.format |= AI_FMT_FLAG_CONST;
    conv_14_batchnorm_scale_array.data = AI_PTR(g_sface_weights_map[0] + 3312304);
    conv_14_batchnorm_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 3312304);
    conv_14_batchnorm_bias_array.format |= AI_FMT_FLAG_CONST;
    conv_14_batchnorm_bias_array.data = AI_PTR(g_sface_weights_map[0] + 3316400);
    conv_14_batchnorm_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 3316400);
    conv_14_relu_alpha_array.format |= AI_FMT_FLAG_CONST;
    conv_14_relu_alpha_array.data = AI_PTR(g_sface_weights_map[0] + 3320496);
    conv_14_relu_alpha_array.data_start = AI_PTR(g_sface_weights_map[0] + 3320496);
    bn1_scale_array.format |= AI_FMT_FLAG_CONST;
    bn1_scale_array.data = AI_PTR(g_sface_weights_map[0] + 3324596);
    bn1_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 3324596);
    bn1_bias_array.format |= AI_FMT_FLAG_CONST;
    bn1_bias_array.data = AI_PTR(g_sface_weights_map[0] + 3328692);
    bn1_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 3328692);
    pre_fc1_MatMul_quantized_weights_array.format |= AI_FMT_FLAG_CONST;
    pre_fc1_MatMul_quantized_weights_array.data = AI_PTR(g_sface_weights_map[0] + 3332788);
    pre_fc1_MatMul_quantized_weights_array.data_start = AI_PTR(g_sface_weights_map[0] + 3332788);
    pre_fc1_bias_quantized_array.format |= AI_FMT_FLAG_CONST;
    pre_fc1_bias_quantized_array.data = AI_PTR(g_sface_weights_map[0] + 9755316);
    pre_fc1_bias_quantized_array.data_start = AI_PTR(g_sface_weights_map[0] + 9755316);
    fc1_scale_array.format |= AI_FMT_FLAG_CONST;
    fc1_scale_array.data = AI_PTR(g_sface_weights_map[0] + 9755444);
    fc1_scale_array.data_start = AI_PTR(g_sface_weights_map[0] + 9755444);
    fc1_bias_array.format |= AI_FMT_FLAG_CONST;
    fc1_bias_array.data = AI_PTR(g_sface_weights_map[0] + 9755956);
    fc1_bias_array.data_start = AI_PTR(g_sface_weights_map[0] + 9755956);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_WEIGHTS);
  return false;
}


/**  PUBLIC APIs SECTION  *****************************************************/



AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_sface_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_SFACE_MODEL_NAME,
      .model_signature   = AI_SFACE_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 614786880,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .params            = AI_STRUCT_INIT,
      .activations       = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0xaf7f5448,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}



AI_API_ENTRY
ai_bool ai_sface_get_report(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_SFACE_MODEL_NAME,
      .model_signature   = AI_SFACE_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 614786880,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .map_signature     = AI_MAGIC_SIGNATURE,
      .map_weights       = AI_STRUCT_INIT,
      .map_activations   = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0xaf7f5448,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}


AI_API_ENTRY
ai_error ai_sface_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}


AI_API_ENTRY
ai_error ai_sface_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    AI_CONTEXT_OBJ(&AI_NET_OBJ_INSTANCE),
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}


AI_API_ENTRY
ai_error ai_sface_create_and_init(
  ai_handle* network, const ai_handle activations[], const ai_handle weights[])
{
  ai_error err;
  ai_network_params params;

  err = ai_sface_create(network, AI_SFACE_DATA_CONFIG);
  if (err.type != AI_ERROR_NONE) {
    return err;
  }
  
  if (ai_sface_data_params_get(&params) != true) {
    err = ai_sface_get_error(*network);
    return err;
  }
#if defined(AI_SFACE_DATA_ACTIVATIONS_COUNT)
  /* set the addresses of the activations buffers */
  for (ai_u16 idx=0; activations && idx<params.map_activations.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_activations, idx, activations[idx]);
  }
#endif
#if defined(AI_SFACE_DATA_WEIGHTS_COUNT)
  /* set the addresses of the weight buffers */
  for (ai_u16 idx=0; weights && idx<params.map_weights.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_weights, idx, weights[idx]);
  }
#endif
  if (ai_sface_init(*network, &params) != true) {
    err = ai_sface_get_error(*network);
  }
  return err;
}


AI_API_ENTRY
ai_buffer* ai_sface_inputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_inputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_buffer* ai_sface_outputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_outputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_handle ai_sface_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}


AI_API_ENTRY
ai_bool ai_sface_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = AI_NETWORK_OBJ(ai_platform_network_init(network, params));
  ai_bool ok = true;

  if (!net_ctx) return false;
  ok &= sface_configure_weights(net_ctx, params);
  ok &= sface_configure_activations(net_ctx, params);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_sface_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}


AI_API_ENTRY
ai_i32 ai_sface_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}



#undef AI_SFACE_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

