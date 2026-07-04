# H7serial — STM32H743IIT6 OV2640 图像采集工程

---

## 2026-06-23 Working Update

- Git rollback baseline created: `47ed3b2` (`baseline: import current H7serial project and blueprint`)
- Rules file added: `rules/H7serial.rules.md`
- Current status update:
  - `D6 = PB8` and `D7 = PB9` hardware mapping has been corrected in CubeMX/H7 code.
  - The remaining bad-JPEG symptom was finally traced to `D5 = PD3` hardware non-contact.
  - Software proof:
    - with `PD3` internal pull-up / no-pull, captured header showed `FF F8 FF E0 20 30 6A 66`
    - with `PD3` internal pull-down, captured header flipped to `DF D8 DF C0 00 10 4A 46`
    - this proved `D5/PD3` was floating rather than being validly driven
  - After physically fixing the `OV2640 D5 -> STM32 PD3` connection, the H7 capture log became:
    - `head=FF D8 FF E0 00 10`
    - `tail=.. .. FF D9`
    - `soi=1 eoi=1 off=0 len≈3110`
  - This confirms:
    - DCMI data bus is now valid
    - OV2640 JPEG output is valid
    - reverse JPEG frame extraction is valid
    - the current remaining step is to disable diagnostic text mode and validate real XCAM display
- Current analysis:
  - The issue is not only `PCLK` instability.
  - The H7 project had drifted away from the verified U5 reference in several key places:
    - JPEG path did not apply `YUV422 -> JPEG` in the verified order.
    - Output size programming only wrote part of the DSP resize registers.
    - Color bar test used the wrong `COM7` bit.
    - XCAM path used continuous timed capture plus debug UART noise, which is fragile.
    - `115200` baud is too low for practical `324x224` JPEG preview in XCAM.
- Current fix direction already applied in code:
  - Align OV2640 JPEG configuration closer to the verified open-source flow.
  - Use proper DSP output-size registers for `324x224`.
  - Switch XCAM runtime path back to single-frame `SNAPSHOT`.
  - Reduce UART debug contamination of the JPEG stream.
  - Raise USART1 baud rate to `921600`.
  - Re-enable `DCMI_IT_FRAME` before each snapshot start, otherwise `g_frame_done` never releases and XCAM stays black with only a few received bytes.
