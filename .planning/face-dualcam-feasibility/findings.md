# Findings

- Current branch: codex/timed-demo-backup; remote branch is at commit 40723a4.
- Working firmware changes include ESP32 TX retries/logging, standby fingerprint polling, half-cup timeout fallback, updated 400 ml flow calibration, and PB5 tank-level input configuration.
- The current macro had been left in APP_MODE_XCAM_VIEW after diagnostics; the formal snapshot must use APP_MODE_PUMP_CTRL.
- Referenced CSDN article uses two OV7670 modules sharing one DCMI bus and GPIO-controlled PWDN selection. It is a topology reference, not an OV2640-qualified design.
- Official OV2640 datasheet: PWDN is active high; RESETB is active low. The datasheet does not explicitly guarantee that all DVP outputs become high impedance in hardware power-down, so directly tying two output buses is not production-safe without validation or a bus mux.
- STM32H743 has one DCMI instance. Two parallel cameras can only be selected in time; simultaneous capture is unavailable.
- Current map: .ai_ram_d1 is 0x3f980 bytes (water AI plus JPEG decode), DCMI JPEG DMA buffer is 0x10000 bytes, and no FMC/SDRAM/QUADSPI peripheral is enabled in the current .ioc.
- AS608/fingerprint is no longer part of the final next-stage pin plan. Face identity will replace it before dispensing.
- The safest workflow is state-based switching inside the existing five-second command window: water camera detects stable cup, face camera captures a short burst and latches an ID, water camera is restored and cup presence is revalidated before pumping.
- Face recognition remains optional identity enrichment. Unknown/failed recognition must stay id=0 and must never disable dispensing.
- A scalable implementation needs face detection/alignment plus an embedding model and similarity database; a fixed-user classifier is simpler but requires retraining for every added user.
- Final next-stage pin plan keeps the OV2640 water-camera DCMI pins unchanged and reserves external FMC/QSPI memory pins for face recognition. Do not use earlier temporary camera-control assignments as the final wiring reference.
- Face inference belongs before dispensing. Automatic/voice use the existing five-second window; mechanical starts after a bounded pre-dispense identity attempt, falling back to id=0.
- Water and face networks must execute sequentially and share an activation arena sized to max(water, face), not the sum.
- A public MobileFaceNet ONNX (1x3x112x112 to 128-D embedding) was accepted by X-CUBE-AI 10.2 with all operators supported.
- Its FP32 footprint is not deployable on H743: 3.78 MiB weights, 1.59 MiB activations, 239.7 MMACC. It needs INT8 quantization and possibly a narrower model.
- Open-source embedding avoids identity-model retraining, but each user still needs several enrollment frames to create an averaged feature template. Public face images can supply PTQ calibration data.
- ST Model Zoo YuNet 320x320 is not an H743 candidate as published because its documented internal RAM footprint is about 1.1 MiB.
- 2026-07-29 direct-parallel validation: cam0 and cam1 both read MID/PID 0x7FA2/0x2642 and both repeatedly produce valid JPEG frames.
- 2026-07-29 face-camera view validation: fixed cam1 XCAM_VIEW shows a clear image, proving the second OV2640 optical path is usable.
- The earlier direct-parallel wiring was only a validation fixture. It is not the final pin map for the upgraded LCD/external-memory design.
- Current generated water model footprint is weights 182,008 B and activations 167,280 B; X-CUBE-AI report total is about Flash 259,020 B and RAM 187,088 B for the model/runtime.
- Internal Flash remains sufficient for a second small INT8 face model, but internal RAM requires a shared activation arena and a constrained face model. Large MobileFaceNet/YuNet-style models still require reduction or external memory.
- Firmware switch implementation now has a common `camera_app_switch_camera_for_capture()` path. It stops DCMI, selects the target camera through OV2640 PWDN control, reinitializes the current DCMI profile, clears JPEG/frame state, reattaches the JPEG buffer, and logs `[CAM] switch ...`.
- The pre-dispense identity hook is placed after `cup_stable` voice announcement and before decision-window timing is calculated. This prevents the optional face-camera capture from shortening the user's voice-command window.
- `CAMERA_FACE_IDENTITY_ENABLE` remains `0U` by default. Enabling it currently performs a bounded face-camera JPEG validation and latches placeholder `id=0`; it is not real recognition yet.
- `APP_MODE_FACE_DIAG` is the standalone face-camera test mode. It selects `OV2640_CAMERA_FACE`, initializes JPEG capture, and prints `[FACE_DIAG] cam=1 frame=... jpeg=... fps=... valid=1`; it does not initialize the pump, water-level AI, fingerprint module, or ESP32 reporting.
- To test face camera on the breadboard, temporarily set `APP_MODE` to `APP_MODE_FACE_DIAG`, build, burn, and check the serial log. After testing, set it back to `APP_MODE_PUMP_CTRL`.

