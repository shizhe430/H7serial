/**
  ******************************************************************************
  * @file    network.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-04-27T12:03:38+0000
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

#include "ai_lite_inspect.h"
#include "ai_platform_interface.h"
#include "layers.h"
#include "core_convert.h"
#include "network.h"
#include "network_details.h"
#include "network_data.h"
#include "stai_events.h"

#include "lite_operators.h"

#include "ai_lite_inspect.h"
/*****************************************************************************/
#define STAI_INTERNAL_API_MAJOR               (1)
#define STAI_INTERNAL_API_MINOR               (0)
#define STAI_INTERNAL_API_MICRO               (0)

#define STAI_MAGIC                            (0xB1C00100)

/*****************************************************************************/
#define _STAI_CONCAT_ARG(a, b)     a ## b
#define STAI_CONCAT(a, b)         _STAI_CONCAT_ARG(a, b)

/*!  STAI_CAST SECTION                       *********************************/
#define STAI_CAST(type, expr) \
  ((type)(expr))


/*****************************************************************************/
#define STAI_SIZE(_size) \
  ((stai_size)(_size))

/*****************************************************************************/
#define STAI_INIT_BUFFER(_flags, _size, _address) \
  { \
    .size = (_size), \
    .address = (uintptr_t)(_address), \
    .flags = (_flags), \
  }

#define STAI_INIT_TENSOR(_name, _flags, _fmt, _size_bytes, _shape, _scale, _zeropoint) \
  { \
    .size_bytes = (_size_bytes), \
    .flags = (_flags), \
    .format = (stai_format)(_fmt), \
    .shape = STAI_PACK(_shape), \
    .scale = STAI_PACK(_scale), \
    .zeropoint = STAI_PACK(_zeropoint), \
    .name = (_name) \
  }

#define STAI_INIT_ARRAY(_size, _ptr) \
  { .size = STAI_SIZE(_size), .data = STAI_PACK(_ptr) }


#define STAI_CAST_ARRAY(_type, _size, _ptr) \
  { .size = STAI_SIZE(_size), .data = (_type)STAI_PACK(_ptr) }


#define STAI_DECLARE_ARRAY(_type, _size, ...) \
  { .size = STAI_SIZE(_size), .data = (_type[_size]) { STAI_PACK(__VA_ARGS__) } }


#define STAI_EMPTY_ARRAY() \
  { .size = 0, .data = NULL }


#define STAI_INIT_VERSION(_major, _minor, _micro) \
  { .major = (_major), .minor = (_minor), .micro = (_micro), .reserved = 0x0 }

/*****************************************************************************/
/**  Getters and setters  **/

#define STAI_GET_ARRAY_SIZE(nd_array) \
  (nd_array.size)


#define STAI_GET_ARRAY_ELEM(nd_array, pos) \
  (nd_array.data[(pos)])

#define _STAI_SET_ERROR(net_ctx, cond, value, exit) { \
  if (!(net_ctx)) { return STAI_ERROR_NETWORK_INVALID_CONTEXT_HANDLE; } \
  if (((uintptr_t)net_ctx) & (_STAI_CONTEXT_ALIGNMENT-1)) { return STAI_ERROR_NETWORK_INVALID_CONTEXT_ALIGNMENT; } \
  if (((value) >= STAI_ERROR_GENERIC) && (cond)) { \
    if ((net_ctx)->_return_code == STAI_SUCCESS) { \
      (net_ctx)->_return_code = (value); \
    } \
    return (exit); \
  } \
}

/*****************************************************************************/
/* TODO REMOVE THESE TWO MACROS */
#define STAI_EVENT_NODE_START_CB
#define STAI_EVENT_NODE_STOP_CB

#ifdef STAI_EVENT_NODE_START_CB
#ifndef _STAI_NETWORK_EVENT_NODE_START_CB
  #define _STAI_NETWORK_EVENT_NODE_START_CB(_node_id, _buffers_size, ...) \
  if (net_ctx->_callback) { \
    const stai_event_node_start_stop _start_event = { \
      .node_id=(_node_id), \
      .buffers={ \
        .size=(_buffers_size), \
        .data=(stai_ptr const*)(const stai_ptr[_buffers_size])STAI_PACK(__VA_ARGS__) \
      } \
    }; \
    net_ctx->_callback(net_ctx->_callback_cookie, STAI_EVENT_NODE_START, (const void*)&_start_event); \
  }
#endif
#else
  #define _STAI_NETWORK_EVENT_NODE_START_CB(_node_id, _buffers_size, ...) \
    do { /* _STAI_NETWORK_EVENT_NODE_START_CB() */ } while(0);
#endif      /* STAI_EVENT_NODE_START_CB */

#ifdef STAI_EVENT_NODE_STOP_CB
#ifndef _STAI_NETWORK_EVENT_NODE_STOP_CB
  #define _STAI_NETWORK_EVENT_NODE_STOP_CB(_node_id, _buffers_size, ...) \
  if (net_ctx->_callback) { \
    const stai_event_node_start_stop _stop_event = { \
      .node_id=(_node_id), \
      .buffers={ \
        .size=(_buffers_size), \
        .data=(stai_ptr const*)(stai_ptr[_buffers_size])STAI_PACK(__VA_ARGS__) \
      } \
    }; \
    net_ctx->_callback(net_ctx->_callback_cookie, STAI_EVENT_NODE_STOP, (const void*)&_stop_event); \
  }
#endif
#else
  #define _STAI_NETWORK_EVENT_NODE_STOP_CB(_node_id, _buffers_size, ...) \
    do { /* _STAI_NETWORK_EVENT_NODE_STOP_CB() */ } while(0);
#endif      /* STAI_EVENT_NODE_STOP_CB */


/*****************************************************************************/
#define _STAI_NETWORK_MODEL_SIGNATURE     "0x3a893f43c35154d963b66c5a4e979441"
#define _STAI_NETWORK_DATETIME            "2026-04-27T12:03:38+0000"
#define _STAI_NETWORK_COMPILE_DATETIME    __DATE__ " " __TIME__

#define _STAI_CONTEXT_ALIGNMENT        STAI_NETWORK_CONTEXT_ALIGNMENT

/*****************************************************************************/
#define g_network_activations_1     (NULL)




#if defined(HAVE_NETWORK_INFO)
/*****************************************************************************/
static const stai_network_info g_network_info = {
  .model_signature = _STAI_NETWORK_MODEL_SIGNATURE,
  .c_compile_datetime = _STAI_NETWORK_COMPILE_DATETIME,
  .c_model_name = STAI_NETWORK_MODEL_NAME,
  .c_model_datetime = _STAI_NETWORK_DATETIME,
  .c_model_signature = 0x0,
  .runtime_version = STAI_INIT_VERSION(12, 0, 0),
  .tool_version = STAI_INIT_VERSION(4, 0, 0),
  .api_version = STAI_INIT_VERSION(1, 0, 0),
  .n_macc = STAI_NETWORK_MACC_NUM,
  .n_nodes = STAI_NETWORK_NODES_NUM,
  .flags = STAI_NETWORK_FLAGS,
  .n_inputs = STAI_NETWORK_IN_NUM,
  .n_outputs = STAI_NETWORK_OUT_NUM,
  .n_activations = STAI_NETWORK_ACTIVATIONS_NUM,
  .n_weights = STAI_NETWORK_WEIGHTS_NUM,
  .n_states = STAI_NETWORK_STATES_NUM,
  .inputs = (stai_tensor[STAI_NETWORK_IN_NUM]) {
    STAI_INIT_TENSOR(
      STAI_NETWORK_IN_1_NAME,
      STAI_NETWORK_IN_1_FLAGS,
      STAI_NETWORK_IN_1_FORMAT,
      STAI_NETWORK_IN_1_SIZE_BYTES,
      STAI_DECLARE_ARRAY(int32_t, 4, 1, 1, 224, 224),
      STAI_DECLARE_ARRAY(float, 1, 0.003921544179320335f),
      STAI_DECLARE_ARRAY(int16_t, 1, -128)),
    },
    .outputs = (stai_tensor[STAI_NETWORK_OUT_NUM]) {
    STAI_INIT_TENSOR(
      STAI_NETWORK_OUT_1_NAME,
      STAI_NETWORK_OUT_1_FLAGS,
      STAI_NETWORK_OUT_1_FORMAT,
      STAI_NETWORK_OUT_1_SIZE_BYTES,
      STAI_DECLARE_ARRAY(int32_t, 2, 1, 5),
      STAI_DECLARE_ARRAY(float, 1, 0.17340436577796936f),
      STAI_DECLARE_ARRAY(int16_t, 1, 25)),
    },
  .activations = (stai_tensor[STAI_NETWORK_ACTIVATIONS_NUM]) {
    STAI_INIT_TENSOR(
      (NULL),
      STAI_NETWORK_ACTIVATION_1_FLAGS,
      STAI_FORMAT_U8,
      STAI_NETWORK_ACTIVATION_1_SIZE_BYTES,
      STAI_DECLARE_ARRAY(int32_t, 1, 323072),
      STAI_EMPTY_ARRAY(),
      STAI_EMPTY_ARRAY()),
    },
  .weights = (stai_tensor[STAI_NETWORK_WEIGHTS_NUM]) {
    STAI_INIT_TENSOR(
      (NULL),
      STAI_NETWORK_WEIGHT_1_FLAGS,
      STAI_FORMAT_U8,
      STAI_NETWORK_WEIGHT_1_SIZE_BYTES,
      STAI_DECLARE_ARRAY(int32_t, 1, 277092),
      STAI_EMPTY_ARRAY(),
      STAI_EMPTY_ARRAY()),
    },

  .states = NULL
};
#endif

#define _STAI_CONTEXT_ACQUIRE(_net_ctx, _net_handle) \
  _stai_network_context* _net_ctx = (_stai_network_context*)(_net_handle); \
  STAI_ASSERT(_net_ctx != NULL) \
  _STAI_SET_ERROR(_net_ctx, _net_ctx->_magic != STAI_MAGIC, \
                  STAI_ERROR_NETWORK_INVALID_CONTEXT_HANDLE, _net_ctx->_return_code)


