/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    quadspi.c
  * @brief   This file provides code for the configuration
  *          of the QUADSPI instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "quadspi.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

QSPI_HandleTypeDef hqspi;

/* QUADSPI init function */
void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */

  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */

  /* USER CODE END QUADSPI_Init 1 */
  hqspi.Instance = QUADSPI;
  /* Keep QSPI at about 100 MHz while PLL2_R is raised for SDRAM. */
  hqspi.Init.ClockPrescaler = 1;
  hqspi.Init.FifoThreshold = 4;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
  hqspi.Init.FlashSize = 24;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_5_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  hqspi.Init.FlashID = QSPI_FLASH_ID_1;
  hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */

  /* USER CODE END QUADSPI_Init 2 */

}

void HAL_QSPI_MspInit(QSPI_HandleTypeDef* qspiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(qspiHandle->Instance==QUADSPI)
  {
  /* USER CODE BEGIN QUADSPI_MspInit 0 */

  /* USER CODE END QUADSPI_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_QSPI;
    PeriphClkInitStruct.PLL2.PLL2M = 5;
    /* Match FMC's shared PLL2 configuration. */
    PeriphClkInitStruct.PLL2.PLL2N = 80;
    PeriphClkInitStruct.PLL2.PLL2P = 2;
    PeriphClkInitStruct.PLL2.PLL2Q = 2;
#if (SDRAM_50MHZ_DIAG != 0U)
    PeriphClkInitStruct.PLL2.PLL2R = 4;
#else
    PeriphClkInitStruct.PLL2.PLL2R = 2;
#endif
    PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_2;
    PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
    PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
    PeriphClkInitStruct.QspiClockSelection = RCC_QSPICLKSOURCE_PLL2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* QUADSPI clock enable */
    __HAL_RCC_QSPI_CLK_ENABLE();

    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**QUADSPI GPIO Configuration
    PF6     ------> QUADSPI_BK1_IO3
    PF7     ------> QUADSPI_BK1_IO2
    PF8     ------> QUADSPI_BK1_IO0
    PF9     ------> QUADSPI_BK1_IO1
    PB2     ------> QUADSPI_CLK
    PB6     ------> QUADSPI_BK1_NCS
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN QUADSPI_MspInit 1 */

  /* USER CODE END QUADSPI_MspInit 1 */
  }
}

void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef* qspiHandle)
{

  if(qspiHandle->Instance==QUADSPI)
  {
  /* USER CODE BEGIN QUADSPI_MspDeInit 0 */

  /* USER CODE END QUADSPI_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_QSPI_CLK_DISABLE();

    /**QUADSPI GPIO Configuration
    PF6     ------> QUADSPI_BK1_IO3
    PF7     ------> QUADSPI_BK1_IO2
    PF8     ------> QUADSPI_BK1_IO0
    PF9     ------> QUADSPI_BK1_IO1
    PB2     ------> QUADSPI_CLK
    PB6     ------> QUADSPI_BK1_NCS
    */
    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_2|GPIO_PIN_6);

  /* USER CODE BEGIN QUADSPI_MspDeInit 1 */

  /* USER CODE END QUADSPI_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
static uint8_t qspi_send_command(uint8_t instruction,
                                 uint32_t address,
                                 uint8_t has_address,
                                 const uint8_t *tx_data,
                                 uint8_t *rx_data,
                                 uint32_t data_size)
{
  QSPI_CommandTypeDef command = {0};

  command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
  command.Instruction = instruction;
  command.AddressMode = (has_address != 0U) ? QSPI_ADDRESS_1_LINE : QSPI_ADDRESS_NONE;
  command.AddressSize = QSPI_ADDRESS_32_BITS;
  command.Address = address;
  command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  command.DataMode = (data_size != 0U) ? QSPI_DATA_1_LINE : QSPI_DATA_NONE;
  command.DummyCycles = 0U;
  command.NbData = data_size;
  command.DdrMode = QSPI_DDR_MODE_DISABLE;
  command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
  command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

  if (HAL_QSPI_Command(&hqspi, &command, HAL_MAX_DELAY) != HAL_OK)
  {
    return 1U;
  }
  if ((tx_data != NULL) && (data_size != 0U))
  {
    return (HAL_QSPI_Transmit(&hqspi, (uint8_t *)tx_data, HAL_MAX_DELAY) == HAL_OK) ? 0U : 2U;
  }
  if ((rx_data != NULL) && (data_size != 0U))
  {
    return (HAL_QSPI_Receive(&hqspi, rx_data, HAL_MAX_DELAY) == HAL_OK) ? 0U : 3U;
  }
  return 0U;
}

static uint8_t qspi_read_status(uint8_t *status)
{
  return qspi_send_command(0x05U, 0U, 0U, NULL, status, 1U);
}

static uint8_t qspi_wait_ready(uint32_t timeout_ms)
{
  uint32_t start = HAL_GetTick();
  uint8_t status = 0U;

  do
  {
    if (qspi_read_status(&status) != 0U)
    {
      return 1U;
    }
    if ((status & 0x01U) == 0U)
    {
      return 0U;
    }
  } while ((HAL_GetTick() - start) < timeout_ms);

  return 2U;
}

static uint8_t qspi_write_enable(void)
{
  uint8_t status = 0U;

  if (qspi_send_command(0x06U, 0U, 0U, NULL, NULL, 0U) != 0U)
  {
    return 1U;
  }
  if (qspi_read_status(&status) != 0U)
  {
    return 2U;
  }
  return ((status & 0x02U) != 0U) ? 0U : 3U;
}

uint8_t MX_QUADSPI_Enable4ByteAddressMode(void)
{
  /* W25Q256 needs four-byte addresses for the upper half of its 32 MB. */
  return qspi_send_command(0xB7U, 0U, 0U, NULL, NULL, 0U);
}

