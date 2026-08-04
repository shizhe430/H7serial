/**
  ******************************************************************************
  * @file    faceid.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-08-02T15:00:31+0800
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


#include "faceid.h"
#include "faceid_data.h"

#include "ai_platform.h"
#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"



#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_faceid
 
#undef AI_FACEID_MODEL_SIGNATURE
#define AI_FACEID_MODEL_SIGNATURE     "0xc5b029527cb6f874e057613fc10bae1b"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "2026-08-02T15:00:31+0800"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_FACEID_N_BATCHES
#define AI_FACEID_N_BATCHES         (1)

static ai_ptr g_faceid_activations_map[1] = AI_C_ARRAY_INIT;
static ai_ptr g_faceid_weights_map[1] = AI_C_ARRAY_INIT;



/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  input_1_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 37632, AI_STATIC)

/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  input_1_Transpose_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 37632, AI_STATIC)

/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  node_517_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  node_339_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  node_520_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  node_343_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  node_523_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  node_347_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  node_526_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  node_351_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  node_529_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  node_532_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  node_357_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  node_535_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  node_361_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  node_538_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  node_364_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#17 */
AI_ARRAY_OBJ_DECLARE(
  node_541_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#18 */
AI_ARRAY_OBJ_DECLARE(
  node_368_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#19 */
AI_ARRAY_OBJ_DECLARE(
  node_544_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#20 */
AI_ARRAY_OBJ_DECLARE(
  node_372_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#21 */
AI_ARRAY_OBJ_DECLARE(
  node_547_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#22 */
AI_ARRAY_OBJ_DECLARE(
  node_375_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#23 */
AI_ARRAY_OBJ_DECLARE(
  node_550_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#24 */
AI_ARRAY_OBJ_DECLARE(
  node_379_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#25 */
AI_ARRAY_OBJ_DECLARE(
  node_553_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#26 */
AI_ARRAY_OBJ_DECLARE(
  node_383_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#27 */
AI_ARRAY_OBJ_DECLARE(
  node_556_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#28 */
AI_ARRAY_OBJ_DECLARE(
  node_386_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#29 */
AI_ARRAY_OBJ_DECLARE(
  node_559_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#30 */
AI_ARRAY_OBJ_DECLARE(
  node_390_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#31 */
AI_ARRAY_OBJ_DECLARE(
  node_562_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#32 */
AI_ARRAY_OBJ_DECLARE(
  node_394_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#33 */
AI_ARRAY_OBJ_DECLARE(
  node_565_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#34 */
AI_ARRAY_OBJ_DECLARE(
  node_397_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#35 */
AI_ARRAY_OBJ_DECLARE(
  node_568_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#36 */
AI_ARRAY_OBJ_DECLARE(
  node_401_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#37 */
AI_ARRAY_OBJ_DECLARE(
  node_571_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#38 */
AI_ARRAY_OBJ_DECLARE(
  node_405_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#39 */
AI_ARRAY_OBJ_DECLARE(
  node_574_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#40 */
AI_ARRAY_OBJ_DECLARE(
  node_577_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#41 */
AI_ARRAY_OBJ_DECLARE(
  node_411_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#42 */
AI_ARRAY_OBJ_DECLARE(
  node_580_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#43 */
AI_ARRAY_OBJ_DECLARE(
  node_415_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#44 */
AI_ARRAY_OBJ_DECLARE(
  node_583_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#45 */
AI_ARRAY_OBJ_DECLARE(
  node_418_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#46 */
AI_ARRAY_OBJ_DECLARE(
  node_586_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#47 */
AI_ARRAY_OBJ_DECLARE(
  node_422_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#48 */
AI_ARRAY_OBJ_DECLARE(
  node_589_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#49 */
AI_ARRAY_OBJ_DECLARE(
  node_426_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#50 */
AI_ARRAY_OBJ_DECLARE(
  node_592_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#51 */
AI_ARRAY_OBJ_DECLARE(
  node_429_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#52 */
AI_ARRAY_OBJ_DECLARE(
  node_595_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#53 */
AI_ARRAY_OBJ_DECLARE(
  node_433_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#54 */
AI_ARRAY_OBJ_DECLARE(
  node_598_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#55 */
AI_ARRAY_OBJ_DECLARE(
  node_437_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#56 */
AI_ARRAY_OBJ_DECLARE(
  node_601_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#57 */
AI_ARRAY_OBJ_DECLARE(
  node_440_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#58 */
AI_ARRAY_OBJ_DECLARE(
  node_604_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#59 */
AI_ARRAY_OBJ_DECLARE(
  node_444_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#60 */
AI_ARRAY_OBJ_DECLARE(
  node_607_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#61 */
AI_ARRAY_OBJ_DECLARE(
  node_448_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#62 */
AI_ARRAY_OBJ_DECLARE(
  node_610_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#63 */
AI_ARRAY_OBJ_DECLARE(
  node_451_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#64 */
AI_ARRAY_OBJ_DECLARE(
  node_613_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#65 */
AI_ARRAY_OBJ_DECLARE(
  node_455_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#66 */
AI_ARRAY_OBJ_DECLARE(
  node_616_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#67 */
AI_ARRAY_OBJ_DECLARE(
  node_459_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#68 */
AI_ARRAY_OBJ_DECLARE(
  node_619_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#69 */
AI_ARRAY_OBJ_DECLARE(
  node_462_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#70 */
AI_ARRAY_OBJ_DECLARE(
  node_622_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#71 */
AI_ARRAY_OBJ_DECLARE(
  node_466_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#72 */
AI_ARRAY_OBJ_DECLARE(
  node_625_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#73 */
AI_ARRAY_OBJ_DECLARE(
  node_470_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#74 */
AI_ARRAY_OBJ_DECLARE(
  node_628_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#75 */
AI_ARRAY_OBJ_DECLARE(
  node_473_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#76 */
AI_ARRAY_OBJ_DECLARE(
  node_631_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#77 */
AI_ARRAY_OBJ_DECLARE(
  node_477_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#78 */
AI_ARRAY_OBJ_DECLARE(
  node_634_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#79 */
AI_ARRAY_OBJ_DECLARE(
  node_481_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#80 */
AI_ARRAY_OBJ_DECLARE(
  node_637_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12544, AI_STATIC)

/* Array#81 */
AI_ARRAY_OBJ_DECLARE(
  node_640_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12544, AI_STATIC)

/* Array#82 */
AI_ARRAY_OBJ_DECLARE(
  node_487_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12544, AI_STATIC)

/* Array#83 */
AI_ARRAY_OBJ_DECLARE(
  node_643_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12544, AI_STATIC)

/* Array#84 */
AI_ARRAY_OBJ_DECLARE(
  node_491_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12544, AI_STATIC)

/* Array#85 */
AI_ARRAY_OBJ_DECLARE(
  node_646_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12544, AI_STATIC)

/* Array#86 */
AI_ARRAY_OBJ_DECLARE(
  node_494_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12544, AI_STATIC)

/* Array#87 */
AI_ARRAY_OBJ_DECLARE(
  node_649_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12544, AI_STATIC)

/* Array#88 */
AI_ARRAY_OBJ_DECLARE(
  node_498_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12544, AI_STATIC)

/* Array#89 */
AI_ARRAY_OBJ_DECLARE(
  node_652_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12544, AI_STATIC)

/* Array#90 */
AI_ARRAY_OBJ_DECLARE(
  node_502_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12544, AI_STATIC)

/* Array#91 */
AI_ARRAY_OBJ_DECLARE(
  node_655_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12544, AI_STATIC)

/* Array#92 */
AI_ARRAY_OBJ_DECLARE(
  node_505_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12544, AI_STATIC)

/* Array#93 */
AI_ARRAY_OBJ_DECLARE(
  node_658_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#94 */
AI_ARRAY_OBJ_DECLARE(
  node_509_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#95 */
AI_ARRAY_OBJ_DECLARE(
  node_661_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3136, AI_STATIC)

/* Array#96 */
AI_ARRAY_OBJ_DECLARE(
  node_513_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3136, AI_STATIC)

/* Array#97 */
AI_ARRAY_OBJ_DECLARE(
  node_514_to_chlast_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3136, AI_STATIC)

/* Array#98 */
AI_ARRAY_OBJ_DECLARE(
  node_515_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 512, AI_STATIC)

/* Array#99 */
AI_ARRAY_OBJ_DECLARE(
  node_517_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3456, AI_STATIC)

/* Array#100 */
AI_ARRAY_OBJ_DECLARE(
  node_517_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#101 */
AI_ARRAY_OBJ_DECLARE(
  node_339_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 129, AI_STATIC)

/* Array#102 */
AI_ARRAY_OBJ_DECLARE(
  node_520_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#103 */
AI_ARRAY_OBJ_DECLARE(
  node_520_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#104 */
AI_ARRAY_OBJ_DECLARE(
  node_343_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 129, AI_STATIC)

/* Array#105 */
AI_ARRAY_OBJ_DECLARE(
  node_523_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16384, AI_STATIC)

/* Array#106 */
AI_ARRAY_OBJ_DECLARE(
  node_523_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#107 */
AI_ARRAY_OBJ_DECLARE(
  node_347_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 129, AI_STATIC)

/* Array#108 */
AI_ARRAY_OBJ_DECLARE(
  node_526_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1152, AI_STATIC)

/* Array#109 */
AI_ARRAY_OBJ_DECLARE(
  node_526_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#110 */
AI_ARRAY_OBJ_DECLARE(
  node_351_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 129, AI_STATIC)

/* Array#111 */
AI_ARRAY_OBJ_DECLARE(
  node_529_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16384, AI_STATIC)

/* Array#112 */
AI_ARRAY_OBJ_DECLARE(
  node_529_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#113 */
AI_ARRAY_OBJ_DECLARE(
  node_532_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16384, AI_STATIC)

/* Array#114 */
AI_ARRAY_OBJ_DECLARE(
  node_532_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#115 */
AI_ARRAY_OBJ_DECLARE(
  node_357_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 129, AI_STATIC)

/* Array#116 */
AI_ARRAY_OBJ_DECLARE(
  node_535_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1152, AI_STATIC)

/* Array#117 */
AI_ARRAY_OBJ_DECLARE(
  node_535_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#118 */
AI_ARRAY_OBJ_DECLARE(
  node_361_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 129, AI_STATIC)

/* Array#119 */
AI_ARRAY_OBJ_DECLARE(
  node_538_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16384, AI_STATIC)

/* Array#120 */
AI_ARRAY_OBJ_DECLARE(
  node_538_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#121 */
AI_ARRAY_OBJ_DECLARE(
  node_541_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16384, AI_STATIC)

/* Array#122 */
AI_ARRAY_OBJ_DECLARE(
  node_541_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#123 */
AI_ARRAY_OBJ_DECLARE(
  node_368_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 129, AI_STATIC)

/* Array#124 */
AI_ARRAY_OBJ_DECLARE(
  node_544_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1152, AI_STATIC)

/* Array#125 */
AI_ARRAY_OBJ_DECLARE(
  node_544_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#126 */
AI_ARRAY_OBJ_DECLARE(
  node_372_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 129, AI_STATIC)

/* Array#127 */
AI_ARRAY_OBJ_DECLARE(
  node_547_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16384, AI_STATIC)

/* Array#128 */
AI_ARRAY_OBJ_DECLARE(
  node_547_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#129 */
AI_ARRAY_OBJ_DECLARE(
  node_550_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16384, AI_STATIC)

/* Array#130 */
AI_ARRAY_OBJ_DECLARE(
  node_550_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#131 */
AI_ARRAY_OBJ_DECLARE(
  node_379_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 129, AI_STATIC)

/* Array#132 */
AI_ARRAY_OBJ_DECLARE(
  node_553_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1152, AI_STATIC)

/* Array#133 */
AI_ARRAY_OBJ_DECLARE(
  node_553_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#134 */
AI_ARRAY_OBJ_DECLARE(
  node_383_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 129, AI_STATIC)

/* Array#135 */
AI_ARRAY_OBJ_DECLARE(
  node_556_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16384, AI_STATIC)

/* Array#136 */
AI_ARRAY_OBJ_DECLARE(
  node_556_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#137 */
AI_ARRAY_OBJ_DECLARE(
  node_559_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16384, AI_STATIC)

/* Array#138 */
AI_ARRAY_OBJ_DECLARE(
  node_559_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#139 */
AI_ARRAY_OBJ_DECLARE(
  node_390_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 129, AI_STATIC)

/* Array#140 */
AI_ARRAY_OBJ_DECLARE(
  node_562_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1152, AI_STATIC)

/* Array#141 */
AI_ARRAY_OBJ_DECLARE(
  node_562_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#142 */
AI_ARRAY_OBJ_DECLARE(
  node_394_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 129, AI_STATIC)

/* Array#143 */
AI_ARRAY_OBJ_DECLARE(
  node_565_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16384, AI_STATIC)

/* Array#144 */
AI_ARRAY_OBJ_DECLARE(
  node_565_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#145 */
AI_ARRAY_OBJ_DECLARE(
  node_568_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 32768, AI_STATIC)

/* Array#146 */
AI_ARRAY_OBJ_DECLARE(
  node_568_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#147 */
AI_ARRAY_OBJ_DECLARE(
  node_401_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 257, AI_STATIC)

/* Array#148 */
AI_ARRAY_OBJ_DECLARE(
  node_571_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#149 */
AI_ARRAY_OBJ_DECLARE(
  node_571_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#150 */
AI_ARRAY_OBJ_DECLARE(
  node_405_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 257, AI_STATIC)

/* Array#151 */
AI_ARRAY_OBJ_DECLARE(
  node_574_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 65536, AI_STATIC)

/* Array#152 */
AI_ARRAY_OBJ_DECLARE(
  node_574_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#153 */
AI_ARRAY_OBJ_DECLARE(
  node_577_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 65536, AI_STATIC)

/* Array#154 */
AI_ARRAY_OBJ_DECLARE(
  node_577_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#155 */
AI_ARRAY_OBJ_DECLARE(
  node_411_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 257, AI_STATIC)

/* Array#156 */
AI_ARRAY_OBJ_DECLARE(
  node_580_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#157 */
AI_ARRAY_OBJ_DECLARE(
  node_580_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#158 */
AI_ARRAY_OBJ_DECLARE(
  node_415_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 257, AI_STATIC)

/* Array#159 */
AI_ARRAY_OBJ_DECLARE(
  node_583_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 65536, AI_STATIC)

/* Array#160 */
AI_ARRAY_OBJ_DECLARE(
  node_583_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#161 */
AI_ARRAY_OBJ_DECLARE(
  node_586_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 65536, AI_STATIC)

/* Array#162 */
AI_ARRAY_OBJ_DECLARE(
  node_586_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#163 */
AI_ARRAY_OBJ_DECLARE(
  node_422_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 257, AI_STATIC)

/* Array#164 */
AI_ARRAY_OBJ_DECLARE(
  node_589_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#165 */
AI_ARRAY_OBJ_DECLARE(
  node_589_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#166 */
AI_ARRAY_OBJ_DECLARE(
  node_426_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 257, AI_STATIC)

/* Array#167 */
AI_ARRAY_OBJ_DECLARE(
  node_592_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 65536, AI_STATIC)

/* Array#168 */
AI_ARRAY_OBJ_DECLARE(
  node_592_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#169 */
AI_ARRAY_OBJ_DECLARE(
  node_595_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 65536, AI_STATIC)

/* Array#170 */
AI_ARRAY_OBJ_DECLARE(
  node_595_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#171 */
AI_ARRAY_OBJ_DECLARE(
  node_433_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 257, AI_STATIC)

/* Array#172 */
AI_ARRAY_OBJ_DECLARE(
  node_598_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#173 */
AI_ARRAY_OBJ_DECLARE(
  node_598_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#174 */
AI_ARRAY_OBJ_DECLARE(
  node_437_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 257, AI_STATIC)

/* Array#175 */
AI_ARRAY_OBJ_DECLARE(
  node_601_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 65536, AI_STATIC)

/* Array#176 */
AI_ARRAY_OBJ_DECLARE(
  node_601_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#177 */
AI_ARRAY_OBJ_DECLARE(
  node_604_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 65536, AI_STATIC)

/* Array#178 */
AI_ARRAY_OBJ_DECLARE(
  node_604_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#179 */
AI_ARRAY_OBJ_DECLARE(
  node_444_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 257, AI_STATIC)

/* Array#180 */
AI_ARRAY_OBJ_DECLARE(
  node_607_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#181 */
AI_ARRAY_OBJ_DECLARE(
  node_607_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#182 */
AI_ARRAY_OBJ_DECLARE(
  node_448_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 257, AI_STATIC)

/* Array#183 */
AI_ARRAY_OBJ_DECLARE(
  node_610_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 65536, AI_STATIC)

/* Array#184 */
AI_ARRAY_OBJ_DECLARE(
  node_610_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#185 */
AI_ARRAY_OBJ_DECLARE(
  node_613_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 65536, AI_STATIC)

/* Array#186 */
AI_ARRAY_OBJ_DECLARE(
  node_613_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#187 */
AI_ARRAY_OBJ_DECLARE(
  node_455_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 257, AI_STATIC)

/* Array#188 */
AI_ARRAY_OBJ_DECLARE(
  node_616_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#189 */
AI_ARRAY_OBJ_DECLARE(
  node_616_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#190 */
AI_ARRAY_OBJ_DECLARE(
  node_459_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 257, AI_STATIC)

/* Array#191 */
AI_ARRAY_OBJ_DECLARE(
  node_619_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 65536, AI_STATIC)

/* Array#192 */
AI_ARRAY_OBJ_DECLARE(
  node_619_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#193 */
AI_ARRAY_OBJ_DECLARE(
  node_622_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 65536, AI_STATIC)

/* Array#194 */
AI_ARRAY_OBJ_DECLARE(
  node_622_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#195 */
AI_ARRAY_OBJ_DECLARE(
  node_466_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 257, AI_STATIC)

/* Array#196 */
AI_ARRAY_OBJ_DECLARE(
  node_625_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#197 */
AI_ARRAY_OBJ_DECLARE(
  node_625_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#198 */
AI_ARRAY_OBJ_DECLARE(
  node_470_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 257, AI_STATIC)

/* Array#199 */
AI_ARRAY_OBJ_DECLARE(
  node_628_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 65536, AI_STATIC)

/* Array#200 */
AI_ARRAY_OBJ_DECLARE(
  node_628_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#201 */
AI_ARRAY_OBJ_DECLARE(
  node_631_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 131072, AI_STATIC)

/* Array#202 */
AI_ARRAY_OBJ_DECLARE(
  node_631_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#203 */
AI_ARRAY_OBJ_DECLARE(
  node_477_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 513, AI_STATIC)

/* Array#204 */
AI_ARRAY_OBJ_DECLARE(
  node_634_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4608, AI_STATIC)

/* Array#205 */
AI_ARRAY_OBJ_DECLARE(
  node_634_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#206 */
AI_ARRAY_OBJ_DECLARE(
  node_481_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 513, AI_STATIC)

/* Array#207 */
AI_ARRAY_OBJ_DECLARE(
  node_637_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 131072, AI_STATIC)

/* Array#208 */
AI_ARRAY_OBJ_DECLARE(
  node_637_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#209 */
AI_ARRAY_OBJ_DECLARE(
  node_640_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 65536, AI_STATIC)

/* Array#210 */
AI_ARRAY_OBJ_DECLARE(
  node_640_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#211 */
AI_ARRAY_OBJ_DECLARE(
  node_487_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 257, AI_STATIC)

/* Array#212 */
AI_ARRAY_OBJ_DECLARE(
  node_643_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#213 */
AI_ARRAY_OBJ_DECLARE(
  node_643_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#214 */
AI_ARRAY_OBJ_DECLARE(
  node_491_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 257, AI_STATIC)

/* Array#215 */
AI_ARRAY_OBJ_DECLARE(
  node_646_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 65536, AI_STATIC)

/* Array#216 */
AI_ARRAY_OBJ_DECLARE(
  node_646_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#217 */
AI_ARRAY_OBJ_DECLARE(
  node_649_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 65536, AI_STATIC)

/* Array#218 */
AI_ARRAY_OBJ_DECLARE(
  node_649_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#219 */
AI_ARRAY_OBJ_DECLARE(
  node_498_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 257, AI_STATIC)

/* Array#220 */
AI_ARRAY_OBJ_DECLARE(
  node_652_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#221 */
AI_ARRAY_OBJ_DECLARE(
  node_652_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#222 */
AI_ARRAY_OBJ_DECLARE(
  node_502_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 257, AI_STATIC)

/* Array#223 */
AI_ARRAY_OBJ_DECLARE(
  node_655_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 65536, AI_STATIC)

/* Array#224 */
AI_ARRAY_OBJ_DECLARE(
  node_655_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#225 */
AI_ARRAY_OBJ_DECLARE(
  node_658_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 131072, AI_STATIC)

/* Array#226 */
AI_ARRAY_OBJ_DECLARE(
  node_658_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#227 */
AI_ARRAY_OBJ_DECLARE(
  node_509_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 513, AI_STATIC)

/* Array#228 */
AI_ARRAY_OBJ_DECLARE(
  node_661_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 32768, AI_STATIC)

/* Array#229 */
AI_ARRAY_OBJ_DECLARE(
  node_661_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#230 */
AI_ARRAY_OBJ_DECLARE(
  node_513_alpha_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 65, AI_STATIC)

/* Array#231 */
AI_ARRAY_OBJ_DECLARE(
  node_515_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1605632, AI_STATIC)

/* Array#232 */
AI_ARRAY_OBJ_DECLARE(
  node_515_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#233 */
AI_ARRAY_OBJ_DECLARE(
  node_517_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 27, AI_STATIC)

/* Array#234 */
AI_ARRAY_OBJ_DECLARE(
  node_523_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#235 */
AI_ARRAY_OBJ_DECLARE(
  node_529_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#236 */
AI_ARRAY_OBJ_DECLARE(
  node_532_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#237 */
AI_ARRAY_OBJ_DECLARE(
  node_538_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#238 */
AI_ARRAY_OBJ_DECLARE(
  node_541_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#239 */
AI_ARRAY_OBJ_DECLARE(
  node_547_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#240 */
AI_ARRAY_OBJ_DECLARE(
  node_550_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#241 */
AI_ARRAY_OBJ_DECLARE(
  node_556_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#242 */
AI_ARRAY_OBJ_DECLARE(
  node_559_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#243 */
AI_ARRAY_OBJ_DECLARE(
  node_565_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#244 */
AI_ARRAY_OBJ_DECLARE(
  node_568_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#245 */
AI_ARRAY_OBJ_DECLARE(
  node_574_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#246 */
AI_ARRAY_OBJ_DECLARE(
  node_577_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#247 */
AI_ARRAY_OBJ_DECLARE(
  node_583_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#248 */
AI_ARRAY_OBJ_DECLARE(
  node_586_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#249 */
AI_ARRAY_OBJ_DECLARE(
  node_592_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#250 */
AI_ARRAY_OBJ_DECLARE(
  node_595_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#251 */
AI_ARRAY_OBJ_DECLARE(
  node_601_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#252 */
AI_ARRAY_OBJ_DECLARE(
  node_604_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#253 */
AI_ARRAY_OBJ_DECLARE(
  node_610_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#254 */
AI_ARRAY_OBJ_DECLARE(
  node_613_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#255 */
AI_ARRAY_OBJ_DECLARE(
  node_619_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#256 */
AI_ARRAY_OBJ_DECLARE(
  node_622_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#257 */
AI_ARRAY_OBJ_DECLARE(
  node_628_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#258 */
AI_ARRAY_OBJ_DECLARE(
  node_631_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#259 */
AI_ARRAY_OBJ_DECLARE(
  node_637_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#260 */
AI_ARRAY_OBJ_DECLARE(
  node_640_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#261 */
AI_ARRAY_OBJ_DECLARE(
  node_646_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#262 */
AI_ARRAY_OBJ_DECLARE(
  node_649_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#263 */
AI_ARRAY_OBJ_DECLARE(
  node_655_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#264 */
AI_ARRAY_OBJ_DECLARE(
  node_658_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#265 */
AI_ARRAY_OBJ_DECLARE(
  node_661_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  input_1_Transpose_output, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 112, 112), AI_STRIDE_INIT(4, 4, 4, 12, 1344),
  1, &input_1_Transpose_output_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  input_1_output, AI_STATIC,
  1, 0x0,
  AI_SHAPE_INIT(4, 1, 112, 112, 3), AI_STRIDE_INIT(4, 4, 4, 448, 50176),
  1, &input_1_output_array, NULL)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  node_339_alpha, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_339_alpha_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  node_339_output, AI_STATIC,
  3, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 4, 4, 512, 28672),
  1, &node_339_output_array, NULL)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  node_343_alpha, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_343_alpha_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  node_343_output, AI_STATIC,
  5, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 4, 4, 512, 28672),
  1, &node_343_output_array, NULL)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  node_347_alpha, AI_STATIC,
  6, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_347_alpha_array, NULL)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  node_347_output, AI_STATIC,
  7, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 4, 4, 512, 28672),
  1, &node_347_output_array, NULL)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  node_351_alpha, AI_STATIC,
  8, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_351_alpha_array, NULL)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  node_351_output, AI_STATIC,
  9, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_351_output_array, NULL)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  node_357_alpha, AI_STATIC,
  10, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_357_alpha_array, NULL)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  node_357_output, AI_STATIC,
  11, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_357_output_array, NULL)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  node_361_alpha, AI_STATIC,
  12, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_361_alpha_array, NULL)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  node_361_output, AI_STATIC,
  13, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_361_output_array, NULL)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  node_364_output, AI_STATIC,
  14, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_364_output_array, NULL)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  node_368_alpha, AI_STATIC,
  15, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_368_alpha_array, NULL)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  node_368_output, AI_STATIC,
  16, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_368_output_array, NULL)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  node_372_alpha, AI_STATIC,
  17, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_372_alpha_array, NULL)

/* Tensor #18 */
AI_TENSOR_OBJ_DECLARE(
  node_372_output, AI_STATIC,
  18, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_372_output_array, NULL)

/* Tensor #19 */
AI_TENSOR_OBJ_DECLARE(
  node_375_output, AI_STATIC,
  19, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_375_output_array, NULL)

/* Tensor #20 */
AI_TENSOR_OBJ_DECLARE(
  node_379_alpha, AI_STATIC,
  20, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_379_alpha_array, NULL)

/* Tensor #21 */
AI_TENSOR_OBJ_DECLARE(
  node_379_output, AI_STATIC,
  21, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_379_output_array, NULL)

/* Tensor #22 */
AI_TENSOR_OBJ_DECLARE(
  node_383_alpha, AI_STATIC,
  22, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_383_alpha_array, NULL)

/* Tensor #23 */
AI_TENSOR_OBJ_DECLARE(
  node_383_output, AI_STATIC,
  23, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_383_output_array, NULL)

/* Tensor #24 */
AI_TENSOR_OBJ_DECLARE(
  node_386_output, AI_STATIC,
  24, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_386_output_array, NULL)

/* Tensor #25 */
AI_TENSOR_OBJ_DECLARE(
  node_390_alpha, AI_STATIC,
  25, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_390_alpha_array, NULL)

/* Tensor #26 */
AI_TENSOR_OBJ_DECLARE(
  node_390_output, AI_STATIC,
  26, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_390_output_array, NULL)

/* Tensor #27 */
AI_TENSOR_OBJ_DECLARE(
  node_394_alpha, AI_STATIC,
  27, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_394_alpha_array, NULL)

/* Tensor #28 */
AI_TENSOR_OBJ_DECLARE(
  node_394_output, AI_STATIC,
  28, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_394_output_array, NULL)

/* Tensor #29 */
AI_TENSOR_OBJ_DECLARE(
  node_397_output, AI_STATIC,
  29, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_397_output_array, NULL)

/* Tensor #30 */
AI_TENSOR_OBJ_DECLARE(
  node_401_alpha, AI_STATIC,
  30, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_401_alpha_array, NULL)

/* Tensor #31 */
AI_TENSOR_OBJ_DECLARE(
  node_401_output, AI_STATIC,
  31, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 28, 28), AI_STRIDE_INIT(4, 4, 4, 1024, 28672),
  1, &node_401_output_array, NULL)

/* Tensor #32 */
AI_TENSOR_OBJ_DECLARE(
  node_405_alpha, AI_STATIC,
  32, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_405_alpha_array, NULL)

/* Tensor #33 */
AI_TENSOR_OBJ_DECLARE(
  node_405_output, AI_STATIC,
  33, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_405_output_array, NULL)

/* Tensor #34 */
AI_TENSOR_OBJ_DECLARE(
  node_411_alpha, AI_STATIC,
  34, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_411_alpha_array, NULL)

/* Tensor #35 */
AI_TENSOR_OBJ_DECLARE(
  node_411_output, AI_STATIC,
  35, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_411_output_array, NULL)

/* Tensor #36 */
AI_TENSOR_OBJ_DECLARE(
  node_415_alpha, AI_STATIC,
  36, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_415_alpha_array, NULL)

/* Tensor #37 */
AI_TENSOR_OBJ_DECLARE(
  node_415_output, AI_STATIC,
  37, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_415_output_array, NULL)

/* Tensor #38 */
AI_TENSOR_OBJ_DECLARE(
  node_418_output, AI_STATIC,
  38, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_418_output_array, NULL)

/* Tensor #39 */
AI_TENSOR_OBJ_DECLARE(
  node_422_alpha, AI_STATIC,
  39, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_422_alpha_array, NULL)

/* Tensor #40 */
AI_TENSOR_OBJ_DECLARE(
  node_422_output, AI_STATIC,
  40, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_422_output_array, NULL)

/* Tensor #41 */
AI_TENSOR_OBJ_DECLARE(
  node_426_alpha, AI_STATIC,
  41, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_426_alpha_array, NULL)

/* Tensor #42 */
AI_TENSOR_OBJ_DECLARE(
  node_426_output, AI_STATIC,
  42, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_426_output_array, NULL)

/* Tensor #43 */
AI_TENSOR_OBJ_DECLARE(
  node_429_output, AI_STATIC,
  43, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_429_output_array, NULL)

/* Tensor #44 */
AI_TENSOR_OBJ_DECLARE(
  node_433_alpha, AI_STATIC,
  44, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_433_alpha_array, NULL)

/* Tensor #45 */
AI_TENSOR_OBJ_DECLARE(
  node_433_output, AI_STATIC,
  45, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_433_output_array, NULL)

/* Tensor #46 */
AI_TENSOR_OBJ_DECLARE(
  node_437_alpha, AI_STATIC,
  46, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_437_alpha_array, NULL)

/* Tensor #47 */
AI_TENSOR_OBJ_DECLARE(
  node_437_output, AI_STATIC,
  47, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_437_output_array, NULL)

/* Tensor #48 */
AI_TENSOR_OBJ_DECLARE(
  node_440_output, AI_STATIC,
  48, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_440_output_array, NULL)

/* Tensor #49 */
AI_TENSOR_OBJ_DECLARE(
  node_444_alpha, AI_STATIC,
  49, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_444_alpha_array, NULL)

/* Tensor #50 */
AI_TENSOR_OBJ_DECLARE(
  node_444_output, AI_STATIC,
  50, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_444_output_array, NULL)

/* Tensor #51 */
AI_TENSOR_OBJ_DECLARE(
  node_448_alpha, AI_STATIC,
  51, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_448_alpha_array, NULL)

/* Tensor #52 */
AI_TENSOR_OBJ_DECLARE(
  node_448_output, AI_STATIC,
  52, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_448_output_array, NULL)

/* Tensor #53 */
AI_TENSOR_OBJ_DECLARE(
  node_451_output, AI_STATIC,
  53, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_451_output_array, NULL)

/* Tensor #54 */
AI_TENSOR_OBJ_DECLARE(
  node_455_alpha, AI_STATIC,
  54, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_455_alpha_array, NULL)

/* Tensor #55 */
AI_TENSOR_OBJ_DECLARE(
  node_455_output, AI_STATIC,
  55, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_455_output_array, NULL)

/* Tensor #56 */
AI_TENSOR_OBJ_DECLARE(
  node_459_alpha, AI_STATIC,
  56, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_459_alpha_array, NULL)

/* Tensor #57 */
AI_TENSOR_OBJ_DECLARE(
  node_459_output, AI_STATIC,
  57, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_459_output_array, NULL)

/* Tensor #58 */
AI_TENSOR_OBJ_DECLARE(
  node_462_output, AI_STATIC,
  58, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_462_output_array, NULL)

/* Tensor #59 */
AI_TENSOR_OBJ_DECLARE(
  node_466_alpha, AI_STATIC,
  59, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_466_alpha_array, NULL)

/* Tensor #60 */
AI_TENSOR_OBJ_DECLARE(
  node_466_output, AI_STATIC,
  60, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_466_output_array, NULL)

/* Tensor #61 */
AI_TENSOR_OBJ_DECLARE(
  node_470_alpha, AI_STATIC,
  61, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_470_alpha_array, NULL)

/* Tensor #62 */
AI_TENSOR_OBJ_DECLARE(
  node_470_output, AI_STATIC,
  62, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_470_output_array, NULL)

/* Tensor #63 */
AI_TENSOR_OBJ_DECLARE(
  node_473_output, AI_STATIC,
  63, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_473_output_array, NULL)

/* Tensor #64 */
AI_TENSOR_OBJ_DECLARE(
  node_477_alpha, AI_STATIC,
  64, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &node_477_alpha_array, NULL)

/* Tensor #65 */
AI_TENSOR_OBJ_DECLARE(
  node_477_output, AI_STATIC,
  65, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &node_477_output_array, NULL)

/* Tensor #66 */
AI_TENSOR_OBJ_DECLARE(
  node_481_alpha, AI_STATIC,
  66, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &node_481_alpha_array, NULL)

/* Tensor #67 */
AI_TENSOR_OBJ_DECLARE(
  node_481_output, AI_STATIC,
  67, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &node_481_output_array, NULL)

/* Tensor #68 */
AI_TENSOR_OBJ_DECLARE(
  node_487_alpha, AI_STATIC,
  68, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_487_alpha_array, NULL)

/* Tensor #69 */
AI_TENSOR_OBJ_DECLARE(
  node_487_output, AI_STATIC,
  69, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 7, 7), AI_STRIDE_INIT(4, 4, 4, 1024, 7168),
  1, &node_487_output_array, NULL)

/* Tensor #70 */
AI_TENSOR_OBJ_DECLARE(
  node_491_alpha, AI_STATIC,
  70, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_491_alpha_array, NULL)

/* Tensor #71 */
AI_TENSOR_OBJ_DECLARE(
  node_491_output, AI_STATIC,
  71, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 7, 7), AI_STRIDE_INIT(4, 4, 4, 1024, 7168),
  1, &node_491_output_array, NULL)

/* Tensor #72 */
AI_TENSOR_OBJ_DECLARE(
  node_494_output, AI_STATIC,
  72, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 7, 7), AI_STRIDE_INIT(4, 4, 4, 1024, 7168),
  1, &node_494_output_array, NULL)

/* Tensor #73 */
AI_TENSOR_OBJ_DECLARE(
  node_498_alpha, AI_STATIC,
  73, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_498_alpha_array, NULL)

/* Tensor #74 */
AI_TENSOR_OBJ_DECLARE(
  node_498_output, AI_STATIC,
  74, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 7, 7), AI_STRIDE_INIT(4, 4, 4, 1024, 7168),
  1, &node_498_output_array, NULL)

/* Tensor #75 */
AI_TENSOR_OBJ_DECLARE(
  node_502_alpha, AI_STATIC,
  75, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_502_alpha_array, NULL)

/* Tensor #76 */
AI_TENSOR_OBJ_DECLARE(
  node_502_output, AI_STATIC,
  76, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 7, 7), AI_STRIDE_INIT(4, 4, 4, 1024, 7168),
  1, &node_502_output_array, NULL)

/* Tensor #77 */
AI_TENSOR_OBJ_DECLARE(
  node_505_output, AI_STATIC,
  77, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 7, 7), AI_STRIDE_INIT(4, 4, 4, 1024, 7168),
  1, &node_505_output_array, NULL)

/* Tensor #78 */
AI_TENSOR_OBJ_DECLARE(
  node_509_alpha, AI_STATIC,
  78, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &node_509_alpha_array, NULL)

/* Tensor #79 */
AI_TENSOR_OBJ_DECLARE(
  node_509_output, AI_STATIC,
  79, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &node_509_output_array, NULL)

/* Tensor #80 */
AI_TENSOR_OBJ_DECLARE(
  node_513_alpha, AI_STATIC,
  80, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &node_513_alpha_array, NULL)

/* Tensor #81 */
AI_TENSOR_OBJ_DECLARE(
  node_513_output, AI_STATIC,
  81, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 7, 7), AI_STRIDE_INIT(4, 4, 4, 256, 1792),
  1, &node_513_output_array, NULL)

/* Tensor #82 */
AI_TENSOR_OBJ_DECLARE(
  node_514_to_chlast_output, AI_STATIC,
  82, 0x0,
  AI_SHAPE_INIT(4, 1, 7, 7, 64), AI_STRIDE_INIT(4, 4, 4, 28, 196),
  1, &node_514_to_chlast_output_array, NULL)

/* Tensor #83 */
AI_TENSOR_OBJ_DECLARE(
  node_514_to_chlast_output0, AI_STATIC,
  83, 0x0,
  AI_SHAPE_INIT(4, 1, 3136, 1, 1), AI_STRIDE_INIT(4, 4, 4, 12544, 12544),
  1, &node_514_to_chlast_output_array, NULL)

/* Tensor #84 */
AI_TENSOR_OBJ_DECLARE(
  node_515_bias, AI_STATIC,
  84, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &node_515_bias_array, NULL)

/* Tensor #85 */
AI_TENSOR_OBJ_DECLARE(
  node_515_output, AI_STATIC,
  85, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &node_515_output_array, NULL)

/* Tensor #86 */
AI_TENSOR_OBJ_DECLARE(
  node_515_weights, AI_STATIC,
  86, 0x0,
  AI_SHAPE_INIT(4, 3136, 512, 1, 1), AI_STRIDE_INIT(4, 4, 12544, 6422528, 6422528),
  1, &node_515_weights_array, NULL)

/* Tensor #87 */
AI_TENSOR_OBJ_DECLARE(
  node_517_bias, AI_STATIC,
  87, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_517_bias_array, NULL)

/* Tensor #88 */
AI_TENSOR_OBJ_DECLARE(
  node_517_output, AI_STATIC,
  88, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 4, 4, 512, 28672),
  1, &node_517_output_array, NULL)

/* Tensor #89 */
AI_TENSOR_OBJ_DECLARE(
  node_517_scratch0, AI_STATIC,
  89, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 3), AI_STRIDE_INIT(4, 4, 4, 12, 36),
  1, &node_517_scratch0_array, NULL)

/* Tensor #90 */
AI_TENSOR_OBJ_DECLARE(
  node_517_weights, AI_STATIC,
  90, 0x0,
  AI_SHAPE_INIT(4, 3, 3, 3, 128), AI_STRIDE_INIT(4, 4, 12, 1536, 4608),
  1, &node_517_weights_array, NULL)

/* Tensor #91 */
AI_TENSOR_OBJ_DECLARE(
  node_520_bias, AI_STATIC,
  91, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_520_bias_array, NULL)

/* Tensor #92 */
AI_TENSOR_OBJ_DECLARE(
  node_520_output, AI_STATIC,
  92, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 4, 4, 512, 28672),
  1, &node_520_output_array, NULL)

/* Tensor #93 */
AI_TENSOR_OBJ_DECLARE(
  node_520_weights, AI_STATIC,
  93, 0x0,
  AI_SHAPE_INIT(4, 2, 3, 3, 128), AI_STRIDE_INIT(4, 4, 8, 1024, 3072),
  1, &node_520_weights_array, NULL)

/* Tensor #94 */
AI_TENSOR_OBJ_DECLARE(
  node_523_bias, AI_STATIC,
  94, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_523_bias_array, NULL)

/* Tensor #95 */
AI_TENSOR_OBJ_DECLARE(
  node_523_output, AI_STATIC,
  95, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 4, 4, 512, 28672),
  1, &node_523_output_array, NULL)

/* Tensor #96 */
AI_TENSOR_OBJ_DECLARE(
  node_523_scratch0, AI_STATIC,
  96, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_523_scratch0_array, NULL)

/* Tensor #97 */
AI_TENSOR_OBJ_DECLARE(
  node_523_weights, AI_STATIC,
  97, 0x0,
  AI_SHAPE_INIT(4, 128, 1, 1, 128), AI_STRIDE_INIT(4, 4, 512, 65536, 65536),
  1, &node_523_weights_array, NULL)

/* Tensor #98 */
AI_TENSOR_OBJ_DECLARE(
  node_526_bias, AI_STATIC,
  98, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_526_bias_array, NULL)

/* Tensor #99 */
AI_TENSOR_OBJ_DECLARE(
  node_526_output, AI_STATIC,
  99, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_526_output_array, NULL)

/* Tensor #100 */
AI_TENSOR_OBJ_DECLARE(
  node_526_weights, AI_STATIC,
  100, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 128), AI_STRIDE_INIT(4, 1, 128, 128, 128),
  1, &node_526_weights_array, NULL)

/* Tensor #101 */
AI_TENSOR_OBJ_DECLARE(
  node_529_bias, AI_STATIC,
  101, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_529_bias_array, NULL)

/* Tensor #102 */
AI_TENSOR_OBJ_DECLARE(
  node_529_output, AI_STATIC,
  102, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_529_output_array, NULL)

/* Tensor #103 */
AI_TENSOR_OBJ_DECLARE(
  node_529_scratch0, AI_STATIC,
  103, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_529_scratch0_array, NULL)

/* Tensor #104 */
AI_TENSOR_OBJ_DECLARE(
  node_529_weights, AI_STATIC,
  104, 0x0,
  AI_SHAPE_INIT(4, 128, 1, 1, 128), AI_STRIDE_INIT(4, 4, 512, 65536, 65536),
  1, &node_529_weights_array, NULL)

/* Tensor #105 */
AI_TENSOR_OBJ_DECLARE(
  node_532_bias, AI_STATIC,
  105, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_532_bias_array, NULL)

/* Tensor #106 */
AI_TENSOR_OBJ_DECLARE(
  node_532_output, AI_STATIC,
  106, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_532_output_array, NULL)

/* Tensor #107 */
AI_TENSOR_OBJ_DECLARE(
  node_532_scratch0, AI_STATIC,
  107, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_532_scratch0_array, NULL)

/* Tensor #108 */
AI_TENSOR_OBJ_DECLARE(
  node_532_weights, AI_STATIC,
  108, 0x0,
  AI_SHAPE_INIT(4, 128, 1, 1, 128), AI_STRIDE_INIT(4, 4, 512, 65536, 65536),
  1, &node_532_weights_array, NULL)

/* Tensor #109 */
AI_TENSOR_OBJ_DECLARE(
  node_535_bias, AI_STATIC,
  109, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_535_bias_array, NULL)

/* Tensor #110 */
AI_TENSOR_OBJ_DECLARE(
  node_535_output, AI_STATIC,
  110, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_535_output_array, NULL)

/* Tensor #111 */
AI_TENSOR_OBJ_DECLARE(
  node_535_weights, AI_STATIC,
  111, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 128), AI_STRIDE_INIT(4, 1, 128, 128, 128),
  1, &node_535_weights_array, NULL)

/* Tensor #112 */
AI_TENSOR_OBJ_DECLARE(
  node_538_bias, AI_STATIC,
  112, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_538_bias_array, NULL)

/* Tensor #113 */
AI_TENSOR_OBJ_DECLARE(
  node_538_output, AI_STATIC,
  113, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_538_output_array, NULL)

/* Tensor #114 */
AI_TENSOR_OBJ_DECLARE(
  node_538_scratch0, AI_STATIC,
  114, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_538_scratch0_array, NULL)

/* Tensor #115 */
AI_TENSOR_OBJ_DECLARE(
  node_538_weights, AI_STATIC,
  115, 0x0,
  AI_SHAPE_INIT(4, 128, 1, 1, 128), AI_STRIDE_INIT(4, 4, 512, 65536, 65536),
  1, &node_538_weights_array, NULL)

/* Tensor #116 */
AI_TENSOR_OBJ_DECLARE(
  node_541_bias, AI_STATIC,
  116, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_541_bias_array, NULL)

/* Tensor #117 */
AI_TENSOR_OBJ_DECLARE(
  node_541_output, AI_STATIC,
  117, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_541_output_array, NULL)

/* Tensor #118 */
AI_TENSOR_OBJ_DECLARE(
  node_541_scratch0, AI_STATIC,
  118, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_541_scratch0_array, NULL)

/* Tensor #119 */
AI_TENSOR_OBJ_DECLARE(
  node_541_weights, AI_STATIC,
  119, 0x0,
  AI_SHAPE_INIT(4, 128, 1, 1, 128), AI_STRIDE_INIT(4, 4, 512, 65536, 65536),
  1, &node_541_weights_array, NULL)

/* Tensor #120 */
AI_TENSOR_OBJ_DECLARE(
  node_544_bias, AI_STATIC,
  120, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_544_bias_array, NULL)

/* Tensor #121 */
AI_TENSOR_OBJ_DECLARE(
  node_544_output, AI_STATIC,
  121, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_544_output_array, NULL)

/* Tensor #122 */
AI_TENSOR_OBJ_DECLARE(
  node_544_weights, AI_STATIC,
  122, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 128), AI_STRIDE_INIT(4, 1, 128, 128, 128),
  1, &node_544_weights_array, NULL)

/* Tensor #123 */
AI_TENSOR_OBJ_DECLARE(
  node_547_bias, AI_STATIC,
  123, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_547_bias_array, NULL)

/* Tensor #124 */
AI_TENSOR_OBJ_DECLARE(
  node_547_output, AI_STATIC,
  124, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_547_output_array, NULL)

/* Tensor #125 */
AI_TENSOR_OBJ_DECLARE(
  node_547_scratch0, AI_STATIC,
  125, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_547_scratch0_array, NULL)

/* Tensor #126 */
AI_TENSOR_OBJ_DECLARE(
  node_547_weights, AI_STATIC,
  126, 0x0,
  AI_SHAPE_INIT(4, 128, 1, 1, 128), AI_STRIDE_INIT(4, 4, 512, 65536, 65536),
  1, &node_547_weights_array, NULL)

/* Tensor #127 */
AI_TENSOR_OBJ_DECLARE(
  node_550_bias, AI_STATIC,
  127, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_550_bias_array, NULL)

/* Tensor #128 */
AI_TENSOR_OBJ_DECLARE(
  node_550_output, AI_STATIC,
  128, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_550_output_array, NULL)

/* Tensor #129 */
AI_TENSOR_OBJ_DECLARE(
  node_550_scratch0, AI_STATIC,
  129, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_550_scratch0_array, NULL)

/* Tensor #130 */
AI_TENSOR_OBJ_DECLARE(
  node_550_weights, AI_STATIC,
  130, 0x0,
  AI_SHAPE_INIT(4, 128, 1, 1, 128), AI_STRIDE_INIT(4, 4, 512, 65536, 65536),
  1, &node_550_weights_array, NULL)

/* Tensor #131 */
AI_TENSOR_OBJ_DECLARE(
  node_553_bias, AI_STATIC,
  131, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_553_bias_array, NULL)

/* Tensor #132 */
AI_TENSOR_OBJ_DECLARE(
  node_553_output, AI_STATIC,
  132, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_553_output_array, NULL)

/* Tensor #133 */
AI_TENSOR_OBJ_DECLARE(
  node_553_weights, AI_STATIC,
  133, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 128), AI_STRIDE_INIT(4, 1, 128, 128, 128),
  1, &node_553_weights_array, NULL)

/* Tensor #134 */
AI_TENSOR_OBJ_DECLARE(
  node_556_bias, AI_STATIC,
  134, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_556_bias_array, NULL)

/* Tensor #135 */
AI_TENSOR_OBJ_DECLARE(
  node_556_output, AI_STATIC,
  135, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_556_output_array, NULL)

/* Tensor #136 */
AI_TENSOR_OBJ_DECLARE(
  node_556_scratch0, AI_STATIC,
  136, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_556_scratch0_array, NULL)

/* Tensor #137 */
AI_TENSOR_OBJ_DECLARE(
  node_556_weights, AI_STATIC,
  137, 0x0,
  AI_SHAPE_INIT(4, 128, 1, 1, 128), AI_STRIDE_INIT(4, 4, 512, 65536, 65536),
  1, &node_556_weights_array, NULL)

/* Tensor #138 */
AI_TENSOR_OBJ_DECLARE(
  node_559_bias, AI_STATIC,
  138, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_559_bias_array, NULL)

/* Tensor #139 */
AI_TENSOR_OBJ_DECLARE(
  node_559_output, AI_STATIC,
  139, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_559_output_array, NULL)

/* Tensor #140 */
AI_TENSOR_OBJ_DECLARE(
  node_559_scratch0, AI_STATIC,
  140, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_559_scratch0_array, NULL)

/* Tensor #141 */
AI_TENSOR_OBJ_DECLARE(
  node_559_weights, AI_STATIC,
  141, 0x0,
  AI_SHAPE_INIT(4, 128, 1, 1, 128), AI_STRIDE_INIT(4, 4, 512, 65536, 65536),
  1, &node_559_weights_array, NULL)

/* Tensor #142 */
AI_TENSOR_OBJ_DECLARE(
  node_562_bias, AI_STATIC,
  142, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_562_bias_array, NULL)

/* Tensor #143 */
AI_TENSOR_OBJ_DECLARE(
  node_562_output, AI_STATIC,
  143, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_562_output_array, NULL)

/* Tensor #144 */
AI_TENSOR_OBJ_DECLARE(
  node_562_weights, AI_STATIC,
  144, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 128), AI_STRIDE_INIT(4, 1, 128, 128, 128),
  1, &node_562_weights_array, NULL)

/* Tensor #145 */
AI_TENSOR_OBJ_DECLARE(
  node_565_bias, AI_STATIC,
  145, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_565_bias_array, NULL)

/* Tensor #146 */
AI_TENSOR_OBJ_DECLARE(
  node_565_output, AI_STATIC,
  146, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &node_565_output_array, NULL)

/* Tensor #147 */
AI_TENSOR_OBJ_DECLARE(
  node_565_scratch0, AI_STATIC,
  147, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_565_scratch0_array, NULL)

/* Tensor #148 */
AI_TENSOR_OBJ_DECLARE(
  node_565_weights, AI_STATIC,
  148, 0x0,
  AI_SHAPE_INIT(4, 128, 1, 1, 128), AI_STRIDE_INIT(4, 4, 512, 65536, 65536),
  1, &node_565_weights_array, NULL)

/* Tensor #149 */
AI_TENSOR_OBJ_DECLARE(
  node_568_bias, AI_STATIC,
  149, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_568_bias_array, NULL)

/* Tensor #150 */
AI_TENSOR_OBJ_DECLARE(
  node_568_output, AI_STATIC,
  150, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 28, 28), AI_STRIDE_INIT(4, 4, 4, 1024, 28672),
  1, &node_568_output_array, NULL)

/* Tensor #151 */
AI_TENSOR_OBJ_DECLARE(
  node_568_scratch0, AI_STATIC,
  151, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &node_568_scratch0_array, NULL)

/* Tensor #152 */
AI_TENSOR_OBJ_DECLARE(
  node_568_weights, AI_STATIC,
  152, 0x0,
  AI_SHAPE_INIT(4, 128, 1, 1, 256), AI_STRIDE_INIT(4, 4, 512, 131072, 131072),
  1, &node_568_weights_array, NULL)

/* Tensor #153 */
AI_TENSOR_OBJ_DECLARE(
  node_571_bias, AI_STATIC,
  153, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_571_bias_array, NULL)

/* Tensor #154 */
AI_TENSOR_OBJ_DECLARE(
  node_571_output, AI_STATIC,
  154, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_571_output_array, NULL)

/* Tensor #155 */
AI_TENSOR_OBJ_DECLARE(
  node_571_weights, AI_STATIC,
  155, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 256), AI_STRIDE_INIT(4, 1, 256, 256, 256),
  1, &node_571_weights_array, NULL)

/* Tensor #156 */
AI_TENSOR_OBJ_DECLARE(
  node_574_bias, AI_STATIC,
  156, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_574_bias_array, NULL)

/* Tensor #157 */
AI_TENSOR_OBJ_DECLARE(
  node_574_output, AI_STATIC,
  157, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_574_output_array, NULL)

/* Tensor #158 */
AI_TENSOR_OBJ_DECLARE(
  node_574_scratch0, AI_STATIC,
  158, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_574_scratch0_array, NULL)

/* Tensor #159 */
AI_TENSOR_OBJ_DECLARE(
  node_574_weights, AI_STATIC,
  159, 0x0,
  AI_SHAPE_INIT(4, 256, 1, 1, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 262144),
  1, &node_574_weights_array, NULL)

/* Tensor #160 */
AI_TENSOR_OBJ_DECLARE(
  node_577_bias, AI_STATIC,
  160, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_577_bias_array, NULL)

/* Tensor #161 */
AI_TENSOR_OBJ_DECLARE(
  node_577_output, AI_STATIC,
  161, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_577_output_array, NULL)

/* Tensor #162 */
AI_TENSOR_OBJ_DECLARE(
  node_577_scratch0, AI_STATIC,
  162, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_577_scratch0_array, NULL)

/* Tensor #163 */
AI_TENSOR_OBJ_DECLARE(
  node_577_weights, AI_STATIC,
  163, 0x0,
  AI_SHAPE_INIT(4, 256, 1, 1, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 262144),
  1, &node_577_weights_array, NULL)

/* Tensor #164 */
AI_TENSOR_OBJ_DECLARE(
  node_580_bias, AI_STATIC,
  164, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_580_bias_array, NULL)

/* Tensor #165 */
AI_TENSOR_OBJ_DECLARE(
  node_580_output, AI_STATIC,
  165, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_580_output_array, NULL)

/* Tensor #166 */
AI_TENSOR_OBJ_DECLARE(
  node_580_weights, AI_STATIC,
  166, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 256), AI_STRIDE_INIT(4, 1, 256, 256, 256),
  1, &node_580_weights_array, NULL)

/* Tensor #167 */
AI_TENSOR_OBJ_DECLARE(
  node_583_bias, AI_STATIC,
  167, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_583_bias_array, NULL)

/* Tensor #168 */
AI_TENSOR_OBJ_DECLARE(
  node_583_output, AI_STATIC,
  168, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_583_output_array, NULL)

/* Tensor #169 */
AI_TENSOR_OBJ_DECLARE(
  node_583_scratch0, AI_STATIC,
  169, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_583_scratch0_array, NULL)

/* Tensor #170 */
AI_TENSOR_OBJ_DECLARE(
  node_583_weights, AI_STATIC,
  170, 0x0,
  AI_SHAPE_INIT(4, 256, 1, 1, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 262144),
  1, &node_583_weights_array, NULL)

/* Tensor #171 */
AI_TENSOR_OBJ_DECLARE(
  node_586_bias, AI_STATIC,
  171, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_586_bias_array, NULL)

/* Tensor #172 */
AI_TENSOR_OBJ_DECLARE(
  node_586_output, AI_STATIC,
  172, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_586_output_array, NULL)

/* Tensor #173 */
AI_TENSOR_OBJ_DECLARE(
  node_586_scratch0, AI_STATIC,
  173, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_586_scratch0_array, NULL)

/* Tensor #174 */
AI_TENSOR_OBJ_DECLARE(
  node_586_weights, AI_STATIC,
  174, 0x0,
  AI_SHAPE_INIT(4, 256, 1, 1, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 262144),
  1, &node_586_weights_array, NULL)

/* Tensor #175 */
AI_TENSOR_OBJ_DECLARE(
  node_589_bias, AI_STATIC,
  175, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_589_bias_array, NULL)

/* Tensor #176 */
AI_TENSOR_OBJ_DECLARE(
  node_589_output, AI_STATIC,
  176, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_589_output_array, NULL)

/* Tensor #177 */
AI_TENSOR_OBJ_DECLARE(
  node_589_weights, AI_STATIC,
  177, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 256), AI_STRIDE_INIT(4, 1, 256, 256, 256),
  1, &node_589_weights_array, NULL)

/* Tensor #178 */
AI_TENSOR_OBJ_DECLARE(
  node_592_bias, AI_STATIC,
  178, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_592_bias_array, NULL)

/* Tensor #179 */
AI_TENSOR_OBJ_DECLARE(
  node_592_output, AI_STATIC,
  179, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_592_output_array, NULL)

/* Tensor #180 */
AI_TENSOR_OBJ_DECLARE(
  node_592_scratch0, AI_STATIC,
  180, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_592_scratch0_array, NULL)

/* Tensor #181 */
AI_TENSOR_OBJ_DECLARE(
  node_592_weights, AI_STATIC,
  181, 0x0,
  AI_SHAPE_INIT(4, 256, 1, 1, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 262144),
  1, &node_592_weights_array, NULL)

/* Tensor #182 */
AI_TENSOR_OBJ_DECLARE(
  node_595_bias, AI_STATIC,
  182, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_595_bias_array, NULL)

/* Tensor #183 */
AI_TENSOR_OBJ_DECLARE(
  node_595_output, AI_STATIC,
  183, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_595_output_array, NULL)

/* Tensor #184 */
AI_TENSOR_OBJ_DECLARE(
  node_595_scratch0, AI_STATIC,
  184, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_595_scratch0_array, NULL)

/* Tensor #185 */
AI_TENSOR_OBJ_DECLARE(
  node_595_weights, AI_STATIC,
  185, 0x0,
  AI_SHAPE_INIT(4, 256, 1, 1, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 262144),
  1, &node_595_weights_array, NULL)

/* Tensor #186 */
AI_TENSOR_OBJ_DECLARE(
  node_598_bias, AI_STATIC,
  186, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_598_bias_array, NULL)

/* Tensor #187 */
AI_TENSOR_OBJ_DECLARE(
  node_598_output, AI_STATIC,
  187, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_598_output_array, NULL)

/* Tensor #188 */
AI_TENSOR_OBJ_DECLARE(
  node_598_weights, AI_STATIC,
  188, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 256), AI_STRIDE_INIT(4, 1, 256, 256, 256),
  1, &node_598_weights_array, NULL)

/* Tensor #189 */
AI_TENSOR_OBJ_DECLARE(
  node_601_bias, AI_STATIC,
  189, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_601_bias_array, NULL)

/* Tensor #190 */
AI_TENSOR_OBJ_DECLARE(
  node_601_output, AI_STATIC,
  190, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_601_output_array, NULL)

/* Tensor #191 */
AI_TENSOR_OBJ_DECLARE(
  node_601_scratch0, AI_STATIC,
  191, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_601_scratch0_array, NULL)

/* Tensor #192 */
AI_TENSOR_OBJ_DECLARE(
  node_601_weights, AI_STATIC,
  192, 0x0,
  AI_SHAPE_INIT(4, 256, 1, 1, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 262144),
  1, &node_601_weights_array, NULL)

/* Tensor #193 */
AI_TENSOR_OBJ_DECLARE(
  node_604_bias, AI_STATIC,
  193, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_604_bias_array, NULL)

/* Tensor #194 */
AI_TENSOR_OBJ_DECLARE(
  node_604_output, AI_STATIC,
  194, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_604_output_array, NULL)

/* Tensor #195 */
AI_TENSOR_OBJ_DECLARE(
  node_604_scratch0, AI_STATIC,
  195, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_604_scratch0_array, NULL)

/* Tensor #196 */
AI_TENSOR_OBJ_DECLARE(
  node_604_weights, AI_STATIC,
  196, 0x0,
  AI_SHAPE_INIT(4, 256, 1, 1, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 262144),
  1, &node_604_weights_array, NULL)

/* Tensor #197 */
AI_TENSOR_OBJ_DECLARE(
  node_607_bias, AI_STATIC,
  197, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_607_bias_array, NULL)

/* Tensor #198 */
AI_TENSOR_OBJ_DECLARE(
  node_607_output, AI_STATIC,
  198, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_607_output_array, NULL)

/* Tensor #199 */
AI_TENSOR_OBJ_DECLARE(
  node_607_weights, AI_STATIC,
  199, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 256), AI_STRIDE_INIT(4, 1, 256, 256, 256),
  1, &node_607_weights_array, NULL)

/* Tensor #200 */
AI_TENSOR_OBJ_DECLARE(
  node_610_bias, AI_STATIC,
  200, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_610_bias_array, NULL)

/* Tensor #201 */
AI_TENSOR_OBJ_DECLARE(
  node_610_output, AI_STATIC,
  201, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_610_output_array, NULL)

/* Tensor #202 */
AI_TENSOR_OBJ_DECLARE(
  node_610_scratch0, AI_STATIC,
  202, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_610_scratch0_array, NULL)

/* Tensor #203 */
AI_TENSOR_OBJ_DECLARE(
  node_610_weights, AI_STATIC,
  203, 0x0,
  AI_SHAPE_INIT(4, 256, 1, 1, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 262144),
  1, &node_610_weights_array, NULL)

/* Tensor #204 */
AI_TENSOR_OBJ_DECLARE(
  node_613_bias, AI_STATIC,
  204, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_613_bias_array, NULL)

/* Tensor #205 */
AI_TENSOR_OBJ_DECLARE(
  node_613_output, AI_STATIC,
  205, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_613_output_array, NULL)

/* Tensor #206 */
AI_TENSOR_OBJ_DECLARE(
  node_613_scratch0, AI_STATIC,
  206, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_613_scratch0_array, NULL)

/* Tensor #207 */
AI_TENSOR_OBJ_DECLARE(
  node_613_weights, AI_STATIC,
  207, 0x0,
  AI_SHAPE_INIT(4, 256, 1, 1, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 262144),
  1, &node_613_weights_array, NULL)

/* Tensor #208 */
AI_TENSOR_OBJ_DECLARE(
  node_616_bias, AI_STATIC,
  208, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_616_bias_array, NULL)

/* Tensor #209 */
AI_TENSOR_OBJ_DECLARE(
  node_616_output, AI_STATIC,
  209, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_616_output_array, NULL)

/* Tensor #210 */
AI_TENSOR_OBJ_DECLARE(
  node_616_weights, AI_STATIC,
  210, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 256), AI_STRIDE_INIT(4, 1, 256, 256, 256),
  1, &node_616_weights_array, NULL)

/* Tensor #211 */
AI_TENSOR_OBJ_DECLARE(
  node_619_bias, AI_STATIC,
  211, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_619_bias_array, NULL)

/* Tensor #212 */
AI_TENSOR_OBJ_DECLARE(
  node_619_output, AI_STATIC,
  212, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_619_output_array, NULL)

/* Tensor #213 */
AI_TENSOR_OBJ_DECLARE(
  node_619_scratch0, AI_STATIC,
  213, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_619_scratch0_array, NULL)

/* Tensor #214 */
AI_TENSOR_OBJ_DECLARE(
  node_619_weights, AI_STATIC,
  214, 0x0,
  AI_SHAPE_INIT(4, 256, 1, 1, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 262144),
  1, &node_619_weights_array, NULL)

/* Tensor #215 */
AI_TENSOR_OBJ_DECLARE(
  node_622_bias, AI_STATIC,
  215, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_622_bias_array, NULL)

/* Tensor #216 */
AI_TENSOR_OBJ_DECLARE(
  node_622_output, AI_STATIC,
  216, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_622_output_array, NULL)

/* Tensor #217 */
AI_TENSOR_OBJ_DECLARE(
  node_622_scratch0, AI_STATIC,
  217, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_622_scratch0_array, NULL)

/* Tensor #218 */
AI_TENSOR_OBJ_DECLARE(
  node_622_weights, AI_STATIC,
  218, 0x0,
  AI_SHAPE_INIT(4, 256, 1, 1, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 262144),
  1, &node_622_weights_array, NULL)

/* Tensor #219 */
AI_TENSOR_OBJ_DECLARE(
  node_625_bias, AI_STATIC,
  219, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_625_bias_array, NULL)

/* Tensor #220 */
AI_TENSOR_OBJ_DECLARE(
  node_625_output, AI_STATIC,
  220, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_625_output_array, NULL)

/* Tensor #221 */
AI_TENSOR_OBJ_DECLARE(
  node_625_weights, AI_STATIC,
  221, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 256), AI_STRIDE_INIT(4, 1, 256, 256, 256),
  1, &node_625_weights_array, NULL)

/* Tensor #222 */
AI_TENSOR_OBJ_DECLARE(
  node_628_bias, AI_STATIC,
  222, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_628_bias_array, NULL)

/* Tensor #223 */
AI_TENSOR_OBJ_DECLARE(
  node_628_output, AI_STATIC,
  223, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &node_628_output_array, NULL)

/* Tensor #224 */
AI_TENSOR_OBJ_DECLARE(
  node_628_scratch0, AI_STATIC,
  224, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_628_scratch0_array, NULL)

/* Tensor #225 */
AI_TENSOR_OBJ_DECLARE(
  node_628_weights, AI_STATIC,
  225, 0x0,
  AI_SHAPE_INIT(4, 256, 1, 1, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 262144),
  1, &node_628_weights_array, NULL)

/* Tensor #226 */
AI_TENSOR_OBJ_DECLARE(
  node_631_bias, AI_STATIC,
  226, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &node_631_bias_array, NULL)

/* Tensor #227 */
AI_TENSOR_OBJ_DECLARE(
  node_631_output, AI_STATIC,
  227, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &node_631_output_array, NULL)

/* Tensor #228 */
AI_TENSOR_OBJ_DECLARE(
  node_631_scratch0, AI_STATIC,
  228, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_631_scratch0_array, NULL)

/* Tensor #229 */
AI_TENSOR_OBJ_DECLARE(
  node_631_weights, AI_STATIC,
  229, 0x0,
  AI_SHAPE_INIT(4, 256, 1, 1, 512), AI_STRIDE_INIT(4, 4, 1024, 524288, 524288),
  1, &node_631_weights_array, NULL)

/* Tensor #230 */
AI_TENSOR_OBJ_DECLARE(
  node_634_bias, AI_STATIC,
  230, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &node_634_bias_array, NULL)

/* Tensor #231 */
AI_TENSOR_OBJ_DECLARE(
  node_634_output, AI_STATIC,
  231, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &node_634_output_array, NULL)

/* Tensor #232 */
AI_TENSOR_OBJ_DECLARE(
  node_634_weights, AI_STATIC,
  232, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 512), AI_STRIDE_INIT(4, 1, 512, 512, 512),
  1, &node_634_weights_array, NULL)

/* Tensor #233 */
AI_TENSOR_OBJ_DECLARE(
  node_637_bias, AI_STATIC,
  233, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_637_bias_array, NULL)

/* Tensor #234 */
AI_TENSOR_OBJ_DECLARE(
  node_637_output, AI_STATIC,
  234, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 7, 7), AI_STRIDE_INIT(4, 4, 4, 1024, 7168),
  1, &node_637_output_array, NULL)

/* Tensor #235 */
AI_TENSOR_OBJ_DECLARE(
  node_637_scratch0, AI_STATIC,
  235, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &node_637_scratch0_array, NULL)

/* Tensor #236 */
AI_TENSOR_OBJ_DECLARE(
  node_637_weights, AI_STATIC,
  236, 0x0,
  AI_SHAPE_INIT(4, 512, 1, 1, 256), AI_STRIDE_INIT(4, 4, 2048, 524288, 524288),
  1, &node_637_weights_array, NULL)

/* Tensor #237 */
AI_TENSOR_OBJ_DECLARE(
  node_640_bias, AI_STATIC,
  237, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_640_bias_array, NULL)

/* Tensor #238 */
AI_TENSOR_OBJ_DECLARE(
  node_640_output, AI_STATIC,
  238, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 7, 7), AI_STRIDE_INIT(4, 4, 4, 1024, 7168),
  1, &node_640_output_array, NULL)

/* Tensor #239 */
AI_TENSOR_OBJ_DECLARE(
  node_640_scratch0, AI_STATIC,
  239, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_640_scratch0_array, NULL)

/* Tensor #240 */
AI_TENSOR_OBJ_DECLARE(
  node_640_weights, AI_STATIC,
  240, 0x0,
  AI_SHAPE_INIT(4, 256, 1, 1, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 262144),
  1, &node_640_weights_array, NULL)

/* Tensor #241 */
AI_TENSOR_OBJ_DECLARE(
  node_643_bias, AI_STATIC,
  241, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_643_bias_array, NULL)

/* Tensor #242 */
AI_TENSOR_OBJ_DECLARE(
  node_643_output, AI_STATIC,
  242, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 7, 7), AI_STRIDE_INIT(4, 4, 4, 1024, 7168),
  1, &node_643_output_array, NULL)

/* Tensor #243 */
AI_TENSOR_OBJ_DECLARE(
  node_643_weights, AI_STATIC,
  243, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 256), AI_STRIDE_INIT(4, 1, 256, 256, 256),
  1, &node_643_weights_array, NULL)

/* Tensor #244 */
AI_TENSOR_OBJ_DECLARE(
  node_646_bias, AI_STATIC,
  244, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_646_bias_array, NULL)

/* Tensor #245 */
AI_TENSOR_OBJ_DECLARE(
  node_646_output, AI_STATIC,
  245, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 7, 7), AI_STRIDE_INIT(4, 4, 4, 1024, 7168),
  1, &node_646_output_array, NULL)

/* Tensor #246 */
AI_TENSOR_OBJ_DECLARE(
  node_646_scratch0, AI_STATIC,
  246, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_646_scratch0_array, NULL)

/* Tensor #247 */
AI_TENSOR_OBJ_DECLARE(
  node_646_weights, AI_STATIC,
  247, 0x0,
  AI_SHAPE_INIT(4, 256, 1, 1, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 262144),
  1, &node_646_weights_array, NULL)

/* Tensor #248 */
AI_TENSOR_OBJ_DECLARE(
  node_649_bias, AI_STATIC,
  248, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_649_bias_array, NULL)

/* Tensor #249 */
AI_TENSOR_OBJ_DECLARE(
  node_649_output, AI_STATIC,
  249, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 7, 7), AI_STRIDE_INIT(4, 4, 4, 1024, 7168),
  1, &node_649_output_array, NULL)

/* Tensor #250 */
AI_TENSOR_OBJ_DECLARE(
  node_649_scratch0, AI_STATIC,
  250, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_649_scratch0_array, NULL)

/* Tensor #251 */
AI_TENSOR_OBJ_DECLARE(
  node_649_weights, AI_STATIC,
  251, 0x0,
  AI_SHAPE_INIT(4, 256, 1, 1, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 262144),
  1, &node_649_weights_array, NULL)

/* Tensor #252 */
AI_TENSOR_OBJ_DECLARE(
  node_652_bias, AI_STATIC,
  252, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_652_bias_array, NULL)

/* Tensor #253 */
AI_TENSOR_OBJ_DECLARE(
  node_652_output, AI_STATIC,
  253, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 7, 7), AI_STRIDE_INIT(4, 4, 4, 1024, 7168),
  1, &node_652_output_array, NULL)

/* Tensor #254 */
AI_TENSOR_OBJ_DECLARE(
  node_652_weights, AI_STATIC,
  254, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 256), AI_STRIDE_INIT(4, 1, 256, 256, 256),
  1, &node_652_weights_array, NULL)

/* Tensor #255 */
AI_TENSOR_OBJ_DECLARE(
  node_655_bias, AI_STATIC,
  255, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_655_bias_array, NULL)

/* Tensor #256 */
AI_TENSOR_OBJ_DECLARE(
  node_655_output, AI_STATIC,
  256, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 7, 7), AI_STRIDE_INIT(4, 4, 4, 1024, 7168),
  1, &node_655_output_array, NULL)

/* Tensor #257 */
AI_TENSOR_OBJ_DECLARE(
  node_655_scratch0, AI_STATIC,
  257, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_655_scratch0_array, NULL)

/* Tensor #258 */
AI_TENSOR_OBJ_DECLARE(
  node_655_weights, AI_STATIC,
  258, 0x0,
  AI_SHAPE_INIT(4, 256, 1, 1, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 262144),
  1, &node_655_weights_array, NULL)

/* Tensor #259 */
AI_TENSOR_OBJ_DECLARE(
  node_658_bias, AI_STATIC,
  259, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &node_658_bias_array, NULL)

/* Tensor #260 */
AI_TENSOR_OBJ_DECLARE(
  node_658_output, AI_STATIC,
  260, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &node_658_output_array, NULL)

/* Tensor #261 */
AI_TENSOR_OBJ_DECLARE(
  node_658_scratch0, AI_STATIC,
  261, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &node_658_scratch0_array, NULL)

/* Tensor #262 */
AI_TENSOR_OBJ_DECLARE(
  node_658_weights, AI_STATIC,
  262, 0x0,
  AI_SHAPE_INIT(4, 256, 1, 1, 512), AI_STRIDE_INIT(4, 4, 1024, 524288, 524288),
  1, &node_658_weights_array, NULL)

/* Tensor #263 */
AI_TENSOR_OBJ_DECLARE(
  node_661_bias, AI_STATIC,
  263, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &node_661_bias_array, NULL)

/* Tensor #264 */
AI_TENSOR_OBJ_DECLARE(
  node_661_output, AI_STATIC,
  264, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 7, 7), AI_STRIDE_INIT(4, 4, 4, 256, 1792),
  1, &node_661_output_array, NULL)

/* Tensor #265 */
AI_TENSOR_OBJ_DECLARE(
  node_661_scratch0, AI_STATIC,
  265, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &node_661_scratch0_array, NULL)

/* Tensor #266 */
AI_TENSOR_OBJ_DECLARE(
  node_661_weights, AI_STATIC,
  266, 0x0,
  AI_SHAPE_INIT(4, 512, 1, 1, 64), AI_STRIDE_INIT(4, 4, 2048, 131072, 131072),
  1, &node_661_weights_array, NULL)



/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_515_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_514_to_chlast_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_515_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_515_weights, &node_515_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_515_layer, 98,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense,
  &node_515_chain,
  NULL, &node_515_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_514_to_chlast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_513_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_514_to_chlast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_514_to_chlast_layer, 96,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &node_514_to_chlast_chain,
  NULL, &node_515_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_HEIGHT, AI_SHAPE_CHANNEL, AI_SHAPE_DEPTH, AI_SHAPE_EXTENSION), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_513_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_661_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_513_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_513_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_513_layer, 95,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_513_chain,
  NULL, &node_514_to_chlast_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_661_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_509_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_661_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_661_weights, &node_661_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_661_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_661_layer, 94,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_661_chain,
  NULL, &node_513_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_509_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_658_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_509_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_509_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_509_layer, 93,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_509_chain,
  NULL, &node_661_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_658_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_505_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_658_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_658_weights, &node_658_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_658_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_658_layer, 92,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_658_chain,
  NULL, &node_509_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_505_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_494_output, &node_655_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_505_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_505_layer, 91,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &node_505_chain,
  NULL, &node_658_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_655_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_502_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_655_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_655_weights, &node_655_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_655_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_655_layer, 90,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_655_chain,
  NULL, &node_505_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_502_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_652_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_502_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_502_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_502_layer, 89,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_502_chain,
  NULL, &node_655_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_652_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_498_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_652_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_652_weights, &node_652_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_652_layer, 88,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_652_chain,
  NULL, &node_502_layer, AI_STATIC, 
  .groups = 256, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_498_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_649_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_498_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_498_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_498_layer, 87,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_498_chain,
  NULL, &node_652_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_649_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_494_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_649_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_649_weights, &node_649_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_649_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_649_layer, 86,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_649_chain,
  NULL, &node_498_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_494_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_637_output, &node_646_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_494_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_494_layer, 85,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &node_494_chain,
  NULL, &node_649_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_646_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_491_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_646_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_646_weights, &node_646_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_646_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_646_layer, 84,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_646_chain,
  NULL, &node_494_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_491_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_643_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_491_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_491_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_491_layer, 83,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_491_chain,
  NULL, &node_646_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_643_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_487_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_643_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_643_weights, &node_643_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_643_layer, 82,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_643_chain,
  NULL, &node_491_layer, AI_STATIC, 
  .groups = 256, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_487_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_640_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_487_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_487_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_487_layer, 81,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_487_chain,
  NULL, &node_643_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_640_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_637_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_640_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_640_weights, &node_640_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_640_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_640_layer, 80,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_640_chain,
  NULL, &node_487_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_637_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_481_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_637_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_637_weights, &node_637_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_637_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_637_layer, 79,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_637_chain,
  NULL, &node_640_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_481_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_634_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_481_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_481_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_481_layer, 78,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_481_chain,
  NULL, &node_637_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_634_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_477_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_634_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_634_weights, &node_634_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_634_layer, 77,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_634_chain,
  NULL, &node_481_layer, AI_STATIC, 
  .groups = 512, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_477_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_631_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_477_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_477_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_477_layer, 76,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_477_chain,
  NULL, &node_634_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_631_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_473_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_631_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_631_weights, &node_631_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_631_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_631_layer, 75,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_631_chain,
  NULL, &node_477_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_473_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_462_output, &node_628_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_473_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_473_layer, 74,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &node_473_chain,
  NULL, &node_631_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_628_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_470_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_628_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_628_weights, &node_628_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_628_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_628_layer, 73,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_628_chain,
  NULL, &node_473_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_470_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_625_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_470_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_470_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_470_layer, 72,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_470_chain,
  NULL, &node_628_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_625_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_466_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_625_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_625_weights, &node_625_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_625_layer, 71,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_625_chain,
  NULL, &node_470_layer, AI_STATIC, 
  .groups = 256, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_466_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_622_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_466_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_466_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_466_layer, 70,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_466_chain,
  NULL, &node_625_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_622_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_462_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_622_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_622_weights, &node_622_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_622_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_622_layer, 69,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_622_chain,
  NULL, &node_466_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_462_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_451_output, &node_619_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_462_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_462_layer, 68,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &node_462_chain,
  NULL, &node_622_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_619_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_459_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_619_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_619_weights, &node_619_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_619_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_619_layer, 67,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_619_chain,
  NULL, &node_462_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_459_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_616_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_459_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_459_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_459_layer, 66,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_459_chain,
  NULL, &node_619_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_616_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_455_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_616_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_616_weights, &node_616_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_616_layer, 65,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_616_chain,
  NULL, &node_459_layer, AI_STATIC, 
  .groups = 256, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_455_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_613_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_455_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_455_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_455_layer, 64,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_455_chain,
  NULL, &node_616_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_613_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_451_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_613_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_613_weights, &node_613_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_613_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_613_layer, 63,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_613_chain,
  NULL, &node_455_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_451_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_440_output, &node_610_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_451_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_451_layer, 62,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &node_451_chain,
  NULL, &node_613_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_610_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_448_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_610_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_610_weights, &node_610_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_610_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_610_layer, 61,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_610_chain,
  NULL, &node_451_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_448_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_607_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_448_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_448_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_448_layer, 60,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_448_chain,
  NULL, &node_610_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_607_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_444_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_607_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_607_weights, &node_607_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_607_layer, 59,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_607_chain,
  NULL, &node_448_layer, AI_STATIC, 
  .groups = 256, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_444_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_604_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_444_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_444_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_444_layer, 58,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_444_chain,
  NULL, &node_607_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_604_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_440_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_604_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_604_weights, &node_604_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_604_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_604_layer, 57,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_604_chain,
  NULL, &node_444_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_440_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_429_output, &node_601_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_440_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_440_layer, 56,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &node_440_chain,
  NULL, &node_604_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_601_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_437_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_601_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_601_weights, &node_601_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_601_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_601_layer, 55,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_601_chain,
  NULL, &node_440_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_437_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_598_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_437_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_437_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_437_layer, 54,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_437_chain,
  NULL, &node_601_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_598_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_433_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_598_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_598_weights, &node_598_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_598_layer, 53,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_598_chain,
  NULL, &node_437_layer, AI_STATIC, 
  .groups = 256, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_433_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_595_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_433_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_433_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_433_layer, 52,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_433_chain,
  NULL, &node_598_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_595_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_429_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_595_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_595_weights, &node_595_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_595_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_595_layer, 51,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_595_chain,
  NULL, &node_433_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_429_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_418_output, &node_592_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_429_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_429_layer, 50,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &node_429_chain,
  NULL, &node_595_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_592_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_426_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_592_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_592_weights, &node_592_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_592_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_592_layer, 49,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_592_chain,
  NULL, &node_429_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_426_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_589_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_426_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_426_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_426_layer, 48,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_426_chain,
  NULL, &node_592_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_589_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_422_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_589_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_589_weights, &node_589_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_589_layer, 47,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_589_chain,
  NULL, &node_426_layer, AI_STATIC, 
  .groups = 256, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_422_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_586_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_422_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_422_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_422_layer, 46,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_422_chain,
  NULL, &node_589_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_586_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_418_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_586_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_586_weights, &node_586_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_586_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_586_layer, 45,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_586_chain,
  NULL, &node_422_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_418_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_574_output, &node_583_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_418_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_418_layer, 44,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &node_418_chain,
  NULL, &node_586_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_583_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_415_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_583_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_583_weights, &node_583_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_583_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_583_layer, 43,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_583_chain,
  NULL, &node_418_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_415_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_580_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_415_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_415_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_415_layer, 42,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_415_chain,
  NULL, &node_583_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_580_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_411_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_580_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_580_weights, &node_580_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_580_layer, 41,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_580_chain,
  NULL, &node_415_layer, AI_STATIC, 
  .groups = 256, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_411_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_577_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_411_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_411_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_411_layer, 40,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_411_chain,
  NULL, &node_580_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_577_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_574_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_577_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_577_weights, &node_577_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_577_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_577_layer, 39,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_577_chain,
  NULL, &node_411_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_574_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_405_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_574_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_574_weights, &node_574_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_574_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_574_layer, 38,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_574_chain,
  NULL, &node_577_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_405_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_571_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_405_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_405_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_405_layer, 37,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_405_chain,
  NULL, &node_574_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_571_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_401_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_571_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_571_weights, &node_571_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_571_layer, 36,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_571_chain,
  NULL, &node_405_layer, AI_STATIC, 
  .groups = 256, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_401_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_568_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_401_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_401_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_401_layer, 35,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_401_chain,
  NULL, &node_571_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_568_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_397_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_568_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_568_weights, &node_568_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_568_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_568_layer, 34,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_568_chain,
  NULL, &node_401_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_397_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_386_output, &node_565_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_397_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_397_layer, 33,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &node_397_chain,
  NULL, &node_568_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_565_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_394_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_565_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_565_weights, &node_565_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_565_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_565_layer, 32,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_565_chain,
  NULL, &node_397_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_394_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_562_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_394_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_394_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_394_layer, 31,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_394_chain,
  NULL, &node_565_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_562_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_390_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_562_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_562_weights, &node_562_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_562_layer, 30,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_562_chain,
  NULL, &node_394_layer, AI_STATIC, 
  .groups = 128, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_390_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_559_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_390_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_390_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_390_layer, 29,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_390_chain,
  NULL, &node_562_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_559_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_386_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_559_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_559_weights, &node_559_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_559_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_559_layer, 28,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_559_chain,
  NULL, &node_390_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_386_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_375_output, &node_556_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_386_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_386_layer, 27,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &node_386_chain,
  NULL, &node_559_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_556_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_383_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_556_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_556_weights, &node_556_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_556_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_556_layer, 26,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_556_chain,
  NULL, &node_386_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_383_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_553_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_383_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_383_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_383_layer, 25,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_383_chain,
  NULL, &node_556_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_553_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_379_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_553_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_553_weights, &node_553_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_553_layer, 24,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_553_chain,
  NULL, &node_383_layer, AI_STATIC, 
  .groups = 128, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_379_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_550_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_379_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_379_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_379_layer, 23,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_379_chain,
  NULL, &node_553_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_550_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_375_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_550_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_550_weights, &node_550_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_550_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_550_layer, 22,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_550_chain,
  NULL, &node_379_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_375_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_364_output, &node_547_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_375_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_375_layer, 21,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &node_375_chain,
  NULL, &node_550_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_547_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_372_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_547_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_547_weights, &node_547_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_547_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_547_layer, 20,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_547_chain,
  NULL, &node_375_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_372_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_544_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_372_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_372_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_372_layer, 19,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_372_chain,
  NULL, &node_547_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_544_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_368_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_544_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_544_weights, &node_544_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_544_layer, 18,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_544_chain,
  NULL, &node_372_layer, AI_STATIC, 
  .groups = 128, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_368_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_541_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_368_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_368_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_368_layer, 17,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_368_chain,
  NULL, &node_544_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_541_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_364_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_541_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_541_weights, &node_541_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_541_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_541_layer, 16,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_541_chain,
  NULL, &node_368_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_364_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_529_output, &node_538_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_364_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_364_layer, 15,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &node_364_chain,
  NULL, &node_541_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_538_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_361_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_538_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_538_weights, &node_538_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_538_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_538_layer, 14,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_538_chain,
  NULL, &node_364_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_361_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_535_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_361_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_361_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_361_layer, 13,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_361_chain,
  NULL, &node_538_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_535_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_357_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_535_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_535_weights, &node_535_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_535_layer, 12,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_535_chain,
  NULL, &node_361_layer, AI_STATIC, 
  .groups = 128, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_357_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_532_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_357_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_357_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_357_layer, 11,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_357_chain,
  NULL, &node_535_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_532_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_529_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_532_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_532_weights, &node_532_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_532_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_532_layer, 10,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_532_chain,
  NULL, &node_357_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_529_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_351_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_529_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_529_weights, &node_529_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_529_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_529_layer, 9,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_529_chain,
  NULL, &node_532_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_351_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_526_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_351_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_351_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_351_layer, 8,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_351_chain,
  NULL, &node_529_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_526_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_347_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_526_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_526_weights, &node_526_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_526_layer, 7,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_526_chain,
  NULL, &node_351_layer, AI_STATIC, 
  .groups = 128, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_347_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_523_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_347_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_347_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_347_layer, 6,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_347_chain,
  NULL, &node_526_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_523_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_343_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_523_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_523_weights, &node_523_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_523_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_523_layer, 5,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_523_chain,
  NULL, &node_347_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_343_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_520_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_343_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_343_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_343_layer, 4,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_343_chain,
  NULL, &node_523_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_520_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_339_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_520_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_520_weights, &node_520_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_520_layer, 3,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32_group,
  &node_520_chain,
  NULL, &node_343_layer, AI_STATIC, 
  .groups = 64, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_339_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_517_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_339_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_339_alpha),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_339_layer, 2,
  NL_TYPE, 0x0, NULL,
  nl, forward_prelu,
  &node_339_chain,
  NULL, &node_520_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_517_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &input_1_Transpose_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_517_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_517_weights, &node_517_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_517_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_517_layer, 1,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_517_chain,
  NULL, &node_339_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  input_1_Transpose_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &input_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &input_1_Transpose_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  input_1_Transpose_layer, 2,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &input_1_Transpose_chain,
  NULL, &node_517_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_HEIGHT, AI_SHAPE_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_DEPTH, AI_SHAPE_EXTENSION), 
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 13593736, 1, 1),
    13593736, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 3211776, 1, 1),
    3211776, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_FACEID_IN_NUM, &input_1_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_FACEID_OUT_NUM, &node_515_output),
  &input_1_Transpose_layer, 0x38d47702, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 13593736, 1, 1),
      13593736, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 3211776, 1, 1),
      3211776, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_FACEID_IN_NUM, &input_1_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_FACEID_OUT_NUM, &node_515_output),
  &input_1_Transpose_layer, 0x38d47702, NULL)

#endif	/*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/



/******************************************************************************/
AI_DECLARE_STATIC
ai_bool faceid_configure_activations(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_activations_map(g_faceid_activations_map, 1, params)) {
    /* Updating activations (byte) offsets */
    
    input_1_output_array.data = AI_PTR(g_faceid_activations_map[0] + 1455616);
    input_1_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 1455616);
    input_1_Transpose_output_array.data = AI_PTR(g_faceid_activations_map[0] + 1606144);
    input_1_Transpose_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 1606144);
    node_517_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 1756672);
    node_517_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 1756672);
    node_517_output_array.data = AI_PTR(g_faceid_activations_map[0] + 512);
    node_517_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 512);
    node_339_output_array.data = AI_PTR(g_faceid_activations_map[0] + 512);
    node_339_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 512);
    node_520_output_array.data = AI_PTR(g_faceid_activations_map[0] + 1606144);
    node_520_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 1606144);
    node_343_output_array.data = AI_PTR(g_faceid_activations_map[0] + 1606144);
    node_343_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 1606144);
    node_523_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 1605632);
    node_523_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 1605632);
    node_523_output_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_523_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_347_output_array.data = AI_PTR(g_faceid_activations_map[0] + 1605632);
    node_347_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 1605632);
    node_526_output_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_526_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_351_output_array.data = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_351_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_529_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_529_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_529_output_array.data = AI_PTR(g_faceid_activations_map[0] + 802816);
    node_529_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 802816);
    node_532_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_532_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_532_output_array.data = AI_PTR(g_faceid_activations_map[0] + 512);
    node_532_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 512);
    node_357_output_array.data = AI_PTR(g_faceid_activations_map[0] + 1204224);
    node_357_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 1204224);
    node_535_output_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_535_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_361_output_array.data = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_361_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_538_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_538_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_538_output_array.data = AI_PTR(g_faceid_activations_map[0] + 1204224);
    node_538_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 1204224);
    node_364_output_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_364_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_541_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_541_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_541_output_array.data = AI_PTR(g_faceid_activations_map[0] + 401920);
    node_541_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 401920);
    node_368_output_array.data = AI_PTR(g_faceid_activations_map[0] + 803328);
    node_368_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 803328);
    node_544_output_array.data = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_544_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_372_output_array.data = AI_PTR(g_faceid_activations_map[0] + 802816);
    node_372_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 802816);
    node_547_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_547_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_547_output_array.data = AI_PTR(g_faceid_activations_map[0] + 1204224);
    node_547_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 1204224);
    node_375_output_array.data = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_375_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_550_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_550_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_550_output_array.data = AI_PTR(g_faceid_activations_map[0] + 802816);
    node_550_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 802816);
    node_379_output_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_379_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_553_output_array.data = AI_PTR(g_faceid_activations_map[0] + 802816);
    node_553_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 802816);
    node_383_output_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_383_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_556_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 802816);
    node_556_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 802816);
    node_556_output_array.data = AI_PTR(g_faceid_activations_map[0] + 803328);
    node_556_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 803328);
    node_386_output_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_386_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_559_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_559_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_559_output_array.data = AI_PTR(g_faceid_activations_map[0] + 401920);
    node_559_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 401920);
    node_390_output_array.data = AI_PTR(g_faceid_activations_map[0] + 803328);
    node_390_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 803328);
    node_562_output_array.data = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_562_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_394_output_array.data = AI_PTR(g_faceid_activations_map[0] + 802816);
    node_394_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 802816);
    node_565_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_565_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_565_output_array.data = AI_PTR(g_faceid_activations_map[0] + 1204224);
    node_565_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 1204224);
    node_397_output_array.data = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_397_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_568_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_568_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_568_output_array.data = AI_PTR(g_faceid_activations_map[0] + 802816);
    node_568_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 802816);
    node_401_output_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_401_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_571_output_array.data = AI_PTR(g_faceid_activations_map[0] + 802816);
    node_571_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 802816);
    node_405_output_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_405_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_574_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 200704);
    node_574_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 200704);
    node_574_output_array.data = AI_PTR(g_faceid_activations_map[0] + 201728);
    node_574_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 201728);
    node_577_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_577_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_577_output_array.data = AI_PTR(g_faceid_activations_map[0] + 1024);
    node_577_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 1024);
    node_411_output_array.data = AI_PTR(g_faceid_activations_map[0] + 402432);
    node_411_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 402432);
    node_580_output_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_580_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_415_output_array.data = AI_PTR(g_faceid_activations_map[0] + 402432);
    node_415_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 402432);
    node_583_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_583_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_583_output_array.data = AI_PTR(g_faceid_activations_map[0] + 1024);
    node_583_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 1024);
    node_418_output_array.data = AI_PTR(g_faceid_activations_map[0] + 402432);
    node_418_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 402432);
    node_586_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_586_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_586_output_array.data = AI_PTR(g_faceid_activations_map[0] + 1024);
    node_586_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 1024);
    node_422_output_array.data = AI_PTR(g_faceid_activations_map[0] + 201728);
    node_422_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 201728);
    node_589_output_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_589_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_426_output_array.data = AI_PTR(g_faceid_activations_map[0] + 200704);
    node_426_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 200704);
    node_592_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_592_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_592_output_array.data = AI_PTR(g_faceid_activations_map[0] + 603136);
    node_592_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 603136);
    node_429_output_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_429_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_595_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 200704);
    node_595_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 200704);
    node_595_output_array.data = AI_PTR(g_faceid_activations_map[0] + 201728);
    node_595_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 201728);
    node_433_output_array.data = AI_PTR(g_faceid_activations_map[0] + 402432);
    node_433_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 402432);
    node_598_output_array.data = AI_PTR(g_faceid_activations_map[0] + 200704);
    node_598_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 200704);
    node_437_output_array.data = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_437_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_601_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 200704);
    node_601_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 200704);
    node_601_output_array.data = AI_PTR(g_faceid_activations_map[0] + 602112);
    node_601_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 602112);
    node_440_output_array.data = AI_PTR(g_faceid_activations_map[0] + 200704);
    node_440_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 200704);
    node_604_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_604_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_604_output_array.data = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_604_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_444_output_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_444_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_607_output_array.data = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_607_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_448_output_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_448_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_610_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_610_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_610_output_array.data = AI_PTR(g_faceid_activations_map[0] + 402432);
    node_610_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 402432);
    node_451_output_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_451_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_613_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 200704);
    node_613_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 200704);
    node_613_output_array.data = AI_PTR(g_faceid_activations_map[0] + 201728);
    node_613_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 201728);
    node_455_output_array.data = AI_PTR(g_faceid_activations_map[0] + 402432);
    node_455_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 402432);
    node_616_output_array.data = AI_PTR(g_faceid_activations_map[0] + 200704);
    node_616_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 200704);
    node_459_output_array.data = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_459_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_619_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 200704);
    node_619_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 200704);
    node_619_output_array.data = AI_PTR(g_faceid_activations_map[0] + 602112);
    node_619_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 602112);
    node_462_output_array.data = AI_PTR(g_faceid_activations_map[0] + 200704);
    node_462_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 200704);
    node_622_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_622_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_622_output_array.data = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_622_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_466_output_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_466_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_625_output_array.data = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_625_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_470_output_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_470_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_628_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_628_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 401408);
    node_628_output_array.data = AI_PTR(g_faceid_activations_map[0] + 402432);
    node_628_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 402432);
    node_473_output_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_473_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_631_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 200704);
    node_631_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 200704);
    node_631_output_array.data = AI_PTR(g_faceid_activations_map[0] + 201728);
    node_631_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 201728);
    node_477_output_array.data = AI_PTR(g_faceid_activations_map[0] + 603136);
    node_477_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 603136);
    node_634_output_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_634_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_481_output_array.data = AI_PTR(g_faceid_activations_map[0] + 100352);
    node_481_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 100352);
    node_637_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_637_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_637_output_array.data = AI_PTR(g_faceid_activations_map[0] + 2048);
    node_637_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 2048);
    node_640_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_640_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_640_output_array.data = AI_PTR(g_faceid_activations_map[0] + 52224);
    node_640_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 52224);
    node_487_output_array.data = AI_PTR(g_faceid_activations_map[0] + 102400);
    node_487_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 102400);
    node_643_output_array.data = AI_PTR(g_faceid_activations_map[0] + 52224);
    node_643_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 52224);
    node_491_output_array.data = AI_PTR(g_faceid_activations_map[0] + 102400);
    node_491_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 102400);
    node_646_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_646_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_646_output_array.data = AI_PTR(g_faceid_activations_map[0] + 52224);
    node_646_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 52224);
    node_494_output_array.data = AI_PTR(g_faceid_activations_map[0] + 102400);
    node_494_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 102400);
    node_649_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_649_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_649_output_array.data = AI_PTR(g_faceid_activations_map[0] + 1024);
    node_649_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 1024);
    node_498_output_array.data = AI_PTR(g_faceid_activations_map[0] + 51200);
    node_498_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 51200);
    node_652_output_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_652_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_502_output_array.data = AI_PTR(g_faceid_activations_map[0] + 50176);
    node_502_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 50176);
    node_655_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_655_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_655_output_array.data = AI_PTR(g_faceid_activations_map[0] + 152576);
    node_655_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 152576);
    node_505_output_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_505_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_658_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 50176);
    node_658_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 50176);
    node_658_output_array.data = AI_PTR(g_faceid_activations_map[0] + 51200);
    node_658_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 51200);
    node_509_output_array.data = AI_PTR(g_faceid_activations_map[0] + 151552);
    node_509_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 151552);
    node_661_scratch0_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_661_scratch0_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_661_output_array.data = AI_PTR(g_faceid_activations_map[0] + 2048);
    node_661_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 2048);
    node_513_output_array.data = AI_PTR(g_faceid_activations_map[0] + 14592);
    node_513_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 14592);
    node_514_to_chlast_output_array.data = AI_PTR(g_faceid_activations_map[0] + 0);
    node_514_to_chlast_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 0);
    node_515_output_array.data = AI_PTR(g_faceid_activations_map[0] + 12544);
    node_515_output_array.data_start = AI_PTR(g_faceid_activations_map[0] + 12544);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_ACTIVATIONS);
  return false;
}




/******************************************************************************/
AI_DECLARE_STATIC
ai_bool faceid_configure_weights(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_weights_map(g_faceid_weights_map, 1, params)) {
    /* Updating weights (byte) offsets */
    
    node_517_weights_array.format |= AI_FMT_FLAG_CONST;
    node_517_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 0);
    node_517_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 0);
    node_517_bias_array.format |= AI_FMT_FLAG_CONST;
    node_517_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 13824);
    node_517_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 13824);
    node_339_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_339_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 14336);
    node_339_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 14336);
    node_520_weights_array.format |= AI_FMT_FLAG_CONST;
    node_520_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 14852);
    node_520_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 14852);
    node_520_bias_array.format |= AI_FMT_FLAG_CONST;
    node_520_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 24068);
    node_520_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 24068);
    node_343_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_343_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 24580);
    node_343_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 24580);
    node_523_weights_array.format |= AI_FMT_FLAG_CONST;
    node_523_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 25096);
    node_523_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 25096);
    node_523_bias_array.format |= AI_FMT_FLAG_CONST;
    node_523_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 90632);
    node_523_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 90632);
    node_347_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_347_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 91144);
    node_347_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 91144);
    node_526_weights_array.format |= AI_FMT_FLAG_CONST;
    node_526_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 91660);
    node_526_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 91660);
    node_526_bias_array.format |= AI_FMT_FLAG_CONST;
    node_526_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 96268);
    node_526_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 96268);
    node_351_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_351_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 96780);
    node_351_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 96780);
    node_529_weights_array.format |= AI_FMT_FLAG_CONST;
    node_529_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 97296);
    node_529_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 97296);
    node_529_bias_array.format |= AI_FMT_FLAG_CONST;
    node_529_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 162832);
    node_529_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 162832);
    node_532_weights_array.format |= AI_FMT_FLAG_CONST;
    node_532_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 163344);
    node_532_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 163344);
    node_532_bias_array.format |= AI_FMT_FLAG_CONST;
    node_532_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 228880);
    node_532_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 228880);
    node_357_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_357_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 229392);
    node_357_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 229392);
    node_535_weights_array.format |= AI_FMT_FLAG_CONST;
    node_535_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 229908);
    node_535_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 229908);
    node_535_bias_array.format |= AI_FMT_FLAG_CONST;
    node_535_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 234516);
    node_535_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 234516);
    node_361_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_361_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 235028);
    node_361_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 235028);
    node_538_weights_array.format |= AI_FMT_FLAG_CONST;
    node_538_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 235544);
    node_538_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 235544);
    node_538_bias_array.format |= AI_FMT_FLAG_CONST;
    node_538_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 301080);
    node_538_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 301080);
    node_541_weights_array.format |= AI_FMT_FLAG_CONST;
    node_541_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 301592);
    node_541_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 301592);
    node_541_bias_array.format |= AI_FMT_FLAG_CONST;
    node_541_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 367128);
    node_541_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 367128);
    node_368_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_368_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 367640);
    node_368_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 367640);
    node_544_weights_array.format |= AI_FMT_FLAG_CONST;
    node_544_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 368156);
    node_544_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 368156);
    node_544_bias_array.format |= AI_FMT_FLAG_CONST;
    node_544_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 372764);
    node_544_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 372764);
    node_372_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_372_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 373276);
    node_372_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 373276);
    node_547_weights_array.format |= AI_FMT_FLAG_CONST;
    node_547_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 373792);
    node_547_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 373792);
    node_547_bias_array.format |= AI_FMT_FLAG_CONST;
    node_547_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 439328);
    node_547_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 439328);
    node_550_weights_array.format |= AI_FMT_FLAG_CONST;
    node_550_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 439840);
    node_550_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 439840);
    node_550_bias_array.format |= AI_FMT_FLAG_CONST;
    node_550_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 505376);
    node_550_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 505376);
    node_379_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_379_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 505888);
    node_379_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 505888);
    node_553_weights_array.format |= AI_FMT_FLAG_CONST;
    node_553_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 506404);
    node_553_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 506404);
    node_553_bias_array.format |= AI_FMT_FLAG_CONST;
    node_553_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 511012);
    node_553_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 511012);
    node_383_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_383_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 511524);
    node_383_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 511524);
    node_556_weights_array.format |= AI_FMT_FLAG_CONST;
    node_556_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 512040);
    node_556_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 512040);
    node_556_bias_array.format |= AI_FMT_FLAG_CONST;
    node_556_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 577576);
    node_556_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 577576);
    node_559_weights_array.format |= AI_FMT_FLAG_CONST;
    node_559_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 578088);
    node_559_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 578088);
    node_559_bias_array.format |= AI_FMT_FLAG_CONST;
    node_559_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 643624);
    node_559_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 643624);
    node_390_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_390_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 644136);
    node_390_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 644136);
    node_562_weights_array.format |= AI_FMT_FLAG_CONST;
    node_562_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 644652);
    node_562_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 644652);
    node_562_bias_array.format |= AI_FMT_FLAG_CONST;
    node_562_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 649260);
    node_562_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 649260);
    node_394_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_394_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 649772);
    node_394_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 649772);
    node_565_weights_array.format |= AI_FMT_FLAG_CONST;
    node_565_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 650288);
    node_565_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 650288);
    node_565_bias_array.format |= AI_FMT_FLAG_CONST;
    node_565_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 715824);
    node_565_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 715824);
    node_568_weights_array.format |= AI_FMT_FLAG_CONST;
    node_568_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 716336);
    node_568_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 716336);
    node_568_bias_array.format |= AI_FMT_FLAG_CONST;
    node_568_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 847408);
    node_568_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 847408);
    node_401_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_401_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 848432);
    node_401_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 848432);
    node_571_weights_array.format |= AI_FMT_FLAG_CONST;
    node_571_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 849460);
    node_571_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 849460);
    node_571_bias_array.format |= AI_FMT_FLAG_CONST;
    node_571_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 858676);
    node_571_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 858676);
    node_405_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_405_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 859700);
    node_405_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 859700);
    node_574_weights_array.format |= AI_FMT_FLAG_CONST;
    node_574_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 860728);
    node_574_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 860728);
    node_574_bias_array.format |= AI_FMT_FLAG_CONST;
    node_574_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 1122872);
    node_574_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 1122872);
    node_577_weights_array.format |= AI_FMT_FLAG_CONST;
    node_577_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 1123896);
    node_577_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 1123896);
    node_577_bias_array.format |= AI_FMT_FLAG_CONST;
    node_577_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 1386040);
    node_577_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 1386040);
    node_411_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_411_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 1387064);
    node_411_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 1387064);
    node_580_weights_array.format |= AI_FMT_FLAG_CONST;
    node_580_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 1388092);
    node_580_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 1388092);
    node_580_bias_array.format |= AI_FMT_FLAG_CONST;
    node_580_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 1397308);
    node_580_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 1397308);
    node_415_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_415_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 1398332);
    node_415_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 1398332);
    node_583_weights_array.format |= AI_FMT_FLAG_CONST;
    node_583_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 1399360);
    node_583_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 1399360);
    node_583_bias_array.format |= AI_FMT_FLAG_CONST;
    node_583_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 1661504);
    node_583_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 1661504);
    node_586_weights_array.format |= AI_FMT_FLAG_CONST;
    node_586_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 1662528);
    node_586_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 1662528);
    node_586_bias_array.format |= AI_FMT_FLAG_CONST;
    node_586_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 1924672);
    node_586_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 1924672);
    node_422_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_422_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 1925696);
    node_422_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 1925696);
    node_589_weights_array.format |= AI_FMT_FLAG_CONST;
    node_589_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 1926724);
    node_589_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 1926724);
    node_589_bias_array.format |= AI_FMT_FLAG_CONST;
    node_589_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 1935940);
    node_589_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 1935940);
    node_426_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_426_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 1936964);
    node_426_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 1936964);
    node_592_weights_array.format |= AI_FMT_FLAG_CONST;
    node_592_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 1937992);
    node_592_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 1937992);
    node_592_bias_array.format |= AI_FMT_FLAG_CONST;
    node_592_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 2200136);
    node_592_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 2200136);
    node_595_weights_array.format |= AI_FMT_FLAG_CONST;
    node_595_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 2201160);
    node_595_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 2201160);
    node_595_bias_array.format |= AI_FMT_FLAG_CONST;
    node_595_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 2463304);
    node_595_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 2463304);
    node_433_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_433_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 2464328);
    node_433_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 2464328);
    node_598_weights_array.format |= AI_FMT_FLAG_CONST;
    node_598_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 2465356);
    node_598_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 2465356);
    node_598_bias_array.format |= AI_FMT_FLAG_CONST;
    node_598_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 2474572);
    node_598_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 2474572);
    node_437_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_437_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 2475596);
    node_437_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 2475596);
    node_601_weights_array.format |= AI_FMT_FLAG_CONST;
    node_601_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 2476624);
    node_601_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 2476624);
    node_601_bias_array.format |= AI_FMT_FLAG_CONST;
    node_601_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 2738768);
    node_601_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 2738768);
    node_604_weights_array.format |= AI_FMT_FLAG_CONST;
    node_604_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 2739792);
    node_604_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 2739792);
    node_604_bias_array.format |= AI_FMT_FLAG_CONST;
    node_604_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 3001936);
    node_604_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 3001936);
    node_444_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_444_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 3002960);
    node_444_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 3002960);
    node_607_weights_array.format |= AI_FMT_FLAG_CONST;
    node_607_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 3003988);
    node_607_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 3003988);
    node_607_bias_array.format |= AI_FMT_FLAG_CONST;
    node_607_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 3013204);
    node_607_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 3013204);
    node_448_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_448_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 3014228);
    node_448_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 3014228);
    node_610_weights_array.format |= AI_FMT_FLAG_CONST;
    node_610_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 3015256);
    node_610_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 3015256);
    node_610_bias_array.format |= AI_FMT_FLAG_CONST;
    node_610_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 3277400);
    node_610_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 3277400);
    node_613_weights_array.format |= AI_FMT_FLAG_CONST;
    node_613_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 3278424);
    node_613_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 3278424);
    node_613_bias_array.format |= AI_FMT_FLAG_CONST;
    node_613_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 3540568);
    node_613_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 3540568);
    node_455_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_455_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 3541592);
    node_455_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 3541592);
    node_616_weights_array.format |= AI_FMT_FLAG_CONST;
    node_616_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 3542620);
    node_616_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 3542620);
    node_616_bias_array.format |= AI_FMT_FLAG_CONST;
    node_616_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 3551836);
    node_616_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 3551836);
    node_459_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_459_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 3552860);
    node_459_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 3552860);
    node_619_weights_array.format |= AI_FMT_FLAG_CONST;
    node_619_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 3553888);
    node_619_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 3553888);
    node_619_bias_array.format |= AI_FMT_FLAG_CONST;
    node_619_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 3816032);
    node_619_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 3816032);
    node_622_weights_array.format |= AI_FMT_FLAG_CONST;
    node_622_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 3817056);
    node_622_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 3817056);
    node_622_bias_array.format |= AI_FMT_FLAG_CONST;
    node_622_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 4079200);
    node_622_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 4079200);
    node_466_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_466_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 4080224);
    node_466_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 4080224);
    node_625_weights_array.format |= AI_FMT_FLAG_CONST;
    node_625_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 4081252);
    node_625_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 4081252);
    node_625_bias_array.format |= AI_FMT_FLAG_CONST;
    node_625_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 4090468);
    node_625_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 4090468);
    node_470_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_470_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 4091492);
    node_470_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 4091492);
    node_628_weights_array.format |= AI_FMT_FLAG_CONST;
    node_628_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 4092520);
    node_628_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 4092520);
    node_628_bias_array.format |= AI_FMT_FLAG_CONST;
    node_628_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 4354664);
    node_628_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 4354664);
    node_631_weights_array.format |= AI_FMT_FLAG_CONST;
    node_631_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 4355688);
    node_631_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 4355688);
    node_631_bias_array.format |= AI_FMT_FLAG_CONST;
    node_631_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 4879976);
    node_631_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 4879976);
    node_477_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_477_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 4882024);
    node_477_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 4882024);
    node_634_weights_array.format |= AI_FMT_FLAG_CONST;
    node_634_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 4884076);
    node_634_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 4884076);
    node_634_bias_array.format |= AI_FMT_FLAG_CONST;
    node_634_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 4902508);
    node_634_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 4902508);
    node_481_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_481_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 4904556);
    node_481_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 4904556);
    node_637_weights_array.format |= AI_FMT_FLAG_CONST;
    node_637_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 4906608);
    node_637_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 4906608);
    node_637_bias_array.format |= AI_FMT_FLAG_CONST;
    node_637_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 5430896);
    node_637_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 5430896);
    node_640_weights_array.format |= AI_FMT_FLAG_CONST;
    node_640_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 5431920);
    node_640_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 5431920);
    node_640_bias_array.format |= AI_FMT_FLAG_CONST;
    node_640_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 5694064);
    node_640_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 5694064);
    node_487_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_487_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 5695088);
    node_487_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 5695088);
    node_643_weights_array.format |= AI_FMT_FLAG_CONST;
    node_643_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 5696116);
    node_643_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 5696116);
    node_643_bias_array.format |= AI_FMT_FLAG_CONST;
    node_643_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 5705332);
    node_643_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 5705332);
    node_491_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_491_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 5706356);
    node_491_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 5706356);
    node_646_weights_array.format |= AI_FMT_FLAG_CONST;
    node_646_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 5707384);
    node_646_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 5707384);
    node_646_bias_array.format |= AI_FMT_FLAG_CONST;
    node_646_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 5969528);
    node_646_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 5969528);
    node_649_weights_array.format |= AI_FMT_FLAG_CONST;
    node_649_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 5970552);
    node_649_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 5970552);
    node_649_bias_array.format |= AI_FMT_FLAG_CONST;
    node_649_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 6232696);
    node_649_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 6232696);
    node_498_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_498_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 6233720);
    node_498_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 6233720);
    node_652_weights_array.format |= AI_FMT_FLAG_CONST;
    node_652_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 6234748);
    node_652_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 6234748);
    node_652_bias_array.format |= AI_FMT_FLAG_CONST;
    node_652_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 6243964);
    node_652_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 6243964);
    node_502_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_502_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 6244988);
    node_502_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 6244988);
    node_655_weights_array.format |= AI_FMT_FLAG_CONST;
    node_655_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 6246016);
    node_655_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 6246016);
    node_655_bias_array.format |= AI_FMT_FLAG_CONST;
    node_655_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 6508160);
    node_655_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 6508160);
    node_658_weights_array.format |= AI_FMT_FLAG_CONST;
    node_658_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 6509184);
    node_658_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 6509184);
    node_658_bias_array.format |= AI_FMT_FLAG_CONST;
    node_658_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 7033472);
    node_658_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 7033472);
    node_509_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_509_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 7035520);
    node_509_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 7035520);
    node_661_weights_array.format |= AI_FMT_FLAG_CONST;
    node_661_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 7037572);
    node_661_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 7037572);
    node_661_bias_array.format |= AI_FMT_FLAG_CONST;
    node_661_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 7168644);
    node_661_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 7168644);
    node_513_alpha_array.format |= AI_FMT_FLAG_CONST;
    node_513_alpha_array.data = AI_PTR(g_faceid_weights_map[0] + 7168900);
    node_513_alpha_array.data_start = AI_PTR(g_faceid_weights_map[0] + 7168900);
    node_515_weights_array.format |= AI_FMT_FLAG_CONST;
    node_515_weights_array.data = AI_PTR(g_faceid_weights_map[0] + 7169160);
    node_515_weights_array.data_start = AI_PTR(g_faceid_weights_map[0] + 7169160);
    node_515_bias_array.format |= AI_FMT_FLAG_CONST;
    node_515_bias_array.data = AI_PTR(g_faceid_weights_map[0] + 13591688);
    node_515_bias_array.data_start = AI_PTR(g_faceid_weights_map[0] + 13591688);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_WEIGHTS);
  return false;
}


