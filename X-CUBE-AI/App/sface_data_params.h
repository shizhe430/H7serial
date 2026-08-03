/**
  ******************************************************************************
  * @file    sface_data_params.h
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-08-02T18:20:41+0800
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

#ifndef SFACE_DATA_PARAMS_H
#define SFACE_DATA_PARAMS_H

#include "ai_platform.h"

/*
#define AI_SFACE_DATA_WEIGHTS_PARAMS \
  (AI_HANDLE_PTR(&ai_sface_data_weights_params[1]))
*/

#define AI_SFACE_DATA_CONFIG               (NULL)


#define AI_SFACE_DATA_ACTIVATIONS_SIZES \
  { 3211264, }
#define AI_SFACE_DATA_ACTIVATIONS_SIZE     (3211264)
#define AI_SFACE_DATA_ACTIVATIONS_COUNT    (1)
#define AI_SFACE_DATA_ACTIVATION_1_SIZE    (3211264)



#define AI_SFACE_DATA_WEIGHTS_SIZES \
  { 9756468, }
#define AI_SFACE_DATA_WEIGHTS_SIZE         (9756468)
#define AI_SFACE_DATA_WEIGHTS_COUNT        (1)
#define AI_SFACE_DATA_WEIGHT_1_SIZE        (9756468)



#define AI_SFACE_DATA_ACTIVATIONS_TABLE_GET() \
  (&g_sface_activations_table[1])

extern ai_handle g_sface_activations_table[1 + 2];



#define AI_SFACE_DATA_WEIGHTS_TABLE_GET() \
  (&g_sface_weights_table[1])

extern ai_handle g_sface_weights_table[1 + 2];


#endif    /* SFACE_DATA_PARAMS_H */
