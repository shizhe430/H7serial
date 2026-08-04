/**
  ******************************************************************************
  * @file    facedet.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-08-02T14:59:50+0800
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


#include "facedet.h"
#include "facedet_data.h"

#include "ai_platform.h"
#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"



#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_facedet
 
#undef AI_FACEDET_MODEL_SIGNATURE
#define AI_FACEDET_MODEL_SIGNATURE     "0xb892888071d7b1dfbc3bc8694a483d08"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "2026-08-02T14:59:50+0800"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_FACEDET_N_BATCHES
#define AI_FACEDET_N_BATCHES         (1)

static ai_ptr g_facedet_activations_map[1] = AI_C_ARRAY_INIT;
static ai_ptr g_facedet_weights_map[1] = AI_C_ARRAY_INIT;



/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  input_1_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 307200, AI_STATIC)

/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  input_1_Transpose_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 307200, AI_STATIC)

/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  node_286_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 409600, AI_STATIC)

/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  node_288_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 409600, AI_STATIC)

/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  node_289_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 409600, AI_STATIC)

/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  node_291_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 409600, AI_STATIC)

/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  node_292_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 409600, AI_STATIC)

/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  node_294_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 409600, AI_STATIC)

/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  node_295_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 102400, AI_STATIC)

/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  node_297_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 102400, AI_STATIC)

/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  node_298_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256000, AI_STATIC)

/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  node_300_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256000, AI_STATIC)

/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  node_301_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256000, AI_STATIC)

/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  node_303_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256000, AI_STATIC)

/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  node_304_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256000, AI_STATIC)

/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  node_306_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256000, AI_STATIC)

/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  node_307_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64000, AI_STATIC)

/* Array#17 */
AI_ARRAY_OBJ_DECLARE(
  node_309_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64000, AI_STATIC)

/* Array#18 */
AI_ARRAY_OBJ_DECLARE(
  node_310_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 115200, AI_STATIC)

/* Array#19 */
AI_ARRAY_OBJ_DECLARE(
  node_312_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 115200, AI_STATIC)

/* Array#20 */
AI_ARRAY_OBJ_DECLARE(
  node_313_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 115200, AI_STATIC)

/* Array#21 */
AI_ARRAY_OBJ_DECLARE(
  node_315_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 115200, AI_STATIC)

/* Array#22 */
AI_ARRAY_OBJ_DECLARE(
  node_316_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 115200, AI_STATIC)

/* Array#23 */
AI_ARRAY_OBJ_DECLARE(
  node_318_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 115200, AI_STATIC)

/* Array#24 */
AI_ARRAY_OBJ_DECLARE(
  node_319_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 115200, AI_STATIC)

/* Array#25 */
AI_ARRAY_OBJ_DECLARE(
  node_321_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 115200, AI_STATIC)

/* Array#26 */
AI_ARRAY_OBJ_DECLARE(
  node_322_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 115200, AI_STATIC)

/* Array#27 */
AI_ARRAY_OBJ_DECLARE(
  node_324_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 115200, AI_STATIC)

/* Array#28 */
AI_ARRAY_OBJ_DECLARE(
  node_373_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25600, AI_STATIC)

