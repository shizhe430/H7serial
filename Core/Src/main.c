/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body — OV2640 XCAM_VIEW Video Stream
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dcmi.h"
#include "dma.h"
#include "i2c.h"
#include "quadspi.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "camera_app.h"
#include "face_ai.h"
#include "oled_status.h"
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static uint32_t g_sdram_diag_phase;
static uint32_t g_sdram_diag_index;
static uint32_t g_sdram_diag_diff;
static uint32_t g_sdram_diag_reread0;
static uint32_t g_sdram_diag_reread1;
static uint32_t g_sdram_diag_reread2;
static uint32_t g_sdram_diag_snap0;
static uint32_t g_sdram_diag_snap1;
static uint32_t g_sdram_diag_snap2;
static uint32_t g_sdram_diag_snap3;
static uint32_t g_sdram_diag_hsnap0;
static uint32_t g_sdram_diag_hsnap1;
static uint32_t g_sdram_diag_hsnap2;
static uint32_t g_sdram_diag_hsnap3;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */
static void main_uart_print(const char *text);
static void main_print_build_info(void);
static void main_boot_stage(const char *tag);
static void main_boot_led(uint8_t on);
static void main_external_memory_diag(void);
static uint8_t main_qspi_read_status(uint8_t command_code, uint8_t *value);
static uint8_t main_qspi_read_sfdp(uint8_t sfdp[8]);
static uint8_t main_sdram_cache_roundtrip(uint32_t *fail_addr, uint32_t *expected, uint32_t *actual);
static uint8_t main_sdram_self_test_detail(uint32_t *fail_addr, uint32_t *expected, uint32_t *actual);
static void main_sdram_d6_pull_diag(void) __attribute__((unused));
static void main_sdram_sequence_diag(void) __attribute__((unused));
static void main_qspi_program_service(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void main_uart_print(const char *text)
{
#if (APP_MODE_STREAM_SILENT == 0U)
  if (text != NULL)
  {
    (void)HAL_UART_Transmit(&huart1, (uint8_t *)text, (uint16_t)strlen(text), HAL_MAX_DELAY);
  }
#else
  (void)text;
#endif
}

static void main_print_build_info(void)
{
#if (APP_MODE_STREAM_SILENT == 0U)
  char buf[96];
  const char *cfg;
  const char *opt;
  int len;

#ifdef DEBUG
  cfg = "Debug";
#else
  cfg = "Release";
#endif

#if defined(__OPTIMIZE_SIZE__)
  opt = "Os";
#elif defined(__OPTIMIZE__)
  opt = "Oopt";
#else
  opt = "O0";
#endif

  len = snprintf(buf, sizeof(buf), "[BUILD] %s %s %s %s\r\n", cfg, opt, __DATE__, __TIME__);
  if (len > 0)
  {
    (void)HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
  }
#endif
}

static void main_boot_stage(const char *tag)
{
#if (APP_MODE_STREAM_SILENT == 0U)
  char buf[64];
  int len;

  if (tag == NULL)
  {
    return;
  }

  len = snprintf(buf, sizeof(buf), "[BOOT] stage=%s\r\n", tag);
  if (len > 0)
  {
    (void)HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
  }
#else
  (void)tag;
#endif
}

static void main_boot_led(uint8_t on)
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, (on != 0U) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

static uint32_t main_crc32_update(uint32_t crc, const uint8_t *data, uint32_t size)
{
  uint32_t i;

  for (i = 0U; i < size; i++)
  {
    uint32_t bit;

    crc ^= data[i];
    for (bit = 0U; bit < 8U; bit++)
    {
      crc = ((crc & 1U) != 0U) ? ((crc >> 1U) ^ 0xEDB88320UL) : (crc >> 1U);
    }
  }
  return crc;
}

static uint32_t main_read_le32(const uint8_t *data)
{
  return ((uint32_t)data[0]) | ((uint32_t)data[1] << 8U) |
         ((uint32_t)data[2] << 16U) | ((uint32_t)data[3] << 24U);
}

static void main_qspi_program_service(void)
{
  static uint8_t chunk[1024];
  uint8_t header[16];
  uint8_t chunk_len_bytes[2];
  uint8_t ack;
  uint32_t address;
  uint32_t size;
  uint32_t expected_crc;
  uint32_t received = 0U;
  uint32_t crc = 0xFFFFFFFFUL;
  char msg[128];
  int len;

  main_uart_print("[QSPI_UPDATER] ready magic=QUP1 baud=921600\r\n");
  if (HAL_UART_Receive(&huart1, header, sizeof(header), 60000U) != HAL_OK)
  {
    main_uart_print("[QSPI_UPDATER] header timeout\r\n");
    return;
  }
  if ((main_read_le32(&header[0]) != 0x31505551UL) ||
      (main_read_le32(&header[4]) >= (32UL * 1024UL * 1024UL)) ||
      (main_read_le32(&header[8]) == 0U))
  {
    main_uart_print("[QSPI_UPDATER] bad header\r\n");
    return;
  }

  address = main_read_le32(&header[4]);
  size = main_read_le32(&header[8]);
  expected_crc = main_read_le32(&header[12]);
  if ((address + size < address) || ((address + size) > (32UL * 1024UL * 1024UL)) ||
      ((address & 0xFFFFU) != 0U) || ((size & 0xFFFFU) != 0U))
  {
    main_uart_print("[QSPI_UPDATER] range must be 64K aligned\r\n");
    return;
  }

  len = snprintf(msg, sizeof(msg), "[QSPI_UPDATER] erase addr=0x%08lX size=%lu\r\n",
                 (unsigned long)address, (unsigned long)size);
  if (len > 0)
  {
    main_uart_print(msg);
  }
  if (MX_QUADSPI_EraseRange(address, size) != 0U)
  {
    main_uart_print("[QSPI_UPDATER] erase fail\r\n");
    return;
  }
  main_uart_print("[QSPI_UPDATER] DATA\r\n");

  while (received < size)
  {
    uint32_t chunk_len;

    if (HAL_UART_Receive(&huart1, chunk_len_bytes, sizeof(chunk_len_bytes), 60000U) != HAL_OK)
    {
      main_uart_print("[QSPI_UPDATER] chunk header timeout\r\n");
      return;
    }
    chunk_len = (uint32_t)chunk_len_bytes[0] | ((uint32_t)chunk_len_bytes[1] << 8U);
    if ((chunk_len == 0U) || (chunk_len > sizeof(chunk)) || (received + chunk_len > size))
    {
      main_uart_print("[QSPI_UPDATER] bad chunk\r\n");
      return;
    }
    if (HAL_UART_Receive(&huart1, chunk, (uint16_t)chunk_len, 5000U) != HAL_OK)
    {
      main_uart_print("[QSPI_UPDATER] chunk timeout\r\n");
      return;
    }
    if (MX_QUADSPI_ProgramRange(address + received, chunk, chunk_len) != 0U)
    {
      main_uart_print("[QSPI_UPDATER] program fail\r\n");
      return;
    }
    crc = main_crc32_update(crc, chunk, chunk_len);
    received += chunk_len;
    ack = 0x06U;
    (void)HAL_UART_Transmit(&huart1, &ack, 1U, HAL_MAX_DELAY);
  }

  crc ^= 0xFFFFFFFFUL;
  len = snprintf(msg, sizeof(msg), "[QSPI_UPDATER] done crc=0x%08lX expected=0x%08lX status=%s\r\n",
                 (unsigned long)crc, (unsigned long)expected_crc,
                 (crc == expected_crc) ? "OK" : "BAD");
  if (len > 0)
  {
    main_uart_print(msg);
  }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* Enable the CPU Cache */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* X-CUBE-AI uses packed internal buffers that may require unaligned reads. */
  SCB->CCR &= ~SCB_CCR_UNALIGN_TRP_Msk;
  __DSB();
  __ISB();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  uint32_t sysinit_timeout = 1000000U;

  __HAL_RCC_CSI_ENABLE();
  while ((__HAL_RCC_GET_FLAG(RCC_FLAG_CSIRDY) == 0U) && (sysinit_timeout > 0U))
  {
    sysinit_timeout--;
  }

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  HAL_EnableCompensationCell();
  sysinit_timeout = 1000000U;
  while (((SYSCFG->CCCSR & SYSCFG_CCCSR_READY) == 0U) && (sysinit_timeout > 0U))
  {
    sysinit_timeout--;
  }

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_DCMI_Init();
  MX_I2C4_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_FMC_Init();
  MX_QUADSPI_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
#if (APP_MODE == APP_MODE_PUMP_CTRL)
  OLED_Status_Init();
#endif
  HAL_Delay(4000U);
  main_print_build_info();
  main_uart_print("\r\n[BOOT] main enter\r\n");
  SCB_CleanInvalidateDCache();
  SCB->CACR |= SCB_CACR_FORCEWT_Msk;
  __DSB();
  __ISB();
  if (MX_QUADSPI_Enable4ByteAddressMode() == 0U)
  {
    main_uart_print("[QSPI] 4byte-address=1\r\n");
  }
  else
  {
    main_uart_print("[QSPI] 4byte-address=0\r\n");
  }
  main_external_memory_diag();
#if (APP_MODE == APP_MODE_QSPI_PROGRAM)
  main_qspi_program_service();
  while (1)
  {
    main_boot_led(1U);
    HAL_Delay(200U);
    main_boot_led(0U);
    HAL_Delay(200U);
  }
#else
  if (MX_QUADSPI_EnableMemoryMapped() == 0U)
  {
    uint32_t yunet_probe = *(volatile uint32_t *)0x90000000UL;
    uint32_t sface_probe = *(volatile uint32_t *)0x90400000UL;
    char qspi_probe[96];
    int qspi_probe_len = snprintf(qspi_probe, sizeof(qspi_probe),
                                  "[QSPI] probe yunet=0x%08lX sface=0x%08lX\r\n",
                                  (unsigned long)yunet_probe,
                                  (unsigned long)sface_probe);
    main_uart_print("[QSPI] memory-mapped=1 base=0x90000000\r\n");
    if (qspi_probe_len > 0)
    {
      main_uart_print(qspi_probe);
    }
  }
  else
  {
    main_uart_print("[QSPI] memory-mapped=0\r\n");
  }
  SCB_CleanInvalidateDCache();
  SCB->CACR &= ~SCB_CACR_FORCEWT_Msk;
  __DSB();
  __ISB();
#if ((APP_MODE == APP_MODE_FACE_AI_DIAG) || (APP_MODE == APP_MODE_FACE_AI_VISUAL))
  if (FaceAI_LoadWeights() != 0U)
  {
    main_uart_print("[FACE_AI] weights load failed\r\n");
  }
  (void)FaceAI_LoadEnrollment();
#endif
  main_boot_stage("camera_init_enter");
  CameraApp_Init();
  main_boot_stage("camera_init_exit");
  main_uart_print("[BOOT] camera init return\r\n");
#endif

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    CameraApp_Run();

    /* LED 心跳 */
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
static uint8_t main_sdram_init_sequence(void)
{
  FMC_SDRAM_CommandTypeDef command = {0};
  uint32_t mode_reg;

  command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
  command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  command.AutoRefreshNumber = 1;
  command.ModeRegisterDefinition = 0;
  if (HAL_SDRAM_SendCommand(&hsdram1, &command, HAL_MAX_DELAY) != HAL_OK)
  {
    return 1U;
  }

  HAL_Delay(1U);

  command.CommandMode = FMC_SDRAM_CMD_PALL;
  command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  command.AutoRefreshNumber = 1;
  command.ModeRegisterDefinition = 0;
  if (HAL_SDRAM_SendCommand(&hsdram1, &command, HAL_MAX_DELAY) != HAL_OK)
  {
    return 2U;
  }

  command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  command.AutoRefreshNumber = 8;
  command.ModeRegisterDefinition = 0;
  if (HAL_SDRAM_SendCommand(&hsdram1, &command, HAL_MAX_DELAY) != HAL_OK)
  {
    return 3U;
  }

  mode_reg = 0x0000U;
  mode_reg |= 0x0000U;
  mode_reg |= 0x0020U;
  mode_reg |= 0x0000U;
  mode_reg |= 0x0000U;
  mode_reg |= 0x0200U;

  command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
  command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  command.AutoRefreshNumber = 1;
  command.ModeRegisterDefinition = mode_reg;
  if (HAL_SDRAM_SendCommand(&hsdram1, &command, HAL_MAX_DELAY) != HAL_OK)
  {
    return 4U;
  }

#if (SDRAM_50MHZ_DIAG != 0U)
  if (HAL_SDRAM_ProgramRefreshRate(&hsdram1, 370U) != HAL_OK)
#else
  if (HAL_SDRAM_ProgramRefreshRate(&hsdram1, 761U) != HAL_OK)
#endif
  {
    return 5U;
  }

  return 0U;
}

static uint8_t main_sdram_cache_roundtrip(uint32_t *fail_addr, uint32_t *expected, uint32_t *actual)
{
  volatile uint32_t *mem = (volatile uint32_t *)0xC0000000UL;
  const uint32_t bytes = 4096U;
  const uint32_t words = bytes / sizeof(uint32_t);
  uint32_t pass;
  uint32_t i;

  for (pass = 0U; pass < 2U; pass++)
  {
    for (i = 0U; i < words; i++)
    {
      uint32_t value = 0xA55A3CC3UL ^ (i * 0x10204081UL);

      mem[i] = (pass == 0U) ? value : ~value;
    }

    __DSB();
    SCB_CleanDCache_by_Addr((uint32_t *)mem, (int32_t)bytes);
    SCB_InvalidateDCache_by_Addr((void *)mem, (int32_t)bytes);

    for (i = 0U; i < words; i++)
    {
      uint32_t value = 0xA55A3CC3UL ^ (i * 0x10204081UL);
      uint32_t exp = (pass == 0U) ? value : ~value;
      uint32_t got = mem[i];

      if (got != exp)
      {
        g_sdram_diag_phase = 13U + pass;
        g_sdram_diag_index = i;
        g_sdram_diag_diff = exp ^ got;
        g_sdram_diag_reread0 = mem[i];
        g_sdram_diag_reread1 = mem[i];
        g_sdram_diag_reread2 = mem[i];
        if (fail_addr != NULL)
        {
          *fail_addr = 0xC0000000UL + (i * sizeof(uint32_t));
        }
        if (expected != NULL)
        {
          *expected = exp;
        }
        if (actual != NULL)
        {
          *actual = got;
        }
        return (uint8_t)(13U + pass);
      }
    }
  }

  return 0U;
}

static void main_sdram_d6_pull_diag(void)
{
  volatile uint16_t *hmem = (volatile uint16_t *)0xC0000000UL;
  const uint32_t pe9_pull_mask = 0x3UL << (9U * 2U);
  uint16_t zero_up;
  uint16_t zero_down;
  uint16_t zero_none;
  uint16_t one_up;
  uint16_t one_down;
  uint16_t one_none;
  char buf[128];
  int len;

  hmem[0] = 0x0000U;
  __DSB();

  MODIFY_REG(GPIOE->PUPDR, pe9_pull_mask, GPIO_PULLUP << (9U * 2U));
  __DSB();
  zero_up = hmem[0];
  MODIFY_REG(GPIOE->PUPDR, pe9_pull_mask, GPIO_PULLDOWN << (9U * 2U));
  __DSB();
  zero_down = hmem[0];
  MODIFY_REG(GPIOE->PUPDR, pe9_pull_mask, GPIO_NOPULL << (9U * 2U));
  __DSB();
  zero_none = hmem[0];

  hmem[0] = 0x0040U;
  __DSB();

  MODIFY_REG(GPIOE->PUPDR, pe9_pull_mask, GPIO_PULLUP << (9U * 2U));
  __DSB();
  one_up = hmem[0];
  MODIFY_REG(GPIOE->PUPDR, pe9_pull_mask, GPIO_PULLDOWN << (9U * 2U));
  __DSB();
  one_down = hmem[0];
  MODIFY_REG(GPIOE->PUPDR, pe9_pull_mask, GPIO_NOPULL << (9U * 2U));
  __DSB();
  one_none = hmem[0];

  MODIFY_REG(GPIOE->PUPDR, pe9_pull_mask, GPIO_NOPULL << (9U * 2U));
  __DSB();
  __ISB();

  len = snprintf(buf, sizeof(buf),
                 "[SDRAM] d6pull zero up=%04X down=%04X none=%04X one up=%04X down=%04X none=%04X run=none\r\n",
                 (unsigned int)zero_up,
                 (unsigned int)zero_down,
                 (unsigned int)zero_none,
                 (unsigned int)one_up,
                 (unsigned int)one_down,
                 (unsigned int)one_none);
  if (len > 0)
  {
    main_uart_print(buf);
  }
}

static void main_sdram_sequence_diag(void)
{
  volatile uint16_t *hmem = (volatile uint16_t *)0xC0000000UL;
  uint16_t ff[4];
  uint16_t fs[4];
  uint16_t sf[4];
  uint16_t ss[4];
  char buf[128];
  int len;

  hmem[0] = 0x1111U;
  hmem[1] = 0x2222U;
  hmem[2] = 0x4444U;
  hmem[3] = 0x8888U;
  __DSB();

  ff[0] = hmem[0];
  ff[1] = hmem[1];
  ff[2] = hmem[2];
  ff[3] = hmem[3];

  fs[0] = hmem[0];
  __DSB();
  fs[1] = hmem[1];
  __DSB();
  fs[2] = hmem[2];
  __DSB();
  fs[3] = hmem[3];
  __DSB();

  hmem[0] = 0x0F0FU;
  __DSB();
  hmem[1] = 0xF0F0U;
  __DSB();
  hmem[2] = 0x55AAU;
  __DSB();
  hmem[3] = 0xAA55U;
  __DSB();

  sf[0] = hmem[0];
  sf[1] = hmem[1];
  sf[2] = hmem[2];
  sf[3] = hmem[3];

  ss[0] = hmem[0];
  __DSB();
  ss[1] = hmem[1];
  __DSB();
  ss[2] = hmem[2];
  __DSB();
  ss[3] = hmem[3];
  __DSB();

  len = snprintf(buf, sizeof(buf),
                 "[SDRAM] seq ff=%04X,%04X,%04X,%04X fs=%04X,%04X,%04X,%04X\r\n",
                 (unsigned int)ff[0], (unsigned int)ff[1],
                 (unsigned int)ff[2], (unsigned int)ff[3],
                 (unsigned int)fs[0], (unsigned int)fs[1],
                 (unsigned int)fs[2], (unsigned int)fs[3]);
  if (len > 0)
  {
    main_uart_print(buf);
  }

  len = snprintf(buf, sizeof(buf),
                 "[SDRAM] seq sf=%04X,%04X,%04X,%04X ss=%04X,%04X,%04X,%04X\r\n",
                 (unsigned int)sf[0], (unsigned int)sf[1],
                 (unsigned int)sf[2], (unsigned int)sf[3],
                 (unsigned int)ss[0], (unsigned int)ss[1],
                 (unsigned int)ss[2], (unsigned int)ss[3]);
  if (len > 0)
  {
    main_uart_print(buf);
  }
}

static uint8_t main_sdram_self_test_detail(uint32_t *fail_addr, uint32_t *expected, uint32_t *actual)
{
  volatile uint32_t *mem = (volatile uint32_t *)0xC0000000UL;
  volatile uint16_t *hmem = (volatile uint16_t *)0xC0000000UL;
  const uint32_t words = 1024U;
  const uint16_t half_patterns[] =
  {
    0x0000U,
    0xFFFFU,
    0xA5A5U,
    0x5A5AU,
    0x0001U,
    0x0040U,
    0x4000U,
    0x8000U
  };
  const uint16_t pair_patterns[][2] =
  {
    {0x0005U, 0xA5A5U},
    {0xA5A5U, 0x0005U},
    {0x0055U, 0xAA55U},
    {0xAA55U, 0x0055U},
    {0x0040U, 0x0000U},
    {0x0000U, 0x0040U},
    {0xFFFFU, 0x0000U},
    {0x0000U, 0xFFFFU}
  };
  const uint32_t probe_offsets[] =
  {
    0x00000000UL,
    0x00001000UL,
    0x00100000UL,
    0x00800000UL,
    0x01000000UL,
    0x01FF0000UL
  };
  uint32_t i;

  g_sdram_diag_phase = 0U;
  g_sdram_diag_index = 0U;
  g_sdram_diag_diff = 0U;
  g_sdram_diag_reread0 = 0U;
  g_sdram_diag_reread1 = 0U;
  g_sdram_diag_reread2 = 0U;
  g_sdram_diag_snap0 = 0U;
  g_sdram_diag_snap1 = 0U;
  g_sdram_diag_snap2 = 0U;
  g_sdram_diag_snap3 = 0U;
  g_sdram_diag_hsnap0 = 0U;
  g_sdram_diag_hsnap1 = 0U;
  g_sdram_diag_hsnap2 = 0U;
  g_sdram_diag_hsnap3 = 0U;

  if (fail_addr != NULL)
  {
    *fail_addr = 0U;
  }
  if (expected != NULL)
  {
    *expected = 0U;
  }
  if (actual != NULL)
  {
    *actual = 0U;
  }

  hmem[0] = 0x0040U;
  hmem[1] = 0x0000U;
  hmem[2] = 0x0000U;
  hmem[3] = 0x0000U;
  __DSB();
  __ISB();

  g_sdram_diag_hsnap0 = hmem[0];
  g_sdram_diag_hsnap1 = hmem[1];
  g_sdram_diag_hsnap2 = hmem[2];
  g_sdram_diag_hsnap3 = hmem[3];
  if ((hmem[0] != 0x0040U) || (hmem[1] != 0x0000U) || (hmem[2] != 0x0000U) || (hmem[3] != 0x0000U))
  {
    uint32_t half_index = (hmem[0] != 0x0040U) ? 0U : ((hmem[1] != 0U) ? 1U : ((hmem[2] != 0U) ? 2U : 3U));
    volatile uint16_t *failp = &hmem[half_index];
    g_sdram_diag_phase = 12U;
    g_sdram_diag_index = half_index;
    g_sdram_diag_diff = (half_index == 0U) ? (0x0040U ^ (uint32_t)hmem[0]) : (uint32_t)hmem[half_index];
    g_sdram_diag_reread0 = failp[0];
    g_sdram_diag_reread1 = failp[0];
    g_sdram_diag_reread2 = failp[0];
    if (fail_addr != NULL)
    {
      *fail_addr = 0xC0000000UL + (half_index * sizeof(uint16_t));
    }
    if (expected != NULL)
    {
      *expected = (half_index == 0U) ? 0x0040U : 0U;
    }
    if (actual != NULL)
    {
      *actual = hmem[half_index];
    }
    return 12U;
  }

  {
    volatile uint8_t *bmem = (volatile uint8_t *)0xC0000000UL;
    volatile uint16_t *hmem_alias = (volatile uint16_t *)0xC0000000UL;

    hmem_alias[0] = 0U;
    hmem_alias[1] = 0U;
    __DSB();
    __ISB();

    bmem[0] = 0x40U;
    __DSB();
    __ISB();

    g_sdram_diag_snap0 = bmem[0];
    g_sdram_diag_snap1 = bmem[1];
    g_sdram_diag_snap2 = bmem[2];
    g_sdram_diag_snap3 = bmem[3];
    if (bmem[2] == 0x40U)
    {
      g_sdram_diag_phase = 11U;
      g_sdram_diag_index = 0U;
      g_sdram_diag_diff = 0x40U;
      g_sdram_diag_reread0 = bmem[2];
      g_sdram_diag_reread1 = bmem[2];
      g_sdram_diag_reread2 = bmem[2];
      if (fail_addr != NULL)
      {
        *fail_addr = 0xC0000002UL;
      }
      if (expected != NULL)
      {
        *expected = 0U;
      }
      if (actual != NULL)
      {
        *actual = bmem[2];
      }
      return 11U;
    }
  }

  for (i = 0U; i < (sizeof(half_patterns) / sizeof(half_patterns[0])); i++)
  {
    uint16_t value = half_patterns[i];
    uint16_t got;
    uint32_t j;

    for (j = 0U; j < 64U; j++)
    {
      hmem[j] = value;
      got = hmem[j];
      if (got != value)
      {
        g_sdram_diag_phase = 1U;
        g_sdram_diag_index = j;
        g_sdram_diag_diff = ((uint32_t)value ^ (uint32_t)got);
        if (fail_addr != NULL)
        {
          *fail_addr = 0xC0000000UL + (j * sizeof(uint16_t));
        }
        if (expected != NULL)
        {
          *expected = value;
        }
        if (actual != NULL)
        {
          *actual = got;
        }
        return 4U;
      }
    }
  }

  for (i = 0U; i < 64U; i++)
  {
    uint16_t marker = (uint16_t)(0x5A00U ^ (uint16_t)i);
    uint32_t j;

    for (j = 0U; j < 64U; j++)
    {
      hmem[j] = 0U;
    }

    __DSB();
    __ISB();

    hmem[i] = marker;

    __DSB();
    __ISB();

    for (j = 0U; j < 64U; j++)
    {
      uint16_t expect = (j == i) ? marker : 0U;
      uint16_t got = hmem[j];

      if (got != expect)
      {
        volatile uint16_t *failp = &hmem[j];
        g_sdram_diag_phase = 6U;
        g_sdram_diag_index = (i << 16U) | j;
        g_sdram_diag_diff = ((uint32_t)expect ^ (uint32_t)got);
        g_sdram_diag_reread0 = failp[0];
        g_sdram_diag_reread1 = failp[0];
        g_sdram_diag_reread2 = failp[0];
        if (fail_addr != NULL)
        {
          *fail_addr = 0xC0000000UL + (j * sizeof(uint16_t));
        }
        if (expected != NULL)
        {
          *expected = expect;
        }
        if (actual != NULL)
        {
          *actual = got;
        }
        return 6U;
      }
    }
  }

  for (i = 0U; i < (sizeof(pair_patterns) / sizeof(pair_patterns[0])); i++)
  {
    volatile uint8_t *bmem = (volatile uint8_t *)0xC0000000UL;
    uint8_t exp0 = (uint8_t)(pair_patterns[i][0] & 0x00FFU);
    uint8_t exp1 = (uint8_t)((pair_patterns[i][0] >> 8U) & 0x00FFU);
    uint8_t exp2 = (uint8_t)(pair_patterns[i][1] & 0x00FFU);
    uint8_t exp3 = (uint8_t)((pair_patterns[i][1] >> 8U) & 0x00FFU);
    uint8_t got0;
    uint8_t got1;
    uint8_t got2;
    uint8_t got3;

    bmem[0] = exp0;
    bmem[1] = exp1;
    bmem[2] = exp2;
    bmem[3] = exp3;
    __DSB();
    __ISB();

    got0 = bmem[0];
    got1 = bmem[1];
    got2 = bmem[2];
    got3 = bmem[3];
    if ((got0 != exp0) || (got1 != exp1) || (got2 != exp2) || (got3 != exp3))
    {
      uint32_t byte_index = (got0 != exp0) ? 0U : ((got1 != exp1) ? 1U : ((got2 != exp2) ? 2U : 3U));
      g_sdram_diag_phase = 10U;
      g_sdram_diag_index = (i << 16U) | byte_index;
      g_sdram_diag_diff = (byte_index == 0U) ? ((uint32_t)exp0 ^ (uint32_t)got0) :
                          ((byte_index == 1U) ? ((uint32_t)exp1 ^ (uint32_t)got1) :
                          ((byte_index == 2U) ? ((uint32_t)exp2 ^ (uint32_t)got2) :
                                                ((uint32_t)exp3 ^ (uint32_t)got3)));
      g_sdram_diag_reread0 = bmem[byte_index];
      g_sdram_diag_reread1 = bmem[byte_index];
      g_sdram_diag_reread2 = bmem[byte_index];
      if (fail_addr != NULL)
      {
        *fail_addr = 0xC0000000UL + byte_index;
      }
      if (expected != NULL)
      {
        *expected = (byte_index == 0U) ? exp0 : ((byte_index == 1U) ? exp1 : ((byte_index == 2U) ? exp2 : exp3));
      }
      if (actual != NULL)
      {
        *actual = (byte_index == 0U) ? got0 : ((byte_index == 1U) ? got1 : ((byte_index == 2U) ? got2 : got3));
      }
      return 10U;
    }
  }

#if 0
  for (i = 0U; i < (sizeof(pair_patterns) / sizeof(pair_patterns[0])); i++)
  {
    uint16_t hal_src[2];
    uint16_t hal_dst[2] = {0U, 0U};
    uint16_t exp0 = pair_patterns[i][0];
    uint16_t exp1 = pair_patterns[i][1];

    hmem[0] = 0U;
    hmem[1] = 0U;
    __DSB();
    __ISB();

    hal_src[0] = exp0;
    hal_src[1] = exp1;
    if ((HAL_SDRAM_Write_16b(&hsdram1, (uint32_t *)0xC0000000UL, hal_src, 2U) != HAL_OK) ||
        (HAL_SDRAM_Read_16b(&hsdram1, (uint32_t *)0xC0000000UL, hal_dst, 2U) != HAL_OK) ||
        (hal_dst[0] != exp0) ||
        (hal_dst[1] != exp1))
    {
      uint32_t half_index = (hal_dst[0] != exp0) ? 0U : 1U;
      volatile uint16_t *failp = &hmem[half_index];
      g_sdram_diag_phase = 9U;
      g_sdram_diag_index = i;
      g_sdram_diag_diff = ((hal_dst[0] != exp0) ? ((uint32_t)exp0 ^ (uint32_t)hal_dst[0]) : ((uint32_t)exp1 ^ (uint32_t)hal_dst[1]));
      g_sdram_diag_reread0 = failp[0];
      g_sdram_diag_reread1 = failp[0];
      g_sdram_diag_reread2 = failp[0];
      if (fail_addr != NULL)
      {
        *fail_addr = 0xC0000000UL + (half_index * sizeof(uint16_t));
      }
      if (expected != NULL)
      {
        *expected = (half_index == 0U) ? exp0 : exp1;
      }
      if (actual != NULL)
      {
        *actual = (half_index == 0U) ? hal_dst[0] : hal_dst[1];
      }
      return 9U;
    }
  }
#endif

  for (i = 0U; i < (sizeof(pair_patterns) / sizeof(pair_patterns[0])); i++)
  {
    uint32_t j;

    for (j = 0U; j < 64U; j++)
    {
      uint32_t half_index;
      volatile uint16_t *failp;
      uint16_t exp0 = (uint16_t)(pair_patterns[i][0] ^ (uint16_t)j);
      uint16_t exp1 = (uint16_t)(pair_patterns[i][1] ^ (uint16_t)j);
      uint16_t got0;
      uint16_t got1;
      volatile uint32_t delay;

      hmem[(j * 2U) + 0U] = exp0;
      __DSB();
      for (delay = 0U; delay < 200U; delay++)
      {
      }
      hmem[(j * 2U) + 1U] = exp1;
      __DSB();
      __ISB();

      got0 = hmem[(j * 2U) + 0U];
      got1 = hmem[(j * 2U) + 1U];
      if ((got0 != exp0) || (got1 != exp1))
      {
        half_index = (j * 2U) + ((got0 != exp0) ? 0U : 1U);
        failp = &hmem[half_index];
        g_sdram_diag_phase = 8U;
        g_sdram_diag_index = (i << 16U) | j;
        g_sdram_diag_diff = ((got0 != exp0) ? ((uint32_t)exp0 ^ (uint32_t)got0) : ((uint32_t)exp1 ^ (uint32_t)got1));
        g_sdram_diag_reread0 = failp[0];
        g_sdram_diag_reread1 = failp[0];
        g_sdram_diag_reread2 = failp[0];
        if (fail_addr != NULL)
        {
          *fail_addr = 0xC0000000UL + (half_index * sizeof(uint16_t));
        }
        if (expected != NULL)
        {
          *expected = (got0 != exp0) ? exp0 : exp1;
        }
        if (actual != NULL)
        {
          *actual = (got0 != exp0) ? got0 : got1;
        }
        return 8U;
      }
    }
  }

  for (i = 0U; i < (sizeof(pair_patterns) / sizeof(pair_patterns[0])); i++)
  {
    uint32_t j;

    for (j = 0U; j < 64U; j++)
    {
      uint32_t half_index;
      volatile uint16_t *failp;
      uint16_t exp0 = (uint16_t)(pair_patterns[i][0] ^ (uint16_t)j);
      uint16_t exp1 = (uint16_t)(pair_patterns[i][1] ^ (uint16_t)j);
      uint16_t got0;
      uint16_t got1;

      hmem[(j * 2U) + 0U] = exp0;
      hmem[(j * 2U) + 1U] = exp1;
      __DSB();
      __ISB();

      got0 = hmem[(j * 2U) + 0U];
      got1 = hmem[(j * 2U) + 1U];
      if ((got0 != exp0) || (got1 != exp1))
      {
        half_index = (j * 2U) + ((got0 != exp0) ? 0U : 1U);
        failp = &hmem[half_index];
        g_sdram_diag_phase = 7U;
        g_sdram_diag_index = (i << 16U) | j;
        g_sdram_diag_diff = ((got0 != exp0) ? ((uint32_t)exp0 ^ (uint32_t)got0) : ((uint32_t)exp1 ^ (uint32_t)got1));
        g_sdram_diag_reread0 = failp[0];
        g_sdram_diag_reread1 = failp[0];
        g_sdram_diag_reread2 = failp[0];
        if (fail_addr != NULL)
        {
          *fail_addr = 0xC0000000UL + (half_index * sizeof(uint16_t));
        }
        if (expected != NULL)
        {
          *expected = (got0 != exp0) ? exp0 : exp1;
        }
        if (actual != NULL)
        {
          *actual = (got0 != exp0) ? got0 : got1;
        }
        return 5U;
      }
    }
  }

  for (i = 0U; i < (sizeof(pair_patterns) / sizeof(pair_patterns[0])); i++)
  {
    uint32_t j;

    for (j = 0U; j < 64U; j++)
    {
      hmem[(j * 2U) + 0U] = (uint16_t)(pair_patterns[i][0] ^ (uint16_t)j);
      hmem[(j * 2U) + 1U] = (uint16_t)(pair_patterns[i][1] ^ (uint16_t)j);
    }

    __DSB();
    __ISB();

    for (j = 0U; j < 64U; j++)
    {
      uint16_t exp0 = (uint16_t)(pair_patterns[i][0] ^ (uint16_t)j);
      uint16_t exp1 = (uint16_t)(pair_patterns[i][1] ^ (uint16_t)j);
      uint16_t got0 = hmem[(j * 2U) + 0U];
      uint16_t got1 = hmem[(j * 2U) + 1U];

      if ((got0 != exp0) || (got1 != exp1))
      {
        uint32_t half_index = (j * 2U) + ((got0 != exp0) ? 0U : 1U);
        volatile uint16_t *failp = &hmem[half_index];
        g_sdram_diag_phase = 5U;
        g_sdram_diag_index = (i << 16U) | j;
        g_sdram_diag_diff = ((got0 != exp0) ? ((uint32_t)exp0 ^ (uint32_t)got0) : ((uint32_t)exp1 ^ (uint32_t)got1));
        g_sdram_diag_reread0 = failp[0];
        g_sdram_diag_reread1 = failp[0];
        g_sdram_diag_reread2 = failp[0];
        if (fail_addr != NULL)
        {
          *fail_addr = 0xC0000000UL + (half_index * sizeof(uint16_t));
        }
        if (expected != NULL)
        {
          *expected = (got0 != exp0) ? exp0 : exp1;
        }
        if (actual != NULL)
        {
          *actual = (got0 != exp0) ? got0 : got1;
        }
        return 5U;
      }
    }
  }

  for (i = 0U; i < words; i++)
  {
    mem[i] = 0xA5A50000UL ^ i;
  }

  __DSB();
  __ISB();

  for (i = 0U; i < words; i++)
  {
    if (mem[i] != (0xA5A50000UL ^ i))
    {
      uint32_t expect = 0xA5A50000UL ^ i;
      uint32_t got = mem[i];
      g_sdram_diag_phase = 2U;
      g_sdram_diag_index = i;
      g_sdram_diag_diff = expect ^ got;
      if (fail_addr != NULL)
      {
        *fail_addr = 0xC0000000UL + (i * sizeof(uint32_t));
      }
      if (expected != NULL)
      {
        *expected = expect;
      }
      if (actual != NULL)
      {
        *actual = got;
      }
      return 1U;
    }
  }

  for (i = 0U; i < words; i++)
  {
    mem[i] = 0x5A5A0000UL ^ (i << 1U);
  }

  __DSB();
  __ISB();

  for (i = 0U; i < words; i++)
  {
    if (mem[i] != (0x5A5A0000UL ^ (i << 1U)))
    {
      uint32_t expect = 0x5A5A0000UL ^ (i << 1U);
      uint32_t got = mem[i];
      g_sdram_diag_phase = 3U;
      g_sdram_diag_index = i;
      g_sdram_diag_diff = expect ^ got;
      if (fail_addr != NULL)
      {
        *fail_addr = 0xC0000000UL + (i * sizeof(uint32_t));
      }
      if (expected != NULL)
      {
        *expected = expect;
      }
      if (actual != NULL)
      {
        *actual = got;
      }
      return 2U;
    }
  }

  for (i = 0U; i < (sizeof(probe_offsets) / sizeof(probe_offsets[0])); i++)
  {
    volatile uint32_t *addr = (volatile uint32_t *)(0xC0000000UL + probe_offsets[i]);
    *addr = 0x13572468UL ^ probe_offsets[i];
  }

  __DSB();
  __ISB();

  for (i = 0U; i < (sizeof(probe_offsets) / sizeof(probe_offsets[0])); i++)
  {
    volatile uint32_t *addr = (volatile uint32_t *)(0xC0000000UL + probe_offsets[i]);
    if (*addr != (0x13572468UL ^ probe_offsets[i]))
    {
      uint32_t expect = 0x13572468UL ^ probe_offsets[i];
      uint32_t got = *addr;
      g_sdram_diag_phase = 4U;
      g_sdram_diag_index = i;
      g_sdram_diag_diff = expect ^ got;
      if (fail_addr != NULL)
      {
        *fail_addr = 0xC0000000UL + probe_offsets[i];
      }
      if (expected != NULL)
      {
        *expected = expect;
      }
      if (actual != NULL)
      {
        *actual = got;
      }
      return 3U;
    }
  }

  return 0U;
}

static uint8_t main_qspi_read_id(uint8_t id[3])
{
  QSPI_CommandTypeDef command = {0};

  command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
  command.Instruction = 0x9FU;
  command.AddressMode = QSPI_ADDRESS_NONE;
  command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  command.DataMode = QSPI_DATA_1_LINE;
  command.DummyCycles = 0U;
  command.NbData = 3U;
  command.DdrMode = QSPI_DDR_MODE_DISABLE;
  command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
  command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

  if (HAL_QSPI_Command(&hqspi, &command, HAL_MAX_DELAY) != HAL_OK)
  {
    return 1U;
  }

  if (HAL_QSPI_Receive(&hqspi, id, HAL_MAX_DELAY) != HAL_OK)
  {
    return 2U;
  }

  return 0U;
}

static uint8_t main_qspi_read_status(uint8_t command_code, uint8_t *value)
{
  QSPI_CommandTypeDef command = {0};

  if (value == NULL)
  {
    return 3U;
  }

  command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
  command.Instruction = command_code;
  command.AddressMode = QSPI_ADDRESS_NONE;
  command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  command.DataMode = QSPI_DATA_1_LINE;
  command.DummyCycles = 0U;
  command.NbData = 1U;
  command.DdrMode = QSPI_DDR_MODE_DISABLE;
  command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
  command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

  if (HAL_QSPI_Command(&hqspi, &command, HAL_MAX_DELAY) != HAL_OK)
  {
    return 1U;
  }

  if (HAL_QSPI_Receive(&hqspi, value, HAL_MAX_DELAY) != HAL_OK)
  {
    return 2U;
  }

  return 0U;
}

static uint8_t main_qspi_read_sfdp(uint8_t sfdp[8])
{
  QSPI_CommandTypeDef command = {0};

  command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
  command.Instruction = 0x5AU;
  command.AddressMode = QSPI_ADDRESS_1_LINE;
  command.AddressSize = QSPI_ADDRESS_24_BITS;
  command.Address = 0U;
  command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  command.DataMode = QSPI_DATA_1_LINE;
  command.DummyCycles = 8U;
  command.NbData = 8U;
  command.DdrMode = QSPI_DDR_MODE_DISABLE;
  command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
  command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

  if (HAL_QSPI_Command(&hqspi, &command, HAL_MAX_DELAY) != HAL_OK)
  {
    return 1U;
  }

  if (HAL_QSPI_Receive(&hqspi, sfdp, HAL_MAX_DELAY) != HAL_OK)
  {
    return 2U;
  }

  return 0U;
}

static void main_external_memory_diag(void)
{
  char buf[128];
  PLL2_ClocksTypeDef pll2_clocks = {0};
  uint8_t qspi_id[3] = {0};
  uint8_t sfdp[8] = {0};
  uint8_t sr1 = 0U;
  uint8_t sr2 = 0U;
  uint8_t sr3 = 0U;
  uint8_t status;
  uint8_t sr_status;
  uint8_t selected_read_pipe = 0xFFU;
  uint32_t fmc_source;
  uint32_t fmc_kernel_hz = 0U;
  uint32_t mpu_saved_rnr;
  uint32_t sdram_rasr;
  uint32_t fail_addr = 0U;
  uint32_t expected = 0U;
  uint32_t actual = 0U;
  int len;

  mpu_saved_rnr = MPU->RNR;
  MPU->RNR = MPU_REGION_NUMBER1;
  sdram_rasr = MPU->RASR;
  MPU->RNR = mpu_saved_rnr;

  len = snprintf(buf, sizeof(buf), "[SDRAM] mpu_r1_en=%u rasr=0x%08lX normal_cacheable=1\r\n",
                 ((sdram_rasr & MPU_RASR_ENABLE_Msk) != 0U) ? 1U : 0U,
                 (unsigned long)sdram_rasr);
  if (len > 0)
  {
    main_uart_print(buf);
  }

  len = snprintf(buf, sizeof(buf), "[SDRAM] csi=%u iocomp en=%u ready=%u forcewt=%u cccsr=0x%08lX\r\n",
                 (__HAL_RCC_GET_FLAG(RCC_FLAG_CSIRDY) != 0U) ? 1U : 0U,
                 ((SYSCFG->CCCSR & SYSCFG_CCCSR_EN) != 0U) ? 1U : 0U,
                 ((SYSCFG->CCCSR & SYSCFG_CCCSR_READY) != 0U) ? 1U : 0U,
                 ((SCB->CACR & SCB_CACR_FORCEWT_Msk) != 0U) ? 1U : 0U,
                 (unsigned long)SYSCFG->CCCSR);
  if (len > 0)
  {
    main_uart_print(buf);
  }

  len = snprintf(buf, sizeof(buf), "[SDRAM] pe9 mode=%lu af=%lu speed=%lu pull=%lu idr=%lu\r\n",
                 (unsigned long)((GPIOE->MODER >> (9U * 2U)) & 0x3U),
                 (unsigned long)((GPIOE->AFR[1] >> ((9U - 8U) * 4U)) & 0xFU),
                 (unsigned long)((GPIOE->OSPEEDR >> (9U * 2U)) & 0x3U),
                 (unsigned long)((GPIOE->PUPDR >> (9U * 2U)) & 0x3U),
                 (unsigned long)((GPIOE->IDR >> 9U) & 0x1U));
  if (len > 0)
  {
    main_uart_print(buf);
  }

  if ((RCC->CR & RCC_CR_PLL2RDY) != 0U)
  {
    HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
  }

  fmc_source = __HAL_RCC_GET_FMC_SOURCE();
  if (fmc_source == RCC_FMCCLKSOURCE_D1HCLK)
  {
    fmc_kernel_hz = HAL_RCC_GetHCLKFreq();
  }
  else if (fmc_source == RCC_FMCCLKSOURCE_PLL2)
  {
    fmc_kernel_hz = pll2_clocks.PLL2_R_Frequency;
  }

  len = snprintf(buf, sizeof(buf), "[SDRAM] clocks sys=%lu hclk=%lu official=%u low50=%u\r\n",
                 (unsigned long)HAL_RCC_GetSysClockFreq(),
                 (unsigned long)HAL_RCC_GetHCLKFreq(),
                 (unsigned int)SDRAM_OFFICIAL_CLOCK_DIAG,
                 (unsigned int)SDRAM_50MHZ_DIAG);
  if (len > 0)
  {
    main_uart_print(buf);
  }

  len = snprintf(buf, sizeof(buf), "[SDRAM] fmc_src=0x%lX pll2_ready=%u kernel=%lu sdclk=%lu sdcr0=0x%08lX\r\n",
                 (unsigned long)fmc_source,
                 ((RCC->CR & RCC_CR_PLL2RDY) != 0U) ? 1U : 0U,
                 (unsigned long)fmc_kernel_hz,
                 (unsigned long)(fmc_kernel_hz / 2U),
                 (unsigned long)FMC_Bank5_6_R->SDCR[0]);
  if (len > 0)
  {
    main_uart_print(buf);
  }

  status = main_sdram_init_sequence();
  if (status == 0U)
  {
    uint32_t read_pipe;

    for (read_pipe = 0U; read_pipe < 3U; read_pipe++)
    {
      MODIFY_REG(FMC_Bank5_6_R->SDCR[0], FMC_SDCRx_RPIPE_Msk,
                 read_pipe << FMC_SDCRx_RPIPE_Pos);
      __DSB();
      __ISB();

      fail_addr = 0U;
      expected = 0U;
      actual = 0U;
      status = main_sdram_cache_roundtrip(&fail_addr, &expected, &actual);
      len = snprintf(buf, sizeof(buf),
                     "[SDRAM] rpipe=%lu sdcr0=0x%08lX status=%u addr=0x%08lX exp=%08lX got=%08lX diff=%08lX\r\n",
                     (unsigned long)read_pipe,
                     (unsigned long)FMC_Bank5_6_R->SDCR[0],
                     (unsigned int)status,
                     (unsigned long)fail_addr,
                     (unsigned long)expected,
                     (unsigned long)actual,
                     (unsigned long)(expected ^ actual));
      if (len > 0)
      {
        main_uart_print(buf);
      }

      if ((status == 0U) && (selected_read_pipe == 0xFFU))
      {
        selected_read_pipe = (uint8_t)read_pipe;
      }
    }

    if (selected_read_pipe == 0xFFU)
    {
      selected_read_pipe = 0U;
    }
    MODIFY_REG(FMC_Bank5_6_R->SDCR[0], FMC_SDCRx_RPIPE_Msk,
               ((uint32_t)selected_read_pipe) << FMC_SDCRx_RPIPE_Pos);
    __DSB();
    __ISB();

    fail_addr = 0U;
    expected = 0U;
    actual = 0U;
    status = main_sdram_cache_roundtrip(&fail_addr, &expected, &actual);
    len = snprintf(buf, sizeof(buf), "[SDRAM] cache_rt %s code=%u bytes=4096 passes=%u\r\n",
                   (status == 0U) ? "ok" : "fail",
                   (unsigned int)status,
                   ((status == 0U) || (status == 14U)) ? 2U : 1U);
    if (len > 0)
    {
      main_uart_print(buf);
    }

    if (status == 0U)
    {
      status = main_sdram_self_test_detail(&fail_addr, &expected, &actual);
    }
    len = snprintf(buf, sizeof(buf), "[SDRAM] test %s code=%u base=0xC0000000 size=32MB\r\n",
                   (status == 0U) ? "ok" : "fail",
                   (unsigned int)status);
  }
  else
  {
    len = snprintf(buf, sizeof(buf), "[SDRAM] init fail code=%u\r\n", (unsigned int)status);
  }

  if (len > 0)
  {
    main_uart_print(buf);
  }

  if ((status != 0U) && (fail_addr != 0U))
  {
    len = snprintf(buf, sizeof(buf), "[SDRAM] fail addr=0x%08lX exp=0x%08lX got=0x%08lX\r\n",
                   (unsigned long)fail_addr,
                   (unsigned long)expected,
                   (unsigned long)actual);
    if (len > 0)
    {
      main_uart_print(buf);
    }

    len = snprintf(buf, sizeof(buf), "[SDRAM] diag phase=%lu index=%lu diff=0x%08lX half=%s\r\n",
                   (unsigned long)g_sdram_diag_phase,
                   (unsigned long)g_sdram_diag_index,
                   (unsigned long)g_sdram_diag_diff,
                   ((g_sdram_diag_diff & 0xFFFF0000UL) != 0U) ? "high" : "low");
    if (len > 0)
    {
      main_uart_print(buf);
    }

    len = snprintf(buf, sizeof(buf), "[SDRAM] reread=%04lX,%04lX,%04lX\r\n",
                   (unsigned long)g_sdram_diag_reread0,
                   (unsigned long)g_sdram_diag_reread1,
                   (unsigned long)g_sdram_diag_reread2);
    if (len > 0)
    {
      main_uart_print(buf);
    }

    len = snprintf(buf, sizeof(buf), "[SDRAM] hsnap=%04lX,%04lX,%04lX,%04lX\r\n",
                   (unsigned long)g_sdram_diag_hsnap0,
                   (unsigned long)g_sdram_diag_hsnap1,
                   (unsigned long)g_sdram_diag_hsnap2,
                   (unsigned long)g_sdram_diag_hsnap3);
    if (len > 0)
    {
      main_uart_print(buf);
    }

    len = snprintf(buf, sizeof(buf), "[SDRAM] snap=%02lX,%02lX,%02lX,%02lX\r\n",
                   (unsigned long)g_sdram_diag_snap0,
                   (unsigned long)g_sdram_diag_snap1,
                   (unsigned long)g_sdram_diag_snap2,
                   (unsigned long)g_sdram_diag_snap3);
    if (len > 0)
    {
      main_uart_print(buf);
    }
  }

  status = main_qspi_read_id(qspi_id);
  len = snprintf(buf, sizeof(buf), "[QSPI] id=%02X%02X%02X status=%u\r\n",
                 (unsigned int)qspi_id[0],
                 (unsigned int)qspi_id[1],
                 (unsigned int)qspi_id[2],
                 (unsigned int)status);
  if (len > 0)
  {
    main_uart_print(buf);
  }

  sr_status = main_qspi_read_status(0x05U, &sr1);
  sr_status |= main_qspi_read_status(0x35U, &sr2);
  sr_status |= main_qspi_read_status(0x15U, &sr3);
  len = snprintf(buf, sizeof(buf), "[QSPI] sr1=%02X sr2=%02X sr3=%02X status=%u\r\n",
                 (unsigned int)sr1,
                 (unsigned int)sr2,
                 (unsigned int)sr3,
                 (unsigned int)sr_status);
  if (len > 0)
  {
    main_uart_print(buf);
  }

  status = main_qspi_read_sfdp(sfdp);
  len = snprintf(buf, sizeof(buf), "[QSPI] sfdp=%02X%02X%02X%02X rev=%02X%02X status=%u\r\n",
                 (unsigned int)sfdp[0],
                 (unsigned int)sfdp[1],
                 (unsigned int)sfdp[2],
                 (unsigned int)sfdp[3],
                 (unsigned int)sfdp[4],
                 (unsigned int)sfdp[5],
                 (unsigned int)status);
  if (len > 0)
  {
    main_uart_print(buf);
  }
}

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x30000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_64KB;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RW;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* External SDRAM must be Normal memory for X-CUBE-AI packed accesses. */
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress = 0xC0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_HFNMI_PRIVDEF);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __disable_irq();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  while (1)
  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
    for (volatile uint32_t i = 0U; i < 6000000U; i++)
    {
    }
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
