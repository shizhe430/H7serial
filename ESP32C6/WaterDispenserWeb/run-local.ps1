param(
    [switch]$SkipRestore
)

$ErrorActionPreference = "Stop"
$mysqlExe = "C:\Program Files\MySQL\MySQL Server 8.4\bin\mysqld.exe"
$mysqlConfig = "C:\ProgramData\MySQL\MySQL Server 8.4\my.ini"

$connectionString = [Environment]::GetEnvironmentVariable("ConnectionStrings__DefaultConnection", "User")
if ([string]::IsNullOrWhiteSpace($connectionString)) {
    throw "ConnectionStrings__DefaultConnection is not set in the current user's environment."
}
$env:ConnectionStrings__DefaultConnection = $connectionString

$deepSeekApiKey = [Environment]::GetEnvironmentVariable("DeepSeek__ApiKey", "User")
if ([string]::IsNullOrWhiteSpace($deepSeekApiKey)) {
    throw "DeepSeek__ApiKey is not set in the current user's environment."
}
$env:DeepSeek__ApiKey = $deepSeekApiKey

if (-not (Get-NetTCPConnection -State Listen -LocalPort 3306 -ErrorAction SilentlyContinue)) {
    if (-not (Test-Path -LiteralPath $mysqlExe)) {
        throw "MySQL Server 8.4 is not installed."
    }
    if (-not (Test-Path -LiteralPath $mysqlConfig)) {
        throw "Local MySQL configuration is missing: $mysqlConfig"
    }

    Start-Process -FilePath $mysqlExe `
        -ArgumentList "--defaults-file=$mysqlConfig" `
        -WindowStyle Hidden

    for ($attempt = 0; $attempt -lt 20; $attempt++) {
        Start-Sleep -Milliseconds 500
        if (Get-NetTCPConnection -State Listen -LocalPort 3306 -ErrorAction SilentlyContinue) {
            break
        }
    }
}

if (-not (Get-NetTCPConnection -State Listen -LocalPort 3306 -ErrorAction SilentlyContinue)) {
    throw "MySQL did not start on port 3306."
}

Set-Location $PSScriptRoot
if (-not $SkipRestore) {
    dotnet restore
    if ($LASTEXITCODE -ne 0) { throw "NuGet restore failed" }
}

dotnet run --launch-profile http