/* Array#29 */
AI_ARRAY_OBJ_DECLARE(
  node_325_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#30 */
AI_ARRAY_OBJ_DECLARE(
  node_327_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#31 */
AI_ARRAY_OBJ_DECLARE(
  node_328_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 60800, AI_STATIC)

/* Array#32 */
AI_ARRAY_OBJ_DECLARE(
  node_330_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 60800, AI_STATIC)

/* Array#33 */
AI_ARRAY_OBJ_DECLARE(
  node_331_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 60800, AI_STATIC)

/* Array#34 */
AI_ARRAY_OBJ_DECLARE(
  node_333_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 60800, AI_STATIC)

/* Array#35 */
AI_ARRAY_OBJ_DECLARE(
  node_334_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 60800, AI_STATIC)

/* Array#36 */
AI_ARRAY_OBJ_DECLARE(
  node_336_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 60800, AI_STATIC)

/* Array#37 */
AI_ARRAY_OBJ_DECLARE(
  node_374_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6400, AI_STATIC)

/* Array#38 */
AI_ARRAY_OBJ_DECLARE(
  node_337_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 15200, AI_STATIC)

/* Array#39 */
AI_ARRAY_OBJ_DECLARE(
  node_339_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 15200, AI_STATIC)

/* Array#40 */
AI_ARRAY_OBJ_DECLARE(
  node_340_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#41 */
AI_ARRAY_OBJ_DECLARE(
  node_342_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#42 */
AI_ARRAY_OBJ_DECLARE(
  node_343_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#43 */
AI_ARRAY_OBJ_DECLARE(
  node_345_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#44 */
AI_ARRAY_OBJ_DECLARE(
  node_346_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#45 */
AI_ARRAY_OBJ_DECLARE(
  node_348_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#46 */
AI_ARRAY_OBJ_DECLARE(
  node_349_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#47 */
AI_ARRAY_OBJ_DECLARE(
  node_351_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#48 */
AI_ARRAY_OBJ_DECLARE(
  node_352_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#49 */
AI_ARRAY_OBJ_DECLARE(
  node_354_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#50 */
AI_ARRAY_OBJ_DECLARE(
  node_355_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#51 */
AI_ARRAY_OBJ_DECLARE(
  node_357_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#52 */
AI_ARRAY_OBJ_DECLARE(
  node_358_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#53 */
AI_ARRAY_OBJ_DECLARE(
  node_360_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#54 */
AI_ARRAY_OBJ_DECLARE(
  node_361_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#55 */
AI_ARRAY_OBJ_DECLARE(
  node_363_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#56 */
AI_ARRAY_OBJ_DECLARE(
  node_364_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#57 */
AI_ARRAY_OBJ_DECLARE(
  node_366_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#58 */
AI_ARRAY_OBJ_DECLARE(
  node_367_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#59 */
AI_ARRAY_OBJ_DECLARE(
  node_369_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#60 */
AI_ARRAY_OBJ_DECLARE(
  node_370_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#61 */
AI_ARRAY_OBJ_DECLARE(
  node_372_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 28800, AI_STATIC)

/* Array#62 */
AI_ARRAY_OBJ_DECLARE(
  node_375_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1600, AI_STATIC)

/* Array#63 */
AI_ARRAY_OBJ_DECLARE(
  node_418_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1600, AI_STATIC)

/* Array#64 */
AI_ARRAY_OBJ_DECLARE(
  node_394_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6400, AI_STATIC)

/* Array#65 */
AI_ARRAY_OBJ_DECLARE(
  node_395_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6400, AI_STATIC)

/* Array#66 */
AI_ARRAY_OBJ_DECLARE(
  node_417_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6400, AI_STATIC)

/* Array#67 */
AI_ARRAY_OBJ_DECLARE(
  node_414_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25600, AI_STATIC)

/* Array#68 */
AI_ARRAY_OBJ_DECLARE(
  node_415_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25600, AI_STATIC)

/* Array#69 */
AI_ARRAY_OBJ_DECLARE(
  node_416_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25600, AI_STATIC)

/* Array#70 */
AI_ARRAY_OBJ_DECLARE(
  node_425_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25600, AI_STATIC)

/* Array#71 */
AI_ARRAY_OBJ_DECLARE(
  node_427_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25600, AI_STATIC)

/* Array#72 */
AI_ARRAY_OBJ_DECLARE(
  node_428_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 102400, AI_STATIC)

/* Array#73 */
AI_ARRAY_OBJ_DECLARE(
  node_430_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 102400, AI_STATIC)

/* Array#74 */
AI_ARRAY_OBJ_DECLARE(
  node_431_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 102400, AI_STATIC)

/* Array#75 */
AI_ARRAY_OBJ_DECLARE(
  node_433_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 102400, AI_STATIC)

/* Array#76 */
AI_ARRAY_OBJ_DECLARE(
  node_434_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 102400, AI_STATIC)

/* Array#77 */
AI_ARRAY_OBJ_DECLARE(
  node_436_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 102400, AI_STATIC)

/* Array#78 */
AI_ARRAY_OBJ_DECLARE(
  node_439_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 32000, AI_STATIC)

/* Array#79 */
AI_ARRAY_OBJ_DECLARE(
  node_447_reshape_in_078_to_chlast_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 32000, AI_STATIC)

/* Array#80 */
AI_ARRAY_OBJ_DECLARE(
  node_449_to_chlast_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 32000, AI_STATIC)

/* Array#81 */
AI_ARRAY_OBJ_DECLARE(
  node_438_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12800, AI_STATIC)

/* Array#82 */
AI_ARRAY_OBJ_DECLARE(
  node_444_reshape_in_081_to_chlast_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12800, AI_STATIC)

/* Array#83 */
AI_ARRAY_OBJ_DECLARE(
  node_446_to_chlast_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 12800, AI_STATIC)

/* Array#84 */
AI_ARRAY_OBJ_DECLARE(
  node_437_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3200, AI_STATIC)

/* Array#85 */
AI_ARRAY_OBJ_DECLARE(
  node_440_reshape_in_084_to_chlast_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3200, AI_STATIC)

/* Array#86 */
AI_ARRAY_OBJ_DECLARE(
  node_442_to_chlast_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3200, AI_STATIC)

/* Array#87 */
AI_ARRAY_OBJ_DECLARE(
  node_443_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 3200, AI_STATIC)

/* Array#88 */
AI_ARRAY_OBJ_DECLARE(
  node_419_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6400, AI_STATIC)

/* Array#89 */
AI_ARRAY_OBJ_DECLARE(
  node_420_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6400, AI_STATIC)

/* Array#90 */
AI_ARRAY_OBJ_DECLARE(
  node_423_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6400, AI_STATIC)

/* Array#91 */
AI_ARRAY_OBJ_DECLARE(
  node_450_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6400, AI_STATIC)

/* Array#92 */
AI_ARRAY_OBJ_DECLARE(
  node_452_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6400, AI_STATIC)

/* Array#93 */
AI_ARRAY_OBJ_DECLARE(
  node_453_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25600, AI_STATIC)

/* Array#94 */
AI_ARRAY_OBJ_DECLARE(
  node_455_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25600, AI_STATIC)

/* Array#95 */
AI_ARRAY_OBJ_DECLARE(
  node_456_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25600, AI_STATIC)

/* Array#96 */
AI_ARRAY_OBJ_DECLARE(
  node_458_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25600, AI_STATIC)

/* Array#97 */
AI_ARRAY_OBJ_DECLARE(
  node_459_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25600, AI_STATIC)

/* Array#98 */
AI_ARRAY_OBJ_DECLARE(
  node_461_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25600, AI_STATIC)

/* Array#99 */
AI_ARRAY_OBJ_DECLARE(
  node_464_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8000, AI_STATIC)

/* Array#100 */
AI_ARRAY_OBJ_DECLARE(
  node_472_reshape_in_099_to_chlast_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8000, AI_STATIC)

/* Array#101 */
AI_ARRAY_OBJ_DECLARE(
  node_474_to_chlast_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 8000, AI_STATIC)

/* Array#102 */
AI_ARRAY_OBJ_DECLARE(
  node_463_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3200, AI_STATIC)

/* Array#103 */
AI_ARRAY_OBJ_DECLARE(
  node_469_reshape_in_0102_to_chlast_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3200, AI_STATIC)

/* Array#104 */
AI_ARRAY_OBJ_DECLARE(
  node_471_to_chlast_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 3200, AI_STATIC)

/* Array#105 */
AI_ARRAY_OBJ_DECLARE(
  node_462_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 800, AI_STATIC)

/* Array#106 */
AI_ARRAY_OBJ_DECLARE(
  node_465_reshape_in_0105_to_chlast_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 800, AI_STATIC)

/* Array#107 */
AI_ARRAY_OBJ_DECLARE(
  node_467_to_chlast_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 800, AI_STATIC)

/* Array#108 */
AI_ARRAY_OBJ_DECLARE(
  node_468_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 800, AI_STATIC)

/* Array#109 */
AI_ARRAY_OBJ_DECLARE(
  node_421_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1600, AI_STATIC)

/* Array#110 */
AI_ARRAY_OBJ_DECLARE(
  node_422_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1600, AI_STATIC)

/* Array#111 */
AI_ARRAY_OBJ_DECLARE(
  node_424_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1600, AI_STATIC)

/* Array#112 */
AI_ARRAY_OBJ_DECLARE(
  node_475_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1600, AI_STATIC)

/* Array#113 */
AI_ARRAY_OBJ_DECLARE(
  node_477_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1600, AI_STATIC)

/* Array#114 */
AI_ARRAY_OBJ_DECLARE(
  node_478_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6400, AI_STATIC)

/* Array#115 */
AI_ARRAY_OBJ_DECLARE(
  node_480_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6400, AI_STATIC)

/* Array#116 */
AI_ARRAY_OBJ_DECLARE(
  node_481_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6400, AI_STATIC)

/* Array#117 */
AI_ARRAY_OBJ_DECLARE(
  node_483_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6400, AI_STATIC)

/* Array#118 */
AI_ARRAY_OBJ_DECLARE(
  node_484_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6400, AI_STATIC)

/* Array#119 */
AI_ARRAY_OBJ_DECLARE(
  node_486_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6400, AI_STATIC)

/* Array#120 */
AI_ARRAY_OBJ_DECLARE(
  node_489_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2000, AI_STATIC)

/* Array#121 */
AI_ARRAY_OBJ_DECLARE(
  node_497_reshape_in_0120_to_chlast_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2000, AI_STATIC)

/* Array#122 */
AI_ARRAY_OBJ_DECLARE(
  node_499_to_chlast_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 2000, AI_STATIC)

/* Array#123 */
AI_ARRAY_OBJ_DECLARE(
  node_488_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 800, AI_STATIC)

/* Array#124 */
AI_ARRAY_OBJ_DECLARE(
  node_494_reshape_in_0123_to_chlast_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 800, AI_STATIC)

/* Array#125 */
AI_ARRAY_OBJ_DECLARE(
  node_496_to_chlast_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 800, AI_STATIC)

/* Array#126 */
AI_ARRAY_OBJ_DECLARE(
  node_487_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200, AI_STATIC)

/* Array#127 */
AI_ARRAY_OBJ_DECLARE(
  node_490_reshape_in_0126_to_chlast_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200, AI_STATIC)

/* Array#128 */
AI_ARRAY_OBJ_DECLARE(
  node_492_to_chlast_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200, AI_STATIC)

/* Array#129 */
AI_ARRAY_OBJ_DECLARE(
  node_493_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 200, AI_STATIC)

/* Array#130 */
AI_ARRAY_OBJ_DECLARE(
  node_286_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 432, AI_STATIC)

/* Array#131 */
AI_ARRAY_OBJ_DECLARE(
  node_286_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#132 */
AI_ARRAY_OBJ_DECLARE(
  node_289_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 144, AI_STATIC)

/* Array#133 */
AI_ARRAY_OBJ_DECLARE(
  node_289_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#134 */
AI_ARRAY_OBJ_DECLARE(
  node_292_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#135 */
AI_ARRAY_OBJ_DECLARE(
  node_292_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#136 */
AI_ARRAY_OBJ_DECLARE(
  node_295_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 144, AI_STATIC)

/* Array#137 */
AI_ARRAY_OBJ_DECLARE(
  node_295_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#138 */
AI_ARRAY_OBJ_DECLARE(
  node_298_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 640, AI_STATIC)

/* Array#139 */
AI_ARRAY_OBJ_DECLARE(
  node_298_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 40, AI_STATIC)

/* Array#140 */
AI_ARRAY_OBJ_DECLARE(
  node_301_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 360, AI_STATIC)

/* Array#141 */
AI_ARRAY_OBJ_DECLARE(
  node_301_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 40, AI_STATIC)

/* Array#142 */
AI_ARRAY_OBJ_DECLARE(
  node_304_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1600, AI_STATIC)

/* Array#143 */
AI_ARRAY_OBJ_DECLARE(
  node_304_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 40, AI_STATIC)

/* Array#144 */
AI_ARRAY_OBJ_DECLARE(
  node_307_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 360, AI_STATIC)

/* Array#145 */
AI_ARRAY_OBJ_DECLARE(
  node_307_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 40, AI_STATIC)

/* Array#146 */
AI_ARRAY_OBJ_DECLARE(
  node_310_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2880, AI_STATIC)

/* Array#147 */
AI_ARRAY_OBJ_DECLARE(
  node_310_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 72, AI_STATIC)

/* Array#148 */
AI_ARRAY_OBJ_DECLARE(
  node_313_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 648, AI_STATIC)

/* Array#149 */
AI_ARRAY_OBJ_DECLARE(
  node_313_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 72, AI_STATIC)

/* Array#150 */
AI_ARRAY_OBJ_DECLARE(
  node_316_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 5184, AI_STATIC)

/* Array#151 */
AI_ARRAY_OBJ_DECLARE(
  node_316_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 72, AI_STATIC)

/* Array#152 */
AI_ARRAY_OBJ_DECLARE(
  node_319_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 648, AI_STATIC)

/* Array#153 */
AI_ARRAY_OBJ_DECLARE(
  node_319_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 72, AI_STATIC)

/* Array#154 */
AI_ARRAY_OBJ_DECLARE(
  node_322_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 5184, AI_STATIC)

/* Array#155 */
AI_ARRAY_OBJ_DECLARE(
  node_322_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 72, AI_STATIC)

/* Array#156 */
AI_ARRAY_OBJ_DECLARE(
  node_373_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1152, AI_STATIC)

/* Array#157 */
AI_ARRAY_OBJ_DECLARE(
  node_373_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#158 */
AI_ARRAY_OBJ_DECLARE(
  node_325_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 648, AI_STATIC)

/* Array#159 */
AI_ARRAY_OBJ_DECLARE(
  node_325_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 72, AI_STATIC)

/* Array#160 */
AI_ARRAY_OBJ_DECLARE(
  node_328_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 10944, AI_STATIC)

/* Array#161 */
AI_ARRAY_OBJ_DECLARE(
  node_328_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 152, AI_STATIC)

/* Array#162 */
AI_ARRAY_OBJ_DECLARE(
  node_331_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1368, AI_STATIC)

/* Array#163 */
AI_ARRAY_OBJ_DECLARE(
  node_331_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 152, AI_STATIC)

/* Array#164 */
AI_ARRAY_OBJ_DECLARE(
  node_334_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 23104, AI_STATIC)

/* Array#165 */
AI_ARRAY_OBJ_DECLARE(
  node_334_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 152, AI_STATIC)

/* Array#166 */
AI_ARRAY_OBJ_DECLARE(
  node_374_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2432, AI_STATIC)

/* Array#167 */
AI_ARRAY_OBJ_DECLARE(
  node_374_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#168 */
AI_ARRAY_OBJ_DECLARE(
  node_337_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1368, AI_STATIC)

/* Array#169 */
AI_ARRAY_OBJ_DECLARE(
  node_337_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 152, AI_STATIC)

/* Array#170 */
AI_ARRAY_OBJ_DECLARE(
  node_340_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 43776, AI_STATIC)

/* Array#171 */
AI_ARRAY_OBJ_DECLARE(
  node_340_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#172 */
AI_ARRAY_OBJ_DECLARE(
  node_343_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2592, AI_STATIC)

/* Array#173 */
AI_ARRAY_OBJ_DECLARE(
  node_343_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#174 */
AI_ARRAY_OBJ_DECLARE(
  node_346_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 82944, AI_STATIC)

/* Array#175 */
AI_ARRAY_OBJ_DECLARE(
  node_346_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#176 */
AI_ARRAY_OBJ_DECLARE(
  node_349_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2592, AI_STATIC)

/* Array#177 */
AI_ARRAY_OBJ_DECLARE(
  node_349_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#178 */
AI_ARRAY_OBJ_DECLARE(
  node_352_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 82944, AI_STATIC)

/* Array#179 */
AI_ARRAY_OBJ_DECLARE(
  node_352_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#180 */
AI_ARRAY_OBJ_DECLARE(
  node_355_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2592, AI_STATIC)

/* Array#181 */
AI_ARRAY_OBJ_DECLARE(
  node_355_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#182 */
AI_ARRAY_OBJ_DECLARE(
  node_358_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 82944, AI_STATIC)

/* Array#183 */
AI_ARRAY_OBJ_DECLARE(
  node_358_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#184 */
AI_ARRAY_OBJ_DECLARE(
  node_361_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2592, AI_STATIC)

/* Array#185 */
AI_ARRAY_OBJ_DECLARE(
  node_361_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#186 */
AI_ARRAY_OBJ_DECLARE(
  node_364_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 82944, AI_STATIC)

/* Array#187 */
AI_ARRAY_OBJ_DECLARE(
  node_364_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#188 */
AI_ARRAY_OBJ_DECLARE(
  node_367_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2592, AI_STATIC)

/* Array#189 */
AI_ARRAY_OBJ_DECLARE(
  node_367_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#190 */
AI_ARRAY_OBJ_DECLARE(
  node_370_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 82944, AI_STATIC)

/* Array#191 */
AI_ARRAY_OBJ_DECLARE(
  node_370_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#192 */
AI_ARRAY_OBJ_DECLARE(
  node_375_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4608, AI_STATIC)

/* Array#193 */
AI_ARRAY_OBJ_DECLARE(
  node_375_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#194 */
AI_ARRAY_OBJ_DECLARE(
  node_418_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#195 */
AI_ARRAY_OBJ_DECLARE(
  node_418_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#196 */
AI_ARRAY_OBJ_DECLARE(
  node_417_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#197 */
AI_ARRAY_OBJ_DECLARE(
  node_417_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#198 */
AI_ARRAY_OBJ_DECLARE(
  node_416_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#199 */
AI_ARRAY_OBJ_DECLARE(
  node_416_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#200 */
AI_ARRAY_OBJ_DECLARE(
  node_425_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 144, AI_STATIC)

/* Array#201 */
AI_ARRAY_OBJ_DECLARE(
  node_425_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#202 */
AI_ARRAY_OBJ_DECLARE(
  node_428_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1024, AI_STATIC)

/* Array#203 */
AI_ARRAY_OBJ_DECLARE(
  node_428_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#204 */
AI_ARRAY_OBJ_DECLARE(
  node_431_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 576, AI_STATIC)

/* Array#205 */
AI_ARRAY_OBJ_DECLARE(
  node_431_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#206 */
AI_ARRAY_OBJ_DECLARE(
  node_434_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4096, AI_STATIC)

/* Array#207 */
AI_ARRAY_OBJ_DECLARE(
  node_434_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#208 */
AI_ARRAY_OBJ_DECLARE(
  node_439_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 11520, AI_STATIC)

/* Array#209 */
AI_ARRAY_OBJ_DECLARE(
  node_439_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 20, AI_STATIC)

/* Array#210 */
AI_ARRAY_OBJ_DECLARE(
  node_438_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4608, AI_STATIC)

/* Array#211 */
AI_ARRAY_OBJ_DECLARE(
  node_438_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)

/* Array#212 */
AI_ARRAY_OBJ_DECLARE(
  node_437_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1152, AI_STATIC)

/* Array#213 */
AI_ARRAY_OBJ_DECLARE(
  node_437_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2, AI_STATIC)

/* Array#214 */
AI_ARRAY_OBJ_DECLARE(
  node_419_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#215 */
AI_ARRAY_OBJ_DECLARE(
  node_423_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#216 */
AI_ARRAY_OBJ_DECLARE(
  node_423_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#217 */
AI_ARRAY_OBJ_DECLARE(
  node_450_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 144, AI_STATIC)

/* Array#218 */
AI_ARRAY_OBJ_DECLARE(
  node_450_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#219 */
AI_ARRAY_OBJ_DECLARE(
  node_453_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1024, AI_STATIC)

/* Array#220 */
AI_ARRAY_OBJ_DECLARE(
  node_453_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#221 */
AI_ARRAY_OBJ_DECLARE(
  node_456_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 576, AI_STATIC)

/* Array#222 */
AI_ARRAY_OBJ_DECLARE(
  node_456_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#223 */
AI_ARRAY_OBJ_DECLARE(
  node_459_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4096, AI_STATIC)

/* Array#224 */
AI_ARRAY_OBJ_DECLARE(
  node_459_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#225 */
AI_ARRAY_OBJ_DECLARE(
  node_464_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 11520, AI_STATIC)

/* Array#226 */
AI_ARRAY_OBJ_DECLARE(
  node_464_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 20, AI_STATIC)

/* Array#227 */
AI_ARRAY_OBJ_DECLARE(
  node_463_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4608, AI_STATIC)

/* Array#228 */
AI_ARRAY_OBJ_DECLARE(
  node_463_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)

/* Array#229 */
AI_ARRAY_OBJ_DECLARE(
  node_462_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1152, AI_STATIC)

/* Array#230 */
AI_ARRAY_OBJ_DECLARE(
  node_462_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2, AI_STATIC)

/* Array#231 */
AI_ARRAY_OBJ_DECLARE(
  node_421_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#232 */
AI_ARRAY_OBJ_DECLARE(
  node_424_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#233 */
AI_ARRAY_OBJ_DECLARE(
  node_424_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#234 */
AI_ARRAY_OBJ_DECLARE(
  node_475_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 144, AI_STATIC)

/* Array#235 */
AI_ARRAY_OBJ_DECLARE(
  node_475_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#236 */
AI_ARRAY_OBJ_DECLARE(
  node_478_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1024, AI_STATIC)

/* Array#237 */
AI_ARRAY_OBJ_DECLARE(
  node_478_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#238 */
AI_ARRAY_OBJ_DECLARE(
  node_481_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 576, AI_STATIC)

/* Array#239 */
AI_ARRAY_OBJ_DECLARE(
  node_481_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#240 */
AI_ARRAY_OBJ_DECLARE(
  node_484_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4096, AI_STATIC)

/* Array#241 */
AI_ARRAY_OBJ_DECLARE(
  node_484_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#242 */
AI_ARRAY_OBJ_DECLARE(
  node_489_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 11520, AI_STATIC)

/* Array#243 */
AI_ARRAY_OBJ_DECLARE(
  node_489_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 20, AI_STATIC)

/* Array#244 */
AI_ARRAY_OBJ_DECLARE(
  node_488_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4608, AI_STATIC)

/* Array#245 */
AI_ARRAY_OBJ_DECLARE(
  node_488_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)

/* Array#246 */
AI_ARRAY_OBJ_DECLARE(
  node_487_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1152, AI_STATIC)

/* Array#247 */
AI_ARRAY_OBJ_DECLARE(
  node_487_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2, AI_STATIC)

/* Array#248 */
AI_ARRAY_OBJ_DECLARE(
  node_286_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 27, AI_STATIC)

/* Array#249 */
AI_ARRAY_OBJ_DECLARE(
  node_292_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#250 */
AI_ARRAY_OBJ_DECLARE(
  node_298_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#251 */
AI_ARRAY_OBJ_DECLARE(
  node_304_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 40, AI_STATIC)

/* Array#252 */
AI_ARRAY_OBJ_DECLARE(
  node_310_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 40, AI_STATIC)

/* Array#253 */
AI_ARRAY_OBJ_DECLARE(
  node_316_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 72, AI_STATIC)

/* Array#254 */
AI_ARRAY_OBJ_DECLARE(
  node_322_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 72, AI_STATIC)

/* Array#255 */
AI_ARRAY_OBJ_DECLARE(
  node_373_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 72, AI_STATIC)

/* Array#256 */
AI_ARRAY_OBJ_DECLARE(
  node_328_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 72, AI_STATIC)

/* Array#257 */
AI_ARRAY_OBJ_DECLARE(
  node_334_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 152, AI_STATIC)

/* Array#258 */
AI_ARRAY_OBJ_DECLARE(
  node_374_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 152, AI_STATIC)

/* Array#259 */
AI_ARRAY_OBJ_DECLARE(
  node_340_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 152, AI_STATIC)

/* Array#260 */
AI_ARRAY_OBJ_DECLARE(
  node_346_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#261 */
AI_ARRAY_OBJ_DECLARE(
  node_352_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#262 */
AI_ARRAY_OBJ_DECLARE(
  node_358_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#263 */
AI_ARRAY_OBJ_DECLARE(
  node_364_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#264 */
AI_ARRAY_OBJ_DECLARE(
  node_370_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#265 */
AI_ARRAY_OBJ_DECLARE(
  node_375_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)

/* Array#266 */
AI_ARRAY_OBJ_DECLARE(
  node_418_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 144, AI_STATIC)

/* Array#267 */
AI_ARRAY_OBJ_DECLARE(
  node_417_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 144, AI_STATIC)

/* Array#268 */
AI_ARRAY_OBJ_DECLARE(
  node_416_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 144, AI_STATIC)

/* Array#269 */
AI_ARRAY_OBJ_DECLARE(
  node_428_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#270 */
AI_ARRAY_OBJ_DECLARE(
  node_434_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#271 */
AI_ARRAY_OBJ_DECLARE(
  node_439_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 576, AI_STATIC)

/* Array#272 */
AI_ARRAY_OBJ_DECLARE(
  node_438_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 576, AI_STATIC)

/* Array#273 */
AI_ARRAY_OBJ_DECLARE(
  node_437_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 576, AI_STATIC)

/* Array#274 */
AI_ARRAY_OBJ_DECLARE(
  node_419_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 144, AI_STATIC)

/* Array#275 */
AI_ARRAY_OBJ_DECLARE(
  node_423_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 144, AI_STATIC)

/* Array#276 */
AI_ARRAY_OBJ_DECLARE(
  node_453_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#277 */
AI_ARRAY_OBJ_DECLARE(
  node_459_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#278 */
AI_ARRAY_OBJ_DECLARE(
  node_464_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 576, AI_STATIC)

/* Array#279 */
AI_ARRAY_OBJ_DECLARE(
  node_463_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 576, AI_STATIC)

/* Array#280 */
AI_ARRAY_OBJ_DECLARE(
  node_462_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 576, AI_STATIC)

/* Array#281 */
AI_ARRAY_OBJ_DECLARE(
  node_421_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 144, AI_STATIC)

/* Array#282 */
AI_ARRAY_OBJ_DECLARE(
  node_424_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 144, AI_STATIC)

/* Array#283 */
AI_ARRAY_OBJ_DECLARE(
  node_478_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#284 */
AI_ARRAY_OBJ_DECLARE(
  node_484_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#285 */
AI_ARRAY_OBJ_DECLARE(
  node_489_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 576, AI_STATIC)

/* Array#286 */
AI_ARRAY_OBJ_DECLARE(
  node_488_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 576, AI_STATIC)

/* Array#287 */
AI_ARRAY_OBJ_DECLARE(
  node_487_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 576, AI_STATIC)

/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  input_1_Transpose_output, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 320, 320), AI_STRIDE_INIT(4, 4, 4, 12, 3840),
  1, &input_1_Transpose_output_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  input_1_output, AI_STATIC,
  1, 0x0,
  AI_SHAPE_INIT(4, 1, 320, 320, 3), AI_STRIDE_INIT(4, 4, 4, 1280, 409600),
  1, &input_1_output_array, NULL)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  node_286_bias, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &node_286_bias_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  node_286_output, AI_STATIC,
  3, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 160, 160), AI_STRIDE_INIT(4, 4, 4, 64, 10240),
  1, &node_286_output_array, NULL)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  node_286_scratch0, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 3), AI_STRIDE_INIT(4, 4, 4, 12, 36),
  1, &node_286_scratch0_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  node_286_weights, AI_STATIC,
  5, 0x0,
  AI_SHAPE_INIT(4, 3, 3, 3, 16), AI_STRIDE_INIT(4, 4, 12, 192, 576),
  1, &node_286_weights_array, NULL)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  node_288_output, AI_STATIC,
  6, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 160, 160), AI_STRIDE_INIT(4, 4, 4, 64, 10240),
  1, &node_288_output_array, NULL)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  node_289_bias, AI_STATIC,
  7, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &node_289_bias_array, NULL)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  node_289_output, AI_STATIC,
  8, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 160, 160), AI_STRIDE_INIT(4, 4, 4, 64, 10240),
  1, &node_289_output_array, NULL)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  node_289_weights, AI_STATIC,
  9, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 16), AI_STRIDE_INIT(4, 1, 16, 16, 16),
  1, &node_289_weights_array, NULL)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  node_291_output, AI_STATIC,
  10, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 160, 160), AI_STRIDE_INIT(4, 4, 4, 64, 10240),
  1, &node_291_output_array, NULL)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  node_292_bias, AI_STATIC,
  11, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &node_292_bias_array, NULL)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  node_292_output, AI_STATIC,
  12, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 160, 160), AI_STRIDE_INIT(4, 4, 4, 64, 10240),
  1, &node_292_output_array, NULL)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  node_292_scratch0, AI_STATIC,
  13, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &node_292_scratch0_array, NULL)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  node_292_weights, AI_STATIC,
  14, 0x0,
  AI_SHAPE_INIT(4, 16, 1, 1, 16), AI_STRIDE_INIT(4, 4, 64, 1024, 1024),
  1, &node_292_weights_array, NULL)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  node_294_output, AI_STATIC,
  15, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 160, 160), AI_STRIDE_INIT(4, 4, 4, 64, 10240),
  1, &node_294_output_array, NULL)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  node_295_bias, AI_STATIC,
  16, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &node_295_bias_array, NULL)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  node_295_output, AI_STATIC,
  17, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 80, 80), AI_STRIDE_INIT(4, 4, 4, 64, 5120),
  1, &node_295_output_array, NULL)

/* Tensor #18 */
AI_TENSOR_OBJ_DECLARE(
  node_295_weights, AI_STATIC,
  18, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 16), AI_STRIDE_INIT(4, 1, 16, 16, 16),
  1, &node_295_weights_array, NULL)

/* Tensor #19 */
AI_TENSOR_OBJ_DECLARE(
  node_297_output, AI_STATIC,
  19, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 80, 80), AI_STRIDE_INIT(4, 4, 4, 64, 5120),
  1, &node_297_output_array, NULL)

/* Tensor #20 */
AI_TENSOR_OBJ_DECLARE(
  node_298_bias, AI_STATIC,
  20, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 1, 1), AI_STRIDE_INIT(4, 4, 4, 160, 160),
  1, &node_298_bias_array, NULL)

/* Tensor #21 */
AI_TENSOR_OBJ_DECLARE(
  node_298_output, AI_STATIC,
  21, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 80, 80), AI_STRIDE_INIT(4, 4, 4, 160, 12800),
  1, &node_298_output_array, NULL)

/* Tensor #22 */
AI_TENSOR_OBJ_DECLARE(
  node_298_scratch0, AI_STATIC,
  22, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &node_298_scratch0_array, NULL)

/* Tensor #23 */
AI_TENSOR_OBJ_DECLARE(
  node_298_weights, AI_STATIC,
  23, 0x0,
  AI_SHAPE_INIT(4, 16, 1, 1, 40), AI_STRIDE_INIT(4, 4, 64, 2560, 2560),
  1, &node_298_weights_array, NULL)

/* Tensor #24 */
AI_TENSOR_OBJ_DECLARE(
  node_300_output, AI_STATIC,
  24, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 80, 80), AI_STRIDE_INIT(4, 4, 4, 160, 12800),
  1, &node_300_output_array, NULL)

/* Tensor #25 */
AI_TENSOR_OBJ_DECLARE(
  node_301_bias, AI_STATIC,
  25, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 1, 1), AI_STRIDE_INIT(4, 4, 4, 160, 160),
  1, &node_301_bias_array, NULL)

/* Tensor #26 */
AI_TENSOR_OBJ_DECLARE(
  node_301_output, AI_STATIC,
  26, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 80, 80), AI_STRIDE_INIT(4, 4, 4, 160, 12800),
  1, &node_301_output_array, NULL)

/* Tensor #27 */
AI_TENSOR_OBJ_DECLARE(
  node_301_weights, AI_STATIC,
  27, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 40), AI_STRIDE_INIT(4, 1, 40, 40, 40),
  1, &node_301_weights_array, NULL)

/* Tensor #28 */
AI_TENSOR_OBJ_DECLARE(
  node_303_output, AI_STATIC,
  28, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 80, 80), AI_STRIDE_INIT(4, 4, 4, 160, 12800),
  1, &node_303_output_array, NULL)

/* Tensor #29 */
AI_TENSOR_OBJ_DECLARE(
  node_304_bias, AI_STATIC,
  29, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 1, 1), AI_STRIDE_INIT(4, 4, 4, 160, 160),
  1, &node_304_bias_array, NULL)

/* Tensor #30 */
AI_TENSOR_OBJ_DECLARE(
  node_304_output, AI_STATIC,
  30, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 80, 80), AI_STRIDE_INIT(4, 4, 4, 160, 12800),
  1, &node_304_output_array, NULL)

/* Tensor #31 */
AI_TENSOR_OBJ_DECLARE(
  node_304_scratch0, AI_STATIC,
  31, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 1, 1), AI_STRIDE_INIT(4, 4, 4, 160, 160),
  1, &node_304_scratch0_array, NULL)

/* Tensor #32 */
AI_TENSOR_OBJ_DECLARE(
  node_304_weights, AI_STATIC,
  32, 0x0,
  AI_SHAPE_INIT(4, 40, 1, 1, 40), AI_STRIDE_INIT(4, 4, 160, 6400, 6400),
  1, &node_304_weights_array, NULL)

/* Tensor #33 */
AI_TENSOR_OBJ_DECLARE(
  node_306_output, AI_STATIC,
  33, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 80, 80), AI_STRIDE_INIT(4, 4, 4, 160, 12800),
  1, &node_306_output_array, NULL)

/* Tensor #34 */
AI_TENSOR_OBJ_DECLARE(
  node_307_bias, AI_STATIC,
  34, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 1, 1), AI_STRIDE_INIT(4, 4, 4, 160, 160),
  1, &node_307_bias_array, NULL)

/* Tensor #35 */
AI_TENSOR_OBJ_DECLARE(
  node_307_output, AI_STATIC,
  35, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 40, 40), AI_STRIDE_INIT(4, 4, 4, 160, 6400),
  1, &node_307_output_array, NULL)

/* Tensor #36 */
AI_TENSOR_OBJ_DECLARE(
  node_307_weights, AI_STATIC,
  36, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 40), AI_STRIDE_INIT(4, 1, 40, 40, 40),
  1, &node_307_weights_array, NULL)

/* Tensor #37 */
AI_TENSOR_OBJ_DECLARE(
  node_309_output, AI_STATIC,
  37, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 40, 40), AI_STRIDE_INIT(4, 4, 4, 160, 6400),
  1, &node_309_output_array, NULL)

/* Tensor #38 */
AI_TENSOR_OBJ_DECLARE(
  node_310_bias, AI_STATIC,
  38, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 1, 1), AI_STRIDE_INIT(4, 4, 4, 288, 288),
  1, &node_310_bias_array, NULL)

/* Tensor #39 */
AI_TENSOR_OBJ_DECLARE(
  node_310_output, AI_STATIC,
  39, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 40, 40), AI_STRIDE_INIT(4, 4, 4, 288, 11520),
  1, &node_310_output_array, NULL)

/* Tensor #40 */
AI_TENSOR_OBJ_DECLARE(
  node_310_scratch0, AI_STATIC,
  40, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 1, 1), AI_STRIDE_INIT(4, 4, 4, 160, 160),
  1, &node_310_scratch0_array, NULL)

/* Tensor #41 */
AI_TENSOR_OBJ_DECLARE(
  node_310_weights, AI_STATIC,
  41, 0x0,
  AI_SHAPE_INIT(4, 40, 1, 1, 72), AI_STRIDE_INIT(4, 4, 160, 11520, 11520),
  1, &node_310_weights_array, NULL)

/* Tensor #42 */
AI_TENSOR_OBJ_DECLARE(
  node_312_output, AI_STATIC,
  42, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 40, 40), AI_STRIDE_INIT(4, 4, 4, 288, 11520),
  1, &node_312_output_array, NULL)

/* Tensor #43 */
AI_TENSOR_OBJ_DECLARE(
  node_313_bias, AI_STATIC,
  43, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 1, 1), AI_STRIDE_INIT(4, 4, 4, 288, 288),
  1, &node_313_bias_array, NULL)

/* Tensor #44 */
AI_TENSOR_OBJ_DECLARE(
  node_313_output, AI_STATIC,
  44, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 40, 40), AI_STRIDE_INIT(4, 4, 4, 288, 11520),
  1, &node_313_output_array, NULL)

/* Tensor #45 */
AI_TENSOR_OBJ_DECLARE(
  node_313_weights, AI_STATIC,
  45, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 72), AI_STRIDE_INIT(4, 1, 72, 72, 72),
  1, &node_313_weights_array, NULL)

/* Tensor #46 */
AI_TENSOR_OBJ_DECLARE(
  node_315_output, AI_STATIC,
  46, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 40, 40), AI_STRIDE_INIT(4, 4, 4, 288, 11520),
  1, &node_315_output_array, NULL)

/* Tensor #47 */
AI_TENSOR_OBJ_DECLARE(
  node_316_bias, AI_STATIC,
  47, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 1, 1), AI_STRIDE_INIT(4, 4, 4, 288, 288),
  1, &node_316_bias_array, NULL)

/* Tensor #48 */
AI_TENSOR_OBJ_DECLARE(
  node_316_output, AI_STATIC,
  48, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 40, 40), AI_STRIDE_INIT(4, 4, 4, 288, 11520),
  1, &node_316_output_array, NULL)

/* Tensor #49 */
AI_TENSOR_OBJ_DECLARE(
  node_316_scratch0, AI_STATIC,
  49, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 1, 1), AI_STRIDE_INIT(4, 4, 4, 288, 288),
  1, &node_316_scratch0_array, NULL)

/* Tensor #50 */
AI_TENSOR_OBJ_DECLARE(
  node_316_weights, AI_STATIC,
  50, 0x0,
  AI_SHAPE_INIT(4, 72, 1, 1, 72), AI_STRIDE_INIT(4, 4, 288, 20736, 20736),
  1, &node_316_weights_array, NULL)

/* Tensor #51 */
AI_TENSOR_OBJ_DECLARE(
  node_318_output, AI_STATIC,
  51, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 40, 40), AI_STRIDE_INIT(4, 4, 4, 288, 11520),
  1, &node_318_output_array, NULL)

/* Tensor #52 */
AI_TENSOR_OBJ_DECLARE(
  node_319_bias, AI_STATIC,
  52, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 1, 1), AI_STRIDE_INIT(4, 4, 4, 288, 288),
  1, &node_319_bias_array, NULL)

/* Tensor #53 */
AI_TENSOR_OBJ_DECLARE(
  node_319_output, AI_STATIC,
  53, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 40, 40), AI_STRIDE_INIT(4, 4, 4, 288, 11520),
  1, &node_319_output_array, NULL)

/* Tensor #54 */
AI_TENSOR_OBJ_DECLARE(
  node_319_weights, AI_STATIC,
  54, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 72), AI_STRIDE_INIT(4, 1, 72, 72, 72),
  1, &node_319_weights_array, NULL)

/* Tensor #55 */
AI_TENSOR_OBJ_DECLARE(
  node_321_output, AI_STATIC,
  55, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 40, 40), AI_STRIDE_INIT(4, 4, 4, 288, 11520),
  1, &node_321_output_array, NULL)

/* Tensor #56 */
AI_TENSOR_OBJ_DECLARE(
  node_322_bias, AI_STATIC,
  56, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 1, 1), AI_STRIDE_INIT(4, 4, 4, 288, 288),
  1, &node_322_bias_array, NULL)

/* Tensor #57 */
AI_TENSOR_OBJ_DECLARE(
  node_322_output, AI_STATIC,
  57, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 40, 40), AI_STRIDE_INIT(4, 4, 4, 288, 11520),
  1, &node_322_output_array, NULL)

/* Tensor #58 */
AI_TENSOR_OBJ_DECLARE(
  node_322_scratch0, AI_STATIC,
  58, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 1, 1), AI_STRIDE_INIT(4, 4, 4, 288, 288),
  1, &node_322_scratch0_array, NULL)

/* Tensor #59 */
AI_TENSOR_OBJ_DECLARE(
  node_322_weights, AI_STATIC,
  59, 0x0,
  AI_SHAPE_INIT(4, 72, 1, 1, 72), AI_STRIDE_INIT(4, 4, 288, 20736, 20736),
  1, &node_322_weights_array, NULL)

/* Tensor #60 */
AI_TENSOR_OBJ_DECLARE(
  node_324_output, AI_STATIC,
  60, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 40, 40), AI_STRIDE_INIT(4, 4, 4, 288, 11520),
  1, &node_324_output_array, NULL)

/* Tensor #61 */
AI_TENSOR_OBJ_DECLARE(
  node_325_bias, AI_STATIC,
  61, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 1, 1), AI_STRIDE_INIT(4, 4, 4, 288, 288),
  1, &node_325_bias_array, NULL)

/* Tensor #62 */
AI_TENSOR_OBJ_DECLARE(
  node_325_output, AI_STATIC,
  62, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 20, 20), AI_STRIDE_INIT(4, 4, 4, 288, 5760),
  1, &node_325_output_array, NULL)

/* Tensor #63 */
AI_TENSOR_OBJ_DECLARE(
  node_325_weights, AI_STATIC,
  63, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 72), AI_STRIDE_INIT(4, 1, 72, 72, 72),
  1, &node_325_weights_array, NULL)

/* Tensor #64 */
AI_TENSOR_OBJ_DECLARE(
  node_327_output, AI_STATIC,
  64, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 20, 20), AI_STRIDE_INIT(4, 4, 4, 288, 5760),
  1, &node_327_output_array, NULL)

/* Tensor #65 */
AI_TENSOR_OBJ_DECLARE(
  node_328_bias, AI_STATIC,
  65, 0x0,
  AI_SHAPE_INIT(4, 1, 152, 1, 1), AI_STRIDE_INIT(4, 4, 4, 608, 608),
  1, &node_328_bias_array, NULL)

/* Tensor #66 */
AI_TENSOR_OBJ_DECLARE(
  node_328_output, AI_STATIC,
  66, 0x0,
  AI_SHAPE_INIT(4, 1, 152, 20, 20), AI_STRIDE_INIT(4, 4, 4, 608, 12160),
  1, &node_328_output_array, NULL)

/* Tensor #67 */
AI_TENSOR_OBJ_DECLARE(
  node_328_scratch0, AI_STATIC,
  67, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 1, 1), AI_STRIDE_INIT(4, 4, 4, 288, 288),
  1, &node_328_scratch0_array, NULL)

/* Tensor #68 */
AI_TENSOR_OBJ_DECLARE(
  node_328_weights, AI_STATIC,
  68, 0x0,
  AI_SHAPE_INIT(4, 72, 1, 1, 152), AI_STRIDE_INIT(4, 4, 288, 43776, 43776),
  1, &node_328_weights_array, NULL)

/* Tensor #69 */
AI_TENSOR_OBJ_DECLARE(
  node_330_output, AI_STATIC,
  69, 0x0,
  AI_SHAPE_INIT(4, 1, 152, 20, 20), AI_STRIDE_INIT(4, 4, 4, 608, 12160),
  1, &node_330_output_array, NULL)

/* Tensor #70 */
AI_TENSOR_OBJ_DECLARE(
  node_331_bias, AI_STATIC,
  70, 0x0,
  AI_SHAPE_INIT(4, 1, 152, 1, 1), AI_STRIDE_INIT(4, 4, 4, 608, 608),
  1, &node_331_bias_array, NULL)

/* Tensor #71 */
AI_TENSOR_OBJ_DECLARE(
  node_331_output, AI_STATIC,
  71, 0x0,
  AI_SHAPE_INIT(4, 1, 152, 20, 20), AI_STRIDE_INIT(4, 4, 4, 608, 12160),
  1, &node_331_output_array, NULL)

/* Tensor #72 */
AI_TENSOR_OBJ_DECLARE(
  node_331_weights, AI_STATIC,
  72, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 152), AI_STRIDE_INIT(4, 1, 152, 152, 152),
  1, &node_331_weights_array, NULL)

/* Tensor #73 */
AI_TENSOR_OBJ_DECLARE(
  node_333_output, AI_STATIC,
  73, 0x0,
  AI_SHAPE_INIT(4, 1, 152, 20, 20), AI_STRIDE_INIT(4, 4, 4, 608, 12160),
  1, &node_333_output_array, NULL)

/* Tensor #74 */
AI_TENSOR_OBJ_DECLARE(
  node_334_bias, AI_STATIC,
  74, 0x0,
  AI_SHAPE_INIT(4, 1, 152, 1, 1), AI_STRIDE_INIT(4, 4, 4, 608, 608),
  1, &node_334_bias_array, NULL)

/* Tensor #75 */
AI_TENSOR_OBJ_DECLARE(
  node_334_output, AI_STATIC,
  75, 0x0,
  AI_SHAPE_INIT(4, 1, 152, 20, 20), AI_STRIDE_INIT(4, 4, 4, 608, 12160),
  1, &node_334_output_array, NULL)

/* Tensor #76 */
AI_TENSOR_OBJ_DECLARE(
  node_334_scratch0, AI_STATIC,
  76, 0x0,
  AI_SHAPE_INIT(4, 1, 152, 1, 1), AI_STRIDE_INIT(4, 4, 4, 608, 608),
  1, &node_334_scratch0_array, NULL)

/* Tensor #77 */
AI_TENSOR_OBJ_DECLARE(
  node_334_weights, AI_STATIC,
  77, 0x0,
  AI_SHAPE_INIT(4, 152, 1, 1, 152), AI_STRIDE_INIT(4, 4, 608, 92416, 92416),
  1, &node_334_weights_array, NULL)

/* Tensor #78 */
AI_TENSOR_OBJ_DECLARE(
  node_336_output, AI_STATIC,
  78, 0x0,
  AI_SHAPE_INIT(4, 1, 152, 20, 20), AI_STRIDE_INIT(4, 4, 4, 608, 12160),
  1, &node_336_output_array, NULL)

/* Tensor #79 */
AI_TENSOR_OBJ_DECLARE(
  node_337_bias, AI_STATIC,
  79, 0x0,
  AI_SHAPE_INIT(4, 1, 152, 1, 1), AI_STRIDE_INIT(4, 4, 4, 608, 608),
  1, &node_337_bias_array, NULL)

/* Tensor #80 */
AI_TENSOR_OBJ_DECLARE(
  node_337_output, AI_STATIC,
  80, 0x0,
  AI_SHAPE_INIT(4, 1, 152, 10, 10), AI_STRIDE_INIT(4, 4, 4, 608, 6080),
  1, &node_337_output_array, NULL)

/* Tensor #81 */
AI_TENSOR_OBJ_DECLARE(
  node_337_weights, AI_STATIC,
  81, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 152), AI_STRIDE_INIT(4, 1, 152, 152, 152),
  1, &node_337_weights_array, NULL)

/* Tensor #82 */
AI_TENSOR_OBJ_DECLARE(
  node_339_output, AI_STATIC,
  82, 0x0,
  AI_SHAPE_INIT(4, 1, 152, 10, 10), AI_STRIDE_INIT(4, 4, 4, 608, 6080),
  1, &node_339_output_array, NULL)

/* Tensor #83 */
AI_TENSOR_OBJ_DECLARE(
  node_340_bias, AI_STATIC,
  83, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &node_340_bias_array, NULL)

/* Tensor #84 */
AI_TENSOR_OBJ_DECLARE(
  node_340_output, AI_STATIC,
  84, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 10, 10), AI_STRIDE_INIT(4, 4, 4, 1152, 11520),
  1, &node_340_output_array, NULL)

/* Tensor #85 */
AI_TENSOR_OBJ_DECLARE(
  node_340_scratch0, AI_STATIC,
  85, 0x0,
  AI_SHAPE_INIT(4, 1, 152, 1, 1), AI_STRIDE_INIT(4, 4, 4, 608, 608),
  1, &node_340_scratch0_array, NULL)

/* Tensor #86 */
AI_TENSOR_OBJ_DECLARE(
  node_340_weights, AI_STATIC,
  86, 0x0,
  AI_SHAPE_INIT(4, 152, 1, 1, 288), AI_STRIDE_INIT(4, 4, 608, 175104, 175104),
  1, &node_340_weights_array, NULL)

/* Tensor #87 */
AI_TENSOR_OBJ_DECLARE(
  node_342_output, AI_STATIC,
  87, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 10, 10), AI_STRIDE_INIT(4, 4, 4, 1152, 11520),
  1, &node_342_output_array, NULL)

/* Tensor #88 */
AI_TENSOR_OBJ_DECLARE(
  node_343_bias, AI_STATIC,
  88, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &node_343_bias_array, NULL)

/* Tensor #89 */
AI_TENSOR_OBJ_DECLARE(
  node_343_output, AI_STATIC,
  89, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 10, 10), AI_STRIDE_INIT(4, 4, 4, 1152, 11520),
  1, &node_343_output_array, NULL)

/* Tensor #90 */
AI_TENSOR_OBJ_DECLARE(
  node_343_weights, AI_STATIC,
  90, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 288), AI_STRIDE_INIT(4, 1, 288, 288, 288),
  1, &node_343_weights_array, NULL)

/* Tensor #91 */
AI_TENSOR_OBJ_DECLARE(
  node_345_output, AI_STATIC,
  91, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 10, 10), AI_STRIDE_INIT(4, 4, 4, 1152, 11520),
  1, &node_345_output_array, NULL)

/* Tensor #92 */
AI_TENSOR_OBJ_DECLARE(
  node_346_bias, AI_STATIC,
  92, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &node_346_bias_array, NULL)

/* Tensor #93 */
AI_TENSOR_OBJ_DECLARE(
  node_346_output, AI_STATIC,
  93, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 10, 10), AI_STRIDE_INIT(4, 4, 4, 1152, 11520),
  1, &node_346_output_array, NULL)

/* Tensor #94 */
AI_TENSOR_OBJ_DECLARE(
  node_346_scratch0, AI_STATIC,
  94, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &node_346_scratch0_array, NULL)

/* Tensor #95 */
AI_TENSOR_OBJ_DECLARE(
  node_346_weights, AI_STATIC,
  95, 0x0,
  AI_SHAPE_INIT(4, 288, 1, 1, 288), AI_STRIDE_INIT(4, 4, 1152, 331776, 331776),
  1, &node_346_weights_array, NULL)

/* Tensor #96 */
AI_TENSOR_OBJ_DECLARE(
  node_348_output, AI_STATIC,
  96, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 10, 10), AI_STRIDE_INIT(4, 4, 4, 1152, 11520),
  1, &node_348_output_array, NULL)

/* Tensor #97 */
AI_TENSOR_OBJ_DECLARE(
  node_349_bias, AI_STATIC,
  97, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &node_349_bias_array, NULL)

/* Tensor #98 */
AI_TENSOR_OBJ_DECLARE(
  node_349_output, AI_STATIC,
  98, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 10, 10), AI_STRIDE_INIT(4, 4, 4, 1152, 11520),
  1, &node_349_output_array, NULL)

/* Tensor #99 */
AI_TENSOR_OBJ_DECLARE(
  node_349_weights, AI_STATIC,
  99, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 288), AI_STRIDE_INIT(4, 1, 288, 288, 288),
  1, &node_349_weights_array, NULL)

/* Tensor #100 */
AI_TENSOR_OBJ_DECLARE(
  node_351_output, AI_STATIC,
  100, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 10, 10), AI_STRIDE_INIT(4, 4, 4, 1152, 11520),
  1, &node_351_output_array, NULL)