## Final H743 Pin Allocation

Source file for the final table: `C:\Users\heshizhe\Desktop\全国大学生嵌入式\STM32H7最终引脚分配.xlsx`.

OV2640 water camera remains unchanged:
- `PB7`: DCMI_VSYNC
- `PH8`: DCMI_HREF
- `PA6`: DCMI_PCLK
- `PC6/PC7/PC8/PC9/PC11/PD3/PB8/PB9`: DCMI_D0-D7
- `PB4`: software SCCB SCL
- `PB3`: software SCCB SDA
- `PA15`: DCMI_RESET
- `PA8`: DCMI_FLASH
- `PA0`: DCMI_PWDN

Peripheral and control pins:
- `PI1`: tank-level input
- `PI0`: pump PWM output
- `PD12`: voice I2C4_SCL
- `PH12`: voice I2C4_SDA
- `PA9/PA10`: USART1 TX/RX
- `PB10/PB11`: USART3 TX/RX to ESP32-C6
- `PB1`: LED0
- `PB0`: LED1
- `PE3`: DS18B20 one-wire data

LCD/FMC pins:
- `PB5`: LCD_BL
- `PD0/PD1/PD8/PD9/PD10/PD14/PD15`: FMC_D2/D3/D13/D14/D15/D0/D1
- `PE7/PE8/PE9/PE10/PE11/PE12/PE13/PE14/PE15`: FMC_D4-D12
- `PD4`: FMC_NOE
- `PD5`: FMC_NWE
- `PD7`: FMC_NE1
- `PD13`: FMC_A18
- `PG3`: T_MISO
- `PH6`: T_SCK
- `PH7`: T_PEN
- `PI3`: T_MOSI
- `PI8`: T_CS

External SDRAM pins:
- `PG15`: FMC_SDNCAS
- `PF11`: FMC_SDNRAS
- `PC2`: FMC_SDNE0
- `PC3`: FMC_SDCKE0
- `PG8`: FMC_SDCLK
- `PC0`: FMC_SDNWE
- `PE0`: FMC_NBL0
- `PE1`: FMC_NBL1
- `PF0/PF1/PF2/PF3/PF4/PF5/PF12/PF13/PF14/PF15`: FMC_A0-A9
- `PG0/PG1/PG2`: FMC_A10-A12
- `PG4/PG5`: FMC_BA0/BA1
- `PD14/PD15/PD0/PD1/PE7/PE8/PE9/PE10/PE11/PE12/PE13/PE14/PE15/PD8/PD9/PD10`: FMC_D0-D15

External QSPI Flash pins:
- `PB6`: QUADSPI_BK1_NCS
- `PB2`: QUADSPI_CLK
- `PF8`: QUADSPI_BK1_IO0
- `PF9`: QUADSPI_BK1_IO1
- `PF7`: QUADSPI_BK1_IO2
- `PF6`: QUADSPI_BK1_IO3

Known firmware migration items before enabling the final board configuration:
- Move tank-level input from `PB5` to `PI1`.
- Move pump control from `PH6/TIM12_CH1` to `PI0`.
- Move I2C4 SDA from `PD13` to `PH12`; keep I2C4 SCL on `PD12`.
- Remove all runtime use of `PF6/PF7` as GPIO or test buttons before enabling QSPI.
- Replace OLED GPIO display code with LCD/FMC display code when the LCD hardware is installed.

## External SDRAM diagnosis

