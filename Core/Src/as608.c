#include "as608.h"

#define AS608_PACKET_IDENTIFIER 0x01U
#define AS608_ACK_IDENTIFIER    0x07U
#define AS608_RESPONSE_MAX_LEN  AS608_LAST_RX_MAX

static HAL_StatusTypeDef as608_set_baudrate(AS608_Handle *handle, uint32_t baudrate);
static void as608_uart_flush(AS608_Handle *handle);
static HAL_StatusTypeDef as608_send_packet(AS608_Handle *handle, const uint8_t *payload, uint16_t payload_len);
static HAL_StatusTypeDef as608_receive_packet(AS608_Handle *handle, uint8_t *response, uint16_t response_size,
                                              uint16_t *response_len, uint32_t timeout_ms);
static HAL_StatusTypeDef as608_send_command(AS608_Handle *handle, const uint8_t *payload, uint16_t payload_len,
                                            uint8_t *response, uint16_t response_size, uint16_t *response_len,
                                            uint32_t timeout_ms);
static uint8_t as608_get_confirm_code(const uint8_t *response, uint16_t response_len);
static uint8_t as608_handshake(AS608_Handle *handle, uint32_t *address);

void AS608_Init(AS608_Handle *handle, UART_HandleTypeDef *uart)
{
  if (handle == NULL)
  {
    return;
  }

  handle->uart = uart;
  handle->address = AS608_DEFAULT_ADDRESS;
  handle->capacity = AS608_DEFAULT_CAPACITY;
  handle->baudrate = 0U;
  handle->ready = 0U;
  handle->last_rx_len = 0U;
}

uint8_t AS608_AutoConnect(AS608_Handle *handle)
{
  static const uint32_t baudrates[] = {57600U, 115200U, 38400U, 19200U, 9600U};
  uint32_t i;

  if (handle == NULL)
  {
    return 0xFFU;
  }

  for (i = 0U; i < (sizeof(baudrates) / sizeof(baudrates[0])); i++)
  {
    if (AS608_TryConnectAtBaud(handle, baudrates[i]) == 0x00U)
    {
      return 0x00U;
    }
  }

  return 0xFFU;
}

uint8_t AS608_TryConnectAtBaud(AS608_Handle *handle, uint32_t baudrate)
{
  uint32_t address = AS608_DEFAULT_ADDRESS;
  uint8_t confirm;

  if (handle == NULL)
  {
    return 0xFFU;
  }

  handle->address = AS608_DEFAULT_ADDRESS;
  handle->ready = 0U;
  handle->last_rx_len = 0U;

  if (as608_set_baudrate(handle, baudrate) != HAL_OK)
  {
    return 0xFFU;
  }

  confirm = as608_handshake(handle, &address);
  if (confirm == 0x00U)
  {
    handle->address = address;
    handle->ready = 1U;
  }

  return confirm;
}

uint8_t AS608_GetImage(AS608_Handle *handle)
{
  uint8_t payload[] = {0x01U};
  uint8_t response[AS608_RESPONSE_MAX_LEN];
  uint16_t response_len = 0U;

  if (as608_send_command(handle, payload, sizeof(payload), response, sizeof(response), &response_len, 1000U) != HAL_OK)
  {
    return 0xFFU;
  }

  return as608_get_confirm_code(response, response_len);
}

uint8_t AS608_GenChar(AS608_Handle *handle, uint8_t buffer_id)
{
  uint8_t payload[] = {0x02U, buffer_id};
  uint8_t response[AS608_RESPONSE_MAX_LEN];
  uint16_t response_len = 0U;

  if (as608_send_command(handle, payload, sizeof(payload), response, sizeof(response), &response_len, 1000U) != HAL_OK)
  {
    return 0xFFU;
  }

  return as608_get_confirm_code(response, response_len);
}

uint8_t AS608_Match(AS608_Handle *handle)
{
  uint8_t payload[] = {0x03U};
  uint8_t response[AS608_RESPONSE_MAX_LEN];
  uint16_t response_len = 0U;

  if (as608_send_command(handle, payload, sizeof(payload), response, sizeof(response), &response_len, 1000U) != HAL_OK)
  {
    return 0xFFU;
  }

  return as608_get_confirm_code(response, response_len);
}

uint8_t AS608_RegModel(AS608_Handle *handle)
{
  uint8_t payload[] = {0x05U};
  uint8_t response[AS608_RESPONSE_MAX_LEN];
  uint16_t response_len = 0U;

  if (as608_send_command(handle, payload, sizeof(payload), response, sizeof(response), &response_len, 1000U) != HAL_OK)
  {
    return 0xFFU;
  }

  return as608_get_confirm_code(response, response_len);
}

