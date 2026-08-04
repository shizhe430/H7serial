param(
    [string]$Configuration = 'Release'
)

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$build = Join-Path $root $Configuration
$out = Join-Path $build 'openmv-board'
$toolRoot = 'D:\ST\STM32CubeIDE_1.19.0\STM32CubeIDE\plugins'
$makeBin = Join-Path $toolRoot 'com.st.stm32cube.ide.mcu.externaltools.make.win32_2.2.0.202409170845\tools\bin'
$gccBin = Join-Path $toolRoot 'com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.13.3.rel1.win32_1.0.0.202411081344\tools\bin'
$env:Path = "$makeBin;$gccBin;$env:Path"

New-Item -ItemType Directory -Force -Path $out | Out-Null
& make -C $build -j2 all
if ($LASTEXITCODE -ne 0) { throw 'Production build failed.' }

$common = @(
    '-mcpu=cortex-m7', '-std=gnu11', '-DUSE_PWR_LDO_SUPPLY', '-DUSE_HAL_DRIVER',
    '-DSTM32H743xx', '-DAPP_MODE=13U', '-c', '-I../Core/Inc',
    '-I../Drivers/STM32H7xx_HAL_Driver/Inc', '-I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy',
    '-I../Drivers/CMSIS/Device/ST/STM32H7xx/Include', '-I../Drivers/CMSIS/Include',
    '-I../Middlewares/ST/AI/Inc', '-I../X-CUBE-AI/App', '-O3', '-ffunction-sections',
    '-fdata-sections', '-Wall', '--specs=nano.specs', '-mfpu=fpv5-d16',
    '-mfloat-abi=hard', '-mthumb'
)

Push-Location $build
try {
    & arm-none-eabi-gcc '../Core/Src/camera_app.c' @common '-o' 'openmv-board/camera_app.o'
    if ($LASTEXITCODE -ne 0) { throw 'OPENMV_BOARD_DIAG camera_app compile failed.' }
    & arm-none-eabi-gcc '../Core/Src/main.c' @common '-o' 'openmv-board/main.o'
    if ($LASTEXITCODE -ne 0) { throw 'OPENMV_BOARD_DIAG main compile failed.' }
    & arm-none-eabi-gcc '../Core/Src/openmv_face.c' @common '-o' 'openmv-board/openmv_face.o'
    if ($LASTEXITCODE -ne 0) { throw 'OPENMV_BOARD_DIAG OpenMV face compile failed.' }
    & arm-none-eabi-gcc '../Core/Src/openmv_frontalface_data.c' @common '-o' 'openmv-board/openmv_frontalface_data.o'
    if ($LASTEXITCODE -ne 0) { throw 'OPENMV_BOARD_DIAG cascade compile failed.' }

    $objects = Get-Content 'objects.list' | ForEach-Object { $_.Trim().TrimEnd('\') } |
        Where-Object { $_ -and ($_ -notmatch 'Core/Src/(camera_app|main)\.o') }
    $objects += './openmv-board/camera_app.o'
    $objects += './openmv-board/main.o'
    $objects += './openmv-board/openmv_face.o'
    $objects += './openmv-board/openmv_frontalface_data.o'
    $linker = Join-Path $root 'STM32H743IITX_FLASH.ld'
    & arm-none-eabi-gcc -o 'openmv-board/H7serial_openmv_board_diag.elf' @objects `
        '-mcpu=cortex-m7' "-T$linker" '--specs=nosys.specs' `
        '-Wl,-Map=openmv-board/H7serial_openmv_board_diag.map' '-Wl,--gc-sections' '-static' `
        '-L../Middlewares/ST/AI/Lib' '--specs=nano.specs' '-mfpu=fpv5-d16' `
        '-mfloat-abi=hard' '-mthumb' '-Wl,--start-group' '-lc' '-lm' '-Wl,--end-group'
    if ($LASTEXITCODE -ne 0) { throw 'OPENMV_BOARD_DIAG link failed.' }
    & arm-none-eabi-objcopy -O ihex 'openmv-board/H7serial_openmv_board_diag.elf' 'openmv-board/H7serial_openmv_board_diag.hex'
    & arm-none-eabi-size 'openmv-board/H7serial_openmv_board_diag.elf'
} finally {
    Pop-Location
}

Write-Host "[OPENMV_BOARD_DIAG] $out\H7serial_openmv_board_diag.hex"