/* Tensor #101 */
AI_TENSOR_OBJ_DECLARE(
  node_352_bias, AI_STATIC,
  101, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &node_352_bias_array, NULL)

/* Tensor #102 */
AI_TENSOR_OBJ_DECLARE(
  node_352_output, AI_STATIC,
  102, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 10, 10), AI_STRIDE_INIT(4, 4, 4, 1152, 11520),
  1, &node_352_output_array, NULL)

/* Tensor #103 */
AI_TENSOR_OBJ_DECLARE(
  node_352_scratch0, AI_STATIC,
  103, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &node_352_scratch0_array, NULL)

/* Tensor #104 */
AI_TENSOR_OBJ_DECLARE(
  node_352_weights, AI_STATIC,
  104, 0x0,
  AI_SHAPE_INIT(4, 288, 1, 1, 288), AI_STRIDE_INIT(4, 4, 1152, 331776, 331776),
  1, &node_352_weights_array, NULL)

/* Tensor #105 */
AI_TENSOR_OBJ_DECLARE(
  node_354_output, AI_STATIC,
  105, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 10, 10), AI_STRIDE_INIT(4, 4, 4, 1152, 11520),
  1, &node_354_output_array, NULL)

/* Tensor #106 */
AI_TENSOR_OBJ_DECLARE(
  node_355_bias, AI_STATIC,
  106, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &node_355_bias_array, NULL)

/* Tensor #107 */
AI_TENSOR_OBJ_DECLARE(
  node_355_output, AI_STATIC,
  107, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 10, 10), AI_STRIDE_INIT(4, 4, 4, 1152, 11520),
  1, &node_355_output_array, NULL)

/* Tensor #108 */
AI_TENSOR_OBJ_DECLARE(
  node_355_weights, AI_STATIC,
  108, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 288), AI_STRIDE_INIT(4, 1, 288, 288, 288),
  1, &node_355_weights_array, NULL)

/* Tensor #109 */
AI_TENSOR_OBJ_DECLARE(
  node_357_output, AI_STATIC,
  109, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 10, 10), AI_STRIDE_INIT(4, 4, 4, 1152, 11520),
  1, &node_357_output_array, NULL)

/* Tensor #110 */
AI_TENSOR_OBJ_DECLARE(
  node_358_bias, AI_STATIC,
  110, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &node_358_bias_array, NULL)

/* Tensor #111 */
AI_TENSOR_OBJ_DECLARE(
  node_358_output, AI_STATIC,
  111, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 10, 10), AI_STRIDE_INIT(4, 4, 4, 1152, 11520),
  1, &node_358_output_array, NULL)

/* Tensor #112 */
AI_TENSOR_OBJ_DECLARE(
  node_358_scratch0, AI_STATIC,
  112, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &node_358_scratch0_array, NULL)

/* Tensor #113 */
AI_TENSOR_OBJ_DECLARE(
  node_358_weights, AI_STATIC,
  113, 0x0,
  AI_SHAPE_INIT(4, 288, 1, 1, 288), AI_STRIDE_INIT(4, 4, 1152, 331776, 331776),
  1, &node_358_weights_array, NULL)

/* Tensor #114 */
AI_TENSOR_OBJ_DECLARE(
  node_360_output, AI_STATIC,
  114, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 10, 10), AI_STRIDE_INIT(4, 4, 4, 1152, 11520),
  1, &node_360_output_array, NULL)

/* Tensor #115 */
AI_TENSOR_OBJ_DECLARE(
  node_361_bias, AI_STATIC,
  115, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &node_361_bias_array, NULL)

/* Tensor #116 */
AI_TENSOR_OBJ_DECLARE(
  node_361_output, AI_STATIC,
  116, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 10, 10), AI_STRIDE_INIT(4, 4, 4, 1152, 11520),
  1, &node_361_output_array, NULL)

/* Tensor #117 */
AI_TENSOR_OBJ_DECLARE(
  node_361_weights, AI_STATIC,
  117, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 288), AI_STRIDE_INIT(4, 1, 288, 288, 288),
  1, &node_361_weights_array, NULL)

/* Tensor #118 */
AI_TENSOR_OBJ_DECLARE(
  node_363_output, AI_STATIC,
  118, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 10, 10), AI_STRIDE_INIT(4, 4, 4, 1152, 11520),
  1, &node_363_output_array, NULL)

/* Tensor #119 */
AI_TENSOR_OBJ_DECLARE(
  node_364_bias, AI_STATIC,
  119, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &node_364_bias_array, NULL)

/* Tensor #120 */
AI_TENSOR_OBJ_DECLARE(
  node_364_output, AI_STATIC,
  120, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 10, 10), AI_STRIDE_INIT(4, 4, 4, 1152, 11520),
  1, &node_364_output_array, NULL)

/* Tensor #121 */
AI_TENSOR_OBJ_DECLARE(
  node_364_scratch0, AI_STATIC,
  121, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &node_364_scratch0_array, NULL)

/* Tensor #122 */
AI_TENSOR_OBJ_DECLARE(
  node_364_weights, AI_STATIC,
  122, 0x0,
  AI_SHAPE_INIT(4, 288, 1, 1, 288), AI_STRIDE_INIT(4, 4, 1152, 331776, 331776),
  1, &node_364_weights_array, NULL)

/* Tensor #123 */
AI_TENSOR_OBJ_DECLARE(
  node_366_output, AI_STATIC,
  123, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 10, 10), AI_STRIDE_INIT(4, 4, 4, 1152, 11520),
  1, &node_366_output_array, NULL)

/* Tensor #124 */
AI_TENSOR_OBJ_DECLARE(
  node_367_bias, AI_STATIC,
  124, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &node_367_bias_array, NULL)

/* Tensor #125 */
AI_TENSOR_OBJ_DECLARE(
  node_367_output, AI_STATIC,
  125, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 10, 10), AI_STRIDE_INIT(4, 4, 4, 1152, 11520),
  1, &node_367_output_array, NULL)

/* Tensor #126 */
AI_TENSOR_OBJ_DECLARE(
  node_367_weights, AI_STATIC,
  126, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 288), AI_STRIDE_INIT(4, 1, 288, 288, 288),
  1, &node_367_weights_array, NULL)

/* Tensor #127 */
AI_TENSOR_OBJ_DECLARE(
  node_369_output, AI_STATIC,
  127, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 10, 10), AI_STRIDE_INIT(4, 4, 4, 1152, 11520),
  1, &node_369_output_array, NULL)

/* Tensor #128 */
AI_TENSOR_OBJ_DECLARE(
  node_370_bias, AI_STATIC,
  128, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &node_370_bias_array, NULL)

/* Tensor #129 */
AI_TENSOR_OBJ_DECLARE(
  node_370_output, AI_STATIC,
  129, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 10, 10), AI_STRIDE_INIT(4, 4, 4, 1152, 11520),
  1, &node_370_output_array, NULL)

