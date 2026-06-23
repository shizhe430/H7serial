/**
  ******************************************************************************
  * @file    network.h
  * @date    2026-04-27T12:03:38+0000
  * @brief   ST.AI Tool Automatic Code Generator for Embedded NN computing
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
#ifndef STAI_NETWORK_DETAILS_H
#define STAI_NETWORK_DETAILS_H

#include "stai.h"
#include "layers.h"

const stai_network_details g_network_details = {
  .tensors = (const stai_tensor[13]) {
   { .size_bytes = 50176, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 224, 224, 1}}, .scale = {1, (const float[1]){0.003921544179320335}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "input_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 112, 112, 16}}, .scale = {1, (const float[1]){0.008772855624556541}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "_feat_feat_1_Relu_output_0_output" },
   { .size_bytes = 207936, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 114, 114, 16}}, .scale = {1, (const float[1]){0.008772855624556541}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "_feat_feat_4_Relu_output_0_pad_before_output" },
   { .size_bytes = 100352, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 56, 56, 32}}, .scale = {1, (const float[1]){0.019329840317368507}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "_feat_feat_4_Relu_output_0_output" },
   { .size_bytes = 107648, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 58, 58, 32}}, .scale = {1, (const float[1]){0.019329840317368507}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "_feat_feat_7_Relu_output_0_pad_before_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 56, 56, 64}}, .scale = {1, (const float[1]){0.025555280968546867}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "_feat_feat_7_Relu_output_0_output" },
   { .size_bytes = 215296, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 58, 58, 64}}, .scale = {1, (const float[1]){0.025555280968546867}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "_feat_feat_9_Relu_output_0_pad_before_output" },
   { .size_bytes = 301056, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 56, 56, 96}}, .scale = {1, (const float[1]){0.2251434624195099}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "_feat_feat_9_Relu_output_0_output" },
   { .size_bytes = 1536, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 4, 4, 96}}, .scale = {1, (const float[1]){0.2251434624195099}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "_pool_AveragePool_output_0_output" },
   { .size_bytes = 1536, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {4, (const int32_t[4]){1, 96, 4, 4}}, .scale = {1, (const float[1]){0.2251434624195099}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "_cls_cls_0_Flatten_output_0_to_chlast_output" },
   { .size_bytes = 1536, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {2, (const int32_t[2]){1, 1536}}, .scale = {1, (const float[1]){0.043284229934215546}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "_cls_cls_0_Flatten_output_0_0_conversion_output" },
   { .size_bytes = 128, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {2, (const int32_t[2]){1, 128}}, .scale = {1, (const float[1]){0.1410362422466278}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "_cls_cls_2_Relu_output_0_output" },
   { .size_bytes = 5, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {2, (const int32_t[2]){1, 5}}, .scale = {1, (const float[1]){0.17340436577796936}}, .zeropoint = {1, (const int16_t[1]){25}}, .name = "output_QuantizeLinear_Input_output" }
  },
  .nodes = (const stai_node_details[12]){
    {.id = 18, .type = AI_LAYER_OPTIMIZED_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){0}}, .output_tensors = {1, (const int32_t[1]){1}} }, /* _feat_feat_1_Relu_output_0 */
    {.id = 21, .type = AI_LAYER_PAD_TYPE, .input_tensors = {1, (const int32_t[1]){1}}, .output_tensors = {1, (const int32_t[1]){2}} }, /* _feat_feat_4_Relu_output_0_pad_before */
    {.id = 24, .type = AI_LAYER_OPTIMIZED_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){2}}, .output_tensors = {1, (const int32_t[1]){3}} }, /* _feat_feat_4_Relu_output_0 */
    {.id = 27, .type = AI_LAYER_PAD_TYPE, .input_tensors = {1, (const int32_t[1]){3}}, .output_tensors = {1, (const int32_t[1]){4}} }, /* _feat_feat_7_Relu_output_0_pad_before */
    {.id = 27, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){4}}, .output_tensors = {1, (const int32_t[1]){5}} }, /* _feat_feat_7_Relu_output_0 */
    {.id = 30, .type = AI_LAYER_PAD_TYPE, .input_tensors = {1, (const int32_t[1]){5}}, .output_tensors = {1, (const int32_t[1]){6}} }, /* _feat_feat_9_Relu_output_0_pad_before */
    {.id = 30, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){6}}, .output_tensors = {1, (const int32_t[1]){7}} }, /* _feat_feat_9_Relu_output_0 */
    {.id = 33, .type = AI_LAYER_POOL_TYPE, .input_tensors = {1, (const int32_t[1]){7}}, .output_tensors = {1, (const int32_t[1]){8}} }, /* _pool_AveragePool_output_0 */
    {.id = 36, .type = AI_LAYER_TRANSPOSE_TYPE, .input_tensors = {1, (const int32_t[1]){8}}, .output_tensors = {1, (const int32_t[1]){9}} }, /* _cls_cls_0_Flatten_output_0_to_chlast */
    {.id = 36, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){9}}, .output_tensors = {1, (const int32_t[1]){10}} }, /* _cls_cls_0_Flatten_output_0_0_conversion */
    {.id = 39, .type = AI_LAYER_DENSE_TYPE, .input_tensors = {1, (const int32_t[1]){10}}, .output_tensors = {1, (const int32_t[1]){11}} }, /* _cls_cls_2_Relu_output_0 */
    {.id = 42, .type = AI_LAYER_DENSE_TYPE, .input_tensors = {1, (const int32_t[1]){11}}, .output_tensors = {1, (const int32_t[1]){12}} } /* output_QuantizeLinear_Input */
  },
  .n_nodes = 12
};
#endif

