@echo off
start "" powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0face_ai_viewer.ps1" -Port COM28 -Baud 921600