/*****************************************************************************/
static
void _stai_network_check(_stai_network_context* net_ctx)
{
  stai_size idx;

// Check activations status
  for (idx=0; idx<STAI_NETWORK_ACTIVATIONS_NUM; idx++) {
    if (net_ctx->_activations[idx] == NULL) break;
  }
  net_ctx->_flags |= (idx == STAI_NETWORK_ACTIVATIONS_NUM) ? STAI_FLAG_ACTIVATIONS : STAI_FLAG_NONE;
// Check inputs status
  for (idx=0; idx<STAI_NETWORK_IN_NUM; idx++) {
    if (net_ctx->_inputs[idx] == NULL) break;
  }
  net_ctx->_flags |= (idx == STAI_NETWORK_IN_NUM) ? STAI_FLAG_INPUTS : STAI_FLAG_NONE;

  // Check outputs status
  for (idx=0; idx<STAI_NETWORK_OUT_NUM; idx++) {
    if (net_ctx->_outputs[idx] == NULL) break;
  }
  net_ctx->_flags |= (idx == STAI_NETWORK_OUT_NUM) ? STAI_FLAG_OUTPUTS : STAI_FLAG_NONE;

// Check weights status
  for (idx=0; idx<STAI_NETWORK_WEIGHTS_NUM; idx++) {
    if (net_ctx->_weights[idx] == NULL) break;
  }
  net_ctx->_flags |= (idx == STAI_NETWORK_WEIGHTS_NUM) ? STAI_FLAG_WEIGHTS : STAI_FLAG_NONE;
STAI_PRINT("  [_stai_network_check] flags: 0x%08x\n", net_ctx->_flags)
}


/*****************************************************************************/
STAI_API_ENTRY
stai_return_code stai_network_init(
  stai_network* network)
{
  /* Memory where to store internal context is provided by applications as a raw byte buffer */
  _stai_network_context* net_ctx = (_stai_network_context*)(network);
  net_ctx->_return_code = STAI_SUCCESS;
  STAI_PRINT("[Entering Network Init] network(%p) context_size(%d)\n", net_ctx, (int32_t)sizeof(_stai_network_context))

  _STAI_SET_ERROR(net_ctx, STAI_NETWORK_CONTEXT_SIZE != sizeof(_stai_network_context),
                 STAI_ERROR_NETWORK_INVALID_CONTEXT_SIZE, net_ctx->_return_code)

  {
    const _stai_network_context _network_context = {
      ._magic = STAI_MAGIC,
      ._signature = STAI_NETWORK_MODEL_SIGNATURE,
      ._flags = STAI_NETWORK_FLAGS,
      ._return_code = STAI_SUCCESS,
      ._callback = NULL,
      ._callback_cookie = NULL,
      ._activations = {
      (stai_ptr)g_network_activations_1
      },
      ._weights = {
      (stai_ptr)g_network_weights_array
      },
      ._inputs = {
    NULL},
      ._outputs = {
    NULL},
    };

    // Deep copy of internal context to opaque buffer provided by app
    *net_ctx = _network_context;

    _stai_network_check(net_ctx);
  }

  return net_ctx->_return_code;
}


STAI_API_ENTRY
stai_return_code stai_network_deinit(
  stai_network* network)
{
  _STAI_CONTEXT_ACQUIRE(net_ctx, network)

  /*  Reset flags to initial state  */
  net_ctx->_flags = STAI_NETWORK_FLAGS;
  return net_ctx->_return_code;
}

/*****************************************************************************/



/* Int quant #0 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(input_output_array_intq, AI_STATIC,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.003921544179320335f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #1 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(_feat_feat_1_Relu_output_0_output_array_intq, AI_STATIC,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.008772855624556541f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #2 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(_feat_feat_1_Relu_output_0_weights_array_intq, AI_STATIC,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 16,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0032464240211993456f, 0.002118445234373212f, 0.004448824096471071f, 0.0033384363632649183f, 0.004219577182084322f, 0.0029712968971580267f, 0.0021139902528375387f, 0.002838811371475458f, 0.003405176103115082f, 0.0019805554766207933f, 0.006785399280488491f, 0.0026317904703319073f, 0.002868874231353402f, 0.004483720753341913f, 0.0034072468988597393f, 0.00226279953494668f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #3 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(_feat_feat_1_Relu_output_0_scratch1_array_intq, AI_STATIC,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.008772855624556541f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #4 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(_feat_feat_4_Relu_output_0_pad_before_output_array_intq, AI_STATIC,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.008772855624556541f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #5 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(_feat_feat_4_Relu_output_0_output_array_intq, AI_STATIC,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.019329840317368507f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #6 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(_feat_feat_4_Relu_output_0_weights_array_intq, AI_STATIC,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 32,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.003506924258545041f, 0.0007482777000404894f, 0.0012786693405359983f, 0.0006621984648518264f, 0.0034784795716404915f, 0.0006990939727984369f, 0.0007364074699580669f, 0.0007759961881674826f, 0.0008545666933059692f, 0.0007252395153045654f, 0.0007907100371085107f, 0.0007260262500494719f, 0.0007807359215803444f, 0.0007509454153478146f, 0.0007767160423099995f, 0.0007527101552113891f, 0.0007827489171177149f, 0.003693788079544902f, 0.004275342915207148f, 0.0038049560971558094f, 0.00111609126906842f, 0.0008755088783800602f, 0.0015312182949855924f, 0.0010263656731694937f, 0.0009391489438712597f, 0.0030361174140125513f, 0.0007879719487391412f, 0.0037678966764360666f, 0.0026606940664350986f, 0.0031947519164532423f, 0.0007660527480766177f, 0.0017469662707298994f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #7 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(_feat_feat_4_Relu_output_0_scratch1_array_intq, AI_STATIC,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.019329840317368507f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #8 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(_feat_feat_9_Relu_output_0_output_array_intq, AI_STATIC,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.2251434624195099f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #9 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(_pool_AveragePool_output_0_output_array_intq, AI_STATIC,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.2251434624195099f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #10 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(_cls_cls_0_Flatten_output_0_to_chlast_output_array_intq, AI_STATIC,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.2251434624195099f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #11 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(_cls_cls_0_Flatten_output_0_0_conversion_output_array_intq, AI_STATIC,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.043284229934215546f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #12 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(_cls_cls_2_Relu_output_0_output_array_intq, AI_STATIC,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.1410362422466278f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #13 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(_cls_cls_2_Relu_output_0_weights_array_intq, AI_STATIC,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 128,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0002831848687492311f, 0.0006346790469251573f, 0.002309759147465229f, 0.002698768163099885f, 0.0002470344479661435f, 0.0002446177531965077f, 0.00029744417406618595f, 0.00033040271955542266f, 0.0029101474210619926f, 0.00034754411899484694f, 0.00024785532150417566f, 0.0028210601303726435f, 0.0002480384719092399f, 0.00030393744236789644f, 0.0002481886767782271f, 0.002844032598659396f, 0.0002469309256412089f, 0.0002477068919688463f, 0.0012212578440085053f, 0.0002621874737087637f, 0.002223529387265444f, 0.003041002433747053f, 0.00254289573058486f, 0.0032211763318628073f, 0.0026927602011710405f, 0.002419943455606699f, 0.002374237636104226f, 0.0003368946199771017f, 0.0021023759618401527f, 0.003356443950906396f, 0.00029147250461392105f, 0.002640517195686698f, 0.00026394875021651387f, 0.000567941926419735f, 0.0020609991624951363f, 0.0007353951805271208f, 0.0003468036011327058f, 0.0003526957589201629f, 0.000247582676820457f, 0.0016600192757323384f, 0.0029118340462446213f, 0.0030012985225766897f, 0.00044785559293814003f, 0.00042230047984048724f, 0.0003506247012410313f, 0.002437224145978689f, 0.00024738715728744864f, 0.00045246825902722776f, 0.00024754228070378304f, 0.0002481031115166843f, 0.0016568956198170781f, 0.00036809424636885524f, 0.00024241273058578372f, 0.0008898833184503019f, 0.0005160514265298843f, 0.00024816085351631045f, 0.000245993840508163f, 0.0011003219988197088f, 0.001600362011231482f, 0.002604771638289094f, 0.0002474795328453183f, 0.00024776984355412424f, 0.00025787443155422807f, 0.0003236612246837467f, 0.0002828695869538933f, 0.0002777879126369953f, 0.003393477061763406f, 0.00024734489852562547f, 0.0012958762235939503f, 0.00037293348577804863f, 0.0003518062294460833f, 0.0002809325815178454f, 0.0002481800620444119f, 0.0027261069044470787f, 0.0002478656533639878f, 0.0034266975708305836f, 0.0005399757064878941f, 0.00037960108602419496f, 0.0004955116310156882f, 0.00024180578475352377f, 0.0027988257352262735f, 0.00029395095771178603f, 0.00024736550403758883f, 0.0028433450497686863f, 0.00043462731991894543f, 0.0002762606309261173f, 0.00024814833886921406f, 0.00042473780922591686f, 0.0002652397088240832f, 0.0017674079863354564f, 0.00027597465668804944f, 0.0003498317673802376f, 0.00037082709604874253f, 0.0002759031776804477f, 0.0003058462461922318f, 0.003087697084993124f, 0.00024806210421957076f, 0.00024795145145617425f, 0.0028223232366144657f, 0.00042157291318289936f, 0.0005143006565049291f, 0.0003158106410410255f, 0.0021721194498240948f, 0.003773168195039034f, 0.0018937725108116865f, 0.0027542579919099808f, 0.0002654257696121931f, 0.00030555971898138523f, 0.0002471571206115186f, 0.0002475365763530135f, 0.00032390624983236194f, 0.002738409675657749f, 0.00030580279417335987f, 0.000246505398536101f, 0.003542175516486168f, 0.003315732581540942f, 0.0003741751133929938f, 0.002939814468845725f, 0.0002478565729688853f, 0.00027958914870396256f, 0.00029293776606209576f, 0.0002664577914401889f, 0.00038569592288695276f, 0.0002431402972433716f, 0.00027392481570132077f, 0.0002478423120919615f, 0.00479228887706995f, 0.00040845287730917335f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #14 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(output_QuantizeLinear_Input_output_array_intq, AI_STATIC,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.17340436577796936f),
    AI_PACK_INTQ_ZP(25)))

/* Int quant #15 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(output_QuantizeLinear_Input_weights_array_intq, AI_STATIC,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 5,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0022453744895756245f, 0.0030573608819395304f, 0.0022575834300369024f, 0.0028970169369131327f, 0.0032043992541730404f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0)))



/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  input_output_array, AI_ARRAY_FORMAT_S8|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 50176, AI_STATIC)

/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_1_Relu_output_0_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 200704, AI_STATIC)

/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_1_Relu_output_0_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 144, AI_STATIC)

/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_1_Relu_output_0_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 16, AI_STATIC)

/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_1_Relu_output_0_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 548, AI_STATIC)

/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_1_Relu_output_0_scratch1_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 7168, AI_STATIC)

/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_4_Relu_output_0_pad_before_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 207936, AI_STATIC)

/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_4_Relu_output_0_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100352, AI_STATIC)

/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_4_Relu_output_0_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 4608, AI_STATIC)

/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_4_Relu_output_0_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 32, AI_STATIC)

/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_4_Relu_output_0_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 6144, AI_STATIC)

/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_4_Relu_output_0_scratch1_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 7168, AI_STATIC)

/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  _feat_feat_9_Relu_output_0_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 301056, AI_STATIC)

/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  _pool_AveragePool_output_0_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1536, AI_STATIC)

/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  _cls_cls_0_Flatten_output_0_to_chlast_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1536, AI_STATIC)

/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  _cls_cls_0_Flatten_output_0_0_conversion_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1536, AI_STATIC)

/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  _cls_cls_2_Relu_output_0_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 128, AI_STATIC)

/* Array#17 */
AI_ARRAY_OBJ_DECLARE(
  _cls_cls_2_Relu_output_0_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 196608, AI_STATIC)