- New confirmed finding after XCAM retest:
  - Current black screen is no longer a "no data" issue. `XCAM V1.3` shows sustained RX throughput around `38~40 KB/s`, but `FrameSize/Width/Height` stay `0`.
  - The immediate root cause in the H7 code was a regression in JPEG frame extraction:
    - The blueprint-required behavior is to search from the end of the DMA buffer and pick the last complete `FFD8 ... FFD9` frame.
    - `jpeg_stream.c` had been changed to search forward for the first `SOI -> EOI`, which can select stale bytes or a cross-frame fragment when the buffer contains residual data.
    - This exactly matches the symptom "XCAM keeps receiving bytes but never recognizes a valid JPEG frame".
  - Corrective action applied:
    - `JPEG_Stream_FindFrame()` restored to reverse-search `EOI -> SOI`.
    - `camera_app.c` now rejects any candidate span whose first two bytes are not `FFD8` or last two bytes are not `FFD9`.
  - Remaining comparison gap with the verified U5 project:
    - `cursor/CoreU5` succeeds with `320x240`, not `324x224`.
  - Current validation build direction:
    - H7 output size has been temporarily aligned to `320x240` to match the proven U5 XCAM path exactly.
    - Once XCAM can display valid frames on H7, resize will be moved back toward the user target `324x224` in a controlled follow-up step.
  - New H7/HAL timing finding:
    - In STM32H7 HAL DCMI, `DCMI_IT_FRAME` is enabled by the HAL only after the DMA transfer-complete path reaches the final transfer count.
    - That means `FRAME` is not a safe "DMA data is already fully in RAM" signal if user code manually enables it too early and immediately calls `HAL_DCMI_Stop()`.
    - The H7 app has now been adjusted to wait for a short DMA/FIFO settle window after `FrameEvent` before stopping DCMI, reducing the chance of truncating the JPEG tail.
  - Current diagnostic mode:
    - `CAMERA_JPEG_DIAG` is temporarily enabled.
    - Firmware now sends one text diagnostic line per captured frame instead of the JPEG payload.
    - Diagnostic mode must also print failure-path tags such as `probe fail`, `init fail`, `start`, `timeout`, `noframe`, `dcmierr`, because "no serial output" is otherwise ambiguous.
    - Required next observation is the actual `[JPEG]` line showing:
      - offset
      - length
      - DMA `NDTR`
      - DCMI `SR`
      - total `SOI/EOI` marker counts
      - first 6 bytes at the chosen frame start
      - last 4 bytes at the chosen frame end
  - Diagnostic conclusion from `[JPEG]` logs:
    - The stream usually contains a syntactically complete JPEG span (`SOI=1`, `EOI=1`, `off=0`, `len≈3400`), so frame extraction is no longer the primary problem.
    - But the payload bytes are corrupted in a very specific way:
      - expected JPEG-like head should resemble `FF D8 FF E0 ...`
      - many captured heads instead look like `FF D8 EB D0 D3 CC` or `FF D8 E3 E0 E6 D9`
      - many tail-adjacent bytes also cluster near `0xC0/0xE0/0xF0`
    - This pattern strongly suggests DCMI data-bus high bits are stuck high during capture, especially `D6/D7`.
  - New stronger evidence from the `noframe` logs:
      - captured start bytes became `FF F8 FF E0 E0 F0 EA E6`
      - the expected JPEG/JFIF start is typically `FF D8 FF E0 00 10 4A 46`
      - bytewise comparison shows a near-exact `OR 0xE0` pattern on the data bytes:
        - `00 -> E0`
        - `10 -> F0`
        - `4A -> EA`
        - `46 -> E6`
      - this is no longer "random corruption"; it indicates `D5/D6/D7` are being sampled as logic-high.
    - On this H7 board the active data mapping is:
      - `D5 = PD3`
      - `D6 = PE5`
      - `D7 = PE6`
    - Therefore the highest-probability fault focus is now the three-bit group `PD3/PE5/PE6`, not the JPEG state machine and not the DCMI sync polarity.
  - Root cause confirmed by hardware pin audit:
    - The actual board wiring for the upper two camera data bits is:
      - `D6 = PB8`
      - `D7 = PB9`
    - The earlier H7 project had incorrectly mapped them to `PE5/PE6`.
    - After correcting CubeMX to `PB8/PB9`, the software side must still manually restore the blueprint-critical settings that CubeMX overwrote:
      - `PCKPolarity = RISING`
      - `SYSCFG_SWITCH_PA0 = OPEN`
      - `USART1 baud = 921600`
      - `DMA2_Stream3_IRQn priority = 5`
      - `DCMI_IRQHandler()` declaration/implementation must exist after CubeMX regeneration; losing it breaks the frame-done callback path.
    - `CAMERA_JPEG_DIAG` should normally stay disabled after the pin fix so the firmware sends real JPEG data to `XCAM`.
    - Current software mitigation under test:
      - remove internal pull-ups on DCMI data pins in `dcmi.c`, because the captured-byte pattern is consistent with upper data bits being biased high.
    - Result after removing pull-ups:
      - corruption pattern stayed essentially unchanged, so the issue is not merely caused by `GPIO_PULLUP` on all DCMI data pins.
    - Current rule after that test:
      - revert DCMI data pins to the blueprint's original `GPIO_PULLUP` configuration.
      - do not keep experimental pulldown settings on `PE5/PE6` as a long-term branch.

## 2026-07-04 Model Replacement Preparation

- Current verified camera baseline:
  - `APP_MODE_XCAM_VIEW` is working again.
  - `OV2640 -> JPEG snapshot -> XCAM` is the current known-good bring-up chain.
  - The current image parameters produce images that the new PC-side model can classify correctly.
- Current verified AI baseline:
  - The correct H7 inference chain is:
    - `OV2640 JPEG snapshot`
    - `TJpgDec decode`
    - training-aligned preprocess
    - `X-CUBE-AI` inference
  - The older direct `RGB565 -> infer` path is **not** the active baseline and must not be restored by accident.
- Current purpose of the next step:
  - replace the model through `CubeMX / X-CUBE-AI`
  - preserve the already verified camera path
  - preserve the training-aligned JPEG decode + preprocess chain

### CubeMX / X-CUBE-AI replacement: previously observed failure modes

1. `APP_MODE` was overwritten
   - Symptom:
     - user intended `APP_MODE_XCAM_VIEW`
     - board booted into `APP_MODE_AI_TEST_IMAGE` or `APP_MODE_AI_INFER`
   - Recovery:
     - re-check `Core/Inc/camera_app.h`
     - confirm:
       - `APP_MODE_XCAM_VIEW`
       - `APP_MODE_AI_INFER`
       - `APP_MODE_AI_TEST_IMAGE`
       - active `APP_MODE` value

2. `DCMI_IRQHandler()` path was lost
   - Symptom:
     - build error: `hdcmi undeclared`
     - or runtime capture never completes
   - Recovery:
     - re-check:
       - `Core/Inc/stm32h7xx_it.h`
       - `Core/Src/stm32h7xx_it.c`
     - confirm:
       - `extern DCMI_HandleTypeDef hdcmi;`
       - `void DCMI_IRQHandler(void)`
       - `HAL_DCMI_IRQHandler(&hdcmi);`