static uint8_t qspi_erase_block64k(uint32_t address)
{
  if (qspi_write_enable() != 0U)
  {
    return 1U;
  }
  if (qspi_send_command(0xDCU, address, 1U, NULL, NULL, 0U) != 0U)
  {
    return 2U;
  }
  return qspi_wait_ready(120000U);
}

uint8_t MX_QUADSPI_EraseRange(uint32_t address, uint32_t size)
{
  uint32_t end;

  if ((size == 0U) || ((address & 0xFFFFU) != 0U))
  {
    return 1U;
  }
  end = address + size;
  if ((end < address) || ((end & 0xFFFFU) != 0U) || (end > (32UL * 1024UL * 1024UL)))
  {
    return 2U;
  }

  while (address < end)
  {
    if (qspi_erase_block64k(address) != 0U)
    {
      return 3U;
    }
    address += 0x10000U;
  }
  return 0U;
}

static uint8_t qspi_program_page(uint32_t address, const uint8_t *data, uint32_t size)
{
  if ((data == NULL) || (size == 0U) || (size > 256U))
  {
    return 1U;
  }
  if (qspi_write_enable() != 0U)
  {
    return 2U;
  }
  if (qspi_send_command(0x12U, address, 1U, data, NULL, size) != 0U)
  {
    return 3U;
  }
  return qspi_wait_ready(5000U);
}

uint8_t MX_QUADSPI_ProgramRange(uint32_t address, const uint8_t *data, uint32_t size)
{
  uint32_t remaining = size;

  if ((data == NULL) || (size == 0U) || (address + size < address) ||
      ((address + size) > (32UL * 1024UL * 1024UL)))
  {
    return 1U;
  }

  while (remaining != 0U)
  {
    uint32_t page_room = 256U - (address & 0xFFU);
    uint32_t page_size = (remaining < page_room) ? remaining : page_room;

    if (qspi_program_page(address, data, page_size) != 0U)
    {
      return 2U;
    }
    address += page_size;
    data += page_size;
    remaining -= page_size;
  }
  return 0U;
}

uint8_t MX_QUADSPI_EnableMemoryMapped(void)
{
  QSPI_CommandTypeDef command = {0};
  QSPI_MemoryMappedTypeDef mapped_cfg = {0};

  if (MX_QUADSPI_Enable4ByteAddressMode() != 0U)
  {
    return 1U;
  }

  command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
  /* W25Q256 4-byte-address quad-output fast read. SR2.QE is set on the board. */
  command.Instruction = 0x6CU;
  command.AddressMode = QSPI_ADDRESS_1_LINE;
  command.AddressSize = QSPI_ADDRESS_32_BITS;
  command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  command.DataMode = QSPI_DATA_4_LINES;
  command.DummyCycles = 8U;
  command.NbData = 0U;
  command.DdrMode = QSPI_DDR_MODE_DISABLE;
  command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
  command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

  mapped_cfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
  mapped_cfg.TimeOutPeriod = 0U;

  return (HAL_QSPI_MemoryMapped(&hqspi, &command, &mapped_cfg) == HAL_OK) ? 0U : 1U;
}

uint8_t MX_QUADSPI_DisableMemoryMapped(void)
{
  return (HAL_QSPI_Abort(&hqspi) == HAL_OK) ? 0U : 1U;
}

/* USER CODE END 1 */
