# qrcode is only needed if CONFIG_APP_NETWORK_PROV_SHOW_QR is enabled
if(CONFIG_APP_NETWORK_PROV_SHOW_QR)
    set(extra_priv_req qrcode)
else()
    set(extra_priv_req)
endif()

set(priv_req nvs_flash esp_event vfs network_provisioning openthread ${extra_priv_req})

idf_component_register(SRCS "app_wifi_internal.c" "app_network.c" "app_thread_internal.c"
                    INCLUDE_DIRS "."
                    PRIV_INCLUDE_DIRS "private_include"
                    PRIV_REQUIRES ${priv_req})

if(CONFIG_APP_WIFI_SHOW_DEMO_INTRO_TEXT)
    target_compile_definitions(${COMPONENT_TARGET} PRIVATE "-D RMAKER_DEMO_PROJECT_NAME=\"${CMAKE_PROJECT_NAME}\"")
endif()
