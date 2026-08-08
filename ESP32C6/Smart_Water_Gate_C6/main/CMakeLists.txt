idf_component_register(SRCS "main.c" "deepseek.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES esp_http_server esp_http_client esp_driver_uart esp_driver_gpio nvs_flash esp_wifi esp_event lwip
                                 esp_netif esp_system json console
                                 esp_rainmaker app_network)
