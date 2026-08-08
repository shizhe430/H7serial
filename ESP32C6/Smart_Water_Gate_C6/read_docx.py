import subprocess, sys
# Use system python
python = r'C:\Users\17360\AppData\Local\Programs\Python\Python313\python.exe'
subprocess.run([python, '-m', 'pip', 'install', 'python-docx', '-q'])
result = subprocess.run([python, '-c', '''
from docx import Document
doc = Document(r"C:\\Users\\17360\\Desktop\\基于ESP32P4的边云协同视觉AI智慧饮水系统.docx")
for i, para in enumerate(doc.paragraphs):
    if para.text.strip() and (para.style.name.startswith("Heading") or para.style.name.startswith("Normal")):
        print(f"{para.style.name}: {para.text}")
    if i > 500:
        break
'''], capture_output=True, text=True, timeout=15)
print(result.stdout[:10000])
if result.stderr:
    print("ERR:", result.stderr[:1000])