3. camera-critical DCMI settings were overwritten
   - Symptom:
     - XCAM black screen
     - `noframe`
     - corrupted image
     - no valid capture
   - Recovery:
     - re-check `Core/Src/dcmi.c`
     - confirm:
       - `PCKPolarity = DCMI_PCKPOLARITY_RISING`
       - `VSPolarity = DCMI_VSPOLARITY_LOW`
       - `HSPolarity = DCMI_HSPOLARITY_LOW`
       - `ExtendedDataMode = DCMI_EXTEND_DATA_8B`
       - `JPEGMode = DCMI_JPEG_DISABLE`

4. GPIO mapping was overwritten
   - Symptom:
     - SCCB reads `mid=0xFFFF pid=0xFFFF`
     - camera dark / no valid response
     - or upper data bits corrupt image
   - Recovery:
     - re-check `Core/Src/gpio.c`
     - confirm:
       - `D5 = PD3`
       - `D6 = PB8`
       - `D7 = PB9`
       - `PB3/PB4` remain SCCB open-drain pull-up
       - `PA15` remains camera reset output
       - `PA0` remains camera power-down output

5. `PA0` analog switch / PWDN chain was broken by regeneration
   - Symptom:
     - probe fails even though wiring is correct
   - Recovery:
     - re-check `Core/Src/gpio.c`
     - confirm:
       - `HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PA0, SYSCFG_SWITCH_PA0_OPEN);`

6. UART was overwritten
   - Symptom:
     - no XCAM image
     - no useful boot log
     - low throughput
   - Recovery:
     - re-check `Core/Src/usart.c`
     - confirm `USART1 baud = 921600`

7. AI runtime alignment / MPU safeguards were overwritten
   - Symptom:
     - boot prints stop before or during infer
     - hard fault near `infer start`
     - target appears to "run away" after flashing
   - Recovery:
     - re-check `Core/Src/main.c`
     - confirm:
       - `SCB->CCR &= ~SCB_CCR_UNALIGN_TRP_Msk;`
       - AXI SRAM MPU region still covers the full `0x24000000-0x2407FFFF` 512KB window
   - Note:
     - this was a real prior failure source on H7 + `X-CUBE-AI`

8. custom JPEG decode path was dropped from the build
   - Symptom:
     - build failure
     - AI mode compiles but no longer uses the verified preprocess path
   - Recovery:
     - confirm these files still exist and still participate in build:
       - `Core/Src/jpeg_decode.c`
       - `Core/Src/tjpgd.c`
       - `Core/Inc/jpeg_decode.h`
       - `Core/Inc/tjpgd.h`
       - `Core/Inc/tjpgdcnf.h`

9. wrong generated model set was linked
   - Symptom:
     - compile succeeds but inference results obviously belong to the wrong network
   - Recovery:
     - inspect generated files under `X-CUBE-AI/App`
     - verify the app is using the intended model family
     - pay attention if both `waterlevel_*` and `water_detect_*` coexist

10. generated/imported garbage source files broke the build
   - Symptom:
     - errors like:
       - missing `../Common/camera.h`
       - unexpected `reg51.h`
       - unrelated legacy camera/demo files being compiled
   - Recovery:
     - inspect `X-CUBE-AI/App`
     - remove or exclude unrelated imported demo sources before debugging runtime

### Fast recovery order after model regeneration

1. Confirm `APP_MODE` in `camera_app.h`
2. Confirm boot log still prints
3. Confirm `CameraApp_Init()` / `CameraApp_Run()` still called in `main.c`
4. Confirm `probe ok mid=0x7FA2 pid=0x2642`
5. Confirm XCAM view still works in `APP_MODE_XCAM_VIEW`
6. Confirm AI path still uses `JPEG snapshot -> TJpgDec decode -> preprocess -> infer`
7. Confirm generated model report matches `camera_app.c` input/output parsing
8. Only then debug model accuracy

### Model swap acceptance rule

- Do not start investigating model accuracy until both are true:
  - `APP_MODE_XCAM_VIEW` image is normal
  - `APP_MODE_AI_TEST_IMAGE` matches PC-side inference on the same fixed test image

## 2026-06-27 AI Integration Update

- Current branch remains `codex/xcubeai`.
- Camera application now supports two compile-time modes in `Core/Inc/camera_app.h`:
  - `APP_MODE_XCAM_VIEW`
    - keeps the known-good JPEG snapshot path for XCAM preview
    - still uses `JPEG_Stream` as the DCMI DMA target
  - `APP_MODE_AI_INFER`
    - does **not** upload JPEG to XCAM
    - switches OV2640 output format to `RGB565`
    - captures one raw `320x240` frame directly through DCMI DMA
    - preprocesses to model input and runs `waterlevel`
