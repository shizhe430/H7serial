# STM32H7 -> 正点原子 ESP32P4M 引脚映射

## 目标
基于 `D:/STM32H7引脚映射.xlsx`、正点原子 DNESP32P4 小系统板原理图/硬件手册和 ESP32-P4 引脚约束，形成逐信号、可布板的迁移映射，优先验证 OV2640 DVP 可行性。

## 阶段
- [complete] 1. 解包并定位小系统板原理图、引脚表和芯片约束
- [complete] 2. 读取并核对 STM32H7 现有信号及实际工程使用情况
- [complete] 3. 分配 ESP32-P4 GPIO，完成 DVP/串口/I2C/泵控冲突检查
- [complete] 4. 输出映射 XLSX 和设计说明
- [complete] 5. 复核启动脚、保留脚、电气与驱动可行性

## 关键约束
- P4 为主控，外置 C6 保留现有 UART 联网协议。
- OV2640 DVP 优先；只有引脚或驱动无法满足时才切 MIPI。
- 不占用小系统板内部 Flash/PSRAM/USB/启动必需信号。
- 摄像头时钟、数据线和泵控必须给出明确 GPIO，不留“任选”占位。

## 错误记录
- Codex bundled workspace dependencies 未配置，XLSX 使用本机 Python/openpyxl 读取。
- 首次读取 XLSX 时本机缺少 `openpyxl`，已安装 3.1.5 后继续。
- GitHub raw 内容首次并行读取有 3 个请求超时，改用 GitHub Contents API 成功取得官方板级 DVP 配置。
