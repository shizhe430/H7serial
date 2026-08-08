@echo off
cd /d C:\Users\17360\workspace\Smart_Water_Gate_C6

set IDF_PATH=C:\Espressif\frameworks\esp-idf-v5.3.1
set IDF_TOOLS_PATH=C:\Espressif
set PYTHONNOUSERSITE=True
set MSYSTEM=
set PYTHONIOENCODING=utf-8
set "IDF_PYTHON_ENV_PATH=C:\Espressif\python_env\idf5.3_py3.11_env"

:: Use export.bat to set up all tool paths
call C:\Espressif\frameworks\esp-idf-v5.3.1\export.bat > nul 2>&1

if "%1"=="flash" goto flash
if "%1"=="monitor" goto monitor

:: Default: build
echo.
echo === ESP-IDF Build ===
echo.
%IDF_PYTHON_ENV_PATH%\Scripts\python.exe %IDF_PATH%\tools\idf.py build
set BUILD_RESULT=%ERRORLEVEL%
echo === Build Exit Code: %BUILD_RESULT% ===
exit /b %BUILD_RESULT%

:flash
echo.
echo === ESP-IDF Flash ===
echo.
%IDF_PYTHON_ENV_PATH%\Scripts\python.exe %IDF_PATH%\tools\idf.py -p COM3 flash
set FLASH_RESULT=%ERRORLEVEL%
echo === Flash Exit Code: %FLASH_RESULT% ===
exit /b %FLASH_RESULT%

:monitor
echo.
echo === ESP-IDF Monitor (Press Ctrl+] to quit) ===
echo.
%IDF_PYTHON_ENV_PATH%\Scripts\python.exe %IDF_PATH%\tools\idf_monitor.py -p COM3 -b 115200 --toolchain-prefix riscv32-esp-elf- --target esp32c6 C:\Users\17360\workspace\Smart_Water_Gate_C6\build\app-template.elf
exit /b %ERRORLEVEL%
