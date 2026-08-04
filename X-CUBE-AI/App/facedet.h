/**
  ******************************************************************************
  * @file    facedet.h
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
#ifndef AI_FACEDET_H
#define AI_FACEDET_H

#include "facedet_config.h"
#include "ai_platform.h"

/******************************************************************************/
#define AI_FACEDET_MODEL_NAME          "facedet"
#define AI_FACEDET_ORIGIN_MODEL_NAME   "det_500m_320_fixed"

/******************************************************************************/
#define AI_FACEDET_ACTIVATIONS_ALIGNMENT   (4)
#define AI_FACEDET_INPUTS_IN_ACTIVATIONS   (4)
#define AI_FACEDET_OUTPUTS_IN_ACTIVATIONS  (4)

/******************************************************************************/
#define AI_FACEDET_IN_NUM        (1)

AI_DEPRECATED
#define AI_FACEDET_IN \
  ai_facedet_inputs_get(AI_HANDLE_NULL, NULL)

#define AI_FACEDET_IN_SIZE { \
  AI_FACEDET_IN_1_SIZE, \
}
#define AI_FACEDET_IN_SIZE_BYTES { \
  AI_FACEDET_IN_1_SIZE_BYTES, \
}
#define AI_FACEDET_IN_1_FORMAT      (AI_BUFFER_FORMAT_FLOAT)
#define AI_FACEDET_IN_1_HEIGHT      (320)
#define AI_FACEDET_IN_1_WIDTH       (3)
#define AI_FACEDET_IN_1_CHANNEL     (320)
#define AI_FACEDET_IN_1_SIZE        (307200)
#define AI_FACEDET_IN_1_SIZE_BYTES  (1228800)

/******************************************************************************/
#define AI_FACEDET_OUT_NUM       (9)

AI_DEPRECATED
#define AI_FACEDET_OUT \
  ai_facedet_outputs_get(AI_HANDLE_NULL, NULL)

