# Progress

## 2026-08-04

- Started OpenMV comparison and face-latency optimization audit.
- Confirmed current model formats, memory locations, SDRAM clock, and measured hardware baseline from existing project evidence.
- Confirmed the working tree contains generated build artifacts; source edits will be kept separate from those artifacts.
- Confirmed Release AI sources use `-O3`, Cortex-M7 hard-float, and X-CUBE-AI time optimization.
- Confirmed product mode already performs one face attempt per stable-cup event and skips SFace when no face is detected.
- Confirmed SFace dominates latency and is only partially quantized; selected recognizer-only replacement as the lowest-risk material optimization.
- GitHub access remained unavailable, so the OpenMV source clone could not be fetched during this audit.
- User selected direct migration to the OpenMV-style Haar/LBP combination.
- Confirmed the existing OV2640 driver supports JPEG/RGB565 but has no grayscale output path; initial OpenMV backend will preserve JPEG capture and convert after decode.
- Confirmed Gitee and GitCode mirror access also failed; continuing with raw/CDN retrieval and local OpenCV cascade investigation.
- User selected isolated testing before any production replacement.
- Added `APP_MODE_OPENMV_HOST_DIAG`: camera 1 streams FAV1 JPEG frames while skipping YuNet/SFace.
- Default remains `APP_MODE_PUMP_CTRL`; existing production models and QSPI enrollment are untouched.
- Diagnostic compilation exposed a pre-existing pump-only conditional-compilation leak; wrapped the affected pump functions without changing their contents.
- Added the isolated PC Haar/LBP viewer with FAV1 parsing, box overlay, timings, five-frame ID=1 enrollment, matching statistics, and a PC-local database.
- Production Release build passed (`text=1355720 data=90936 bss=361568`).
- Separate `OPENMV_HOST_DIAG` ELF/HEX linked successfully (`text=45260 data=96 bss=91296`).
- CubeMX regression check and Python syntax/LBP descriptor checks passed.
- Completed the physical host validation: 42/42 same-person accepts and 0/162 different-person accepts at threshold 0.55.
- Advanced to a board-only diagnostic port; production YuNet/SFace remains unchanged.
- Downloaded and audited the official OpenMV source archive via `codeload.github.com` after git/raw GitHub access failed.
- Confirmed the exact OpenMV descriptor differs from the first host harness (7x7 weighted versus 8x8 normalized); this is recorded as a required board-validation constraint.
- Restored and verified the production `PUMP_CTRL` firmware after the host test.
