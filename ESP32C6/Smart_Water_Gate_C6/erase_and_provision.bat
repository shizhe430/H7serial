@echo off
set MSYSTEM=
set MSYS=
set MINGW_PREFIX=
set MINGW_CHOST=
set "PATH=C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem"
set "PATH=%PATH%;C:\Espressif\tools\idf-git\2.44.0\cmd"
set "PATH=%PATH%;C:\Espressif\tools\cmake\3.24.0\bin"
set "PATH=%PATH%;C:\Espressif\tools\ninja\1.11.1"
set "PATH=%PATH%;C:\Espressif\tools\riscv32-esp-elf\esp-13.2.0_20240530\riscv32-esp-elf\bin"
set "PATH=%PATH%;C:\Espressif\tools\xtensa-esp-elf\esp-13.2.0_20240530\xtensa-esp-elf\bin"
set "PATH=%PATH%;C:\Espressif\tools\esp32ulp-elf\2.35_20220830\esp32ulp-elf\bin"
set "PATH=%PATH%;C:\Espressif\tools\idf-exe\1.0.3"
set "PATH=%PATH%;C:\Espressif\tools\ccache\4.8\ccache-4.8-windows-x86_64"
set "PATH=%PATH%;C:\Espressif\python_env\idf5.3_py3.11_env\Scripts"
set IDF_PATH=C:\Espressif\frameworks\esp-idf-v5.3.1
set IDF_TOOLS_PATH=C:\Espressif
cd /d C:\Users\17360\workspace\Smart_Water_Gate_C6

echo ========================================
echo Step 1: Erase all old NVS data (WiFi credentials, etc.)
echo ========================================
python.exe "%IDF_PATH%\tools\idf.py" -p COM3 erase-flash
if %ERRORLEVEL% NEQ 0 (
    echo Erase failed!
    pause
    exit /b 1
)
echo OK - Flash erased.

echo ========================================
echo Step 2: Flash new firmware
echo ========================================
python.exe "%IDF_PATH%\tools\idf.py" -p COM3 flash
if %ERRORLEVEL% NEQ 0 (
    echo Flash failed!
    pause
    exit /b 1
)
echo OK - Firmware flashed.

echo ========================================
echo Step 3: Monitor boot log for BLE mode
echo ========================================
echo Device will start BLE provisioning mode now.
echo Check the log for "QR code" or "PoP" info...
timeout /t 3
python.exe C:\Users\17360\workspace\Smart_Water_Gate_C6\read_log.py
pause
