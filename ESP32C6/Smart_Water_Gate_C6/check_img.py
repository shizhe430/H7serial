import subprocess, sys, os
# Try to describe the image or convert it
img_path = r'C:\Users\17360\Desktop\5686454283fe184a9d4f255855f36a12.jpg'
print(f"Image: {img_path}")
print(f"Size: {os.path.getsize(img_path)} bytes")

# Check if any OCR or image tools available
for cmd in ['tesseract', 'python3', 'magick']:
    try:
        subprocess.run([cmd, '--version'], capture_output=True, timeout=2)
        print(f"{cmd} available")
    except:
        print(f"{cmd} not available")
