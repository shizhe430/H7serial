# 复盘证据索引

更新时间：2026-07-30

本目录保存从 Codex 临时附件和工程外部资料中复制出的关键原始材料。串口文件保持原始字节，不做编码修复或内容改写，因此部分中文接收标记可能乱码。

## 硬件资料

| 文件 | 用途 | SHA-256 |
| --- | --- | --- |
| `hardware/STM32H7-final-pin-assignment.xlsx` | 最终引脚分配，后续迁移的唯一表格来源 | `EB9F53CC0EDE7A244D1091E7028D40A621D518CCD02A7F117272F5F3E0DB07A3` |

## 主流程和视觉日志

| 文件 | 主要内容 | SHA-256 |
| --- | --- | --- |
| `serial/2026-07-17-camera-recovery.txt` | 摄像头恢复与主流程启动记录 | `8E33EB93C49220AA8D07364E973D1096244831F1E39D08F101F397D589C3689A` |
| `serial/2026-07-17-camera-decode-instability.txt` | JPEG 解码失败概率升高 | `F65D0FE74B630088615927B9324FFA91AFEE06235B45EC52144E294ECEBBDC00` |
| `serial/2026-07-17-workflow-three-issues.txt` | 语音窗口、光照误判和流速问题 | `8E200E7B828A37C09D406886FFB731B3A139DC2A704A2F6D889DD9BE1F05C123` |
| `serial/2026-07-17-successful-dispense-calibration.txt` | 自动、语音和异常保护成功；流量标定依据 | `2B161A2C17BD0C79B8E1EC2791E1FC065400F96AEB70DD2FC96C4E5C0AD3D50B` |
| `serial/2026-07-18-fingerprint-and-serial-abnormal.txt` | 指纹失效和串口异常 | `EBF46C4B2BDF578A36BBCA123504F7F41228AA692B3E1BEE8D0E96E45AA989F3` |
| `serial/2026-07-18-half-cup-transition.txt` | 低水位直接跳满杯、半杯稀少 | `09B2F3AC00264988AAA2F61AA98C38A1F1BA63EC7295DF1864B91ECE8D58B5EF` |
| `serial/2026-07-18-full-cup-flow-calibration.txt` | 满杯重新标定和水箱传感器阶段记录 | `DB3509F95A12F8E4E92393AA93DFF863A17B8073B151430824A74C3E85AF8AC2` |
| `serial/2026-07-18-esp32-reporting.txt` | STM32 端 ESP32 上报证据 | `05B56C526C90C44E9F25D6927B9B40C8C666E924385C626A8A6A7EE2C29B4BD2` |
| `serial/2026-07-18-second-cycle-cls2.txt` | 第二次放杯后长期 cls=2 | `3E2C57102776284DD23F93C30DB9CF38790A30085B389229F9B8639F9A69F844` |
| `serial/2026-07-19-post-dispense-abnormal.txt` | 出水结束后持续异常 | `A81F5ACBBDA6AC42A2207FDF9FF38CC738FDB474F0B5745FDFEF22AD8E8E7BDD` |
| `serial/2026-07-23-half-threshold-run-a.txt` | 半杯阈值实验 A | `BD369EEE541BBE1F61180D5C2CDE12F5EB634F79084DDED20A102A5EE82FD7AF` |
| `serial/2026-07-23-half-threshold-run-b.txt` | 半杯阈值实验 B | `F42D5D00798623955855E9EA668A63765113E97D6B8F89ADFAA9EE9B7DA874F4` |
| `serial/2026-07-24-early-abnormal-stop.txt` | 刚出水即异常停水 | `73CE279254D9ED910D6A8F042EAC8FF3EFA483098ACC5E2097F6083EC0DC4BEC` |
| `serial/2026-07-24-full-stop-regression.txt` | 满杯停水回归 | `606E065CDC04FB1AFB09668954B37A8C4C39820DC421A806D338DC7B56F161B2` |
| `serial/2026-07-29-dual-camera-stable.txt` | 双摄交替探测和 JPEG 捕获成功 | `3BACA9667F2001325C4DC450805CCA89F8CADA03F2A4E160BA67CB1A18AA5E82` |

## 外部存储日志

| 文件 | 主要内容 | SHA-256 |
| --- | --- | --- |
| `serial/2026-07-29-sdram-qspi-first-test.txt` | SDRAM 初测失败、QSPI ID 成功 | `BACE77341ECE40E2F90FF0B082DC8F81415719D19226343CFA47C4D7392B6692` |
| `serial/2026-07-29-sdram-diagnostic-a.txt` | SDRAM 诊断阶段 A | `568324CA0008FDCEA6EB4CBC8662B35166F5D0B73D2D9153F10A118DA45C634A` |
| `serial/2026-07-29-sdram-diagnostic-b.txt` | SDRAM 诊断阶段 B | `A1C0995D90129F38DE7FF59BC9B779ED0F00A6CBDD1BD3F9848330A90A84BAE9` |
| `serial/2026-07-30-vendor-sdram-example-output.txt` | 正点原子 SDRAM 例程输出 | `D636CCB3350DD6A7975A6A4EEF67084FDBAA16ADC5AC1D2A58B1FDCE15FBF4F6` |
| `serial/2026-07-30-vendor-sdram-stress-output.txt` | 官方例程压力/探针输出 | `FAD9D25D9EF37F40AE1A1CEA2308F8883140AD1CEB0B9708CBA75EF65CA7273B` |
| `serial/2026-07-30-sdram-capacity-run-a.txt` | SDRAM 容量扫描 A | `24161AD5A146CB53AF2971A139DB428DE464C6981E76BA9AF28D8A7C019C3978` |
| `serial/2026-07-30-sdram-capacity-run-b.txt` | SDRAM 容量扫描 B | `724EA4ABDDF99CCFD5419193A199CA099168F16632E432673ABC2FEB606FE16F` |
| `serial/2026-07-30-experiment65-nand-ecc.txt` | 综合实验 65：LCD/触摸初始化及 NAND ECC 错误 | `D52AE06F5DF6389AB45EBE6F5E2AB0782D25B4590BC5091EB6E84DE57E507E71` |

## 使用规则

- 分析前先核对日志对应的 Git 提交、编译时间和烧录文件，不以文件日期替代固件身份。
- 保留原始文件；需要清洗乱码或提取数据时生成新文件，不覆盖本目录内容。
- 新增证据时记录来源、实验唯一变量、硬件连接、固件 SHA-256 和结果。
- 根因结论必须至少有一次单变量对照实验支撑。

