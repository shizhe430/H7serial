param(
    [string]$Configuration = 'Release',
    [ValidateRange(-128, 127)]
    [int]$RoiOffsetX = 46,
    [ValidateRange(-128, 127)]
    [int]$RoiOffsetY = 15,
    [ValidateRange(0, 255)]
    [int]$ViewFill = 180
)

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$build = Join-Path $root $Configuration
$outName = 'water-roi-calib'
$artifact = 'H7serial_water_roi_calib'
$out = Join-Path $build $outName
$toolRoot = 'D:\ST\STM32CubeIDE_1.19.0\STM32CubeIDE\plugins'
$makeBin = Join-Path $toolRoot 'com.st.stm32cube.ide.mcu.externaltools.make.win32_2.2.100.202601091506\tools\bin'
$gccBin = Join-Path $toolRoot 'com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.13.3.rel1.win32_1.0.0.202411081344\tools\bin'
$env:Path = "$makeBin;$gccBin;$env:Path"

New-Item -ItemType Directory -Force -Path $out | Out-Null

$baseObjects = Get-Content (Join-Path $build 'objects.list') | ForEach-Object {
    $_.Trim().Trim('"').TrimEnd('\')
} | Where-Object {
    $_ -and ($_ -notmatch 'Core/Src/(camera_app|main)\.o')
}

& make -C $build -j2 @baseObjects
if ($LASTEXITCODE -ne 0) { throw 'Generated prerequisite build failed.' }

$common = @(
    '-mcpu=cortex-m7', '-std=gnu11', '-DUSE_PWR_LDO_SUPPLY', '-DUSE_HAL_DRIVER',
    '-DSTM32H743xx', '-DAPP_MODE=14U', "-DWATER_AI_ROI_OFFSET_X=$RoiOffsetX",
    "-DWATER_AI_ROI_OFFSET_Y=$RoiOffsetY", "-DWATER_AI_VIEW_FILL=$ViewFill", '-c',
    '-I../Core/Inc', '-I../Drivers/STM32H7xx_HAL_Driver/Inc',
    '-I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy',
    '-I../Drivers/CMSIS/Device/ST/STM32H7xx/Include', '-I../Drivers/CMSIS/Include',
    '-I../Middlewares/ST/AI/Inc', '-I../X-CUBE-AI/App', '-O3',
    '-ffunction-sections', '-fdata-sections', '-Wall', '--specs=nano.specs',
    '-mfpu=fpv5-d16', '-mfloat-abi=hard', '-mthumb'
)

Push-Location $build
try {
    & arm-none-eabi-gcc '../Core/Src/camera_app.c' @common '-o' "$outName/camera_app.o"
    if ($LASTEXITCODE -ne 0) { throw 'Water ROI camera_app compile failed.' }
    & arm-none-eabi-gcc '../Core/Src/main.c' @common '-o' "$outName/main.o"
    if ($LASTEXITCODE -ne 0) { throw 'Water ROI main compile failed.' }

    $objects = @($baseObjects)
    $objects += "./$outName/camera_app.o"
    $objects += "./$outName/main.o"
    $linker = Join-Path $root 'STM32H743IITX_FLASH.ld'
    & arm-none-eabi-gcc -o "$outName/$artifact.elf" @objects `
        '-l:NetworkRuntime1020_CM7_GCC.a' '-mcpu=cortex-m7' "-T$linker" '--specs=nosys.specs' `
        "-Wl,-Map=$outName/$artifact.map" '-Wl,--gc-sections' '-static' `
        '-L../Middlewares/ST/AI/Lib' '--specs=nano.specs' '-mfpu=fpv5-d16' `
        '-mfloat-abi=hard' '-mthumb' '-Wl,--start-group' '-lc' '-lm' '-Wl,--end-group'
    if ($LASTEXITCODE -ne 0) { throw 'Water ROI firmware link failed.' }

    & arm-none-eabi-objcopy -O ihex "$outName/$artifact.elf" "$outName/$artifact.hex"
    if ($LASTEXITCODE -ne 0) { throw 'Water ROI HEX generation failed.' }
    & arm-none-eabi-size "$outName/$artifact.elf"
} finally {
    Pop-Location
}

Write-Host "[WATER_ROI] offset=($RoiOffsetX,$RoiOffsetY) fill=$ViewFill"
Write-Host "[WATER_ROI] $out\$artifact.hex"
Write-Host "[VIEWER] powershell -File tools\ai_visual_viewer.ps1 -RoiOffsetX $RoiOffsetX -RoiOffsetY $RoiOffsetY"
