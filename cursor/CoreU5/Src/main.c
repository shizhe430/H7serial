/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body - OV2640 JPEG Continuous Capture
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "dcmi.h"
#include "gpdma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "atk_mc2640.h"
#include "atk_mc2640_dcmi.h"
#include "ai_app.h"
#include "network.h"
#include "preprocess.h"
#include "jpeg_decode.h"
#include "test_input_empty.h"
#include "delay.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define APP_MODE_XCAM_VIEW      0U
#define APP_MODE_AI_INFER       1U
#define APP_MODE_AI_TEST_INPUT  2U
#define APP_MODE_SNAPSHOT       3U
#define APP_MODE            APP_MODE_XCAM_VIEW
#define CAMERA_WIDTH        320U
#define CAMERA_HEIGHT       240U
#define RGB565_BUF_PIXELS   (CAMERA_WIDTH * CAMERA_HEIGHT)
#define RGB565_BUF_WORDS    ((RGB565_BUF_PIXELS * sizeof(uint16_t)) / sizeof(uint32_t))
#define JPEG_BUF_SIZE       (50U * 1024U)
#define JPEG_BUF_WORDS      (JPEG_BUF_SIZE / 4U)
#define INFER_PERIOD_MS       100U
#define FILTER_WINDOW         3U
#define REPORT_MIN_CONF_PCT   0U
#define NO_CUP_FAST_MARGIN    25
#define ROI_DARK_TH           20U
#define ROI_BRIGHT_TH         235U
#define ROI_DARK_RATIO_TH     70U
#define ROI_BRIGHT_RATIO_TH   70U
#define NO_CUP_MARGIN_TH      18
#define NO_CUP_MIN_CONF_PCT   65U
#define NO_CUP_GUARD_ENABLE   0U
#define EMPTY_BIAS_ENABLE     1U
#define EMPTY_BIAS_CONF_MAX   45U
#define EMPTY_BIAS_DELTA_TH   20
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint8_t g_frame_ok = 0;
uint16_t g_rgb565_buf[RGB565_BUF_PIXELS] __attribute__((aligned(4)));
uint8_t g_jpeg_buf[JPEG_BUF_SIZE] __attribute__((aligned(4)));
static int8_t g_ai_input[PREPROCESS_DST_SIZE] __attribute__((aligned(4)));
static ai_class_t g_cls_hist[FILTER_WINDOW];
static uint32_t g_cls_hist_idx = 0U;
static uint8_t g_cls_hist_filled = 0U;
extern DCMI_HandleTypeDef hdcmi;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void print_text(const char *text);

static void print_result(const ai_result_t *result);
static ai_class_t stable_class_update(ai_class_t cls);
static uint32_t softmax_confidence_percent(const ai_result_t *result);
static uint32_t stable_confidence_percent(const ai_result_t *result);
static ai_class_t apply_no_cup_guard(const ai_result_t *result, ai_class_t cls, uint32_t conf_pct);
static ai_class_t apply_empty_bias(const ai_result_t *result, ai_class_t cls, uint32_t conf_pct);
static ai_class_t apply_state_machine(ai_class_t cls);
static uint8_t roi_is_abnormal(const int8_t *input);
static uint32_t find_jpeg_end(const uint8_t *buf, uint32_t max_len);
#if (APP_MODE == APP_MODE_XCAM_VIEW)
static void xcam_send_frame_once(void);
#endif
static void capture_and_infer_once(void);
static void run_test_input_once(void);
static void snapshot_send_once(void);
static void print_input_stats(const int8_t *input);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 * @brief DCMI 帧完成中断回调。
 * @note  当一帧图像搬运完成时由 HAL 调用：
 *        1) 置位 g_frame_ok（通用帧完成标志）
 *        2) 通知 ATK DCMI 驱动本帧已完成
 */
void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi_ptr)
{
    (void)hdcmi_ptr;
    g_frame_ok = 1;
    atk_mc2640_dcmi_set_frame_done(1U);
}

/**
 * @brief DCMI 错误中断回调。
 * @note  发生采集/传输错误时由 HAL 调用：
 *        1) 置位错误标志
 *        2) 同时置位帧完成，避免上层一直等待
 */
void HAL_DCMI_ErrorCallback(DCMI_HandleTypeDef *hdcmi_ptr)
{
    (void)hdcmi_ptr;
    g_frame_ok = 1;
    /* 错误中断只标记错误，不伪造“帧完成” */
    atk_mc2640_dcmi_set_error(1U);
}

/**
 * @brief 串口输出一段文本（USART1）。
 * @param text 要发送的以 '\0' 结尾字符串。
 */