#define AI_FACEDET_OUT_SIZE { \
  AI_FACEDET_OUT_1_SIZE, \
  AI_FACEDET_OUT_2_SIZE, \
  AI_FACEDET_OUT_3_SIZE, \
  AI_FACEDET_OUT_4_SIZE, \
  AI_FACEDET_OUT_5_SIZE, \
  AI_FACEDET_OUT_6_SIZE, \
  AI_FACEDET_OUT_7_SIZE, \
  AI_FACEDET_OUT_8_SIZE, \
  AI_FACEDET_OUT_9_SIZE, \
}
#define AI_FACEDET_OUT_SIZE_BYTES { \
  AI_FACEDET_OUT_1_SIZE_BYTES, \
  AI_FACEDET_OUT_2_SIZE_BYTES, \
  AI_FACEDET_OUT_3_SIZE_BYTES, \
  AI_FACEDET_OUT_4_SIZE_BYTES, \
  AI_FACEDET_OUT_5_SIZE_BYTES, \
  AI_FACEDET_OUT_6_SIZE_BYTES, \
  AI_FACEDET_OUT_7_SIZE_BYTES, \
  AI_FACEDET_OUT_8_SIZE_BYTES, \
  AI_FACEDET_OUT_9_SIZE_BYTES, \
}
#define AI_FACEDET_OUT_1_FORMAT      (AI_BUFFER_FORMAT_FLOAT)
#define AI_FACEDET_OUT_1_HEIGHT      (1)
#define AI_FACEDET_OUT_1_CHANNEL     (3200)
#define AI_FACEDET_OUT_1_SIZE        (3200)
#define AI_FACEDET_OUT_1_SIZE_BYTES  (12800)
#define AI_FACEDET_OUT_2_FORMAT      (AI_BUFFER_FORMAT_FLOAT)
#define AI_FACEDET_OUT_2_HEIGHT      (1)
#define AI_FACEDET_OUT_2_CHANNEL     (800)
#define AI_FACEDET_OUT_2_SIZE        (800)
#define AI_FACEDET_OUT_2_SIZE_BYTES  (3200)
#define AI_FACEDET_OUT_3_FORMAT      (AI_BUFFER_FORMAT_FLOAT)
#define AI_FACEDET_OUT_3_HEIGHT      (1)
#define AI_FACEDET_OUT_3_CHANNEL     (200)
#define AI_FACEDET_OUT_3_SIZE        (200)
#define AI_FACEDET_OUT_3_SIZE_BYTES  (800)
#define AI_FACEDET_OUT_4_FORMAT      (AI_BUFFER_FORMAT_FLOAT)
#define AI_FACEDET_OUT_4_HEIGHT      (8)
#define AI_FACEDET_OUT_4_WIDTH       (40)
#define AI_FACEDET_OUT_4_DEPTH       (40)
#define AI_FACEDET_OUT_4_CHANNEL     (1)
#define AI_FACEDET_OUT_4_SIZE        (12800)
#define AI_FACEDET_OUT_4_SIZE_BYTES  (51200)
#define AI_FACEDET_OUT_5_FORMAT      (AI_BUFFER_FORMAT_FLOAT)
#define AI_FACEDET_OUT_5_HEIGHT      (8)
#define AI_FACEDET_OUT_5_WIDTH       (20)
#define AI_FACEDET_OUT_5_DEPTH       (20)
#define AI_FACEDET_OUT_5_CHANNEL     (1)
#define AI_FACEDET_OUT_5_SIZE        (3200)
#define AI_FACEDET_OUT_5_SIZE_BYTES  (12800)
#define AI_FACEDET_OUT_6_FORMAT      (AI_BUFFER_FORMAT_FLOAT)
#define AI_FACEDET_OUT_6_HEIGHT      (8)
#define AI_FACEDET_OUT_6_WIDTH       (10)
#define AI_FACEDET_OUT_6_DEPTH       (10)
#define AI_FACEDET_OUT_6_CHANNEL     (1)
#define AI_FACEDET_OUT_6_SIZE        (800)
#define AI_FACEDET_OUT_6_SIZE_BYTES  (3200)
#define AI_FACEDET_OUT_7_FORMAT      (AI_BUFFER_FORMAT_FLOAT)
#define AI_FACEDET_OUT_7_HEIGHT      (20)
#define AI_FACEDET_OUT_7_WIDTH       (40)
#define AI_FACEDET_OUT_7_DEPTH       (40)
#define AI_FACEDET_OUT_7_CHANNEL     (1)
#define AI_FACEDET_OUT_7_SIZE        (32000)
#define AI_FACEDET_OUT_7_SIZE_BYTES  (128000)
#define AI_FACEDET_OUT_8_FORMAT      (AI_BUFFER_FORMAT_FLOAT)
#define AI_FACEDET_OUT_8_HEIGHT      (20)
#define AI_FACEDET_OUT_8_WIDTH       (20)
#define AI_FACEDET_OUT_8_DEPTH       (20)
#define AI_FACEDET_OUT_8_CHANNEL     (1)
#define AI_FACEDET_OUT_8_SIZE        (8000)
#define AI_FACEDET_OUT_8_SIZE_BYTES  (32000)
#define AI_FACEDET_OUT_9_FORMAT      (AI_BUFFER_FORMAT_FLOAT)
#define AI_FACEDET_OUT_9_HEIGHT      (20)
#define AI_FACEDET_OUT_9_WIDTH       (10)
#define AI_FACEDET_OUT_9_DEPTH       (10)
#define AI_FACEDET_OUT_9_CHANNEL     (1)
#define AI_FACEDET_OUT_9_SIZE        (2000)
#define AI_FACEDET_OUT_9_SIZE_BYTES  (8000)

/******************************************************************************/
#define AI_FACEDET_N_NODES (129)


AI_API_DECLARE_BEGIN

/*!
 * @defgroup facedet
 * @brief Public neural network APIs
 * @details This is the header for the network public APIs declarations
 * for interfacing a generated network model.
 * @details The public neural network APIs hide the structure of the network
 * and offer a set of interfaces to create, initialize, query, configure, 
 * run and destroy a network instance.
 * To handle this, an opaque handler to the network context is provided 
 * on creation.
 * The APIs are meant as stadard interfaces for the calling code; depending on
 * the supported platforms and the models, different implementations could be
 * available.
 */

/******************************************************************************/
/*! Public API Functions Declarations */

/*!
 * @brief Get network library info as a datastruct.
 * @ingroup facedet
 * @param[in] network: the handler to the network context
 * @param[out] report a pointer to the report struct where to
 * store network info. See @ref ai_network_report struct for details
 * @return a boolean reporting the exit status of the API
 */
AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_facedet_get_info(
  ai_handle network, ai_network_report* report);



/*!
 * @brief Get network library report as a datastruct.
 * @ingroup facedet
 * @param[in] network: the handler to the network context
 * @param[out] report a pointer to the report struct where to
 * store network info. See @ref ai_network_report struct for details
 * @return a boolean reporting the exit status of the API
 */
AI_API_ENTRY
ai_bool ai_facedet_get_report(
  ai_handle network, ai_network_report* report);