/* Array#18 */
AI_ARRAY_OBJ_DECLARE(
  _cls_cls_2_Relu_output_0_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 128, AI_STATIC)

/* Array#19 */
AI_ARRAY_OBJ_DECLARE(
  _cls_cls_2_Relu_output_0_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 2176, AI_STATIC)

/* Array#20 */
AI_ARRAY_OBJ_DECLARE(
  output_QuantizeLinear_Input_output_array, AI_ARRAY_FORMAT_S8|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 5, AI_STATIC)

/* Array#21 */
AI_ARRAY_OBJ_DECLARE(
  output_QuantizeLinear_Input_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 640, AI_STATIC)

/* Array#22 */
AI_ARRAY_OBJ_DECLARE(
  output_QuantizeLinear_Input_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 5, AI_STATIC)

/* Array#23 */
AI_ARRAY_OBJ_DECLARE(
  output_QuantizeLinear_Input_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 153, AI_STATIC)



/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_1_Relu_output_0_bias, AI_STATIC,
  7, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &_feat_feat_1_Relu_output_0_bias_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_1_Relu_output_0_output, AI_STATIC,
  8, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 112, 112), AI_STRIDE_INIT(4, 1, 1, 16, 1792),
  1, &_feat_feat_1_Relu_output_0_output_array, &_feat_feat_1_Relu_output_0_output_array_intq)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_1_Relu_output_0_scratch0, AI_STATIC,
  9, 0x0,
  AI_SHAPE_INIT(4, 1, 548, 1, 1), AI_STRIDE_INIT(4, 1, 1, 548, 548),
  1, &_feat_feat_1_Relu_output_0_scratch0_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_1_Relu_output_0_scratch1, AI_STATIC,
  10, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 224, 2), AI_STRIDE_INIT(4, 1, 1, 16, 3584),
  1, &_feat_feat_1_Relu_output_0_scratch1_array, &_feat_feat_1_Relu_output_0_scratch1_array_intq)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_1_Relu_output_0_weights, AI_STATIC,
  11, 0x1,
  AI_SHAPE_INIT(4, 1, 3, 3, 16), AI_STRIDE_INIT(4, 1, 1, 16, 48),
  1, &_feat_feat_1_Relu_output_0_weights_array, &_feat_feat_1_Relu_output_0_weights_array_intq)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  input_output, AI_STATIC,
  29, 0x1,
  AI_SHAPE_INIT(4, 1, 1, 224, 224), AI_STRIDE_INIT(4, 1, 1, 1, 224),
  1, &input_output_array, &input_output_array_intq)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_4_Relu_output_0_bias, AI_STATIC,
  12, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &_feat_feat_4_Relu_output_0_bias_array, NULL)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_4_Relu_output_0_output, AI_STATIC,
  13, 0x1,
  AI_SHAPE_INIT(4, 1, 32, 56, 56), AI_STRIDE_INIT(4, 1, 1, 32, 1792),
  1, &_feat_feat_4_Relu_output_0_output_array, &_feat_feat_4_Relu_output_0_output_array_intq)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_4_Relu_output_0_pad_before_output, AI_STATIC,
  14, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 114, 114), AI_STRIDE_INIT(4, 1, 1, 16, 1824),
  1, &_feat_feat_4_Relu_output_0_pad_before_output_array, &_feat_feat_4_Relu_output_0_pad_before_output_array_intq)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_4_Relu_output_0_scratch0, AI_STATIC,
  15, 0x0,
  AI_SHAPE_INIT(4, 1, 6144, 1, 1), AI_STRIDE_INIT(4, 1, 1, 6144, 6144),
  1, &_feat_feat_4_Relu_output_0_scratch0_array, NULL)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_4_Relu_output_0_scratch1, AI_STATIC,
  16, 0x1,
  AI_SHAPE_INIT(4, 1, 32, 112, 2), AI_STRIDE_INIT(4, 1, 1, 32, 3584),
  1, &_feat_feat_4_Relu_output_0_scratch1_array, &_feat_feat_4_Relu_output_0_scratch1_array_intq)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_4_Relu_output_0_weights, AI_STATIC,
  17, 0x1,
  AI_SHAPE_INIT(4, 16, 3, 3, 32), AI_STRIDE_INIT(4, 1, 16, 512, 1536),
  1, &_feat_feat_4_Relu_output_0_weights_array, &_feat_feat_4_Relu_output_0_weights_array_intq)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  _feat_feat_9_Relu_output_0_output, AI_STATIC,
  24, 0x1,
  AI_SHAPE_INIT(4, 1, 96, 56, 56), AI_STRIDE_INIT(4, 1, 1, 96, 5376),
  1, &_feat_feat_9_Relu_output_0_output_array, &_feat_feat_9_Relu_output_0_output_array_intq)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  _pool_AveragePool_output_0_output, AI_STATIC,
  28, 0x1,
  AI_SHAPE_INIT(4, 1, 96, 4, 4), AI_STRIDE_INIT(4, 1, 1, 96, 384),
  1, &_pool_AveragePool_output_0_output_array, &_pool_AveragePool_output_0_output_array_intq)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  _cls_cls_0_Flatten_output_0_to_chlast_output, AI_STATIC,
  1, 0x1,
  AI_SHAPE_INIT(4, 1, 4, 4, 96), AI_STRIDE_INIT(4, 1, 1, 4, 16),
  1, &_cls_cls_0_Flatten_output_0_to_chlast_output_array, &_cls_cls_0_Flatten_output_0_to_chlast_output_array_intq)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  _cls_cls_0_Flatten_output_0_0_conversion_output, AI_STATIC,
  0, 0x1,
  AI_SHAPE_INIT(4, 1, 1536, 1, 1), AI_STRIDE_INIT(4, 1, 1, 1536, 1536),
  1, &_cls_cls_0_Flatten_output_0_0_conversion_output_array, &_cls_cls_0_Flatten_output_0_0_conversion_output_array_intq)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  _cls_cls_0_Flatten_output_0_to_chlast_output0, AI_STATIC,
  2, 0x1,
  AI_SHAPE_INIT(4, 1, 1536, 1, 1), AI_STRIDE_INIT(4, 1, 1, 1536, 1536),
  1, &_cls_cls_0_Flatten_output_0_to_chlast_output_array, &_cls_cls_0_Flatten_output_0_to_chlast_output_array_intq)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  _cls_cls_2_Relu_output_0_bias, AI_STATIC,
  3, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &_cls_cls_2_Relu_output_0_bias_array, NULL)

/* Tensor #18 */
AI_TENSOR_OBJ_DECLARE(
  _cls_cls_2_Relu_output_0_output, AI_STATIC,
  4, 0x1,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 1, 1, 128, 128),
  1, &_cls_cls_2_Relu_output_0_output_array, &_cls_cls_2_Relu_output_0_output_array_intq)

/* Tensor #19 */
AI_TENSOR_OBJ_DECLARE(
  _cls_cls_2_Relu_output_0_scratch0, AI_STATIC,
  5, 0x0,
  AI_SHAPE_INIT(4, 1, 2176, 1, 1), AI_STRIDE_INIT(4, 2, 2, 4352, 4352),
  1, &_cls_cls_2_Relu_output_0_scratch0_array, NULL)

/* Tensor #20 */
AI_TENSOR_OBJ_DECLARE(
  _cls_cls_2_Relu_output_0_weights, AI_STATIC,
  6, 0x1,
  AI_SHAPE_INIT(4, 1536, 128, 1, 1), AI_STRIDE_INIT(4, 1, 1536, 196608, 196608),
  1, &_cls_cls_2_Relu_output_0_weights_array, &_cls_cls_2_Relu_output_0_weights_array_intq)

/* Tensor #21 */
AI_TENSOR_OBJ_DECLARE(
  output_QuantizeLinear_Input_bias, AI_STATIC,
  30, 0x0,
  AI_SHAPE_INIT(4, 1, 5, 1, 1), AI_STRIDE_INIT(4, 4, 4, 20, 20),
  1, &output_QuantizeLinear_Input_bias_array, NULL)

/* Tensor #22 */
AI_TENSOR_OBJ_DECLARE(
  output_QuantizeLinear_Input_output, AI_STATIC,
  31, 0x1,
  AI_SHAPE_INIT(4, 1, 5, 1, 1), AI_STRIDE_INIT(4, 1, 1, 5, 5),
  1, &output_QuantizeLinear_Input_output_array, &output_QuantizeLinear_Input_output_array_intq)

