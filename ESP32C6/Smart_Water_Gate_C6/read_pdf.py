import io
import subprocess
# Try extracting text from the nanoESP32C6 PDF using available tools
pdf_path = r'C:\Users\17360\Desktop\ESP32C6使用\nanoESP32C6(1).pdf'

# First check if PyMuPDF is available
try:
    import fitz
    doc = fitz.open(pdf_path)
    for page in doc:
        print(page.get_text())
except ImportError:
    print("fitz not available, trying pdftotext...")
    try:
        result = subprocess.run(['pdftotext', '-layout', pdf_path, '-'],
                              capture_output=True, text=True, timeout=10)
        print(result.stdout[:3000])
    except:
        print("No PDF tools available")
