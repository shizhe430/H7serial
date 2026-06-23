# H7serial — STM32H743IIT6 OV2640 图像采集工程

## 目标
将 `cursor/CoreU5` 中的 OV2640 相关底层能力迁移到当前 `Core` 工程，形成适配 STM32H743IIT6 的纯底层图像采集方案。

## 硬件引脚映射

| 功能 | 引脚 | 说明 |
|------|------|------|
| DCMI_D0~D4 | PC6, PC7, PC8, PC9, PC11 | |
| DCMI_D5~D7 | PD3, PE5, PE6 | CubeMX 实际生成 |
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

