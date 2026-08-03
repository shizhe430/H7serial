# CubeMX Regression and Dual-Camera Face AI

## Goal

Set the validated external-memory clock, then replace the slow `buffalo_sc` face models with a lightweight YuNet + SFace candidate pair while preserving the camera-0 water-level/pump workflow.

## Phases

- [x] Baseline the generated project and identify CubeMX regressions.
- [x] Restore clock, UART, QSPI/SDRAM, and camera initialization settings surgically.
- [x] Verify the project builds without changing the existing pump-control behavior.
- [x] Add coordinated face detection/recognition test flow on camera 1.
- [x] Set FMC-SDRAM to 100 MHz and verify the complete external-memory startup path on hardware.
- [x] Download/analyze the ST YuNet INT8 and OpenCV SFace INT8 models for STM32H7.
- [x] Replace the camera-1 face diagnostic preprocessing/postprocessing for the new model interfaces.
- [x] Add an isolated FAV1 JPEG/face-box protocol and a visual camera-1 face diagnostic mode.
- [x] Add a dedicated Windows visual viewer without changing the existing water-level AIV1 viewer.
- [x] Add fixed-ID=1 enrollment, QSPI persistence, and cosine-similarity matching.
- [x] Persist YuNet/SFace and validated clock/UART settings in CubeMX metadata and add a post-generation regression check.
- [ ] Verify a real detected face through five-point alignment on camera 1; model runtime and the original pump build are already verified.

## Success Criteria

- Existing `PUMP_CTRL` camera path remains unchanged except for required generated-code repairs.
- UART baud/format and system clocks match the last known stable configuration.
- YuNet and SFace generated sources are represented in both Release and Debug build configurations.
- Camera 1 can be selected, captured, and passed through the detector and recognizer without using camera 0's water-level buffers.
- A full Release build completes, or any blocker is recorded with its exact error.

## Constraints

- Do not overwrite or revert unrelated user changes.
- Do not allocate the face AI activation buffer over existing LCD/camera buffers.
- Keep face AI validation isolated from the normal pump workflow until the second-camera test passes.
- FAV1 header is 48 bytes, followed by JPEG and a 4-byte FAE1 tail; it is independent of AIV1.
- Enrollment record uses QSPI offset `0x01000000` and is loaded into the SDRAM face database at `0xC1800000`.
- SFace matching uses OpenCV's documented LFW cosine-similarity threshold of `0.363` and exposes `Enroll ID=1` and `Clear ID` in the face viewer.

## Current Decisions

- Keep `APP_MODE_PUMP_CTRL` unchanged while validating face AI.
- Add a separate `APP_MODE_FACE_AI_DIAG` for camera 1.
- Reuse one SDRAM activation pool sequentially for YuNet and SFace.
- Place YuNet weights at `0x90000000` and SFace weights at `0x90400000`.
- W25Q256 requires four-byte address mode; mapped reads and programming use 32-bit addresses.

## Errors Encountered

| Error | Attempt | Resolution |
|---|---:|---|
| CubeMX reset HSE, PLL range, UART baud, DCMI polarity, DMA priority, GPIO analog switch and IRQ declarations | 1 | Pending surgical source and `.ioc` repair |
| FMC/QSPI selected PLL2 but generated MSP code supplied no PLL2 parameters | 1 | Pending explicit PLL2 configuration in MSP clock setup |
| Release makefiles omit generated `facedet` and `faceid` sources | 1 | Pending makefile update |
| Face diagnostic stopped after first JPEG | 1 | External weights were not programmed; added a temporary updater and programmed both weights with device-side CRC checks |
| YuNet raised `CFSR=0x01000000` at model entry | 1 | Added a 32 MB cacheable Normal-memory MPU region for external SDRAM; YuNet and SFace then ran successfully |
| `.ioc` and `.mxproject` still referenced `facedet/faceid` after the runtime had moved to YuNet/SFace | 1 | Replaced the CubeMX model metadata with project-local YuNet/SFace paths and added `tools/check_cubemx_regressions.ps1` |
