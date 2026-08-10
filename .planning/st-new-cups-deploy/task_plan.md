# ST New-Cup Model Training and Deployment

## Goal

Integrate the three additional cup types into the existing multi-domain water-level dataset, train and validate an ST-aligned model, deploy it to the connected STM32H743 board, and verify the live empty-cup result.

## Phases

- [x] Import the new cup JPEG data and complete multi-domain train/validation splits.
- [x] Train float/QAT models and export a pure-INT8 ONNX model for X-CUBE-AI.
- [x] Generate X-CUBE-AI sources, build firmware, and compare board preprocessing with PC preprocessing.
- [x] Resolve the current board-vs-ONNX inference output mismatch.
- [x] Restore formal JPEG visual firmware, rebuild, and flash the board.
- [x] Record the final visual acceptance result, low/half boundary calibration, and production integration gates.

## Success Criteria

- Board and PC ONNX produce the same raw class for the same exact 224x224 INT8 input.
- Formal visual firmware uses ROI `(50,-1)`, fill `180`, CLAHE, center crop, and circular mask.
- Connected board reports a stable empty-cup class for the cup currently in view.
- Final model, generated X-CUBE-AI sources, firmware image, dataset split manifest, and metrics are retained.

## Constraints

- Preserve unrelated dirty working-tree changes.
- Keep class IDs: 0=no cup, 1=low, 2=half, 3=full, 4=abnormal.
- Keep the common paper cup dominant; new cup samples are lower-weight generalization data.
- Do not treat diagnostic gray-input streaming firmware as the final deliverable.

## Errors Encountered

| Error | Attempt | Resolution |
|---|---:|---|
| Board classified the captured empty-cup input as class 4 while PC ONNX classified the exact saved input as class 1 | 1 | In progress: inspect generated model output buffers and validate generated runtime against ONNX |
| Initial PTQ sampler required 45 class-0 samples from the new ST train set, but only 24 exist and the cup-extension domain has no class 0 | 1 | Use unique-sample round-robin allocation over available classes and redistribute unavailable class quota |
| QAT cleanup retained an unoptimized 768 KB QDQ graph and Cube.AI disagreed with ONNX | 1 | Renamed exported source nodes from `node_NNNN` to `qat_export_source_NNNN`; the folded 282 KB pure-INT8 graph now matches the Cube.AI host model exactly |
| First v4c live capture included a scene transition and was mostly class 3 | 1 | Inspected the transmitted JPEGs and repeated the test after the empty-cup scene settled; ROI metadata is `(50,-1)` with fill 180 |

## Final Acceptance

- The user confirmed the v4c model accuracy is sufficient for integration.
- The final low-water calibration produced 82 low and 18 half decisions over 100 frames, with a maximum false-half run of two; the production three-frame rule rejects every observed false run.
- The validated rollback image remains the AI visual firmware. Production pump-control integration must follow `WATERLEVEL_ST_V4C_PRODUCTION_INTEGRATION.md` and pass its no-pump and live-pump gates.