/* Tensor #23 */
AI_TENSOR_OBJ_DECLARE(
  output_QuantizeLinear_Input_scratch0, AI_STATIC,
  32, 0x0,
  AI_SHAPE_INIT(4, 1, 153, 1, 1), AI_STRIDE_INIT(4, 2, 2, 306, 306),
  1, &output_QuantizeLinear_Input_scratch0_array, NULL)

/* Tensor #24 */
AI_TENSOR_OBJ_DECLARE(
  output_QuantizeLinear_Input_weights, AI_STATIC,
  33, 0x1,
  AI_SHAPE_INIT(4, 128, 5, 1, 1), AI_STRIDE_INIT(4, 1, 128, 640, 640),
  1, &output_QuantizeLinear_Input_weights_array, &output_QuantizeLinear_Input_weights_array_intq)


AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_1_Relu_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &input_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_1_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_1_Relu_output_0_weights, &_feat_feat_1_Relu_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_1_Relu_output_0_scratch0, &_feat_feat_1_Relu_output_0_scratch1)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_1_Relu_output_0_layer, 18,
  OPTIMIZED_CONV2D_TYPE, 0x0, NULL,
  conv2d_nl_pool, forward_conv2d_sssa8_ch_nl_pool,
  &_feat_feat_1_Relu_output_0_chain,
  NULL, &_feat_feat_1_Relu_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .pool_size = AI_SHAPE_2D_INIT(2, 2), 
  .pool_stride = AI_SHAPE_2D_INIT(2, 2), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .pool_func = AI_HANDLE_PTR(pool_func_mp_array_integer_INT8), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _feat_feat_4_Relu_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_4_Relu_output_0_pad_before_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_4_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_feat_feat_4_Relu_output_0_weights, &_feat_feat_4_Relu_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_feat_feat_4_Relu_output_0_scratch0, &_feat_feat_4_Relu_output_0_scratch1)
)

AI_LAYER_OBJ_DECLARE(
  _feat_feat_4_Relu_output_0_layer, 24,
  OPTIMIZED_CONV2D_TYPE, 0x0, NULL,
  conv2d_nl_pool,  forward_conv2d_deep_3x3_sssa8_ch_nl_pool,
  &_feat_feat_4_Relu_output_0_chain,
  NULL, &_feat_feat_4_Relu_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .pool_size = AI_SHAPE_2D_INIT(2, 2), 
  .pool_stride = AI_SHAPE_2D_INIT(2, 2), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .pool_func = AI_HANDLE_PTR(pool_func_mp_array_integer_INT8), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _pool_AveragePool_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_feat_feat_9_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_pool_AveragePool_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _pool_AveragePool_output_0_layer, 33,
  POOL_TYPE, 0x0, NULL,
  pool, forward_ap_integer_INT8,
  &_pool_AveragePool_output_0_chain,
  NULL, &_pool_AveragePool_output_0_layer, AI_STATIC, 
  .pool_size = AI_SHAPE_2D_INIT(14, 14), 
  .pool_stride = AI_SHAPE_2D_INIT(14, 14), 
  .count_include_pad = 0, 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _cls_cls_0_Flatten_output_0_to_chlast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_pool_AveragePool_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_cls_cls_0_Flatten_output_0_to_chlast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _cls_cls_0_Flatten_output_0_to_chlast_layer, 36,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &_cls_cls_0_Flatten_output_0_to_chlast_chain,
  NULL, &_cls_cls_0_Flatten_output_0_to_chlast_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_HEIGHT, AI_SHAPE_CHANNEL, AI_SHAPE_DEPTH, AI_SHAPE_EXTENSION), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _cls_cls_0_Flatten_output_0_0_conversion_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_cls_cls_0_Flatten_output_0_to_chlast_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_cls_cls_0_Flatten_output_0_0_conversion_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _cls_cls_0_Flatten_output_0_0_conversion_layer, 36,
  NL_TYPE, 0x0, NULL,
  nl, node_convert_integer,
  &_cls_cls_0_Flatten_output_0_0_conversion_chain,
  NULL, &_cls_cls_0_Flatten_output_0_0_conversion_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _cls_cls_2_Relu_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_cls_cls_0_Flatten_output_0_0_conversion_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_cls_cls_2_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_cls_cls_2_Relu_output_0_weights, &_cls_cls_2_Relu_output_0_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_cls_cls_2_Relu_output_0_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  _cls_cls_2_Relu_output_0_layer, 39,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA_ch,
  &_cls_cls_2_Relu_output_0_chain,
  NULL, &_cls_cls_2_Relu_output_0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  output_QuantizeLinear_Input_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_cls_cls_2_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &output_QuantizeLinear_Input_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &output_QuantizeLinear_Input_weights, &output_QuantizeLinear_Input_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &output_QuantizeLinear_Input_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  output_QuantizeLinear_Input_layer, 42,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA_ch,
  &output_QuantizeLinear_Input_chain,
  NULL, &output_QuantizeLinear_Input_layer, AI_STATIC, 
)
/**  Hybrid layers declarations section  *************************************/
void forward_lite_conv2d_sssa8_ch_nl_pool__feat_feat_1_Relu_output_0(_stai_network_context* net_ctx)
{
  input_output_array.data = AI_PTR(net_ctx->_inputs[0] + 0);
  input_output_array.data_start = AI_PTR(net_ctx->_inputs[0] + 0);
  _feat_feat_1_Relu_output_0_weights_array.data = AI_PTR(net_ctx->_weights[0] + 0);
  _feat_feat_1_Relu_output_0_weights_array.data_start = AI_PTR(net_ctx->_weights[0] + 0);
  _feat_feat_1_Relu_output_0_bias_array.data = AI_PTR(net_ctx->_weights[0] + 144);
  _feat_feat_1_Relu_output_0_bias_array.data_start = AI_PTR(net_ctx->_weights[0] + 144);
  _feat_feat_1_Relu_output_0_scratch0_array.data = AI_PTR(net_ctx->_activations[0] + 224864);
  _feat_feat_1_Relu_output_0_scratch0_array.data_start = AI_PTR(net_ctx->_activations[0] + 224864);
  _feat_feat_1_Relu_output_0_scratch1_array.data = AI_PTR(net_ctx->_activations[0] + 225412);
  _feat_feat_1_Relu_output_0_scratch1_array.data_start = AI_PTR(net_ctx->_activations[0] + 225412);
  _feat_feat_1_Relu_output_0_output_array.data = AI_PTR(net_ctx->_activations[0] + 20544);
  _feat_feat_1_Relu_output_0_output_array.data_start = AI_PTR(net_ctx->_activations[0] + 20544);
  _STAI_NETWORK_EVENT_NODE_START_CB(18, 1, { input_output.data->data});
  forward_conv2d_sssa8_ch_nl_pool(&_feat_feat_1_Relu_output_0_layer);
  _STAI_NETWORK_EVENT_NODE_STOP_CB(18, 1, { _feat_feat_1_Relu_output_0_output.data->data});
}
void forward_lite_conv2d_deep_3x3_sssa8_ch_nl_pool__feat_feat_4_Relu_output_0(_stai_network_context* net_ctx)
{
  _feat_feat_4_Relu_output_0_pad_before_output_array.data = AI_PTR(net_ctx->_activations[0] + 13312);
  _feat_feat_4_Relu_output_0_pad_before_output_array.data_start = AI_PTR(net_ctx->_activations[0] + 13312);
  _feat_feat_4_Relu_output_0_weights_array.data = AI_PTR(net_ctx->_weights[0] + 208);
  _feat_feat_4_Relu_output_0_weights_array.data_start = AI_PTR(net_ctx->_weights[0] + 208);
  _feat_feat_4_Relu_output_0_bias_array.data = AI_PTR(net_ctx->_weights[0] + 4816);
  _feat_feat_4_Relu_output_0_bias_array.data_start = AI_PTR(net_ctx->_weights[0] + 4816);
  _feat_feat_4_Relu_output_0_scratch0_array.data = AI_PTR(net_ctx->_activations[0] + 7168);
  _feat_feat_4_Relu_output_0_scratch0_array.data_start = AI_PTR(net_ctx->_activations[0] + 7168);
  _feat_feat_4_Relu_output_0_scratch1_array.data = AI_PTR(net_ctx->_activations[0] + 0);
  _feat_feat_4_Relu_output_0_scratch1_array.data_start = AI_PTR(net_ctx->_activations[0] + 0);
  _feat_feat_4_Relu_output_0_output_array.data = AI_PTR(net_ctx->_activations[0] + 221248);
  _feat_feat_4_Relu_output_0_output_array.data_start = AI_PTR(net_ctx->_activations[0] + 221248);
  _STAI_NETWORK_EVENT_NODE_START_CB(24, 1, { _feat_feat_4_Relu_output_0_pad_before_output.data->data});
   forward_conv2d_deep_3x3_sssa8_ch_nl_pool(&_feat_feat_4_Relu_output_0_layer);
  _STAI_NETWORK_EVENT_NODE_STOP_CB(24, 1, { _feat_feat_4_Relu_output_0_output.data->data});
}
void forward_lite_ap_integer_INT8__pool_AveragePool_output_0(_stai_network_context* net_ctx)
{
  _feat_feat_9_Relu_output_0_output_array.data = AI_PTR(net_ctx->_activations[0] + 448);
  _feat_feat_9_Relu_output_0_output_array.data_start = AI_PTR(net_ctx->_activations[0] + 448);
  _pool_AveragePool_output_0_output_array.data = AI_PTR(net_ctx->_activations[0] + 301504);
  _pool_AveragePool_output_0_output_array.data_start = AI_PTR(net_ctx->_activations[0] + 301504);
  _STAI_NETWORK_EVENT_NODE_START_CB(33, 1, { _feat_feat_9_Relu_output_0_output.data->data});
  forward_ap_integer_INT8(&_pool_AveragePool_output_0_layer);
  _STAI_NETWORK_EVENT_NODE_STOP_CB(33, 1, { _pool_AveragePool_output_0_output.data->data});
}
void forward_lite_transpose__cls_cls_0_Flatten_output_0_to_chlast(_stai_network_context* net_ctx)
{
  _pool_AveragePool_output_0_output_array.data = AI_PTR(net_ctx->_activations[0] + 301504);
  _pool_AveragePool_output_0_output_array.data_start = AI_PTR(net_ctx->_activations[0] + 301504);
  _cls_cls_0_Flatten_output_0_to_chlast_output_array.data = AI_PTR(net_ctx->_activations[0] + 0);
  _cls_cls_0_Flatten_output_0_to_chlast_output_array.data_start = AI_PTR(net_ctx->_activations[0] + 0);
  _STAI_NETWORK_EVENT_NODE_START_CB(36, 1, { _pool_AveragePool_output_0_output.data->data});
  forward_transpose(&_cls_cls_0_Flatten_output_0_to_chlast_layer);
  _STAI_NETWORK_EVENT_NODE_STOP_CB(36, 1, { _cls_cls_0_Flatten_output_0_to_chlast_output.data->data});
}
void forward_lite_node_convert_integer__cls_cls_0_Flatten_output_0_0_conversion(_stai_network_context* net_ctx)
{
  _cls_cls_0_Flatten_output_0_to_chlast_output_array.data = AI_PTR(net_ctx->_activations[0] + 0);
  _cls_cls_0_Flatten_output_0_to_chlast_output_array.data_start = AI_PTR(net_ctx->_activations[0] + 0);
  _cls_cls_0_Flatten_output_0_0_conversion_output_array.data = AI_PTR(net_ctx->_activations[0] + 1536);
  _cls_cls_0_Flatten_output_0_0_conversion_output_array.data_start = AI_PTR(net_ctx->_activations[0] + 1536);
  _STAI_NETWORK_EVENT_NODE_START_CB(36, 1, { _cls_cls_0_Flatten_output_0_to_chlast_output0.data->data});
  node_convert_integer((ai_node *)&_cls_cls_0_Flatten_output_0_0_conversion_layer);
  _STAI_NETWORK_EVENT_NODE_STOP_CB(36, 1, { _cls_cls_0_Flatten_output_0_0_conversion_output.data->data});
}
void forward_lite_dense_integer_SSSA_ch__cls_cls_2_Relu_output_0(_stai_network_context* net_ctx)
{
  _cls_cls_0_Flatten_output_0_0_conversion_output_array.data = AI_PTR(net_ctx->_activations[0] + 1536);
  _cls_cls_0_Flatten_output_0_0_conversion_output_array.data_start = AI_PTR(net_ctx->_activations[0] + 1536);
  _cls_cls_2_Relu_output_0_weights_array.data = AI_PTR(net_ctx->_weights[0] + 79312);
  _cls_cls_2_Relu_output_0_weights_array.data_start = AI_PTR(net_ctx->_weights[0] + 79312);
  _cls_cls_2_Relu_output_0_bias_array.data = AI_PTR(net_ctx->_weights[0] + 275920);
  _cls_cls_2_Relu_output_0_bias_array.data_start = AI_PTR(net_ctx->_weights[0] + 275920);
  _cls_cls_2_Relu_output_0_scratch0_array.data = AI_PTR(net_ctx->_activations[0] + 3072);
  _cls_cls_2_Relu_output_0_scratch0_array.data_start = AI_PTR(net_ctx->_activations[0] + 3072);
  _cls_cls_2_Relu_output_0_output_array.data = AI_PTR(net_ctx->_activations[0] + 0);
  _cls_cls_2_Relu_output_0_output_array.data_start = AI_PTR(net_ctx->_activations[0] + 0);
  _STAI_NETWORK_EVENT_NODE_START_CB(39, 1, { _cls_cls_0_Flatten_output_0_0_conversion_output.data->data});
  forward_dense_integer_SSSA_ch(&_cls_cls_2_Relu_output_0_layer);
  _STAI_NETWORK_EVENT_NODE_STOP_CB(39, 1, { _cls_cls_2_Relu_output_0_output.data->data});
}
void forward_lite_dense_integer_SSSA_ch_output_QuantizeLinear_Input(_stai_network_context* net_ctx)
{
  _cls_cls_2_Relu_output_0_output_array.data = AI_PTR(net_ctx->_activations[0] + 0);
  _cls_cls_2_Relu_output_0_output_array.data_start = AI_PTR(net_ctx->_activations[0] + 0);
  output_QuantizeLinear_Input_weights_array.data = AI_PTR(net_ctx->_weights[0] + 276432);
  output_QuantizeLinear_Input_weights_array.data_start = AI_PTR(net_ctx->_weights[0] + 276432);
  output_QuantizeLinear_Input_bias_array.data = AI_PTR(net_ctx->_weights[0] + 277072);
  output_QuantizeLinear_Input_bias_array.data_start = AI_PTR(net_ctx->_weights[0] + 277072);
  output_QuantizeLinear_Input_scratch0_array.data = AI_PTR(net_ctx->_activations[0] + 128);
  output_QuantizeLinear_Input_scratch0_array.data_start = AI_PTR(net_ctx->_activations[0] + 128);
  output_QuantizeLinear_Input_output_array.data = AI_PTR(net_ctx->_outputs[0] + 0);
  output_QuantizeLinear_Input_output_array.data_start = AI_PTR(net_ctx->_outputs[0] + 0);
  _STAI_NETWORK_EVENT_NODE_START_CB(42, 1, { _cls_cls_2_Relu_output_0_output.data->data});
  forward_dense_integer_SSSA_ch(&output_QuantizeLinear_Input_layer);
  _STAI_NETWORK_EVENT_NODE_STOP_CB(42, 1, { output_QuantizeLinear_Input_output.data->data});
}