- Continuity between STM32 `PE9/FMC_D6` and SDRAM `DQ6` passed, so the PCB trace is not open.
- The detailed test has produced different values on repeated reads of the same address (`0x0040` then `0x0000`), which points to read sampling instability rather than a stable address-map error.
- ALIENTEK experiment 14 confirms Bank1, 13 row bits, 9 column bits, 16-bit width, four internal banks, pull-ups, and the initialization command sequence.
- The conservative 80 MHz/CAS3/read-pipe-1 test still failed with stable D6-only `0x0040`, so it did not resolve the fault.
- The historical project is not a trusted SDRAM baseline: its SDRAM MPU region was cacheable and bufferable, which can let a CPU memory test pass in D-cache without proving external SDRAM operation.
- The SDRAM interface follows the official ALIENTEK experiment: dedicated PLL2R 200 MHz FMC kernel, SDCLK 100 MHz, CAS2, read-pipe 0, timing 2/8/6/6/2/2/2, mode-register CAS2, and refresh count 761.
- `HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_FMC)` returns zero because this H7 HAL getter has no FMC branch; it does not prove PLL2 is stopped. Diagnostics now read `PLL2_R_Frequency` directly and report `pll2_ready`.
- A PE9/D6 pull-dependency test now reads stored zero and stored `0x0040` with PE9 pull-up, pull-down, and no pull. If the returned D6 follows the GPIO pull, the SDRAM is not driving DQ6 during reads; if all pulls produce the same wrong value, the fault is elsewhere in the SDRAM write/read path.
- Board pull-dependency result was conclusive: stored zero read `0000` and stored D6 one read `0040` under pull-up, pull-down, and no pull. PLL2R was also confirmed at 200 MHz. PE9/DQ6 and the SDRAM bit cell are working; the failure only appears during consecutive-address reads, which repeat the first D6 value.
- The next single-variable test disables FMC ReadBurst while retaining the official 100 MHz/CAS2/timing configuration. Expected SDCR0 is `0x00000959`; success would identify the FMC burst-read path as the cause.
- Disabling FMC ReadBurst produced the expected `SDCR0=0x00000959` but did not change the repeated-D6 failure, so ReadBurst is excluded.
- Release disassembly confirms four distinct volatile `STRH` writes to offsets 0/2/4/6 and four distinct `LDRH` reads from those offsets; compiler optimization is not aliasing the addresses.
- The next diagnostic reports fast-write/fast-read, fast-write/barrier-read, barrier-write/fast-read, and barrier-write/barrier-read results to isolate write completion from read ordering.
- Sequence results show bit 6 is the only systematic corruption: samples whose low-byte D6 is already one remain correct, while zero samples increasingly read with `+0x0040`. Barriers do not resolve it, excluding CPU write/read ordering.
- The full sequence/self-test had restored PE9 to pull-up after the pull diagnostic. The next controlled run leaves PE9 with no pull for all sequence and full-memory tests to check whether the internal pull-up is consuming the D6 low-level margin.
- PE9 no-pull did not change the D6 corruption. The next test matches the official SDRAM MPU attributes (`TEX0`, non-shareable, cacheable, bufferable) but explicitly cleans and invalidates D-cache before verification, so a passing result must come from an external-memory roundtrip rather than a cache hit. Failure codes 13 and 14 identify the first and inverse-pattern passes.
- With official MPU attributes, the physical roundtrip read `0xA5523CC3` for expected `0xA55A3CC3`; the changed failing bit is high-halfword D3, not PE9/D6. This points to access/timing/cache-policy behavior rather than a fixed open data trace.
- ALIENTEK experiment 14 also sets `SCB_CACR_FORCEWT` and enables FMC ReadBurst. Its normal array test does not invalidate D-cache before reading, so a visible pass is not by itself a physical SDRAM-read proof. The next firmware applies FORCEWT only during the physical diagnostic, restores ReadBurst, then restores the original cache policy before camera/AI startup.
- FORCEWT and ReadBurst did not alter the physical failure. The remaining clock-tree difference is now isolated by `SDRAM_OFFICIAL_CLOCK_DIAG`: it changes PLL1 from 480 to 400 MHz and selects the 200 MHz D1HCLK directly for FMC, matching experiment 14 while leaving the production `.ioc` clock configuration unchanged.
- The official experiment includes `mpu.c` but never calls `MPU_Memory_Protection()` and does not define `DATA_IN_ExtSDRAM`; its SDRAM accesses therefore use the Cortex-M7 default external Device memory map. The project's explicit Normal-memory SDRAM region was a real execution-path difference that can permit merged/speculative transactions. Region 1 is now disabled for the diagnostic, while the DMA and QSPI MPU regions remain intact.
- The ALIENTEK archive contains its original prebuilt `OBJ/Template.hex`, so the board can be tested with the vendor's exact binary without changing or rebuilding the example source.
- The exact vendor image does not pass a physical SDRAM test on this board. Its cache-visible `KEY1` array print shows `0,1,2...`, but its sparse 16-KB `KEY0` capacity test produces 1024-KB jumps/repeats instead of a monotonic 16-KB sequence.
- A CPU-side probe executed after vendor initialization disabled D-cache, read SDRAM through the H743 core, and copied the values into internal SRAM. It returned `0040,0041,...004F` for expected `0000,0001,...000F`; bit D6 is forced high in this real roundtrip.
- Therefore the persistent failure is not caused by CubeMX regeneration, the drinking-machine clock tree, MPU policy, cache maintenance, ReadBurst, ReadPipe, PLL2 selection, or ST-LINK's external-memory read path. It remains an electrical/board-level FMC D6 path problem or contention from another device sharing PE9/FMC_D6.
- PE9-to-SDRAM-DQ6 continuity alone is insufficient: it does not exclude leakage/shorting, a damaged SDRAM DQ6 output, soldering defects, or another FMC device driving the shared D6 bus.
- Apollo core-board schematic page 3 maps `FMC_D6` to SDRAM U3/W9825G6KH DQ6 pin 11 and NAND U4/MT29F4G08 I/O6 pin 43; the net is also exported through the core-board connectors.
- PG9/FMC_NCE3 sampled high with PG9 configured as high-impedance input, and actively driving PG9 high did not change the `A55A3CC3 -> A5523CC3` failure. Normal NAND selection and a weak R17 pull-up are excluded, although a damaged NAND I/O6 leakage path remains electrically possible.
- A controlled 50 MHz run used PLL2R 100 MHz, SDCLK 50 MHz, refresh count 370, CAS2, the official 400/200 MHz CPU/HCLK tree, and all three ReadPipe values. Two consecutive boots failed identically at `0xC0000008`: expected `0x851ABDC1`, read `0x855ABDC1`, diff `0x00400000`. This is D6 in the high halfword and excludes a 100 MHz signal-margin-only explanation.
- After a full power cycle with the LCD physically disconnected, two more 50 MHz boots reproduced the exact same address, expected value, returned value, and `0x00400000` D6 difference for ReadPipe 0/1/2. The LCD and its connector-side FMC loading are excluded as the cause.
- The old official QSPI example leaves W25Q256 in QPI mode across MCU reset, so the project's subsequent single-line QSPI diagnostic reports `FFFFFF` until the flash is power-cycled or explicitly sent a four-line exit-QPI command. This is independent of the SDRAM result.

