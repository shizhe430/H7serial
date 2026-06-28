/**
  ******************************************************************************
  * @file    water_detect.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-06-27T17:49:15+0800
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


#include "water_detect.h"
#include "water_detect_data.h"

#include "ai_platform.h"
#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"



#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_water_detect
 
#undef AI_WATER_DETECT_MODEL_SIGNATURE
#define AI_WATER_DETECT_MODEL_SIGNATURE     "0x676756861f5b23933d4e8aabe7b01f97"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "2026-06-27T17:49:15+0800"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_WATER_DETECT_N_BATCHES
#define AI_WATER_DETECT_N_BATCHES         (1)

static ai_ptr g_water_detect_activations_map[1] = AI_C_ARRAY_INIT;
static ai_ptr g_water_detect_weights_map[1] = AI_C_ARRAY_INIT;



/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  input_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 50176, AI_STATIC)

/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_0_feat_0_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_0_feat_0_2_Clip_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_1_block_block_0_block_0_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_1_block_block_0_block_0_2_Relu_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_1_block_block_2_block_2_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_2_block_block_0_block_0_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 125440, AI_STATIC)

/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_2_block_block_0_block_0_2_Relu_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 125440, AI_STATIC)

/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_2_block_block_1_block_1_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 31360, AI_STATIC)

/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_2_block_block_1_block_1_2_Relu_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 31360, AI_STATIC)

/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_2_block_block_2_block_2_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12544, AI_STATIC)

/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_3_block_block_0_block_0_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 37632, AI_STATIC)

/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_3_block_block_0_block_0_2_Relu_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 37632, AI_STATIC)

/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_3_block_block_1_block_1_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 37632, AI_STATIC)

/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_3_block_block_1_block_1_2_Relu_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 37632, AI_STATIC)

/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_3_block_block_2_block_2_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12544, AI_STATIC)

/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_3_Add_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12544, AI_STATIC)

/* Array#17 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_4_block_block_0_block_0_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 37632, AI_STATIC)

/* Array#18 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_4_block_block_0_block_0_2_Clip_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 37632, AI_STATIC)

/* Array#19 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_4_block_block_1_block_1_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 9408, AI_STATIC)

/* Array#20 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_4_block_block_1_block_1_2_Clip_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 9408, AI_STATIC)

/* Array#21 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_4_block_block_3_block_3_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4704, AI_STATIC)

/* Array#22 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_5_block_block_0_block_0_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 23520, AI_STATIC)

/* Array#23 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_5_block_block_0_block_0_2_Clip_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 23520, AI_STATIC)

/* Array#24 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_5_block_block_1_block_1_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 23520, AI_STATIC)

/* Array#25 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_5_block_block_1_block_1_2_Clip_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 23520, AI_STATIC)

/* Array#26 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_5_block_block_3_block_3_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4704, AI_STATIC)

/* Array#27 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_5_Add_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4704, AI_STATIC)

/* Array#28 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_6_block_block_0_block_0_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 23520, AI_STATIC)

/* Array#29 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_6_block_block_0_block_0_2_Clip_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 23520, AI_STATIC)

/* Array#30 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_6_block_block_1_block_1_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 23520, AI_STATIC)

/* Array#31 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_6_block_block_1_block_1_2_Clip_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 23520, AI_STATIC)

/* Array#32 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_6_block_block_3_block_3_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4704, AI_STATIC)

/* Array#33 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_6_Add_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4704, AI_STATIC)

/* Array#34 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_7_block_block_0_block_0_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12544, AI_STATIC)

/* Array#35 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_7_block_block_0_block_0_2_Clip_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12544, AI_STATIC)

/* Array#36 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_7_block_block_1_block_1_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12544, AI_STATIC)

/* Array#37 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_7_block_block_1_block_1_2_Clip_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12544, AI_STATIC)

/* Array#38 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_7_block_block_3_block_3_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4704, AI_STATIC)

/* Array#39 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_7_Add_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4704, AI_STATIC)

/* Array#40 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_8_block_block_0_block_0_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 14112, AI_STATIC)

/* Array#41 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_8_block_block_0_block_0_2_Clip_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 14112, AI_STATIC)

/* Array#42 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_8_block_block_1_block_1_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 14112, AI_STATIC)

/* Array#43 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_8_block_block_1_block_1_2_Clip_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 14112, AI_STATIC)

/* Array#44 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_8_block_block_3_block_3_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4704, AI_STATIC)

/* Array#45 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_8_Add_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4704, AI_STATIC)

/* Array#46 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_9_block_block_0_block_0_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28224, AI_STATIC)

/* Array#47 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_9_block_block_0_block_0_2_Clip_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28224, AI_STATIC)

/* Array#48 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_9_block_block_1_block_1_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 7056, AI_STATIC)

/* Array#49 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_9_block_block_1_block_1_2_Clip_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 7056, AI_STATIC)

/* Array#50 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_9_block_block_3_block_3_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2352, AI_STATIC)

/* Array#51 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_10_block_block_0_block_0_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 14112, AI_STATIC)

/* Array#52 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_10_block_block_0_block_0_2_Clip_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 14112, AI_STATIC)

/* Array#53 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_10_block_block_1_block_1_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 14112, AI_STATIC)

/* Array#54 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_10_block_block_1_block_1_2_Clip_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 14112, AI_STATIC)

/* Array#55 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_10_block_block_3_block_3_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2352, AI_STATIC)

/* Array#56 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_10_Add_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2352, AI_STATIC)

/* Array#57 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_11_block_block_0_block_0_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 14112, AI_STATIC)

/* Array#58 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_11_block_block_0_block_0_2_Clip_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 14112, AI_STATIC)

/* Array#59 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_11_block_block_1_block_1_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 14112, AI_STATIC)

/* Array#60 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_11_block_block_1_block_1_2_Clip_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 14112, AI_STATIC)

/* Array#61 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_11_block_block_3_block_3_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2352, AI_STATIC)

/* Array#62 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_11_Add_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2352, AI_STATIC)

/* Array#63 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_12_feat_12_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 14112, AI_STATIC)

/* Array#64 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_12_feat_12_2_Clip_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 14112, AI_STATIC)

/* Array#65 */
AI_ARRAY_OBJ_DECLARE(
  _pool_GlobalAveragePool_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#66 */
AI_ARRAY_OBJ_DECLARE(
  _shared_shared_1_Gemm_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#67 */
AI_ARRAY_OBJ_DECLARE(
  _shared_shared_2_Clip_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#68 */
AI_ARRAY_OBJ_DECLARE(
  logits_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 5, AI_STATIC)

/* Array#69 */
AI_ARRAY_OBJ_DECLARE(
  _reg_head_Gemm_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#70 */
AI_ARRAY_OBJ_DECLARE(
  _Sigmoid_output_0_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 1, AI_STATIC)

/* Array#71 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_0_feat_0_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 72, AI_STATIC)

/* Array#72 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_0_feat_0_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)

/* Array#73 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_1_block_block_0_block_0_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 72, AI_STATIC)

/* Array#74 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_1_block_block_0_block_0_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)

/* Array#75 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_1_block_block_2_block_2_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#76 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_1_block_block_2_block_2_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)

/* Array#77 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_2_block_block_0_block_0_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 320, AI_STATIC)

/* Array#78 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_2_block_block_0_block_0_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 40, AI_STATIC)

/* Array#79 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_2_block_block_1_block_1_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 360, AI_STATIC)

/* Array#80 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_2_block_block_1_block_1_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 40, AI_STATIC)

/* Array#81 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_2_block_block_2_block_2_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 640, AI_STATIC)

/* Array#82 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_2_block_block_2_block_2_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#83 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_3_block_block_0_block_0_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 768, AI_STATIC)

/* Array#84 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_3_block_block_0_block_0_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/* Array#85 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_3_block_block_1_block_1_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 432, AI_STATIC)

/* Array#86 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_3_block_block_1_block_1_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/* Array#87 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_3_block_block_2_block_2_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 768, AI_STATIC)

/* Array#88 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_3_block_block_2_block_2_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#89 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_4_block_block_0_block_0_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 768, AI_STATIC)

/* Array#90 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_4_block_block_0_block_0_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/* Array#91 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_4_block_block_1_block_1_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1200, AI_STATIC)

/* Array#92 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_4_block_block_1_block_1_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/* Array#93 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_4_block_block_3_block_3_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1152, AI_STATIC)

/* Array#94 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_4_block_block_3_block_3_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 24, AI_STATIC)

/* Array#95 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_5_block_block_0_block_0_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2880, AI_STATIC)

/* Array#96 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_5_block_block_0_block_0_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 120, AI_STATIC)

/* Array#97 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_5_block_block_1_block_1_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3000, AI_STATIC)

/* Array#98 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_5_block_block_1_block_1_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 120, AI_STATIC)

/* Array#99 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_5_block_block_3_block_3_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2880, AI_STATIC)

/* Array#100 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_5_block_block_3_block_3_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 24, AI_STATIC)

/* Array#101 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_6_block_block_0_block_0_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2880, AI_STATIC)

/* Array#102 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_6_block_block_0_block_0_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 120, AI_STATIC)

/* Array#103 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_6_block_block_1_block_1_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3000, AI_STATIC)

/* Array#104 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_6_block_block_1_block_1_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 120, AI_STATIC)

/* Array#105 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_6_block_block_3_block_3_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2880, AI_STATIC)

/* Array#106 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_6_block_block_3_block_3_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 24, AI_STATIC)

/* Array#107 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_7_block_block_0_block_0_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1536, AI_STATIC)

/* Array#108 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_7_block_block_0_block_0_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#109 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_7_block_block_1_block_1_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1600, AI_STATIC)

/* Array#110 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_7_block_block_1_block_1_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#111 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_7_block_block_3_block_3_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1536, AI_STATIC)

/* Array#112 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_7_block_block_3_block_3_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 24, AI_STATIC)

/* Array#113 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_8_block_block_0_block_0_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1728, AI_STATIC)

/* Array#114 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_8_block_block_0_block_0_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 72, AI_STATIC)

/* Array#115 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_8_block_block_1_block_1_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1800, AI_STATIC)

/* Array#116 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_8_block_block_1_block_1_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 72, AI_STATIC)

/* Array#117 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_8_block_block_3_block_3_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1728, AI_STATIC)

/* Array#118 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_8_block_block_3_block_3_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 24, AI_STATIC)

/* Array#119 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_9_block_block_0_block_0_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3456, AI_STATIC)

/* Array#120 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_9_block_block_0_block_0_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 144, AI_STATIC)

/* Array#121 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_9_block_block_1_block_1_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3600, AI_STATIC)

/* Array#122 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_9_block_block_1_block_1_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 144, AI_STATIC)

/* Array#123 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_9_block_block_3_block_3_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6912, AI_STATIC)

/* Array#124 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_9_block_block_3_block_3_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/* Array#125 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_10_block_block_0_block_0_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 13824, AI_STATIC)

/* Array#126 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_10_block_block_0_block_0_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#127 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_10_block_block_1_block_1_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 7200, AI_STATIC)

/* Array#128 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_10_block_block_1_block_1_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#129 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_10_block_block_3_block_3_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 13824, AI_STATIC)

/* Array#130 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_10_block_block_3_block_3_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/* Array#131 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_11_block_block_0_block_0_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 13824, AI_STATIC)

/* Array#132 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_11_block_block_0_block_0_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#133 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_11_block_block_1_block_1_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 7200, AI_STATIC)

/* Array#134 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_11_block_block_1_block_1_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#135 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_11_block_block_3_block_3_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 13824, AI_STATIC)

/* Array#136 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_11_block_block_3_block_3_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/* Array#137 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_12_feat_12_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 13824, AI_STATIC)

/* Array#138 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_12_feat_12_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#139 */
AI_ARRAY_OBJ_DECLARE(
  _shared_shared_1_Gemm_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 36864, AI_STATIC)

/* Array#140 */
AI_ARRAY_OBJ_DECLARE(
  _shared_shared_1_Gemm_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#141 */
AI_ARRAY_OBJ_DECLARE(
  logits_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 640, AI_STATIC)

/* Array#142 */
AI_ARRAY_OBJ_DECLARE(
  logits_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 5, AI_STATIC)

/* Array#143 */
AI_ARRAY_OBJ_DECLARE(
  _reg_head_Gemm_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#144 */
AI_ARRAY_OBJ_DECLARE(
  _reg_head_Gemm_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)

/* Array#145 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_0_feat_0_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 9, AI_STATIC)

/* Array#146 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_1_block_block_2_block_2_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)

/* Array#147 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_2_block_block_0_block_0_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)

/* Array#148 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_2_block_block_2_block_2_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 40, AI_STATIC)

/* Array#149 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_3_block_block_0_block_0_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#150 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_3_block_block_2_block_2_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/* Array#151 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_4_block_block_0_block_0_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#152 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_4_block_block_3_block_3_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/* Array#153 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_5_block_block_0_block_0_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 24, AI_STATIC)

/* Array#154 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_5_block_block_3_block_3_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 120, AI_STATIC)

/* Array#155 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_6_block_block_0_block_0_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 24, AI_STATIC)

/* Array#156 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_6_block_block_3_block_3_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 120, AI_STATIC)

/* Array#157 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_7_block_block_0_block_0_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 24, AI_STATIC)

/* Array#158 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_7_block_block_3_block_3_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#159 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_8_block_block_0_block_0_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 24, AI_STATIC)

/* Array#160 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_8_block_block_3_block_3_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 72, AI_STATIC)

/* Array#161 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_9_block_block_0_block_0_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 24, AI_STATIC)

/* Array#162 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_9_block_block_3_block_3_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 144, AI_STATIC)

/* Array#163 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_10_block_block_0_block_0_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/* Array#164 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_10_block_block_3_block_3_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#165 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_11_block_block_0_block_0_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/* Array#166 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_11_block_block_3_block_3_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#167 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_12_feat_12_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  _Sigmoid_output_0_output, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &_Sigmoid_output_0_output_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_0_feat_0_0_Conv_output_0_bias, AI_STATIC,
  1, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &_feat_feat_0_feat_0_0_Conv_output_0_bias_array, NULL)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_0_feat_0_0_Conv_output_0_output, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 112, 112), AI_STRIDE_INIT(4, 4, 4, 32, 3584),
  1, &_feat_feat_0_feat_0_0_Conv_output_0_output_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_0_feat_0_0_Conv_output_0_scratch0, AI_STATIC,
  3, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 3, 3), AI_STRIDE_INIT(4, 4, 4, 4, 12),
  1, &_feat_feat_0_feat_0_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_0_feat_0_0_Conv_output_0_weights, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 8), AI_STRIDE_INIT(4, 4, 4, 32, 96),
  1, &_feat_feat_0_feat_0_0_Conv_output_0_weights_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_0_feat_0_2_Clip_output_0_output, AI_STATIC,
  5, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 112, 112), AI_STRIDE_INIT(4, 4, 4, 32, 3584),
  1, &_feat_feat_0_feat_0_2_Clip_output_0_output_array, NULL)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_10_Add_output_0_output, AI_STATIC,
  6, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 7, 7), AI_STRIDE_INIT(4, 4, 4, 192, 1344),
  1, &_feat_feat_10_Add_output_0_output_array, NULL)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_10_block_block_0_block_0_0_Conv_output_0_bias, AI_STATIC,
  7, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &_feat_feat_10_block_block_0_block_0_0_Conv_output_0_bias_array, NULL)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_10_block_block_0_block_0_0_Conv_output_0_output, AI_STATIC,
  8, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 7, 7), AI_STRIDE_INIT(4, 4, 4, 1152, 8064),
  1, &_feat_feat_10_block_block_0_block_0_0_Conv_output_0_output_array, NULL)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_10_block_block_0_block_0_0_Conv_output_0_scratch0, AI_STATIC,
  9, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 1, 1), AI_STRIDE_INIT(4, 4, 4, 192, 192),
  1, &_feat_feat_10_block_block_0_block_0_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_10_block_block_0_block_0_0_Conv_output_0_weights, AI_STATIC,
  10, 0x0,
  AI_SHAPE_INIT(4, 48, 1, 1, 288), AI_STRIDE_INIT(4, 4, 192, 55296, 55296),
  1, &_feat_feat_10_block_block_0_block_0_0_Conv_output_0_weights_array, NULL)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_10_block_block_0_block_0_2_Clip_output_0_output, AI_STATIC,
  11, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 7, 7), AI_STRIDE_INIT(4, 4, 4, 1152, 8064),
  1, &_feat_feat_10_block_block_0_block_0_2_Clip_output_0_output_array, NULL)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_10_block_block_1_block_1_0_Conv_output_0_bias, AI_STATIC,
  12, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &_feat_feat_10_block_block_1_block_1_0_Conv_output_0_bias_array, NULL)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_10_block_block_1_block_1_0_Conv_output_0_output, AI_STATIC,
  13, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 7, 7), AI_STRIDE_INIT(4, 4, 4, 1152, 8064),
  1, &_feat_feat_10_block_block_1_block_1_0_Conv_output_0_output_array, NULL)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_10_block_block_1_block_1_0_Conv_output_0_weights, AI_STATIC,
  14, 0x0,
  AI_SHAPE_INIT(4, 1, 5, 5, 288), AI_STRIDE_INIT(4, 1, 288, 288, 288),
  1, &_feat_feat_10_block_block_1_block_1_0_Conv_output_0_weights_array, NULL)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_10_block_block_1_block_1_2_Clip_output_0_output, AI_STATIC,
  15, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 7, 7), AI_STRIDE_INIT(4, 4, 4, 1152, 8064),
  1, &_feat_feat_10_block_block_1_block_1_2_Clip_output_0_output_array, NULL)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_10_block_block_3_block_3_0_Conv_output_0_bias, AI_STATIC,
  16, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 1, 1), AI_STRIDE_INIT(4, 4, 4, 192, 192),
  1, &_feat_feat_10_block_block_3_block_3_0_Conv_output_0_bias_array, NULL)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_10_block_block_3_block_3_0_Conv_output_0_output, AI_STATIC,
  17, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 7, 7), AI_STRIDE_INIT(4, 4, 4, 192, 1344),
  1, &_feat_feat_10_block_block_3_block_3_0_Conv_output_0_output_array, NULL)

/* Tensor #18 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_10_block_block_3_block_3_0_Conv_output_0_scratch0, AI_STATIC,
  18, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &_feat_feat_10_block_block_3_block_3_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #19 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_10_block_block_3_block_3_0_Conv_output_0_weights, AI_STATIC,
  19, 0x0,
  AI_SHAPE_INIT(4, 288, 1, 1, 48), AI_STRIDE_INIT(4, 4, 1152, 55296, 55296),
  1, &_feat_feat_10_block_block_3_block_3_0_Conv_output_0_weights_array, NULL)

/* Tensor #20 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_11_Add_output_0_output, AI_STATIC,
  20, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 7, 7), AI_STRIDE_INIT(4, 4, 4, 192, 1344),
  1, &_feat_feat_11_Add_output_0_output_array, NULL)

/* Tensor #21 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_11_block_block_0_block_0_0_Conv_output_0_bias, AI_STATIC,
  21, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &_feat_feat_11_block_block_0_block_0_0_Conv_output_0_bias_array, NULL)

/* Tensor #22 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_11_block_block_0_block_0_0_Conv_output_0_output, AI_STATIC,
  22, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 7, 7), AI_STRIDE_INIT(4, 4, 4, 1152, 8064),
  1, &_feat_feat_11_block_block_0_block_0_0_Conv_output_0_output_array, NULL)

/* Tensor #23 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_11_block_block_0_block_0_0_Conv_output_0_scratch0, AI_STATIC,
  23, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 1, 1), AI_STRIDE_INIT(4, 4, 4, 192, 192),
  1, &_feat_feat_11_block_block_0_block_0_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #24 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_11_block_block_0_block_0_0_Conv_output_0_weights, AI_STATIC,
  24, 0x0,
  AI_SHAPE_INIT(4, 48, 1, 1, 288), AI_STRIDE_INIT(4, 4, 192, 55296, 55296),
  1, &_feat_feat_11_block_block_0_block_0_0_Conv_output_0_weights_array, NULL)

/* Tensor #25 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_11_block_block_0_block_0_2_Clip_output_0_output, AI_STATIC,
  25, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 7, 7), AI_STRIDE_INIT(4, 4, 4, 1152, 8064),
  1, &_feat_feat_11_block_block_0_block_0_2_Clip_output_0_output_array, NULL)

/* Tensor #26 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_11_block_block_1_block_1_0_Conv_output_0_bias, AI_STATIC,
  26, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &_feat_feat_11_block_block_1_block_1_0_Conv_output_0_bias_array, NULL)

/* Tensor #27 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_11_block_block_1_block_1_0_Conv_output_0_output, AI_STATIC,
  27, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 7, 7), AI_STRIDE_INIT(4, 4, 4, 1152, 8064),
  1, &_feat_feat_11_block_block_1_block_1_0_Conv_output_0_output_array, NULL)

/* Tensor #28 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_11_block_block_1_block_1_0_Conv_output_0_weights, AI_STATIC,
  28, 0x0,
  AI_SHAPE_INIT(4, 1, 5, 5, 288), AI_STRIDE_INIT(4, 1, 288, 288, 288),
  1, &_feat_feat_11_block_block_1_block_1_0_Conv_output_0_weights_array, NULL)

/* Tensor #29 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_11_block_block_1_block_1_2_Clip_output_0_output, AI_STATIC,
  29, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 7, 7), AI_STRIDE_INIT(4, 4, 4, 1152, 8064),
  1, &_feat_feat_11_block_block_1_block_1_2_Clip_output_0_output_array, NULL)

/* Tensor #30 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_11_block_block_3_block_3_0_Conv_output_0_bias, AI_STATIC,
  30, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 1, 1), AI_STRIDE_INIT(4, 4, 4, 192, 192),
  1, &_feat_feat_11_block_block_3_block_3_0_Conv_output_0_bias_array, NULL)

/* Tensor #31 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_11_block_block_3_block_3_0_Conv_output_0_output, AI_STATIC,
  31, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 7, 7), AI_STRIDE_INIT(4, 4, 4, 192, 1344),
  1, &_feat_feat_11_block_block_3_block_3_0_Conv_output_0_output_array, NULL)

/* Tensor #32 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_11_block_block_3_block_3_0_Conv_output_0_scratch0, AI_STATIC,
  32, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &_feat_feat_11_block_block_3_block_3_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #33 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_11_block_block_3_block_3_0_Conv_output_0_weights, AI_STATIC,
  33, 0x0,
  AI_SHAPE_INIT(4, 288, 1, 1, 48), AI_STRIDE_INIT(4, 4, 1152, 55296, 55296),
  1, &_feat_feat_11_block_block_3_block_3_0_Conv_output_0_weights_array, NULL)

/* Tensor #34 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_12_feat_12_0_Conv_output_0_bias, AI_STATIC,
  34, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &_feat_feat_12_feat_12_0_Conv_output_0_bias_array, NULL)

/* Tensor #35 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_12_feat_12_0_Conv_output_0_output, AI_STATIC,
  35, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 7, 7), AI_STRIDE_INIT(4, 4, 4, 1152, 8064),
  1, &_feat_feat_12_feat_12_0_Conv_output_0_output_array, NULL)

/* Tensor #36 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_12_feat_12_0_Conv_output_0_scratch0, AI_STATIC,
  36, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 1, 1), AI_STRIDE_INIT(4, 4, 4, 192, 192),
  1, &_feat_feat_12_feat_12_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #37 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_12_feat_12_0_Conv_output_0_weights, AI_STATIC,
  37, 0x0,
  AI_SHAPE_INIT(4, 48, 1, 1, 288), AI_STRIDE_INIT(4, 4, 192, 55296, 55296),
  1, &_feat_feat_12_feat_12_0_Conv_output_0_weights_array, NULL)

/* Tensor #38 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_12_feat_12_2_Clip_output_0_output, AI_STATIC,
  38, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 7, 7), AI_STRIDE_INIT(4, 4, 4, 1152, 8064),
  1, &_feat_feat_12_feat_12_2_Clip_output_0_output_array, NULL)

/* Tensor #39 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_1_block_block_0_block_0_0_Conv_output_0_bias, AI_STATIC,
  39, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &_feat_feat_1_block_block_0_block_0_0_Conv_output_0_bias_array, NULL)

/* Tensor #40 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_1_block_block_0_block_0_0_Conv_output_0_output, AI_STATIC,
  40, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 56, 56), AI_STRIDE_INIT(4, 4, 4, 32, 1792),
  1, &_feat_feat_1_block_block_0_block_0_0_Conv_output_0_output_array, NULL)

/* Tensor #41 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_1_block_block_0_block_0_0_Conv_output_0_weights, AI_STATIC,
  41, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 8), AI_STRIDE_INIT(4, 1, 8, 8, 8),
  1, &_feat_feat_1_block_block_0_block_0_0_Conv_output_0_weights_array, NULL)

/* Tensor #42 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_1_block_block_0_block_0_2_Relu_output_0_output, AI_STATIC,
  42, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 56, 56), AI_STRIDE_INIT(4, 4, 4, 32, 1792),
  1, &_feat_feat_1_block_block_0_block_0_2_Relu_output_0_output_array, NULL)

/* Tensor #43 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_1_block_block_2_block_2_0_Conv_output_0_bias, AI_STATIC,
  43, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &_feat_feat_1_block_block_2_block_2_0_Conv_output_0_bias_array, NULL)

/* Tensor #44 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_1_block_block_2_block_2_0_Conv_output_0_output, AI_STATIC,
  44, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 56, 56), AI_STRIDE_INIT(4, 4, 4, 32, 1792),
  1, &_feat_feat_1_block_block_2_block_2_0_Conv_output_0_output_array, NULL)

/* Tensor #45 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_1_block_block_2_block_2_0_Conv_output_0_scratch0, AI_STATIC,
  45, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &_feat_feat_1_block_block_2_block_2_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #46 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_1_block_block_2_block_2_0_Conv_output_0_weights, AI_STATIC,
  46, 0x0,
  AI_SHAPE_INIT(4, 8, 1, 1, 8), AI_STRIDE_INIT(4, 4, 32, 256, 256),
  1, &_feat_feat_1_block_block_2_block_2_0_Conv_output_0_weights_array, NULL)

/* Tensor #47 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_2_block_block_0_block_0_0_Conv_output_0_bias, AI_STATIC,
  47, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 1, 1), AI_STRIDE_INIT(4, 4, 4, 160, 160),
  1, &_feat_feat_2_block_block_0_block_0_0_Conv_output_0_bias_array, NULL)

/* Tensor #48 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_2_block_block_0_block_0_0_Conv_output_0_output, AI_STATIC,
  48, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 56, 56), AI_STRIDE_INIT(4, 4, 4, 160, 8960),
  1, &_feat_feat_2_block_block_0_block_0_0_Conv_output_0_output_array, NULL)

/* Tensor #49 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_2_block_block_0_block_0_0_Conv_output_0_scratch0, AI_STATIC,
  49, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &_feat_feat_2_block_block_0_block_0_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #50 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_2_block_block_0_block_0_0_Conv_output_0_weights, AI_STATIC,
  50, 0x0,
  AI_SHAPE_INIT(4, 8, 1, 1, 40), AI_STRIDE_INIT(4, 4, 32, 1280, 1280),
  1, &_feat_feat_2_block_block_0_block_0_0_Conv_output_0_weights_array, NULL)

/* Tensor #51 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_2_block_block_0_block_0_2_Relu_output_0_output, AI_STATIC,
  51, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 56, 56), AI_STRIDE_INIT(4, 4, 4, 160, 8960),
  1, &_feat_feat_2_block_block_0_block_0_2_Relu_output_0_output_array, NULL)

/* Tensor #52 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_2_block_block_1_block_1_0_Conv_output_0_bias, AI_STATIC,
  52, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 1, 1), AI_STRIDE_INIT(4, 4, 4, 160, 160),
  1, &_feat_feat_2_block_block_1_block_1_0_Conv_output_0_bias_array, NULL)

/* Tensor #53 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_2_block_block_1_block_1_0_Conv_output_0_output, AI_STATIC,
  53, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 28, 28), AI_STRIDE_INIT(4, 4, 4, 160, 4480),
  1, &_feat_feat_2_block_block_1_block_1_0_Conv_output_0_output_array, NULL)

/* Tensor #54 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_2_block_block_1_block_1_0_Conv_output_0_weights, AI_STATIC,
  54, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 40), AI_STRIDE_INIT(4, 1, 40, 40, 40),
  1, &_feat_feat_2_block_block_1_block_1_0_Conv_output_0_weights_array, NULL)

/* Tensor #55 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_2_block_block_1_block_1_2_Relu_output_0_output, AI_STATIC,
  55, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 28, 28), AI_STRIDE_INIT(4, 4, 4, 160, 4480),
  1, &_feat_feat_2_block_block_1_block_1_2_Relu_output_0_output_array, NULL)

/* Tensor #56 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_2_block_block_2_block_2_0_Conv_output_0_bias, AI_STATIC,
  56, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &_feat_feat_2_block_block_2_block_2_0_Conv_output_0_bias_array, NULL)

/* Tensor #57 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_2_block_block_2_block_2_0_Conv_output_0_output, AI_STATIC,
  57, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 28, 28), AI_STRIDE_INIT(4, 4, 4, 64, 1792),
  1, &_feat_feat_2_block_block_2_block_2_0_Conv_output_0_output_array, NULL)

/* Tensor #58 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_2_block_block_2_block_2_0_Conv_output_0_scratch0, AI_STATIC,
  58, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 1, 1), AI_STRIDE_INIT(4, 4, 4, 160, 160),
  1, &_feat_feat_2_block_block_2_block_2_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #59 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_2_block_block_2_block_2_0_Conv_output_0_weights, AI_STATIC,
  59, 0x0,
  AI_SHAPE_INIT(4, 40, 1, 1, 16), AI_STRIDE_INIT(4, 4, 160, 2560, 2560),
  1, &_feat_feat_2_block_block_2_block_2_0_Conv_output_0_weights_array, NULL)

/* Tensor #60 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_3_Add_output_0_output, AI_STATIC,
  60, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 28, 28), AI_STRIDE_INIT(4, 4, 4, 64, 1792),
  1, &_feat_feat_3_Add_output_0_output_array, NULL)

/* Tensor #61 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_3_block_block_0_block_0_0_Conv_output_0_bias, AI_STATIC,
  61, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 1, 1), AI_STRIDE_INIT(4, 4, 4, 192, 192),
  1, &_feat_feat_3_block_block_0_block_0_0_Conv_output_0_bias_array, NULL)

/* Tensor #62 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_3_block_block_0_block_0_0_Conv_output_0_output, AI_STATIC,
  62, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 28, 28), AI_STRIDE_INIT(4, 4, 4, 192, 5376),
  1, &_feat_feat_3_block_block_0_block_0_0_Conv_output_0_output_array, NULL)

/* Tensor #63 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_3_block_block_0_block_0_0_Conv_output_0_scratch0, AI_STATIC,
  63, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &_feat_feat_3_block_block_0_block_0_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #64 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_3_block_block_0_block_0_0_Conv_output_0_weights, AI_STATIC,
  64, 0x0,
  AI_SHAPE_INIT(4, 16, 1, 1, 48), AI_STRIDE_INIT(4, 4, 64, 3072, 3072),
  1, &_feat_feat_3_block_block_0_block_0_0_Conv_output_0_weights_array, NULL)

/* Tensor #65 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_3_block_block_0_block_0_2_Relu_output_0_output, AI_STATIC,
  65, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 28, 28), AI_STRIDE_INIT(4, 4, 4, 192, 5376),
  1, &_feat_feat_3_block_block_0_block_0_2_Relu_output_0_output_array, NULL)

/* Tensor #66 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_3_block_block_1_block_1_0_Conv_output_0_bias, AI_STATIC,
  66, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 1, 1), AI_STRIDE_INIT(4, 4, 4, 192, 192),
  1, &_feat_feat_3_block_block_1_block_1_0_Conv_output_0_bias_array, NULL)

/* Tensor #67 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_3_block_block_1_block_1_0_Conv_output_0_output, AI_STATIC,
  67, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 28, 28), AI_STRIDE_INIT(4, 4, 4, 192, 5376),
  1, &_feat_feat_3_block_block_1_block_1_0_Conv_output_0_output_array, NULL)

/* Tensor #68 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_3_block_block_1_block_1_0_Conv_output_0_weights, AI_STATIC,
  68, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 48), AI_STRIDE_INIT(4, 1, 48, 48, 48),
  1, &_feat_feat_3_block_block_1_block_1_0_Conv_output_0_weights_array, NULL)

/* Tensor #69 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_3_block_block_1_block_1_2_Relu_output_0_output, AI_STATIC,
  69, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 28, 28), AI_STRIDE_INIT(4, 4, 4, 192, 5376),
  1, &_feat_feat_3_block_block_1_block_1_2_Relu_output_0_output_array, NULL)

/* Tensor #70 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_3_block_block_2_block_2_0_Conv_output_0_bias, AI_STATIC,
  70, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &_feat_feat_3_block_block_2_block_2_0_Conv_output_0_bias_array, NULL)

/* Tensor #71 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_3_block_block_2_block_2_0_Conv_output_0_output, AI_STATIC,
  71, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 28, 28), AI_STRIDE_INIT(4, 4, 4, 64, 1792),
  1, &_feat_feat_3_block_block_2_block_2_0_Conv_output_0_output_array, NULL)

/* Tensor #72 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_3_block_block_2_block_2_0_Conv_output_0_scratch0, AI_STATIC,
  72, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 1, 1), AI_STRIDE_INIT(4, 4, 4, 192, 192),
  1, &_feat_feat_3_block_block_2_block_2_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #73 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_3_block_block_2_block_2_0_Conv_output_0_weights, AI_STATIC,
  73, 0x0,
  AI_SHAPE_INIT(4, 48, 1, 1, 16), AI_STRIDE_INIT(4, 4, 192, 3072, 3072),
  1, &_feat_feat_3_block_block_2_block_2_0_Conv_output_0_weights_array, NULL)

/* Tensor #74 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_4_block_block_0_block_0_0_Conv_output_0_bias, AI_STATIC,
  74, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 1, 1), AI_STRIDE_INIT(4, 4, 4, 192, 192),
  1, &_feat_feat_4_block_block_0_block_0_0_Conv_output_0_bias_array, NULL)

/* Tensor #75 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_4_block_block_0_block_0_0_Conv_output_0_output, AI_STATIC,
  75, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 28, 28), AI_STRIDE_INIT(4, 4, 4, 192, 5376),
  1, &_feat_feat_4_block_block_0_block_0_0_Conv_output_0_output_array, NULL)

/* Tensor #76 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_4_block_block_0_block_0_0_Conv_output_0_scratch0, AI_STATIC,
  76, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &_feat_feat_4_block_block_0_block_0_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #77 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_4_block_block_0_block_0_0_Conv_output_0_weights, AI_STATIC,
  77, 0x0,
  AI_SHAPE_INIT(4, 16, 1, 1, 48), AI_STRIDE_INIT(4, 4, 64, 3072, 3072),
  1, &_feat_feat_4_block_block_0_block_0_0_Conv_output_0_weights_array, NULL)

/* Tensor #78 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_4_block_block_0_block_0_2_Clip_output_0_output, AI_STATIC,
  78, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 28, 28), AI_STRIDE_INIT(4, 4, 4, 192, 5376),
  1, &_feat_feat_4_block_block_0_block_0_2_Clip_output_0_output_array, NULL)

/* Tensor #79 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_4_block_block_1_block_1_0_Conv_output_0_bias, AI_STATIC,
  79, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 1, 1), AI_STRIDE_INIT(4, 4, 4, 192, 192),
  1, &_feat_feat_4_block_block_1_block_1_0_Conv_output_0_bias_array, NULL)

/* Tensor #80 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_4_block_block_1_block_1_0_Conv_output_0_output, AI_STATIC,
  80, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 14, 14), AI_STRIDE_INIT(4, 4, 4, 192, 2688),
  1, &_feat_feat_4_block_block_1_block_1_0_Conv_output_0_output_array, NULL)

/* Tensor #81 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_4_block_block_1_block_1_0_Conv_output_0_weights, AI_STATIC,
  81, 0x0,
  AI_SHAPE_INIT(4, 1, 5, 5, 48), AI_STRIDE_INIT(4, 1, 48, 48, 48),
  1, &_feat_feat_4_block_block_1_block_1_0_Conv_output_0_weights_array, NULL)

/* Tensor #82 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_4_block_block_1_block_1_2_Clip_output_0_output, AI_STATIC,
  82, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 14, 14), AI_STRIDE_INIT(4, 4, 4, 192, 2688),
  1, &_feat_feat_4_block_block_1_block_1_2_Clip_output_0_output_array, NULL)

/* Tensor #83 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_4_block_block_3_block_3_0_Conv_output_0_bias, AI_STATIC,
  83, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 1), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &_feat_feat_4_block_block_3_block_3_0_Conv_output_0_bias_array, NULL)

/* Tensor #84 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_4_block_block_3_block_3_0_Conv_output_0_output, AI_STATIC,
  84, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 14, 14), AI_STRIDE_INIT(4, 4, 4, 96, 1344),
  1, &_feat_feat_4_block_block_3_block_3_0_Conv_output_0_output_array, NULL)

/* Tensor #85 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_4_block_block_3_block_3_0_Conv_output_0_scratch0, AI_STATIC,
  85, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 1, 1), AI_STRIDE_INIT(4, 4, 4, 192, 192),
  1, &_feat_feat_4_block_block_3_block_3_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #86 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_4_block_block_3_block_3_0_Conv_output_0_weights, AI_STATIC,
  86, 0x0,
  AI_SHAPE_INIT(4, 48, 1, 1, 24), AI_STRIDE_INIT(4, 4, 192, 4608, 4608),
  1, &_feat_feat_4_block_block_3_block_3_0_Conv_output_0_weights_array, NULL)

/* Tensor #87 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_5_Add_output_0_output, AI_STATIC,
  87, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 14, 14), AI_STRIDE_INIT(4, 4, 4, 96, 1344),
  1, &_feat_feat_5_Add_output_0_output_array, NULL)

/* Tensor #88 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_5_block_block_0_block_0_0_Conv_output_0_bias, AI_STATIC,
  88, 0x0,
  AI_SHAPE_INIT(4, 1, 120, 1, 1), AI_STRIDE_INIT(4, 4, 4, 480, 480),
  1, &_feat_feat_5_block_block_0_block_0_0_Conv_output_0_bias_array, NULL)

/* Tensor #89 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_5_block_block_0_block_0_0_Conv_output_0_output, AI_STATIC,
  89, 0x0,
  AI_SHAPE_INIT(4, 1, 120, 14, 14), AI_STRIDE_INIT(4, 4, 4, 480, 6720),
  1, &_feat_feat_5_block_block_0_block_0_0_Conv_output_0_output_array, NULL)

/* Tensor #90 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_5_block_block_0_block_0_0_Conv_output_0_scratch0, AI_STATIC,
  90, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 1), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &_feat_feat_5_block_block_0_block_0_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #91 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_5_block_block_0_block_0_0_Conv_output_0_weights, AI_STATIC,
  91, 0x0,
  AI_SHAPE_INIT(4, 24, 1, 1, 120), AI_STRIDE_INIT(4, 4, 96, 11520, 11520),
  1, &_feat_feat_5_block_block_0_block_0_0_Conv_output_0_weights_array, NULL)

/* Tensor #92 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_5_block_block_0_block_0_2_Clip_output_0_output, AI_STATIC,
  92, 0x0,
  AI_SHAPE_INIT(4, 1, 120, 14, 14), AI_STRIDE_INIT(4, 4, 4, 480, 6720),
  1, &_feat_feat_5_block_block_0_block_0_2_Clip_output_0_output_array, NULL)

/* Tensor #93 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_5_block_block_1_block_1_0_Conv_output_0_bias, AI_STATIC,
  93, 0x0,
  AI_SHAPE_INIT(4, 1, 120, 1, 1), AI_STRIDE_INIT(4, 4, 4, 480, 480),
  1, &_feat_feat_5_block_block_1_block_1_0_Conv_output_0_bias_array, NULL)

/* Tensor #94 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_5_block_block_1_block_1_0_Conv_output_0_output, AI_STATIC,
  94, 0x0,
  AI_SHAPE_INIT(4, 1, 120, 14, 14), AI_STRIDE_INIT(4, 4, 4, 480, 6720),
  1, &_feat_feat_5_block_block_1_block_1_0_Conv_output_0_output_array, NULL)

/* Tensor #95 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_5_block_block_1_block_1_0_Conv_output_0_weights, AI_STATIC,
  95, 0x0,
  AI_SHAPE_INIT(4, 1, 5, 5, 120), AI_STRIDE_INIT(4, 1, 120, 120, 120),
  1, &_feat_feat_5_block_block_1_block_1_0_Conv_output_0_weights_array, NULL)

/* Tensor #96 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_5_block_block_1_block_1_2_Clip_output_0_output, AI_STATIC,
  96, 0x0,
  AI_SHAPE_INIT(4, 1, 120, 14, 14), AI_STRIDE_INIT(4, 4, 4, 480, 6720),
  1, &_feat_feat_5_block_block_1_block_1_2_Clip_output_0_output_array, NULL)

/* Tensor #97 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_5_block_block_3_block_3_0_Conv_output_0_bias, AI_STATIC,
  97, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 1), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &_feat_feat_5_block_block_3_block_3_0_Conv_output_0_bias_array, NULL)

/* Tensor #98 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_5_block_block_3_block_3_0_Conv_output_0_output, AI_STATIC,
  98, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 14, 14), AI_STRIDE_INIT(4, 4, 4, 96, 1344),
  1, &_feat_feat_5_block_block_3_block_3_0_Conv_output_0_output_array, NULL)

/* Tensor #99 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_5_block_block_3_block_3_0_Conv_output_0_scratch0, AI_STATIC,
  99, 0x0,
  AI_SHAPE_INIT(4, 1, 120, 1, 1), AI_STRIDE_INIT(4, 4, 4, 480, 480),
  1, &_feat_feat_5_block_block_3_block_3_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #100 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_5_block_block_3_block_3_0_Conv_output_0_weights, AI_STATIC,
  100, 0x0,
  AI_SHAPE_INIT(4, 120, 1, 1, 24), AI_STRIDE_INIT(4, 4, 480, 11520, 11520),
  1, &_feat_feat_5_block_block_3_block_3_0_Conv_output_0_weights_array, NULL)

/* Tensor #101 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_6_Add_output_0_output, AI_STATIC,
  101, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 14, 14), AI_STRIDE_INIT(4, 4, 4, 96, 1344),
  1, &_feat_feat_6_Add_output_0_output_array, NULL)

/* Tensor #102 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_6_block_block_0_block_0_0_Conv_output_0_bias, AI_STATIC,
  102, 0x0,
  AI_SHAPE_INIT(4, 1, 120, 1, 1), AI_STRIDE_INIT(4, 4, 4, 480, 480),
  1, &_feat_feat_6_block_block_0_block_0_0_Conv_output_0_bias_array, NULL)

/* Tensor #103 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_6_block_block_0_block_0_0_Conv_output_0_output, AI_STATIC,
  103, 0x0,
  AI_SHAPE_INIT(4, 1, 120, 14, 14), AI_STRIDE_INIT(4, 4, 4, 480, 6720),
  1, &_feat_feat_6_block_block_0_block_0_0_Conv_output_0_output_array, NULL)

/* Tensor #104 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_6_block_block_0_block_0_0_Conv_output_0_scratch0, AI_STATIC,
  104, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 1), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &_feat_feat_6_block_block_0_block_0_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #105 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_6_block_block_0_block_0_0_Conv_output_0_weights, AI_STATIC,
  105, 0x0,
  AI_SHAPE_INIT(4, 24, 1, 1, 120), AI_STRIDE_INIT(4, 4, 96, 11520, 11520),
  1, &_feat_feat_6_block_block_0_block_0_0_Conv_output_0_weights_array, NULL)

/* Tensor #106 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_6_block_block_0_block_0_2_Clip_output_0_output, AI_STATIC,
  106, 0x0,
  AI_SHAPE_INIT(4, 1, 120, 14, 14), AI_STRIDE_INIT(4, 4, 4, 480, 6720),
  1, &_feat_feat_6_block_block_0_block_0_2_Clip_output_0_output_array, NULL)

/* Tensor #107 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_6_block_block_1_block_1_0_Conv_output_0_bias, AI_STATIC,
  107, 0x0,
  AI_SHAPE_INIT(4, 1, 120, 1, 1), AI_STRIDE_INIT(4, 4, 4, 480, 480),
  1, &_feat_feat_6_block_block_1_block_1_0_Conv_output_0_bias_array, NULL)

/* Tensor #108 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_6_block_block_1_block_1_0_Conv_output_0_output, AI_STATIC,
  108, 0x0,
  AI_SHAPE_INIT(4, 1, 120, 14, 14), AI_STRIDE_INIT(4, 4, 4, 480, 6720),
  1, &_feat_feat_6_block_block_1_block_1_0_Conv_output_0_output_array, NULL)

/* Tensor #109 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_6_block_block_1_block_1_0_Conv_output_0_weights, AI_STATIC,
  109, 0x0,
  AI_SHAPE_INIT(4, 1, 5, 5, 120), AI_STRIDE_INIT(4, 1, 120, 120, 120),
  1, &_feat_feat_6_block_block_1_block_1_0_Conv_output_0_weights_array, NULL)

/* Tensor #110 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_6_block_block_1_block_1_2_Clip_output_0_output, AI_STATIC,
  110, 0x0,
  AI_SHAPE_INIT(4, 1, 120, 14, 14), AI_STRIDE_INIT(4, 4, 4, 480, 6720),
  1, &_feat_feat_6_block_block_1_block_1_2_Clip_output_0_output_array, NULL)

/* Tensor #111 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_6_block_block_3_block_3_0_Conv_output_0_bias, AI_STATIC,
  111, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 1), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &_feat_feat_6_block_block_3_block_3_0_Conv_output_0_bias_array, NULL)

/* Tensor #112 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_6_block_block_3_block_3_0_Conv_output_0_output, AI_STATIC,
  112, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 14, 14), AI_STRIDE_INIT(4, 4, 4, 96, 1344),
  1, &_feat_feat_6_block_block_3_block_3_0_Conv_output_0_output_array, NULL)

/* Tensor #113 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_6_block_block_3_block_3_0_Conv_output_0_scratch0, AI_STATIC,
  113, 0x0,
  AI_SHAPE_INIT(4, 1, 120, 1, 1), AI_STRIDE_INIT(4, 4, 4, 480, 480),
  1, &_feat_feat_6_block_block_3_block_3_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #114 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_6_block_block_3_block_3_0_Conv_output_0_weights, AI_STATIC,
  114, 0x0,
  AI_SHAPE_INIT(4, 120, 1, 1, 24), AI_STRIDE_INIT(4, 4, 480, 11520, 11520),
  1, &_feat_feat_6_block_block_3_block_3_0_Conv_output_0_weights_array, NULL)

/* Tensor #115 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_7_Add_output_0_output, AI_STATIC,
  115, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 14, 14), AI_STRIDE_INIT(4, 4, 4, 96, 1344),
  1, &_feat_feat_7_Add_output_0_output_array, NULL)

/* Tensor #116 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_7_block_block_0_block_0_0_Conv_output_0_bias, AI_STATIC,
  116, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &_feat_feat_7_block_block_0_block_0_0_Conv_output_0_bias_array, NULL)

/* Tensor #117 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_7_block_block_0_block_0_0_Conv_output_0_output, AI_STATIC,
  117, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 14, 14), AI_STRIDE_INIT(4, 4, 4, 256, 3584),
  1, &_feat_feat_7_block_block_0_block_0_0_Conv_output_0_output_array, NULL)

/* Tensor #118 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_7_block_block_0_block_0_0_Conv_output_0_scratch0, AI_STATIC,
  118, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 1), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &_feat_feat_7_block_block_0_block_0_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #119 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_7_block_block_0_block_0_0_Conv_output_0_weights, AI_STATIC,
  119, 0x0,
  AI_SHAPE_INIT(4, 24, 1, 1, 64), AI_STRIDE_INIT(4, 4, 96, 6144, 6144),
  1, &_feat_feat_7_block_block_0_block_0_0_Conv_output_0_weights_array, NULL)

/* Tensor #120 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_7_block_block_0_block_0_2_Clip_output_0_output, AI_STATIC,
  120, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 14, 14), AI_STRIDE_INIT(4, 4, 4, 256, 3584),
  1, &_feat_feat_7_block_block_0_block_0_2_Clip_output_0_output_array, NULL)

/* Tensor #121 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_7_block_block_1_block_1_0_Conv_output_0_bias, AI_STATIC,
  121, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &_feat_feat_7_block_block_1_block_1_0_Conv_output_0_bias_array, NULL)

/* Tensor #122 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_7_block_block_1_block_1_0_Conv_output_0_output, AI_STATIC,
  122, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 14, 14), AI_STRIDE_INIT(4, 4, 4, 256, 3584),
  1, &_feat_feat_7_block_block_1_block_1_0_Conv_output_0_output_array, NULL)

/* Tensor #123 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_7_block_block_1_block_1_0_Conv_output_0_weights, AI_STATIC,
  123, 0x0,
  AI_SHAPE_INIT(4, 1, 5, 5, 64), AI_STRIDE_INIT(4, 1, 64, 64, 64),
  1, &_feat_feat_7_block_block_1_block_1_0_Conv_output_0_weights_array, NULL)

/* Tensor #124 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_7_block_block_1_block_1_2_Clip_output_0_output, AI_STATIC,
  124, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 14, 14), AI_STRIDE_INIT(4, 4, 4, 256, 3584),
  1, &_feat_feat_7_block_block_1_block_1_2_Clip_output_0_output_array, NULL)

/* Tensor #125 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_7_block_block_3_block_3_0_Conv_output_0_bias, AI_STATIC,
  125, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 1), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &_feat_feat_7_block_block_3_block_3_0_Conv_output_0_bias_array, NULL)

/* Tensor #126 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_7_block_block_3_block_3_0_Conv_output_0_output, AI_STATIC,
  126, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 14, 14), AI_STRIDE_INIT(4, 4, 4, 96, 1344),
  1, &_feat_feat_7_block_block_3_block_3_0_Conv_output_0_output_array, NULL)

/* Tensor #127 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_7_block_block_3_block_3_0_Conv_output_0_scratch0, AI_STATIC,
  127, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &_feat_feat_7_block_block_3_block_3_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #128 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_7_block_block_3_block_3_0_Conv_output_0_weights, AI_STATIC,
  128, 0x0,
  AI_SHAPE_INIT(4, 64, 1, 1, 24), AI_STRIDE_INIT(4, 4, 256, 6144, 6144),
  1, &_feat_feat_7_block_block_3_block_3_0_Conv_output_0_weights_array, NULL)

/* Tensor #129 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_8_Add_output_0_output, AI_STATIC,
  129, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 14, 14), AI_STRIDE_INIT(4, 4, 4, 96, 1344),
  1, &_feat_feat_8_Add_output_0_output_array, NULL)

/* Tensor #130 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_8_block_block_0_block_0_0_Conv_output_0_bias, AI_STATIC,
  130, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 1, 1), AI_STRIDE_INIT(4, 4, 4, 288, 288),
  1, &_feat_feat_8_block_block_0_block_0_0_Conv_output_0_bias_array, NULL)

/* Tensor #131 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_8_block_block_0_block_0_0_Conv_output_0_output, AI_STATIC,
  131, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 14, 14), AI_STRIDE_INIT(4, 4, 4, 288, 4032),
  1, &_feat_feat_8_block_block_0_block_0_0_Conv_output_0_output_array, NULL)

/* Tensor #132 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_8_block_block_0_block_0_0_Conv_output_0_scratch0, AI_STATIC,
  132, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 1), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &_feat_feat_8_block_block_0_block_0_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #133 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_8_block_block_0_block_0_0_Conv_output_0_weights, AI_STATIC,
  133, 0x0,
  AI_SHAPE_INIT(4, 24, 1, 1, 72), AI_STRIDE_INIT(4, 4, 96, 6912, 6912),
  1, &_feat_feat_8_block_block_0_block_0_0_Conv_output_0_weights_array, NULL)

/* Tensor #134 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_8_block_block_0_block_0_2_Clip_output_0_output, AI_STATIC,
  134, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 14, 14), AI_STRIDE_INIT(4, 4, 4, 288, 4032),
  1, &_feat_feat_8_block_block_0_block_0_2_Clip_output_0_output_array, NULL)

/* Tensor #135 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_8_block_block_1_block_1_0_Conv_output_0_bias, AI_STATIC,
  135, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 1, 1), AI_STRIDE_INIT(4, 4, 4, 288, 288),
  1, &_feat_feat_8_block_block_1_block_1_0_Conv_output_0_bias_array, NULL)

/* Tensor #136 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_8_block_block_1_block_1_0_Conv_output_0_output, AI_STATIC,
  136, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 14, 14), AI_STRIDE_INIT(4, 4, 4, 288, 4032),
  1, &_feat_feat_8_block_block_1_block_1_0_Conv_output_0_output_array, NULL)

/* Tensor #137 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_8_block_block_1_block_1_0_Conv_output_0_weights, AI_STATIC,
  137, 0x0,
  AI_SHAPE_INIT(4, 1, 5, 5, 72), AI_STRIDE_INIT(4, 1, 72, 72, 72),
  1, &_feat_feat_8_block_block_1_block_1_0_Conv_output_0_weights_array, NULL)

/* Tensor #138 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_8_block_block_1_block_1_2_Clip_output_0_output, AI_STATIC,
  138, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 14, 14), AI_STRIDE_INIT(4, 4, 4, 288, 4032),
  1, &_feat_feat_8_block_block_1_block_1_2_Clip_output_0_output_array, NULL)

/* Tensor #139 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_8_block_block_3_block_3_0_Conv_output_0_bias, AI_STATIC,
  139, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 1), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &_feat_feat_8_block_block_3_block_3_0_Conv_output_0_bias_array, NULL)

/* Tensor #140 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_8_block_block_3_block_3_0_Conv_output_0_output, AI_STATIC,
  140, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 14, 14), AI_STRIDE_INIT(4, 4, 4, 96, 1344),
  1, &_feat_feat_8_block_block_3_block_3_0_Conv_output_0_output_array, NULL)

/* Tensor #141 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_8_block_block_3_block_3_0_Conv_output_0_scratch0, AI_STATIC,
  141, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 1, 1), AI_STRIDE_INIT(4, 4, 4, 288, 288),
  1, &_feat_feat_8_block_block_3_block_3_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #142 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_8_block_block_3_block_3_0_Conv_output_0_weights, AI_STATIC,
  142, 0x0,
  AI_SHAPE_INIT(4, 72, 1, 1, 24), AI_STRIDE_INIT(4, 4, 288, 6912, 6912),
  1, &_feat_feat_8_block_block_3_block_3_0_Conv_output_0_weights_array, NULL)

/* Tensor #143 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_9_block_block_0_block_0_0_Conv_output_0_bias, AI_STATIC,
  143, 0x0,
  AI_SHAPE_INIT(4, 1, 144, 1, 1), AI_STRIDE_INIT(4, 4, 4, 576, 576),
  1, &_feat_feat_9_block_block_0_block_0_0_Conv_output_0_bias_array, NULL)

/* Tensor #144 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_9_block_block_0_block_0_0_Conv_output_0_output, AI_STATIC,
  144, 0x0,
  AI_SHAPE_INIT(4, 1, 144, 14, 14), AI_STRIDE_INIT(4, 4, 4, 576, 8064),
  1, &_feat_feat_9_block_block_0_block_0_0_Conv_output_0_output_array, NULL)

/* Tensor #145 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_9_block_block_0_block_0_0_Conv_output_0_scratch0, AI_STATIC,
  145, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 1), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &_feat_feat_9_block_block_0_block_0_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #146 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_9_block_block_0_block_0_0_Conv_output_0_weights, AI_STATIC,
  146, 0x0,
  AI_SHAPE_INIT(4, 24, 1, 1, 144), AI_STRIDE_INIT(4, 4, 96, 13824, 13824),
  1, &_feat_feat_9_block_block_0_block_0_0_Conv_output_0_weights_array, NULL)

/* Tensor #147 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_9_block_block_0_block_0_2_Clip_output_0_output, AI_STATIC,
  147, 0x0,
  AI_SHAPE_INIT(4, 1, 144, 14, 14), AI_STRIDE_INIT(4, 4, 4, 576, 8064),
  1, &_feat_feat_9_block_block_0_block_0_2_Clip_output_0_output_array, NULL)

/* Tensor #148 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_9_block_block_1_block_1_0_Conv_output_0_bias, AI_STATIC,
  148, 0x0,
  AI_SHAPE_INIT(4, 1, 144, 1, 1), AI_STRIDE_INIT(4, 4, 4, 576, 576),
  1, &_feat_feat_9_block_block_1_block_1_0_Conv_output_0_bias_array, NULL)

/* Tensor #149 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_9_block_block_1_block_1_0_Conv_output_0_output, AI_STATIC,
  149, 0x0,
  AI_SHAPE_INIT(4, 1, 144, 7, 7), AI_STRIDE_INIT(4, 4, 4, 576, 4032),
  1, &_feat_feat_9_block_block_1_block_1_0_Conv_output_0_output_array, NULL)

/* Tensor #150 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_9_block_block_1_block_1_0_Conv_output_0_weights, AI_STATIC,
  150, 0x0,
  AI_SHAPE_INIT(4, 1, 5, 5, 144), AI_STRIDE_INIT(4, 1, 144, 144, 144),
  1, &_feat_feat_9_block_block_1_block_1_0_Conv_output_0_weights_array, NULL)

/* Tensor #151 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_9_block_block_1_block_1_2_Clip_output_0_output, AI_STATIC,
  151, 0x0,
  AI_SHAPE_INIT(4, 1, 144, 7, 7), AI_STRIDE_INIT(4, 4, 4, 576, 4032),
  1, &_feat_feat_9_block_block_1_block_1_2_Clip_output_0_output_array, NULL)

/* Tensor #152 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_9_block_block_3_block_3_0_Conv_output_0_bias, AI_STATIC,
  152, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 1, 1), AI_STRIDE_INIT(4, 4, 4, 192, 192),
  1, &_feat_feat_9_block_block_3_block_3_0_Conv_output_0_bias_array, NULL)

/* Tensor #153 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_9_block_block_3_block_3_0_Conv_output_0_output, AI_STATIC,
  153, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 7, 7), AI_STRIDE_INIT(4, 4, 4, 192, 1344),
  1, &_feat_feat_9_block_block_3_block_3_0_Conv_output_0_output_array, NULL)

/* Tensor #154 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_9_block_block_3_block_3_0_Conv_output_0_scratch0, AI_STATIC,
  154, 0x0,
  AI_SHAPE_INIT(4, 1, 144, 1, 1), AI_STRIDE_INIT(4, 4, 4, 576, 576),
  1, &_feat_feat_9_block_block_3_block_3_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #155 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_9_block_block_3_block_3_0_Conv_output_0_weights, AI_STATIC,
  155, 0x0,
  AI_SHAPE_INIT(4, 144, 1, 1, 48), AI_STRIDE_INIT(4, 4, 576, 27648, 27648),
  1, &_feat_feat_9_block_block_3_block_3_0_Conv_output_0_weights_array, NULL)

/* Tensor #156 */
AI_TENSOR_OBJ_DECLARE(
  _pool_GlobalAveragePool_output_0_output, AI_STATIC,
  156, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &_pool_GlobalAveragePool_output_0_output_array, NULL)

/* Tensor #157 */
AI_TENSOR_OBJ_DECLARE(
  _reg_head_Gemm_output_0_bias, AI_STATIC,
  157, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &_reg_head_Gemm_output_0_bias_array, NULL)

/* Tensor #158 */
AI_TENSOR_OBJ_DECLARE(
  _reg_head_Gemm_output_0_output, AI_STATIC,
  158, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &_reg_head_Gemm_output_0_output_array, NULL)

/* Tensor #159 */
AI_TENSOR_OBJ_DECLARE(
  _reg_head_Gemm_output_0_weights, AI_STATIC,
  159, 0x0,
  AI_SHAPE_INIT(4, 128, 1, 1, 1), AI_STRIDE_INIT(4, 4, 512, 512, 512),
  1, &_reg_head_Gemm_output_0_weights_array, NULL)

/* Tensor #160 */
AI_TENSOR_OBJ_DECLARE(
  _shared_shared_1_Gemm_output_0_bias, AI_STATIC,
  160, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &_shared_shared_1_Gemm_output_0_bias_array, NULL)

/* Tensor #161 */
AI_TENSOR_OBJ_DECLARE(
  _shared_shared_1_Gemm_output_0_output, AI_STATIC,
  161, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &_shared_shared_1_Gemm_output_0_output_array, NULL)

/* Tensor #162 */
AI_TENSOR_OBJ_DECLARE(
  _shared_shared_1_Gemm_output_0_weights, AI_STATIC,
  162, 0x0,
  AI_SHAPE_INIT(4, 288, 128, 1, 1), AI_STRIDE_INIT(4, 4, 1152, 147456, 147456),
  1, &_shared_shared_1_Gemm_output_0_weights_array, NULL)

/* Tensor #163 */
AI_TENSOR_OBJ_DECLARE(
  _shared_shared_2_Clip_output_0_output, AI_STATIC,
  163, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &_shared_shared_2_Clip_output_0_output_array, NULL)

/* Tensor #164 */
AI_TENSOR_OBJ_DECLARE(
  input_output, AI_STATIC,
  164, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 224, 224), AI_STRIDE_INIT(4, 4, 4, 4, 896),
  1, &input_output_array, NULL)

/* Tensor #165 */
AI_TENSOR_OBJ_DECLARE(
  logits_bias, AI_STATIC,
  165, 0x0,
  AI_SHAPE_INIT(4, 1, 5, 1, 1), AI_STRIDE_INIT(4, 4, 4, 20, 20),
  1, &logits_bias_array, NULL)

/* Tensor #166 */
AI_TENSOR_OBJ_DECLARE(
  logits_output, AI_STATIC,
  166, 0x0,
  AI_SHAPE_INIT(4, 1, 5, 1, 1), AI_STRIDE_INIT(4, 4, 4, 20, 20),
  1, &logits_output_array, NULL)

/* Tensor #167 */
AI_TENSOR_OBJ_DECLARE(
  logits_weights, AI_STATIC,
  167, 0x0,
  AI_SHAPE_INIT(4, 128, 5, 1, 1), AI_STRIDE_INIT(4, 4, 512, 2560, 2560),
  1, &logits_weights_array, NULL)



/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  _Sigmoid_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_reg_head_Gemm_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_Sigmoid_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _Sigmoid_output_0_layer, 109,
  NL_TYPE, 0x0, NULL,
  nl, forward_sigmoid,
  &_Sigmoid_output_0_chain,
  NULL, &_Sigmoid_output_0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _reg_head_Gemm_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_shared_shared_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_reg_head_Gemm_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_reg_head_Gemm_output_0_weights, &_reg_head_Gemm_output_0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _reg_head_Gemm_output_0_layer, 108,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense,
  &_reg_head_Gemm_output_0_chain,
  NULL, &_Sigmoid_output_0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  logits_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_shared_shared_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &logits_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &logits_weights, &logits_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  logits_layer, 107,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense,
  &logits_chain,
  NULL, &_reg_head_Gemm_output_0_layer, AI_STATIC, 
)


AI_STATIC_CONST ai_float _shared_shared_2_Clip_output_0_nl_params_data[] = { 0.0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    _shared_shared_2_Clip_output_0_nl_params, AI_ARRAY_FORMAT_FLOAT,
    _shared_shared_2_Clip_output_0_nl_params_data, _shared_shared_2_Clip_output_0_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  _shared_shared_2_Clip_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_shared_shared_1_Gemm_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_shared_shared_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _shared_shared_2_Clip_output_0_layer, 106,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &_shared_shared_2_Clip_output_0_chain,
  NULL, &logits_layer, AI_STATIC, 
  .nl_params = &_shared_shared_2_Clip_output_0_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _shared_shared_1_Gemm_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_pool_GlobalAveragePool_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_shared_shared_1_Gemm_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_shared_shared_1_Gemm_output_0_weights, &_shared_shared_1_Gemm_output_0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _shared_shared_1_Gemm_output_0_layer, 103,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense,
  &_shared_shared_1_Gemm_output_0_chain,
  NULL, &_shared_shared_2_Clip_output_0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _pool_GlobalAveragePool_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_12_feat_12_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_pool_GlobalAveragePool_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _pool_GlobalAveragePool_output_0_layer, 101,
  POOL_TYPE, 0x0, NULL,
  pool, forward_ap,
  &_pool_GlobalAveragePool_output_0_chain,
  NULL, &_shared_shared_1_Gemm_output_0_layer, AI_STATIC, 
  .pool_size = AI_SHAPE_2D_INIT(7, 7), 
  .pool_stride = AI_SHAPE_2D_INIT(7, 7), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)


AI_STATIC_CONST ai_float _feat_feat_12_feat_12_2_Clip_output_0_nl_params_data[] = { 0.0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    _feat_feat_12_feat_12_2_Clip_output_0_nl_params, AI_ARRAY_FORMAT_FLOAT,
    _feat_feat_12_feat_12_2_Clip_output_0_nl_params_data, _feat_feat_12_feat_12_2_Clip_output_0_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_12_feat_12_2_Clip_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_12_feat_12_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_12_feat_12_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_12_feat_12_2_Clip_output_0_layer, 100,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &_feat_feat_12_feat_12_2_Clip_output_0_chain,
  NULL, &_pool_GlobalAveragePool_output_0_layer, AI_STATIC, 
  .nl_params = &_feat_feat_12_feat_12_2_Clip_output_0_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_12_feat_12_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_11_Add_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_12_feat_12_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_12_feat_12_0_Conv_output_0_weights, &_feat_feat_12_feat_12_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_12_feat_12_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_12_feat_12_0_Conv_output_0_layer, 97,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_feat_feat_12_feat_12_0_Conv_output_0_chain,
  NULL, &_feat_feat_12_feat_12_2_Clip_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_11_Add_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_11_block_block_3_block_3_0_Conv_output_0_output, &_feat_feat_10_Add_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_11_Add_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_11_Add_output_0_layer, 96,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &_feat_feat_11_Add_output_0_chain,
  NULL, &_feat_feat_12_feat_12_0_Conv_output_0_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_11_block_block_3_block_3_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_11_block_block_1_block_1_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_11_block_block_3_block_3_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_11_block_block_3_block_3_0_Conv_output_0_weights, &_feat_feat_11_block_block_3_block_3_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_11_block_block_3_block_3_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_11_block_block_3_block_3_0_Conv_output_0_layer, 95,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_feat_feat_11_block_block_3_block_3_0_Conv_output_0_chain,
  NULL, &_feat_feat_11_Add_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float _feat_feat_11_block_block_1_block_1_2_Clip_output_0_nl_params_data[] = { 0.0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    _feat_feat_11_block_block_1_block_1_2_Clip_output_0_nl_params, AI_ARRAY_FORMAT_FLOAT,
    _feat_feat_11_block_block_1_block_1_2_Clip_output_0_nl_params_data, _feat_feat_11_block_block_1_block_1_2_Clip_output_0_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_11_block_block_1_block_1_2_Clip_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_11_block_block_1_block_1_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_11_block_block_1_block_1_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_11_block_block_1_block_1_2_Clip_output_0_layer, 94,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &_feat_feat_11_block_block_1_block_1_2_Clip_output_0_chain,
  NULL, &_feat_feat_11_block_block_3_block_3_0_Conv_output_0_layer, AI_STATIC, 
  .nl_params = &_feat_feat_11_block_block_1_block_1_2_Clip_output_0_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_11_block_block_1_block_1_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_11_block_block_0_block_0_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_11_block_block_1_block_1_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_11_block_block_1_block_1_0_Conv_output_0_weights, &_feat_feat_11_block_block_1_block_1_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_11_block_block_1_block_1_0_Conv_output_0_layer, 91,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &_feat_feat_11_block_block_1_block_1_0_Conv_output_0_chain,
  NULL, &_feat_feat_11_block_block_1_block_1_2_Clip_output_0_layer, AI_STATIC, 
  .groups = 288, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 2, 2, 2, 2), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float _feat_feat_11_block_block_0_block_0_2_Clip_output_0_nl_params_data[] = { 0.0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    _feat_feat_11_block_block_0_block_0_2_Clip_output_0_nl_params, AI_ARRAY_FORMAT_FLOAT,
    _feat_feat_11_block_block_0_block_0_2_Clip_output_0_nl_params_data, _feat_feat_11_block_block_0_block_0_2_Clip_output_0_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_11_block_block_0_block_0_2_Clip_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_11_block_block_0_block_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_11_block_block_0_block_0_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_11_block_block_0_block_0_2_Clip_output_0_layer, 90,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &_feat_feat_11_block_block_0_block_0_2_Clip_output_0_chain,
  NULL, &_feat_feat_11_block_block_1_block_1_0_Conv_output_0_layer, AI_STATIC, 
  .nl_params = &_feat_feat_11_block_block_0_block_0_2_Clip_output_0_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_11_block_block_0_block_0_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_10_Add_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_11_block_block_0_block_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_11_block_block_0_block_0_0_Conv_output_0_weights, &_feat_feat_11_block_block_0_block_0_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_11_block_block_0_block_0_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_11_block_block_0_block_0_0_Conv_output_0_layer, 87,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_feat_feat_11_block_block_0_block_0_0_Conv_output_0_chain,
  NULL, &_feat_feat_11_block_block_0_block_0_2_Clip_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_10_Add_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_10_block_block_3_block_3_0_Conv_output_0_output, &_feat_feat_9_block_block_3_block_3_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_10_Add_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_10_Add_output_0_layer, 86,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &_feat_feat_10_Add_output_0_chain,
  NULL, &_feat_feat_11_block_block_0_block_0_0_Conv_output_0_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_10_block_block_3_block_3_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_10_block_block_1_block_1_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_10_block_block_3_block_3_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_10_block_block_3_block_3_0_Conv_output_0_weights, &_feat_feat_10_block_block_3_block_3_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_10_block_block_3_block_3_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_10_block_block_3_block_3_0_Conv_output_0_layer, 85,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_feat_feat_10_block_block_3_block_3_0_Conv_output_0_chain,
  NULL, &_feat_feat_10_Add_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float _feat_feat_10_block_block_1_block_1_2_Clip_output_0_nl_params_data[] = { 0.0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    _feat_feat_10_block_block_1_block_1_2_Clip_output_0_nl_params, AI_ARRAY_FORMAT_FLOAT,
    _feat_feat_10_block_block_1_block_1_2_Clip_output_0_nl_params_data, _feat_feat_10_block_block_1_block_1_2_Clip_output_0_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_10_block_block_1_block_1_2_Clip_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_10_block_block_1_block_1_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_10_block_block_1_block_1_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_10_block_block_1_block_1_2_Clip_output_0_layer, 84,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &_feat_feat_10_block_block_1_block_1_2_Clip_output_0_chain,
  NULL, &_feat_feat_10_block_block_3_block_3_0_Conv_output_0_layer, AI_STATIC, 
  .nl_params = &_feat_feat_10_block_block_1_block_1_2_Clip_output_0_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_10_block_block_1_block_1_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_10_block_block_0_block_0_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_10_block_block_1_block_1_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_10_block_block_1_block_1_0_Conv_output_0_weights, &_feat_feat_10_block_block_1_block_1_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_10_block_block_1_block_1_0_Conv_output_0_layer, 81,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &_feat_feat_10_block_block_1_block_1_0_Conv_output_0_chain,
  NULL, &_feat_feat_10_block_block_1_block_1_2_Clip_output_0_layer, AI_STATIC, 
  .groups = 288, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 2, 2, 2, 2), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float _feat_feat_10_block_block_0_block_0_2_Clip_output_0_nl_params_data[] = { 0.0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    _feat_feat_10_block_block_0_block_0_2_Clip_output_0_nl_params, AI_ARRAY_FORMAT_FLOAT,
    _feat_feat_10_block_block_0_block_0_2_Clip_output_0_nl_params_data, _feat_feat_10_block_block_0_block_0_2_Clip_output_0_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_10_block_block_0_block_0_2_Clip_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_10_block_block_0_block_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_10_block_block_0_block_0_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_10_block_block_0_block_0_2_Clip_output_0_layer, 80,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &_feat_feat_10_block_block_0_block_0_2_Clip_output_0_chain,
  NULL, &_feat_feat_10_block_block_1_block_1_0_Conv_output_0_layer, AI_STATIC, 
  .nl_params = &_feat_feat_10_block_block_0_block_0_2_Clip_output_0_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_10_block_block_0_block_0_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_9_block_block_3_block_3_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_10_block_block_0_block_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_10_block_block_0_block_0_0_Conv_output_0_weights, &_feat_feat_10_block_block_0_block_0_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_10_block_block_0_block_0_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_10_block_block_0_block_0_0_Conv_output_0_layer, 77,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_feat_feat_10_block_block_0_block_0_0_Conv_output_0_chain,
  NULL, &_feat_feat_10_block_block_0_block_0_2_Clip_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_9_block_block_3_block_3_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_9_block_block_1_block_1_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_9_block_block_3_block_3_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_9_block_block_3_block_3_0_Conv_output_0_weights, &_feat_feat_9_block_block_3_block_3_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_9_block_block_3_block_3_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_9_block_block_3_block_3_0_Conv_output_0_layer, 76,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_feat_feat_9_block_block_3_block_3_0_Conv_output_0_chain,
  NULL, &_feat_feat_10_block_block_0_block_0_0_Conv_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float _feat_feat_9_block_block_1_block_1_2_Clip_output_0_nl_params_data[] = { 0.0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    _feat_feat_9_block_block_1_block_1_2_Clip_output_0_nl_params, AI_ARRAY_FORMAT_FLOAT,
    _feat_feat_9_block_block_1_block_1_2_Clip_output_0_nl_params_data, _feat_feat_9_block_block_1_block_1_2_Clip_output_0_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_9_block_block_1_block_1_2_Clip_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_9_block_block_1_block_1_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_9_block_block_1_block_1_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_9_block_block_1_block_1_2_Clip_output_0_layer, 75,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &_feat_feat_9_block_block_1_block_1_2_Clip_output_0_chain,
  NULL, &_feat_feat_9_block_block_3_block_3_0_Conv_output_0_layer, AI_STATIC, 
  .nl_params = &_feat_feat_9_block_block_1_block_1_2_Clip_output_0_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_9_block_block_1_block_1_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_9_block_block_0_block_0_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_9_block_block_1_block_1_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_9_block_block_1_block_1_0_Conv_output_0_weights, &_feat_feat_9_block_block_1_block_1_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_9_block_block_1_block_1_0_Conv_output_0_layer, 72,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &_feat_feat_9_block_block_1_block_1_0_Conv_output_0_chain,
  NULL, &_feat_feat_9_block_block_1_block_1_2_Clip_output_0_layer, AI_STATIC, 
  .groups = 144, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 2, 2, 2, 2), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float _feat_feat_9_block_block_0_block_0_2_Clip_output_0_nl_params_data[] = { 0.0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    _feat_feat_9_block_block_0_block_0_2_Clip_output_0_nl_params, AI_ARRAY_FORMAT_FLOAT,
    _feat_feat_9_block_block_0_block_0_2_Clip_output_0_nl_params_data, _feat_feat_9_block_block_0_block_0_2_Clip_output_0_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_9_block_block_0_block_0_2_Clip_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_9_block_block_0_block_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_9_block_block_0_block_0_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_9_block_block_0_block_0_2_Clip_output_0_layer, 71,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &_feat_feat_9_block_block_0_block_0_2_Clip_output_0_chain,
  NULL, &_feat_feat_9_block_block_1_block_1_0_Conv_output_0_layer, AI_STATIC, 
  .nl_params = &_feat_feat_9_block_block_0_block_0_2_Clip_output_0_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_9_block_block_0_block_0_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_8_Add_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_9_block_block_0_block_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_9_block_block_0_block_0_0_Conv_output_0_weights, &_feat_feat_9_block_block_0_block_0_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_9_block_block_0_block_0_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_9_block_block_0_block_0_0_Conv_output_0_layer, 68,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_feat_feat_9_block_block_0_block_0_0_Conv_output_0_chain,
  NULL, &_feat_feat_9_block_block_0_block_0_2_Clip_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_8_Add_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_8_block_block_3_block_3_0_Conv_output_0_output, &_feat_feat_7_Add_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_8_Add_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_8_Add_output_0_layer, 67,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &_feat_feat_8_Add_output_0_chain,
  NULL, &_feat_feat_9_block_block_0_block_0_0_Conv_output_0_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_8_block_block_3_block_3_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_8_block_block_1_block_1_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_8_block_block_3_block_3_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_8_block_block_3_block_3_0_Conv_output_0_weights, &_feat_feat_8_block_block_3_block_3_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_8_block_block_3_block_3_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_8_block_block_3_block_3_0_Conv_output_0_layer, 66,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_feat_feat_8_block_block_3_block_3_0_Conv_output_0_chain,
  NULL, &_feat_feat_8_Add_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float _feat_feat_8_block_block_1_block_1_2_Clip_output_0_nl_params_data[] = { 0.0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    _feat_feat_8_block_block_1_block_1_2_Clip_output_0_nl_params, AI_ARRAY_FORMAT_FLOAT,
    _feat_feat_8_block_block_1_block_1_2_Clip_output_0_nl_params_data, _feat_feat_8_block_block_1_block_1_2_Clip_output_0_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_8_block_block_1_block_1_2_Clip_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_8_block_block_1_block_1_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_8_block_block_1_block_1_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_8_block_block_1_block_1_2_Clip_output_0_layer, 65,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &_feat_feat_8_block_block_1_block_1_2_Clip_output_0_chain,
  NULL, &_feat_feat_8_block_block_3_block_3_0_Conv_output_0_layer, AI_STATIC, 
  .nl_params = &_feat_feat_8_block_block_1_block_1_2_Clip_output_0_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_8_block_block_1_block_1_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_8_block_block_0_block_0_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_8_block_block_1_block_1_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_8_block_block_1_block_1_0_Conv_output_0_weights, &_feat_feat_8_block_block_1_block_1_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_8_block_block_1_block_1_0_Conv_output_0_layer, 62,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &_feat_feat_8_block_block_1_block_1_0_Conv_output_0_chain,
  NULL, &_feat_feat_8_block_block_1_block_1_2_Clip_output_0_layer, AI_STATIC, 
  .groups = 72, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 2, 2, 2, 2), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float _feat_feat_8_block_block_0_block_0_2_Clip_output_0_nl_params_data[] = { 0.0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    _feat_feat_8_block_block_0_block_0_2_Clip_output_0_nl_params, AI_ARRAY_FORMAT_FLOAT,
    _feat_feat_8_block_block_0_block_0_2_Clip_output_0_nl_params_data, _feat_feat_8_block_block_0_block_0_2_Clip_output_0_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_8_block_block_0_block_0_2_Clip_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_8_block_block_0_block_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_8_block_block_0_block_0_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_8_block_block_0_block_0_2_Clip_output_0_layer, 61,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &_feat_feat_8_block_block_0_block_0_2_Clip_output_0_chain,
  NULL, &_feat_feat_8_block_block_1_block_1_0_Conv_output_0_layer, AI_STATIC, 
  .nl_params = &_feat_feat_8_block_block_0_block_0_2_Clip_output_0_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_8_block_block_0_block_0_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_7_Add_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_8_block_block_0_block_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_8_block_block_0_block_0_0_Conv_output_0_weights, &_feat_feat_8_block_block_0_block_0_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_8_block_block_0_block_0_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_8_block_block_0_block_0_0_Conv_output_0_layer, 58,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_feat_feat_8_block_block_0_block_0_0_Conv_output_0_chain,
  NULL, &_feat_feat_8_block_block_0_block_0_2_Clip_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_7_Add_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_7_block_block_3_block_3_0_Conv_output_0_output, &_feat_feat_6_Add_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_7_Add_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_7_Add_output_0_layer, 57,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &_feat_feat_7_Add_output_0_chain,
  NULL, &_feat_feat_8_block_block_0_block_0_0_Conv_output_0_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_7_block_block_3_block_3_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_7_block_block_1_block_1_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_7_block_block_3_block_3_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_7_block_block_3_block_3_0_Conv_output_0_weights, &_feat_feat_7_block_block_3_block_3_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_7_block_block_3_block_3_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_7_block_block_3_block_3_0_Conv_output_0_layer, 56,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_feat_feat_7_block_block_3_block_3_0_Conv_output_0_chain,
  NULL, &_feat_feat_7_Add_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float _feat_feat_7_block_block_1_block_1_2_Clip_output_0_nl_params_data[] = { 0.0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    _feat_feat_7_block_block_1_block_1_2_Clip_output_0_nl_params, AI_ARRAY_FORMAT_FLOAT,
    _feat_feat_7_block_block_1_block_1_2_Clip_output_0_nl_params_data, _feat_feat_7_block_block_1_block_1_2_Clip_output_0_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_7_block_block_1_block_1_2_Clip_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_7_block_block_1_block_1_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_7_block_block_1_block_1_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_7_block_block_1_block_1_2_Clip_output_0_layer, 55,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &_feat_feat_7_block_block_1_block_1_2_Clip_output_0_chain,
  NULL, &_feat_feat_7_block_block_3_block_3_0_Conv_output_0_layer, AI_STATIC, 
  .nl_params = &_feat_feat_7_block_block_1_block_1_2_Clip_output_0_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_7_block_block_1_block_1_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_7_block_block_0_block_0_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_7_block_block_1_block_1_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_7_block_block_1_block_1_0_Conv_output_0_weights, &_feat_feat_7_block_block_1_block_1_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_7_block_block_1_block_1_0_Conv_output_0_layer, 52,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &_feat_feat_7_block_block_1_block_1_0_Conv_output_0_chain,
  NULL, &_feat_feat_7_block_block_1_block_1_2_Clip_output_0_layer, AI_STATIC, 
  .groups = 64, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 2, 2, 2, 2), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float _feat_feat_7_block_block_0_block_0_2_Clip_output_0_nl_params_data[] = { 0.0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    _feat_feat_7_block_block_0_block_0_2_Clip_output_0_nl_params, AI_ARRAY_FORMAT_FLOAT,
    _feat_feat_7_block_block_0_block_0_2_Clip_output_0_nl_params_data, _feat_feat_7_block_block_0_block_0_2_Clip_output_0_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_7_block_block_0_block_0_2_Clip_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_7_block_block_0_block_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_7_block_block_0_block_0_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_7_block_block_0_block_0_2_Clip_output_0_layer, 51,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &_feat_feat_7_block_block_0_block_0_2_Clip_output_0_chain,
  NULL, &_feat_feat_7_block_block_1_block_1_0_Conv_output_0_layer, AI_STATIC, 
  .nl_params = &_feat_feat_7_block_block_0_block_0_2_Clip_output_0_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_7_block_block_0_block_0_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_6_Add_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_7_block_block_0_block_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_7_block_block_0_block_0_0_Conv_output_0_weights, &_feat_feat_7_block_block_0_block_0_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_7_block_block_0_block_0_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_7_block_block_0_block_0_0_Conv_output_0_layer, 48,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_feat_feat_7_block_block_0_block_0_0_Conv_output_0_chain,
  NULL, &_feat_feat_7_block_block_0_block_0_2_Clip_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_6_Add_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_6_block_block_3_block_3_0_Conv_output_0_output, &_feat_feat_5_Add_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_6_Add_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_6_Add_output_0_layer, 47,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &_feat_feat_6_Add_output_0_chain,
  NULL, &_feat_feat_7_block_block_0_block_0_0_Conv_output_0_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_6_block_block_3_block_3_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_6_block_block_1_block_1_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_6_block_block_3_block_3_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_6_block_block_3_block_3_0_Conv_output_0_weights, &_feat_feat_6_block_block_3_block_3_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_6_block_block_3_block_3_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_6_block_block_3_block_3_0_Conv_output_0_layer, 46,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_feat_feat_6_block_block_3_block_3_0_Conv_output_0_chain,
  NULL, &_feat_feat_6_Add_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float _feat_feat_6_block_block_1_block_1_2_Clip_output_0_nl_params_data[] = { 0.0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    _feat_feat_6_block_block_1_block_1_2_Clip_output_0_nl_params, AI_ARRAY_FORMAT_FLOAT,
    _feat_feat_6_block_block_1_block_1_2_Clip_output_0_nl_params_data, _feat_feat_6_block_block_1_block_1_2_Clip_output_0_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_6_block_block_1_block_1_2_Clip_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_6_block_block_1_block_1_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_6_block_block_1_block_1_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_6_block_block_1_block_1_2_Clip_output_0_layer, 45,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &_feat_feat_6_block_block_1_block_1_2_Clip_output_0_chain,
  NULL, &_feat_feat_6_block_block_3_block_3_0_Conv_output_0_layer, AI_STATIC, 
  .nl_params = &_feat_feat_6_block_block_1_block_1_2_Clip_output_0_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_6_block_block_1_block_1_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_6_block_block_0_block_0_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_6_block_block_1_block_1_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_6_block_block_1_block_1_0_Conv_output_0_weights, &_feat_feat_6_block_block_1_block_1_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_6_block_block_1_block_1_0_Conv_output_0_layer, 42,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &_feat_feat_6_block_block_1_block_1_0_Conv_output_0_chain,
  NULL, &_feat_feat_6_block_block_1_block_1_2_Clip_output_0_layer, AI_STATIC, 
  .groups = 120, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 2, 2, 2, 2), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float _feat_feat_6_block_block_0_block_0_2_Clip_output_0_nl_params_data[] = { 0.0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    _feat_feat_6_block_block_0_block_0_2_Clip_output_0_nl_params, AI_ARRAY_FORMAT_FLOAT,
    _feat_feat_6_block_block_0_block_0_2_Clip_output_0_nl_params_data, _feat_feat_6_block_block_0_block_0_2_Clip_output_0_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_6_block_block_0_block_0_2_Clip_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_6_block_block_0_block_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_6_block_block_0_block_0_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_6_block_block_0_block_0_2_Clip_output_0_layer, 41,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &_feat_feat_6_block_block_0_block_0_2_Clip_output_0_chain,
  NULL, &_feat_feat_6_block_block_1_block_1_0_Conv_output_0_layer, AI_STATIC, 
  .nl_params = &_feat_feat_6_block_block_0_block_0_2_Clip_output_0_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_6_block_block_0_block_0_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_5_Add_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_6_block_block_0_block_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_6_block_block_0_block_0_0_Conv_output_0_weights, &_feat_feat_6_block_block_0_block_0_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_6_block_block_0_block_0_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_6_block_block_0_block_0_0_Conv_output_0_layer, 38,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_feat_feat_6_block_block_0_block_0_0_Conv_output_0_chain,
  NULL, &_feat_feat_6_block_block_0_block_0_2_Clip_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_5_Add_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_5_block_block_3_block_3_0_Conv_output_0_output, &_feat_feat_4_block_block_3_block_3_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_5_Add_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_5_Add_output_0_layer, 37,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &_feat_feat_5_Add_output_0_chain,
  NULL, &_feat_feat_6_block_block_0_block_0_0_Conv_output_0_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_5_block_block_3_block_3_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_5_block_block_1_block_1_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_5_block_block_3_block_3_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_5_block_block_3_block_3_0_Conv_output_0_weights, &_feat_feat_5_block_block_3_block_3_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_5_block_block_3_block_3_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_5_block_block_3_block_3_0_Conv_output_0_layer, 36,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_feat_feat_5_block_block_3_block_3_0_Conv_output_0_chain,
  NULL, &_feat_feat_5_Add_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float _feat_feat_5_block_block_1_block_1_2_Clip_output_0_nl_params_data[] = { 0.0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    _feat_feat_5_block_block_1_block_1_2_Clip_output_0_nl_params, AI_ARRAY_FORMAT_FLOAT,
    _feat_feat_5_block_block_1_block_1_2_Clip_output_0_nl_params_data, _feat_feat_5_block_block_1_block_1_2_Clip_output_0_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_5_block_block_1_block_1_2_Clip_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_5_block_block_1_block_1_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_5_block_block_1_block_1_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_5_block_block_1_block_1_2_Clip_output_0_layer, 35,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &_feat_feat_5_block_block_1_block_1_2_Clip_output_0_chain,
  NULL, &_feat_feat_5_block_block_3_block_3_0_Conv_output_0_layer, AI_STATIC, 
  .nl_params = &_feat_feat_5_block_block_1_block_1_2_Clip_output_0_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_5_block_block_1_block_1_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_5_block_block_0_block_0_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_5_block_block_1_block_1_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_5_block_block_1_block_1_0_Conv_output_0_weights, &_feat_feat_5_block_block_1_block_1_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_5_block_block_1_block_1_0_Conv_output_0_layer, 32,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &_feat_feat_5_block_block_1_block_1_0_Conv_output_0_chain,
  NULL, &_feat_feat_5_block_block_1_block_1_2_Clip_output_0_layer, AI_STATIC, 
  .groups = 120, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 2, 2, 2, 2), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float _feat_feat_5_block_block_0_block_0_2_Clip_output_0_nl_params_data[] = { 0.0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    _feat_feat_5_block_block_0_block_0_2_Clip_output_0_nl_params, AI_ARRAY_FORMAT_FLOAT,
    _feat_feat_5_block_block_0_block_0_2_Clip_output_0_nl_params_data, _feat_feat_5_block_block_0_block_0_2_Clip_output_0_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_5_block_block_0_block_0_2_Clip_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_5_block_block_0_block_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_5_block_block_0_block_0_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_5_block_block_0_block_0_2_Clip_output_0_layer, 31,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &_feat_feat_5_block_block_0_block_0_2_Clip_output_0_chain,
  NULL, &_feat_feat_5_block_block_1_block_1_0_Conv_output_0_layer, AI_STATIC, 
  .nl_params = &_feat_feat_5_block_block_0_block_0_2_Clip_output_0_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_5_block_block_0_block_0_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_4_block_block_3_block_3_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_5_block_block_0_block_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_5_block_block_0_block_0_0_Conv_output_0_weights, &_feat_feat_5_block_block_0_block_0_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_5_block_block_0_block_0_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_5_block_block_0_block_0_0_Conv_output_0_layer, 28,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_feat_feat_5_block_block_0_block_0_0_Conv_output_0_chain,
  NULL, &_feat_feat_5_block_block_0_block_0_2_Clip_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_4_block_block_3_block_3_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_4_block_block_1_block_1_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_4_block_block_3_block_3_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_4_block_block_3_block_3_0_Conv_output_0_weights, &_feat_feat_4_block_block_3_block_3_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_4_block_block_3_block_3_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_4_block_block_3_block_3_0_Conv_output_0_layer, 27,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_feat_feat_4_block_block_3_block_3_0_Conv_output_0_chain,
  NULL, &_feat_feat_5_block_block_0_block_0_0_Conv_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float _feat_feat_4_block_block_1_block_1_2_Clip_output_0_nl_params_data[] = { 0.0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    _feat_feat_4_block_block_1_block_1_2_Clip_output_0_nl_params, AI_ARRAY_FORMAT_FLOAT,
    _feat_feat_4_block_block_1_block_1_2_Clip_output_0_nl_params_data, _feat_feat_4_block_block_1_block_1_2_Clip_output_0_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_4_block_block_1_block_1_2_Clip_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_4_block_block_1_block_1_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_4_block_block_1_block_1_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_4_block_block_1_block_1_2_Clip_output_0_layer, 26,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &_feat_feat_4_block_block_1_block_1_2_Clip_output_0_chain,
  NULL, &_feat_feat_4_block_block_3_block_3_0_Conv_output_0_layer, AI_STATIC, 
  .nl_params = &_feat_feat_4_block_block_1_block_1_2_Clip_output_0_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_4_block_block_1_block_1_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_4_block_block_0_block_0_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_4_block_block_1_block_1_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_4_block_block_1_block_1_0_Conv_output_0_weights, &_feat_feat_4_block_block_1_block_1_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_4_block_block_1_block_1_0_Conv_output_0_layer, 23,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &_feat_feat_4_block_block_1_block_1_0_Conv_output_0_chain,
  NULL, &_feat_feat_4_block_block_1_block_1_2_Clip_output_0_layer, AI_STATIC, 
  .groups = 48, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 2, 2, 2, 2), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float _feat_feat_4_block_block_0_block_0_2_Clip_output_0_nl_params_data[] = { 0.0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    _feat_feat_4_block_block_0_block_0_2_Clip_output_0_nl_params, AI_ARRAY_FORMAT_FLOAT,
    _feat_feat_4_block_block_0_block_0_2_Clip_output_0_nl_params_data, _feat_feat_4_block_block_0_block_0_2_Clip_output_0_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_4_block_block_0_block_0_2_Clip_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_4_block_block_0_block_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_4_block_block_0_block_0_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_4_block_block_0_block_0_2_Clip_output_0_layer, 22,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &_feat_feat_4_block_block_0_block_0_2_Clip_output_0_chain,
  NULL, &_feat_feat_4_block_block_1_block_1_0_Conv_output_0_layer, AI_STATIC, 
  .nl_params = &_feat_feat_4_block_block_0_block_0_2_Clip_output_0_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_4_block_block_0_block_0_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_3_Add_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_4_block_block_0_block_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_4_block_block_0_block_0_0_Conv_output_0_weights, &_feat_feat_4_block_block_0_block_0_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_4_block_block_0_block_0_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_4_block_block_0_block_0_0_Conv_output_0_layer, 19,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_feat_feat_4_block_block_0_block_0_0_Conv_output_0_chain,
  NULL, &_feat_feat_4_block_block_0_block_0_2_Clip_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_3_Add_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_3_block_block_2_block_2_0_Conv_output_0_output, &_feat_feat_2_block_block_2_block_2_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_3_Add_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_3_Add_output_0_layer, 18,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &_feat_feat_3_Add_output_0_chain,
  NULL, &_feat_feat_4_block_block_0_block_0_0_Conv_output_0_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_3_block_block_2_block_2_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_3_block_block_1_block_1_2_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_3_block_block_2_block_2_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_3_block_block_2_block_2_0_Conv_output_0_weights, &_feat_feat_3_block_block_2_block_2_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_3_block_block_2_block_2_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_3_block_block_2_block_2_0_Conv_output_0_layer, 17,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_feat_feat_3_block_block_2_block_2_0_Conv_output_0_chain,
  NULL, &_feat_feat_3_Add_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_3_block_block_1_block_1_2_Relu_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_3_block_block_1_block_1_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_3_block_block_1_block_1_2_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_3_block_block_1_block_1_2_Relu_output_0_layer, 16,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &_feat_feat_3_block_block_1_block_1_2_Relu_output_0_chain,
  NULL, &_feat_feat_3_block_block_2_block_2_0_Conv_output_0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_3_block_block_1_block_1_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_3_block_block_0_block_0_2_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_3_block_block_1_block_1_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_3_block_block_1_block_1_0_Conv_output_0_weights, &_feat_feat_3_block_block_1_block_1_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_3_block_block_1_block_1_0_Conv_output_0_layer, 15,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &_feat_feat_3_block_block_1_block_1_0_Conv_output_0_chain,
  NULL, &_feat_feat_3_block_block_1_block_1_2_Relu_output_0_layer, AI_STATIC, 
  .groups = 48, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_3_block_block_0_block_0_2_Relu_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_3_block_block_0_block_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_3_block_block_0_block_0_2_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_3_block_block_0_block_0_2_Relu_output_0_layer, 14,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &_feat_feat_3_block_block_0_block_0_2_Relu_output_0_chain,
  NULL, &_feat_feat_3_block_block_1_block_1_0_Conv_output_0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_3_block_block_0_block_0_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_2_block_block_2_block_2_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_3_block_block_0_block_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_3_block_block_0_block_0_0_Conv_output_0_weights, &_feat_feat_3_block_block_0_block_0_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_3_block_block_0_block_0_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_3_block_block_0_block_0_0_Conv_output_0_layer, 13,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_feat_feat_3_block_block_0_block_0_0_Conv_output_0_chain,
  NULL, &_feat_feat_3_block_block_0_block_0_2_Relu_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_2_block_block_2_block_2_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_2_block_block_1_block_1_2_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_2_block_block_2_block_2_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_2_block_block_2_block_2_0_Conv_output_0_weights, &_feat_feat_2_block_block_2_block_2_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_2_block_block_2_block_2_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_2_block_block_2_block_2_0_Conv_output_0_layer, 12,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_feat_feat_2_block_block_2_block_2_0_Conv_output_0_chain,
  NULL, &_feat_feat_3_block_block_0_block_0_0_Conv_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_2_block_block_1_block_1_2_Relu_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_2_block_block_1_block_1_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_2_block_block_1_block_1_2_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_2_block_block_1_block_1_2_Relu_output_0_layer, 11,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &_feat_feat_2_block_block_1_block_1_2_Relu_output_0_chain,
  NULL, &_feat_feat_2_block_block_2_block_2_0_Conv_output_0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_2_block_block_1_block_1_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_2_block_block_0_block_0_2_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_2_block_block_1_block_1_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_2_block_block_1_block_1_0_Conv_output_0_weights, &_feat_feat_2_block_block_1_block_1_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_2_block_block_1_block_1_0_Conv_output_0_layer, 10,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &_feat_feat_2_block_block_1_block_1_0_Conv_output_0_chain,
  NULL, &_feat_feat_2_block_block_1_block_1_2_Relu_output_0_layer, AI_STATIC, 
  .groups = 40, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_2_block_block_0_block_0_2_Relu_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_2_block_block_0_block_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_2_block_block_0_block_0_2_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_2_block_block_0_block_0_2_Relu_output_0_layer, 9,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &_feat_feat_2_block_block_0_block_0_2_Relu_output_0_chain,
  NULL, &_feat_feat_2_block_block_1_block_1_0_Conv_output_0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_2_block_block_0_block_0_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_1_block_block_2_block_2_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_2_block_block_0_block_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_2_block_block_0_block_0_0_Conv_output_0_weights, &_feat_feat_2_block_block_0_block_0_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_2_block_block_0_block_0_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_2_block_block_0_block_0_0_Conv_output_0_layer, 8,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_feat_feat_2_block_block_0_block_0_0_Conv_output_0_chain,
  NULL, &_feat_feat_2_block_block_0_block_0_2_Relu_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_1_block_block_2_block_2_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_1_block_block_0_block_0_2_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_1_block_block_2_block_2_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_1_block_block_2_block_2_0_Conv_output_0_weights, &_feat_feat_1_block_block_2_block_2_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_1_block_block_2_block_2_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_1_block_block_2_block_2_0_Conv_output_0_layer, 7,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_feat_feat_1_block_block_2_block_2_0_Conv_output_0_chain,
  NULL, &_feat_feat_2_block_block_0_block_0_0_Conv_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_1_block_block_0_block_0_2_Relu_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_1_block_block_0_block_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_1_block_block_0_block_0_2_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_1_block_block_0_block_0_2_Relu_output_0_layer, 6,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &_feat_feat_1_block_block_0_block_0_2_Relu_output_0_chain,
  NULL, &_feat_feat_1_block_block_2_block_2_0_Conv_output_0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_1_block_block_0_block_0_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_0_feat_0_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_1_block_block_0_block_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_1_block_block_0_block_0_0_Conv_output_0_weights, &_feat_feat_1_block_block_0_block_0_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_1_block_block_0_block_0_0_Conv_output_0_layer, 5,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &_feat_feat_1_block_block_0_block_0_0_Conv_output_0_chain,
  NULL, &_feat_feat_1_block_block_0_block_0_2_Relu_output_0_layer, AI_STATIC, 
  .groups = 8, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float _feat_feat_0_feat_0_2_Clip_output_0_nl_params_data[] = { 0.0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    _feat_feat_0_feat_0_2_Clip_output_0_nl_params, AI_ARRAY_FORMAT_FLOAT,
    _feat_feat_0_feat_0_2_Clip_output_0_nl_params_data, _feat_feat_0_feat_0_2_Clip_output_0_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_0_feat_0_2_Clip_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_0_feat_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_0_feat_0_2_Clip_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_0_feat_0_2_Clip_output_0_layer, 4,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &_feat_feat_0_feat_0_2_Clip_output_0_chain,
  NULL, &_feat_feat_1_block_block_0_block_0_0_Conv_output_0_layer, AI_STATIC, 
  .nl_params = &_feat_feat_0_feat_0_2_Clip_output_0_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_0_feat_0_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &input_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_0_feat_0_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_0_feat_0_0_Conv_output_0_weights, &_feat_feat_0_feat_0_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_0_feat_0_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_0_feat_0_0_Conv_output_0_layer, 1,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_feat_feat_0_feat_0_0_Conv_output_0_chain,
  NULL, &_feat_feat_0_feat_0_2_Clip_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 689560, 1, 1),
    689560, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 627200, 1, 1),
    627200, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_WATER_DETECT_IN_NUM, &input_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_WATER_DETECT_OUT_NUM, &logits_output, &_Sigmoid_output_0_output),
  &_feat_feat_0_feat_0_0_Conv_output_0_layer, 0x22b63cfc, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 689560, 1, 1),
      689560, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 627200, 1, 1),
      627200, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_WATER_DETECT_IN_NUM, &input_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_WATER_DETECT_OUT_NUM, &logits_output, &_Sigmoid_output_0_output),
  &_feat_feat_0_feat_0_0_Conv_output_0_layer, 0x22b63cfc, NULL)

#endif	/*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/



/******************************************************************************/
AI_DECLARE_STATIC
ai_bool water_detect_configure_activations(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_activations_map(g_water_detect_activations_map, 1, params)) {
    /* Updating activations (byte) offsets */
    
    input_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 401408);
    input_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 401408);
    _feat_feat_0_feat_0_0_Conv_output_0_scratch0_array.data = AI_PTR(g_water_detect_activations_map[0] + 602112);
    _feat_feat_0_feat_0_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 602112);
    _feat_feat_0_feat_0_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_0_feat_0_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_0_feat_0_2_Clip_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_0_feat_0_2_Clip_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_1_block_block_0_block_0_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 501796);
    _feat_feat_1_block_block_0_block_0_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 501796);
    _feat_feat_1_block_block_0_block_0_2_Relu_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 401444);
    _feat_feat_1_block_block_0_block_0_2_Relu_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 401444);
    _feat_feat_1_block_block_2_block_2_0_Conv_output_0_scratch0_array.data = AI_PTR(g_water_detect_activations_map[0] + 401412);
    _feat_feat_1_block_block_2_block_2_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 401412);
    _feat_feat_1_block_block_2_block_2_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 501796);
    _feat_feat_1_block_block_2_block_2_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 501796);
    _feat_feat_2_block_block_0_block_0_0_Conv_output_0_scratch0_array.data = AI_PTR(g_water_detect_activations_map[0] + 602148);
    _feat_feat_2_block_block_0_block_0_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 602148);
    _feat_feat_2_block_block_0_block_0_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_2_block_block_0_block_0_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_2_block_block_0_block_0_2_Relu_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_2_block_block_0_block_0_2_Relu_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_2_block_block_1_block_1_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 501760);
    _feat_feat_2_block_block_1_block_1_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 501760);
    _feat_feat_2_block_block_1_block_1_2_Relu_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_2_block_block_1_block_1_2_Relu_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_2_block_block_2_block_2_0_Conv_output_0_scratch0_array.data = AI_PTR(g_water_detect_activations_map[0] + 125440);
    _feat_feat_2_block_block_2_block_2_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 125440);
    _feat_feat_2_block_block_2_block_2_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 125600);
    _feat_feat_2_block_block_2_block_2_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 125600);
    _feat_feat_3_block_block_0_block_0_0_Conv_output_0_scratch0_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_3_block_block_0_block_0_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_3_block_block_0_block_0_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 175776);
    _feat_feat_3_block_block_0_block_0_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 175776);
    _feat_feat_3_block_block_0_block_0_2_Relu_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 326304);
    _feat_feat_3_block_block_0_block_0_2_Relu_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 326304);
    _feat_feat_3_block_block_1_block_1_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 175776);
    _feat_feat_3_block_block_1_block_1_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 175776);
    _feat_feat_3_block_block_1_block_1_2_Relu_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 326304);
    _feat_feat_3_block_block_1_block_1_2_Relu_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 326304);
    _feat_feat_3_block_block_2_block_2_0_Conv_output_0_scratch0_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_3_block_block_2_block_2_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_3_block_block_2_block_2_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 192);
    _feat_feat_3_block_block_2_block_2_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 192);
    _feat_feat_3_Add_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 50368);
    _feat_feat_3_Add_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 50368);
    _feat_feat_4_block_block_0_block_0_0_Conv_output_0_scratch0_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_4_block_block_0_block_0_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_4_block_block_0_block_0_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 100544);
    _feat_feat_4_block_block_0_block_0_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 100544);
    _feat_feat_4_block_block_0_block_0_2_Clip_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 251072);
    _feat_feat_4_block_block_0_block_0_2_Clip_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 251072);
    _feat_feat_4_block_block_1_block_1_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_4_block_block_1_block_1_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_4_block_block_1_block_1_2_Clip_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 37632);
    _feat_feat_4_block_block_1_block_1_2_Clip_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 37632);
    _feat_feat_4_block_block_3_block_3_0_Conv_output_0_scratch0_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_4_block_block_3_block_3_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_4_block_block_3_block_3_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 192);
    _feat_feat_4_block_block_3_block_3_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 192);
    _feat_feat_5_block_block_0_block_0_0_Conv_output_0_scratch0_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_5_block_block_0_block_0_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_5_block_block_0_block_0_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 19008);
    _feat_feat_5_block_block_0_block_0_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 19008);
    _feat_feat_5_block_block_0_block_0_2_Clip_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 113088);
    _feat_feat_5_block_block_0_block_0_2_Clip_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 113088);
    _feat_feat_5_block_block_1_block_1_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 19008);
    _feat_feat_5_block_block_1_block_1_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 19008);
    _feat_feat_5_block_block_1_block_1_2_Clip_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 113088);
    _feat_feat_5_block_block_1_block_1_2_Clip_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 113088);
    _feat_feat_5_block_block_3_block_3_0_Conv_output_0_scratch0_array.data = AI_PTR(g_water_detect_activations_map[0] + 19008);
    _feat_feat_5_block_block_3_block_3_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 19008);
    _feat_feat_5_block_block_3_block_3_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 19488);
    _feat_feat_5_block_block_3_block_3_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 19488);
    _feat_feat_5_Add_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 38304);
    _feat_feat_5_Add_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 38304);
    _feat_feat_6_block_block_0_block_0_0_Conv_output_0_scratch0_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_6_block_block_0_block_0_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_6_block_block_0_block_0_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 57120);
    _feat_feat_6_block_block_0_block_0_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 57120);
    _feat_feat_6_block_block_0_block_0_2_Clip_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 151200);
    _feat_feat_6_block_block_0_block_0_2_Clip_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 151200);
    _feat_feat_6_block_block_1_block_1_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 57120);
    _feat_feat_6_block_block_1_block_1_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 57120);
    _feat_feat_6_block_block_1_block_1_2_Clip_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 151200);
    _feat_feat_6_block_block_1_block_1_2_Clip_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 151200);
    _feat_feat_6_block_block_3_block_3_0_Conv_output_0_scratch0_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_6_block_block_3_block_3_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_6_block_block_3_block_3_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 480);
    _feat_feat_6_block_block_3_block_3_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 480);
    _feat_feat_6_Add_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 19296);
    _feat_feat_6_Add_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 19296);
    _feat_feat_7_block_block_0_block_0_0_Conv_output_0_scratch0_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_7_block_block_0_block_0_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_7_block_block_0_block_0_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 38112);
    _feat_feat_7_block_block_0_block_0_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 38112);
    _feat_feat_7_block_block_0_block_0_2_Clip_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 88288);
    _feat_feat_7_block_block_0_block_0_2_Clip_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 88288);
    _feat_feat_7_block_block_1_block_1_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 38112);
    _feat_feat_7_block_block_1_block_1_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 38112);
    _feat_feat_7_block_block_1_block_1_2_Clip_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 88288);
    _feat_feat_7_block_block_1_block_1_2_Clip_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 88288);
    _feat_feat_7_block_block_3_block_3_0_Conv_output_0_scratch0_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_7_block_block_3_block_3_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_7_block_block_3_block_3_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 256);
    _feat_feat_7_block_block_3_block_3_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 256);
    _feat_feat_7_Add_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 38112);
    _feat_feat_7_Add_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 38112);
    _feat_feat_8_block_block_0_block_0_0_Conv_output_0_scratch0_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_8_block_block_0_block_0_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_8_block_block_0_block_0_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 56928);
    _feat_feat_8_block_block_0_block_0_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 56928);
    _feat_feat_8_block_block_0_block_0_2_Clip_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 113376);
    _feat_feat_8_block_block_0_block_0_2_Clip_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 113376);
    _feat_feat_8_block_block_1_block_1_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 56928);
    _feat_feat_8_block_block_1_block_1_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 56928);
    _feat_feat_8_block_block_1_block_1_2_Clip_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 113376);
    _feat_feat_8_block_block_1_block_1_2_Clip_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 113376);
    _feat_feat_8_block_block_3_block_3_0_Conv_output_0_scratch0_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_8_block_block_3_block_3_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_8_block_block_3_block_3_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 288);
    _feat_feat_8_block_block_3_block_3_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 288);
    _feat_feat_8_Add_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 19104);
    _feat_feat_8_Add_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 19104);
    _feat_feat_9_block_block_0_block_0_0_Conv_output_0_scratch0_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_9_block_block_0_block_0_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_9_block_block_0_block_0_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 37920);
    _feat_feat_9_block_block_0_block_0_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 37920);
    _feat_feat_9_block_block_0_block_0_2_Clip_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 150816);
    _feat_feat_9_block_block_0_block_0_2_Clip_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 150816);
    _feat_feat_9_block_block_1_block_1_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_9_block_block_1_block_1_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_9_block_block_1_block_1_2_Clip_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 28224);
    _feat_feat_9_block_block_1_block_1_2_Clip_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 28224);
    _feat_feat_9_block_block_3_block_3_0_Conv_output_0_scratch0_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_9_block_block_3_block_3_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_9_block_block_3_block_3_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 576);
    _feat_feat_9_block_block_3_block_3_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 576);
    _feat_feat_10_block_block_0_block_0_0_Conv_output_0_scratch0_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_10_block_block_0_block_0_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_10_block_block_0_block_0_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 9984);
    _feat_feat_10_block_block_0_block_0_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 9984);
    _feat_feat_10_block_block_0_block_0_2_Clip_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 66432);
    _feat_feat_10_block_block_0_block_0_2_Clip_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 66432);
    _feat_feat_10_block_block_1_block_1_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 9984);
    _feat_feat_10_block_block_1_block_1_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 9984);
    _feat_feat_10_block_block_1_block_1_2_Clip_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 66432);
    _feat_feat_10_block_block_1_block_1_2_Clip_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 66432);
    _feat_feat_10_block_block_3_block_3_0_Conv_output_0_scratch0_array.data = AI_PTR(g_water_detect_activations_map[0] + 9984);
    _feat_feat_10_block_block_3_block_3_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 9984);
    _feat_feat_10_block_block_3_block_3_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 11136);
    _feat_feat_10_block_block_3_block_3_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 11136);
    _feat_feat_10_Add_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 20544);
    _feat_feat_10_Add_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 20544);
    _feat_feat_11_block_block_0_block_0_0_Conv_output_0_scratch0_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_11_block_block_0_block_0_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_11_block_block_0_block_0_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 29952);
    _feat_feat_11_block_block_0_block_0_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 29952);
    _feat_feat_11_block_block_0_block_0_2_Clip_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 86400);
    _feat_feat_11_block_block_0_block_0_2_Clip_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 86400);
    _feat_feat_11_block_block_1_block_1_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 29952);
    _feat_feat_11_block_block_1_block_1_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 29952);
    _feat_feat_11_block_block_1_block_1_2_Clip_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 86400);
    _feat_feat_11_block_block_1_block_1_2_Clip_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 86400);
    _feat_feat_11_block_block_3_block_3_0_Conv_output_0_scratch0_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_11_block_block_3_block_3_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_11_block_block_3_block_3_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 1152);
    _feat_feat_11_block_block_3_block_3_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 1152);
    _feat_feat_11_Add_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 10560);
    _feat_feat_11_Add_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 10560);
    _feat_feat_12_feat_12_0_Conv_output_0_scratch0_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_12_feat_12_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _feat_feat_12_feat_12_0_Conv_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 19968);
    _feat_feat_12_feat_12_0_Conv_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 19968);
    _feat_feat_12_feat_12_2_Clip_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 76416);
    _feat_feat_12_feat_12_2_Clip_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 76416);
    _pool_GlobalAveragePool_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _pool_GlobalAveragePool_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    _shared_shared_1_Gemm_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 1152);
    _shared_shared_1_Gemm_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 1152);
    _shared_shared_2_Clip_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _shared_shared_2_Clip_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    logits_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 512);
    logits_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 512);
    _reg_head_Gemm_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 532);
    _reg_head_Gemm_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 532);
    _Sigmoid_output_0_output_array.data = AI_PTR(g_water_detect_activations_map[0] + 0);
    _Sigmoid_output_0_output_array.data_start = AI_PTR(g_water_detect_activations_map[0] + 0);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_ACTIVATIONS);
  return false;
}




