# Findings

## Repository State
- Branch: `codex/xcubeai`; latest commit is `945e072` from 2026-07-13.
- There is no Git commit at the reported stable time of Wednesday 2026-07-15 23:16.
- Current generated model timestamp is 2026-07-15 22:43:45 and origin name is `v3_w05_origline_plus_newdata_qat_st_pure_int8`.
- The working tree contains substantial uncommitted CubeMX/model/integration changes, so no broad reset is safe.

## Confirmed Configuration Risks
- Formal mode and ESP32 diagnostics have now been corrected in source; hardware verification is pending.
- Current DCMI DMA interrupt priority differs from the older Git baseline (`0` current versus `5` baseline), but priority 0 was already compiled in the Wednesday 23:05 stable build and is therefore not the new regression.
- Current model postprocessing constants are 0.395/0.665 as requested.
- Current hard-coded output quantization constants have been verified against generated model metadata.

## Preserved Functionality
- USART3 is configured as PB10 TX / PB11 RX, 115200 8N1.
- Physical PB10 GPIO operation and STM32-to-ESP32 reception have been verified on hardware.
- Formal `STM_FP` and `STM_DONE` functions exist in the current pump-control code and must be retained.
- Completed sessions calculate volume from the measured 400 ml / 18.058 s calibration and report both user ID and volume.
- Normal, manual, and fault stops use the same session reporter, then require stable cup removal before automatic rearming.

## Model I/O Alignment
- X-CUBE-AI report confirms input int8 scale/zero-point `0.003920059/-128`.
- X-CUBE-AI report confirms classification output scale/zero-point `0.074205332/9`.
- X-CUBE-AI report confirms regression output scale/zero-point `0.002708061/-128`.
- Current firmware output dequantization constants exactly match the report.
- Current input mapping `gray - 128` is valid for the generated input scale, which is effectively `1/255`.
- Current 1/2/3 postprocessing is structurally identical to PC `board_style_class` and uses the approved thresholds.
- Pump target logic still used the old half regression stop of 0.50 and only had a class-based stop for full at 0.68. It has now been supplemented with three-frame target confirmation at 0.395 for half and 0.665 for full.

## Preprocessing
- PC reference applies OpenCV BGR-to-gray, OpenCV CLAHE (clip 2.0, 8x8), 320x240 pad to 320x320, center crop 224x224, radius-100 circular mask, then `/255`.
- Firmware geometry, mask, and normalization order match the PC reference.
- Firmware CLAHE is a custom approximation. Its residual histogram redistribution and interpolation coordinates are not bit-equivalent to OpenCV CLAHE. This is a real PC/MCU mismatch, but it predates the reported Wednesday stable state and is not yet proven to be the new regression.
- AI visual and pump modes infer from JPEG decoded by `jpeg_decode.c`; the separate RGB565 preprocessing path is not the active path for these modes.
- Offline comparison on 150 `teste` images: PC preprocessing classified 149/150 correctly; an exact simulation of the current MCU integer preprocessing classified 146/150 correctly.
- The MCU preprocessing mismatch changed 5/150 predictions, including three half-water images becoming low-water. It explains some low/half boundary instability but not arbitrary 0-4 jumps.
- After aligning grayscale coefficients, CLAHE residual distribution, and tile-center interpolation, simulated MCU preprocessing differs from OpenCV by only 0.00003 mean normalized pixel value; all 30 half samples are classified correctly.

## Camera Pipeline
- Current DCMI pixel clock edge is `RISING`, matching the user's hardware observation that restored stable image transport.
- Current DMA2 Stream3 interrupt priority is 0; committed formal baseline used priority 5. This changed before the new model generation and still requires interaction review.
- Stable Git camera baselines already used RISING PCLK, the same GPIO pull configuration, DMA normal mode, low DMA priority, and FIFO disabled. These are not new differences.
- JPEG DMA buffer occupies exactly 64 KiB at `0x30000000` in the MPU non-cacheable D2 region; there is no cache maintenance defect on the capture buffer.
- AI activations plus JPEG decoder grayscale/CLAHE buffers occupy `0x24006260..0x24045bdf` in D1 and do not overlap or exceed D1 RAM.
- Generated model input/output buffers are allocated inside the activation arena as expected; firmware obtains their runtime addresses from X-CUBE-AI before preprocessing.

