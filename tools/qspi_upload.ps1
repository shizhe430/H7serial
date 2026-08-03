param(
    [Parameter(Mandatory = $true)] [string]$FilePath,
    [Parameter(Mandatory = $true)] [uint32]$Address,
    [string]$SerialPort = 'COM27',
    [string]$ProgrammerCli = 'C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe'
)

Add-Type @'
using System;
public static class QspiCrc32 {
    public static uint Compute(byte[] data) {
        uint crc = 0xffffffffu;
        for (int i = 0; i < data.Length; i++) {
            crc ^= data[i];
            for (int bit = 0; bit < 8; bit++) {
                crc = ((crc & 1u) != 0u) ? ((crc >> 1) ^ 0xedb88320u) : (crc >> 1);
            }
        }
        return crc ^ 0xffffffffu;
    }
}
'@

$raw = [IO.File]::ReadAllBytes((Resolve-Path -LiteralPath $FilePath))
$paddedSize = [uint32](($raw.Length + 0xFFFF) -band (-bnot 0xFFFF))
$payload = [byte[]]::new($paddedSize)
[Array]::Copy($raw, $payload, $raw.Length)
$crc = [QspiCrc32]::Compute($payload)

function Write-Bytes([System.IO.Ports.SerialPort]$Port, [byte[]]$Bytes) {
    $Port.Write($Bytes, 0, $Bytes.Length)
}

function Wait-Token([System.IO.Ports.SerialPort]$Port, [string]$Token, [int]$TimeoutMs) {
    $deadline = [Environment]::TickCount64 + $TimeoutMs
    $text = ''
    while ([Environment]::TickCount64 -lt $deadline) {
        $part = $Port.ReadExisting()
        if ($part) {
            $text += $part
            if ($text.Contains($Token)) { return $true }
        }
        Start-Sleep -Milliseconds 25
    }
    return $false
}

function Wait-Ack([System.IO.Ports.SerialPort]$Port, [int]$TimeoutMs) {
    $deadline = [Environment]::TickCount64 + $TimeoutMs
    while ([Environment]::TickCount64 -lt $deadline) {
        try {
            if ($Port.BytesToRead -gt 0) { return ($Port.ReadByte() -eq 0x06) }
        } catch [TimeoutException] {}
        Start-Sleep -Milliseconds 2
    }
    return $false
}

$port = New-Object System.IO.Ports.SerialPort($SerialPort, 921600, ([System.IO.Ports.Parity]::None), 8, ([System.IO.Ports.StopBits]::One))
$port.Encoding = [Text.Encoding]::ASCII
$port.ReadTimeout = 100
$port.WriteTimeout = 5000
$port.Open()
try {
    & $ProgrammerCli -c port=SWD freq=1000 mode=UR -rst 2>&1 | Out-Null
    if (-not (Wait-Token $port 'QSPI_UPDATER] ready' 15000)) {
        throw 'Updater ready token not received.'
    }

    $header = [byte[]](0x51, 0x55, 0x50, 0x31) +
              [BitConverter]::GetBytes([uint32]$Address) +
              [BitConverter]::GetBytes([uint32]$paddedSize) +
              [BitConverter]::GetBytes([uint32]$crc)
    Write-Bytes $port ([byte[]]$header)
    if (-not (Wait-Token $port "QSPI_UPDATER] DATA`r`n" 180000)) {
        throw 'Flash erase did not finish.'
    }

    $sent = 0
    while ($sent -lt $payload.Length) {
        $count = [Math]::Min(1024, $payload.Length - $sent)
        Write-Bytes $port ([BitConverter]::GetBytes([uint16]$count))
        $port.Write($payload, $sent, $count)
        if (-not (Wait-Ack $port 20000)) {
            throw "No ACK at offset $sent."
        }
        $sent += $count
        if (($sent % (256 * 1024)) -lt $count -or $sent -eq $payload.Length) {
            Write-Host ("{0}/{1} KiB" -f [Math]::Floor($sent / 1024), [Math]::Ceiling($payload.Length / 1024))
        }
    }

    if (-not (Wait-Token $port 'QSPI_UPDATER] done' 30000)) {
        throw 'Updater completion token not received.'
    }
    $tail = $port.ReadExisting()
    Write-Host "Upload verified by device: $FilePath at offset 0x$('{0:X8}' -f $Address)"
    if ($tail) { Write-Host $tail.Trim() }
}
finally {
    $port.Close()
}