/* Tensor #130 */
AI_TENSOR_OBJ_DECLARE(
  node_370_scratch0, AI_STATIC,
  130, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &node_370_scratch0_array, NULL)

/* Tensor #131 */
AI_TENSOR_OBJ_DECLARE(
  node_370_weights, AI_STATIC,
  131, 0x0,
  AI_SHAPE_INIT(4, 288, 1, 1, 288), AI_STRIDE_INIT(4, 4, 1152, 331776, 331776),
  1, &node_370_weights_array, NULL)

/* Tensor #132 */
AI_TENSOR_OBJ_DECLARE(
  node_372_output, AI_STATIC,
  132, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 10, 10), AI_STRIDE_INIT(4, 4, 4, 1152, 11520),
  1, &node_372_output_array, NULL)

/* Tensor #133 */
AI_TENSOR_OBJ_DECLARE(
  node_373_bias, AI_STATIC,
  133, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &node_373_bias_array, NULL)

/* Tensor #134 */
AI_TENSOR_OBJ_DECLARE(
  node_373_output, AI_STATIC,
  134, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 40, 40), AI_STRIDE_INIT(4, 4, 4, 64, 2560),
  1, &node_373_output_array, NULL)

/* Tensor #135 */
AI_TENSOR_OBJ_DECLARE(
  node_373_scratch0, AI_STATIC,
  135, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 1, 1), AI_STRIDE_INIT(4, 4, 4, 288, 288),
  1, &node_373_scratch0_array, NULL)

/* Tensor #136 */
AI_TENSOR_OBJ_DECLARE(
  node_373_weights, AI_STATIC,
  136, 0x0,
  AI_SHAPE_INIT(4, 72, 1, 1, 16), AI_STRIDE_INIT(4, 4, 288, 4608, 4608),
  1, &node_373_weights_array, NULL)

/* Tensor #137 */
AI_TENSOR_OBJ_DECLARE(
  node_374_bias, AI_STATIC,
  137, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &node_374_bias_array, NULL)

/* Tensor #138 */
AI_TENSOR_OBJ_DECLARE(
  node_374_output, AI_STATIC,
  138, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 20, 20), AI_STRIDE_INIT(4, 4, 4, 64, 1280),
  1, &node_374_output_array, NULL)

/* Tensor #139 */
AI_TENSOR_OBJ_DECLARE(
  node_374_scratch0, AI_STATIC,
  139, 0x0,
  AI_SHAPE_INIT(4, 1, 152, 1, 1), AI_STRIDE_INIT(4, 4, 4, 608, 608),
  1, &node_374_scratch0_array, NULL)

/* Tensor #140 */
AI_TENSOR_OBJ_DECLARE(
  node_374_weights, AI_STATIC,
  140, 0x0,
  AI_SHAPE_INIT(4, 152, 1, 1, 16), AI_STRIDE_INIT(4, 4, 608, 9728, 9728),
  1, &node_374_weights_array, NULL)

/* Tensor #141 */
AI_TENSOR_OBJ_DECLARE(
  node_375_bias, AI_STATIC,
  141, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &node_375_bias_array, NULL)

/* Tensor #142 */
AI_TENSOR_OBJ_DECLARE(
  node_375_output, AI_STATIC,
  142, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 10, 10), AI_STRIDE_INIT(4, 4, 4, 64, 640),
  1, &node_375_output_array, NULL)

/* Tensor #143 */
AI_TENSOR_OBJ_DECLARE(
  node_375_scratch0, AI_STATIC,
  143, 0x0,
  AI_SHAPE_INIT(4, 1, 288, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1152, 1152),
  1, &node_375_scratch0_array, NULL)

/* Tensor #144 */
AI_TENSOR_OBJ_DECLARE(
  node_375_weights, AI_STATIC,
  144, 0x0,
  AI_SHAPE_INIT(4, 288, 1, 1, 16), AI_STRIDE_INIT(4, 4, 1152, 18432, 18432),
  1, &node_375_weights_array, NULL)

/* Tensor #145 */
AI_TENSOR_OBJ_DECLARE(
  node_394_output, AI_STATIC,
  145, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 20, 20), AI_STRIDE_INIT(4, 4, 4, 64, 1280),
  1, &node_394_output_array, NULL)

/* Tensor #146 */
AI_TENSOR_OBJ_DECLARE(
  node_395_output, AI_STATIC,
  146, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 20, 20), AI_STRIDE_INIT(4, 4, 4, 64, 1280),
  1, &node_395_output_array, NULL)

/* Tensor #147 */
AI_TENSOR_OBJ_DECLARE(
  node_414_output, AI_STATIC,
  147, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 40, 40), AI_STRIDE_INIT(4, 4, 4, 64, 2560),
  1, &node_414_output_array, NULL)

/* Tensor #148 */
AI_TENSOR_OBJ_DECLARE(
  node_415_output, AI_STATIC,
  148, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 40, 40), AI_STRIDE_INIT(4, 4, 4, 64, 2560),
  1, &node_415_output_array, NULL)

/* Tensor #149 */
AI_TENSOR_OBJ_DECLARE(
  node_416_bias, AI_STATIC,
  149, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &node_416_bias_array, NULL)

/* Tensor #150 */
AI_TENSOR_OBJ_DECLARE(
  node_416_output, AI_STATIC,
  150, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 40, 40), AI_STRIDE_INIT(4, 4, 4, 64, 2560),
  1, &node_416_output_array, NULL)

/* Tensor #151 */
AI_TENSOR_OBJ_DECLARE(
  node_416_scratch0, AI_STATIC,
  151, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 3, 3), AI_STRIDE_INIT(4, 4, 4, 64, 192),
  1, &node_416_scratch0_array, NULL)

/* Tensor #152 */
AI_TENSOR_OBJ_DECLARE(
  node_416_weights, AI_STATIC,
  152, 0x0,
  AI_SHAPE_INIT(4, 16, 3, 3, 16), AI_STRIDE_INIT(4, 4, 64, 1024, 3072),
  1, &node_416_weights_array, NULL)

/* Tensor #153 */
AI_TENSOR_OBJ_DECLARE(
  node_417_bias, AI_STATIC,
  153, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &node_417_bias_array, NULL)

/* Tensor #154 */
AI_TENSOR_OBJ_DECLARE(
  node_417_output, AI_STATIC,
  154, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 20, 20), AI_STRIDE_INIT(4, 4, 4, 64, 1280),
  1, &node_417_output_array, NULL)

/* Tensor #155 */
AI_TENSOR_OBJ_DECLARE(
  node_417_scratch0, AI_STATIC,
  155, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 3, 3), AI_STRIDE_INIT(4, 4, 4, 64, 192),
  1, &node_417_scratch0_array, NULL)

/* Tensor #156 */
AI_TENSOR_OBJ_DECLARE(
  node_417_weights, AI_STATIC,
  156, 0x0,
  AI_SHAPE_INIT(4, 16, 3, 3, 16), AI_STRIDE_INIT(4, 4, 64, 1024, 3072),
  1, &node_417_weights_array, NULL)

/* Tensor #157 */
AI_TENSOR_OBJ_DECLARE(
  node_418_bias, AI_STATIC,
  157, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &node_418_bias_array, NULL)

/* Tensor #158 */
AI_TENSOR_OBJ_DECLARE(
  node_418_output, AI_STATIC,
  158, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 10, 10), AI_STRIDE_INIT(4, 4, 4, 64, 640),
  1, &node_418_output_array, NULL)

/* Tensor #159 */
AI_TENSOR_OBJ_DECLARE(
  node_418_scratch0, AI_STATIC,
  159, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 3, 3), AI_STRIDE_INIT(4, 4, 4, 64, 192),
  1, &node_418_scratch0_array, NULL)

/* Tensor #160 */
AI_TENSOR_OBJ_DECLARE(
  node_418_weights, AI_STATIC,
  160, 0x0,
  AI_SHAPE_INIT(4, 16, 3, 3, 16), AI_STRIDE_INIT(4, 4, 64, 1024, 3072),
  1, &node_418_weights_array, NULL)

/* Tensor #161 */
AI_TENSOR_OBJ_DECLARE(
  node_419_output, AI_STATIC,
  161, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 20, 20), AI_STRIDE_INIT(4, 4, 4, 64, 1280),
  1, &node_419_output_array, NULL)

/* Tensor #162 */
AI_TENSOR_OBJ_DECLARE(
  node_419_scratch0, AI_STATIC,
  162, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 3, 3), AI_STRIDE_INIT(4, 4, 4, 64, 192),
  1, &node_419_scratch0_array, NULL)

/* Tensor #163 */
AI_TENSOR_OBJ_DECLARE(
  node_419_weights, AI_STATIC,
  163, 0x0,
  AI_SHAPE_INIT(4, 16, 3, 3, 16), AI_STRIDE_INIT(4, 4, 64, 1024, 3072),
  1, &node_419_weights_array, NULL)

/* Tensor #164 */
AI_TENSOR_OBJ_DECLARE(
  node_420_output, AI_STATIC,
  164, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 20, 20), AI_STRIDE_INIT(4, 4, 4, 64, 1280),
  1, &node_420_output_array, NULL)

/* Tensor #165 */
AI_TENSOR_OBJ_DECLARE(
  node_421_output, AI_STATIC,
  165, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 10, 10), AI_STRIDE_INIT(4, 4, 4, 64, 640),
  1, &node_421_output_array, NULL)

/* Tensor #166 */
AI_TENSOR_OBJ_DECLARE(
  node_421_scratch0, AI_STATIC,
  166, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 3, 3), AI_STRIDE_INIT(4, 4, 4, 64, 192),
  1, &node_421_scratch0_array, NULL)

/* Tensor #167 */
AI_TENSOR_OBJ_DECLARE(
  node_421_weights, AI_STATIC,
  167, 0x0,
  AI_SHAPE_INIT(4, 16, 3, 3, 16), AI_STRIDE_INIT(4, 4, 64, 1024, 3072),
  1, &node_421_weights_array, NULL)

/* Tensor #168 */
AI_TENSOR_OBJ_DECLARE(
  node_422_output, AI_STATIC,
  168, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 10, 10), AI_STRIDE_INIT(4, 4, 4, 64, 640),
  1, &node_422_output_array, NULL)

/* Tensor #169 */
AI_TENSOR_OBJ_DECLARE(
  node_423_bias, AI_STATIC,
  169, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &node_423_bias_array, NULL)

/* Tensor #170 */
AI_TENSOR_OBJ_DECLARE(
  node_423_output, AI_STATIC,
  170, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 20, 20), AI_STRIDE_INIT(4, 4, 4, 64, 1280),
  1, &node_423_output_array, NULL)

/* Tensor #171 */
AI_TENSOR_OBJ_DECLARE(
  node_423_scratch0, AI_STATIC,
  171, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 3, 3), AI_STRIDE_INIT(4, 4, 4, 64, 192),
  1, &node_423_scratch0_array, NULL)

/* Tensor #172 */
AI_TENSOR_OBJ_DECLARE(
  node_423_weights, AI_STATIC,
  172, 0x0,
  AI_SHAPE_INIT(4, 16, 3, 3, 16), AI_STRIDE_INIT(4, 4, 64, 1024, 3072),
  1, &node_423_weights_array, NULL)

/* Tensor #173 */
AI_TENSOR_OBJ_DECLARE(
  node_424_bias, AI_STATIC,
  173, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &node_424_bias_array, NULL)

/* Tensor #174 */
AI_TENSOR_OBJ_DECLARE(
  node_424_output, AI_STATIC,
  174, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 10, 10), AI_STRIDE_INIT(4, 4, 4, 64, 640),
  1, &node_424_output_array, NULL)

/* Tensor #175 */
AI_TENSOR_OBJ_DECLARE(
  node_424_scratch0, AI_STATIC,
  175, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 3, 3), AI_STRIDE_INIT(4, 4, 4, 64, 192),
  1, &node_424_scratch0_array, NULL)

/* Tensor #176 */
AI_TENSOR_OBJ_DECLARE(
  node_424_weights, AI_STATIC,
  176, 0x0,
  AI_SHAPE_INIT(4, 16, 3, 3, 16), AI_STRIDE_INIT(4, 4, 64, 1024, 3072),
  1, &node_424_weights_array, NULL)

/* Tensor #177 */
AI_TENSOR_OBJ_DECLARE(
  node_425_bias, AI_STATIC,
  177, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &node_425_bias_array, NULL)

/* Tensor #178 */
AI_TENSOR_OBJ_DECLARE(
  node_425_output, AI_STATIC,
  178, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 40, 40), AI_STRIDE_INIT(4, 4, 4, 64, 2560),
  1, &node_425_output_array, NULL)

/* Tensor #179 */
AI_TENSOR_OBJ_DECLARE(
  node_425_weights, AI_STATIC,
  179, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 16), AI_STRIDE_INIT(4, 1, 16, 16, 16),
  1, &node_425_weights_array, NULL)

/* Tensor #180 */
AI_TENSOR_OBJ_DECLARE(
  node_427_output, AI_STATIC,
  180, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 40, 40), AI_STRIDE_INIT(4, 4, 4, 64, 2560),
  1, &node_427_output_array, NULL)

/* Tensor #181 */
AI_TENSOR_OBJ_DECLARE(
  node_428_bias, AI_STATIC,
  181, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &node_428_bias_array, NULL)

/* Tensor #182 */
AI_TENSOR_OBJ_DECLARE(
  node_428_output, AI_STATIC,
  182, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 40, 40), AI_STRIDE_INIT(4, 4, 4, 256, 10240),
  1, &node_428_output_array, NULL)

/* Tensor #183 */
AI_TENSOR_OBJ_DECLARE(
  node_428_scratch0, AI_STATIC,
  183, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &node_428_scratch0_array, NULL)

/* Tensor #184 */
AI_TENSOR_OBJ_DECLARE(
  node_428_weights, AI_STATIC,
  184, 0x0,
  AI_SHAPE_INIT(4, 16, 1, 1, 64), AI_STRIDE_INIT(4, 4, 64, 4096, 4096),
  1, &node_428_weights_array, NULL)

/* Tensor #185 */
AI_TENSOR_OBJ_DECLARE(
  node_430_output, AI_STATIC,
  185, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 40, 40), AI_STRIDE_INIT(4, 4, 4, 256, 10240),
  1, &node_430_output_array, NULL)

/* Tensor #186 */
AI_TENSOR_OBJ_DECLARE(
  node_431_bias, AI_STATIC,
  186, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &node_431_bias_array, NULL)

/* Tensor #187 */
AI_TENSOR_OBJ_DECLARE(
  node_431_output, AI_STATIC,
  187, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 40, 40), AI_STRIDE_INIT(4, 4, 4, 256, 10240),
  1, &node_431_output_array, NULL)

/* Tensor #188 */
AI_TENSOR_OBJ_DECLARE(
  node_431_weights, AI_STATIC,
  188, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 64), AI_STRIDE_INIT(4, 1, 64, 64, 64),
  1, &node_431_weights_array, NULL)

/* Tensor #189 */
AI_TENSOR_OBJ_DECLARE(
  node_433_output, AI_STATIC,
  189, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 40, 40), AI_STRIDE_INIT(4, 4, 4, 256, 10240),
  1, &node_433_output_array, NULL)

/* Tensor #190 */
AI_TENSOR_OBJ_DECLARE(
  node_434_bias, AI_STATIC,
  190, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &node_434_bias_array, NULL)

/* Tensor #191 */
AI_TENSOR_OBJ_DECLARE(
  node_434_output, AI_STATIC,
  191, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 40, 40), AI_STRIDE_INIT(4, 4, 4, 256, 10240),
  1, &node_434_output_array, NULL)

/* Tensor #192 */
AI_TENSOR_OBJ_DECLARE(
  node_434_scratch0, AI_STATIC,
  192, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &node_434_scratch0_array, NULL)

/* Tensor #193 */
AI_TENSOR_OBJ_DECLARE(
  node_434_weights, AI_STATIC,
  193, 0x0,
  AI_SHAPE_INIT(4, 64, 1, 1, 64), AI_STRIDE_INIT(4, 4, 256, 16384, 16384),
  1, &node_434_weights_array, NULL)

/* Tensor #194 */
AI_TENSOR_OBJ_DECLARE(
  node_436_output, AI_STATIC,
  194, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 40, 40), AI_STRIDE_INIT(4, 4, 4, 256, 10240),
  1, &node_436_output_array, NULL)

/* Tensor #195 */
AI_TENSOR_OBJ_DECLARE(
  node_437_bias, AI_STATIC,
  195, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &node_437_bias_array, NULL)

/* Tensor #196 */
AI_TENSOR_OBJ_DECLARE(
  node_437_output, AI_STATIC,
  196, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 40, 40), AI_STRIDE_INIT(4, 4, 4, 8, 320),
  1, &node_437_output_array, NULL)

/* Tensor #197 */
AI_TENSOR_OBJ_DECLARE(
  node_437_scratch0, AI_STATIC,
  197, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 3, 3), AI_STRIDE_INIT(4, 4, 4, 256, 768),
  1, &node_437_scratch0_array, NULL)

/* Tensor #198 */
AI_TENSOR_OBJ_DECLARE(
  node_437_weights, AI_STATIC,
  198, 0x0,
  AI_SHAPE_INIT(4, 64, 3, 3, 2), AI_STRIDE_INIT(4, 4, 256, 512, 1536),
  1, &node_437_weights_array, NULL)

/* Tensor #199 */
AI_TENSOR_OBJ_DECLARE(
  node_438_bias, AI_STATIC,
  199, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &node_438_bias_array, NULL)

/* Tensor #200 */
AI_TENSOR_OBJ_DECLARE(
  node_438_output, AI_STATIC,
  200, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 40, 40), AI_STRIDE_INIT(4, 4, 4, 32, 1280),
  1, &node_438_output_array, NULL)

/* Tensor #201 */
AI_TENSOR_OBJ_DECLARE(
  node_438_scratch0, AI_STATIC,
  201, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 3, 3), AI_STRIDE_INIT(4, 4, 4, 256, 768),
  1, &node_438_scratch0_array, NULL)

/* Tensor #202 */
AI_TENSOR_OBJ_DECLARE(
  node_438_weights, AI_STATIC,
  202, 0x0,
  AI_SHAPE_INIT(4, 64, 3, 3, 8), AI_STRIDE_INIT(4, 4, 256, 2048, 6144),
  1, &node_438_weights_array, NULL)

/* Tensor #203 */
AI_TENSOR_OBJ_DECLARE(
  node_439_bias, AI_STATIC,
  203, 0x0,
  AI_SHAPE_INIT(4, 1, 20, 1, 1), AI_STRIDE_INIT(4, 4, 4, 80, 80),
  1, &node_439_bias_array, NULL)

/* Tensor #204 */
AI_TENSOR_OBJ_DECLARE(
  node_439_output, AI_STATIC,
  204, 0x0,
  AI_SHAPE_INIT(4, 1, 20, 40, 40), AI_STRIDE_INIT(4, 4, 4, 80, 3200),
  1, &node_439_output_array, NULL)

/* Tensor #205 */
AI_TENSOR_OBJ_DECLARE(
  node_439_scratch0, AI_STATIC,
  205, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 3, 3), AI_STRIDE_INIT(4, 4, 4, 256, 768),
  1, &node_439_scratch0_array, NULL)

/* Tensor #206 */
AI_TENSOR_OBJ_DECLARE(
  node_439_weights, AI_STATIC,
  206, 0x0,
  AI_SHAPE_INIT(4, 64, 3, 3, 20), AI_STRIDE_INIT(4, 4, 256, 5120, 15360),
  1, &node_439_weights_array, NULL)

/* Tensor #207 */
AI_TENSOR_OBJ_DECLARE(
  node_440_reshape_in_084_to_chlast_output, AI_STATIC,
  207, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 40, 2), AI_STRIDE_INIT(4, 4, 4, 160, 6400),
  1, &node_440_reshape_in_084_to_chlast_output_array, NULL)

/* Tensor #208 */
AI_TENSOR_OBJ_DECLARE(
  node_440_reshape_in_084_to_chlast_output0, AI_STATIC,
  208, 0x0,
  AI_SHAPE_INIT(5, 1, 40, 2, 1, 40), AI_STRIDE_INIT(5, 4, 4, 6400, 12800, 160),
  1, &node_440_reshape_in_084_to_chlast_output_array, NULL)

/* Tensor #209 */
AI_TENSOR_OBJ_DECLARE(
  node_442_to_chlast_output, AI_STATIC,
  209, 0x0,
  AI_SHAPE_INIT(5, 1, 2, 40, 40, 1), AI_STRIDE_INIT(5, 4, 4, 8, 320, 8),
  1, &node_442_to_chlast_output_array, NULL)

