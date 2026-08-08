& $env:IDF_PATH = "C:\Espressif\frameworks\esp-idf-v5.3.1"
& C:\Espressif\frameworks\esp-idf-v5.3.1\export.ps1 | Out-Null
Set-Location C:\Users\17360\workspace\Smart_Water_Gate_C6
idf.py build
if ($LASTEXITCODE -ne 0) {
    Write-Host "BUILD FAILED"
    exit 1
}
Write-Host "BUILD SUCCESS"