/**  PUBLIC APIs SECTION  *****************************************************/



AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_faceid_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_FACEID_MODEL_NAME,
      .model_signature   = AI_FACEID_MODEL_SIGNATURE,
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
      
      .n_macc            = 447532768,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .params            = AI_STRUCT_INIT,
      .activations       = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x38d47702,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}



AI_API_ENTRY
ai_bool ai_faceid_get_report(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_FACEID_MODEL_NAME,
      .model_signature   = AI_FACEID_MODEL_SIGNATURE,
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
      
      .n_macc            = 447532768,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .map_signature     = AI_MAGIC_SIGNATURE,
      .map_weights       = AI_STRUCT_INIT,
      .map_activations   = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x38d47702,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}


AI_API_ENTRY
ai_error ai_faceid_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}


AI_API_ENTRY
ai_error ai_faceid_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    AI_CONTEXT_OBJ(&AI_NET_OBJ_INSTANCE),
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}


AI_API_ENTRY
ai_error ai_faceid_create_and_init(
  ai_handle* network, const ai_handle activations[], const ai_handle weights[])
{
  ai_error err;
  ai_network_params params;

  err = ai_faceid_create(network, AI_FACEID_DATA_CONFIG);
  if (err.type != AI_ERROR_NONE) {
    return err;
  }
  
  if (ai_faceid_data_params_get(&params) != true) {
    err = ai_faceid_get_error(*network);
    return err;
  }
#if defined(AI_FACEID_DATA_ACTIVATIONS_COUNT)
  /* set the addresses of the activations buffers */
  for (ai_u16 idx=0; activations && idx<params.map_activations.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_activations, idx, activations[idx]);
  }
#endif
#if defined(AI_FACEID_DATA_WEIGHTS_COUNT)
  /* set the addresses of the weight buffers */
  for (ai_u16 idx=0; weights && idx<params.map_weights.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_weights, idx, weights[idx]);
  }
#endif
  if (ai_faceid_init(*network, &params) != true) {
    err = ai_faceid_get_error(*network);
  }
  return err;
}


AI_API_ENTRY
ai_buffer* ai_faceid_inputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_inputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_buffer* ai_faceid_outputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_outputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_handle ai_faceid_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}


AI_API_ENTRY
ai_bool ai_faceid_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = AI_NETWORK_OBJ(ai_platform_network_init(network, params));
  ai_bool ok = true;

  if (!net_ctx) return false;
  ok &= faceid_configure_weights(net_ctx, params);
  ok &= faceid_configure_activations(net_ctx, params);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_faceid_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}


AI_API_ENTRY
ai_i32 ai_faceid_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}



#undef AI_FACEID_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

