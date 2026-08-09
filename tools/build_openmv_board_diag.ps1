param([string]$Configuration = 'Release')

& "$PSScriptRoot\build_openmv_firmware.ps1" -Configuration $Configuration -Target Diagnostic
exit $LASTEXITCODE
