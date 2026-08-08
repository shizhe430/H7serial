#pragma once
#include "esp_err.h"

#define DEEPSEEK_RESPONSE_MAX_LEN 1024

/**
 * @brief Send a query to DeepSeek API and get the text reply
 * @param user_message The user's input text (UTF-8)
 * @param response Output buffer for the assistant's reply
 * @param response_size Size of output buffer
 * @return ESP_OK on success
 */
esp_err_t deepseek_query(const char *user_message, char *response, size_t response_size);