/* Tensor #210 */
AI_TENSOR_OBJ_DECLARE(
  node_442_to_chlast_output0, AI_STATIC,
  210, 0x0,
  AI_SHAPE_INIT(4, 1, 3200, 1, 1), AI_STRIDE_INIT(4, 4, 4, 12800, 12800),
  1, &node_442_to_chlast_output_array, NULL)

/* Tensor #211 */
AI_TENSOR_OBJ_DECLARE(
  node_443_output, AI_STATIC,
  211, 0x0,
  AI_SHAPE_INIT(4, 1, 3200, 1, 1), AI_STRIDE_INIT(4, 4, 4, 12800, 12800),
  1, &node_443_output_array, NULL)

/* Tensor #212 */
AI_TENSOR_OBJ_DECLARE(
  node_444_reshape_in_081_to_chlast_output, AI_STATIC,
  212, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 40, 8), AI_STRIDE_INIT(4, 4, 4, 160, 6400),
  1, &node_444_reshape_in_081_to_chlast_output_array, NULL)

/* Tensor #213 */
AI_TENSOR_OBJ_DECLARE(
  node_444_reshape_in_081_to_chlast_output0, AI_STATIC,
  213, 0x0,
  AI_SHAPE_INIT(5, 1, 40, 8, 1, 40), AI_STRIDE_INIT(5, 4, 4, 6400, 51200, 160),
  1, &node_444_reshape_in_081_to_chlast_output_array, NULL)

/* Tensor #214 */
AI_TENSOR_OBJ_DECLARE(
  node_446_to_chlast_output, AI_STATIC,
  214, 0x0,
  AI_SHAPE_INIT(5, 1, 8, 40, 40, 1), AI_STRIDE_INIT(5, 4, 4, 32, 1280, 32),
  1, &node_446_to_chlast_output_array, NULL)

/* Tensor #215 */
AI_TENSOR_OBJ_DECLARE(
  node_447_reshape_in_078_to_chlast_output, AI_STATIC,
  215, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 40, 20), AI_STRIDE_INIT(4, 4, 4, 160, 6400),
  1, &node_447_reshape_in_078_to_chlast_output_array, NULL)

/* Tensor #216 */
AI_TENSOR_OBJ_DECLARE(
  node_447_reshape_in_078_to_chlast_output0, AI_STATIC,
  216, 0x0,
  AI_SHAPE_INIT(5, 1, 40, 20, 1, 40), AI_STRIDE_INIT(5, 4, 4, 6400, 128000, 160),
  1, &node_447_reshape_in_078_to_chlast_output_array, NULL)

/* Tensor #217 */
AI_TENSOR_OBJ_DECLARE(
  node_449_to_chlast_output, AI_STATIC,
  217, 0x0,
  AI_SHAPE_INIT(5, 1, 20, 40, 40, 1), AI_STRIDE_INIT(5, 4, 4, 80, 3200, 80),
  1, &node_449_to_chlast_output_array, NULL)

/* Tensor #218 */
AI_TENSOR_OBJ_DECLARE(
  node_450_bias, AI_STATIC,
  218, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &node_450_bias_array, NULL)

/* Tensor #219 */
AI_TENSOR_OBJ_DECLARE(
  node_450_output, AI_STATIC,
  219, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 20, 20), AI_STRIDE_INIT(4, 4, 4, 64, 1280),
  1, &node_450_output_array, NULL)

/* Tensor #220 */
AI_TENSOR_OBJ_DECLARE(
  node_450_weights, AI_STATIC,
  220, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 16), AI_STRIDE_INIT(4, 1, 16, 16, 16),
  1, &node_450_weights_array, NULL)

/* Tensor #221 */
AI_TENSOR_OBJ_DECLARE(
  node_452_output, AI_STATIC,
  221, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 20, 20), AI_STRIDE_INIT(4, 4, 4, 64, 1280),
  1, &node_452_output_array, NULL)

/* Tensor #222 */
AI_TENSOR_OBJ_DECLARE(
  node_453_bias, AI_STATIC,
  222, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &node_453_bias_array, NULL)

/* Tensor #223 */
AI_TENSOR_OBJ_DECLARE(
  node_453_output, AI_STATIC,
  223, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 20, 20), AI_STRIDE_INIT(4, 4, 4, 256, 5120),
  1, &node_453_output_array, NULL)

/* Tensor #224 */
AI_TENSOR_OBJ_DECLARE(
  node_453_scratch0, AI_STATIC,
  224, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &node_453_scratch0_array, NULL)

/* Tensor #225 */
AI_TENSOR_OBJ_DECLARE(
  node_453_weights, AI_STATIC,
  225, 0x0,
  AI_SHAPE_INIT(4, 16, 1, 1, 64), AI_STRIDE_INIT(4, 4, 64, 4096, 4096),
  1, &node_453_weights_array, NULL)

/* Tensor #226 */
AI_TENSOR_OBJ_DECLARE(
  node_455_output, AI_STATIC,
  226, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 20, 20), AI_STRIDE_INIT(4, 4, 4, 256, 5120),
  1, &node_455_output_array, NULL)

/* Tensor #227 */
AI_TENSOR_OBJ_DECLARE(
  node_456_bias, AI_STATIC,
  227, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &node_456_bias_array, NULL)

/* Tensor #228 */
AI_TENSOR_OBJ_DECLARE(
  node_456_output, AI_STATIC,
  228, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 20, 20), AI_STRIDE_INIT(4, 4, 4, 256, 5120),
  1, &node_456_output_array, NULL)

/* Tensor #229 */
AI_TENSOR_OBJ_DECLARE(
  node_456_weights, AI_STATIC,
  229, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 64), AI_STRIDE_INIT(4, 1, 64, 64, 64),
  1, &node_456_weights_array, NULL)

/* Tensor #230 */
AI_TENSOR_OBJ_DECLARE(
  node_458_output, AI_STATIC,
  230, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 20, 20), AI_STRIDE_INIT(4, 4, 4, 256, 5120),
  1, &node_458_output_array, NULL)

/* Tensor #231 */
AI_TENSOR_OBJ_DECLARE(
  node_459_bias, AI_STATIC,
  231, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &node_459_bias_array, NULL)

/* Tensor #232 */
AI_TENSOR_OBJ_DECLARE(
  node_459_output, AI_STATIC,
  232, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 20, 20), AI_STRIDE_INIT(4, 4, 4, 256, 5120),
  1, &node_459_output_array, NULL)

/* Tensor #233 */
AI_TENSOR_OBJ_DECLARE(
  node_459_scratch0, AI_STATIC,
  233, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &node_459_scratch0_array, NULL)

/* Tensor #234 */
AI_TENSOR_OBJ_DECLARE(
  node_459_weights, AI_STATIC,
  234, 0x0,
  AI_SHAPE_INIT(4, 64, 1, 1, 64), AI_STRIDE_INIT(4, 4, 256, 16384, 16384),
  1, &node_459_weights_array, NULL)

/* Tensor #235 */
AI_TENSOR_OBJ_DECLARE(
  node_461_output, AI_STATIC,
  235, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 20, 20), AI_STRIDE_INIT(4, 4, 4, 256, 5120),
  1, &node_461_output_array, NULL)

/* Tensor #236 */
AI_TENSOR_OBJ_DECLARE(
  node_462_bias, AI_STATIC,
  236, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &node_462_bias_array, NULL)

/* Tensor #237 */
AI_TENSOR_OBJ_DECLARE(
  node_462_output, AI_STATIC,
  237, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 20, 20), AI_STRIDE_INIT(4, 4, 4, 8, 160),
  1, &node_462_output_array, NULL)

/* Tensor #238 */
AI_TENSOR_OBJ_DECLARE(
  node_462_scratch0, AI_STATIC,
  238, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 3, 3), AI_STRIDE_INIT(4, 4, 4, 256, 768),
  1, &node_462_scratch0_array, NULL)

/* Tensor #239 */
AI_TENSOR_OBJ_DECLARE(
  node_462_weights, AI_STATIC,
  239, 0x0,
  AI_SHAPE_INIT(4, 64, 3, 3, 2), AI_STRIDE_INIT(4, 4, 256, 512, 1536),
  1, &node_462_weights_array, NULL)

/* Tensor #240 */
AI_TENSOR_OBJ_DECLARE(
  node_463_bias, AI_STATIC,
  240, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &node_463_bias_array, NULL)

/* Tensor #241 */
AI_TENSOR_OBJ_DECLARE(
  node_463_output, AI_STATIC,
  241, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 20, 20), AI_STRIDE_INIT(4, 4, 4, 32, 640),
  1, &node_463_output_array, NULL)

/* Tensor #242 */
AI_TENSOR_OBJ_DECLARE(
  node_463_scratch0, AI_STATIC,
  242, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 3, 3), AI_STRIDE_INIT(4, 4, 4, 256, 768),
  1, &node_463_scratch0_array, NULL)

/* Tensor #243 */
AI_TENSOR_OBJ_DECLARE(
  node_463_weights, AI_STATIC,
  243, 0x0,
  AI_SHAPE_INIT(4, 64, 3, 3, 8), AI_STRIDE_INIT(4, 4, 256, 2048, 6144),
  1, &node_463_weights_array, NULL)

/* Tensor #244 */
AI_TENSOR_OBJ_DECLARE(
  node_464_bias, AI_STATIC,
  244, 0x0,
  AI_SHAPE_INIT(4, 1, 20, 1, 1), AI_STRIDE_INIT(4, 4, 4, 80, 80),
  1, &node_464_bias_array, NULL)

/* Tensor #245 */
AI_TENSOR_OBJ_DECLARE(
  node_464_output, AI_STATIC,
  245, 0x0,
  AI_SHAPE_INIT(4, 1, 20, 20, 20), AI_STRIDE_INIT(4, 4, 4, 80, 1600),
  1, &node_464_output_array, NULL)

/* Tensor #246 */
AI_TENSOR_OBJ_DECLARE(
  node_464_scratch0, AI_STATIC,
  246, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 3, 3), AI_STRIDE_INIT(4, 4, 4, 256, 768),
  1, &node_464_scratch0_array, NULL)

/* Tensor #247 */
AI_TENSOR_OBJ_DECLARE(
  node_464_weights, AI_STATIC,
  247, 0x0,
  AI_SHAPE_INIT(4, 64, 3, 3, 20), AI_STRIDE_INIT(4, 4, 256, 5120, 15360),
  1, &node_464_weights_array, NULL)

/* Tensor #248 */
AI_TENSOR_OBJ_DECLARE(
  node_465_reshape_in_0105_to_chlast_output, AI_STATIC,
  248, 0x0,
  AI_SHAPE_INIT(4, 1, 20, 20, 2), AI_STRIDE_INIT(4, 4, 4, 80, 1600),
  1, &node_465_reshape_in_0105_to_chlast_output_array, NULL)

/* Tensor #249 */
AI_TENSOR_OBJ_DECLARE(
  node_465_reshape_in_0105_to_chlast_output0, AI_STATIC,
  249, 0x0,
  AI_SHAPE_INIT(5, 1, 20, 2, 1, 20), AI_STRIDE_INIT(5, 4, 4, 1600, 3200, 80),
  1, &node_465_reshape_in_0105_to_chlast_output_array, NULL)

/* Tensor #250 */
AI_TENSOR_OBJ_DECLARE(
  node_467_to_chlast_output, AI_STATIC,
  250, 0x0,
  AI_SHAPE_INIT(5, 1, 2, 20, 20, 1), AI_STRIDE_INIT(5, 4, 4, 8, 160, 8),
  1, &node_467_to_chlast_output_array, NULL)

/* Tensor #251 */
AI_TENSOR_OBJ_DECLARE(
  node_467_to_chlast_output0, AI_STATIC,
  251, 0x0,
  AI_SHAPE_INIT(4, 1, 800, 1, 1), AI_STRIDE_INIT(4, 4, 4, 3200, 3200),
  1, &node_467_to_chlast_output_array, NULL)

/* Tensor #252 */
AI_TENSOR_OBJ_DECLARE(
  node_468_output, AI_STATIC,
  252, 0x0,
  AI_SHAPE_INIT(4, 1, 800, 1, 1), AI_STRIDE_INIT(4, 4, 4, 3200, 3200),
  1, &node_468_output_array, NULL)

/* Tensor #253 */
AI_TENSOR_OBJ_DECLARE(
  node_469_reshape_in_0102_to_chlast_output, AI_STATIC,
  253, 0x0,
  AI_SHAPE_INIT(4, 1, 20, 20, 8), AI_STRIDE_INIT(4, 4, 4, 80, 1600),
  1, &node_469_reshape_in_0102_to_chlast_output_array, NULL)

/* Tensor #254 */
AI_TENSOR_OBJ_DECLARE(
  node_469_reshape_in_0102_to_chlast_output0, AI_STATIC,
  254, 0x0,
  AI_SHAPE_INIT(5, 1, 20, 8, 1, 20), AI_STRIDE_INIT(5, 4, 4, 1600, 12800, 80),
  1, &node_469_reshape_in_0102_to_chlast_output_array, NULL)

/* Tensor #255 */
AI_TENSOR_OBJ_DECLARE(
  node_471_to_chlast_output, AI_STATIC,
  255, 0x0,
  AI_SHAPE_INIT(5, 1, 8, 20, 20, 1), AI_STRIDE_INIT(5, 4, 4, 32, 640, 32),
  1, &node_471_to_chlast_output_array, NULL)

/* Tensor #256 */
AI_TENSOR_OBJ_DECLARE(
  node_472_reshape_in_099_to_chlast_output, AI_STATIC,
  256, 0x0,
  AI_SHAPE_INIT(4, 1, 20, 20, 20), AI_STRIDE_INIT(4, 4, 4, 80, 1600),
  1, &node_472_reshape_in_099_to_chlast_output_array, NULL)

/* Tensor #257 */
AI_TENSOR_OBJ_DECLARE(
  node_472_reshape_in_099_to_chlast_output0, AI_STATIC,
  257, 0x0,
  AI_SHAPE_INIT(5, 1, 20, 20, 1, 20), AI_STRIDE_INIT(5, 4, 4, 1600, 32000, 80),
  1, &node_472_reshape_in_099_to_chlast_output_array, NULL)

/* Tensor #258 */
AI_TENSOR_OBJ_DECLARE(
  node_474_to_chlast_output, AI_STATIC,
  258, 0x0,
  AI_SHAPE_INIT(5, 1, 20, 20, 20, 1), AI_STRIDE_INIT(5, 4, 4, 80, 1600, 80),
  1, &node_474_to_chlast_output_array, NULL)

/* Tensor #259 */
AI_TENSOR_OBJ_DECLARE(
  node_475_bias, AI_STATIC,
  259, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &node_475_bias_array, NULL)

/* Tensor #260 */
AI_TENSOR_OBJ_DECLARE(
  node_475_output, AI_STATIC,
  260, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 10, 10), AI_STRIDE_INIT(4, 4, 4, 64, 640),
  1, &node_475_output_array, NULL)

/* Tensor #261 */
AI_TENSOR_OBJ_DECLARE(
  node_475_weights, AI_STATIC,
  261, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 16), AI_STRIDE_INIT(4, 1, 16, 16, 16),
  1, &node_475_weights_array, NULL)

/* Tensor #262 */
AI_TENSOR_OBJ_DECLARE(
  node_477_output, AI_STATIC,
  262, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 10, 10), AI_STRIDE_INIT(4, 4, 4, 64, 640),
  1, &node_477_output_array, NULL)

/* Tensor #263 */
AI_TENSOR_OBJ_DECLARE(
  node_478_bias, AI_STATIC,
  263, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &node_478_bias_array, NULL)

/* Tensor #264 */
AI_TENSOR_OBJ_DECLARE(
  node_478_output, AI_STATIC,
  264, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 10, 10), AI_STRIDE_INIT(4, 4, 4, 256, 2560),
  1, &node_478_output_array, NULL)

/* Tensor #265 */
AI_TENSOR_OBJ_DECLARE(
  node_478_scratch0, AI_STATIC,
  265, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &node_478_scratch0_array, NULL)

/* Tensor #266 */
AI_TENSOR_OBJ_DECLARE(
  node_478_weights, AI_STATIC,
  266, 0x0,
  AI_SHAPE_INIT(4, 16, 1, 1, 64), AI_STRIDE_INIT(4, 4, 64, 4096, 4096),
  1, &node_478_weights_array, NULL)

/* Tensor #267 */
AI_TENSOR_OBJ_DECLARE(
  node_480_output, AI_STATIC,
  267, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 10, 10), AI_STRIDE_INIT(4, 4, 4, 256, 2560),
  1, &node_480_output_array, NULL)

/* Tensor #268 */
AI_TENSOR_OBJ_DECLARE(
  node_481_bias, AI_STATIC,
  268, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &node_481_bias_array, NULL)

/* Tensor #269 */
AI_TENSOR_OBJ_DECLARE(
  node_481_output, AI_STATIC,
  269, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 10, 10), AI_STRIDE_INIT(4, 4, 4, 256, 2560),
  1, &node_481_output_array, NULL)

/* Tensor #270 */
AI_TENSOR_OBJ_DECLARE(
  node_481_weights, AI_STATIC,
  270, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 64), AI_STRIDE_INIT(4, 1, 64, 64, 64),
  1, &node_481_weights_array, NULL)

/* Tensor #271 */
AI_TENSOR_OBJ_DECLARE(
  node_483_output, AI_STATIC,
  271, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 10, 10), AI_STRIDE_INIT(4, 4, 4, 256, 2560),
  1, &node_483_output_array, NULL)

/* Tensor #272 */
AI_TENSOR_OBJ_DECLARE(
  node_484_bias, AI_STATIC,
  272, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &node_484_bias_array, NULL)

/* Tensor #273 */
AI_TENSOR_OBJ_DECLARE(
  node_484_output, AI_STATIC,
  273, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 10, 10), AI_STRIDE_INIT(4, 4, 4, 256, 2560),
  1, &node_484_output_array, NULL)

/* Tensor #274 */
AI_TENSOR_OBJ_DECLARE(
  node_484_scratch0, AI_STATIC,
  274, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &node_484_scratch0_array, NULL)

/* Tensor #275 */
AI_TENSOR_OBJ_DECLARE(
  node_484_weights, AI_STATIC,
  275, 0x0,
  AI_SHAPE_INIT(4, 64, 1, 1, 64), AI_STRIDE_INIT(4, 4, 256, 16384, 16384),
  1, &node_484_weights_array, NULL)

/* Tensor #276 */
AI_TENSOR_OBJ_DECLARE(
  node_486_output, AI_STATIC,
  276, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 10, 10), AI_STRIDE_INIT(4, 4, 4, 256, 2560),
  1, &node_486_output_array, NULL)

/* Tensor #277 */
AI_TENSOR_OBJ_DECLARE(
  node_487_bias, AI_STATIC,
  277, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &node_487_bias_array, NULL)

/* Tensor #278 */
AI_TENSOR_OBJ_DECLARE(
  node_487_output, AI_STATIC,
  278, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 10, 10), AI_STRIDE_INIT(4, 4, 4, 8, 80),
  1, &node_487_output_array, NULL)

/* Tensor #279 */
AI_TENSOR_OBJ_DECLARE(
  node_487_scratch0, AI_STATIC,
  279, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 3, 3), AI_STRIDE_INIT(4, 4, 4, 256, 768),
  1, &node_487_scratch0_array, NULL)

/* Tensor #280 */
AI_TENSOR_OBJ_DECLARE(
  node_487_weights, AI_STATIC,
  280, 0x0,
  AI_SHAPE_INIT(4, 64, 3, 3, 2), AI_STRIDE_INIT(4, 4, 256, 512, 1536),
  1, &node_487_weights_array, NULL)

/* Tensor #281 */
AI_TENSOR_OBJ_DECLARE(
  node_488_bias, AI_STATIC,
  281, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &node_488_bias_array, NULL)

/* Tensor #282 */
AI_TENSOR_OBJ_DECLARE(
  node_488_output, AI_STATIC,
  282, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 10, 10), AI_STRIDE_INIT(4, 4, 4, 32, 320),
  1, &node_488_output_array, NULL)

/* Tensor #283 */
AI_TENSOR_OBJ_DECLARE(
  node_488_scratch0, AI_STATIC,
  283, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 3, 3), AI_STRIDE_INIT(4, 4, 4, 256, 768),
  1, &node_488_scratch0_array, NULL)

/* Tensor #284 */
AI_TENSOR_OBJ_DECLARE(
  node_488_weights, AI_STATIC,
  284, 0x0,
  AI_SHAPE_INIT(4, 64, 3, 3, 8), AI_STRIDE_INIT(4, 4, 256, 2048, 6144),
  1, &node_488_weights_array, NULL)

/* Tensor #285 */
AI_TENSOR_OBJ_DECLARE(
  node_489_bias, AI_STATIC,
  285, 0x0,
  AI_SHAPE_INIT(4, 1, 20, 1, 1), AI_STRIDE_INIT(4, 4, 4, 80, 80),
  1, &node_489_bias_array, NULL)

