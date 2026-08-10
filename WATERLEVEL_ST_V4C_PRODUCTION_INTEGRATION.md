# Water-Level v4c Production Integration

## Validated Deployment Unit

- Model: `models/waterlevel/waterlevel_st_roi_x50_y-1_fill180_clahe_r100_v4c_cupmix_live_empty_pure_int8.onnx`
- Visual rollback firmware: `Release/H7serial_st_cupmix_v4c_lowhalf0470_ai_visual.hex`
- Classes: `0=no cup, 1=low/empty, 2=half, 3=full, 4=abnormal`
- Low/half boundary: `0.470`
- Half/full boundary: `0.665`
- Production confirmation: three consecutive target frames
- Regression smoothing: five-frame window

The model, generated Cube.AI files, camera view, preprocessing, output quantization, thresholds, and state-machine rules form one deployment unit. Do not update one item without validating the complete unit.

## Fixed Camera And Input Contract

```text
OV2640 SVGA sensor view -> DSP 320x240 JPEG
-> RGB decode
-> grayscale: (77R + 150G + 29B + 128) >> 8
-> aligned[y][x] = source[y - 1][x + 50]
-> out-of-range fill = 180
-> CLAHE, 8x8 grid, clip limit 2
-> pad to 320x320
-> center crop 224x224
-> circular mask, center (112,112), radius 100
-> INT8 input, gray - 128
```

Do not replace this path with direct resize, a display-circle crop, RGB input, a second normalization, or a different JPEG sensor window.

## Cube.AI Contract

- Replace all seven generated `X-CUBE-AI/App/waterlevel*` C and header files together.
- Classification output: scale `0.103576131`, zero point `6`.
- Regression output: scale `0.003226662`, zero point `-128`.
- Expected resources: 182,008 bytes weights, 138,352 bytes activations, 16,479,543 MACC.
- Expected board inference time: approximately 78.8 ms.

Never mix `waterlevel.c`, `waterlevel_data.c`, or `waterlevel_data_params.c` from different generations.

## Production Build

The repository remains in `APP_MODE_AI_VISUAL` because that is the independently verified rollback configuration. For the production build:

1. Set `APP_MODE` to `APP_MODE_PUMP_CTRL` in `Core/Inc/camera_app.h`, or pass the equivalent build definition.
2. Keep ROI `(50,-1)`, fill 180, CLAHE, crop, mask, input quantization, and output quantization unchanged.
3. Confirm `APP_MODE_PUMP_CTRL` remains included in both `OV2640_SetWideView320x240()` initialization and camera-recovery conditions.
4. Keep `WATER_DATASET_LIGHT_CYCLE_ENABLE=0`.
5. Keep input dump, dump-only inference, gray diagnostic streaming, model self-test, and forced-pump test disabled.
6. Build Release and confirm RAM and Flash usage remain within the validated H743 placement.

The committed Release build list includes `camera_light.c` and `environment_sensors.c`; removing either entry causes unresolved references before the water-level firmware can link.

Do not commit a production-mode switch until the complete integrated application is ready, because pump-control mode also enables the project's unrelated face, voice, cloud, and safety flows.

## Acceptance Order

1. In AI visual mode, verify no cup, low, half, full, and abnormal sequences.
2. Switch to pump-control mode with pump power disconnected and verify state transitions and GPIO commands.
3. Connect the pump and test start, low-to-half stop, low-to-full stop, cup removal, obstruction, low-confidence timeout, and maximum-duration timeout separately.
4. Confirm isolated single-frame and two-frame errors never change the pump state.
5. Confirm UART or display traffic cannot block inference long enough to bypass a stop condition.
6. Retain the visual rollback HEX and this model's seven generated files before testing another candidate.

## Competition-Site Update

Keep the camera geometry and preprocessing fixed. Capture original 320x240 JPEGs without overlays into five class directories, using separate temporal sessions. Seal at least one later session per class for external testing and do not randomly split adjacent frames.

Use the model-repository tools:

```powershell
python .\prepare_st_field_adaptation.py `
  --source C:\field_data\competition_site `
  --tag competition_site `
  --field-train-weight 0.15

.\run_st_field_qat.ps1 `
  -TrainRoot processed\field_adapt_competition_site\qat_train `
  -ValRoot processed\field_adapt_competition_site\qat_val `
  -Tag competition_site `
  -RegressionRoots @(
    'processed\st_field_competition_site\external_test',
    'processed\st_new_roi_x50_y-1_fill180_clahe_r100_v2\external_test',
    'processed\st_new_roi_x50_y-1_fill180_clahe_r100_v2_cup_extension\external_test'
  )
```

Inspect the generated preprocessing contact sheet before training. Do not replace the board model until field external testing, retained-domain regression, Cube.AI/ONNX agreement, memory analysis, exact-input replay, and live three-frame acceptance all pass.
