# Progress

## 2026-08-02

- Started a surgical post-CubeMX regression repair and camera-1 face AI integration.
- Read the coding and persistent-planning guidelines.
- Ran session catch-up; no prior unsynced plan was found.
- Created `task_plan.md`, `findings.md`, and this progress log.
- Confirmed CubeMX generated `facedet`, `faceid`, `facedet_data.bin`, and `faceid_data.bin`.
- Confirmed the current `.ioc` contains external SDRAM/model entries, but its clock tree is currently 153.6 MHz SYSCLK / 76.8 MHz HCLK.
- Confirmed generated model APIs accept caller-supplied activation and weight addresses.
- Confirmed `facedet` and `faceid` currently overlap at `0x90000000` and are absent from `Release/X-CUBE-AI/App/subdir.mk`.
- Confirmed CubeMX generated FMC/QSPI MSP code selects PLL2 without populating `RCC_PeriphCLKInitTypeDef.PLL2`.

## Hardware Test

- Recovered ST-LINK connection at 3.26 V and programmed the temporary QSPI updater.
- SDRAM and QSPI identification/SFDP checks passed on the new core board.
- Wrote `facedet_data.bin` at offset `0x00000000` and `faceid_data.bin` at offset `0x00400000`; both device-side CRC checks passed.
- Restored the default application mode to `APP_MODE_FACE_AI_DIAG` for runtime validation.
- Switched shared PLL2 to 200 MHz, FMC SDCLK to 100 MHz, and QSPI back to approximately 100 MHz with a divide-by-2 prescaler.
- Built Release successfully with the 100 MHz configuration.
- Flashed and verified the Release image on ST-LINK; the board reported `kernel=200000000 sdclk=100000000`, `cache_rt ok`, and `SDRAM test ok` for 32 MB.
- Confirmed QSPI ID/SFDP, memory mapping, weight loading, and camera-1 initialization still pass after the frequency change.
- Downloaded and generated STM32H7 code for ST YuNet INT8 and OpenCV SFace INT8; both pass ST Edge AI analysis and generation.
- Confirmed YuNet tensor ordering and quantization against both the ONNX graph and the ST generation report before implementing postprocessing.
- Replaced the camera-1 diagnostic models with YuNet and SFace, added YuNet INT8 decoding, five-landmark similarity alignment, and SFace embedding diagnostics; Release builds successfully.
- Programmed YuNet at QSPI offset `0x00000000` and SFace at `0x00400000`; both uploads passed device-side CRC verification.
- First runtime reached YuNet execution but raised an unaligned UsageFault because SDRAM had no Normal-memory MPU region; prepared a 32 MB cacheable Normal-memory region at `0xC0000000`.
- Enabled MPU region 1 as 32 MB cacheable Normal memory. SDRAM 100 MHz diagnostics remained green and YuNet completed in 848-851 ms.
- Benchmarked SFace once with a diagnostic-only center alignment because no face was present in the camera view: SFace completed in 4,426 ms and the full pipeline in 5,328 ms. Removed the fallback afterward so production diagnostics skip recognition when no face is detected.
- Rebuilt `APP_MODE_PUMP_CTRL` successfully without flashing, then restored, rebuilt, and flashed `APP_MODE_FACE_AI_DIAG`.
- Final hardware boot reconfirmed SDRAM 100 MHz, QSPI mapping, both weight loads, camera 1, JPEG decode, and YuNet at 848 ms. The current scene had no face (`score=0.157`), so the final firmware correctly skipped SFace.
## 2026-08-02

- Replaced stale CubeMX `facedet/faceid` metadata with YuNet/SFace metadata and project-local ONNX paths.
- Updated `.mxproject` to list YuNet/SFace reports and generated source files.
- Synchronized cached IOC clock values with the hardware-validated 480/240 MHz system and 100 MHz SDRAM/QSPI configuration.
- Added explicit IOC USART1 921600 and QSPI PLL2/prescaler settings.
- Added and passed `tools/check_cubemx_regressions.ps1`.
- Rebuilt the Release configuration successfully. The final image links YuNet/SFace only and reports `text=144296`, `data=71792`, `bss=98480`.
- Verified the IOC has no duplicate keys, both project-local ONNX paths exist, and the link map contains no `facedet/faceid` objects.
- On 2026-08-03, ST-LINK was detected again and `Release/H7serial.elf` was programmed and verified successfully. The board reported SDRAM 100 MHz/cache test OK, QSPI memory-mapped OK, both YuNet/SFace weight copies verified OK, and camera 1 JPEG capture/decode OK.
- The live face attempt produced YuNet scores from `0.002` to `0.359`, all below the `0.55` acceptance threshold, so SFace was correctly skipped. No valid real-face inference has been recorded yet.
- Added isolated `APP_MODE_FACE_AI_VISUAL` and the `FAV1`/`FAE1` binary protocol. Each frame carries camera ID, JPEG, YuNet score and box, detection time, SFace time, total time, and status flags; the original AIV1 water-level protocol is unchanged.
- Replaced the temporary face text monitor with `tools/face_ai_viewer.ps1` and `tools/start_face_ai_viewer.cmd`. The viewer was tested on COM28 at 921600 baud and displayed more than 60 live camera-1 JPEG frames with the candidate box and `NO FACE` result. A prior frame also completed SFace (`id_ms=4421`, `emb_norm=9.022`).
- Added a fixed `ID=1` enrollment database. The complete 128-float SFace embedding is saved as a CRC-protected record at QSPI offset `0x01000000`, loaded into SDRAM at `0xC1800000`, and compared with cosine similarity.
- The visual viewer now provides `Enroll ID=1`, `Clear ID`, reference status, similarity, and matched-ID fields. Enrollment is pending until the next valid detected face, so a background frame cannot be registered accidentally.
- The first enrollment completed successfully, but the initial `0.60` cosine threshold was stricter than OpenCV's documented SFace LFW threshold (`0.363`) and the text log did not expose recognition decisions. Changed the threshold to `0.363`, added per-frame identity status/similarity logging, rebuilt Release, and programmed/verified the MCU without touching QSPI weights or enrollment data.
- The follow-up hardware run produced 9 confirmed `status=0 ref=1 match=1 id=1` results from 26 SFace runs, with observed cosine similarities from `0.552` to `0.776`. The same run had no serial errors, no repeated boot, and only four explicit viewer-open events across separate sessions.