/* Tensor #286 */
AI_TENSOR_OBJ_DECLARE(
  node_489_output, AI_STATIC,
  286, 0x0,
  AI_SHAPE_INIT(4, 1, 20, 10, 10), AI_STRIDE_INIT(4, 4, 4, 80, 800),
  1, &node_489_output_array, NULL)

/* Tensor #287 */
AI_TENSOR_OBJ_DECLARE(
  node_489_scratch0, AI_STATIC,
  287, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 3, 3), AI_STRIDE_INIT(4, 4, 4, 256, 768),
  1, &node_489_scratch0_array, NULL)

/* Tensor #288 */
AI_TENSOR_OBJ_DECLARE(
  node_489_weights, AI_STATIC,
  288, 0x0,
  AI_SHAPE_INIT(4, 64, 3, 3, 20), AI_STRIDE_INIT(4, 4, 256, 5120, 15360),
  1, &node_489_weights_array, NULL)

/* Tensor #289 */
AI_TENSOR_OBJ_DECLARE(
  node_490_reshape_in_0126_to_chlast_output, AI_STATIC,
  289, 0x0,
  AI_SHAPE_INIT(4, 1, 10, 10, 2), AI_STRIDE_INIT(4, 4, 4, 40, 400),
  1, &node_490_reshape_in_0126_to_chlast_output_array, NULL)

/* Tensor #290 */
AI_TENSOR_OBJ_DECLARE(
  node_490_reshape_in_0126_to_chlast_output0, AI_STATIC,
  290, 0x0,
  AI_SHAPE_INIT(5, 1, 10, 2, 1, 10), AI_STRIDE_INIT(5, 4, 4, 400, 800, 40),
  1, &node_490_reshape_in_0126_to_chlast_output_array, NULL)

/* Tensor #291 */
AI_TENSOR_OBJ_DECLARE(
  node_492_to_chlast_output, AI_STATIC,
  291, 0x0,
  AI_SHAPE_INIT(5, 1, 2, 10, 10, 1), AI_STRIDE_INIT(5, 4, 4, 8, 80, 8),
  1, &node_492_to_chlast_output_array, NULL)

/* Tensor #292 */
AI_TENSOR_OBJ_DECLARE(
  node_492_to_chlast_output0, AI_STATIC,
  292, 0x0,
  AI_SHAPE_INIT(4, 1, 200, 1, 1), AI_STRIDE_INIT(4, 4, 4, 800, 800),
  1, &node_492_to_chlast_output_array, NULL)

/* Tensor #293 */
AI_TENSOR_OBJ_DECLARE(
  node_493_output, AI_STATIC,
  293, 0x0,
  AI_SHAPE_INIT(4, 1, 200, 1, 1), AI_STRIDE_INIT(4, 4, 4, 800, 800),
  1, &node_493_output_array, NULL)

/* Tensor #294 */
AI_TENSOR_OBJ_DECLARE(
  node_494_reshape_in_0123_to_chlast_output, AI_STATIC,
  294, 0x0,
  AI_SHAPE_INIT(4, 1, 10, 10, 8), AI_STRIDE_INIT(4, 4, 4, 40, 400),
  1, &node_494_reshape_in_0123_to_chlast_output_array, NULL)

/* Tensor #295 */
AI_TENSOR_OBJ_DECLARE(
  node_494_reshape_in_0123_to_chlast_output0, AI_STATIC,
  295, 0x0,
  AI_SHAPE_INIT(5, 1, 10, 8, 1, 10), AI_STRIDE_INIT(5, 4, 4, 400, 3200, 40),
  1, &node_494_reshape_in_0123_to_chlast_output_array, NULL)

/* Tensor #296 */
AI_TENSOR_OBJ_DECLARE(
  node_496_to_chlast_output, AI_STATIC,
  296, 0x0,
  AI_SHAPE_INIT(5, 1, 8, 10, 10, 1), AI_STRIDE_INIT(5, 4, 4, 32, 320, 32),
  1, &node_496_to_chlast_output_array, NULL)

/* Tensor #297 */
AI_TENSOR_OBJ_DECLARE(
  node_497_reshape_in_0120_to_chlast_output, AI_STATIC,
  297, 0x0,
  AI_SHAPE_INIT(4, 1, 10, 10, 20), AI_STRIDE_INIT(4, 4, 4, 40, 400),
  1, &node_497_reshape_in_0120_to_chlast_output_array, NULL)

/* Tensor #298 */
AI_TENSOR_OBJ_DECLARE(
  node_497_reshape_in_0120_to_chlast_output0, AI_STATIC,
  298, 0x0,
  AI_SHAPE_INIT(5, 1, 10, 20, 1, 10), AI_STRIDE_INIT(5, 4, 4, 400, 8000, 40),
  1, &node_497_reshape_in_0120_to_chlast_output_array, NULL)

/* Tensor #299 */
AI_TENSOR_OBJ_DECLARE(
  node_499_to_chlast_output, AI_STATIC,
  299, 0x0,
  AI_SHAPE_INIT(5, 1, 20, 10, 10, 1), AI_STRIDE_INIT(5, 4, 4, 80, 800, 80),
  1, &node_499_to_chlast_output_array, NULL)



/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_493_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_492_to_chlast_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_493_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_493_layer, 142,
  NL_TYPE, 0x0, NULL,
  nl, forward_sigmoid,
  &node_493_chain,
  NULL, &node_493_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_492_to_chlast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_490_reshape_in_0126_to_chlast_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_492_to_chlast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_492_to_chlast_layer, 141,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &node_492_to_chlast_chain,
  NULL, &node_493_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_CHANNEL, AI_SHAPE_DEPTH, AI_SHAPE_HEIGHT, AI_SHAPE_EXTENSION), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_490_reshape_in_0126_to_chlast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_487_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_490_reshape_in_0126_to_chlast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_490_reshape_in_0126_to_chlast_layer, 140,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &node_490_reshape_in_0126_to_chlast_chain,
  NULL, &node_492_to_chlast_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_HEIGHT, AI_SHAPE_CHANNEL, AI_SHAPE_DEPTH, AI_SHAPE_EXTENSION), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_487_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_486_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_487_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_487_weights, &node_487_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_487_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_487_layer, 137,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_487_chain,
  NULL, &node_490_reshape_in_0126_to_chlast_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_496_to_chlast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_494_reshape_in_0123_to_chlast_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_496_to_chlast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_496_to_chlast_layer, 144,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &node_496_to_chlast_chain,
  NULL, &node_487_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_CHANNEL, AI_SHAPE_DEPTH, AI_SHAPE_HEIGHT, AI_SHAPE_EXTENSION), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_494_reshape_in_0123_to_chlast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_488_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_494_reshape_in_0123_to_chlast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_494_reshape_in_0123_to_chlast_layer, 143,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &node_494_reshape_in_0123_to_chlast_chain,
  NULL, &node_496_to_chlast_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_HEIGHT, AI_SHAPE_CHANNEL, AI_SHAPE_DEPTH, AI_SHAPE_EXTENSION), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_488_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_486_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_488_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_488_weights, &node_488_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_488_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_488_layer, 138,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_488_chain,
  NULL, &node_494_reshape_in_0123_to_chlast_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_499_to_chlast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_497_reshape_in_0120_to_chlast_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_499_to_chlast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_499_to_chlast_layer, 146,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &node_499_to_chlast_chain,
  NULL, &node_488_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_CHANNEL, AI_SHAPE_DEPTH, AI_SHAPE_HEIGHT, AI_SHAPE_EXTENSION), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_497_reshape_in_0120_to_chlast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_489_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_497_reshape_in_0120_to_chlast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_497_reshape_in_0120_to_chlast_layer, 145,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &node_497_reshape_in_0120_to_chlast_chain,
  NULL, &node_499_to_chlast_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_HEIGHT, AI_SHAPE_CHANNEL, AI_SHAPE_DEPTH, AI_SHAPE_EXTENSION), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_489_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_486_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_489_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_489_weights, &node_489_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_489_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_489_layer, 139,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_489_chain,
  NULL, &node_497_reshape_in_0120_to_chlast_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_486_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_484_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_486_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_486_layer, 136,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_486_chain,
  NULL, &node_489_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_484_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_483_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_484_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_484_weights, &node_484_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_484_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_484_layer, 135,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_484_chain,
  NULL, &node_486_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_483_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_481_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_483_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_483_layer, 134,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_483_chain,
  NULL, &node_484_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_481_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_480_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_481_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_481_weights, &node_481_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_481_layer, 133,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_481_chain,
  NULL, &node_483_layer, AI_STATIC, 
  .groups = 64, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_480_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_478_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_480_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_480_layer, 132,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_480_chain,
  NULL, &node_481_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_478_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_477_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_478_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_478_weights, &node_478_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_478_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_478_layer, 131,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_478_chain,
  NULL, &node_480_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_477_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_475_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_477_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_477_layer, 130,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_477_chain,
  NULL, &node_478_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_475_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_424_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_475_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_475_weights, &node_475_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_475_layer, 129,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_475_chain,
  NULL, &node_477_layer, AI_STATIC, 
  .groups = 16, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_424_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_422_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_424_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_424_weights, &node_424_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_424_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_424_layer, 92,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_424_chain,
  NULL, &node_475_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_422_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_418_output, &node_421_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_422_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_422_layer, 90,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &node_422_chain,
  NULL, &node_424_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_421_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_420_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_421_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_421_weights, &node_418_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_421_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_421_layer, 89,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_421_chain,
  NULL, &node_422_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_468_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_467_to_chlast_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_468_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_468_layer, 124,
  NL_TYPE, 0x0, NULL,
  nl, forward_sigmoid,
  &node_468_chain,
  NULL, &node_421_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_467_to_chlast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_465_reshape_in_0105_to_chlast_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_467_to_chlast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_467_to_chlast_layer, 123,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &node_467_to_chlast_chain,
  NULL, &node_468_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_CHANNEL, AI_SHAPE_DEPTH, AI_SHAPE_HEIGHT, AI_SHAPE_EXTENSION), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_465_reshape_in_0105_to_chlast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_462_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_465_reshape_in_0105_to_chlast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_465_reshape_in_0105_to_chlast_layer, 122,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &node_465_reshape_in_0105_to_chlast_chain,
  NULL, &node_467_to_chlast_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_HEIGHT, AI_SHAPE_CHANNEL, AI_SHAPE_DEPTH, AI_SHAPE_EXTENSION), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_462_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_461_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_462_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_462_weights, &node_462_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_462_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_462_layer, 119,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_462_chain,
  NULL, &node_465_reshape_in_0105_to_chlast_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_471_to_chlast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_469_reshape_in_0102_to_chlast_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_471_to_chlast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_471_to_chlast_layer, 126,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &node_471_to_chlast_chain,
  NULL, &node_462_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_CHANNEL, AI_SHAPE_DEPTH, AI_SHAPE_HEIGHT, AI_SHAPE_EXTENSION), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_469_reshape_in_0102_to_chlast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_463_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_469_reshape_in_0102_to_chlast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_469_reshape_in_0102_to_chlast_layer, 125,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &node_469_reshape_in_0102_to_chlast_chain,
  NULL, &node_471_to_chlast_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_HEIGHT, AI_SHAPE_CHANNEL, AI_SHAPE_DEPTH, AI_SHAPE_EXTENSION), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_463_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_461_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_463_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_463_weights, &node_463_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_463_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_463_layer, 120,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_463_chain,
  NULL, &node_469_reshape_in_0102_to_chlast_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_474_to_chlast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_472_reshape_in_099_to_chlast_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_474_to_chlast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_474_to_chlast_layer, 128,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &node_474_to_chlast_chain,
  NULL, &node_463_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_CHANNEL, AI_SHAPE_DEPTH, AI_SHAPE_HEIGHT, AI_SHAPE_EXTENSION), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_472_reshape_in_099_to_chlast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_464_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_472_reshape_in_099_to_chlast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_472_reshape_in_099_to_chlast_layer, 127,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &node_472_reshape_in_099_to_chlast_chain,
  NULL, &node_474_to_chlast_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_HEIGHT, AI_SHAPE_CHANNEL, AI_SHAPE_DEPTH, AI_SHAPE_EXTENSION), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_464_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_461_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_464_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_464_weights, &node_464_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_464_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_464_layer, 121,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_464_chain,
  NULL, &node_472_reshape_in_099_to_chlast_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_461_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_459_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_461_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_461_layer, 118,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_461_chain,
  NULL, &node_464_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_459_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_458_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_459_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_459_weights, &node_459_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_459_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_459_layer, 117,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_459_chain,
  NULL, &node_461_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_458_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_456_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_458_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_458_layer, 116,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_458_chain,
  NULL, &node_459_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_456_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_455_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_456_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_456_weights, &node_456_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_456_layer, 115,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_456_chain,
  NULL, &node_458_layer, AI_STATIC, 
  .groups = 64, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_455_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_453_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_455_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_455_layer, 114,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_455_chain,
  NULL, &node_456_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_453_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_452_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_453_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_453_weights, &node_453_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_453_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_453_layer, 113,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_453_chain,
  NULL, &node_455_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_452_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_450_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_452_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_452_layer, 112,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_452_chain,
  NULL, &node_453_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_450_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_423_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_450_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_450_weights, &node_450_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_450_layer, 111,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_450_chain,
  NULL, &node_452_layer, AI_STATIC, 
  .groups = 16, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_423_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_420_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_423_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_423_weights, &node_423_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_423_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_423_layer, 91,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_423_chain,
  NULL, &node_450_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_420_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_417_output, &node_419_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_420_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_420_layer, 88,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &node_420_chain,
  NULL, &node_423_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_419_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_416_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_419_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_419_weights, &node_417_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_419_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_419_layer, 87,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_419_chain,
  NULL, &node_420_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_443_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_442_to_chlast_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_443_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_443_layer, 106,
  NL_TYPE, 0x0, NULL,
  nl, forward_sigmoid,
  &node_443_chain,
  NULL, &node_419_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_442_to_chlast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_440_reshape_in_084_to_chlast_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_442_to_chlast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_442_to_chlast_layer, 105,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &node_442_to_chlast_chain,
  NULL, &node_443_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_CHANNEL, AI_SHAPE_DEPTH, AI_SHAPE_HEIGHT, AI_SHAPE_EXTENSION), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_440_reshape_in_084_to_chlast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_437_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_440_reshape_in_084_to_chlast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_440_reshape_in_084_to_chlast_layer, 104,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &node_440_reshape_in_084_to_chlast_chain,
  NULL, &node_442_to_chlast_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_HEIGHT, AI_SHAPE_CHANNEL, AI_SHAPE_DEPTH, AI_SHAPE_EXTENSION), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_437_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_436_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_437_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_437_weights, &node_437_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_437_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_437_layer, 101,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_437_chain,
  NULL, &node_440_reshape_in_084_to_chlast_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_446_to_chlast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_444_reshape_in_081_to_chlast_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_446_to_chlast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_446_to_chlast_layer, 108,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &node_446_to_chlast_chain,
  NULL, &node_437_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_CHANNEL, AI_SHAPE_DEPTH, AI_SHAPE_HEIGHT, AI_SHAPE_EXTENSION), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_444_reshape_in_081_to_chlast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_438_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_444_reshape_in_081_to_chlast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_444_reshape_in_081_to_chlast_layer, 107,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &node_444_reshape_in_081_to_chlast_chain,
  NULL, &node_446_to_chlast_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_HEIGHT, AI_SHAPE_CHANNEL, AI_SHAPE_DEPTH, AI_SHAPE_EXTENSION), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_438_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_436_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_438_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_438_weights, &node_438_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_438_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_438_layer, 102,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_438_chain,
  NULL, &node_444_reshape_in_081_to_chlast_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_449_to_chlast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_447_reshape_in_078_to_chlast_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_449_to_chlast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_449_to_chlast_layer, 110,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &node_449_to_chlast_chain,
  NULL, &node_438_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_CHANNEL, AI_SHAPE_DEPTH, AI_SHAPE_HEIGHT, AI_SHAPE_EXTENSION), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_447_reshape_in_078_to_chlast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_439_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_447_reshape_in_078_to_chlast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_447_reshape_in_078_to_chlast_layer, 109,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &node_447_reshape_in_078_to_chlast_chain,
  NULL, &node_449_to_chlast_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_HEIGHT, AI_SHAPE_CHANNEL, AI_SHAPE_DEPTH, AI_SHAPE_EXTENSION), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_439_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_436_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_439_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_439_weights, &node_439_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_439_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_439_layer, 103,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_439_chain,
  NULL, &node_447_reshape_in_078_to_chlast_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_436_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_434_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_436_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_436_layer, 100,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_436_chain,
  NULL, &node_439_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_434_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_433_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_434_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_434_weights, &node_434_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_434_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_434_layer, 99,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_434_chain,
  NULL, &node_436_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_433_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_431_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_433_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_433_layer, 98,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_433_chain,
  NULL, &node_434_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_431_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_430_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_431_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_431_weights, &node_431_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_431_layer, 97,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_431_chain,
  NULL, &node_433_layer, AI_STATIC, 
  .groups = 64, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_430_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_428_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_430_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_430_layer, 96,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_430_chain,
  NULL, &node_431_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_428_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_427_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_428_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_428_weights, &node_428_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_428_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_428_layer, 95,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_428_chain,
  NULL, &node_430_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_427_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_425_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_427_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_427_layer, 94,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_427_chain,
  NULL, &node_428_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_425_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_416_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_425_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_425_weights, &node_425_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_425_layer, 93,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_425_chain,
  NULL, &node_427_layer, AI_STATIC, 
  .groups = 16, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_416_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_415_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_416_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_416_weights, &node_416_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_416_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_416_layer, 84,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_416_chain,
  NULL, &node_425_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_415_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_373_output, &node_414_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_415_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_415_layer, 83,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &node_415_chain,
  NULL, &node_416_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)


AI_STATIC_CONST ai_float node_414_scales_data[] = { 2.0, 2.0, 1.0, 1.0 };
AI_ARRAY_OBJ_DECLARE(
    node_414_scales, AI_ARRAY_FORMAT_FLOAT,
    node_414_scales_data, node_414_scales_data, 4, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_414_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_395_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_414_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_414_layer, 82,
  UPSAMPLE_TYPE, 0x0, NULL,
  upsample, forward_upsample_nearest,
  &node_414_chain,
  NULL, &node_415_layer, AI_STATIC, 
  .scales = &node_414_scales, 
  .center = false, 
  .mode = AI_UPSAMPLE_NEAREST, 
  .nearest_mode = AI_ROUND_FLOOR, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_417_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_395_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_417_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_417_weights, &node_417_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_417_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_417_layer, 85,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_417_chain,
  NULL, &node_414_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_395_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_374_output, &node_394_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_395_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_395_layer, 72,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &node_395_chain,
  NULL, &node_417_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)


