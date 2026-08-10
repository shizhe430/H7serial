# Progress

## 2026-08-10

- Resumed from completed new-cup import, training, QAT export, X-CUBE-AI generation, build, flash, and input-alignment diagnostics.
- Confirmed the board is currently running temporary gray-input diagnostic firmware rather than the final JPEG visual firmware.
- Started tracing the generated model output and board-vs-ONNX inference mismatch before final deployment validation.
- Reproduced the mismatch in Cube.AI host mode for both pure-INT8 and ST-compatible QDQ models; the C-model reports class 4 for the exact input that ONNX reports as class 1.
- Verified the float model through Cube.AI host mode: class 1 and 100% classifier agreement with ONNX.
- Added a reproducible multi-domain static PTQ script using only training samples; adjusted sampling for domains with missing or limited classes.
- Identified the QAT cleanup regression: source node names collided with ORT-generated `node_NNNN` names, preventing the same graph optimization used by the old deployable model. Updated the exporter to use a collision-resistant source prefix; no retraining is required.
- Exported and validated the v4c live-empty candidate. Cube.AI host output matches ONNX exactly while retaining the compact pure-INT8 resource profile.
- Generated the seven Cube.AI `waterlevel` sources, backed up the v3 generated files, replaced the project model, and archived the v4c ONNX under `models/waterlevel`.
- Built Release firmware and created `Release/H7serial_st_cupmix_v4c_live_empty_ai_visual.hex` (SHA-256 `BE3B2822D16DA8387F87137F4D3FF687403682693967254E6468704E8C46B5D8`).
- Flashed, verified, and reset the STM32H743 through ST-LINK. Captured normal JPEG visual frames from COM28 and visually confirmed the empty cup and correct ROI geometry.
- Long empty-cup acceptance capture: 100 frames, final class 1 = 90, class 2 = 7, class 3 = 3; mean inference 78.81 ms and mean pipeline 173.78 ms. Continue with physical low/half/full checks before final production sign-off.
- Calibrated the low/half boundary against the user's current physical low-water scene. Changed both `WATERLEVEL_LOW_HALF_SPLIT` and `WATERLEVEL_RAW_HALF_KEEP_REG` from `0.395` to `0.470`.
- Built, flashed, verified, and reset `Release/H7serial_st_cupmix_v4c_lowhalf0470_ai_visual.hex` (SHA-256 `E957539E3974FB1C564928CEBD233272519CFAD397E9DD0F15EB5B9BCE612498`).
- Sequence acceptance after the threshold change: at `0.470`, 82/100 low and 18/100 half with a maximum half-cup run of two frames. The existing three-frame production decision therefore rejects every observed false half-cup run.
- Added the production integration record and retained AI visual mode as the safe validated rollback configuration. The production build changes only `APP_MODE` after completing independent inference acceptance.
- Audited the production camera path and added pump-control mode to both SVGA wide-view setup conditions; this prevents a silent camera-domain change when moving from visual validation to the working state machine.
