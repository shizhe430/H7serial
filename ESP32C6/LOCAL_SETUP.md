# ESP32-C6 + WaterDispenserWeb 本机运行说明

## 当前本机配置

- ESP32-C6 串口：`COM29`
- 网页监听：`http://0.0.0.0:5000`
- 当前热点：`hsz 8`（电脑和手机/C6 使用同一热点）
- 电脑 WLAN 地址：`192.168.86.151`
- C6 访问网页 API：`http://192.168.86.151:5000`
- 数据库：MySQL 8.4，数据库名 `water_dispenser`
- 手机设备控制：ESP RainMaker App（BLE 首次配网）

电脑和 ESP32-C6 必须连接到同一个局域网。电脑 IP 改变后，用新的 WLAN IPv4 地址执行
`build.ps1 -ServerIp <新地址> -Flash`。脚本会同步服务器 URL、Host 和 Port 并重新烧录。

## 日常启动网页

数据库连接和 DeepSeek 密钥只保存在 Windows 当前用户环境变量中，不写入源码或 Git。
迁移到另一台电脑时，先设置下面两个变量，然后重新打开终端：

```powershell
[Environment]::SetEnvironmentVariable("ConnectionStrings__DefaultConnection", "Server=127.0.0.1;Database=water_dispenser;User=<数据库用户>;Password=<数据库密码>;", "User")
[Environment]::SetEnvironmentVariable("DeepSeek__ApiKey", "<你的密钥>", "User")
```

数据库备份包含真实用户资料和密码数据，不随 Git 提交；需要时从受控位置单独导入 `water_dispenser_backup.sql`。

在 PowerShell 中执行：

```powershell
cd C:\Users\heshizhe\Desktop\STM32\H7serial\ESP32C6\WaterDispenserWeb
.\run-local.ps1
```

电脑浏览器访问 `http://localhost:5000`，同一 Wi-Fi 下的手机访问
`http://192.168.86.151:5000`。保持这个 PowerShell 窗口运行。

## 构建和烧录 C6

```powershell
cd C:\Users\heshizhe\Desktop\STM32\H7serial\ESP32C6\Smart_Water_Gate_C6
.\build.ps1 -Port COM29 -ServerIp 192.168.86.151 -ServerPort 5000 -Flash
```

查看串口日志：

```powershell
& C:\Espressif\python_env\idf5.5_py3.10_env\Scripts\python.exe .\monitor_local.py --port COM29 --seconds 60
```

日志同时保存到 `boot_log_local.txt`。也可以运行 `.\build.ps1 -Port COM29 -Monitor`
进入 ESP-IDF 交互监视器，按 `Ctrl+]` 退出。

## 连接 ESP RainMaker 手机 App

1. 手机安装并登录 `ESP RainMaker`，打开蓝牙、Wi-Fi 和定位权限。
2. 第一次配网前清除开发板上队友留下的 Wi-Fi/RainMaker NVS，再重新烧录固件。
3. 打开串口监视器，等待日志显示 `Scan this QR code`。日志同时会给出设备名、PoP 和二维码 URL。
4. App 中点添加设备/扫描二维码；若无法扫描，选择 BLE 设备 `PROV_xxxxxx` 并按日志输入 PoP。
5. 选择手机当前连接的 2.4 GHz Wi-Fi，输入 Wi-Fi 密码。ESP32-C6 只应配置到能访问这台电脑的同一局域网。
6. 配网完成后，App 中会出现“智能饮水机”，可控制 Power、目标水量等 RainMaker 参数。
7. 网页负责用户、饮水记录、AI 建议和账单；RainMaker App 负责远程设备控制，两者不是同一个账号系统。

若 C6 能连 Wi-Fi 但网页没有数据，先检查 Windows 防火墙是否允许 `dotnet` 在专用/公用网络监听 TCP 5000，再确认 C6 日志中的服务器 URL 是当前电脑 WLAN 地址。

当前 WLAN 是公用网络，首次使用前请在“管理员 PowerShell”运行：

```powershell
$app = "C:\Users\heshizhe\Desktop\STM32\H7serial\ESP32C6\WaterDispenserWeb\bin\Debug\net8.0\WaterDispenserWeb.exe"
New-NetFirewallRule -DisplayName "WaterDispenserWeb TCP 5000 (LocalSubnet)" -Direction Inbound -Action Allow -Program $app -Protocol TCP -LocalPort 5000 -RemoteAddress LocalSubnet -Profile Any
```

若配网等待超过 30 分钟，按一下 C6 的复位键，未配网设备会重新广播 `PROV_0e8d04`。
