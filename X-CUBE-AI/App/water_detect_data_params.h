/**
  ******************************************************************************
  * @file    water_detect_data_params.h
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-06-27T17:49:15+0800
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

#ifndef WATER_DETECT_DATA_PARAMS_H
#define WATER_DETECT_DATA_PARAMS_H

#include "ai_platform.h"

/*
#define AI_WATER_DETECT_DATA_WEIGHTS_PARAMS \
  (AI_HANDLE_PTR(&ai_water_detect_data_weights_params[1]))
*/

#define AI_WATER_DETECT_DATA_CONFIG               (NULL)


#define AI_WATER_DETECT_DATA_ACTIVATIONS_SIZES \
  { 627200, }
#define AI_WATER_DETECT_DATA_ACTIVATIONS_SIZE     (627200)
#define AI_WATER_DETECT_DATA_ACTIVATIONS_COUNT    (1)
#define AI_WATER_DETECT_DATA_ACTIVATION_1_SIZE    (627200)



#define AI_WATER_DETECT_DATA_WEIGHTS_SIZES \
  { 689560, }
#define AI_WATER_DETECT_DATA_WEIGHTS_SIZE         (689560)
#define AI_WATER_DETECT_DATA_WEIGHTS_COUNT        (1)
#define AI_WATER_DETECT_DATA_WEIGHT_1_SIZE        (689560)



#define AI_WATER_DETECT_DATA_ACTIVATIONS_TABLE_GET() \
  (&g_water_detect_activations_table[1])

extern ai_handle g_water_detect_activations_table[1 + 2];



#define AI_WATER_DETECT_DATA_WEIGHTS_TABLE_GET() \
  (&g_water_detect_weights_table[1])

extern ai_handle g_water_detect_weights_table[1 + 2];


#endif    /* WATER_DETECT_DATA_PARAMS_H */