/*!
 * @brief Get first network error code.
 * @ingroup facedet
 * @details Get an error code related to the 1st error generated during
 * network processing. The error code is structure containing an 
 * error type indicating the type of error with an associated error code
 * Note: after this call the error code is internally reset to AI_ERROR_NONE
 * @param network an opaque handle to the network context
 * @return an error type/code pair indicating both the error type and code
 * see @ref ai_error for struct definition
 */
AI_API_ENTRY
ai_error ai_facedet_get_error(ai_handle network);


/*!
 * @brief Create a neural network.
 * @ingroup facedet
 * @details Instantiate a network and returns an object to handle it;
 * @param network an opaque handle to the network context
 * @param network_config a pointer to the network configuration info coded as a 
 * buffer
 * @return an error code reporting the status of the API on exit
 */
AI_API_ENTRY
ai_error ai_facedet_create(
  ai_handle* network, const ai_buffer* network_config);


/*!
 * @brief Destroy a neural network and frees the allocated memory.
 * @ingroup facedet
 * @details Destroys the network and frees its memory. The network handle is returned;
 * if the handle is not NULL, the unloading has not been successful.
 * @param network an opaque handle to the network context
 * @return an object handle : AI_HANDLE_NULL if network was destroyed
 * correctly. The same input network handle if destroy failed.
 */
AI_API_ENTRY
ai_handle ai_facedet_destroy(ai_handle network);


/*!
 * @brief Initialize the data structures of the network.
 * @ingroup facedet
 * @details This API initialized the network after a successfull
 * @ref ai_facedet_create. Both the activations memory buffer 
 * and params (i.e. weights) need to be provided by caller application
 * 
 * @param network an opaque handle to the network context
 * @param params the parameters of the network (required). 
 * see @ref ai_network_params struct for details
 * @return true if the network was correctly initialized, false otherwise
 * in case of error the error type could be queried by 
 * using @ref ai_facedet_get_error
 */
AI_API_ENTRY
ai_bool ai_facedet_init(
  ai_handle network, const ai_network_params* params);


/*!
 * @brief Create and initialize a neural network (helper function)
 * @ingroup facedet
 * @details Helper function to instantiate and to initialize a network. It returns an object to handle it;
 * @param network an opaque handle to the network context
 * @param activations array of addresses of the activations buffers
 * @param weights array of addresses of the weights buffers
 * @return an error code reporting the status of the API on exit
 */
AI_API_ENTRY
ai_error ai_facedet_create_and_init(
  ai_handle* network, const ai_handle activations[], const ai_handle weights[]);


/*!
 * @brief Get network inputs array pointer as a ai_buffer array pointer.
 * @ingroup facedet
 * @param network an opaque handle to the network context
 * @param n_buffer optional parameter to return the number of outputs
 * @return a ai_buffer pointer to the inputs arrays
 */
AI_API_ENTRY
ai_buffer* ai_facedet_inputs_get(
  ai_handle network, ai_u16 *n_buffer);


/*!
 * @brief Get network outputs array pointer as a ai_buffer array pointer.
 * @ingroup facedet
 * @param network an opaque handle to the network context
 * @param n_buffer optional parameter to return the number of outputs
 * @return a ai_buffer pointer to the outputs arrays
 */
AI_API_ENTRY
ai_buffer* ai_facedet_outputs_get(
  ai_handle network, ai_u16 *n_buffer);


/*!
 * @brief Run the network and return the output
 * @ingroup facedet
 *
 * @details Runs the network on the inputs and returns the corresponding output.
 * The size of the input and output buffers is stored in this
 * header generated by the code generation tool. See AI_FACEDET_*
 * defines into file @ref facedet.h for all network sizes defines
 *
 * @param network an opaque handle to the network context
 * @param[in] input buffer with the input data
 * @param[out] output buffer with the output data
 * @return the number of input batches processed (default 1) or <= 0 if it fails
 * in case of error the error type could be queried by 
 * using @ref ai_facedet_get_error
 */
AI_API_ENTRY
ai_i32 ai_facedet_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output);


/*!
 * @brief Runs the network on the inputs.
 * @ingroup facedet
 *
 * @details Differently from @ref ai_network_run, no output is returned, e.g. for
 * temporal models with a fixed step size.
 *
 * @param network the network to be run
 * @param[in] input buffer with the input data
 * @return the number of input batches processed (usually 1) or <= 0 if it fails
 * in case of error the error type could be queried by 
 * using @ref ai_facedet_get_error
 */
AI_API_ENTRY
ai_i32 ai_facedet_forward(
  ai_handle network, const ai_buffer* input);

AI_API_DECLARE_END

#endif /* AI_FACEDET_H */
