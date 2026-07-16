# H7serial Rules

This rules file tracks the active blueprint for the `STM32H743IIT6 + OV2640 + XCAM` bring-up work.

## Canonical Blueprint

The canonical project blueprint is:

- [H7serial.md](C:\Users\heshizhe\Desktop\STM32\H7serial\H7serial.md)

## Working Rule

Before changing camera, DCMI, DMA, SCCB, cache, or UART streaming logic:

1. Read `H7serial.md`.
2. Treat `H7serial.md` as the source of truth for completed steps, blocked issues, and test history.
3. Keep this rules file updated whenever the blueprint location or usage rule changes.
4. Keep `H7serial.md` updated whenever progress, findings, or blocking issues change.

## Current Goal

Make the STM32H743 project output a stable `320x240` OV2640 JPEG stream that can be displayed in XCAM.

Current parallel goal:

Add a second compile-time mode `APP_MODE_AI_INFER` that keeps `APP_MODE_XCAM_VIEW` intact while using a raw-frame camera path for on-board inference.

## X-CUBE-AI Integration Rule

Before enabling or regenerating `X-CUBE-AI` from CubeMX:

1. Commit the current working state first.
2. Keep AI-related changes on a dedicated branch.
3. Change only the `.ioc` AI configuration first, then regenerate once.
4. Immediately inspect the following files after generation before doing any manual merge work:
   - `Core/Src/dcmi.c`
   - `Core/Src/gpio.c`
   - `Core/Src/dma.c`
   - `Core/Src/usart.c`
   - `Core/Src/main.c`
   - `Core/Src/stm32h7xx_it.c`
   - `Core/Inc/stm32h7xx_it.h`
   - `H7serial.ioc`
5. Re-check these camera-critical invariants after every CubeMX regeneration:
   - `DCMI_D5/D6/D7 = PD3/PB8/PB9`
   - `PCKPolarity = RISING`
   - `VSPolarity = LOW`
   - `HSPolarity = LOW`
   - `USART1 baud = 921600`
   - `DMA2_Stream3_IRQn priority = 5`
   - `DCMI_IRQHandler()` declaration and implementation still exist
   - `HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PA0, SYSCFG_SWITCH_PA0_OPEN)` still exists
   - `CameraApp_Init()` and `CameraApp_Run()` calls in `main.c` still exist
6. Keep camera bring-up logic in custom source files, not in CubeMX-generated peripheral files unless only a `USER CODE` hook is possible.

## Current Diagnostic Rule

- For XCAM black-screen cases with nonzero RX count, check JPEG frame extraction before changing DCMI polarity.
- Prefer the last complete `FFD8 ... FFD9` span in the DMA buffer; do not use the first forward match when residual bytes may exist in the buffer.
- The verified active H7 camera data mapping is:
  - `D5 = PD3`
  - `D6 = PB8`
  - `D7 = PB9`

## AI Mode Rule

- `APP_MODE_XCAM_VIEW` and `APP_MODE_AI_INFER` must stay isolated.
- `APP_MODE_PUMP_CTRL` must stay isolated from both preview and debug-infer behavior.
- `APP_MODE_AI_VISUAL` must stay isolated from all working modes and serve only as a host-side demonstration mode.
- The current H7 AI baseline is `JPEG snapshot -> TJpgDec decode -> training-aligned preprocess -> infer`.
- Preferred AI path on H7:
  - OV2640 JPEG snapshot capture
  - TJpgDec decode to camera frame
  - firmware-side training-aligned preprocess to the model input
  - direct `waterlevel` inference
- Keep XCAM preview mode on the existing JPEG path so camera bring-up remains independently verifiable.
- On H7 + X-CUBE-AI, do not enable alignment trapping for the CM7 core while running the generated AI runtime.
- Keep AI activations, input, and output buffers as standalone aligned arrays; do not pack them back into a mixed metadata struct.
- Keep `RAM_D1` cacheable for AI buffers.
- Keep only the dedicated DMA window non-cacheable:
  - `0x30000000`
  - `64KB`
  - used for the JPEG frame buffer in `.dma_buffer`
- Current performance baseline after fixing MPU / buffer placement:
  - `APP_MODE_AI_TEST_IMAGE`: about `76ms`
  - `APP_MODE_AI_INFER`: about `199ms pipe`, `76ms nn`, `5.0 fps`
  - remaining optimization target is the front-end path: capture + JPEG decode + preprocess