/*****************************************************************************/



static const ai_i8 _feat_feat_4_Relu_output_0_pad_before_v_pad_constant_value_const_s8[] = LITE_ARRAY_VALUES(-128);
static const ai_i16 _feat_feat_4_Relu_output_0_pad_before_t_in_0_fmt_bitsize_const_s16 = 8;
static const ai_u32 _feat_feat_4_Relu_output_0_pad_before_t_in_0_shape_h_const_u32 = 112;


static const ai_i8 _feat_feat_7_Relu_output_0_pad_before_v_pad_constant_value_const_s8[] = LITE_ARRAY_VALUES(-128);
static const ai_i16 _feat_feat_7_Relu_output_0_pad_before_t_in_0_fmt_bitsize_const_s16 = 8;
static const ai_u32 _feat_feat_7_Relu_output_0_pad_before_t_in_0_shape_h_const_u32 = 56;

static const ai_u16 _feat_feat_7_Relu_output_0_t_in_0_shape_w_const_u16 = 58;
static const ai_u16 _feat_feat_7_Relu_output_0_t_in_0_shape_h_const_u16 = 58;
static const ai_u16 _feat_feat_7_Relu_output_0_t_in_0_shape_ch_const_u16 = 32;
static const ai_u16 _feat_feat_7_Relu_output_0_t_out_0_shape_ch_const_u16 = 64;
static const ai_i8 _feat_feat_7_Relu_output_0_t_in_0_fmt_zero_const_s8 = -128;
static const ai_i8 _feat_feat_7_Relu_output_0_t_out_0_fmt_zero_const_s8 = -128;
static const ai_float _feat_feat_7_Relu_output_0_t_in_0_fmt_scale_const_f32 = 0.019329840317368507f;
static const ai_float _feat_feat_7_Relu_output_0_t_out_0_fmt_scale_const_f32 = 0.025555280968546867f;
static const ai_float _feat_feat_7_Relu_output_0_t_weight_0_fmt_scale_const_f32[] = LITE_ARRAY_VALUES(0.0007770026568323374f, 0.0008236558060161769f, 0.0006093218107707798f, 0.0005802895757369697f, 0.002195892622694373f, 0.0013117305934429169f, 0.0014515166403725743f, 0.002082322258502245f, 0.0036481316201388836f, 0.002393541391938925f, 0.0014876213390380144f, 0.0006580740446224809f, 0.0036661969497799873f, 0.0005827210261486471f, 0.0007004805374890566f, 0.0022218688391149044f, 0.00221612979657948f, 0.002316355239599943f, 0.0005011866451241076f, 0.002159710740670562f, 0.0010865640360862017f, 0.0023734087590128183f, 0.0017858626088127494f, 0.0006052357493899763f, 0.0006175685557536781f, 0.0008376695332117379f, 0.0045313844457268715f, 0.000609646609518677f, 0.002157394075766206f, 0.0006246544071473181f, 0.0033375155180692673f, 0.0014701467007398605f, 0.004535422660410404f, 0.0033189498353749514f, 0.0023865436669439077f, 0.0004913852317258716f, 0.0005807066336274147f, 0.0025345426984131336f, 0.0011945246951654553f, 0.0005716037121601403f, 0.003991453908383846f, 0.0022030328400433064f, 0.002613761695101857f, 0.0005996538093313575f, 0.002009798539802432f, 0.0012619765475392342f, 0.0022979904897511005f, 0.0019289426272735f, 0.002787297824397683f, 0.004531733226031065f, 0.0031456314027309418f, 0.0013439609901979566f, 0.00172212696634233f, 0.001281102653592825f, 0.0033973415847867727f, 0.0005127328331582248f, 0.0005093681975267828f, 0.0024004073347896338f, 0.001544128404930234f, 0.000817258725874126f, 0.0047235931269824505f, 0.0005680588074028492f, 0.0004966703127138317f, 0.0005204958142712712f);
static const ai_layer_format_type _feat_feat_7_Relu_output_0_l_out_ch_format_const_layer_format_type = AI_LAYER_FORMAT_CHANNEL_LAST_VALID;
static const ai_u16 _feat_feat_7_Relu_output_0_t_out_0_shape_w_const_u16 = 56;
static const ai_u16 _feat_feat_7_Relu_output_0_t_out_0_shape_h_const_u16 = 56;

static const ai_i8 _feat_feat_9_Relu_output_0_pad_before_v_pad_constant_value_const_s8[] = LITE_ARRAY_VALUES(-128);
static const ai_i16 _feat_feat_9_Relu_output_0_pad_before_t_in_0_fmt_bitsize_const_s16 = 8;
static const ai_u32 _feat_feat_9_Relu_output_0_pad_before_t_in_0_shape_h_const_u32 = 56;

