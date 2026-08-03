# Face model artifacts

The camera-1 diagnostic uses these two STM32H7-compatible models:

- `yunetn_320_qdq_int8.onnx`: ST STM32 AI Model Zoo YuNet INT8 detector.
- `face_recognition_sface_2021dec_int8.onnx`: OpenCV Zoo SFace INT8 recognizer.
- `yunet_data.bin`: ST Edge AI 10.2 `-O time --binary` weights, programmed at QSPI offset `0x00000000`.
- `sface_data.bin`: ST Edge AI 10.2 `-O time --binary` weights, programmed at QSPI offset `0x00400000`.

Sources:

- https://github.com/STMicroelectronics/stm32ai-modelzoo/tree/main/face_detection/yunet
- https://github.com/opencv/opencv_zoo/tree/main/models/face_recognition_sface

Weight checksums (SHA-256):

```text
yunet_data.bin  5AB0E0159F50920BEFDC50CBCCEFAAE6B9BC7E5F5BA8D37F0C285CFE7D06A8FF
sface_data.bin  F31BACCD6C2A459F9C6CD243C76B7DEC5322A7FFBF7B2CD5DCD48644486E40BB
```

Use `tools/qspi_upload.ps1` while the firmware is built with
`APP_MODE_QSPI_PROGRAM`. The default runtime remains
`APP_MODE_FACE_AI_DIAG` during camera-1 validation.

## CubeMX persistence

`H7serial.ioc` uses these project-local ONNX paths and the model names `yunet` and `sface`.
After every CubeMX **Generate Code**, run:

```powershell
powershell -ExecutionPolicy Bypass -File tools/check_cubemx_regressions.ps1
```

Do not accept a generated project that fails this check. It checks the face model list,
480/240 MHz system clocks, 100 MHz SDRAM, the SDRAM MPU region, 100 MHz QSPI,
USART1 at 921600 baud, and Debug/Release model build lists.
