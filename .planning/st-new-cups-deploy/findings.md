# Findings

## Existing Artifacts

- Pure-INT8 model: `models/waterlevel/waterlevel_st_roi_x50_y-1_fill180_clahe_r100_v2_pure_int8.onnx`.
- Formal firmware image: `Release/H7serial_st_new_roi_x50_y-1_pure_int8.hex`.
- Board input snapshot: `tmp/board_empty_input_snapshot.bin`.
- Reported validation: new ST 99.24%, new ST external 95.95%, old ST 98.35%, P4 99.16%, new-cup validation 70.24%.

## Confirmed Preprocessing

`320x240 JPEG -> RGB -> grayscale -> aligned[y][x]=source[y-1][x+50] -> fill 180 -> CLAHE -> pad 320x320 -> center crop 224x224 -> circle mask r=100 -> int8(gray-128)`.

## Current Blocker

- Board snapshot and PC reconstruction differ by only 1.71 mean absolute gray levels; 93.1% of pixels differ by at most 5.
- PC pure-INT8 ONNX logits for the exact board snapshot are `[-8.60, 5.61, 2.71, -3.55, -6.45]`, raw class 1.
- Board firmware reported raw class 4 with confidence 0.999.
- The remaining mismatch is downstream of preprocessing: generated X-CUBE-AI model/runtime or output-buffer interpretation.

## Cube.AI Host Isolation

- Both the v3 pure-INT8 graph and ST-compatible QDQ graph classify the corrected empty-cup INT8 input as class 4 in the Cube.AI 10.2 host C-model, while ONNX Runtime classifies it as class 1.
- Enabling or disabling the ST ONNX optimizer does not change the mismatch.
- The v3 float ONNX and Cube.AI host C-model agree to 100% classification accuracy and approximately 3e-6 RMSE on the same input. The incompatibility is isolated to the exported QAT quantization graph.
- Float deployment needs 522,560 B of activations, so the next route is static PTQ from the verified float model using 500 unique multi-domain training samples.
- Standard per-tensor PTQ is fully compatible with the Cube.AI host C-model and reduces resources to 182,008 B weights and 128,872 B activations, but both ONNX and C-model classify the captured empty cup as class 0. It is not accurate enough to deploy; per-channel weight PTQ is the next candidate.
- The new QAT export renamed every source node to `node_NNNN`; ORT's QDQ optimizer creates nodes using the same naming scheme, collided at `node_0085`, and the cleanup fallback retained the unoptimized 768 KB graph (96 activation Q nodes). The old successful ST model is a 283 KB optimized/folded graph with 49 activation Q nodes. Use a `qat_export_source_NNNN` prefix and rerun export/folding from the saved QAT checkpoint.

## Final v4c Deployment Candidate

- Model: `models/waterlevel/waterlevel_st_roi_x50_y-1_fill180_clahe_r100_v4c_cupmix_live_empty_pure_int8.onnx`.
- Renaming the source nodes before QDQ cleanup produced a 282,298-byte pure-INT8 ONNX graph. Cube.AI host and ONNX outputs agree exactly on the validation inputs (100% class agreement, RMSE 0).
- Offline accuracy: live empty temporal validation 95.00%, new ST validation 95.42%, new ST external 95.38%, new-cup validation 88.00%, new-cup external 93.60%, legacy ST 96.30%, P4 validation 94.43%, and P4 external 93.52%.
- Cube.AI output quantization remains logits `scale=0.103576131, zero_point=6` and regression `scale=0.003226662, zero_point=-128`, matching `camera_app.c`.
- The generated model uses about 182,008 bytes of weights and 138,352 bytes of activations. Release firmware links successfully with `text=294,036`, `data=19,224`, and `bss=406,156` bytes.
- The board was flashed and verified through ST-LINK `0671FF535251888281013738`. Normal AIV1 JPEG frames report ROI `(50,-1)` and fill 180; inference is approximately 78.8 ms.
- A settled empty-cup check produced 90/100 final class 1, 7/100 class 2, and 3/100 class 3. Raw results were 82/100 class 1, 13/100 class 2, and 5/100 class 3. This is suitable for continued visual and pump-control integration, but low/half/full live acceptance is still required before declaring the firmware final.
- Pump-control target detection already requires three consecutive target-class frames and uses a five-frame regression window, so isolated display-level class changes do not immediately stop the pump.

## Low/Half Boundary Calibration

- The original board threshold `0.395` classified the current physical low-water scene as half-cup for 91/100 valid frames, with a maximum consecutive half-cup run of 28 frames.
- The deployed low/half split and raw-half keep threshold were raised together to `0.470`. On a fresh 100-frame sequence from the same scene, the reconstructed result was low 82, half 18, full 0; the maximum consecutive half-cup run was only 2 frames.
- The production decision layer requires three consecutive target frames, so the remaining isolated half-cup frames cannot trigger an early half-cup stop.
- Raising the boundary further to `0.485` would reduce this live sequence to four half-cup frames, but offline half-cup recall across the retained validation domains falls from 85.71% at `0.395` to 45.38%. `0.470` is the safety-oriented compromise and preserves a visibly higher water level before entering the half-cup state.

## Production Integration Audit

- The shared water-input decoder already applies ROI `(50,-1)` in pump-control mode.
- The wide-view camera setup was initially guarded only for AI visual and ROI-calibration modes. Pump-control mode would therefore have reverted to a different sensor view after switching modes or recovering the camera.
- Added `APP_MODE_PUMP_CTRL` to both `OV2640_SetWideView320x240()` conditions so production initialization and camera recovery preserve the training camera contract.
