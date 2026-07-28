# Progress

## 2026-07-22

- Started a camera-first ESP32-P4 migration task.
- Confirmed ESP-IDF tools are not currently available in the shell.
- Preserved the STM32 project and prior migration artifacts unchanged.
- Downloaded the official 1.62 GB ESP-IDF v5.5.5 offline installer from `dl.espressif.com`.
- Installed and verified ESP-IDF v5.5.5, Python 3.11.2 dependencies, and ESP32-P4 target support.
- Created the independent P4 project from Espressif's `esp_video` 2.3.0 `capture_stream` example.
- Resolved official managed components and confirmed native OV2640 QVGA JPEG support.
- Ported the XCAM camera path with continuous V4L2 capture, JPEG marker validation, error counters, and video-stack recovery.
- Added optional raw JPEG UART streaming at the STM32-compatible 921600 bit/s.
- Built the default diagnostic mode and raw JPEG mode successfully, then restored and rebuilt the default diagnostic mode.
- Final image is approximately 481 KB; 54% of the 1 MB app partition remains free.
- No COM port was present, so flashing and physical image verification were not performed.