static const ai_u16 _feat_feat_9_Relu_output_0_t_in_0_shape_w_const_u16 = 58;
static const ai_u16 _feat_feat_9_Relu_output_0_t_in_0_shape_h_const_u16 = 58;
static const ai_u16 _feat_feat_9_Relu_output_0_t_in_0_shape_ch_const_u16 = 64;
static const ai_u16 _feat_feat_9_Relu_output_0_t_out_0_shape_ch_const_u16 = 96;
static const ai_i8 _feat_feat_9_Relu_output_0_t_in_0_fmt_zero_const_s8 = -128;
static const ai_i8 _feat_feat_9_Relu_output_0_t_out_0_fmt_zero_const_s8 = -128;
static const ai_float _feat_feat_9_Relu_output_0_t_in_0_fmt_scale_const_f32 = 0.025555280968546867f;
static const ai_float _feat_feat_9_Relu_output_0_t_out_0_fmt_scale_const_f32 = 0.2251434624195099f;
static const ai_float _feat_feat_9_Relu_output_0_t_weight_0_fmt_scale_const_f32[] = LITE_ARRAY_VALUES(0.0009935114067047834f, 0.00047222658758983016f, 0.0005129111814312637f, 0.007382724434137344f, 0.0016407332150265574f, 0.0019943788647651672f, 0.0005545290769077837f, 0.0004800050228368491f, 0.0005217037396505475f, 0.0005019968375563622f, 0.002136574126780033f, 0.0007713030208833516f, 0.0051759397611021996f, 0.0005882216501049697f, 0.0006043718312866986f, 0.00299608102068305f, 0.0006434155511669815f, 0.002741761039942503f, 0.0005207756767049432f, 0.0005544525920413435f, 0.00047631506458856165f, 0.0048009054735302925f, 0.000549771822988987f, 0.002428683452308178f, 0.002259007887914777f, 0.0006173496367409825f, 0.0004644024884328246f, 0.0005339584313333035f, 0.00048485660227015615f, 0.0005272286362014711f, 0.0026129453908652067f, 0.0006949056987650692f, 0.0006152847781777382f, 0.0006750822649337351f, 0.0005364784155972302f, 0.000612631265539676f, 0.00303554511629045f, 0.0015014195814728737f, 0.00041303838952444494f, 0.0004977233475074172f, 0.0005762663204222918f, 0.0005103344446979463f, 0.002315904712304473f, 0.0005505207227542996f, 0.000725187361240387f, 0.0005363413947634399f, 0.000791326048783958f, 0.0005758681218139827f, 0.0008059454266913235f, 0.0006217736518010497f, 0.0005018803640268743f, 0.0022063530050218105f, 0.0007468463736586273f, 0.0005538794794119895f, 0.0021598688326776028f, 0.0052065677009522915f, 0.00046318036038428545f, 0.004063559230417013f, 0.0025133630260825157f, 0.0005337290349416435f, 0.00043429856305010617f, 0.0004852295096497983f, 0.00589425303041935f, 0.0006934417760930955f, 0.0004975534393452108f, 0.0006678036879748106f, 0.0006374307558871806f, 0.0008549785707145929f, 0.0005013292538933456f, 0.000702667748555541f, 0.003415329148992896f, 0.0013710827333852649f, 0.0004825782380066812f, 0.0011323831276968122f, 0.003380338428542018f, 0.000586575479246676f, 0.002572601195424795f, 0.004017654340714216f, 0.0015445726457983255f, 0.0009832585928961635f, 0.002860683249309659f, 0.0007412736886180937f, 0.0027400043327361345f, 0.0006790598854422569f, 0.0017544340807944536f, 0.0005826473934575915f, 0.0003917826106771827f, 0.005007877945899963f, 0.0026143784634768963f, 0.0031080853659659624f, 0.0006200448842719197f, 0.0023445235565304756f, 0.00047056295443326235f, 0.0005297203315421939f, 0.001194245065562427f, 0.0005275539588183165f);
static const ai_layer_format_type _feat_feat_9_Relu_output_0_l_out_ch_format_const_layer_format_type = AI_LAYER_FORMAT_CHANNEL_LAST_VALID;
static const ai_u16 _feat_feat_9_Relu_output_0_t_out_0_shape_w_const_u16 = 56;
static const ai_u16 _feat_feat_9_Relu_output_0_t_out_0_shape_h_const_u16 = 56;





