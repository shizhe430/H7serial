@echo off
call C:\Espressif\frameworks\esp-idf-v5.3.1\export.bat
cd /d C:\Users\17360\workspace\Smart_Water_Gate_C6
echo ===== Reconfiguring =====
idf.py reconfigure
echo ===== Building =====
idf.py build
echo ===== Done =====
pause
