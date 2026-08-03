param(
    [string]$ProjectRoot = (Split-Path -Parent $PSScriptRoot)
)

$ErrorActionPreference = 'Stop'
$failures = [System.Collections.Generic.List[string]]::new()

function Assert-FileContains {
    param([string]$RelativePath, [string]$Pattern, [string]$Description)

    $path = Join-Path $ProjectRoot $RelativePath
    if (-not (Test-Path -LiteralPath $path)) {
        $failures.Add("missing file: $RelativePath")
        return
    }
    if ((Get-Content -LiteralPath $path -Raw) -notmatch $Pattern) {
        $failures.Add("${RelativePath}: $Description")
    }
}

function Assert-FileExcludes {
    param([string]$RelativePath, [string]$Pattern, [string]$Description)

    $path = Join-Path $ProjectRoot $RelativePath
    if (-not (Test-Path -LiteralPath $path)) {
        $failures.Add("missing file: $RelativePath")
        return
    }
    if ((Get-Content -LiteralPath $path -Raw) -match $Pattern) {
        $failures.Add("${RelativePath}: $Description")
    }
}

Assert-FileContains 'H7serial.ioc' 'ModelNameList=waterlevel,yunet,sface' 'CubeMX model list is not waterlevel/yunet/sface'
Assert-FileContains 'H7serial.ioc' 'ModelStructureFile-[^=]+=models/face/yunetn_320_qdq_int8\.onnx' 'YuNet does not use the project-local ONNX file'
Assert-FileContains 'H7serial.ioc' 'ModelStructureFile-[^=]+=models/face/face_recognition_sface_2021dec_int8\.onnx' 'SFace does not use the project-local ONNX file'
Assert-FileExcludes 'H7serial.ioc' '(facedet|faceid|buffalo_sc)' 'legacy InsightFace model metadata returned'

Assert-FileContains 'Core/Src/main.c' 'RCC_OscInitStruct\.PLL\.PLLM\s*=\s*5;' 'PLL1 M divider was reset'
Assert-FileContains 'Core/Src/main.c' 'RCC_OscInitStruct\.PLL\.PLLN\s*=\s*192;' 'PLL1 N multiplier was reset; SYSCLK must remain 480 MHz'
Assert-FileContains 'Core/Src/main.c' 'RCC_ClkInitStruct\.AHBCLKDivider\s*=\s*RCC_HCLK_DIV2;' 'HCLK divider was reset; HCLK must remain 240 MHz'
Assert-FileContains 'Core/Src/main.c' 'MPU_InitStruct\.BaseAddress\s*=\s*0xC0000000;' 'SDRAM MPU region is missing'
Assert-FileContains 'Core/Src/main.c' 'MPU_InitStruct\.Size\s*=\s*MPU_REGION_SIZE_32MB;' 'SDRAM MPU region size is not 32 MB'
Assert-FileContains 'Core/Src/main.c' 'MPU_InitStruct\.IsCacheable\s*=\s*MPU_ACCESS_CACHEABLE;' 'SDRAM is not configured as cacheable Normal memory'

Assert-FileContains 'Core/Src/fmc.c' 'PLL2\.PLL2M\s*=\s*5;' 'FMC PLL2 M divider was reset'
Assert-FileContains 'Core/Src/fmc.c' 'PLL2\.PLL2N\s*=\s*80;' 'FMC PLL2 N multiplier was reset'
Assert-FileContains 'Core/Src/fmc.c' 'PLL2\.PLL2R\s*=\s*2;' 'FMC PLL2 R divider was reset'
Assert-FileContains 'Core/Src/fmc.c' 'SDClockPeriod\s*=\s*FMC_SDRAM_CLOCK_PERIOD_2;' 'SDRAM divider was reset; SDCLK must remain 100 MHz'
Assert-FileContains 'Core/Src/quadspi.c' 'ClockPrescaler\s*=\s*1;' 'QSPI prescaler was reset; QSPI must remain 100 MHz'
Assert-FileContains 'Core/Src/quadspi.c' 'QspiClockSelection\s*=\s*RCC_QSPICLKSOURCE_PLL2;' 'QSPI clock source was reset'
Assert-FileContains 'Core/Src/usart.c' 'huart1\.Init\.BaudRate\s*=\s*921600;' 'USART1 diagnostic baud rate was reset'

foreach ($configuration in @('Debug', 'Release')) {
    $subdir = "$configuration/X-CUBE-AI/App/subdir.mk"
    $objects = "$configuration/objects.list"
    Assert-FileContains $subdir '\.\./X-CUBE-AI/App/yunet\.c' 'YuNet sources are missing from the build'
    Assert-FileContains $subdir '\.\./X-CUBE-AI/App/sface\.c' 'SFace sources are missing from the build'
    Assert-FileExcludes $subdir 'X-CUBE-AI/App/(facedet|faceid)\.c' 'legacy face model sources returned to the build'
    Assert-FileContains $objects 'X-CUBE-AI/App/yunet\.o' 'YuNet objects are missing from the link'
    Assert-FileContains $objects 'X-CUBE-AI/App/sface\.o' 'SFace objects are missing from the link'
}

if ($failures.Count -ne 0) {
    Write-Host '[CUBEMX_CHECK] FAILED' -ForegroundColor Red
    foreach ($failure in $failures) {
        Write-Host " - $failure" -ForegroundColor Red
    }
    exit 1
}

Write-Host '[CUBEMX_CHECK] OK: clocks, external memory, UART and YuNet/SFace integration are intact.' -ForegroundColor Green