STAI_API_ENTRY
stai_return_code stai_network_run(
  stai_network* network,
  const stai_run_mode mode)
{
   STAI_UNUSED(mode)
  _STAI_CONTEXT_ACQUIRE(net_ctx, network)

  _STAI_SET_ERROR(net_ctx, (net_ctx->_flags & STAI_FLAG_ACTIVATIONS) != STAI_FLAG_ACTIVATIONS,
        STAI_ERROR_NETWORK_INVALID_ACTIVATIONS_PTR, net_ctx->_return_code)

  _STAI_SET_ERROR(net_ctx, (net_ctx->_flags & STAI_FLAG_INPUTS) != STAI_FLAG_INPUTS,
                  STAI_ERROR_NETWORK_INVALID_IN_PTR, net_ctx->_return_code)
  _STAI_SET_ERROR(net_ctx, (net_ctx->_flags & STAI_FLAG_OUTPUTS) != STAI_FLAG_OUTPUTS,
                  STAI_ERROR_NETWORK_INVALID_OUT_PTR, net_ctx->_return_code)

  _STAI_SET_ERROR(net_ctx, (net_ctx->_flags & STAI_FLAG_WEIGHTS) != STAI_FLAG_WEIGHTS,
                  STAI_ERROR_NETWORK_INVALID_WEIGHTS_PTR, net_ctx->_return_code)


  /* LITE_KERNEL_SECTION BEGIN _feat_feat_1_Relu_output_0 */
  {
    
  forward_lite_conv2d_sssa8_ch_nl_pool__feat_feat_1_Relu_output_0(net_ctx);
  }
  /* LITE_KERNEL_SECTION END _feat_feat_1_Relu_output_0 */
  /* LITE_KERNEL_SECTION BEGIN _feat_feat_4_Relu_output_0_pad_before */
  {
      const ai_ptr _feat_feat_4_Relu_output_0_pad_before_t_in_0_ptr_const_ptr = (ai_ptr)(net_ctx->_activations[0] + 20544);
    ai_ptr _feat_feat_4_Relu_output_0_pad_before_t_out_0_ptr_ptr = (ai_ptr)(net_ctx->_activations[0] + 13312);
  
  _STAI_NETWORK_EVENT_NODE_START_CB(21, 1, {(stai_ptr) _feat_feat_4_Relu_output_0_pad_before_t_in_0_ptr_const_ptr});
    
  forward_lite_pad_constant(_feat_feat_4_Relu_output_0_pad_before_t_in_0_ptr_const_ptr, _feat_feat_4_Relu_output_0_pad_before_t_out_0_ptr_ptr, (ai_handle)(_feat_feat_4_Relu_output_0_pad_before_v_pad_constant_value_const_s8), _feat_feat_4_Relu_output_0_pad_before_t_in_0_fmt_bitsize_const_s16, _feat_feat_4_Relu_output_0_pad_before_t_in_0_shape_h_const_u32, (ai_i32)(1), (ai_i32)(1792), (ai_i32)(1824), (ai_i32)(1824), (ai_i32)(16), (ai_i32)(16));
    
  _STAI_NETWORK_EVENT_NODE_STOP_CB(21, 1, {(stai_ptr) _feat_feat_4_Relu_output_0_pad_before_t_out_0_ptr_ptr});
  }
  /* LITE_KERNEL_SECTION END _feat_feat_4_Relu_output_0_pad_before */
  /* LITE_KERNEL_SECTION BEGIN _feat_feat_4_Relu_output_0 */
  {
    
  forward_lite_conv2d_deep_3x3_sssa8_ch_nl_pool__feat_feat_4_Relu_output_0(net_ctx);
  }
  /* LITE_KERNEL_SECTION END _feat_feat_4_Relu_output_0 */
  /* LITE_KERNEL_SECTION BEGIN _feat_feat_7_Relu_output_0_pad_before */
  {
      const ai_ptr _feat_feat_7_Relu_output_0_pad_before_t_in_0_ptr_const_ptr = (ai_ptr)(net_ctx->_activations[0] + 221248);
    ai_ptr _feat_feat_7_Relu_output_0_pad_before_t_out_0_ptr_ptr = (ai_ptr)(net_ctx->_activations[0] + 213952);
  
  _STAI_NETWORK_EVENT_NODE_START_CB(27, 1, {(stai_ptr) _feat_feat_7_Relu_output_0_pad_before_t_in_0_ptr_const_ptr});
    
  forward_lite_pad_constant(_feat_feat_7_Relu_output_0_pad_before_t_in_0_ptr_const_ptr, _feat_feat_7_Relu_output_0_pad_before_t_out_0_ptr_ptr, (ai_handle)(_feat_feat_7_Relu_output_0_pad_before_v_pad_constant_value_const_s8), _feat_feat_7_Relu_output_0_pad_before_t_in_0_fmt_bitsize_const_s16, _feat_feat_7_Relu_output_0_pad_before_t_in_0_shape_h_const_u32, (ai_i32)(1), (ai_i32)(1792), (ai_i32)(1856), (ai_i32)(1856), (ai_i32)(32), (ai_i32)(32));
    
  _STAI_NETWORK_EVENT_NODE_STOP_CB(27, 1, {(stai_ptr) _feat_feat_7_Relu_output_0_pad_before_t_out_0_ptr_ptr});
  }
  /* LITE_KERNEL_SECTION END _feat_feat_7_Relu_output_0_pad_before */
  /* LITE_KERNEL_SECTION BEGIN _feat_feat_7_Relu_output_0 */
  {
      const ai_i8* _feat_feat_7_Relu_output_0_t_in_0_ptr_const_s8 = (ai_i8*)(net_ctx->_activations[0] + 213952);
    const ai_i8* _feat_feat_7_Relu_output_0_t_weight_0_ptr_const_s8 = (ai_i8*)(net_ctx->_weights[0] + 4944);
    const ai_i32* _feat_feat_7_Relu_output_0_t_weight_1_ptr_const_s32 = (ai_i32*)(net_ctx->_weights[0] + 23376);
    ai_i8* _feat_feat_7_Relu_output_0_t_out_0_ptr_s8 = (ai_i8*)(net_ctx->_activations[0] + 113600);
    ai_i16* _feat_feat_7_Relu_output_0_t_scratch_0_ptr_s16 = (ai_i16*)(net_ctx->_activations[0] + 0);
  
  _STAI_NETWORK_EVENT_NODE_START_CB(27, 1, {(stai_ptr) _feat_feat_7_Relu_output_0_t_in_0_ptr_const_s8});
    
  forward_lite_conv2d_deep_3x3_sssa8_ch(_feat_feat_7_Relu_output_0_t_in_0_ptr_const_s8, _feat_feat_7_Relu_output_0_t_in_0_shape_w_const_u16, _feat_feat_7_Relu_output_0_t_in_0_shape_h_const_u16, _feat_feat_7_Relu_output_0_t_in_0_shape_ch_const_u16, _feat_feat_7_Relu_output_0_t_weight_0_ptr_const_s8, _feat_feat_7_Relu_output_0_t_out_0_shape_ch_const_u16, _feat_feat_7_Relu_output_0_t_weight_1_ptr_const_s32, _feat_feat_7_Relu_output_0_t_in_0_fmt_zero_const_s8, _feat_feat_7_Relu_output_0_t_out_0_fmt_zero_const_s8, _feat_feat_7_Relu_output_0_t_in_0_fmt_scale_const_f32, _feat_feat_7_Relu_output_0_t_out_0_fmt_scale_const_f32, _feat_feat_7_Relu_output_0_t_weight_0_fmt_scale_const_f32, _feat_feat_7_Relu_output_0_l_out_ch_format_const_layer_format_type, _feat_feat_7_Relu_output_0_t_out_0_ptr_s8, _feat_feat_7_Relu_output_0_t_out_0_shape_w_const_u16, _feat_feat_7_Relu_output_0_t_out_0_shape_h_const_u16, 1, 7168, _feat_feat_7_Relu_output_0_t_scratch_0_ptr_s16);
    
  _STAI_NETWORK_EVENT_NODE_STOP_CB(27, 1, {(stai_ptr) _feat_feat_7_Relu_output_0_t_out_0_ptr_s8});
  }
  /* LITE_KERNEL_SECTION END _feat_feat_7_Relu_output_0 */
  /* LITE_KERNEL_SECTION BEGIN _feat_feat_9_Relu_output_0_pad_before */
  {
      const ai_ptr _feat_feat_9_Relu_output_0_pad_before_t_in_0_ptr_const_ptr = (ai_ptr)(net_ctx->_activations[0] + 113600);
    ai_ptr _feat_feat_9_Relu_output_0_pad_before_t_out_0_ptr_ptr = (ai_ptr)(net_ctx->_activations[0] + 99008);
  
  _STAI_NETWORK_EVENT_NODE_START_CB(30, 1, {(stai_ptr) _feat_feat_9_Relu_output_0_pad_before_t_in_0_ptr_const_ptr});
    
  forward_lite_pad_constant(_feat_feat_9_Relu_output_0_pad_before_t_in_0_ptr_const_ptr, _feat_feat_9_Relu_output_0_pad_before_t_out_0_ptr_ptr, (ai_handle)(_feat_feat_9_Relu_output_0_pad_before_v_pad_constant_value_const_s8), _feat_feat_9_Relu_output_0_pad_before_t_in_0_fmt_bitsize_const_s16, _feat_feat_9_Relu_output_0_pad_before_t_in_0_shape_h_const_u32, (ai_i32)(1), (ai_i32)(3584), (ai_i32)(3712), (ai_i32)(3712), (ai_i32)(64), (ai_i32)(64));
    
  _STAI_NETWORK_EVENT_NODE_STOP_CB(30, 1, {(stai_ptr) _feat_feat_9_Relu_output_0_pad_before_t_out_0_ptr_ptr});
  }
  /* LITE_KERNEL_SECTION END _feat_feat_9_Relu_output_0_pad_before */
  /* LITE_KERNEL_SECTION BEGIN _feat_feat_9_Relu_output_0 */
  {
      const ai_i8* _feat_feat_9_Relu_output_0_t_in_0_ptr_const_s8 = (ai_i8*)(net_ctx->_activations[0] + 99008);
    const ai_i8* _feat_feat_9_Relu_output_0_t_weight_0_ptr_const_s8 = (ai_i8*)(net_ctx->_weights[0] + 23632);
    const ai_i32* _feat_feat_9_Relu_output_0_t_weight_1_ptr_const_s32 = (ai_i32*)(net_ctx->_weights[0] + 78928);
    ai_i8* _feat_feat_9_Relu_output_0_t_out_0_ptr_s8 = (ai_i8*)(net_ctx->_activations[0] + 448);
    ai_i16* _feat_feat_9_Relu_output_0_t_scratch_0_ptr_s16 = (ai_i16*)(net_ctx->_activations[0] + 314304);
  
  _STAI_NETWORK_EVENT_NODE_START_CB(30, 1, {(stai_ptr) _feat_feat_9_Relu_output_0_t_in_0_ptr_const_s8});
    
  forward_lite_conv2d_deep_3x3_sssa8_ch(_feat_feat_9_Relu_output_0_t_in_0_ptr_const_s8, _feat_feat_9_Relu_output_0_t_in_0_shape_w_const_u16, _feat_feat_9_Relu_output_0_t_in_0_shape_h_const_u16, _feat_feat_9_Relu_output_0_t_in_0_shape_ch_const_u16, _feat_feat_9_Relu_output_0_t_weight_0_ptr_const_s8, _feat_feat_9_Relu_output_0_t_out_0_shape_ch_const_u16, _feat_feat_9_Relu_output_0_t_weight_1_ptr_const_s32, _feat_feat_9_Relu_output_0_t_in_0_fmt_zero_const_s8, _feat_feat_9_Relu_output_0_t_out_0_fmt_zero_const_s8, _feat_feat_9_Relu_output_0_t_in_0_fmt_scale_const_f32, _feat_feat_9_Relu_output_0_t_out_0_fmt_scale_const_f32, _feat_feat_9_Relu_output_0_t_weight_0_fmt_scale_const_f32, _feat_feat_9_Relu_output_0_l_out_ch_format_const_layer_format_type, _feat_feat_9_Relu_output_0_t_out_0_ptr_s8, _feat_feat_9_Relu_output_0_t_out_0_shape_w_const_u16, _feat_feat_9_Relu_output_0_t_out_0_shape_h_const_u16, 1, 8768, _feat_feat_9_Relu_output_0_t_scratch_0_ptr_s16);
    
  _STAI_NETWORK_EVENT_NODE_STOP_CB(30, 1, {(stai_ptr) _feat_feat_9_Relu_output_0_t_out_0_ptr_s8});
  }
  /* LITE_KERNEL_SECTION END _feat_feat_9_Relu_output_0 */
  /* LITE_KERNEL_SECTION BEGIN _pool_AveragePool_output_0 */
  {
    
  forward_lite_ap_integer_INT8__pool_AveragePool_output_0(net_ctx);
  }
  /* LITE_KERNEL_SECTION END _pool_AveragePool_output_0 */
  /* LITE_KERNEL_SECTION BEGIN _cls_cls_0_Flatten_output_0_to_chlast */
  {
    
  forward_lite_transpose__cls_cls_0_Flatten_output_0_to_chlast(net_ctx);
  }
  /* LITE_KERNEL_SECTION END _cls_cls_0_Flatten_output_0_to_chlast */
  /* LITE_KERNEL_SECTION BEGIN _cls_cls_0_Flatten_output_0_0_conversion */
  {
    
  forward_lite_node_convert_integer__cls_cls_0_Flatten_output_0_0_conversion(net_ctx);
  }
  /* LITE_KERNEL_SECTION END _cls_cls_0_Flatten_output_0_0_conversion */
  /* LITE_KERNEL_SECTION BEGIN _cls_cls_2_Relu_output_0 */
  {
    
  forward_lite_dense_integer_SSSA_ch__cls_cls_2_Relu_output_0(net_ctx);
  }
  /* LITE_KERNEL_SECTION END _cls_cls_2_Relu_output_0 */
  /* LITE_KERNEL_SECTION BEGIN output_QuantizeLinear_Input */
  {
    
  forward_lite_dense_integer_SSSA_ch_output_QuantizeLinear_Input(net_ctx);
  }
  /* LITE_KERNEL_SECTION END output_QuantizeLinear_Input */
  return net_ctx->_return_code;
}

/*****************************************************************************/
/*  Getters APIs Section  */
STAI_API_ENTRY
stai_size stai_network_get_context_size()
{
  return (stai_size)STAI_NETWORK_CONTEXT_SIZE;
}

#if defined(HAVE_NETWORK_INFO)
STAI_API_ENTRY
stai_return_code stai_network_get_info(
  stai_network* network,
  stai_network_info* info)
{
  _STAI_CONTEXT_ACQUIRE(net_ctx, network)
  _STAI_SET_ERROR(net_ctx, info==NULL, STAI_ERROR_NETWORK_INVALID_INFO, net_ctx->_return_code)

  // Copy of network info struct
  *info = g_network_info;

  return STAI_SUCCESS;
}
#endif


STAI_API_ENTRY
stai_return_code stai_network_get_activations(
  stai_network* network, stai_ptr* activations, stai_size* n_activations)
{
  _STAI_CONTEXT_ACQUIRE(net_ctx, network)

  _STAI_SET_ERROR(net_ctx, !n_activations, STAI_ERROR_NETWORK_INVALID_API_ARGUMENTS, net_ctx->_return_code)
  *n_activations = STAI_NETWORK_ACTIVATIONS_NUM;
for (stai_size idx=0; activations && (idx<STAI_NETWORK_ACTIVATIONS_NUM); idx++) {
    // get address of the activations buffers
    activations[idx] = net_ctx->_activations[idx];
  }return net_ctx->_return_code;
}


STAI_API_ENTRY
stai_return_code stai_network_get_weights(
  stai_network* network, stai_ptr* weights, stai_size* n_weights)
{
  _STAI_CONTEXT_ACQUIRE(net_ctx, network)
  _STAI_SET_ERROR(net_ctx, !n_weights, STAI_ERROR_NETWORK_INVALID_API_ARGUMENTS, net_ctx->_return_code)
  *n_weights = STAI_NETWORK_WEIGHTS_NUM;
for (stai_size idx=0; weights && (idx<STAI_NETWORK_WEIGHTS_NUM); idx++) {
    // get address of the weights buffers
    weights[idx] = net_ctx->_weights[idx];
  }return net_ctx->_return_code;
}