- After any CubeMX/X-CUBE-AI regeneration, re-check:
  - `camera_app.h` still has both mode macros
  - `camera_app.c` still uses:
    - JPEG snapshot capture in `APP_MODE_AI_INFER`
    - `jpeg_to_ai_input()` before inference
  - `jpeg_decode.c` still matches the training preprocess contract
  - `main.c` still clears `SCB_CCR_UNALIGN_TRP_Msk` before AI inference is used
  - `main.c` still configures the MPU DMA window size as `MPU_REGION_SIZE_64KB`

## Model Replacement Rule

- Before replacing the model with CubeMX / X-CUBE-AI, commit the current working state first.
- The current H7 AI baseline is `JPEG snapshot -> TJpgDec decode -> training-aligned preprocess -> infer`.
- Do not accidentally revert to the older `RGB565 direct -> infer` branch during regeneration.
- Do not treat model replacement as an AI-only change. In this project it is a full recovery event touching clock, MPU, UART, camera, and generated model glue.
- After regeneration, re-check these files immediately:
  - `Core/Inc/camera_app.h`
  - `Core/Src/camera_app.c`
  - `Core/Src/main.c`
  - `Core/Src/dcmi.c`
  - `Core/Src/usart.c`
  - `Core/Src/stm32h7xx_it.c`
  - `Core/Src/ov2640.c`
- After regeneration, verify these invariants:
  - `HSE = external crystal`
  - `HSE_VALUE = 25000000`
  - PLL1 remains `M=5 / N=192 / P=2 / Q=2`
  - `PCKPolarity = DCMI_PCKPOLARITY_RISING`
  - `PD3/PB8/PB9 = D5/D6/D7`
  - `USART1 baud = 921600`
  - `SCB->CCR &= ~SCB_CCR_UNALIGN_TRP_Msk;`
  - MPU DMA window remains `0x30000000` / `64KB` / non-cacheable / `MPU_TEX_LEVEL1`
  - `DCMI_IRQHandler()` still calls `HAL_DCMI_IRQHandler(&hdcmi);`
  - `Release` optimization stays at `-O3` instead of falling back to `-Os`
  - `STM32H743IITX_FLASH.ld` still contains:
    - `.ai_ram_d1 > RAM_D1`
    - `.dma_buffer > RAM_D2`
    - `.ai_dtcm > DTCMRAM`
  - `Core/Src/camera_app.c` still places `g_ai_activations` in `.ai_ram_d1`
  - `Core/Src/jpeg_decode.c` still places grayscale scratch / CLAHE LUT in `.ai_ram_d1`
  - `Core/Src/jpeg_stream.c` still places the JPEG DMA frame buffer in `.dma_buffer`
  - do not revert to a full-`RAM_D1` non-cacheable MPU policy; that pushes pure `AI_TEST_IMAGE` inference from about `76ms` back toward about `160ms`
- After regeneration, verify these mode-specific behaviors:
  - `APP_MODE_PUMP_CTRL` still prints normal boot logs
  - `APP_MODE_AI_VISUAL` stays binary-stream only
  - `APP_MODE_AI_VISUAL` viewer must use `USART1 = 921600`
  - `APP_MODE_XCAM_VIEW` still produces a valid camera stream
- These custom files must remain in the build:
  - `Core/Src/jpeg_decode.c`
  - `Core/Src/tjpgd.c`
  - `Core/Inc/jpeg_decode.h`
  - `Core/Inc/tjpgd.h`
  - `Core/Inc/tjpgdcnf.h`
- Re-check the generated model report every time:
  - `X-CUBE-AI/App/<model>_generate_report.txt`
  - sync input/output shape, type, scale, and zero-point into `camera_app.c`
- Re-check that the actual generated model set used by the app is the intended one, especially if both `waterlevel_*` and `water_detect_*` exist in the tree.
- Re-check that CubeMX / X-CUBE-AI did not pull unrelated demo sources into `X-CUBE-AI/App` such as:
  - legacy `ov2640.*`
  - `test.c`
  - any source that includes missing headers like `../Common/camera.h` or `reg51.h`
- Recommended recovery order after model replacement:
  1. restore fixed invariants
  2. boot log
  3. UART
  4. camera probe
  5. JPEG capture
  6. JPEG decode / preprocess
  7. AI infer
- If `APP_MODE_AI_TEST_IMAGE` matches PC but live AI is wrong, prioritize preprocess/image-quality/camera-position checks over model-runtime suspicion.