## Local History
- CubeIDE local history contains three `camera_app.c` snapshots from 2026-07-16 at 16:47, 17:31, and 17:56.
- The earliest discovered snapshot already contains the new model scales, flow calibration, and ESP32 integration, so it is not automatically the Wednesday pre-integration state.
- Exact diff between current `camera_app.c` and the 16:47 snapshot shows no inference, preprocessing, threshold, or pump-state differences.
- The only current per-frame additions are the ESP32 periodic self-test poll plus two 5 ms delays. `main.c` also has a second periodic test transmitter and the temporary 10-second PB10 GPIO test.
- Duplicate self-tests are therefore a confirmed runtime regression relative to the earlier snapshot, although they do not by themselves explain deterministic wrong classification of an intact frame.
- PB10 is physically adjacent to DCMI data pins PB8/PB9. Continuous diagnostic UART traffic began with the same integration period in which intermittent image disorder appeared, so removing diagnostic traffic is the narrowest hardware-facing correction. Formal event messages remain infrequent and outside normal frame-by-frame traffic.

## Workflow Robustness
- Decision-window entry previously required three identical low/half/full classes. Alternation between low and half reset the counter and could prevent a known cup from entering the 5-second window.
- Stable cup counting now treats classes 1/2/3 as one cup-presence group while retaining the latest water class for reporting.
- Half and full class-based stops both require three consecutive qualifying frames, so a one-frame boundary crossing cannot stop the pump.

## Final Binary Inspection
- Release ELF links successfully after a forced rebuild of every source and generated model object.
- Binary contains `[APP] mode=PUMP_CTRL`, `STM_FP:%u`, and `STM_DONE:%u`.
- Binary does not contain `STM_HELLO`, confirming diagnostic ESP32 traffic is compiled out.
- Source-only `git diff --check` passes; whitespace warnings come only from line-ending conversion notices and generated files when the whole tree is checked.

## Pump No-Rotation Check
- Current source selects `APP_MODE_PUMP_CTRL`, and the current Release ELF contains `[APP] mode=PUMP_CTRL` and `[APP] pump init ok`.
- Fast pump commands bypass TIM12 PWM and configure PH6 as a push-pull GPIO driven high; the expected log is `duty=999 ph6=1`.
- A voltage measured at a low-side N-MOS drain relative to ground does not prove conduction. When the MOSFET is off, the drain can sit at the supply voltage through the pump winding.
- If the active command log reports `ph6=1` and PH6 measures about 3.3 V relative to MOS source/ground, further diagnosis must use loaded pump-terminal voltage, gate-source voltage, drain-source voltage, and supply current.

## ESP32 Main-Flow Reporting
- Stable pre-integration baseline was committed and pushed as `2b995ea` on `origin/codex/xcubeai`.
- USART3 remains PB10 TX / PB11 RX at 115200 8N1.
- A recognized fingerprint is latched and reported once as `STM_FP:<id>\r\n`; an anonymous completed session reports `STM_FP:0\r\n`.
- Every session with nonzero pump-on time reports `STM_DONE:<ml>\r\n` once, using the 400 ml / 18.058 s calibration.
- Normal, manual, and fault stop paths share the same report function.
- Flashing the integration build did not start because ST-Link measured only 1.15 V and could not read the core ID; the target was not erased or programmed.