- Important memory rule:
  - JPEG mode and AI mode do **not** keep both large buffers resident at once.
  - In `APP_MODE_AI_INFER`, DCMI uses a dedicated raw-frame buffer sized for one `RGB565 320x240` frame.
  - In `APP_MODE_XCAM_VIEW`, DCMI continues using the original JPEG stream buffer.
- Current H7 AI path design:
  1. OV2640 raw output: `RGB565 320x240`
  2. `RGB565 -> gray`
  3. local contrast enhancement approximation for CLAHE intent
  4. logical `320x240 -> pad to 320x320 -> center crop 224x224`
  5. circular mask: center `(112,112)`, radius `100`
  6. quantize to model input `int8`, zero point `-128`
  7. optional brightness normalization toward nonzero mean `~0.355`
  8. run `waterlevel` and print class / confidence / reg / inference time
- Current generated model facts confirmed from `waterlevel_generate_report.txt`:
  - input: `int8 1x224x224x1`, `scale=0.003921569`, `zero_point=-128`
  - output0 logits: `int8 x5`, `scale=0.105105773`, `zero_point=42`
  - output1 reg(sigmoid): `int8 x1`, `scale=0.000057468`, `zero_point=-128`
- Current status:
  - `camera_app.c` object-level compile was validated for both:
    - `APP_MODE_XCAM_VIEW`
    - `APP_MODE_AI_INFER`
  - Full project link/build was not run in terminal because this shell session does not expose `make`.
## 目标
将 `cursor/CoreU5` 中的 OV2640 相关底层能力迁移到当前 `Core` 工程，形成适配 STM32H743IIT6 的纯底层图像采集方案。

## 硬件引脚映射

| 功能 | 引脚 | 说明 |
|------|------|------|
| DCMI_D0~D4 | PC6, PC7, PC8, PC9, PC11 | |
| DCMI_D5~D7 | PD3, PB8, PB9 | 当前已确认真实连线 |
| DCMI_VSYNC | PB7 | |
| DCMI_HREF | PH8 | |
| DCMI_PCLK | PA6 | |
| SCCB SCL | PB4 | 开漏+上拉 |
| SCCB SDA | PB3 | 开漏+上拉 |
| LED 补光 | PA8 | |
| RESET | PA15 | 低电平复位 |
| PWDN | PA0 | 高电平有效 |
| USART1 TX/RX | PA9/PA10 | 调试+JPEG输出 |
| 外部触发键 | 任意空闲 GPIO 输入 | |

- OV2640 使用模块自带晶振提供 XCLK，不使用 MCO1

## 模块分层

```
main.c          ─── 系统初始化 + 主循环
camera_app      ─── 应用状态机 (XCAM_VIEW 视频流)
├── ov2640      ─── 上电/复位/ID/JPEG配置表
│   └── ov2640_sccb ─── PB3/PB4 软件 SCCB 时序
└── jpeg_stream ─── 帧缓冲(64KB, 32B对齐) + SOI/EOI搜索
```

## 必须遵守的时序

1. PA0 置低，释放/使能 OV2640 供电（按硬件电平定义执行）
2. 延时 15ms（当前用 300ms 安全余量）
3. PA15 拉低 10ms，再拉高
4. 延时 25ms（当前用 100ms 安全余量）
5. SCCB 读 MID/PID
6. ID 成功后写入 JPEG 初始化表

## 当前状态 (2026-06-18)

### ✅ 已验证通过的
- SCCB 通信稳定 (MID=0x7FA2 PID=0x2642)
- OV2640 寄存器读写正确 (COM7/COM10/CLKRC/0x33/0x3C/0xDA 均可回读验证)
- DCMI 外设配置正确 (CR 寄存器确认 ENABLE+CAPTURE 已置位)
- DCMI 中断向量表正确 (DCMI_IRQHandler 已在 startup 和 it.c 中)
- 模块供电 3.2V ✅
- PA0 模拟开关 → OPEN (GPIO 模式, PWDN 控制)
- PA15 → 必须直接写 MODER 寄存器强制 GPIO 输出 (HAL_GPIO_WritePin 对 PA15 不可靠)
- 帧搜索算法: 从 buffer 尾部向前搜 EOI→SOI (最后一个完整帧)

### 🔴 当前阻塞: PCLK 间歇性不启动

**现象:** DCMI SR FNE=0 (FIFO永不收到数据), VSYNC=0, HSYNC=0。
即使CONTINUOUS模式(不依赖VSYNC)也收不到任何字节。

