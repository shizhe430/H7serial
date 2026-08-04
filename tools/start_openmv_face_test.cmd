@echo off
setlocal
cd /d "%~dp0.."
python tools\openmv_face_test.py --auto-open %*