AI_STATIC_CONST ai_float node_394_scales_data[] = { 2.0, 2.0, 1.0, 1.0 };
AI_ARRAY_OBJ_DECLARE(
    node_394_scales, AI_ARRAY_FORMAT_FLOAT,
    node_394_scales_data, node_394_scales_data, 4, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_394_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_375_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_394_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_394_layer, 71,
  UPSAMPLE_TYPE, 0x0, NULL,
  upsample, forward_upsample_nearest,
  &node_394_chain,
  NULL, &node_395_layer, AI_STATIC, 
  .scales = &node_394_scales, 
  .center = false, 
  .mode = AI_UPSAMPLE_NEAREST, 
  .nearest_mode = AI_ROUND_FLOOR, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_418_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_375_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_418_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_418_weights, &node_418_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_418_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_418_layer, 86,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_418_chain,
  NULL, &node_394_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_375_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_372_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_375_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_375_weights, &node_375_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_375_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_375_layer, 61,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_375_chain,
  NULL, &node_418_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_372_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_370_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_372_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_372_layer, 58,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_372_chain,
  NULL, &node_375_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_370_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_369_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_370_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_370_weights, &node_370_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_370_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_370_layer, 57,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_370_chain,
  NULL, &node_372_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_369_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_367_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_369_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_369_layer, 56,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_369_chain,
  NULL, &node_370_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_367_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_366_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_367_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_367_weights, &node_367_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_367_layer, 55,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_367_chain,
  NULL, &node_369_layer, AI_STATIC, 
  .groups = 288, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_366_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_364_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_366_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_366_layer, 54,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_366_chain,
  NULL, &node_367_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_364_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_363_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_364_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_364_weights, &node_364_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_364_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_364_layer, 53,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_364_chain,
  NULL, &node_366_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_363_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_361_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_363_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_363_layer, 52,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_363_chain,
  NULL, &node_364_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_361_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_360_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_361_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_361_weights, &node_361_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_361_layer, 51,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_361_chain,
  NULL, &node_363_layer, AI_STATIC, 
  .groups = 288, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_360_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_358_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_360_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_360_layer, 50,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_360_chain,
  NULL, &node_361_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_358_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_357_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_358_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_358_weights, &node_358_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_358_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_358_layer, 49,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_358_chain,
  NULL, &node_360_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_357_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_355_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_357_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_357_layer, 48,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_357_chain,
  NULL, &node_358_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_355_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_354_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_355_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_355_weights, &node_355_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_355_layer, 47,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_355_chain,
  NULL, &node_357_layer, AI_STATIC, 
  .groups = 288, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_354_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_352_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_354_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_354_layer, 46,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_354_chain,
  NULL, &node_355_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_352_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_351_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_352_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_352_weights, &node_352_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_352_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_352_layer, 45,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_352_chain,
  NULL, &node_354_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_351_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_349_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_351_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_351_layer, 44,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_351_chain,
  NULL, &node_352_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_349_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_348_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_349_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_349_weights, &node_349_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_349_layer, 43,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_349_chain,
  NULL, &node_351_layer, AI_STATIC, 
  .groups = 288, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_348_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_346_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_348_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_348_layer, 42,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_348_chain,
  NULL, &node_349_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_346_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_345_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_346_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_346_weights, &node_346_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_346_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_346_layer, 41,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_346_chain,
  NULL, &node_348_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_345_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_343_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_345_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_345_layer, 40,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_345_chain,
  NULL, &node_346_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_343_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_342_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_343_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_343_weights, &node_343_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_343_layer, 39,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_343_chain,
  NULL, &node_345_layer, AI_STATIC, 
  .groups = 288, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_342_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_340_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_342_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_342_layer, 38,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_342_chain,
  NULL, &node_343_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_340_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_339_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_340_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_340_weights, &node_340_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_340_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_340_layer, 37,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_340_chain,
  NULL, &node_342_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_339_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_337_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_339_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_339_layer, 36,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_339_chain,
  NULL, &node_340_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_337_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_336_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_337_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_337_weights, &node_337_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_337_layer, 35,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_337_chain,
  NULL, &node_339_layer, AI_STATIC, 
  .groups = 152, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_374_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_336_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_374_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_374_weights, &node_374_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_374_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_374_layer, 60,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_374_chain,
  NULL, &node_337_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_336_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_334_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_336_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_336_layer, 34,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_336_chain,
  NULL, &node_374_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_334_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_333_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_334_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_334_weights, &node_334_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_334_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_334_layer, 33,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_334_chain,
  NULL, &node_336_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_333_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_331_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_333_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_333_layer, 32,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_333_chain,
  NULL, &node_334_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_331_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_330_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_331_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_331_weights, &node_331_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_331_layer, 31,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_331_chain,
  NULL, &node_333_layer, AI_STATIC, 
  .groups = 152, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_330_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_328_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_330_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_330_layer, 30,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_330_chain,
  NULL, &node_331_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_328_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_327_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_328_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_328_weights, &node_328_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_328_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_328_layer, 29,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_328_chain,
  NULL, &node_330_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_327_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_325_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_327_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_327_layer, 28,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_327_chain,
  NULL, &node_328_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_325_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_324_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_325_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_325_weights, &node_325_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_325_layer, 27,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_325_chain,
  NULL, &node_327_layer, AI_STATIC, 
  .groups = 72, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_373_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_324_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_373_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_373_weights, &node_373_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_373_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_373_layer, 59,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_373_chain,
  NULL, &node_325_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_324_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_322_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_324_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_324_layer, 26,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_324_chain,
  NULL, &node_373_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_322_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_321_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_322_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_322_weights, &node_322_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_322_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_322_layer, 25,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_322_chain,
  NULL, &node_324_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_321_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_319_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_321_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_321_layer, 24,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_321_chain,
  NULL, &node_322_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_319_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_318_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_319_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_319_weights, &node_319_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_319_layer, 23,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_319_chain,
  NULL, &node_321_layer, AI_STATIC, 
  .groups = 72, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_318_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_316_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_318_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_318_layer, 22,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_318_chain,
  NULL, &node_319_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_316_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_315_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_316_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_316_weights, &node_316_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_316_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_316_layer, 21,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_316_chain,
  NULL, &node_318_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_315_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_313_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_315_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_315_layer, 20,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_315_chain,
  NULL, &node_316_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_313_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_312_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_313_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_313_weights, &node_313_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_313_layer, 19,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_313_chain,
  NULL, &node_315_layer, AI_STATIC, 
  .groups = 72, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_312_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_310_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_312_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_312_layer, 18,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_312_chain,
  NULL, &node_313_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_310_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_309_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_310_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_310_weights, &node_310_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_310_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_310_layer, 17,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_310_chain,
  NULL, &node_312_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_309_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_307_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_309_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_309_layer, 16,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_309_chain,
  NULL, &node_310_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_307_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_306_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_307_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_307_weights, &node_307_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_307_layer, 15,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_307_chain,
  NULL, &node_309_layer, AI_STATIC, 
  .groups = 40, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_306_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_304_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_306_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_306_layer, 14,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_306_chain,
  NULL, &node_307_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_304_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_303_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_304_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_304_weights, &node_304_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_304_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_304_layer, 13,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_304_chain,
  NULL, &node_306_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_303_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_301_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_303_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_303_layer, 12,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_303_chain,
  NULL, &node_304_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_301_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_300_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_301_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_301_weights, &node_301_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_301_layer, 11,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_301_chain,
  NULL, &node_303_layer, AI_STATIC, 
  .groups = 40, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_300_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_298_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_300_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_300_layer, 10,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_300_chain,
  NULL, &node_301_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_298_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_297_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_298_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_298_weights, &node_298_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_298_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_298_layer, 9,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_298_chain,
  NULL, &node_300_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_297_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_295_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_297_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_297_layer, 8,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_297_chain,
  NULL, &node_298_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_295_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_294_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_295_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_295_weights, &node_295_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_295_layer, 7,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_295_chain,
  NULL, &node_297_layer, AI_STATIC, 
  .groups = 16, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_294_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_292_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_294_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_294_layer, 6,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_294_chain,
  NULL, &node_295_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_292_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_291_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_292_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_292_weights, &node_292_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_292_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_292_layer, 5,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_292_chain,
  NULL, &node_294_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_291_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_289_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_291_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_291_layer, 4,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_291_chain,
  NULL, &node_292_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_289_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_288_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_289_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_289_weights, &node_289_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_289_layer, 3,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &node_289_chain,
  NULL, &node_291_layer, AI_STATIC, 
  .groups = 16, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_288_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_286_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_288_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  node_288_layer, 2,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &node_288_chain,
  NULL, &node_289_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  node_286_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &input_1_Transpose_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &node_286_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &node_286_weights, &node_286_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &node_286_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  node_286_layer, 1,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &node_286_chain,
  NULL, &node_288_layer, AI_STATIC, 
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
  NULL, &node_286_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_HEIGHT, AI_SHAPE_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_DEPTH, AI_SHAPE_EXTENSION), 
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 2505288, 1, 1),
    2505288, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 3276864, 1, 1),
    3276864, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_FACEDET_IN_NUM, &input_1_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_FACEDET_OUT_NUM, &node_443_output, &node_468_output, &node_493_output, &node_446_to_chlast_output, &node_471_to_chlast_output, &node_496_to_chlast_output, &node_449_to_chlast_output, &node_474_to_chlast_output, &node_499_to_chlast_output),
  &input_1_Transpose_layer, 0xd8cd5a83, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 2505288, 1, 1),
      2505288, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 3276864, 1, 1),
      3276864, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_FACEDET_IN_NUM, &input_1_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_FACEDET_OUT_NUM, &node_443_output, &node_468_output, &node_493_output, &node_446_to_chlast_output, &node_471_to_chlast_output, &node_496_to_chlast_output, &node_449_to_chlast_output, &node_474_to_chlast_output, &node_499_to_chlast_output),
  &input_1_Transpose_layer, 0xd8cd5a83, NULL)

#endif	/*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/



/******************************************************************************/
AI_DECLARE_STATIC
ai_bool facedet_configure_activations(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_activations_map(g_facedet_activations_map, 1, params)) {
    /* Updating activations (byte) offsets */
    
    input_1_output_array.data = AI_PTR(g_facedet_activations_map[0] + 409600);
    input_1_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 409600);
    input_1_Transpose_output_array.data = AI_PTR(g_facedet_activations_map[0] + 1638400);
    input_1_Transpose_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 1638400);
    node_286_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 2867200);
    node_286_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 2867200);
    node_286_output_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_286_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_288_output_array.data = AI_PTR(g_facedet_activations_map[0] + 1638400);
    node_288_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 1638400);
    node_289_output_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_289_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_291_output_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_291_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_292_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 1638400);
    node_292_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 1638400);
    node_292_output_array.data = AI_PTR(g_facedet_activations_map[0] + 1638464);
    node_292_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 1638464);
    node_294_output_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_294_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_295_output_array.data = AI_PTR(g_facedet_activations_map[0] + 1638400);
    node_295_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 1638400);
    node_297_output_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_297_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_298_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 409600);
    node_298_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 409600);
    node_298_output_array.data = AI_PTR(g_facedet_activations_map[0] + 409664);
    node_298_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 409664);
    node_300_output_array.data = AI_PTR(g_facedet_activations_map[0] + 1433664);
    node_300_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 1433664);
    node_301_output_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_301_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_303_output_array.data = AI_PTR(g_facedet_activations_map[0] + 1024000);
    node_303_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 1024000);
    node_304_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_304_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_304_output_array.data = AI_PTR(g_facedet_activations_map[0] + 2048000);
    node_304_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 2048000);
    node_306_output_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_306_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_307_output_array.data = AI_PTR(g_facedet_activations_map[0] + 1024000);
    node_307_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 1024000);
    node_309_output_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_309_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_310_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 256000);
    node_310_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 256000);
    node_310_output_array.data = AI_PTR(g_facedet_activations_map[0] + 256160);
    node_310_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 256160);
    node_312_output_array.data = AI_PTR(g_facedet_activations_map[0] + 716960);
    node_312_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 716960);
    node_313_output_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_313_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_315_output_array.data = AI_PTR(g_facedet_activations_map[0] + 460800);
    node_315_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 460800);
    node_316_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_316_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_316_output_array.data = AI_PTR(g_facedet_activations_map[0] + 921600);
    node_316_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 921600);
    node_318_output_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_318_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_319_output_array.data = AI_PTR(g_facedet_activations_map[0] + 460800);
    node_319_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 460800);
    node_321_output_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_321_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_322_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 460800);
    node_322_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 460800);
    node_322_output_array.data = AI_PTR(g_facedet_activations_map[0] + 461088);
    node_322_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 461088);
    node_324_output_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_324_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_373_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 460800);
    node_373_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 460800);
    node_373_output_array.data = AI_PTR(g_facedet_activations_map[0] + 461088);
    node_373_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 461088);
    node_325_output_array.data = AI_PTR(g_facedet_activations_map[0] + 563488);
    node_325_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 563488);
    node_327_output_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_327_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_328_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 115200);
    node_328_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 115200);
    node_328_output_array.data = AI_PTR(g_facedet_activations_map[0] + 115488);
    node_328_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 115488);
    node_330_output_array.data = AI_PTR(g_facedet_activations_map[0] + 563488);
    node_330_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 563488);
    node_331_output_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_331_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_333_output_array.data = AI_PTR(g_facedet_activations_map[0] + 563488);
    node_333_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 563488);
    node_334_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_334_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_334_output_array.data = AI_PTR(g_facedet_activations_map[0] + 608);
    node_334_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 608);
    node_336_output_array.data = AI_PTR(g_facedet_activations_map[0] + 563488);
    node_336_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 563488);
    node_374_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_374_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_374_output_array.data = AI_PTR(g_facedet_activations_map[0] + 608);
    node_374_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 608);
    node_337_output_array.data = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_337_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_339_output_array.data = AI_PTR(g_facedet_activations_map[0] + 87008);
    node_339_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 87008);
    node_340_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_340_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_340_output_array.data = AI_PTR(g_facedet_activations_map[0] + 147808);
    node_340_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 147808);
    node_342_output_array.data = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_342_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_343_output_array.data = AI_PTR(g_facedet_activations_map[0] + 141408);
    node_343_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 141408);
    node_345_output_array.data = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_345_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_346_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 141408);
    node_346_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 141408);
    node_346_output_array.data = AI_PTR(g_facedet_activations_map[0] + 142560);
    node_346_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 142560);
    node_348_output_array.data = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_348_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_349_output_array.data = AI_PTR(g_facedet_activations_map[0] + 141408);
    node_349_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 141408);
    node_351_output_array.data = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_351_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_352_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 141408);
    node_352_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 141408);
    node_352_output_array.data = AI_PTR(g_facedet_activations_map[0] + 142560);
    node_352_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 142560);
    node_354_output_array.data = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_354_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_355_output_array.data = AI_PTR(g_facedet_activations_map[0] + 141408);
    node_355_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 141408);
    node_357_output_array.data = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_357_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_358_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 141408);
    node_358_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 141408);
    node_358_output_array.data = AI_PTR(g_facedet_activations_map[0] + 142560);
    node_358_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 142560);
    node_360_output_array.data = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_360_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_361_output_array.data = AI_PTR(g_facedet_activations_map[0] + 141408);
    node_361_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 141408);
    node_363_output_array.data = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_363_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_364_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 141408);
    node_364_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 141408);
    node_364_output_array.data = AI_PTR(g_facedet_activations_map[0] + 142560);
    node_364_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 142560);
    node_366_output_array.data = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_366_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_367_output_array.data = AI_PTR(g_facedet_activations_map[0] + 141408);
    node_367_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 141408);
    node_369_output_array.data = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_369_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_370_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 141408);
    node_370_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 141408);
    node_370_output_array.data = AI_PTR(g_facedet_activations_map[0] + 142560);
    node_370_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 142560);
    node_372_output_array.data = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_372_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_375_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 141408);
    node_375_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 141408);
    node_375_output_array.data = AI_PTR(g_facedet_activations_map[0] + 142560);
    node_375_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 142560);
    node_418_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_418_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_418_output_array.data = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_418_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 26208);
    node_394_output_array.data = AI_PTR(g_facedet_activations_map[0] + 32608);
    node_394_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 32608);
    node_395_output_array.data = AI_PTR(g_facedet_activations_map[0] + 58208);
    node_395_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 58208);
    node_417_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_417_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_417_output_array.data = AI_PTR(g_facedet_activations_map[0] + 576);
    node_417_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 576);
    node_414_output_array.data = AI_PTR(g_facedet_activations_map[0] + 83808);
    node_414_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 83808);
    node_415_output_array.data = AI_PTR(g_facedet_activations_map[0] + 186208);
    node_415_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 186208);
    node_416_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_416_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_416_output_array.data = AI_PTR(g_facedet_activations_map[0] + 32608);
    node_416_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 32608);
    node_425_output_array.data = AI_PTR(g_facedet_activations_map[0] + 135008);
    node_425_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 135008);
    node_427_output_array.data = AI_PTR(g_facedet_activations_map[0] + 237408);
    node_427_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 237408);
    node_428_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_428_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_428_output_array.data = AI_PTR(g_facedet_activations_map[0] + 339808);
    node_428_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 339808);
    node_430_output_array.data = AI_PTR(g_facedet_activations_map[0] + 749408);
    node_430_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 749408);
    node_431_output_array.data = AI_PTR(g_facedet_activations_map[0] + 135008);
    node_431_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 135008);
    node_433_output_array.data = AI_PTR(g_facedet_activations_map[0] + 544608);
    node_433_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 544608);
    node_434_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_434_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_434_output_array.data = AI_PTR(g_facedet_activations_map[0] + 135008);
    node_434_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 135008);
    node_436_output_array.data = AI_PTR(g_facedet_activations_map[0] + 544608);
    node_436_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 544608);
    node_439_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 135008);
    node_439_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 135008);
    node_439_output_array.data = AI_PTR(g_facedet_activations_map[0] + 137312);
    node_439_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 137312);
    node_447_reshape_in_078_to_chlast_output_array.data = AI_PTR(g_facedet_activations_map[0] + 265312);
    node_447_reshape_in_078_to_chlast_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 265312);
    node_449_to_chlast_output_array.data = AI_PTR(g_facedet_activations_map[0] + 135008);
    node_449_to_chlast_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 135008);
    node_438_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 263008);
    node_438_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 263008);
    node_438_output_array.data = AI_PTR(g_facedet_activations_map[0] + 265312);
    node_438_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 265312);
    node_444_reshape_in_081_to_chlast_output_array.data = AI_PTR(g_facedet_activations_map[0] + 316512);
    node_444_reshape_in_081_to_chlast_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 316512);
    node_446_to_chlast_output_array.data = AI_PTR(g_facedet_activations_map[0] + 263008);
    node_446_to_chlast_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 263008);
    node_437_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 314208);
    node_437_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 314208);
    node_437_output_array.data = AI_PTR(g_facedet_activations_map[0] + 316512);
    node_437_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 316512);
    node_440_reshape_in_084_to_chlast_output_array.data = AI_PTR(g_facedet_activations_map[0] + 329312);
    node_440_reshape_in_084_to_chlast_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 329312);
    node_442_to_chlast_output_array.data = AI_PTR(g_facedet_activations_map[0] + 314208);
    node_442_to_chlast_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 314208);
    node_443_output_array.data = AI_PTR(g_facedet_activations_map[0] + 327008);
    node_443_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 327008);
    node_419_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_419_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_419_output_array.data = AI_PTR(g_facedet_activations_map[0] + 339808);
    node_419_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 339808);
    node_420_output_array.data = AI_PTR(g_facedet_activations_map[0] + 32608);
    node_420_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 32608);
    node_423_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_423_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_423_output_array.data = AI_PTR(g_facedet_activations_map[0] + 576);
    node_423_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 576);
    node_450_output_array.data = AI_PTR(g_facedet_activations_map[0] + 58208);
    node_450_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 58208);
    node_452_output_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_452_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_453_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 25600);
    node_453_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 25600);
    node_453_output_array.data = AI_PTR(g_facedet_activations_map[0] + 339808);
    node_453_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 339808);
    node_455_output_array.data = AI_PTR(g_facedet_activations_map[0] + 442208);
    node_455_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 442208);
    node_456_output_array.data = AI_PTR(g_facedet_activations_map[0] + 339808);
    node_456_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 339808);
    node_458_output_array.data = AI_PTR(g_facedet_activations_map[0] + 442208);
    node_458_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 442208);
    node_459_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_459_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_459_output_array.data = AI_PTR(g_facedet_activations_map[0] + 339808);
    node_459_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 339808);
    node_461_output_array.data = AI_PTR(g_facedet_activations_map[0] + 442208);
    node_461_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 442208);
    node_464_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_464_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_464_output_array.data = AI_PTR(g_facedet_activations_map[0] + 58208);
    node_464_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 58208);
    node_472_reshape_in_099_to_chlast_output_array.data = AI_PTR(g_facedet_activations_map[0] + 90208);
    node_472_reshape_in_099_to_chlast_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 90208);
    node_474_to_chlast_output_array.data = AI_PTR(g_facedet_activations_map[0] + 58208);
    node_474_to_chlast_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 58208);
    node_463_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_463_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_463_output_array.data = AI_PTR(g_facedet_activations_map[0] + 2304);
    node_463_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 2304);
    node_469_reshape_in_0102_to_chlast_output_array.data = AI_PTR(g_facedet_activations_map[0] + 90208);
    node_469_reshape_in_0102_to_chlast_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 90208);
    node_471_to_chlast_output_array.data = AI_PTR(g_facedet_activations_map[0] + 0);
    node_471_to_chlast_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 0);
    node_462_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 12800);
    node_462_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 12800);
    node_462_output_array.data = AI_PTR(g_facedet_activations_map[0] + 15104);
    node_462_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 15104);
    node_465_reshape_in_0105_to_chlast_output_array.data = AI_PTR(g_facedet_activations_map[0] + 18304);
    node_465_reshape_in_0105_to_chlast_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 18304);
    node_467_to_chlast_output_array.data = AI_PTR(g_facedet_activations_map[0] + 12800);
    node_467_to_chlast_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 12800);
    node_468_output_array.data = AI_PTR(g_facedet_activations_map[0] + 16000);
    node_468_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 16000);
    node_421_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 12800);
    node_421_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 12800);
    node_421_output_array.data = AI_PTR(g_facedet_activations_map[0] + 19200);
    node_421_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 19200);
    node_422_output_array.data = AI_PTR(g_facedet_activations_map[0] + 32608);
    node_422_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 32608);
    node_424_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 12800);
    node_424_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 12800);
    node_424_output_array.data = AI_PTR(g_facedet_activations_map[0] + 19200);
    node_424_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 19200);
    node_475_output_array.data = AI_PTR(g_facedet_activations_map[0] + 25600);
    node_475_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 25600);
    node_477_output_array.data = AI_PTR(g_facedet_activations_map[0] + 19200);
    node_477_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 19200);
    node_478_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 12800);
    node_478_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 12800);
    node_478_output_array.data = AI_PTR(g_facedet_activations_map[0] + 25600);
    node_478_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 25600);
    node_480_output_array.data = AI_PTR(g_facedet_activations_map[0] + 90208);
    node_480_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 90208);
    node_481_output_array.data = AI_PTR(g_facedet_activations_map[0] + 19200);
    node_481_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 19200);
    node_483_output_array.data = AI_PTR(g_facedet_activations_map[0] + 90208);
    node_483_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 90208);
    node_484_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 12800);
    node_484_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 12800);
    node_484_output_array.data = AI_PTR(g_facedet_activations_map[0] + 19200);
    node_484_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 19200);
    node_486_output_array.data = AI_PTR(g_facedet_activations_map[0] + 90208);
    node_486_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 90208);
    node_489_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 12800);
    node_489_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 12800);
    node_489_output_array.data = AI_PTR(g_facedet_activations_map[0] + 19200);
    node_489_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 19200);
    node_497_reshape_in_0120_to_chlast_output_array.data = AI_PTR(g_facedet_activations_map[0] + 27200);
    node_497_reshape_in_0120_to_chlast_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 27200);
    node_499_to_chlast_output_array.data = AI_PTR(g_facedet_activations_map[0] + 19200);
    node_499_to_chlast_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 19200);
    node_488_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 12800);
    node_488_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 12800);
    node_488_output_array.data = AI_PTR(g_facedet_activations_map[0] + 27200);
    node_488_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 27200);
    node_494_reshape_in_0123_to_chlast_output_array.data = AI_PTR(g_facedet_activations_map[0] + 12800);
    node_494_reshape_in_0123_to_chlast_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 12800);
    node_496_to_chlast_output_array.data = AI_PTR(g_facedet_activations_map[0] + 27200);
    node_496_to_chlast_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 27200);
    node_487_scratch0_array.data = AI_PTR(g_facedet_activations_map[0] + 12800);
    node_487_scratch0_array.data_start = AI_PTR(g_facedet_activations_map[0] + 12800);
    node_487_output_array.data = AI_PTR(g_facedet_activations_map[0] + 15104);
    node_487_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 15104);
    node_490_reshape_in_0126_to_chlast_output_array.data = AI_PTR(g_facedet_activations_map[0] + 12800);
    node_490_reshape_in_0126_to_chlast_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 12800);
    node_492_to_chlast_output_array.data = AI_PTR(g_facedet_activations_map[0] + 13600);
    node_492_to_chlast_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 13600);
    node_493_output_array.data = AI_PTR(g_facedet_activations_map[0] + 12800);
    node_493_output_array.data_start = AI_PTR(g_facedet_activations_map[0] + 12800);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_ACTIVATIONS);
  return false;
}