static void print_text(const char *text)
{
    if (text != NULL)
    {
        (void)HAL_UART_Transmit(&huart1, (uint8_t *)text, (uint16_t)strlen(text), HAL_MAX_DELAY);
    }
}


/**
 * @brief 打印一次 AI 推理结果。
 * @param result 推理输出结构体（类别、置信度、原始分数）。
 * @note  输出格式示例：water=half class=3 conf=87%
 */
static void print_result(const ai_result_t *result)
{
    char msg[192];
    uint32_t conf_pct;
    ai_class_t cls;

    if (result == NULL)
    {
        return;
    }

    conf_pct = softmax_confidence_percent(result);
    cls = apply_state_machine(result->class_id);

    (void)snprintf(msg,
                   sizeof(msg),
                   "water=%s class=%lu conf=%lu%% raw=[%d,%d,%d,%d,%d]\r\n",
                   ai_app_class_name(cls),
                   (uint32_t)cls,
                   conf_pct,
                   (int)result->raw_scores[0],
                   (int)result->raw_scores[1],
                   (int)result->raw_scores[2],
                   (int)result->raw_scores[3],
                   (int)result->raw_scores[4]);

    print_text(msg);
}

/**
 * @brief 在 JPEG 缓冲区内查找 EOI(0xFFD9) 结束标记。
 * @param buf JPEG 数据缓冲区。
 * @param max_len 最大扫描长度。
 * @retval 找到则返回 JPEG 实际长度；否则返回 0。
 */
static ai_class_t stable_class_update(ai_class_t cls)
{
    uint32_t i;
    uint32_t n;
    uint32_t counts[AI_CLASS_COUNT] = {0U};
    uint32_t best_count = 0U;
    ai_class_t best_cls = cls;

    g_cls_hist[g_cls_hist_idx] = cls;
    g_cls_hist_idx = (g_cls_hist_idx + 1U) % FILTER_WINDOW;
    if (g_cls_hist_filled < FILTER_WINDOW)
    {
        g_cls_hist_filled++;
    }

    n = g_cls_hist_filled;
    for (i = 0U; i < n; i++)
    {
        ai_class_t c = g_cls_hist[i];
        if ((uint32_t)c < AI_CLASS_COUNT)
        {
            counts[(uint32_t)c]++;
        }
    }

    for (i = 0U; i < AI_CLASS_COUNT; i++)
    {
        if (counts[i] > best_count)
        {
            best_count = counts[i];
            best_cls = (ai_class_t)i;
        }
    }

    return best_cls;
}

static uint32_t softmax_confidence_percent(const ai_result_t *result)
{
    float logits[AI_CLASS_COUNT];
    float max_logit;
    float sum_exp = 0.0f;
    float best_prob = 0.0f;
    uint32_t i;

    if (result == NULL)
    {
        return 0U;
    }

    for (i = 0U; i < AI_CLASS_COUNT; i++)
    {
        logits[i] = ((float)((int32_t)result->raw_scores[i] - STAI_NETWORK_OUT_1_ZERO_POINT)) * STAI_NETWORK_OUT_1_SCALE;
    }

    max_logit = logits[0];
    for (i = 1U; i < AI_CLASS_COUNT; i++)
    {
        if (logits[i] > max_logit)
        {
            max_logit = logits[i];
        }
    }

    for (i = 0U; i < AI_CLASS_COUNT; i++)
    {
        float e = expf(logits[i] - max_logit);
        sum_exp += e;
        if (e > best_prob)
        {
            best_prob = e;
        }
    }

    if (sum_exp <= 0.0f)
    {
        return 0U;
    }

    best_prob /= sum_exp;
    if (best_prob >= 1.0f)
    {
        return 100U;
    }
    if (best_prob <= 0.0f)
    {
        return 0U;
    }

    return (uint32_t)(best_prob * 100.0f + 0.5f);
}

static uint32_t stable_confidence_percent(const ai_result_t *result)
{
    (void)result;
    return 0U;
}

static ai_class_t apply_no_cup_guard(const ai_result_t *result, ai_class_t cls, uint32_t conf_pct)
{
#if (NO_CUP_GUARD_ENABLE == 0U)
    (void)result;
    (void)conf_pct;
    return cls;
#else
    int16_t s0;
    int16_t s1;
    int16_t margin01;

    if ((result == NULL) || (cls != AI_CLASS_NO_CUP))
    {
        return cls;
    }

    s0 = (int16_t)result->raw_scores[AI_CLASS_NO_CUP];
    s1 = (int16_t)result->raw_scores[AI_CLASS_EMPTY];
    margin01 = (int16_t)(s0 - s1);

    if ((margin01 < NO_CUP_MARGIN_TH) || (conf_pct < NO_CUP_MIN_CONF_PCT))
    {
        return AI_CLASS_EMPTY;
    }

    return cls;
#endif
}