**关键线索:**
- 2026-06-18 17:40: FNE=1 ✅ (首次出现, 添加 sw_reset 后)
- 2026-06-18 18:13: FNE=1 ✅ (回退代码后再次出现)
- Raw PCLK 测试 (连 Probe 都不做): 始终 DEAD
- 彩条模式 (传感器内部生成, 不依赖镜头): 同样无 PCLK
- 寄存器全部验证正确 (0x33=0x80 0x3C=0x40 COM7=0x00 COM10=0x00)

### 🔧 已尝试的方案 (均未稳定解决)

| 尝试 | 结果 |
|------|------|
| PCLK 极性 FALLING→RISING | ❌ 单改无效 |
| JPEGMode ENABLE→DISABLE | ❌ |
| Capture SNAPSHOT→CONTINUOUS | ❌ SNAPSHOT永远超时 |
| PA0 模拟开关 CLOSE→OPEN | ❓ 2次FNE=1均为OPEN, 但CLOSE有时也能Probe |
| PA15 HAL_GPIO_WritePin→寄存器直接写 | ✅ 确认PA15能拉高, 但PCLK问题无关 |
| SW复位 COM7=0x80 | ❓ 添加后FNE=1出现过2次, 去掉后始终DEAD |
| 上电时序: 300ms/500ms/1000ms/断电再上电 | ❌ 时序敏感但不稳定 |
| yuv422_cfg (0x33=0x80, 0x3C=0x40) | ❓ 可能有帮助 |
| jpeg_cfg DSP bank vs Sensor bank | ❌ 均不稳定 |
| 单次Init vs 双次Probe+Init | ❌ 双次更稳定(Probe成功率高) |
| VCC提升至3.2V | ✅ 必要条件但非充分 |

### 📋 待验证假设

1. **晶振起振条件**: 模块需要特定的电源斜率或时序才能稳定起振
2. **SCCB时序容差**: 我们的位带操作 vs U5的HAL写Pin可能有微小延迟差异
3. **寄存器配置遗漏**: U5配置表中可能有我们未覆盖的寄存器
4. **DCMI FIFO配置**: H7 DCMI 可能需要特定FIFO阈值设置

## DCMI 配置

| 参数 | 当前值 | 备选值 |
|------|--------|--------|
| PCKPolarity | RISING | FALLING (原始) |
| HSPolarity | LOW | HIGH (OV2640 HREF=高有效) |
| VSPolarity | LOW | HIGH |
| JPEGMode | ENABLE | DISABLE (U5用) |
| Capture Mode | CONTINUOUS | SNAPSHOT |
| DMA | DMA2_Stream3, WORD, NORMAL | |

## DCMI 中断
- DCMI_IRQn 优先级 = 5（不阻塞 SysTick）
- CONTINUOUS 模式不需要帧中断

## 已验证的关键坑

### PA0 模拟开关必须设为 OPEN
```c
HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PA0, SYSCFG_SWITCH_PA0_OPEN);
```

### PA15 必须用寄存器直接操作
```c
GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE15) | GPIO_MODER_MODE15_0;
GPIOA->BSRR = (uint32_t)GPIO_PIN_15 << 16U;  /* LOW */
GPIOA->BSRR = GPIO_PIN_15;                   /* HIGH */
```

### VCC 必须 ≥ 2.8V
模块 VCC < 2.7V 时晶振不起振、PLL 不锁。务必保证排线供电 ≥ 3.2V。

## H7 关键注意点

- **不使用 MCO1**，不输出摄像头外部时钟
- **DMA 缓冲区放在 D1 AXI SRAM**（0x24000000），并保证 32 字节对齐
- **DCMI 单帧完成后必须 `SCB_InvalidateDCache_by_Addr`**；当前方案用 MPU 设 NON_CACHEABLE + `volatile` 绕过
- **JPEG 数据发送仅发送 `0xFF 0xD8` 到 `0xFF 0xD9` 之间的纯二进制流**
- **CubeMX 必须设 SWD 模式** — 默认 JTAG 5线会锁 PB3(JTDO)/PB4(JNTRST)/PA15(JTDI)
- **CubeMX 重新生成后检查回退** — PCKPolarity、JPEGMode、DMA优先级、MPU注释会被重置
- **__HAL_RCC_DBGMCU_CLK_ENABLE 在 H7 上不存在**
- 外设初始化文件保持 CubeMX 生成结构，手写代码只放在 `USER CODE` 区域内
- 新增业务逻辑独立放在自定义应用层文件（camera_app、ov2640、jpeg_stream）
- 代码风格遵循 STM32CubeIDE 生成风格：句柄全局化、初始化函数单一职责、错误处理统一 `Error_Handler()`
- 迁移期间允许学习 `cursor/CoreU5` 参考代码，但平台相关部分（GPIO/DMA/SCCB/DCMI/Cache/串口句柄）必须按 H7 的 CubeMX 配置重新适配
- 参考 U5 时优先学习：业务流程、状态机划分、寄存器配置顺序、错误处理、模式切换设计
- 当工程实践与旧代码冲突时，以 **H7 官方手册和当前芯片实际行为** 为准