## Pump Control Rule

- `APP_MODE_PUMP_CTRL` is the integrated dispense-control mode.
- It must remain isolated from:
  - `APP_MODE_XCAM_VIEW`
  - `APP_MODE_AI_INFER`
  - `APP_MODE_AI_TEST_IMAGE`
- The pump architecture must now support three workflow paths sharing one backend:
  - full automatic
  - voice semi-automatic
  - full mechanical
- All three paths must converge onto the same shared control variables:
  - `state`
  - `target`
  - `temp`
  - `pump_cmd`
- Full automatic mode:
  - requires stable visual cup detection
  - uses the 5-second command wait window
  - defaults to `full + cold` after timeout
- Voice semi-automatic mode:
  - requires stable visual cup detection
  - enters through the same 5-second command wait window
  - voice layer only assigns shared target / temperature values
  - after a valid voice command is accepted, delay pump start by `1s`
  - this buffer is reserved for the module's local acknowledgment speech
  - currently enabled protocol IDs:
    - `0x01 = half cold`
    - `0x02 = full cold`
  - reserved for later hot-water hardware phase:
    - `0x03 = half hot`
    - `0x04 = full hot`
- Full mechanical mode:
  - must be independent of visual gating
  - must be able to start directly from standby by key input
  - must not require prior cup detection
  - must not auto-stop on visual class `0`
  - must not auto-stop on visual class `4`
  - must stop only on the matching key action
- Priority rule:
  - key input > voice input > auto-timeout fallback
- Visual decision rule:
  - automatic and voice paths use continuous 3-frame confirmation
  - full mechanical path may log/display visual classes but must not use them for stop control
- Voice module integration rule:
  - use `I2C2` on `PB10/PB11`
  - do not use `PB6/PB9` because `PB9` is occupied by `DCMI_D7`
  - retained announcer IDs:
    - `0x10 = dispensing`
    - `0x11 = aborted`
    - `0x12 = done`
    - `0x13 = cup detected`
  - when entering `WAIT_CMD`, first播报 `0x13` to告诉用户当前处于命令输入窗口
  - after every CubeMX generation for voice I2C, immediately re-check and restore if needed:
    - `DCMI PCKPolarity = RISING`
    - `USART1 baud = 921600`
    - `DMA2_Stream3_IRQn priority = 5`
    - `HSE_VALUE = 25000000`
    - `SYSCFG_SWITCH_PA0 = OPEN`
    - MPU DMA window remains `0x30000000 / 64KB / non-cacheable`
    - `DCMI_IRQHandler()` declaration and implementation still exist
- Keep pump hardware access inside the dedicated pump driver layer.
- Keep workflow/state-machine logic outside the low-level driver layer.
- Debug prints are allowed during bring-up, but the final competition version should be able to run with pump control logic decoupled from verbose UART output.

## AI Visual Rule

- `APP_MODE_AI_VISUAL` is for competition presentation only.
- It must not alter the behavior of:
  - `APP_MODE_PUMP_CTRL`
  - `APP_MODE_XCAM_VIEW`
  - `APP_MODE_AI_INFER`
- This mode should send:
  - JPEG image
  - AI metadata
  - no human-readable boot/debug text on the same UART stream
- The current host packet magic is:
  - `AIV1`
- Host-side viewer script path:
  - `tools/ai_visual_viewer.ps1`

## Rapid Recovery Checklist

- If build fails with `hdcmi undeclared`:
  - restore `extern DCMI_HandleTypeDef hdcmi;`
  - restore `DCMI_IRQHandler()` calling `HAL_DCMI_IRQHandler(&hdcmi);`
- If boot mode is wrong:
  - re-check `Core/Inc/camera_app.h`
  - CubeMX regeneration may have disturbed the active `APP_MODE`
- If boot log exists but camera probe becomes `0xFFFF/0xFFFF`:
  - first suspect `gpio.c` / `PA0` analog switch / SCCB pin configuration, not JPEG logic
- If flashing succeeds but runtime hangs near inference:
  - first re-check `main.c` for:
    - `SCB->CCR &= ~SCB_CCR_UNALIGN_TRP_Msk;`
    - AXI SRAM MPU region still `512KB`
- If XCAM is black after regeneration:
  - first re-check `dcmi.c`, `gpio.c`, and `usart.c`
  - only debug JPEG framing after those invariants are restored
