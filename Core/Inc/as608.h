#ifndef __AS608_H
#define __AS608_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define AS608_DEFAULT_ADDRESS   0xFFFFFFFFUL
#define AS608_DEFAULT_CAPACITY  300U
#define AS608_CHAR_BUFFER1      0x01U
#define AS608_CHAR_BUFFER2      0x02U
#define AS608_LAST_RX_MAX       64U

typedef struct
{
  uint16_t page_id;
  uint16_t match_score;
} AS608_SearchResult;

typedef struct
{
  uint16_t capacity;
  uint8_t security_level;
  uint32_t address;
  uint8_t packet_size_code;
  uint8_t baud_multiplier;
} AS608_SysPara;

typedef struct
{
  UART_HandleTypeDef *uart;
  uint32_t address;
  uint16_t capacity;
  uint32_t baudrate;
  uint8_t ready;
  uint8_t last_rx[AS608_LAST_RX_MAX];
  uint16_t last_rx_len;
} AS608_Handle;

void AS608_Init(AS608_Handle *handle, UART_HandleTypeDef *uart);
uint8_t AS608_AutoConnect(AS608_Handle *handle);
uint8_t AS608_TryConnectAtBaud(AS608_Handle *handle, uint32_t baudrate);
uint8_t AS608_GetImage(AS608_Handle *handle);
uint8_t AS608_GenChar(AS608_Handle *handle, uint8_t buffer_id);
uint8_t AS608_Match(AS608_Handle *handle);
uint8_t AS608_RegModel(AS608_Handle *handle);
uint8_t AS608_StoreChar(AS608_Handle *handle, uint8_t buffer_id, uint16_t page_id);
uint8_t AS608_DeleteChar(AS608_Handle *handle, uint16_t page_id, uint16_t count);
uint8_t AS608_Empty(AS608_Handle *handle);
uint8_t AS608_ReadSysPara(AS608_Handle *handle, AS608_SysPara *sys_para);
uint8_t AS608_ValidTemplateNum(AS608_Handle *handle, uint16_t *valid_count);
uint8_t AS608_HighSpeedSearch(AS608_Handle *handle, uint8_t buffer_id, uint16_t start_page, uint16_t page_num,
                              AS608_SearchResult *result);
const char *AS608_ConfirmMessage(uint8_t code);

#ifdef __cplusplus
}
#endif

#endif /* __AS608_H */
