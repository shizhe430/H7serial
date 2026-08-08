import os
from PIL import Image

path = r'C:\Users\17360\Desktop\5686454283fe184a9d4f255855f36a12.jpg'
img = Image.open(path)
print(f"Size: {img.size}, Mode: {img.mode}")
img.show()
