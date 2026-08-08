@echo off
call C:\Espressif\frameworks\esp-idf-v5.3.1\export.bat >nul 2>&1
cd /d C:\Users\17360\workspace\Smart_Water_Gate_C6
idf.py build
if %ERRORLEVEL% NEQ 0 (
    echo BUILD FAILED
    exit /b 1
)
echo BUILD SUCCESS