uint8_t AS608_StoreChar(AS608_Handle *handle, uint8_t buffer_id, uint16_t page_id)
{
  uint8_t payload[] = {0x06U, buffer_id, (uint8_t)(page_id >> 8), (uint8_t)page_id};
  uint8_t response[AS608_RESPONSE_MAX_LEN];
  uint16_t response_len = 0U;

  if (as608_send_command(handle, payload, sizeof(payload), response, sizeof(response), &response_len, 2000U) != HAL_OK)
  {
    return 0xFFU;
  }

  return as608_get_confirm_code(response, response_len);
}

uint8_t AS608_DeleteChar(AS608_Handle *handle, uint16_t page_id, uint16_t count)
{
  uint8_t payload[] = {0x0CU, (uint8_t)(page_id >> 8), (uint8_t)page_id, (uint8_t)(count >> 8), (uint8_t)count};
  uint8_t response[AS608_RESPONSE_MAX_LEN];
  uint16_t response_len = 0U;

  if (as608_send_command(handle, payload, sizeof(payload), response, sizeof(response), &response_len, 2000U) != HAL_OK)
  {
    return 0xFFU;
  }

  return as608_get_confirm_code(response, response_len);
}

uint8_t AS608_Empty(AS608_Handle *handle)
{
  uint8_t payload[] = {0x0DU};
  uint8_t response[AS608_RESPONSE_MAX_LEN];
  uint16_t response_len = 0U;

  if (as608_send_command(handle, payload, sizeof(payload), response, sizeof(response), &response_len, 3000U) != HAL_OK)
  {
    return 0xFFU;
  }

  return as608_get_confirm_code(response, response_len);
}

uint8_t AS608_ReadSysPara(AS608_Handle *handle, AS608_SysPara *sys_para)
{
  uint8_t payload[] = {0x0FU};
  uint8_t response[AS608_RESPONSE_MAX_LEN];
  uint16_t response_len = 0U;
  uint8_t confirm;

  if ((handle == NULL) || (sys_para == NULL))
  {
    return 0xFFU;
  }

  if (as608_send_command(handle, payload, sizeof(payload), response, sizeof(response), &response_len, 1000U) != HAL_OK)
  {
    return 0xFFU;
  }

  confirm = as608_get_confirm_code(response, response_len);
  if ((confirm == 0x00U) && (response_len >= 28U))
  {
    sys_para->capacity = ((uint16_t)response[14] << 8) | response[15];
    sys_para->security_level = response[17];
    sys_para->address = ((uint32_t)response[18] << 24) | ((uint32_t)response[19] << 16) |
                        ((uint32_t)response[20] << 8) | response[21];
    sys_para->packet_size_code = response[23];
    sys_para->baud_multiplier = response[25];

    handle->capacity = sys_para->capacity;
    handle->address = sys_para->address;
  }

  return confirm;
}

uint8_t AS608_ValidTemplateNum(AS608_Handle *handle, uint16_t *valid_count)
{
  uint8_t payload[] = {0x1DU};
  uint8_t response[AS608_RESPONSE_MAX_LEN];
  uint16_t response_len = 0U;
  uint8_t confirm;

  if ((handle == NULL) || (valid_count == NULL))
  {
    return 0xFFU;
  }

  if (as608_send_command(handle, payload, sizeof(payload), response, sizeof(response), &response_len, 1000U) != HAL_OK)
  {
    return 0xFFU;
  }

  confirm = as608_get_confirm_code(response, response_len);
  if ((confirm == 0x00U) && (response_len >= 14U))
  {
    *valid_count = ((uint16_t)response[10] << 8) | response[11];
  }

  return confirm;
}

uint8_t AS608_HighSpeedSearch(AS608_Handle *handle, uint8_t buffer_id, uint16_t start_page, uint16_t page_num,
                              AS608_SearchResult *result)
{
  uint8_t payload[] = {0x1BU, buffer_id, (uint8_t)(start_page >> 8), (uint8_t)start_page,
                       (uint8_t)(page_num >> 8), (uint8_t)page_num};
  uint8_t response[AS608_RESPONSE_MAX_LEN];
  uint16_t response_len = 0U;
  uint8_t confirm;

  if ((handle == NULL) || (result == NULL))
  {
    return 0xFFU;
  }

  if (as608_send_command(handle, payload, sizeof(payload), response, sizeof(response), &response_len, 1000U) != HAL_OK)
  {
    return 0xFFU;
  }

  confirm = as608_get_confirm_code(response, response_len);
  if ((confirm == 0x00U) && (response_len >= 16U))
  {
    result->page_id = ((uint16_t)response[10] << 8) | response[11];
    result->match_score = ((uint16_t)response[12] << 8) | response[13];
  }

  return confirm;
}

