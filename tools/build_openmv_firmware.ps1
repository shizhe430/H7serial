param(
    [string]$Configuration = 'Release',
    [ValidateSet('Diagnostic', 'Production')]
    [string]$Target = 'Diagnostic',
    [switch]$Sdram100MHz
)

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$build = Join-Path $root $Configuration
$appMode = if ($Target -eq 'Production') { '3U' } else { '13U' }
$artifact = if ($Target -eq 'Production') { 'H7serial_openmv_production' } else { 'H7serial_openmv_board_diag' }
$outName = if ($Target -eq 'Production') { 'openmv-production' } else { 'openmv-board' }
$clockSuffix = if ($Sdram100MHz) { '_sdram100' } else { '' }
$artifact = "$artifact$clockSuffix"
$out = Join-Path $build $outName
$toolRoot = 'D:\ST\STM32CubeIDE_1.19.0\STM32CubeIDE\plugins'
$makeBin = Join-Path $toolRoot 'com.st.stm32cube.ide.mcu.externaltools.make.win32_2.2.0.202409170845\tools\bin'
$gccBin = Join-Path $toolRoot 'com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.13.3.rel1.win32_1.0.0.202411081344\tools\bin'
$env:Path = "$makeBin;$gccBin;$env:Path"

New-Item -ItemType Directory -Force -Path $out | Out-Null

$baseObjects = Get-Content (Join-Path $build 'objects.list') | ForEach-Object {
    $_.Trim().Trim('"').TrimEnd('\')
} | Where-Object {
    $_ -and ($_ -notmatch 'Core/Src/(camera_app|fmc|main|openmv_face|openmv_frontalface_data|quadspi)\.o')
}

# Build generated prerequisites without invoking the generated final link,
# which does not know about the OpenMV sources after CubeMX regeneration.
& make -C $build -j2 @baseObjects
if ($LASTEXITCODE -ne 0) { throw 'Generated prerequisite build failed.' }

$common = @(
    '-mcpu=cortex-m7', '-std=gnu11', '-DUSE_PWR_LDO_SUPPLY', '-DUSE_HAL_DRIVER',
    '-DSTM32H743xx', "-DAPP_MODE=$appMode", '-c', '-I../Core/Inc',
    '-I../Drivers/STM32H7xx_HAL_Driver/Inc', '-I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy',
    '-I../Drivers/CMSIS/Device/ST/STM32H7xx/Include', '-I../Drivers/CMSIS/Include',
    '-I../Middlewares/ST/AI/Inc', '-I../X-CUBE-AI/App', '-O3', '-ffunction-sections',
    '-fdata-sections', '-Wall', '--specs=nano.specs', '-mfpu=fpv5-d16',
    '-mfloat-abi=hard', '-mthumb'
)
if ($Sdram100MHz) {
    $common += '-DSDRAM_50MHZ_DIAG=0U'
}

Push-Location $build
try {
    & arm-none-eabi-gcc '../Core/Src/camera_app.c' @common '-o' "$outName/camera_app.o"
    if ($LASTEXITCODE -ne 0) { throw "$Target camera_app compile failed." }
    & arm-none-eabi-gcc '../Core/Src/fmc.c' @common '-o' "$outName/fmc.o"
    if ($LASTEXITCODE -ne 0) { throw "$Target FMC compile failed." }
    & arm-none-eabi-gcc '../Core/Src/quadspi.c' @common '-o' "$outName/quadspi.o"
    if ($LASTEXITCODE -ne 0) { throw "$Target QSPI compile failed." }
    & arm-none-eabi-gcc '../Core/Src/main.c' @common '-o' "$outName/main.o"
    if ($LASTEXITCODE -ne 0) { throw "$Target main compile failed." }
    & arm-none-eabi-gcc '../Core/Src/openmv_face.c' @common '-o' "$outName/openmv_face.o"
    if ($LASTEXITCODE -ne 0) { throw "$Target OpenMV face compile failed." }
    & arm-none-eabi-gcc '../Core/Src/openmv_frontalface_data.c' @common '-o' "$outName/openmv_frontalface_data.o"
    if ($LASTEXITCODE -ne 0) { throw "$Target cascade compile failed." }

    $objects = @($baseObjects)
    $objects += "./$outName/camera_app.o"
    $objects += "./$outName/fmc.o"
    $objects += "./$outName/quadspi.o"
    $objects += "./$outName/main.o"
    $objects += "./$outName/openmv_face.o"
    $objects += "./$outName/openmv_frontalface_data.o"
    $linker = Join-Path $root 'STM32H743IITX_FLASH.ld'
    & arm-none-eabi-gcc -o "$outName/$artifact.elf" @objects `
        '-l:NetworkRuntime1020_CM7_GCC.a' '-mcpu=cortex-m7' "-T$linker" '--specs=nosys.specs' `
        "-Wl,-Map=$outName/$artifact.map" '-Wl,--gc-sections' '-static' `
        '-L../Middlewares/ST/AI/Lib' '--specs=nano.specs' '-mfpu=fpv5-d16' `
        '-mfloat-abi=hard' '-mthumb' '-Wl,--start-group' '-lc' '-lm' '-Wl,--end-group'
    if ($LASTEXITCODE -ne 0) { throw "$Target link failed." }
    & arm-none-eabi-objcopy -O ihex "$outName/$artifact.elf" "$outName/$artifact.hex"
    & arm-none-eabi-size "$outName/$artifact.elf"
} finally {
    Pop-Location
}

Write-Host "[$Target] $out\$artifact.hex"