/******************************************************************************/
AI_DECLARE_STATIC
ai_bool water_detect_configure_weights(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_weights_map(g_water_detect_weights_map, 1, params)) {
    /* Updating weights (byte) offsets */
    
    _feat_feat_0_feat_0_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_0_feat_0_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 0);
    _feat_feat_0_feat_0_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 0);
    _feat_feat_0_feat_0_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_0_feat_0_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 288);
    _feat_feat_0_feat_0_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 288);
    _feat_feat_1_block_block_0_block_0_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_1_block_block_0_block_0_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 320);
    _feat_feat_1_block_block_0_block_0_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 320);
    _feat_feat_1_block_block_0_block_0_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_1_block_block_0_block_0_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 608);
    _feat_feat_1_block_block_0_block_0_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 608);
    _feat_feat_1_block_block_2_block_2_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_1_block_block_2_block_2_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 640);
    _feat_feat_1_block_block_2_block_2_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 640);
    _feat_feat_1_block_block_2_block_2_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_1_block_block_2_block_2_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 896);
    _feat_feat_1_block_block_2_block_2_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 896);
    _feat_feat_2_block_block_0_block_0_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_2_block_block_0_block_0_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 928);
    _feat_feat_2_block_block_0_block_0_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 928);
    _feat_feat_2_block_block_0_block_0_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_2_block_block_0_block_0_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 2208);
    _feat_feat_2_block_block_0_block_0_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 2208);
    _feat_feat_2_block_block_1_block_1_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_2_block_block_1_block_1_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 2368);
    _feat_feat_2_block_block_1_block_1_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 2368);
    _feat_feat_2_block_block_1_block_1_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_2_block_block_1_block_1_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 3808);
    _feat_feat_2_block_block_1_block_1_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 3808);
    _feat_feat_2_block_block_2_block_2_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_2_block_block_2_block_2_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 3968);
    _feat_feat_2_block_block_2_block_2_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 3968);
    _feat_feat_2_block_block_2_block_2_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_2_block_block_2_block_2_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 6528);
    _feat_feat_2_block_block_2_block_2_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 6528);
    _feat_feat_3_block_block_0_block_0_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_3_block_block_0_block_0_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 6592);
    _feat_feat_3_block_block_0_block_0_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 6592);
    _feat_feat_3_block_block_0_block_0_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_3_block_block_0_block_0_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 9664);
    _feat_feat_3_block_block_0_block_0_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 9664);
    _feat_feat_3_block_block_1_block_1_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_3_block_block_1_block_1_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 9856);
    _feat_feat_3_block_block_1_block_1_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 9856);
    _feat_feat_3_block_block_1_block_1_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_3_block_block_1_block_1_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 11584);
    _feat_feat_3_block_block_1_block_1_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 11584);
    _feat_feat_3_block_block_2_block_2_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_3_block_block_2_block_2_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 11776);
    _feat_feat_3_block_block_2_block_2_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 11776);
    _feat_feat_3_block_block_2_block_2_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_3_block_block_2_block_2_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 14848);
    _feat_feat_3_block_block_2_block_2_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 14848);
    _feat_feat_4_block_block_0_block_0_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_4_block_block_0_block_0_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 14912);
    _feat_feat_4_block_block_0_block_0_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 14912);
    _feat_feat_4_block_block_0_block_0_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_4_block_block_0_block_0_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 17984);
    _feat_feat_4_block_block_0_block_0_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 17984);
    _feat_feat_4_block_block_1_block_1_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_4_block_block_1_block_1_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 18176);
    _feat_feat_4_block_block_1_block_1_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 18176);
    _feat_feat_4_block_block_1_block_1_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_4_block_block_1_block_1_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 22976);
    _feat_feat_4_block_block_1_block_1_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 22976);
    _feat_feat_4_block_block_3_block_3_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_4_block_block_3_block_3_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 23168);
    _feat_feat_4_block_block_3_block_3_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 23168);
    _feat_feat_4_block_block_3_block_3_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_4_block_block_3_block_3_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 27776);
    _feat_feat_4_block_block_3_block_3_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 27776);
    _feat_feat_5_block_block_0_block_0_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_5_block_block_0_block_0_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 27872);
    _feat_feat_5_block_block_0_block_0_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 27872);
    _feat_feat_5_block_block_0_block_0_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_5_block_block_0_block_0_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 39392);
    _feat_feat_5_block_block_0_block_0_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 39392);
    _feat_feat_5_block_block_1_block_1_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_5_block_block_1_block_1_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 39872);
    _feat_feat_5_block_block_1_block_1_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 39872);
    _feat_feat_5_block_block_1_block_1_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_5_block_block_1_block_1_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 51872);
    _feat_feat_5_block_block_1_block_1_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 51872);
    _feat_feat_5_block_block_3_block_3_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_5_block_block_3_block_3_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 52352);
    _feat_feat_5_block_block_3_block_3_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 52352);
    _feat_feat_5_block_block_3_block_3_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_5_block_block_3_block_3_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 63872);
    _feat_feat_5_block_block_3_block_3_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 63872);
    _feat_feat_6_block_block_0_block_0_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_6_block_block_0_block_0_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 63968);
    _feat_feat_6_block_block_0_block_0_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 63968);
    _feat_feat_6_block_block_0_block_0_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_6_block_block_0_block_0_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 75488);
    _feat_feat_6_block_block_0_block_0_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 75488);
    _feat_feat_6_block_block_1_block_1_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_6_block_block_1_block_1_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 75968);
    _feat_feat_6_block_block_1_block_1_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 75968);
    _feat_feat_6_block_block_1_block_1_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_6_block_block_1_block_1_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 87968);
    _feat_feat_6_block_block_1_block_1_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 87968);
    _feat_feat_6_block_block_3_block_3_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_6_block_block_3_block_3_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 88448);
    _feat_feat_6_block_block_3_block_3_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 88448);
    _feat_feat_6_block_block_3_block_3_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_6_block_block_3_block_3_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 99968);
    _feat_feat_6_block_block_3_block_3_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 99968);
    _feat_feat_7_block_block_0_block_0_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_7_block_block_0_block_0_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 100064);
    _feat_feat_7_block_block_0_block_0_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 100064);
    _feat_feat_7_block_block_0_block_0_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_7_block_block_0_block_0_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 106208);
    _feat_feat_7_block_block_0_block_0_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 106208);
    _feat_feat_7_block_block_1_block_1_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_7_block_block_1_block_1_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 106464);
    _feat_feat_7_block_block_1_block_1_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 106464);
    _feat_feat_7_block_block_1_block_1_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_7_block_block_1_block_1_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 112864);
    _feat_feat_7_block_block_1_block_1_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 112864);
    _feat_feat_7_block_block_3_block_3_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_7_block_block_3_block_3_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 113120);
    _feat_feat_7_block_block_3_block_3_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 113120);
    _feat_feat_7_block_block_3_block_3_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_7_block_block_3_block_3_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 119264);
    _feat_feat_7_block_block_3_block_3_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 119264);
    _feat_feat_8_block_block_0_block_0_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_8_block_block_0_block_0_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 119360);
    _feat_feat_8_block_block_0_block_0_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 119360);
    _feat_feat_8_block_block_0_block_0_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_8_block_block_0_block_0_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 126272);
    _feat_feat_8_block_block_0_block_0_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 126272);
    _feat_feat_8_block_block_1_block_1_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_8_block_block_1_block_1_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 126560);
    _feat_feat_8_block_block_1_block_1_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 126560);
    _feat_feat_8_block_block_1_block_1_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_8_block_block_1_block_1_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 133760);
    _feat_feat_8_block_block_1_block_1_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 133760);
    _feat_feat_8_block_block_3_block_3_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_8_block_block_3_block_3_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 134048);
    _feat_feat_8_block_block_3_block_3_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 134048);
    _feat_feat_8_block_block_3_block_3_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_8_block_block_3_block_3_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 140960);
    _feat_feat_8_block_block_3_block_3_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 140960);
    _feat_feat_9_block_block_0_block_0_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_9_block_block_0_block_0_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 141056);
    _feat_feat_9_block_block_0_block_0_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 141056);
    _feat_feat_9_block_block_0_block_0_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_9_block_block_0_block_0_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 154880);
    _feat_feat_9_block_block_0_block_0_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 154880);
    _feat_feat_9_block_block_1_block_1_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_9_block_block_1_block_1_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 155456);
    _feat_feat_9_block_block_1_block_1_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 155456);
    _feat_feat_9_block_block_1_block_1_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_9_block_block_1_block_1_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 169856);
    _feat_feat_9_block_block_1_block_1_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 169856);
    _feat_feat_9_block_block_3_block_3_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_9_block_block_3_block_3_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 170432);
    _feat_feat_9_block_block_3_block_3_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 170432);
    _feat_feat_9_block_block_3_block_3_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_9_block_block_3_block_3_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 198080);
    _feat_feat_9_block_block_3_block_3_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 198080);
    _feat_feat_10_block_block_0_block_0_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_10_block_block_0_block_0_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 198272);
    _feat_feat_10_block_block_0_block_0_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 198272);
    _feat_feat_10_block_block_0_block_0_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_10_block_block_0_block_0_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 253568);
    _feat_feat_10_block_block_0_block_0_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 253568);
    _feat_feat_10_block_block_1_block_1_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_10_block_block_1_block_1_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 254720);
    _feat_feat_10_block_block_1_block_1_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 254720);
    _feat_feat_10_block_block_1_block_1_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_10_block_block_1_block_1_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 283520);
    _feat_feat_10_block_block_1_block_1_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 283520);
    _feat_feat_10_block_block_3_block_3_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_10_block_block_3_block_3_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 284672);
    _feat_feat_10_block_block_3_block_3_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 284672);
    _feat_feat_10_block_block_3_block_3_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_10_block_block_3_block_3_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 339968);
    _feat_feat_10_block_block_3_block_3_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 339968);
    _feat_feat_11_block_block_0_block_0_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_11_block_block_0_block_0_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 340160);
    _feat_feat_11_block_block_0_block_0_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 340160);
    _feat_feat_11_block_block_0_block_0_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_11_block_block_0_block_0_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 395456);
    _feat_feat_11_block_block_0_block_0_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 395456);
    _feat_feat_11_block_block_1_block_1_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_11_block_block_1_block_1_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 396608);
    _feat_feat_11_block_block_1_block_1_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 396608);
    _feat_feat_11_block_block_1_block_1_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_11_block_block_1_block_1_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 425408);
    _feat_feat_11_block_block_1_block_1_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 425408);
    _feat_feat_11_block_block_3_block_3_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_11_block_block_3_block_3_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 426560);
    _feat_feat_11_block_block_3_block_3_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 426560);
    _feat_feat_11_block_block_3_block_3_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_11_block_block_3_block_3_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 481856);
    _feat_feat_11_block_block_3_block_3_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 481856);
    _feat_feat_12_feat_12_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_12_feat_12_0_Conv_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 482048);
    _feat_feat_12_feat_12_0_Conv_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 482048);
    _feat_feat_12_feat_12_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _feat_feat_12_feat_12_0_Conv_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 537344);
    _feat_feat_12_feat_12_0_Conv_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 537344);
    _shared_shared_1_Gemm_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _shared_shared_1_Gemm_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 538496);
    _shared_shared_1_Gemm_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 538496);
    _shared_shared_1_Gemm_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _shared_shared_1_Gemm_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 685952);
    _shared_shared_1_Gemm_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 685952);
    logits_weights_array.format |= AI_FMT_FLAG_CONST;
    logits_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 686464);
    logits_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 686464);
    logits_bias_array.format |= AI_FMT_FLAG_CONST;
    logits_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 689024);
    logits_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 689024);
    _reg_head_Gemm_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _reg_head_Gemm_output_0_weights_array.data = AI_PTR(g_water_detect_weights_map[0] + 689044);
    _reg_head_Gemm_output_0_weights_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 689044);
    _reg_head_Gemm_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _reg_head_Gemm_output_0_bias_array.data = AI_PTR(g_water_detect_weights_map[0] + 689556);
    _reg_head_Gemm_output_0_bias_array.data_start = AI_PTR(g_water_detect_weights_map[0] + 689556);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_WEIGHTS);
  return false;
}