## Yakhyo face model and LCD memory audit

- `yakhyo/face-recognition` is a training/inference framework, not a single detector-recognizer MCU model. Its recognition networks require an aligned `1x3x112x112` face crop and produce a 512-element embedding; its example full pipeline separately invokes UniFace RetinaFace for detection and five-landmark alignment.
- The smallest released recognizer is `MobileNetV1_0.25`: 357,968 parameters, 1,452,631-byte FP32 ONNX, and reported LFW accuracy 98.76%. The other released ONNX files range from 4.79 MiB to 132.03 MiB and cannot fit internal H743 Flash with the existing application.
- STM32Cube.AI 10.2 successfully analyzed the unmodified FP32 MobileNetV1 model. It requires 1,463,540 bytes of Flash and 836,836 bytes of RAM, including 817,248 bytes of activations. This cannot coexist with the current firmware and LCD assets in 2 MiB internal Flash, and its activation arena exceeds every individual internal SRAM bank.
- A calibration-only QDQ INT8 conversion reduced MobileNetV1 weights to 380,380 bytes, but STM32Cube.AI still requires 802,816 bytes of activations and 836,044 bytes total RAM. Quantized convolutions are converted back to FP32 around the model's PReLU layers, producing a single 802,816-byte tensor that cannot fit the H743's largest 512-KiB internal SRAM bank. This conversion is for memory feasibility only and is not accuracy-qualified.
- The current drinking-machine ELF uses 343,100 bytes of internal Flash and 377,860 bytes of internal RAM. Its 260,480-byte water-AI/preprocessing region can be overlaid during face inference, but overlaying does not solve the face model's 802,816-byte single-buffer requirement.
- The compiled LCD ELF uses 1,004,556 bytes of Flash but only 2,272 bytes of RAM. It writes directly to LCD GRAM and has no full-screen SRAM framebuffer. Most Flash use is image data: `gImage_background` is 768,000 bytes and `gImage_9th_logo2` is 159,728 bytes. Moving these assets to QSPI can recover about 906 KiB of internal Flash without requiring SDRAM.
- Therefore the released FP32 model plus the current LCD assets does not fit internal memory. Recognition-only MobileNetV1 INT8 is marginally acceptable in Flash but still cannot run from internal RAM in its current architecture; a reliable external SDRAM or a retrained/changed fully quantizable network is required. Adding the repository's separate face detector makes the internal-only option even less feasible.

## ALIENTEK experiment 65 cross-check

- The supplied experiment 65 is an STM32H743 comprehensive application, not an F4/F7 binary. It calls `SDRAM_Init()`, reserves a 2,048,000-byte LTDC framebuffer at `0xC0000000`, and creates a 28,912-KiB external allocator beginning at `0xC01F4000`.
- This experiment provides a stronger functional SDRAM observation than a cache-visible CPU array print because LTDC continuously reads the external framebuffer independently of the Cortex-M7 data cache. A stable, artifact-free RGB display would prove that the SDRAM bus works for this firmware's LTDC access pattern, although it would not by itself invalidate the repeatable CPU physical-roundtrip D6 failures.
- Durable retrospective records now live in `docs/project-retrospective-materials.md` and `docs/retrospective-evidence/`. The archive contains 23 original serial logs and the final pin workbook; temporary Codex attachment paths are no longer the only copy of this evidence.
- The original `OBJ/TEST.hex` SHA-256 is `FDC0B7B1C66F670C668FE2A90C988F9EDCAF459638BBC44206EED5F9B3001FAA`.
