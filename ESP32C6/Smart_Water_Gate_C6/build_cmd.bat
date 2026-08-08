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
set "PATH=%PATH%;C:\Espressif\tools\idf-exe\1.0.3"
set "PATH=%PATH%;C:\Espressif\python_env\idf5.3_py3.11_env\Scripts"

set IDF_PATH=C:\Espressif\frameworks\esp-idf-v5.3.1
set IDF_TOOLS_PATH=C:\Espressif

cd /d C:\Users\17360\workspace\Smart_Water_Gate_C6

echo Building (software TLS - may take several minutes)...
python.exe "%IDF_PATH%\tools\idf.py" reconfigure
if %ERRORLEVEL% NEQ 0 (
    echo ===== RECONFIGURE FAILED =====
    pause
    exit /b 1
)

python.exe "%IDF_PATH%\tools\idf.py" build
if %ERRORLEVEL% NEQ 0 (
    echo ===== BUILD FAILED =====
    pause
    exit /b 1
)
echo ===== BUILD SUCCESS =====

echo Flashing...
python.exe -m esptool --chip esp32c6 -p COM3 -b 460800 --before default_reset --after hard_reset write_flash --flash_mode dio --flash_size 4MB --flash_freq 80m 0x0 build/bootloader/bootloader.bin 0x8000 build/partition_table/partition-table.bin 0xd000 build/ota_data_initial.bin 0x20000 build/app-template.bin
if %ERRORLEVEL% NEQ 0 (
    echo ===== FLASH FAILED =====
    pause
    exit /b 1
)
echo ===== FLASH SUCCESS =====
