param(
    [string]$SerialPort = 'COM27',
    [int]$Baud = 921600,
    [int]$TimeoutSeconds = 420,
    [string]$ProgrammerCli = 'C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe'
)

$port = New-Object System.IO.Ports.SerialPort(
    $SerialPort,
    $Baud,
    [System.IO.Ports.Parity]::None,
    8,
    [System.IO.Ports.StopBits]::One)
$port.Encoding = [Text.Encoding]::ASCII
$port.ReadTimeout = 100
$port.ReadBufferSize = 1048576
$port.Open()

try {
    & $ProgrammerCli -c port=SWD freq=1000 mode=UR -rst 2>&1 | Out-Null
    $deadline = [Environment]::TickCount64 + ($TimeoutSeconds * 1000L)
    $all = ''

    while ([Environment]::TickCount64 -lt $deadline) {
        $text = $port.ReadExisting()
        if ($text) {
            [Console]::Write($text)
            $all += $text
            if ($all.Contains('[FACE_AI] sface id_ms=') -or
                $all.Contains('[FACE_AI] no face; sface skipped') -or
                $all.Contains('[FACE_AI] weights load failed') -or
                $all.Contains('[FACE_AI] yunet run err') -or
                $all.Contains('[FACE_AI] sface run err')) {
                break
            }
            if ($all.Length -gt 16384) {
                $all = $all.Substring($all.Length - 8192)
            }
        }
        Start-Sleep -Milliseconds 20
    }
}
finally {
    $port.Close()
}