static ai_class_t apply_empty_bias(const ai_result_t *result, ai_class_t cls, uint32_t conf_pct)
{
    if ((EMPTY_BIAS_ENABLE == 0U) || (result == NULL))
    {
        return cls;
    }

    if (conf_pct > EMPTY_BIAS_CONF_MAX)
    {
        return cls;
    }

    if (((int16_t)result->raw_scores[AI_CLASS_EMPTY] - (int16_t)result->raw_scores[AI_CLASS_NO_CUP]) >= EMPTY_BIAS_DELTA_TH)
    {
        return AI_CLASS_EMPTY;
    }

    return cls;
}

static ai_class_t apply_state_machine(ai_class_t cls)
{
    static ai_class_t last_cls = AI_CLASS_EMPTY;

    if ((cls >= AI_CLASS_EMPTY) && (cls <= AI_CLASS_FULL) &&
        (last_cls >= AI_CLASS_EMPTY) && (last_cls <= AI_CLASS_FULL))
    {
        int32_t diff = (int32_t)cls - (int32_t)last_cls;
        if (diff > 1)
        {
            cls = (ai_class_t)((uint32_t)last_cls + 1U);
        }
        else if (diff < -1)
        {
            cls = (ai_class_t)((uint32_t)last_cls - 1U);
        }
    }

    last_cls = cls;
    return cls;
}

static uint8_t roi_is_abnormal(const int8_t *input)
{
    uint32_t i;
    uint32_t dark_cnt = 0U;
    uint32_t bright_cnt = 0U;
    uint32_t total = PREPROCESS_DST_SIZE;

    if (input == NULL)
    {
        return 1U;
    }

    for (i = 0U; i < total; i++)
    {
        int16_t u8v = (int16_t)input[i] + 128;
        if (u8v < ROI_DARK_TH)
        {
            dark_cnt++;
        }
        if (u8v > ROI_BRIGHT_TH)
        {
            bright_cnt++;
        }
    }

    if (((dark_cnt * 100U) / total) > ROI_DARK_RATIO_TH)
    {
        return 1U;
    }

    if (((bright_cnt * 100U) / total) > ROI_BRIGHT_RATIO_TH)
    {
        return 1U;
    }

    return 0U;
}

static uint32_t find_jpeg_end(const uint8_t *buf, uint32_t max_len)
{
    uint32_t i;

    if ((buf == NULL) || (max_len < 2U))
    {
        return 0U;
    }

    for (i = 0U; i < (max_len - 1U); i++)
    {
        if ((buf[i] == 0xFFU) && (buf[i + 1U] == 0xD9U))
        {
            return i + 2U;
        }
    }

    return 0U;
}

/**
 * @brief 获取一帧 JPEG 并原样发送到串口（供 XCAM 显示）。
 */
static void xcam_send_frame_once(void)
{
    uint32_t jpeg_len;

    if (atk_mc2640_get_frame((uint32_t)g_jpeg_buf, ATK_MC2640_GET_TYPE_DTS_32B_INC, NULL) != ATK_MC2640_EOK)
    {
        print_text("camera capture failed\r\n");
        return;
    }

    jpeg_len = find_jpeg_end(g_jpeg_buf, JPEG_BUF_SIZE);
    if (jpeg_len == 0U)
    {
        print_text("jpeg end not found\r\n");
        return;
    }

    (void)HAL_UART_Transmit(&huart1, g_jpeg_buf, (uint16_t)jpeg_len, HAL_MAX_DELAY);
}

/**
 * @brief 执行一次“采图 -> 预处理 -> 推理 -> 打印”。
 * @details
 *  1) 从 OV2640 获取一帧 RGB565（320x240）到 g_rgb565_buf
 *  2) resize + 灰度/量化到模型输入 g_ai_input
 *  3) 调用 ai_app_run 执行 CNN 推理
 *  4) 串口打印分类结果
 */
