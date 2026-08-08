param(
    [string]$IdfPath,
    [string]$Port = "COM29",
    [string]$ServerIp = "192.168.86.151",
    [ValidateRange(1, 65535)]
    [int]$ServerPort = 5000,
    [switch]$Flash,
    [switch]$Monitor
)

$ErrorActionPreference = "Stop"

$parsedServerIp = $null
if (-not [System.Net.IPAddress]::TryParse($ServerIp, [ref]$parsedServerIp) -or
    $parsedServerIp.AddressFamily -ne [System.Net.Sockets.AddressFamily]::InterNetwork) {
    throw "ServerIp must be a valid IPv4 address: $ServerIp"
}

function Set-WaterServerConfig {
    param([string]$ConfigPath)

    $content = [System.IO.File]::ReadAllText($ConfigPath)
    $serverUrl = "http://${ServerIp}:${ServerPort}"
    $replacements = [ordered]@{
        '(?m)^CONFIG_WATER_WEB_SERVER_URL=.*$'  = "CONFIG_WATER_WEB_SERVER_URL=`"$serverUrl`""
        '(?m)^CONFIG_WATER_WEB_SERVER_HOST=.*$' = "CONFIG_WATER_WEB_SERVER_HOST=`"$ServerIp`""
        '(?m)^CONFIG_WATER_WEB_SERVER_PORT=.*$' = "CONFIG_WATER_WEB_SERVER_PORT=$ServerPort"
    }

    foreach ($pattern in $replacements.Keys) {
        if (-not [regex]::IsMatch($content, $pattern)) {
            throw "Missing water server setting in $ConfigPath for pattern: $pattern"
        }
        $content = [regex]::Replace($content, $pattern, $replacements[$pattern])
    }

    [System.IO.File]::WriteAllText(
        $ConfigPath,
        $content,
        [System.Text.UTF8Encoding]::new($false)
    )
}

if (-not $IdfPath) {
    $IdfPath = $env:IDF_PATH
}
if (-not $IdfPath) {
    $IdfPath = "C:\Espressif\frameworks\esp-idf-v5.5.5"
}

$exportScript = Join-Path $IdfPath "export.ps1"
if (-not (Test-Path -LiteralPath $exportScript)) {
    throw "ESP-IDF export script not found: $exportScript"
}

& $exportScript
Set-Location $PSScriptRoot

Set-WaterServerConfig (Join-Path $PSScriptRoot "sdkconfig")
Set-WaterServerConfig (Join-Path $PSScriptRoot "sdkconfig.defaults")
Write-Host "C6 web server: http://${ServerIp}:${ServerPort}"

idf.py reconfigure
if ($LASTEXITCODE -ne 0) { throw "ESP-IDF reconfigure failed" }

idf.py build
if ($LASTEXITCODE -ne 0) { throw "ESP-IDF build failed" }

if ($Flash) {
    idf.py -p $Port flash
    if ($LASTEXITCODE -ne 0) { throw "ESP32-C6 flash failed on $Port" }
}

if ($Monitor) {
    idf.py -p $Port monitor
}