const char *AS608_ConfirmMessage(uint8_t code)
{
  switch (code)
  {
    case 0x00U:
      return "OK";
    case 0x01U:
      return "packet receive error";
    case 0x02U:
      return "no finger on sensor";
    case 0x03U:
      return "failed to capture image";
    case 0x04U:
      return "fingerprint image too dry or too light";
    case 0x05U:
      return "fingerprint image too wet or too blurry";
    case 0x06U:
      return "fingerprint image too messy";
    case 0x07U:
      return "too few minutiae";
    case 0x08U:
      return "fingerprints do not match";
    case 0x09U:
      return "fingerprint not found";
    case 0x0AU:
      return "template merge failed";
    case 0x0BU:
      return "page ID out of range";
    case 0x10U:
      return "template delete failed";
    case 0x11U:
      return "database clear failed";
    case 0x15U:
      return "no valid image in buffer";
    case 0x18U:
      return "flash read/write error";
    case 0x19U:
      return "undefined error";
    case 0x1AU:
      return "invalid register number";
    case 0x1BU:
      return "incorrect register configuration";
    case 0x1CU:
      return "notepad page error";
    case 0x1FU:
      return "fingerprint database full";
    case 0x20U:
      return "module address error";
    case 0xFFU:
      return "timeout or invalid response";
    default:
      return "unknown confirmation code";
  }
}

