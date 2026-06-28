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
- Do not force AI mode through the JPEG preview path unless raw-frame capture is proven impossible.
- Preferred AI path on H7:
  - OV2640 output format `RGB565`
  - DCMI snapshot raw frame capture
  - firmware-side preprocess to `224x224x1 int8`
  - direct `waterlevel` inference
- Keep XCAM preview mode on the existing JPEG path so camera bring-up remains independently verifiable.
- On H7 + X-CUBE-AI, do not enable alignment trapping for the CM7 core while running the generated AI runtime.
- Keep AI activations, input, and output buffers as standalone aligned arrays; do not pack them back into a mixed metadata struct.
- Keep the D1 AXI SRAM MPU region covering the full `0x24000000-0x2407FFFF` 512KB window once AI buffers are placed there.
- After any CubeMX/X-CUBE-AI regeneration, re-check:
  - `camera_app.h` still has both mode macros
  - `ov2640.h/.c` still contains `OV2640_SetOutputFormatRGB565()`
  - `camera_app.c` still selects:
    - JPEG buffer in `APP_MODE_XCAM_VIEW`
    - raw RGB565 frame buffer in `APP_MODE_AI_INFER`
  - `main.c` still clears `SCB_CCR_UNALIGN_TRP_Msk` before AI inference is used
  - `main.c` still configures AXI SRAM MPU size as `MPU_REGION_SIZE_512KB`