## 当前执行流程

按以下顺序推进，**任何一步未验证通过前都不要跳到下一步**：

1. ✅ 保持当前时钟树不变，不修改系统时钟、PLL 与分频参数
2. ✅ 保持 CubeMX 已配置好的引脚不变，仅在用户代码区和独立应用层文件中写业务逻辑
3. ✅ 直接使用 PA0 控制 OV2640 PWDN，不再使用 PCF8574
4. ✅ 完成 `ov2640_sccb` 模块 — PB3/PB4 软件 SCCB 时序与寄存器读写
5. ✅ 完成 `ov2640` 模块 — 供电、复位、读 ID、写 JPEG 初始化表
6. ✅ 完成 `jpeg_stream` 模块 — 帧缓冲管理、SOI/EOI 搜索
7. ✅ 完成 `camera_app` 模块 — XCAM_VIEW 状态机
8. ✅ main.c 只负责在用户代码区初始化应用层、轮询任务和 LED 活性指示
9. 🔴 **调试优先级：供电与复位 > SCCB 读 ID > JPEG 初始化 > DCMI 单帧采集 > Cache 处理 > JPEG 帧解析**
10. 🔴 **在未确认单帧采集稳定前，不进入连续帧、显示、回传优化等后续功能**

## CubeMX 回退检查清单

CubeMX 重新生成后需要手动恢复：
1. `dcmi.c`: `PCKPolarity` → `RISING`
2. `gpio.c`: `SYSCFG_SWITCH_PA0` → `OPEN`
3. `gpio.c`: PA15 HAL 写换成 BSRR 直接操作 (或 `USER CODE` 中加)
4. DMA 优先级 ≠ 0
5. Debug 模式确保为 SWD（不是 JTAG 5线）
6. MPU 区域注释不会被重置

## 参考代码

- `cursor/CoreU5/` — 正点原子 ATK-MC2640 U5 工程（已验证跑通）
- 参考其业务流程、状态机划分、寄存器配置顺序、错误处理与模式切换设计
- 平台相关部分（GPIO/DMA/SCCB/DCMI/Cache/串口句柄）以 H7 CubeMX 配置为准

---

## 2026-06-18 PCLK 调试记录

### FNE=1 成功次数: 3次

| # | 时间 | 触发条件 | 现象 |
|---|------|---------|------|
| 1 | 17:40 | 添加 sw_reset(COM7=0x80) | FNE=1, 多次复位稳定 |
| 2 | 18:13 | 回退到#1相同代码 | FNE=1, SOI=30348 |
| 3 | 18:45 | 时序调整后某版本 | FNE=1, COM7/COM10读数被PCLK噪声干扰→证明DVP有信号 |

### 成功时共同特征
- `0x3C` 读回值始终为 `0xFF`（DSP默认，说明DSP有时钟驱动）
- SCCB 读数可能被 DVP 噪声干扰（COM7=0xFE/0xFF 等异常值）
- SW 复位 `COM7=0x80` 始终存在
- PA0 模拟开关 = OPEN
- DCMI: RISING, LOW, LOW, DISABLE

### 失败时共同特征
- `0x3C` 读回 `0x00`（DSP无时钟）
- DCMI SR 始终全零: VSYNC=0, HSYNC=0, FNE=0
- SCCB 能读 ID，寄存器也能读写——数字部分正常，模拟部分(PLL/晶振)停摆
---

## 2026-06-27 AI inference HardFault

- 现象：`APP_MODE_AI_INFER` 下日志停在 `[AI] infer start`。
- 故障寄存器：`[FAULT:Hard] CFSR=0x01000000 HFSR=0x40000000`。
- 结论：这是 `UNALIGNED` 使用错误，不是相机链路错误；问题点在 `ai_waterlevel_run()` 内部。

### 已修复

1. `Core/Src/main.c`
   - 清除 `SCB_CCR_UNALIGN_TRP_Msk`
   - 保留 `DSB/ISB`
2. `Core/Src/main.c`
   - AXI SRAM MPU 非缓存区从 `256KB` 扩到 `512KB`
