# 乐鑫主控迁移与参赛要求分析

## 目标
基于赛事 PDF 的乐鑫赛道要求和当前 STM32H7 工程，形成一份可直接执行的主控迁移全流程文档，覆盖芯片选型、软硬件迁移、模型/语音/联网、验证、参赛材料与风险。

## 阶段
- [complete] 1. 提取并核对赛事 PDF 中乐鑫赛道条款
- [complete] 2. 盘点当前 STM32H7 工程架构、外设和功能链路
- [complete] 3. 完成目标架构、芯片选型和迁移差异分析
- [complete] 4. 编写迁移全流程及赛事合规清单
- [complete] 5. 复核条款引用、工程可行性和交付文件

## 约束
- 赛事硬性要求与建议项必须明确区分。
- 不假设 STM32 专有中间件可以直接移植。
- 保留现有作品的养老饮水场景、泵控安全逻辑和主要交互。
- 迁移建议应落到模块、接口、工具链和验收标准。

## 错误记录
- `rg.exe` 在当前环境启动时报 Access is denied，后续使用 PowerShell `Get-ChildItem` / `Select-String`。
- 工作区未配置 Codex bundled dependencies，PDF 处理改用本机 Python/Poppler 探测。
- `pypdf` 首次输出因 PowerShell GBK 编码无法显示项目符号，设置 `PYTHONIOENCODING=utf-8` 后成功。
- 清理 `tmp/pdfs` 渲染图片时，环境策略拒绝 `Remove-Item`；这些文件仅为 4 张临时页面 PNG，不影响交付文档。
