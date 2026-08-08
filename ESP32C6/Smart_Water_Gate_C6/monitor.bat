@echo off
cd /d C:\Users\17360\workspace\Smart_Water_Gate_C6
set MSYSTEM=
set PYTHONIOENCODING=utf-8
C:\Espressif\python_env\idf5.3_py3.11_env\Scripts\python.exe C:\Espressif\frameworks\esp-idf-v5.3.1\tools\idf_monitor.py -p COM3 -b 115200 --toolchain-prefix riscv32-esp-elf- --target esp32c6 --revision 0 --decode-panic backtrace C:\Users\17360\workspace\Smart_Water_Gate_C6\build\app-template.elf