3. `Core/Src/camera_app.c`
   - AI `activations/input/output` 改成独立 `AI_ALIGNED(32)` 静态数组
   - 不再把这些大数组塞进 `g_ai_ctx` 结构体

### 回归检查

- 每次 CubeMX / X-CUBE-AI 重新生成后，复查：
  - `SCB->CCR &= ~SCB_CCR_UNALIGN_TRP_Msk;`
  - `MPU_REGION_SIZE_512KB`
  - AI 大缓冲仍是独立对齐数组

### 当前状态

- `APP_MODE_AI_INFER` 已可连续运行，不再 HardFault。
- 当前总耗时约 `1.58s` 一帧，主要瓶颈在固件侧预处理，不在 `ai_waterlevel_run()` 本身。
- 串口日志不能再使用 `%f`，否则 `newlib-nano` 下会出现 `conf/reg` 空白和乱码；统一改为整数定点打印。

### 2026-06-27 进一步结论

- 当前部署模型不是 “Cube.AI 随机量化” 产物：
  - `waterlevel_generate_report.txt` 显示导入文件就是 `v3_w05_pretrained_no_se_PerChannel_quant_calib_200_npz_1.onnx`
  - 输入/输出与内部算子均为 `int8`
- 因此，当前 “稳定误判 low” 的更高概率原因不是运行时量化失败，而是 **MCU 端输入分布和训练端预处理仍不完全一致**。
- 当前 H7 端已实现：
  - `320x240` 相机输入
  - 中心裁剪等价于 `320x320 pad` 后再 `224x224 crop`
  - 圆形遮罩
- 当前 H7 端仍与训练端存在的主要风险：
  - CLAHE 不是 OpenCV 原版实现
  - 尚未做 “导出 MCU 端 224x224 输入图” 与训练端逐像素比对
- `nn=334ms` 不能据此判断模型跑成了 FP32：
  - 当前工程运行在 `Debug/-O0`
  - AI activations 位于非缓存 AXI SRAM
  - 这两点都会显著拉慢 H7 上的推理时间

---

## 2026-07-01 CubeMX / X-CUBE-AI Model Replacement Recovery Checklist

### Current AI baseline

- The current H7 AI path to preserve is:
  - `OV2640 JPEG snapshot`
  - `TJpgDec decode`
  - `training-aligned preprocess`
  - `X-CUBE-AI infer`
- Do not accidentally fall back to the older experimental `RGB565 direct -> infer` branch when regenerating the project.
- The current preprocess expectation is:
  - `320x240`
  - `CLAHE`
  - `pad to 320x320`
  - `center crop to 224x224`
  - `circular mask radius=100`
  - then convert to the model input format

### Before replacing the model

1. Commit the current working state to git first.
2. Record the current generated model name and the matching `*_generate_report.txt`.
3. Change only the CubeMX / X-CUBE-AI model configuration first.
4. Regenerate once, then repair and verify the camera/AI chain in a controlled order.

### Files most likely to be broken by regeneration

- `Core/Inc/camera_app.h`
  - keep `APP_MODE_XCAM_VIEW`
  - keep `APP_MODE_AI_INFER`
  - keep `APP_MODE_AI_TEST_IMAGE`
  - re-check the default `APP_MODE`
- `Core/Src/camera_app.c`
  - keep the current AI flow:
    - `camera_app_capture_jpeg_snapshot()`
    - `jpeg_to_ai_input()`
    - `camera_app_ai_run()`
  - do not revert to the old raw RGB565 path by mistake
  - re-check all model input/output scale and zero-point constants
- `Core/Src/main.c`
  - keep:
    - `SCB->CCR &= ~SCB_CCR_UNALIGN_TRP_Msk;`
    - `__DSB();`
    - `__ISB();`
  - keep AXI SRAM MPU settings:
    - `BaseAddress = 0x24000000`
    - `Size = MPU_REGION_SIZE_512KB`
    - `TypeExtField = MPU_TEX_LEVEL1`
    - `IsCacheable = MPU_ACCESS_NOT_CACHEABLE`
  - keep `CameraApp_Init();`
  - keep `CameraApp_Run();`
- `Core/Src/dcmi.c`
  - keep `PCKPolarity = DCMI_PCKPOLARITY_RISING`
  - keep the verified mapping:
    - `PD3 = D5`
    - `PB8 = D6`
    - `PB9 = D7`
- `Core/Src/usart.c`
  - keep `USART1 BaudRate = 921600`
- `Core/Src/stm32h7xx_it.c`
  - keep `#include "dcmi.h"`
  - keep `DCMI_IRQHandler()`
  - keep `HAL_DCMI_IRQHandler(&hdcmi);`
- `Core/Src/ov2640.c`
  - keep `OV2640_VIDEO_JPEG_QUALITY = 0x03U`
  - keep the current `320x240` output size
  - keep the current JPEG register programming order