## Pre-Dispense Fault Announcement
- `ASR_SPEAK_ABORTED` is shared by abnormal and cup-loss faults, so hearing "abnormal stop" does not prove the triggering class was 4.
- Abnormal accumulation is now limited to actual automatic/voice dispensing states.
- During the 5-second decision window and 1-second voice delay, no stop/fault announcement is issued.
- At the end of either pre-dispense delay, dispensing starts only after three stable cup frames. Stable non-cup/abnormal input cancels the pending session silently; unstable input keeps waiting.
- Cup-loss and abnormal checks remain active after the pump starts.
- The 2026-07-17 log proved the immediate announcement was specifically `wait_cmd -> fault reason=decode_err`, not an AI abnormal-class decision.
- `decomp=6` is TinyJPEG `JDR_FMT1`, indicating malformed or corrupted JPEG data despite a valid SOI header.
- Capture/decode/inference errors now enter the fault/stop path only when `pump_run_active` is set. During the pre-dispense window, bad frames are discarded and camera recovery remains silent.
- The same log contains frequent JPEG corruption, repeated camera recovery, fingerprint UART failure, and an unexplained MCU reboot. The recovery function contains no system-reset call, making shared-power instability or external reset the leading hardware cause.

## AI Visual Viewer Lag
- User switched the source back to `APP_MODE_AI_VISUAL`; ESP32 diagnostic traffic remains disabled and DCMI PCLK remains RISING.
- Screenshot shows a valid frame with ID 40, 9057-byte JPEG, 164 ms board pipeline, and 76 ms inference, but viewer FPS remains 0.0.
- At 921600 baud, a 9057-byte UART frame alone needs about 98 ms before framing overhead; expected end-to-end ceiling is roughly 3.8 FPS with a 164 ms pipeline.
- Viewer currently appends every received byte using a PowerShell `for` loop on the WinForms UI timer. This creates thousands of interpreted operations per frame and can freeze the UI, batch frames, and invalidate the displayed FPS.
- Viewer FPS uses UI update intervals rather than frame-ID deltas, so batched/dropped display frames are undercounted.
- The active system currently exposes COM16. An older viewer process was launched before the optimized script and must be restarted for changes to take effect.
- Viewer decoded the five logit bytes as signed int8, but firmware encodes them as `(logit * 4) + 128`. This made the displayed values misleading; the class selection itself remained correct.
- After the first buffering optimization, the restarted viewer remains responsive but consumes about 1.94 CPU-seconds over 2 wall-clock seconds, effectively saturating one CPU core. More parser/UI work remains.
- COM16 is a CH340 device (`VID_1A86&PID_7523`), which can potentially support a higher baud rate later, but baud should not be raised until parser load and framing stability are fixed.
- UI Automation measured Frame ID advancing from 1815 to 1829 in 3 seconds, about 4.7 received FPS, while the label stayed at 0.0. The board is not stalled; the FPS label implementation is faulty.
- Live pipeline values vary around 159-213 ms with stable 76 ms inference, consistent with normal board execution rather than a camera deadlock.
- Windows PowerShell 5.1 runs on a .NET Framework build without `Environment.TickCount64`; the property evaluated to null, so `LastFrameTick` never became usable and FPS remained 0.0.
- After viewer fixes, live FPS reports about 3.2 and Frame ID advances normally, but pipeline increased to about 221 ms while inference stayed 76 ms. The precise CLAHE interpolation added MCU preprocessing cost.
- CLAHE interpolation lookup tables reduced the flashed board pipeline from about 221 ms back to 168 ms while preserving OpenCV-aligned output; inference remains 76-77 ms.
- After flashing, the viewer receives about 3.8-4.8 FPS with valid JPEG boundaries and continuous frame IDs. Remaining throughput is primarily the 921600-baud JPEG transfer plus board pipeline, not frame loss.
- A 10-second live UI sample collected 20 observations from frame IDs 525 through 589. Every observation was class 1/raw class 1; no 0-4 jumping occurred. Regression varied from 0.227 to 0.368 while final/raw class remained stable.
- Viewer polling at 150 ms reduced CPU usage from about 89% to about 77% of one core without reducing received FPS. Further smoothness is limited by the board pipeline and 921600-baud JPEG transfer.