static void capture_and_infer_once(void)
{
    uint32_t jpeg_len;
    ai_result_t result;

    print_text("step1 capture(jpeg)...\r\n");
    if (atk_mc2640_get_frame((uint32_t)g_jpeg_buf, ATK_MC2640_GET_TYPE_DTS_32B_INC, NULL) != ATK_MC2640_EOK)
    {
        print_text("capture fail\r\n");
        return;
    }

    jpeg_len = find_jpeg_end(g_jpeg_buf, JPEG_BUF_SIZE);
    if (jpeg_len == 0U)
    {
        print_text("jpeg end not found\r\n");
        return;
    }

    print_text("step2 decode...\r\n");
    if (jpeg_to_ai_input(g_jpeg_buf, jpeg_len, g_ai_input) != 0U)
    {
        print_text("jpeg decode fail\r\n");
        return;
    }

    print_text("step3 infer...\r\n");
    if (ai_app_run(g_ai_input, &result) != 0U)
    {
        print_text("infer fail\r\n");
        return;
    }

    print_text("step4 done\r\n");
    print_result(&result);
}

static void run_test_input_once(void)
{
    ai_result_t result;

    memcpy(g_ai_input, g_test_input_empty, sizeof(g_ai_input));

    print_text("test_input infer...\r\n");
    if (ai_app_run(g_ai_input, &result) != 0U)
    {
        print_text("infer fail\r\n");
        return;
    }

    print_result(&result);
}

static void snapshot_send_once(void)
{
    uint32_t jpeg_len;

    if (atk_mc2640_get_frame((uint32_t)g_jpeg_buf, ATK_MC2640_GET_TYPE_DTS_32B_INC, NULL) != ATK_MC2640_EOK)
    {
        return;
    }

    jpeg_len = find_jpeg_end(g_jpeg_buf, JPEG_BUF_SIZE);
    if (jpeg_len == 0U)
    {
        return;
    }

    (void)HAL_UART_Transmit(&huart1, g_jpeg_buf, (uint16_t)jpeg_len, HAL_MAX_DELAY);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
/**
  * @brief  程序入口。
  * @details
  * 初始化流程：
  *  1) HAL 与系统时钟初始化
  *  2) GPIO/GPDMA/DCMI/UART/CRC 外设初始化
  *  3) OV2640 初始化并设置为 RGB565 320x240
  *  4) AI 网络初始化
  * 主循环每 INFER_PERIOD_MS 执行一次推理流程。
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_GPDMA1_Init();
  MX_DCMI_Init();
  MX_USART1_UART_Init();
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */
  delay_init();

  uint8_t ret = atk_mc2640_init();
  if (ret != ATK_MC2640_EOK)
  {
      while(1)
      {
          HAL_Delay(500);
      }
  }

#if (APP_MODE == APP_MODE_AI_INFER)
  atk_mc2640_set_output_format(ATK_MC2640_OUTPUT_FORMAT_JPEG);
  atk_mc2640_set_output_size(CAMERA_WIDTH, CAMERA_HEIGHT);

  if (ai_app_init() != 0U)
  {
      print_text("ai init failed\r\n");
      while(1)
      {
          HAL_Delay(500);
      }
  }

  print_text("=== NEW FW 2026-04-29 ===\r\n");
  print_text("mode=AI_INFER (jpeg capture test)\r\n");
#elif (APP_MODE == APP_MODE_AI_TEST_INPUT)
  if (ai_app_init() != 0U)
  {
      print_text("ai init failed\r\n");
      while(1)
      {
          HAL_Delay(500);
      }
  }

  print_text("=== NEW FW 2026-04-29 ===\r\n");
  print_text("mode=AI_TEST_INPUT\r\n");
#elif (APP_MODE == APP_MODE_SNAPSHOT)
  atk_mc2640_set_output_format(ATK_MC2640_OUTPUT_FORMAT_JPEG);
  atk_mc2640_set_output_size(CAMERA_WIDTH, CAMERA_HEIGHT);
#else
  atk_mc2640_set_output_format(ATK_MC2640_OUTPUT_FORMAT_JPEG);
  atk_mc2640_set_output_size(CAMERA_WIDTH, CAMERA_HEIGHT);
  print_text("=== NEW FW 2026-04-29 ===\r\n");
  print_text("mode=XCAM_VIEW\r\n");
#endif

  HAL_Delay(1500);  /* 等待AWB白平衡+AEC曝光收敛 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
#if (APP_MODE == APP_MODE_AI_INFER)
    capture_and_infer_once();
    HAL_Delay(INFER_PERIOD_MS);
#elif (APP_MODE == APP_MODE_AI_TEST_INPUT)
    run_test_input_once();
    HAL_Delay(INFER_PERIOD_MS);
#elif (APP_MODE == APP_MODE_SNAPSHOT)
    snapshot_send_once();
    while (1)
    {
      HAL_Delay(1000);
    }
#else
    xcam_send_frame_once();
#endif
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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_0;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLMBOOST = RCC_PLLMBOOST_DIV4;
  RCC_OscInitStruct.PLL.PLLM = 3;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 1;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLLVCIRANGE_1;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
