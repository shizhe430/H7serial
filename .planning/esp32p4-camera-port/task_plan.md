# ESP32-P4 camera proof-of-concept

## Goal

Install a reproducible ESP-IDF toolchain and create an independent ESP32-P4 project that ports the camera behavior behind STM32 `APP_MODE_XCAM_VIEW`, using the reviewed DVP pin mapping and the existing OV2640 module.

## Success criteria

- ESP-IDF and the ESP32-P4 build tools can be initialized from a documented command.
- The new project targets `esp32p4` and builds without source or dependency errors.
- OV2640 DVP pins match the reviewed mapping workbook.
- The application initializes the camera and continuously validates captured frames.
- Where hardware is connected, flash/monitor steps are documented; actual image validity remains a bench test.

## Phases

- [complete] Inspect STM32 `APP_MODE_XCAM_VIEW` and local board/reference material.
- [complete] Install and verify a stable ESP-IDF release suitable for ESP32-P4.
- [complete] Create a separate ESP32-P4 camera project and port the camera behavior.
- [complete] Build, inspect configuration, and write board bring-up instructions.
- [complete] Record hardware-only verification items and final outcome.

## Decisions

- Keep the ESP32-P4 application in an independent project directory rather than inside the STM32 source tree.
- Reuse the external ESP32-C6 later; it is outside this camera-first proof of concept.
- Prefer the existing OV2640 DVP module. MIPI is a fallback only after a documented DVP hardware/driver failure.
- Use ESP-IDF v5.5.5 with `esp_video` and `esp_cam_sensor` v2.3.0.
- Use the sensor's native 320x240 JPEG mode; do not add a P4 JPEG encode stage.
- Default to diagnostic logs; provide an optional raw JPEG UART mode compatible with the STM32 stream behavior.

## Errors

- `session-catchup.py` was not present under the expected skill path; continued from the existing planning files and conversation summary.
- Bundled `rg.exe` was denied by Windows; use PowerShell `Select-String` for repository search.
- `idf.py create-project-from-example` parsed a trailing `-p` as another action. The downloaded example was verified and moved to the intended independent project path.