/******************************************************************************/
AI_DECLARE_STATIC
ai_bool facedet_configure_weights(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_weights_map(g_facedet_weights_map, 1, params)) {
    /* Updating weights (byte) offsets */
    
    node_286_weights_array.format |= AI_FMT_FLAG_CONST;
    node_286_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 0);
    node_286_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 0);
    node_286_bias_array.format |= AI_FMT_FLAG_CONST;
    node_286_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 1728);
    node_286_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 1728);
    node_289_weights_array.format |= AI_FMT_FLAG_CONST;
    node_289_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 1792);
    node_289_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 1792);
    node_289_bias_array.format |= AI_FMT_FLAG_CONST;
    node_289_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2368);
    node_289_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2368);
    node_292_weights_array.format |= AI_FMT_FLAG_CONST;
    node_292_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2432);
    node_292_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2432);
    node_292_bias_array.format |= AI_FMT_FLAG_CONST;
    node_292_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 3456);
    node_292_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 3456);
    node_295_weights_array.format |= AI_FMT_FLAG_CONST;
    node_295_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 3520);
    node_295_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 3520);
    node_295_bias_array.format |= AI_FMT_FLAG_CONST;
    node_295_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 4096);
    node_295_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 4096);
    node_298_weights_array.format |= AI_FMT_FLAG_CONST;
    node_298_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 4160);
    node_298_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 4160);
    node_298_bias_array.format |= AI_FMT_FLAG_CONST;
    node_298_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 6720);
    node_298_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 6720);
    node_301_weights_array.format |= AI_FMT_FLAG_CONST;
    node_301_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 6880);
    node_301_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 6880);
    node_301_bias_array.format |= AI_FMT_FLAG_CONST;
    node_301_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 8320);
    node_301_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 8320);
    node_304_weights_array.format |= AI_FMT_FLAG_CONST;
    node_304_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 8480);
    node_304_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 8480);
    node_304_bias_array.format |= AI_FMT_FLAG_CONST;
    node_304_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 14880);
    node_304_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 14880);
    node_307_weights_array.format |= AI_FMT_FLAG_CONST;
    node_307_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 15040);
    node_307_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 15040);
    node_307_bias_array.format |= AI_FMT_FLAG_CONST;
    node_307_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 16480);
    node_307_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 16480);
    node_310_weights_array.format |= AI_FMT_FLAG_CONST;
    node_310_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 16640);
    node_310_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 16640);
    node_310_bias_array.format |= AI_FMT_FLAG_CONST;
    node_310_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 28160);
    node_310_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 28160);
    node_313_weights_array.format |= AI_FMT_FLAG_CONST;
    node_313_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 28448);
    node_313_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 28448);
    node_313_bias_array.format |= AI_FMT_FLAG_CONST;
    node_313_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 31040);
    node_313_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 31040);
    node_316_weights_array.format |= AI_FMT_FLAG_CONST;
    node_316_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 31328);
    node_316_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 31328);
    node_316_bias_array.format |= AI_FMT_FLAG_CONST;
    node_316_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 52064);
    node_316_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 52064);
    node_319_weights_array.format |= AI_FMT_FLAG_CONST;
    node_319_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 52352);
    node_319_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 52352);
    node_319_bias_array.format |= AI_FMT_FLAG_CONST;
    node_319_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 54944);
    node_319_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 54944);
    node_322_weights_array.format |= AI_FMT_FLAG_CONST;
    node_322_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 55232);
    node_322_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 55232);
    node_322_bias_array.format |= AI_FMT_FLAG_CONST;
    node_322_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 75968);
    node_322_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 75968);
    node_373_weights_array.format |= AI_FMT_FLAG_CONST;
    node_373_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 76256);
    node_373_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 76256);
    node_373_bias_array.format |= AI_FMT_FLAG_CONST;
    node_373_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 80864);
    node_373_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 80864);
    node_325_weights_array.format |= AI_FMT_FLAG_CONST;
    node_325_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 80928);
    node_325_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 80928);
    node_325_bias_array.format |= AI_FMT_FLAG_CONST;
    node_325_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 83520);
    node_325_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 83520);
    node_328_weights_array.format |= AI_FMT_FLAG_CONST;
    node_328_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 83808);
    node_328_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 83808);
    node_328_bias_array.format |= AI_FMT_FLAG_CONST;
    node_328_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 127584);
    node_328_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 127584);
    node_331_weights_array.format |= AI_FMT_FLAG_CONST;
    node_331_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 128192);
    node_331_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 128192);
    node_331_bias_array.format |= AI_FMT_FLAG_CONST;
    node_331_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 133664);
    node_331_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 133664);
    node_334_weights_array.format |= AI_FMT_FLAG_CONST;
    node_334_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 134272);
    node_334_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 134272);
    node_334_bias_array.format |= AI_FMT_FLAG_CONST;
    node_334_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 226688);
    node_334_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 226688);
    node_374_weights_array.format |= AI_FMT_FLAG_CONST;
    node_374_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 227296);
    node_374_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 227296);
    node_374_bias_array.format |= AI_FMT_FLAG_CONST;
    node_374_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 237024);
    node_374_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 237024);
    node_337_weights_array.format |= AI_FMT_FLAG_CONST;
    node_337_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 237088);
    node_337_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 237088);
    node_337_bias_array.format |= AI_FMT_FLAG_CONST;
    node_337_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 242560);
    node_337_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 242560);
    node_340_weights_array.format |= AI_FMT_FLAG_CONST;
    node_340_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 243168);
    node_340_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 243168);
    node_340_bias_array.format |= AI_FMT_FLAG_CONST;
    node_340_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 418272);
    node_340_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 418272);
    node_343_weights_array.format |= AI_FMT_FLAG_CONST;
    node_343_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 419424);
    node_343_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 419424);
    node_343_bias_array.format |= AI_FMT_FLAG_CONST;
    node_343_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 429792);
    node_343_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 429792);
    node_346_weights_array.format |= AI_FMT_FLAG_CONST;
    node_346_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 430944);
    node_346_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 430944);
    node_346_bias_array.format |= AI_FMT_FLAG_CONST;
    node_346_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 762720);
    node_346_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 762720);
    node_349_weights_array.format |= AI_FMT_FLAG_CONST;
    node_349_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 763872);
    node_349_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 763872);
    node_349_bias_array.format |= AI_FMT_FLAG_CONST;
    node_349_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 774240);
    node_349_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 774240);
    node_352_weights_array.format |= AI_FMT_FLAG_CONST;
    node_352_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 775392);
    node_352_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 775392);
    node_352_bias_array.format |= AI_FMT_FLAG_CONST;
    node_352_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 1107168);
    node_352_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 1107168);
    node_355_weights_array.format |= AI_FMT_FLAG_CONST;
    node_355_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 1108320);
    node_355_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 1108320);
    node_355_bias_array.format |= AI_FMT_FLAG_CONST;
    node_355_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 1118688);
    node_355_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 1118688);
    node_358_weights_array.format |= AI_FMT_FLAG_CONST;
    node_358_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 1119840);
    node_358_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 1119840);
    node_358_bias_array.format |= AI_FMT_FLAG_CONST;
    node_358_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 1451616);
    node_358_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 1451616);
    node_361_weights_array.format |= AI_FMT_FLAG_CONST;
    node_361_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 1452768);
    node_361_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 1452768);
    node_361_bias_array.format |= AI_FMT_FLAG_CONST;
    node_361_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 1463136);
    node_361_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 1463136);
    node_364_weights_array.format |= AI_FMT_FLAG_CONST;
    node_364_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 1464288);
    node_364_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 1464288);
    node_364_bias_array.format |= AI_FMT_FLAG_CONST;
    node_364_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 1796064);
    node_364_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 1796064);
    node_367_weights_array.format |= AI_FMT_FLAG_CONST;
    node_367_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 1797216);
    node_367_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 1797216);
    node_367_bias_array.format |= AI_FMT_FLAG_CONST;
    node_367_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 1807584);
    node_367_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 1807584);
    node_370_weights_array.format |= AI_FMT_FLAG_CONST;
    node_370_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 1808736);
    node_370_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 1808736);
    node_370_bias_array.format |= AI_FMT_FLAG_CONST;
    node_370_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2140512);
    node_370_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2140512);
    node_375_weights_array.format |= AI_FMT_FLAG_CONST;
    node_375_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2141664);
    node_375_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2141664);
    node_375_bias_array.format |= AI_FMT_FLAG_CONST;
    node_375_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2160096);
    node_375_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2160096);
    node_418_weights_array.format |= AI_FMT_FLAG_CONST;
    node_418_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2160160);
    node_418_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2160160);
    node_418_bias_array.format |= AI_FMT_FLAG_CONST;
    node_418_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2169376);
    node_418_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2169376);
    node_417_weights_array.format |= AI_FMT_FLAG_CONST;
    node_417_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2169440);
    node_417_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2169440);
    node_417_bias_array.format |= AI_FMT_FLAG_CONST;
    node_417_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2178656);
    node_417_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2178656);
    node_416_weights_array.format |= AI_FMT_FLAG_CONST;
    node_416_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2178720);
    node_416_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2178720);
    node_416_bias_array.format |= AI_FMT_FLAG_CONST;
    node_416_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2187936);
    node_416_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2187936);
    node_425_weights_array.format |= AI_FMT_FLAG_CONST;
    node_425_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2188000);
    node_425_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2188000);
    node_425_bias_array.format |= AI_FMT_FLAG_CONST;
    node_425_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2188576);
    node_425_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2188576);
    node_428_weights_array.format |= AI_FMT_FLAG_CONST;
    node_428_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2188640);
    node_428_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2188640);
    node_428_bias_array.format |= AI_FMT_FLAG_CONST;
    node_428_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2192736);
    node_428_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2192736);
    node_431_weights_array.format |= AI_FMT_FLAG_CONST;
    node_431_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2192992);
    node_431_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2192992);
    node_431_bias_array.format |= AI_FMT_FLAG_CONST;
    node_431_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2195296);
    node_431_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2195296);
    node_434_weights_array.format |= AI_FMT_FLAG_CONST;
    node_434_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2195552);
    node_434_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2195552);
    node_434_bias_array.format |= AI_FMT_FLAG_CONST;
    node_434_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2211936);
    node_434_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2211936);
    node_439_weights_array.format |= AI_FMT_FLAG_CONST;
    node_439_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2212192);
    node_439_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2212192);
    node_439_bias_array.format |= AI_FMT_FLAG_CONST;
    node_439_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2258272);
    node_439_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2258272);
    node_438_weights_array.format |= AI_FMT_FLAG_CONST;
    node_438_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2258352);
    node_438_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2258352);
    node_438_bias_array.format |= AI_FMT_FLAG_CONST;
    node_438_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2276784);
    node_438_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2276784);
    node_437_weights_array.format |= AI_FMT_FLAG_CONST;
    node_437_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2276816);
    node_437_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2276816);
    node_437_bias_array.format |= AI_FMT_FLAG_CONST;
    node_437_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2281424);
    node_437_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2281424);
    node_419_weights_array.format |= AI_FMT_FLAG_CONST;
    node_419_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2281432);
    node_419_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2281432);
    node_423_weights_array.format |= AI_FMT_FLAG_CONST;
    node_423_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2290648);
    node_423_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2290648);
    node_423_bias_array.format |= AI_FMT_FLAG_CONST;
    node_423_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2299864);
    node_423_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2299864);
    node_450_weights_array.format |= AI_FMT_FLAG_CONST;
    node_450_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2299928);
    node_450_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2299928);
    node_450_bias_array.format |= AI_FMT_FLAG_CONST;
    node_450_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2300504);
    node_450_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2300504);
    node_453_weights_array.format |= AI_FMT_FLAG_CONST;
    node_453_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2300568);
    node_453_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2300568);
    node_453_bias_array.format |= AI_FMT_FLAG_CONST;
    node_453_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2304664);
    node_453_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2304664);
    node_456_weights_array.format |= AI_FMT_FLAG_CONST;
    node_456_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2304920);
    node_456_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2304920);
    node_456_bias_array.format |= AI_FMT_FLAG_CONST;
    node_456_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2307224);
    node_456_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2307224);
    node_459_weights_array.format |= AI_FMT_FLAG_CONST;
    node_459_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2307480);
    node_459_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2307480);
    node_459_bias_array.format |= AI_FMT_FLAG_CONST;
    node_459_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2323864);
    node_459_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2323864);
    node_464_weights_array.format |= AI_FMT_FLAG_CONST;
    node_464_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2324120);
    node_464_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2324120);
    node_464_bias_array.format |= AI_FMT_FLAG_CONST;
    node_464_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2370200);
    node_464_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2370200);
    node_463_weights_array.format |= AI_FMT_FLAG_CONST;
    node_463_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2370280);
    node_463_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2370280);
    node_463_bias_array.format |= AI_FMT_FLAG_CONST;
    node_463_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2388712);
    node_463_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2388712);
    node_462_weights_array.format |= AI_FMT_FLAG_CONST;
    node_462_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2388744);
    node_462_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2388744);
    node_462_bias_array.format |= AI_FMT_FLAG_CONST;
    node_462_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2393352);
    node_462_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2393352);
    node_421_weights_array.format |= AI_FMT_FLAG_CONST;
    node_421_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2393360);
    node_421_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2393360);
    node_424_weights_array.format |= AI_FMT_FLAG_CONST;
    node_424_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2402576);
    node_424_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2402576);
    node_424_bias_array.format |= AI_FMT_FLAG_CONST;
    node_424_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2411792);
    node_424_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2411792);
    node_475_weights_array.format |= AI_FMT_FLAG_CONST;
    node_475_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2411856);
    node_475_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2411856);
    node_475_bias_array.format |= AI_FMT_FLAG_CONST;
    node_475_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2412432);
    node_475_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2412432);
    node_478_weights_array.format |= AI_FMT_FLAG_CONST;
    node_478_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2412496);
    node_478_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2412496);
    node_478_bias_array.format |= AI_FMT_FLAG_CONST;
    node_478_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2416592);
    node_478_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2416592);
    node_481_weights_array.format |= AI_FMT_FLAG_CONST;
    node_481_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2416848);
    node_481_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2416848);
    node_481_bias_array.format |= AI_FMT_FLAG_CONST;
    node_481_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2419152);
    node_481_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2419152);
    node_484_weights_array.format |= AI_FMT_FLAG_CONST;
    node_484_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2419408);
    node_484_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2419408);
    node_484_bias_array.format |= AI_FMT_FLAG_CONST;
    node_484_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2435792);
    node_484_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2435792);
    node_489_weights_array.format |= AI_FMT_FLAG_CONST;
    node_489_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2436048);
    node_489_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2436048);
    node_489_bias_array.format |= AI_FMT_FLAG_CONST;
    node_489_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2482128);
    node_489_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2482128);
    node_488_weights_array.format |= AI_FMT_FLAG_CONST;
    node_488_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2482208);
    node_488_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2482208);
    node_488_bias_array.format |= AI_FMT_FLAG_CONST;
    node_488_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2500640);
    node_488_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2500640);
    node_487_weights_array.format |= AI_FMT_FLAG_CONST;
    node_487_weights_array.data = AI_PTR(g_facedet_weights_map[0] + 2500672);
    node_487_weights_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2500672);
    node_487_bias_array.format |= AI_FMT_FLAG_CONST;
    node_487_bias_array.data = AI_PTR(g_facedet_weights_map[0] + 2505280);
    node_487_bias_array.data_start = AI_PTR(g_facedet_weights_map[0] + 2505280);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_WEIGHTS);
  return false;
}


/**  PUBLIC APIs SECTION  *****************************************************/



AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_facedet_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_FACEDET_MODEL_NAME,
      .model_signature   = AI_FACEDET_MODEL_SIGNATURE,
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
      
      .n_macc            = 187409506,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .params            = AI_STRUCT_INIT,
      .activations       = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0xd8cd5a83,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}



AI_API_ENTRY
ai_bool ai_facedet_get_report(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_FACEDET_MODEL_NAME,
      .model_signature   = AI_FACEDET_MODEL_SIGNATURE,
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
      
      .n_macc            = 187409506,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .map_signature     = AI_MAGIC_SIGNATURE,
      .map_weights       = AI_STRUCT_INIT,
      .map_activations   = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0xd8cd5a83,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}


AI_API_ENTRY
ai_error ai_facedet_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}


AI_API_ENTRY
ai_error ai_facedet_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    AI_CONTEXT_OBJ(&AI_NET_OBJ_INSTANCE),
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}


AI_API_ENTRY
ai_error ai_facedet_create_and_init(
  ai_handle* network, const ai_handle activations[], const ai_handle weights[])
{
  ai_error err;
  ai_network_params params;

  err = ai_facedet_create(network, AI_FACEDET_DATA_CONFIG);
  if (err.type != AI_ERROR_NONE) {
    return err;
  }
  
  if (ai_facedet_data_params_get(&params) != true) {
    err = ai_facedet_get_error(*network);
    return err;
  }
#if defined(AI_FACEDET_DATA_ACTIVATIONS_COUNT)
  /* set the addresses of the activations buffers */
  for (ai_u16 idx=0; activations && idx<params.map_activations.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_activations, idx, activations[idx]);
  }
#endif
#if defined(AI_FACEDET_DATA_WEIGHTS_COUNT)
  /* set the addresses of the weight buffers */
  for (ai_u16 idx=0; weights && idx<params.map_weights.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_weights, idx, weights[idx]);
  }
#endif
  if (ai_facedet_init(*network, &params) != true) {
    err = ai_facedet_get_error(*network);
  }
  return err;
}


AI_API_ENTRY
ai_buffer* ai_facedet_inputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_inputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_buffer* ai_facedet_outputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_outputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_handle ai_facedet_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}


AI_API_ENTRY
ai_bool ai_facedet_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = AI_NETWORK_OBJ(ai_platform_network_init(network, params));
  ai_bool ok = true;

  if (!net_ctx) return false;
  ok &= facedet_configure_weights(net_ctx, params);
  ok &= facedet_configure_activations(net_ctx, params);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_facedet_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}


AI_API_ENTRY
ai_i32 ai_facedet_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}



#undef AI_FACEDET_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