### Non-generated files that must remain in the build

- `Core/Src/jpeg_decode.c`
- `Core/Inc/jpeg_decode.h`
- `Core/Src/tjpgd.c`
- `Core/Inc/tjpgd.h`
- `Core/Inc/tjpgdcnf.h`

Important:

- These files are not CubeMX-generated peripheral files.
- After regeneration, they may still exist on disk but silently drop out of the build.
- Always verify both:
  - the file still exists
  - the project is still compiling and linking it

### Build-list checks after regeneration

The project has already had a failure mode where source files existed but were not part of the actual build. Re-check:

- `Debug/Core/Src/subdir.mk`
  - should still include:
    - `../Core/Src/jpeg_decode.c`
    - `../Core/Src/tjpgd.c`
- `Debug/objects.list`
  - should still include:
    - `./Core/Src/jpeg_decode.o`
    - `./Core/Src/tjpgd.o`

If the regenerated project suddenly loses JPEG-decode or AI-preprocess behavior, inspect these first.

### Model report items that must be re-synced

Always use the new generated report under:

- `X-CUBE-AI/App/<model>_generate_report.txt`

Do not keep the previous quantization constants by habit. Re-check:

- input shape
- input type
- input scale
- input zero-point
- logits output type/scale/zero-point
- reg output type/scale/zero-point

Current `waterlevel` example values are:

- input: `int8(1x224x224x1)`, `QLinear(0.003921569, -128, int8)`
- logits: `int8(1x5)`, `QLinear(0.094021469, -8, int8)`
- reg: `int8(1x1)`, `QLinear(0.003912641, -128, int8)`

These are mirrored in `Core/Src/camera_app.c` and must be re-checked after every model replacement:

- `WATERLEVEL_IN_ZERO_POINT`
- `WATERLEVEL_OUT0_SCALE`
- `WATERLEVEL_OUT0_ZERO_POINT`
- `WATERLEVEL_OUT1_SCALE`
- `WATERLEVEL_OUT1_ZERO_POINT`

If the new model is `float32` instead of `int8`, the current `int8` input/output parsing path is no longer valid and must be changed together with the model.

### Generated-model selection check

This project has already contained multiple generated model sets, for example:

- `waterlevel_*`
- `water_detect_*`

After replacing the model, verify that `camera_app.c` is actually including and calling the intended generated network, rather than still linking an older one.

### Recovery order after model replacement

1. Make the project build cleanly first.
2. Confirm boot logs still exist:
   - `[BOOT] main enter`
   - `[APP] CameraApp_Init enter`
3. Confirm the camera chain still works:
   - `[APP] probe ok`
   - `[AI] capture ok`
   - `[AI] prep ok`
4. Then confirm the AI chain:
   - `[AI] infer start`
   - `[AI] infer ok`
5. If `APP_MODE_XCAM_VIEW` is broken, fix camera/JPEG first.
6. If `APP_MODE_XCAM_VIEW` is fine but `APP_MODE_AI_INFER` is wrong, inspect model I/O format and quantization first.

### Fast validation sequence

1. `APP_MODE_AI_TEST_IMAGE`
   - feed the fixed test image
   - compare MCU result with the PC-side result
2. `APP_MODE_XCAM_VIEW`
   - confirm the camera still outputs a stable image
   - use this to re-adjust camera position if needed
3. `APP_MODE_AI_INFER`
   - only after the first two pass, test real-time inference

If the fixed test image matches PC inference but the live camera result does not, the issue is much more likely to be:

- preprocess mismatch
- live image quality degradation
- camera position / lighting drift

### Common symptom -> first check

- build error: `hdcmi undeclared`
  - check whether `stm32h7xx_it.c` lost `#include "dcmi.h"`
- flash succeeds but board boots badly / no UART log
  - check `main.c` for boot prints, MPU settings, and UNALIGN handling
  - then re-check clock/HSE/PLL settings
- `APP_MODE_XCAM_VIEW` has no image
  - check `dcmi.c` polarity and D5/D6/D7 mapping first
  - then check `USART1` baud rate
- AI always outputs the wrong class
  - check the new `*_generate_report.txt`
  - then check `camera_app.c` quantization constants
  - then check that `jpeg_decode.c` still matches the training preprocess

### Operational rule for the next model swap

- Change only the model first, not the camera chain.
- Validate camera output and AI output separately.
- After every regeneration, restore the chain in this order:
  - boot
  - UART
  - camera
  - JPEG decode
  - preprocess
  - inference
- If the project starts drifting in multiple places at once, stop and return to the last good git baseline instead of continuing to stack fixes.
