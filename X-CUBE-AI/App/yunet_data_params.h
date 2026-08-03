/**
  ******************************************************************************
  * @file    yunet_data_params.h
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-08-02T18:20:44+0800
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#ifndef YUNET_DATA_PARAMS_H
#define YUNET_DATA_PARAMS_H

#include "ai_platform.h"

/*
#define AI_YUNET_DATA_WEIGHTS_PARAMS \
  (AI_HANDLE_PTR(&ai_yunet_data_weights_params[1]))
*/

#define AI_YUNET_DATA_CONFIG               (NULL)


#define AI_YUNET_DATA_ACTIVATIONS_SIZES \
  { 843008, }
#define AI_YUNET_DATA_ACTIVATIONS_SIZE     (843008)
#define AI_YUNET_DATA_ACTIVATIONS_COUNT    (1)
#define AI_YUNET_DATA_ACTIVATION_1_SIZE    (843008)



#define AI_YUNET_DATA_WEIGHTS_SIZES \
  { 80152, }
#define AI_YUNET_DATA_WEIGHTS_SIZE         (80152)
#define AI_YUNET_DATA_WEIGHTS_COUNT        (1)
#define AI_YUNET_DATA_WEIGHT_1_SIZE        (80152)



#define AI_YUNET_DATA_ACTIVATIONS_TABLE_GET() \
  (&g_yunet_activations_table[1])

extern ai_handle g_yunet_activations_table[1 + 2];



#define AI_YUNET_DATA_WEIGHTS_TABLE_GET() \
  (&g_yunet_weights_table[1])

extern ai_handle g_yunet_weights_table[1 + 2];


#endif    /* YUNET_DATA_PARAMS_H */