static HAL_StatusTypeDef as608_set_baudrate(AS608_Handle *handle, uint32_t baudrate)
{
  if ((handle == NULL) || (handle->uart == NULL))
  {
    return HAL_ERROR;
  }

  if (handle->uart->Init.BaudRate == baudrate)
  {
    handle->baudrate = baudrate;
    return HAL_OK;
  }

  handle->uart->Init.BaudRate = baudrate;
  if (HAL_UART_DeInit(handle->uart) != HAL_OK)
  {
    return HAL_ERROR;
  }

  if (HAL_UART_Init(handle->uart) != HAL_OK)
  {
    return HAL_ERROR;
  }

  if (HAL_UARTEx_SetTxFifoThreshold(handle->uart, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    return HAL_ERROR;
  }

  if (HAL_UARTEx_SetRxFifoThreshold(handle->uart, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    return HAL_ERROR;
  }

  if (HAL_UARTEx_DisableFifoMode(handle->uart) != HAL_OK)
  {
    return HAL_ERROR;
  }

  handle->baudrate = baudrate;
  return HAL_OK;
}

static void as608_uart_flush(AS608_Handle *handle)
{
  uint8_t byte;

  if ((handle == NULL) || (handle->uart == NULL))
  {
    return;
  }

  while (HAL_UART_Receive(handle->uart, &byte, 1U, 2U) == HAL_OK)
  {
  }
}

static HAL_StatusTypeDef as608_send_packet(AS608_Handle *handle, const uint8_t *payload, uint16_t payload_len)
{
  uint8_t packet[32];
  uint16_t packet_len = 0U;
  uint16_t length_field;
  uint16_t checksum;
  uint16_t i;

  if ((handle == NULL) || (handle->uart == NULL) || (payload == NULL) || (payload_len > 21U))
  {
    return HAL_ERROR;
  }

  packet[packet_len++] = 0xEFU;
  packet[packet_len++] = 0x01U;
  packet[packet_len++] = (uint8_t)(handle->address >> 24);
  packet[packet_len++] = (uint8_t)(handle->address >> 16);
  packet[packet_len++] = (uint8_t)(handle->address >> 8);
  packet[packet_len++] = (uint8_t)handle->address;
  packet[packet_len++] = AS608_PACKET_IDENTIFIER;

  length_field = payload_len + 2U;
  packet[packet_len++] = (uint8_t)(length_field >> 8);
  packet[packet_len++] = (uint8_t)length_field;

  checksum = AS608_PACKET_IDENTIFIER + packet[7] + packet[8];
  for (i = 0U; i < payload_len; i++)
  {
    packet[packet_len++] = payload[i];
    checksum = (uint16_t)(checksum + payload[i]);
  }

  packet[packet_len++] = (uint8_t)(checksum >> 8);
  packet[packet_len++] = (uint8_t)checksum;

  return HAL_UART_Transmit(handle->uart, packet, packet_len, HAL_MAX_DELAY);
}

static HAL_StatusTypeDef as608_receive_packet(AS608_Handle *handle, uint8_t *response, uint16_t response_size,
                                              uint16_t *response_len, uint32_t timeout_ms)
{
  uint8_t byte;
  uint16_t received_len = 0U;
  uint16_t remaining_len;
  uint16_t total_len;
  uint32_t start_tick;

  if ((handle == NULL) || (handle->uart == NULL) || (response == NULL) || (response_len == NULL) || (response_size < 12U))
  {
    return HAL_ERROR;
  }

  handle->last_rx_len = 0U;
  start_tick = HAL_GetTick();

  while ((HAL_GetTick() - start_tick) < timeout_ms)
  {
    if (HAL_UART_Receive(handle->uart, &byte, 1U, 5U) != HAL_OK)
    {
      continue;
    }

    if (received_len < response_size)
    {
      response[received_len] = byte;
    }
    if (received_len < AS608_LAST_RX_MAX)
    {
      handle->last_rx[received_len] = byte;
      handle->last_rx_len++;
    }
    received_len++;

    if (received_len >= 9U)
    {
      break;
    }
  }

  *response_len = received_len;
  if (received_len < 9U)
  {
    return HAL_TIMEOUT;
  }

  if ((response[0] != 0xEFU) || (response[1] != 0x01U))
  {
    return HAL_ERROR;
  }

  remaining_len = ((uint16_t)response[7] << 8) | response[8];
  total_len = (uint16_t)(9U + remaining_len);
  if (total_len > response_size)
  {
    return HAL_ERROR;
  }

  while ((HAL_GetTick() - start_tick) < timeout_ms)
  {
    if (received_len >= total_len)
    {
      *response_len = received_len;
      return HAL_OK;
    }

    if (HAL_UART_Receive(handle->uart, &byte, 1U, 5U) != HAL_OK)
    {
      continue;
    }

    if (received_len < response_size)
    {
      response[received_len] = byte;
    }
    if (received_len < AS608_LAST_RX_MAX)
    {
      handle->last_rx[received_len] = byte;
      handle->last_rx_len++;
    }
    received_len++;
  }

  *response_len = received_len;
  return HAL_TIMEOUT;
}

static HAL_StatusTypeDef as608_send_command(AS608_Handle *handle, const uint8_t *payload, uint16_t payload_len,
                                            uint8_t *response, uint16_t response_size, uint16_t *response_len,
                                            uint32_t timeout_ms)
{
  as608_uart_flush(handle);

  if (as608_send_packet(handle, payload, payload_len) != HAL_OK)
  {
    return HAL_ERROR;
  }

  return as608_receive_packet(handle, response, response_size, response_len, timeout_ms);
}

static uint8_t as608_get_confirm_code(const uint8_t *response, uint16_t response_len)
{
  if ((response == NULL) || (response_len < 12U) || (response[6] != AS608_ACK_IDENTIFIER))
  {
    return 0xFFU;
  }

  return response[9];
}

static uint8_t as608_handshake(AS608_Handle *handle, uint32_t *address)
{
  uint8_t request[9];
  uint8_t response[AS608_RESPONSE_MAX_LEN];
  uint16_t response_len = 0U;
  uint8_t confirm;

  if ((handle == NULL) || (handle->uart == NULL) || (address == NULL))
  {
    return 0xFFU;
  }

  request[0] = 0xEFU;
  request[1] = 0x01U;
  request[2] = (uint8_t)(handle->address >> 24);
  request[3] = (uint8_t)(handle->address >> 16);
  request[4] = (uint8_t)(handle->address >> 8);
  request[5] = (uint8_t)handle->address;
  request[6] = AS608_PACKET_IDENTIFIER;
  request[7] = 0x00U;
  request[8] = 0x00U;

  as608_uart_flush(handle);
  if (HAL_UART_Transmit(handle->uart, request, sizeof(request), HAL_MAX_DELAY) != HAL_OK)
  {
    return 0xFFU;
  }

  if (as608_receive_packet(handle, response, sizeof(response), &response_len, 300U) != HAL_OK)
  {
    return 0xFFU;
  }

  if ((response_len < 12U) || (response[6] != AS608_ACK_IDENTIFIER))
  {
    return 0xFFU;
  }

  *address = ((uint32_t)response[2] << 24) | ((uint32_t)response[3] << 16) |
             ((uint32_t)response[4] << 8) | response[5];
  confirm = response[9];

  if (confirm == 0x01U)
  {
    return 0x00U;
  }

  return confirm;
}