STAI_API_ENTRY
stai_return_code stai_network_get_inputs(
  stai_network* network, stai_ptr* inputs, stai_size* n_inputs)
{
  _STAI_CONTEXT_ACQUIRE(net_ctx, network)
  _STAI_SET_ERROR(net_ctx, !n_inputs, STAI_ERROR_NETWORK_INVALID_API_ARGUMENTS, net_ctx->_return_code)
  *n_inputs = STAI_NETWORK_IN_NUM;
  for (stai_size idx=0; inputs && (idx<STAI_NETWORK_IN_NUM); idx++) {
    inputs[idx] = net_ctx->_inputs[idx];
  }
  return net_ctx->_return_code;
}


STAI_API_ENTRY
stai_return_code stai_network_get_outputs(
  stai_network* network, stai_ptr* outputs, stai_size* n_outputs)
{
  _STAI_CONTEXT_ACQUIRE(net_ctx, network)
  _STAI_SET_ERROR(net_ctx, !n_outputs, STAI_ERROR_NETWORK_INVALID_API_ARGUMENTS, net_ctx->_return_code)
  *n_outputs = STAI_NETWORK_OUT_NUM;
  for (stai_size idx=0; outputs && (idx<STAI_NETWORK_OUT_NUM); idx++) {
    outputs[idx] = net_ctx->_outputs[idx];
  }
  return net_ctx->_return_code;
}


STAI_API_ENTRY
stai_return_code stai_network_get_error(
  stai_network* network)
{
  _STAI_CONTEXT_ACQUIRE(net_ctx, network)

  /* return 1st generated error or STAI_SUCCESS if no errors so far */
  return net_ctx->_return_code;
}


STAI_API_ENTRY
stai_return_code stai_network_get_states(
  stai_network* network, stai_ptr* states, stai_size* n_states)
{
  _STAI_CONTEXT_ACQUIRE(net_ctx, network)
  _STAI_SET_ERROR(net_ctx, !n_states, STAI_ERROR_NETWORK_INVALID_API_ARGUMENTS, net_ctx->_return_code)
  /* get the number of internals states (supporting multi-heap also for internal states) */
  *n_states = STAI_NETWORK_STATES_NUM;

  STAI_UNUSED(states)
return net_ctx->_return_code;
}


/*****************************************************************************/
/*  Setters APIs Section  */

STAI_API_ENTRY
stai_return_code stai_network_set_activations(
  stai_network* network,
  const stai_ptr* activations,
  const stai_size n_activations)
{
  _STAI_CONTEXT_ACQUIRE(net_ctx, network)
const uintptr_t _activations_alignment[] = STAI_NETWORK_ACTIVATIONS_ALIGNMENTS;
  STAI_PRINT("  [stai_network_set_activations] network(%p) activations[%d]: %p\n\n", net_ctx, n_activations, activations)
  _STAI_SET_ERROR(net_ctx, !activations,
                  STAI_ERROR_NETWORK_INVALID_API_ARGUMENTS, net_ctx->_return_code)
  _STAI_SET_ERROR(net_ctx, n_activations!=STAI_NETWORK_ACTIVATIONS_NUM,
                  STAI_ERROR_NETWORK_INVALID_ACTIVATIONS_NUM, net_ctx->_return_code)

  for (stai_size idx=0; activations && idx<STAI_NETWORK_ACTIVATIONS_NUM; idx++) {
    STAI_PRINT("  activation[%d]: %p\n", idx, activations[idx])
    _STAI_SET_ERROR(net_ctx, activations[idx]==NULL,
                    STAI_ERROR_NETWORK_INVALID_ACTIVATIONS_PTR, net_ctx->_return_code)
    _STAI_SET_ERROR(net_ctx, ((uintptr_t)activations[idx]) & (_activations_alignment[idx]-1),
                    STAI_ERROR_INVALID_BUFFER_ALIGNMENT, net_ctx->_return_code)
    net_ctx->_activations[idx] = activations[idx];
  }
  net_ctx->_inputs[0] = activations[0] + 174688;

  net_ctx->_outputs[0] = activations[0] + 436;
_stai_network_check(net_ctx);
  return net_ctx->_return_code;
}


STAI_API_ENTRY
stai_return_code stai_network_set_weights(
  stai_network* network,
  const stai_ptr* weights,
  const stai_size n_weights)
{
  _STAI_CONTEXT_ACQUIRE(net_ctx, network)
const uintptr_t _weights_alignment[] = STAI_NETWORK_WEIGHTS_ALIGNMENTS;
  _STAI_SET_ERROR(net_ctx, !weights,
                  STAI_ERROR_NETWORK_INVALID_API_ARGUMENTS, net_ctx->_return_code)
  _STAI_SET_ERROR(net_ctx, n_weights!=STAI_NETWORK_WEIGHTS_NUM,
                  STAI_ERROR_NETWORK_INVALID_WEIGHTS_NUM, net_ctx->_return_code)
  for (stai_size idx=0; weights && idx<STAI_NETWORK_WEIGHTS_NUM; idx++) {
    STAI_PRINT("  weight[%d]: %p\n", idx, weights[idx])
    _STAI_SET_ERROR(net_ctx, weights[idx]==NULL,
                    STAI_ERROR_NETWORK_INVALID_WEIGHTS_PTR, net_ctx->_return_code)
    _STAI_SET_ERROR(net_ctx, ((uintptr_t)weights[idx]) & (_weights_alignment[idx]-1),
                    STAI_ERROR_INVALID_BUFFER_ALIGNMENT, net_ctx->_return_code)
    net_ctx->_weights[idx] = weights[idx];
  }_stai_network_check(net_ctx);
  return net_ctx->_return_code;
}


STAI_API_ENTRY
stai_return_code stai_network_set_inputs(
  stai_network* network,
  const stai_ptr* inputs,
  const stai_size n_inputs)
{
  const uintptr_t _inputs_alignment[] = STAI_NETWORK_IN_ALIGNMENTS;
  _STAI_CONTEXT_ACQUIRE(net_ctx, network)
  _STAI_SET_ERROR(net_ctx, !inputs,
                  STAI_ERROR_NETWORK_INVALID_API_ARGUMENTS, net_ctx->_return_code)
  _STAI_SET_ERROR(net_ctx, n_inputs!=STAI_NETWORK_IN_NUM,
                  STAI_ERROR_NETWORK_INVALID_IN_NUM, net_ctx->_return_code)

  for (stai_size idx=0; inputs && idx<STAI_NETWORK_IN_NUM; idx++) {
    STAI_PRINT("  input[%d]: %p\n", idx, inputs[idx])
    _STAI_SET_ERROR(net_ctx, inputs[idx]==NULL,
                    STAI_ERROR_NETWORK_INVALID_IN_PTR, net_ctx->_return_code)
    _STAI_SET_ERROR(net_ctx, ((uintptr_t)inputs[idx]) & (_inputs_alignment[idx]-1),
                    STAI_ERROR_INVALID_BUFFER_ALIGNMENT, net_ctx->_return_code)
    net_ctx->_inputs[idx] = inputs[idx];
  }

  _stai_network_check(net_ctx);
  return net_ctx->_return_code;
}


STAI_API_ENTRY
stai_return_code stai_network_set_outputs(
  stai_network* network,
  const stai_ptr* outputs,
  const stai_size n_outputs)
{
  const uintptr_t _outputs_alignment[] = STAI_NETWORK_OUT_ALIGNMENTS;
  _STAI_CONTEXT_ACQUIRE(net_ctx, network)
  _STAI_SET_ERROR(net_ctx, !outputs,
                  STAI_ERROR_NETWORK_INVALID_API_ARGUMENTS, net_ctx->_return_code)
  _STAI_SET_ERROR(net_ctx, n_outputs!=STAI_NETWORK_OUT_NUM,
                  STAI_ERROR_NETWORK_INVALID_OUT_NUM, net_ctx->_return_code)

  for (stai_size idx=0; outputs && idx<n_outputs; idx++) {
    STAI_PRINT("  output[%d]: %p\n", idx, outputs[idx])
    _STAI_SET_ERROR(net_ctx, outputs[idx]==NULL,
                    STAI_ERROR_NETWORK_INVALID_OUT_PTR, net_ctx->_return_code)
    _STAI_SET_ERROR(net_ctx, ((uintptr_t)outputs[idx]) & (_outputs_alignment[idx]-1),
                    STAI_ERROR_INVALID_BUFFER_ALIGNMENT, net_ctx->_return_code)
    net_ctx->_outputs[idx] = outputs[idx];
  }

  _stai_network_check(net_ctx);
  return net_ctx->_return_code;
}


STAI_API_ENTRY
stai_return_code stai_network_set_states(
  stai_network* network,
  const stai_ptr* states,
  const stai_size n_states)
{
  _STAI_CONTEXT_ACQUIRE(net_ctx, network)

  STAI_UNUSED(states)
  STAI_UNUSED(n_states)
_stai_network_check(net_ctx);
  return net_ctx->_return_code;
}

STAI_API_ENTRY
stai_return_code stai_network_set_callback(
  stai_network* network, const stai_event_cb cb, void* cb_cookie)
{
  _STAI_CONTEXT_ACQUIRE(net_ctx, network)
  STAI_PRINT("  set_callback %p cb %p cookie %p\n", net_ctx, cb, cb_cookie)
  // _STAI_SET_ERROR(net_ctx, cb==NULL, STAI_ERROR_NETWORK_INVALID_CALLBACK, net_ctx->_return_code)
  net_ctx->_callback = cb;
  net_ctx->_callback_cookie = cb_cookie;
  return net_ctx->_return_code;
}

#undef _STAI_SET_ERROR
#undef _STAI_CONTEXT_ALIGNMENT
#undef _STAI_CONTEXT_ACQUIRE
#undef _STAI_NETWORK_EVENT_NODE_START_CB
#undef _STAI_NETWORK_EVENT_NODE_STOP_CB
#undef _STAI_NETWORK_MODEL_SIGNATURE
#undef _STAI_NETWORK_DATETIME
#undef _STAI_NETWORK_COMPILE_DATETIME

