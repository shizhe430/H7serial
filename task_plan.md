# Model Stability and Formal Workflow Recovery

## Goal
Restore stable STM32 water-level inference and return the firmware to the formal pump-control workflow without losing the verified ESP32-C6 reporting interface.

## Success Criteria
- Board preprocessing and model I/O conversion match the current QAT INT8 model.
- Static cup scenes do not jump arbitrarily across all five classes because of firmware defects.
- Low/half/full postprocessing uses the approved 0.395 and 0.665 boundaries.
- DCMI capture and cache handling match the known stable camera pipeline.
- Formal `APP_MODE_PUMP_CTRL` is selected; ESP32 diagnostic traffic and PB10 GPIO delay are disabled.
- Release build succeeds, with automatic/voice/manual targets and abnormal/cup-loss stops still reachable.

## Phases
- [completed] 1. Compare current camera, model I/O, and postprocessing against generated metadata and PC reference code.
- [completed] 2. Compare camera/DMA/cache configuration against stable Git baselines and identify regressions.
- [completed] 3. Apply only confirmed fixes and restore formal runtime configuration.
- [completed] 4. Build, inspect the resulting binary, and provide a focused board verification sequence.
- [in_progress] 5. Flash and verify ESP32 fingerprint/volume session reporting on hardware.

## Constraints
- Preserve the current generated QAT INT8 model files.
- Preserve STM32-to-ESP32 `STM_FP` and `STM_DONE` reporting.
- Do not alter unrelated pump, voice, fingerprint, OLED, or safety behavior.

## Errors Encountered
- `rg.exe` cannot execute in this environment (`Access is denied`); use PowerShell `Select-String`.
- First session-catchup path used `.codex`; the installed planning skill is under `.agents`.
- Hardware flash attempt failed because ST-Link reported target voltage `0.00 V` and no core ID; retry only after the board is powered.
- UI Automation could not find the viewer's custom-styled Connect button. Replaced external clicking with script-level auto-connect when a port is selected.
- A one-line UI Automation enumeration command had an invalid empty PowerShell pipeline element; it was not repeated because auto-connect removes the need for enumeration.
- ESP32 integration ELF flashing did not start because ST-Link measured only 1.15 V and could not read the target core ID.
- Integration commit `104c380` could not be pushed because HTTPS connections to GitHub timed out; stable baseline `2b995ea` was pushed before the outage.
