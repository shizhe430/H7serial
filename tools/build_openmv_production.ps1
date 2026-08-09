param(
    [string]$Configuration = 'Release',
    [switch]$Sdram100MHz
)

& "$PSScriptRoot\build_openmv_firmware.ps1" -Configuration $Configuration -Target Production -Sdram100MHz:$Sdram100MHz
exit $LASTEXITCODE
