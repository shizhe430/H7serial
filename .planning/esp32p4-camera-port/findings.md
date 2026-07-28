# Findings

## Starting state

- Current STM32 project: `C:\Users\heshizhe\Desktop\STM32\H7serial`.
- No active `idf.py`, CMake, or Ninja was found in the current PowerShell environment.
- Reviewed P4 mapping assigns OV2640 D0-D7 to GPIO2-GPIO9, PCLK/VSYNC/HREF to GPIO10/11/12, optional XCLK to GPIO13, SCCB to GPIO14/15, RESET/PWDN/flash to GPIO16/17/18.
- The selected DNESP32P4M module has PSRAM and does not integrate C6.

## Unknowns to resolve

- Exact behavior owned by `APP_MODE_XCAM_VIEW` beyond camera capture.
- Whether the existing OV2640 module has an onboard oscillator or requires GPIO13 XCLK.
- DVP PCLK, VSYNC, and HREF polarities on the physical module.

## STM32 XCAM behavior

- `APP_MODE_XCAM_VIEW` probes and initializes OV2640, selects JPEG at 320x240, captures snapshots, finds SOI/EOI, validates range and JPEG markers, and emits the raw JPEG stream over UART1.
- It treats two consecutive capture/format failures as a camera recovery trigger.
- No AI inference, pump, voice, fingerprint, or C6 behavior is part of this mode.

## ESP-IDF and component basis

- ESP-IDF v5.5.5 is installed under `C:\Espressif` and reports `esp32p4` as a supported target.
- Independent project path: `C:\Users\heshizhe\Desktop\ESP32P4\p4_xcam_view`.
- `esp_video` and `esp_cam_sensor` v2.3.0 were resolved from the Espressif Component Registry.
- The official OV2640 component provides native `320x240 JPEG 50fps` for 8-bit DVP with 20 MHz XCLK.
- The P4 DVP video device supports `V4L2_PIX_FMT_JPEG`; software re-encoding is unnecessary for this proof of concept.

## Final implementation

- GPIO mapping is fixed in `sdkconfig.defaults.esp32p4`: D0-D7 GPIO2-9, PCLK GPIO10, VSYNC GPIO11, HREF/DE GPIO12, XCLK GPIO13, SCCB GPIO14/15, RESET GPIO16, PWDN GPIO17.
- The runtime selects only 320x240 native JPEG, validates SOI/EOI, logs frame statistics, and restarts the video stack after two consecutive invalid frames.
- An optional `CONFIG_XCAM_RAW_JPEG_STREAM` mode sends validated JPEG frames on UART0 at 921600 bit/s and disables runtime logs before binary streaming.
- Flash size is fixed at 16 MB and PSRAM at 32 MB/200 MHz configuration expected by DNESP32P4M.
- No serial port was detected on the host, so PID detection, frame validity, image content, and signal polarity still require the physical board and camera.
