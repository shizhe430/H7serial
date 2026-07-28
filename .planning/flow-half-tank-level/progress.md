# Progress

## 2026-07-18
- Created a scoped plan for flow calibration, half-cup fallback, and PB5 tank-level integration.
- Recorded the planning catch-up path error and recovery.
- Confirmed the current flow calibration is still 17301 ms per 400 ml and PB5 is unused in the target project.
- Restricted reference-project inspection to source and CubeMX files after an ELF binary was accidentally included in a read-only search.
- Extracted the new 400 ml calibration point as 22700 ms and confirmed the sensor is active-high with no internal pull.
- Completed the control design: visual-first 11350 ms half fallback and 300 ms active-low tank protection across all pump modes.
- Implemented the 22700 ms flow calibration, half timeout service, PB5 debounce/protection, OLED tank-low override, and CubeMX-aligned pin metadata.
- Built Release successfully; final ELF size is 315868 bytes text, 19232 bytes data, and 358556 bytes BSS.
- Audited the ELF and arithmetic: formal mode is preserved, half timeout and tank-low strings are linked, and 11350/22700 ms map to 200/400 ml.
- Manually verified all scoped plan phases complete because the helper checked the unrelated legacy root plan.
- Reopened the scoped task after the user clarified that PB5 must only drive OLED water-level display.
- Confirmed OLED water-level semantics and isolated unchecked USART3 transmission as the STM32 reporting reliability defect.
- Removed every PB5 pump-control, fault, speech, ESP32, and serial-log side effect; PB5 now maps only to OLED water level.
- Added checked USART3 sends with three retries, success/failure diagnostics, and success-only report flags.
- Rebuilt Release successfully; audited ELF strings and confirmed all tank-control artifacts are absent.
- Prepared a two-sided hardware check using `[ESP32_TX]` and the ESP32 UART receive log.