/**  PUBLIC APIs SECTION  *****************************************************/



AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_water_detect_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_WATER_DETECT_MODEL_NAME,
      .model_signature   = AI_WATER_DETECT_MODEL_SIGNATURE,
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
      
      .n_macc            = 17537632,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .params            = AI_STRUCT_INIT,
      .activations       = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x22b63cfc,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}



AI_API_ENTRY
ai_bool ai_water_detect_get_report(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_WATER_DETECT_MODEL_NAME,
      .model_signature   = AI_WATER_DETECT_MODEL_SIGNATURE,
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
      
      .n_macc            = 17537632,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .map_signature     = AI_MAGIC_SIGNATURE,
      .map_weights       = AI_STRUCT_INIT,
      .map_activations   = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x22b63cfc,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}


AI_API_ENTRY
ai_error ai_water_detect_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}


AI_API_ENTRY
ai_error ai_water_detect_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    AI_CONTEXT_OBJ(&AI_NET_OBJ_INSTANCE),
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}


AI_API_ENTRY
ai_error ai_water_detect_create_and_init(
  ai_handle* network, const ai_handle activations[], const ai_handle weights[])
{
  ai_error err;
  ai_network_params params;

  err = ai_water_detect_create(network, AI_WATER_DETECT_DATA_CONFIG);
  if (err.type != AI_ERROR_NONE) {
    return err;
  }
  
  if (ai_water_detect_data_params_get(&params) != true) {
    err = ai_water_detect_get_error(*network);
    return err;
  }
#if defined(AI_WATER_DETECT_DATA_ACTIVATIONS_COUNT)
  /* set the addresses of the activations buffers */
  for (ai_u16 idx=0; activations && idx<params.map_activations.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_activations, idx, activations[idx]);
  }
#endif
#if defined(AI_WATER_DETECT_DATA_WEIGHTS_COUNT)
  /* set the addresses of the weight buffers */
  for (ai_u16 idx=0; weights && idx<params.map_weights.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_weights, idx, weights[idx]);
  }
#endif
  if (ai_water_detect_init(*network, &params) != true) {
    err = ai_water_detect_get_error(*network);
  }
  return err;
}


AI_API_ENTRY
ai_buffer* ai_water_detect_inputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_inputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_buffer* ai_water_detect_outputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_outputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_handle ai_water_detect_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}


AI_API_ENTRY
ai_bool ai_water_detect_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = AI_NETWORK_OBJ(ai_platform_network_init(network, params));
  ai_bool ok = true;

  if (!net_ctx) return false;
  ok &= water_detect_configure_weights(net_ctx, params);
  ok &= water_detect_configure_activations(net_ctx, params);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_water_detect_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}


AI_API_ENTRY
ai_i32 ai_water_detect_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}



#undef AI_WATER_DETECT_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

