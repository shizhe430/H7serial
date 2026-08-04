/**
  ******************************************************************************
  * @file    faceid_data_params.h
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-08-02T15:00:31+0800
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

#ifndef FACEID_DATA_PARAMS_H
#define FACEID_DATA_PARAMS_H

#include "ai_platform.h"

/*
#define AI_FACEID_DATA_WEIGHTS_PARAMS \
  (AI_HANDLE_PTR(&ai_faceid_data_weights_params[1]))
*/

#define AI_FACEID_DATA_CONFIG               (NULL)


#define AI_FACEID_DATA_ACTIVATIONS_SIZES \
  { 3211776, }
#define AI_FACEID_DATA_ACTIVATIONS_SIZE     (3211776)
#define AI_FACEID_DATA_ACTIVATIONS_COUNT    (1)
#define AI_FACEID_DATA_ACTIVATION_1_SIZE    (3211776)



#define AI_FACEID_DATA_WEIGHTS_SIZES \
  { 13593736, }
#define AI_FACEID_DATA_WEIGHTS_SIZE         (13593736)
#define AI_FACEID_DATA_WEIGHTS_COUNT        (1)
#define AI_FACEID_DATA_WEIGHT_1_SIZE        (13593736)



#define AI_FACEID_DATA_ACTIVATIONS_TABLE_GET() \
  (&g_faceid_activations_table[1])

extern ai_handle g_faceid_activations_table[1 + 2];



#define AI_FACEID_DATA_WEIGHTS_TABLE_GET() \
  (&g_faceid_weights_table[1])

extern ai_handle g_faceid_weights_table[1 + 2];


#endif    /* FACEID_DATA_PARAMS_H */
